/*
 * int_handler.c
 *
 *  Created on: 29.11.2020
 *      Author: Hauke
 */

#include <component/Sensor/int_handler.h>
#include <stdint.h>
#include <stdbool.h> // type bool for giop.h
#include "inc/hw_types.h"
#include "inc/tm4c1294ncpdt.h"
#include <stdio.h>   // Debug only
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>     // GPIO_PIN_X
#include <inc/hw_memmap.h>      // GPIO_PORTX_BASE

void IntPortPHandler(void){
    GPIO_PORTP_ICR_R |= 0x03;   // Clear interrupt register
    //GPIO_PORTP_MIS_R &= 0x00;     // Test
    //GPIO_PORTP_RIS_R &= 0x00;     // Test
    printf("I\n");
    direction = BACKWARD;
   // GPIO_PORTP_IM_R |= 0x03;      // Test
}
