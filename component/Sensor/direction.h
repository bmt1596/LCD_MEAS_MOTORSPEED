/*
 * @file        : direction.h
 * @author      : Minh Tung Bui and Hauke Kosmiter
 * @copyright   : HAW-Hamburg
 */

#ifndef COMPONENT_SENSOR_DIRECTION_H_
#define COMPONENT_SENSOR_DIRECTION_H_

#include <component/LCD/lcd_paint.h>
#include <component/LCD/lcd_config.h>
#include <stdbool.h>
#include <stdint.h>
#include "inc/tm4c1294ncpdt.h"  // Macros fpr Registers and Bitfields
#include "inc/hw_memmap.h"      // Tivaware Macros ...BASE as base address for peripheral modules

// Define variables
uint8_t move_direction;                     // Moving direction
enum direction{FORWARD, BACKWARD, STILL };  // Enum direction type
bool toggle;

// Define prototype
void determine_direction(uint8_t move_direction);


#endif /* COMPONENT_SENSOR_DIRECTION_H_ */
