#include "TM4C123GH6PM.h"
#include "tm4c123gh6pm_def.h"
#include <stdio.h>
#include <stdlib.h>
#define TIMESLICE 100000 // 2ms thread switching
int32_t FIFO = 1; //FIFO Semaphore
int32_t count = 0;
int32_t LCD = 1;
int32_t X = 0; // current speed
int32_t Xstar = 0; // desired speed
int32_t voltage = 0;
int32_t E = 0; // speed, fixed-point
int32_t U = 0;
uint8_t Key_ASCII;
uint8_t key; 
uint16_t period = 2500; // for PWM

void Task0(void){ //MatrixKeypad_In
	while(1){
		Read_Key();
		key = Key_ASCII;
		if(Key_ASCII-0x30 <= 9 || Key_ASCII == 0x23)
		{
			if(count != 4) {
				OS_FIFO_Put(key);
				count++;
				OS_Sleep(20);
			}
		}
	}
}

void Task1(void){ 		// LCD
	uint8_t rpmcount = 0;
	uint8_t isNum = 0;
	char rpmbuff[4] = "";
	char target[4] = "0";
	int desiredSpeed = 0;
	while(1){
		uint32_t num;
		OS_Wait(&LCD);
		Set_Position(0x00);
		Display_Msg("Input RPM:");
		OS_Signal(&LCD);
		//Grabs from FIFO
		if(count != 0 && rpmcount != 4) {
			num = OS_FIFO_Get();
			isNum = 1;
			count--;
		}
		if(isNum || rpmcount == 4){
			sscanf(rpmbuff, "%d", &desiredSpeed);
			if(num == 0x23 || rpmcount == 4){ //Pound recieved (0x23 is #)
				if (desiredSpeed < 400 && desiredSpeed != 0) {
					rpmbuff[3] = 0;
					rpmbuff[2] = '0';
					rpmbuff[1] = '0';
					rpmbuff[0] = '4';
					rpmcount = 3;
				}
				else if(desiredSpeed > 2400){
					rpmbuff[3] = '0';
					rpmbuff[2] = '0';
					rpmbuff[1] = '4';
					rpmbuff[0] = '2';
					rpmcount = 4;
				}
				else if (desiredSpeed == 0){
					rpmbuff[1] = ' ';
					rpmbuff[2] = ' ';
					rpmbuff[3] = ' ';
				}
				sscanf(rpmbuff, "%d", &desiredSpeed);
				Xstar = desiredSpeed;
				
				U = (desiredSpeed*2400)/2500-1;
				//PWM1C_Duty(U);
				int j = 0;
				if(rpmcount == 3){
					target[3] = ' ';
				}
				for(; rpmcount != 0; rpmcount--){
					target[rpmcount-1] = rpmbuff[rpmcount-1];
					rpmbuff[rpmcount-1] = 0;
				}
				OS_Wait(&LCD);
				Set_Position(0x0C);
				Display_Msg("0   ");
				OS_Signal(&LCD);
			}
			else{ 
				sprintf(rpmbuff,"%s%c",rpmbuff,num); //<- This is the right thing to do
				//sprintf(rpmbuff,"%c",num);
				rpmcount++;
			}
			isNum = 0;
	 }
		OS_Wait(&LCD);
		Set_Position(0x0C);
		Display_Msg(rpmbuff);
		Set_Position(0x40);
		//   0 C:    0"
		Display_Msg("T: ");
		Display_Msg(target);
	  int i = 0;
	  for(i = 0; i <4; i++)
		{
			target[i] = 0x00;
		}
		OS_Signal(&LCD);
	}
}

void Task2(void){ // motor controller
	while(1){
		char currSpeed[4];
		voltage = (RecvMail()*10000)/126; // estimated speed (Get mail)
		// divide by 126, we observed this is the value at 10V, then conv to mV
		X = Current_speed(voltage);
		OS_Wait(&LCD);
		Set_Position(0x48);
	  Display_Msg("C:      ");
		if (X > 9999) // just in case, to prevent overflow of char array
			X = -1; // indicates error
		sprintf(currSpeed, "%ld", X); // convert X to char array to print
		Set_Position(0x4B);
		Display_Msg(currSpeed);
		OS_Signal(&LCD);
		E = Xstar-X; // error
		if(E < -10)
			U--; // decrease if too fast
		else if(E > 10)
			U++; // increase if too slow
			// leave as is if close enough
		if(U<450 && U != 0)
			U=450; // underflow (minimum PWM)
		if(U>2490)
			U=2490; // overflow (maximum PWM)
		PWM1C_Duty(U-1); // output to actuator
		OS_Sleep(10);
	}
	
}


int main () {
	uint32_t Reload = 16000000*.01 - 1; // for Timer0 FIX ME!!!!
	uint16_t duty =  0; // for PWM 0.1*(period) - 1
	//U = duty; // buzz at startup
	OS_Init();
	OS_FIFO_Init();
	Init_LCD_Ports();
	Init_LCD(); 
	ADC_Init();
	PortB_Init();
	GPIO_PORTB_DATA_R |= 0x01;
	PWM_Init(period, duty);
	Timer0_Config(Reload);
	Init_Keypad();
	OS_AddThreads(&Task0, &Task1, &Task2);
	OS_Launch(TIMESLICE);
	return 0;
}