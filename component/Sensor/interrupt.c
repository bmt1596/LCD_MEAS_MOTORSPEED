/*
 * @file        : interrupt.c
 * @author      : Minh Tung Bui and Hauke Kosmiter
 * @copyright   : HAW-Hamburg
 *
 * In this file are the two interrupt handlers for Port P0 and P1
 */
#include "interrupt.h"

// Function for P0
void S1Handler(void)
{
    GPIO_PORTP_ICR_R |= 0x01;   // Clear interrupt p0
    S1counter++;                // Count of runs of this handler
    GPIO_PORTP_ICR_R |= 0x01;   // Clear interrupt p0
}

// Function for P1
void S2Handler(void)
{
    GPIO_PORTP_ICR_R |= 0x02;   // Clear interrupt p1
    S2counter++;                // Count the runs of this handler

    // Check which direction
    if (GPIO_PORTP_DATA_R & 0x02 == 0x02)
    {
        move_direction = BACKWARD;
    }
    else
    {
        move_direction = FORWARD;
    }
}

