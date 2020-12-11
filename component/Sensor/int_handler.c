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
    GPIO_PORTP_IM_R &= 0x00;
    if(GPIO_PORTP_DATA_R == 0x03){
        //printf("DATA: %#02x\n", GPIO_PORTP_DATA_R);
        direction = FORWARD;
    }
    else if(GPIO_PORTP_DATA_R == 0x02){
        //printf("DATA: %#02x\n", GPIO_PORTP_DATA_R);
        direction = BACKWARD;
    }


    /* Wont work, to much interrupts
    if(GPIO_PORTP_DATA_R == 0x01){          // Check P1 = 0
        direction = FORWARD;
    }
    else if(GPIO_PORTP_DATA_R == 0x03){     // Check P1 = 1
        direction = BACKWARD;
    }
    */
    // Both edges
    // Not working
    /*if(GPIO_PORTP_IEV_R == 0x00){           // Falling edge detected
        if(GPIO_PORTP_DATA_R == 0x00){      // If a falling edge on P0 and P1 is 0 then backward
            direction = BACKWARD;
        }
        else if(GPIO_PORTP_DATA_R == 0x02){ // If a falling edge on P0 and P1 is 1 then forward
            direction = FORWARD;
        }
    }
    else if(GPIO_PORTP_IEV_R == 0x01){      // Rising edge detected
        if(GPIO_PORTP_DATA_R == 0x01){
            direction = FORWARD;
        }
        else if(GPIO_PORTP_DATA_R == 0x03){
            direction = BACKWARD;
        }
    }*/
    //GPIO_PORTP_MIS_R &= 0x00;     // Test
    //GPIO_PORTP_RIS_R &= 0x00;     // Test
    //printf("IEV: %x\n", GPIO_PORTP_IEV_R);
    printf("%i\n", direction);
    printf("%#x\n", GPIO_PORTP_ICR_R);
    GPIO_PORTP_ICR_R = 0x02;               // Clear interrupt register (Sorgt dafür, dass keine neue Flnake erkannt wird, wenn der Motor aus ist, aber cleared nicht)
    printf("%#x\n", GPIO_PORTP_ICR_R);
    GPIO_PORTP_IM_R |= 0x02;
   // GPIO_PORTP_IM_R |= 0x03;      // Test
}
