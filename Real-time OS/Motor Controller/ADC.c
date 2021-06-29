// Authors: Will Y, Erik R, Hunter W, Kalika S, Jake G
// File: ADC.c
// Desc: Initializes the ADC for use and also has a function to return a reading
// Assignment: CPEN 481 (Real time OS), Project 3

#include "TM4C123GH6PM.h"
#include "tm4c123gh6pm_def.h"

void ADC_Init(void){
	// Activate Port A B C E
	SYSCTL_RCGCGPIO_R |= 0x17;
	while((SYSCTL_RCGCGPIO_R&0x17) == 0){};
	//activate PB2,3,4,5
	GPIO_PORTB_DIR_R &= ~0x3C;
	GPIO_PORTB_DEN_R |= 0x3C;
	//activate PA6,7
	GPIO_PORTA_DIR_R &= ~0xC0;
	GPIO_PORTA_DEN_R |= 0xC0;
	//activate PE1(in),2,3
	GPIO_PORTE_DIR_R &= ~0x0E;
	GPIO_PORTE_DEN_R |= 0x0E;
	//activate PC7
	GPIO_PORTC_DIR_R |= 0x80;
	GPIO_PORTC_DEN_R |= 0x80;
}

int8_t readADC() {
	// max value to be returned was observed to be 0x7E at 10V
	int8_t sampleVal = 0;
	GPIO_PORTC_DATA_R |= 0x80;
	GPIO_PORTC_DATA_R &= ~0x80; //start conversion
	while(GPIO_PORTE_DATA_R & 0x02 == 0){} // wait for conversion to be done
	GPIO_PORTC_DATA_R |= 0x80; //star reading
	sampleVal |= (GPIO_PORTE_DATA_R & 0x0C) >> 2;
	sampleVal |= GPIO_PORTB_DATA_R & 0x3C;
	sampleVal |= GPIO_PORTA_DATA_R & 0xC0;
	return sampleVal;
}