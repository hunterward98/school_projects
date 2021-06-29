// os_V2.c
// Runs on LM4F120/TM4C123
// A very simple real time operating system with additional features: OS_Suspend, OS_Sleep, Blocking Semaphores, FIFO, Priority
// John Tadrous
// July 10, 2020

//#define PE (*((volatile unsigned long *)0x40024030));
#define PB ((volatile unsigned long *)0x400050F0);
#define PA ((volatile unsigned long *)0x40004300);

/* Assume a 16 MHz clock frequency
 */

#include "TM4C123GH6PM.h"
#include "tm4c123gh6pm_def.h"

/******************** GLOBAL VARIABLES/DECLARATIONS ******************************/
uint32_t samples = 0;
uint32_t sampleSum = 0;
uint32_t Mail;		// mailbox support
int32_t Send = 0;    // mailbox semaphore
uint32_t Lost_mailbox;     // mailbox lost data
void SendMail(uint32_t);
// function definitions in osasm.s
void OS_DisableInterrupts(void); // Disable interrupts
void DisableInterrupts(void);
void OS_EnableInterrupts(void);  // Enable interrupts
void EnableInterrupts(void);
int32_t StartCritical(void);
void EndCritical(int32_t primask);
void Clock_Init(void);
void StartOS(void);
void Scheduler(void);
void OS_InitSemaphore(int32_t *Sem, int32_t val);

#define NUMTHREADS  3        // maximum number of threads
#define STACKSIZE   100      // number of 32-bit words in stack


// Timer Stuff *******************************************
// *******************************************************

void Timer0_Config(uint32_t Reload) {
	// disable interrupts globally
	DisableInterrupts();
	
	// activate timer
	SYSCTL_RCGCTIMER_R |= 0x01;
	
	// disable counting
	TIMER0_CTL_R &= ~0x01;
	
	TIMER0_CFG_R = 0x00000000;
	
	// Periodic Counting Mode
	TIMER0_TAMR_R |= 0x02;
	TIMER0_TAMR_R &= ~0x01;
	// write Reload val
	TIMER0_TAILR_R = Reload;
	
	// arm interrupt
	TIMER0_IMR_R |= 0x01;
	
	// enable interrupts from Timer0A
	NVIC_EN0_R |= 0x80000;
	
	// assign priority = 2
	NVIC_PRI4_R |= 0x20000000;
	
	// acknowledge pending Timer0A interrupts
	TIMER0_ICR_R |= 0x01;
	
	// enable timer
	TIMER0_CTL_R |= 0x01;
	
	// enable interrupts globally
	EnableInterrupts();
}

// Init stuff ***************************
// **************************************

// direction of motor
void PortB_Init()
{
	// Activate Port B
	SYSCTL_RCGCGPIO_R |= 0x02;
	while((SYSCTL_RCGCGPIO_R&0x02) == 0){};
	
	GPIO_PORTB_DIR_R |= 0x03;
	GPIO_PORTB_DEN_R |= 0x03;
}

void TIMER0A_Handler(void)
{
	uint32_t avgSamp = 0;
	int32_t ADCreading = readADC();
	
	sampleSum += ADCreading;
	samples++; // samples from ADC
	if (samples == 100) {
		avgSamp = sampleSum/samples;
		SendMail(avgSamp);
		sampleSum = 0;
		samples = 0;
	}
	TIMER0_ICR_R = 0x01;	
}

struct tcb{						// thread control block supports blocking, sleeping and priority
  int32_t *sp;       // pointer to stack (valid for threads not running
  struct tcb *next;  // linked-list pointer
	int32_t	*blocked;  // nonzero if blocked on this semaphore
	uint32_t Sleep; // nonzero if this thread is sleeping
	uint8_t  WorkingPriority; // used by the scheduler
	uint8_t FixedPriority; // permanent priority
	uint32_t Age; // time since last execution
};
typedef struct tcb tcbType;
tcbType tcbs[NUMTHREADS];
tcbType *RunPt;
int32_t Stacks[NUMTHREADS][STACKSIZE];


// ******** OS_Suspend ************
// suspends the current threads and triggers SysTick 
// input:  none
// output: none
void OS_Suspend(void){ 
	NVIC_INT_CTRL_R = 0x04000000; // trigger SysTick
}

// ******** OS_Wait ************
// wait function on a blocking semaphore 
// input:  semaphore pointer
// output: none
void OS_Wait(int32_t *s){
	DisableInterrupts();
	(*s) = (*s) - 1;
	if((*s) < 0){
		RunPt->blocked = s; // reason it is blocked
		EnableInterrupts();
		OS_Suspend();       // run thread switcher
	}
	EnableInterrupts();
}

// ******** OS_Signal ************
// signal function on a blocking semaphore 
// input:  semaphore pointer
// output: none
void OS_Signal(int32_t *s){
	tcbType *pt;
	DisableInterrupts();
	(*s) = (*s) + 1;
	if((*s) <= 0){
		pt = RunPt->next; // search for one blocked on this
		while(pt->blocked != s){
			pt = pt->next;
		}
		pt->blocked = 0;   // wakeup this one
	}
	EnableInterrupts();
}

// ******** OS_Sleep ************
// sleeps the current thread by setting its Sleep counter 
// input:  integer multiple of thread switching intervals
// output: none
void OS_Sleep(uint32_t SleepCtr){ 
	 RunPt->Sleep=SleepCtr;
	 OS_Suspend();
}

/*Secheduler*/
// Selects the next thread to run (unblocked, non-sleeping, modifies the sleep counter
// input: none
// output: none
void Scheduler(void){
	tcbType *pt;
	pt=RunPt;
	if (NVIC_ST_CTRL_R & 0x10000){  // full thread time has passed
		while (pt->next != RunPt){
			pt=pt->next;
			if (pt->Sleep){
				pt->Sleep=(pt->Sleep)-1;
			}
		}
	}
	RunPt = RunPt->next;     // skip at least one
	while((RunPt->Sleep)||(RunPt-> blocked)){
		RunPt = RunPt->next;   // find one not sleeping and not blocked
	}
}



void SendMail(uint32_t data){
  Mail=data;
	if(Send && Send != 0xFFFFFFFF){
		Lost_mailbox++;
	}
	else{
		OS_Signal(&Send);
	}
}

uint32_t RecvMail(void){
  OS_Wait(&Send);
	return Mail;
}

// OS_InitSmeaphore
// Initializes a semaphore
void OS_InitSemaphore(int32_t *Sem, int32_t val){
	*Sem=val;
}

// The FIFO Support - one semaphore FIFO
// This FIFO is dedicated to communicate data between
// one event thread and one main thread
#define FIFOSIZE 10 // can be any size
uint32_t PutI;      // index of where to put next
uint32_t GetI;      // index of where to get next
uint32_t Fifo[FIFOSIZE];
int32_t CurrentSize; // 0 means FIFO empty, FIFOSIZE means full
uint32_t LostData;  // number of lost pieces of data

void OS_FIFO_Init(void){
	PutI = GetI = 0;   // Empty
	OS_InitSemaphore(&CurrentSize, 0);
  LostData = 0;
} 

int OS_FIFO_Put(uint32_t data){
	if(CurrentSize == FIFOSIZE){
		LostData++; return -1;  // full
		}
	else{
		Fifo[PutI] = data;       // Put
		PutI = (PutI+1)%FIFOSIZE;
		OS_Signal(&CurrentSize);
		return 0;   // success
		} 
} 

uint32_t OS_FIFO_Get(void){
	uint32_t FIFO_data; 
	OS_Wait(&CurrentSize);    // block if empty
	FIFO_data = Fifo[GetI];        // get 
	GetI = (GetI+1)%FIFOSIZE; // place to get next return
	return FIFO_data;
}



// ******** OS_Init ************
// initialize operating system, disable interrupts until OS_Launch
// initialize OS controlled I/O: systick, 16 MHz clock
// input:  none
// output: none
void OS_Init(void){
  OS_DisableInterrupts();
  Clock_Init();                 // set processor clock to 16 MHz
  NVIC_ST_CTRL_R = 0;         // disable SysTick during setup
  NVIC_ST_CURRENT_R = 0;      // any write to current clears it
  NVIC_SYS_PRI3_R =(NVIC_SYS_PRI3_R&0x00FFFFFF)|0xE0000000; // priority 7
}

void SetInitialStack(int i){
  tcbs[i].sp = &Stacks[i][STACKSIZE-16]; // thread stack pointer
  Stacks[i][STACKSIZE-1] = 0x01000000;   // thumb bit
  Stacks[i][STACKSIZE-3] = 0x14141414;   // R14
  Stacks[i][STACKSIZE-4] = 0x12121212;   // R12
  Stacks[i][STACKSIZE-5] = 0x03030303;   // R3
  Stacks[i][STACKSIZE-6] = 0x02020202;   // R2
  Stacks[i][STACKSIZE-7] = 0x01010101;   // R1
  Stacks[i][STACKSIZE-8] = 0x00000000;   // R0
  Stacks[i][STACKSIZE-9] = 0x11111111;   // R11
  Stacks[i][STACKSIZE-10] = 0x10101010;  // R10
  Stacks[i][STACKSIZE-11] = 0x09090909;  // R9
  Stacks[i][STACKSIZE-12] = 0x08080808;  // R8
  Stacks[i][STACKSIZE-13] = 0x07070707;  // R7
  Stacks[i][STACKSIZE-14] = 0x06060606;  // R6
  Stacks[i][STACKSIZE-15] = 0x05050505;  // R5
  Stacks[i][STACKSIZE-16] = 0x04040404;  // R4
}




//******** OS_AddThread ***************
// add three foregound threads to the scheduler
// Inputs: three pointers to a void/void foreground tasks
// Outputs: 1 if successful, 0 if this thread can not be added

// Threads for this project:
// task0 = keypad
// task1 = LCD
// task2 = motor driving
// task3 = motor measuring - periodic, every 10 ms - takes 100 samples
int OS_AddThreads(void(*task0)(void),
                 void(*task1)(void),
                 void(*task2)(void))
                                { int32_t status;
  status = StartCritical();
  tcbs[0].next = &tcbs[1]; // 0 points to 1
  tcbs[1].next = &tcbs[2]; // 1 points to 2
  tcbs[2].next = &tcbs[0]; // 2 points to 0
  SetInitialStack(0); Stacks[0][STACKSIZE-2] = (int32_t)(task0); // PC
  SetInitialStack(1); Stacks[1][STACKSIZE-2] = (int32_t)(task1); // PC
  SetInitialStack(2); Stacks[2][STACKSIZE-2] = (int32_t)(task2); // PC
  RunPt = &tcbs[0];       // thread 0 will run first
  EndCritical(status);
  return 1;               // successful
}

///******** OS_Launch ***************
// start the scheduler, enable interrupts
// Inputs: number of 60ns clock cycles for each time slice
//         (maximum of 24 bits)
// Outputs: none (does not return)
void OS_Launch(uint32_t theTimeSlice){
  NVIC_ST_RELOAD_R = theTimeSlice - 1; // reload value
  NVIC_ST_CTRL_R = 0x00000007; // enable, core clock and interrupt arm
  StartOS();                   // start on the first task
}

void Clock_Init(void){
	SYSCTL_RCC_R|=0x810;
	SYSCTL_RCC_R&=~(0x400020);
}

