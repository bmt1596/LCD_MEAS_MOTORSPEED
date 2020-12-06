/*
 * timer.h
 *
 *  Created on: 03.12.2020
 *      Author: Hauke
 */

#include <stdint.h>
#include <stdbool.h> // type bool for giop.h
#include "inc/hw_types.h"
#include "inc/tm4c1294ncpdt.h"
#include <stdio.h>   // Debug only
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>     // GPIO_PIN_X
#include <inc/hw_memmap.h>      // GPIO_PORTX_BASE

#ifndef COMPONENT_TIMER_TIMER_H_
#define COMPONENT_TIMER_TIMER_H_

volatile unsigned char globalCount = 0;

void timerLCD(void);
void egdeCountTimer(void);

#endif /* COMPONENT_TIMER_TIMER_H_ */
