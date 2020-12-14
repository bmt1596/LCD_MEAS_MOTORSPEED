/*
 * display_init.h
 *
 *  Created on: 13.12.2020
 *      Author: Hauke
 */

#include <component/LCD/lcd_config.h>
#include <component/LCD/lcd_paint.h>
#include "driverlib/timer.h"
//#include <component/Calculation/calculation.h>
//#include <component/Sensor/Timer/timer.h>
#include <stdio.h>
#include <math.h>

#ifndef COMPONENT_LCD_DISPLAY_INIT_H_
#define COMPONENT_LCD_DISPLAY_INIT_H_

// Prototypes
void display_number_and_line(void);
void display_test(void);

// Variable
uint32_t speed;


#endif /* COMPONENT_LCD_DISPLAY_INIT_H_ */
