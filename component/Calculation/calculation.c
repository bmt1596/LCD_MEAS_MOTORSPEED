/*
 * calculation.c
 *
 *  Created on: 12.12.2020
 *      Author: Hauke
 */

/*
 * File for the calculation functions:
 * - Function for calculation current speed
 * - Function for calculation kilometers of the day
 */

#include "calculation.h"

// Global variables


// Function to calculate the kilometers of the day
float clacKM(void){
    // Wheels = 20'' = 50,8cm --> circumference = 159.593
    float circumference = 159.593;
    return circumference * global_count;
}

// Function to calculate the actual speed
int calcSpeed(void){
    // The x value should be the max cycle you can have so that ....
    int max_rpm = 25000;
    int max_rps = max_rpm/60;
    return max_rps/240;
}

