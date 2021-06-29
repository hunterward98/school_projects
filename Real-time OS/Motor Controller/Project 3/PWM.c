#include "TM4C123GH6PM.h"
#include "tm4c123gh6pm_def.h"

void PWM1C_Duty(uint16_t duty)
{
	PWM1_3_CMPA_R = duty - 1;
}

void PWM_Init(uint16_t period, uint16_t duty)
{
	//Activate M1; bit 1
	SYSCTL_RCGCPWM_R |= 0x02;
	while((SYSCTL_PRPWM_R&0x02) == 0){};
		
	// Configure PF2 fpr M1PWM6 output
	SYSCTL_RCGCGPIO_R |= 0x20;
	while((SYSCTL_RCGCGPIO_R&0x20) == 0){};	
		
	GPIO_PORTF_AFSEL_R |= 0x04;
	GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFFF0FF)+0x00000500;
	GPIO_PORTF_AMSEL_R &= ~0x04;
	GPIO_PORTF_DEN_R |= 0x04;
	SYSCTL_RCC_R |= 0x1E0000; // check
	PWM1_3_CTL_R &= ~0xFFFFFFFF;
	PWM1_3_GENA_R = 0xC8;
	PWM1_3_LOAD_R = period - 1;
	PWM1_3_CMPA_R = duty - 1;
	PWM1_3_CTL_R |= 0x01;
	PWM1_ENABLE_R |= 0x40;
}

void Motor_Init() {
	SYSCTL_RCGCGPIO_R  |= 0x22; //Port B and Port F clock on
	GPIO_PORTB_DIR_R  |= 0x03; //PB0 and PB1 output
	GPIO_PORTF_DIR_R  |= 0x04; //PF2 is output
//	GPIO_PORTF_AFSEL_R |= 0x05; //PWM on
	//GPIO_PORTF_PCTL_R  |= 0x02; //Turn on PMC1 
	//GPIO_PORTF_AMSEL_R &= ~0x04; //AMSEL of for PF2
	GPIO_PORTF_DEN_R	 |= 0x04; //DEN PF2
	GPIO_PORTB_DEN_R   |= 0x03; //DEN PB0 and PB1
}