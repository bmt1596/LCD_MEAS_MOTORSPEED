/*
 * timer.c
 *
 *  Created on: 09.01.2021
 *      Author: Hauke
 */

#include "timer.h"

void Timer1_DisplayIntHandler(void)
{
    static uint16_t radius = 160;
    static uint16_t phinull = 150;
    static uint16_t phi;
    static uint16_t x, y;
    static uint16_t x_old = X_CENTER, y_old = Y_CENTER;
    char buffer1[20], buffer2[20], buffer3[20];

    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

    veclocity_tacho = (double) (S2counter * 1.151);

    if(veclocity_tacho > 180)
    {
        if (toggle == true)
        {
            print_string1216("TOO FAST!!", 350, 220, COLOR_RED, COLOR_BLACK);
        }else
        {
            print_string1216("          ", 350, 220, COLOR_BLACK, COLOR_BLACK);
        }

    }
    else if (veclocity_tacho >= 240)
    {
        veclocity_tacho = 240;
    }
    else
    {
        print_string1216("          ", 350, 220, COLOR_BLACK, COLOR_BLACK);
    }

    distance_in_m = distance_in_m + (double) (veclocity_tacho / 9);
    distance_in_km = (double) (distance_in_m / 1000);

    if (veclocity_tacho < 11)
    {
        sprintf(buffer1, " %3.2lf   ", veclocity_tacho);
    }
    else if (veclocity_tacho < 101)
    {
        sprintf(buffer1, " %3.2lf   ", veclocity_tacho);
    }
    else
    {
        sprintf(buffer1, "%3.2lf   ", veclocity_tacho);
    }

    print_string1216(buffer1, 410, 238, COLOR_WHITE, COLOR_BLACK);

    sprintf(buffer2, "%3.2lf", distance_in_m);
    print_string1216(buffer2, 270, 700, COLOR_BLACK, COLOR_YELLO);

    sprintf(buffer3, "%3.2lf", distance_in_km);
    print_string1216(buffer3, 295, 700, COLOR_BLACK, COLOR_YELLO);

    phi = veclocity_tacho + phinull;
    x = X_CENTER + round(radius * cos((double) (phi) * 2 * PI / 360));
    y = Y_CENTER + round(radius * sin((double) (phi) * 2 * PI / 360));

    // remove a old line and set to background color
    if (x != x_old || y != y_old)
    {
        drawline(X_CENTER, Y_CENTER, x_old, y_old, COLOR_BLACK);
        drawCircle_px(x_old, y_old, 6, COLOR_BLACK, 2);
    }

    // display new line
    drawline(X_CENTER, Y_CENTER, x, y, COLOR_RED);
    drawCircle_px(x, y, 6, COLOR_RED, 2);

    // display center point
    drawCircle_px(X_CENTER, Y_CENTER, 6, COLOR_WHITE, 4);
    drawCircle_px(X_CENTER, Y_CENTER, 30, COLOR_YELLO, 2);
    print_string1216("Km/h", 135, 253, COLOR_WHITE, COLOR_BLACK);

    x_old = x;
    y_old = y;

    determine_direction(move_direction);
    S2counter = 0;
    move_direction = STILL;

    toggle = !toggle;

}

