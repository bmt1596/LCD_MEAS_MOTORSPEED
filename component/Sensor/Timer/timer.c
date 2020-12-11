/*
 * timer.c
 *
 *  Created on: 03.12.2020
 *      Author: Hauke
 */

#include "timer.h"

// Prototypes
void tt_handler(void);
void init_clock(void);
/*
 * Function to initiate the sysclock and enable the tt_handler
 */
void init_clock(void){
    int sysClock;
    sysClock = SysCtlClockFreqSet(SYSCTL_OSC_INT | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480, 120000000);
    SysTickPeriodSet(sysClock);
    SysTickIntRegister(tt_handler);
    SysTickIntEnable();
    printf("%i\n", sysClock);
}
void tt_handler(void){

}

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
    TIMER0_CTL_R &= ~0x01;              // Disable Timer
    TIMER0_CFG_R = 0x04;                // 16-Bit mode
    TIMER0_TAMR_R |= 0x02;              // No match enabled and periodic mode
    TIMER0_TAPR_R = 5 - 1;               // Prescaler = ceil((0.02s*16MHz)/2^16) -1 = 5 - 1
    TIMER0_TAILR_R = 64000 - 1;         // Load Value = ceil((0.2s*16MHz)/5) - = 64000 - 1
    TIMER0_CTL_R |= 0x01;               // Enable Timer


    // Maybe more efficent to take it in an other function as the configuration
    while(1){
        while((TIMER0_RIS_R & (1<<8)) == 0);    // Wait for time out flag
        // Display interrupt
        //globalCount++; // TEST
        TIMER0_ICR_R |= (1<<8);                  // Clear time out flag
    }
}


void edgeCountTimer(void){
    /*
     * The timer is counting the edge that are happens.
     * It's used to calculate the driven day kilometers
     */

    printf("2\n2\n2\n");
    // Configure Port D
    SYSCTL_RCGCGPIO_R = 0x08;
    while(!(SYSCTL_PRGPIO_R & 0x08));
    // PD1 is activated
    GPIO_PORTD_AHB_DEN_R |= 0x02;         // Enable PD1
    GPIO_PORTD_AHB_DIR_R |= 0x02;         // Set PD1 to output
    GPIO_PORTD_AHB_DATA_R &= ~(1<<1);   // Clear PD1
    GPIO_PORTD_AHB_AFSEL_R |= (1<<1);   // Set PD1 to alternate function
    //GPIO_PORTD_AHB_PCTL_R

    // Configure Timer 0B
    SYSCTL_RCGCTIMER_R |= (1<<0);       // Systemclock for Timer
    while(!(SYSCTL_PRTIMER_R & 0x01));  // Wait for activation
    TIMER0_CTL_R &= ~0x02;              // Disable Timer
    TIMER0_CFG_R = 0x04;                // 16-Bit mode
    TIMER0_TBMR_R = 0x03;               // Capture mode
    TIMER0_TBMR_R &= (0<<2);            // Edge-Count mode
    TIMER0_CTL_R |= 0xC00;              // Both edges event type for Timer B
    TIMER0_TBILR_R = 1 - 1;             // Test Load Value of 0, so when one edge is detected global var count up
    //TIMER0_BPR_R = 1 - 1;
    TIMER0_TBMATCHR_R = 2 - 1;           // Set Match Value to 1 so the difference between Load Value and Match value is 1
    TIMER0_CTL_R |= 0x02;               // Enable Timer
    //printf("5");
    /*
     * While loop for counting the global
     * variable up and reset Timer
     */
    /*while(1){
        while((TIMER0_RIS_R & (1<<11)) == 0);   // Check if there is an edge-count
        //globalCount++;                          // Count global variable up
        TIMER0_RIS_R = (1<<11);                 // Clear match flag
        TIMER0_TBILR_R = 1 - 1;                 // Reset load value
        }*/
    printf("3\n3\n3\n");
    }


