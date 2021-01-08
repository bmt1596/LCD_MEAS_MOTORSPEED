/*
 * @file        : sensor.c
 * @author      : Minh Tung Bui and Hauke Kosmiter
 * @copyright   : HAW-Hamburg
 * @addtogroup  : component/LCD
 * @{
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
}

