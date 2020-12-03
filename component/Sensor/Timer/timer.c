/*
 * timer.c
 *
 *  Created on: 03.12.2020
 *      Author: Hauke
 */

#include "timer.h"

void timerLCD(void){
    /*
     * The timer for updating the LCD Display
     * should be give a signal every 20 ms
     */

    // Configure Port D
    SYSCTL_RCGCGPIO_R = 0x08;
    while(!(SYSCTL_PRGPIO_R & 0x08));
    // PD0 is activated, if more Timer in D are used, maybe put the configuration together
    GPIO_PORTD_AHB_DEN_R |= 0x01;       //PD0 initalize
    GPIO_PORTD_AHB_DIR_R |= 0x01;       // PD0 is an output

    // Configure Timer 0A
    SYSCTL_RCGCTIMER_R |= (1<<0);       // Systemclock for Timer
    while(!(SYSCTL_PRTIMER_R & 0x01));  // Wait for activation
    TIMER0=_CTL_R &= ~0x01;             // Disable Timer
    TIMER0_CFG_R = 0x04;                // 16-Bit mode
    TIMER0_TAMR_R |= 0x02;              // No match enabled and periodic mode
    TIMER0_APR_R = 5 - 1;               // Prescaler = ceil((0.02s*16MHz)/2^16) -1 = 5 - 1
    TIMER0_TAILR_R = 64000 - 1;         // Load Value = ceil((0.2s*16MHz)/5) - = 64000 - 1
    TIMER0_CTL_R |= 0x01;               // Enable Timer
}
