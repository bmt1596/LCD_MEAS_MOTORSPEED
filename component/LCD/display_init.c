/*
 * display_init.c
 *
 *  Created on: 13.12.2020
 *      Author: Hauke
 */

/*
 * Funktion zum display die Geschwindigkeit und die rote Line
 *
 */

#include "display_init.h"


void display_number_and_line(void)
{
    static uint16_t radius = 160;
    static uint16_t phinull = 150;
    static uint16_t phi;
    static uint16_t x, y;
    static uint16_t x_old = X_CENTER, y_old = Y_CENTER;

    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT); // Neu
    char buffer[20];
    sprintf(buffer, "%3d km/h", speed);
    print_string1216(buffer, 400, 245, COLOR_BLACK, COLOR_YELLO);

    phi = speed + phinull;
    x = X_CENTER + round(radius* cos((double)(phi)*2*PI/360));
    y = Y_CENTER + round(radius* sin((double)(phi)*2*PI/360));

    // Remove a old line and set to background color
    if (x != x_old || y != y_old)
    {
        drawline(X_CENTER, Y_CENTER, x_old, y_old, COLOR_BLACK);
        drawCircle_px(x_old, y_old, 6, COLOR_BLACK, 2);
    }

    // Display new line
    drawline(X_CENTER, Y_CENTER , x, y, COLOR_RED);
    drawCircle_px(x, y, 6, COLOR_RED, 2);

    // Display center point
    drawCircle_px(X_CENTER, Y_CENTER, 6, COLOR_WHITE, 4);
    drawCircle_px(X_CENTER, Y_CENTER, 30, COLOR_YELLO, 2);
    print_string1216("Km/h", 135, 253, COLOR_WHITE, COLOR_BLACK);

    x_old = x; y_old = y;
}

void display_test(void){
    // Test function to display the number and red line
    int i = 0;
        for (i = 0; i <= 240; i++)
        {
            speed = i;
            wait(10);
            display_number_and_line();
        }

        for (i = 240; i >= 0; i--)
        {
            speed = i;
            wait(2);
            display_number_and_line();
        }
}
