/*
 * sensor.h
 *
 *  Created on: 15 Nov 2020
 *      Author: minht
 */

#ifndef COMPONENT_SENSOR_SENSOR_H_
#define COMPONENT_SENSOR_SENSOR_H_

#include <stdint.h>
#include <stdbool.h> // type bool for giop.h
#include "inc/hw_types.h"
#include "inc/tm4c1294ncpdt.h"
#include <stdio.h>   // Debug only
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>     // GPIO_PIN_X
#include <inc/hw_memmap.h>      // GPIO_PORTX_BASE

// Init Port N for User LED and Port P for digital Input from Motor
void init_and_config_sensor(void);


#endif /* COMPONENT_SENSOR_SENSOR_H_ */
