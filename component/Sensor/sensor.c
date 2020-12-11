/*
 * sensor.c
 *
 *  Created on: 15 Nov 2020
 *      Author: minht and Hauke Kosmiter
 */

#include "sensor.h"

void init_and_config_sensor(void)
{
    // Enable ports
    SYSCTL_RCGCGPIO_R |= 0x3000; // Port P
    // Wait for the cores to power up
    while ((SYSCTL_PRGPIO_R & (0x3000)) == 0)
        ;
    GPIO_PORTN_DEN_R |= 0x03;    // Digital Enable for PN0 and PN1              // Wieso?
    GPIO_PORTN_DIR_R |= 0x03;    // Define port as Output for PIN PN0 and PN1   // Wieso?
    GPIO_PORTP_DEN_R |= 0x03;    // Digital Enable for PP0 and PP1
    GPIO_PORTP_DIR_R &= 0xFC;    // Define port as Input for PIN PP0 and PP1

    // Edge events are recommended to be ADC triggered
    //GPIO_PORTP_ADCCTL             //Enable ADC trigger?
    GPIO_PORTP_IS_R |= 0xFF;        // Debug
    GPIO_PORTP_IS_R &= ~0x02;       // Clear P1 for activating edge sensitive
    GPIO_PORTP_IBE_R |= 0xFF;       // Debug
    GPIO_PORTP_IBE_R &= ~0x02;      // Set P1 to trigger on single edge
    GPIO_PORTP_IEV_R |= 0x00;       // Clear Register IEV
    GPIO_PORTP_IEV_R |= 0x02;       // Set P1 to rising edge detecting
    GPIO_PORTP_ICR_R |= 0x02;       // Clear the interrupt to avoid that the interrupt handler is executed again without a trigger
    GPIO_PORTP_IM_R |= 0x02;        // Unmask P1

    NVIC_EN2_R = (1<<13);            // Enable Port P1 interrupt
    printf("%#X\n", GPIO_PORTP_IEV_R);

}


ISR(INT1_vec){

}


