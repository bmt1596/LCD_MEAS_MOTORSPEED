/*
 * timer.h
 *
 *  Created on: 09.01.2021
 *      Author: Hauke
 */

#include <stdint.h>
#include <math.h>
//#include <component/LCD/lcd_paint.h>
//#include <component/LCD/lcd_config.h>
#include <component/Sensor/interrupt.h>
#include "driverlib/timer.h"
#include "driverlib/gpio.h"      //  Tivaware functions: GPIO... + Macros  GPIO_...
#include "inc/tm4c1294ncpdt.h" // Macros fpr Registers and Bitfields
#include "inc/hw_memmap.h" //  Tivaware Macros ...BASE as base address for peripheral modules

#ifndef COMPONENT_SENSOR_TIMER_H_
#define COMPONENT_SENSOR_TIMER_H_

typedef float float32_t;

float32_t veclocity_tacho;
float32_t distance_in_m;
float32_t distance_in_km;

void Timer1_DisplayIntHandler(void);

#endif /* COMPONENT_SENSOR_TIMER_H_ */
