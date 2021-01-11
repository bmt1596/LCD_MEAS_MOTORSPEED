/*
 * @file        : init.h
 * @author      : Minh Tung Bui and Hauke Kosmiter
 * @copyright   : HAW-Hamburg
 */

#include <stdint.h>
#include <stdio.h>
#include <component/Sensor/timer.h>

#include "driverlib/systick.h"      //  Tivaware functions: SysTick...
#include "driverlib/interrupt.h"    //  Tivaware functions: Int...
#include "driverlib/sysctl.h"       //  Tivaware functions: SysCtl...  + Macros  SYSCTL_...
#include "driverlib/gpio.h"         //  Tivaware functions: GPIO... + Macros  GPIO_...
#include "driverlib/timer.h"
#include "inc/tm4c1294ncpdt.h"      // Macros fpr Registers and Bitfields
#include "inc/hw_memmap.h"          //  Tivaware Macros ...BASE as base address for peripheral modules

#ifndef COMPONENT_INITIALIZATION_INIT_H_
#define COMPONENT_INITIALIZATION_INIT_H_

// Define variable
uint32_t sysCLK;

// Define prototypes
void init_peripherals(void);
void init_var(void);


#endif /* COMPONENT_INITIALIZATION_INIT_H_ */
