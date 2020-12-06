/*
 * int_handler.h
 *
 *  Created on: 29.11.2020
 *      Author: Hauke
 */

#ifndef COMPONENT_SENSOR_INT_HANDLER_H_
#define COMPONENT_SENSOR_INT_HANDLER_H_

// Global varibales for drving direction
#define FORWARD 0
#define BACKWARD 1

void IntPortPHandler(void);
extern volatile unsigned char direction = FORWARD;

#endif /* COMPONENT_SENSOR_INT_HANDLER_H_ */
