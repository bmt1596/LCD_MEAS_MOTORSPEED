/*
 * @file        : interrupt.c
 * @author      : Minh Tung Bui and Hauke Kosmiter
 * @copyright   : HAW-Hamburg
 *
 * In this file are the two interrupt handlers for Port P0 and P1
 */

#include <stdbool.h>
#include <stdint.h>
#include <component/Sensor/direction.h>
#include "driverlib/gpio.h"             // Tivaware functions: GPIO... + Macros  GPIO_...
#include "inc/tm4c1294ncpdt.h"          // Macros fpr Registers and Bitfields
#include "inc/hw_memmap.h"              // Tivaware Macros ...BASE as base address for peripheral modules

#ifndef COMPONENT_SENSOR_INTERRUPT_H_
#define COMPONENT_SENSOR_INTERRUPT_H_

// Define variables
uint8_t move_direction;
uint32_t S1counter;
uint32_t S2counter;

// Define prototype
void S1Handler(void);
void S2Handler(void);

#endif /* COMPONENT_SENSOR_INTERRUPT_H_ */
