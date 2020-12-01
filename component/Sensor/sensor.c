/*
 * sensor.c
 *
 *  Created on: 15 Nov 2020
 *      Author: minht and Hauke Kosmiter
 */

#include "sensor.h"

void configPorts(void)
{
    // Enable ports
    SYSCTL_RCGCGPIO_R |= 0x3000; // Port P
    // Wait for the cores to power up
    while ((SYSCTL_PRGPIO_R & (0x3000)) == 0)
        ;
    GPIO_PORTN_DEN_R |= 0x03;    // Digital Enable for PN0 and PN1              // Wieso?
    GPIO_PORTN_DIR_R |= 0x03;    // Define port as Output for PIN PN0 and PN1   // Wieso?
    GPIO_PORTP_DEN_R |= 0x07;    // Digital Enable for PP0 and PP1
    GPIO_PORTP_DIR_R &= 0xF8;    // Define port as Input for PIN PP0 and PP1

    // Edge events are recommended to be ADC triggered
    //GPIO_PORTP_ADCCTL           //Enable ADC trigger?
    GPIO_PORTP_IS_R &= 0XFC;    // Clear P0 an P1 for activating edge sensitive
    GPIO_PORTP_IBE_R |= 0x03;   // Set P0 and P1 to set the trigger on both edges
    GPIO_PORTP_IEV_R &= 0x00;   // Clear Register IEV
    // Test rising edge, not final
    GPIO_PORTP_IEV_R |= 0x03;   // Set P0 and P1 to rising edge detecting
    // Test end
    GPIO_PORTP_ICR_R |= 0x03;   // Clear the interrupt to avoid that the interrupt handler is executed again without a trigger
    GPIO_PORTP_IM_R |= 0x03;    // Unmask P0  and P1

    NVIC_EN2_R = 0xFFFF;         // Enable PortP interrupt // To update
}


ISR(INT1_vec){

}


