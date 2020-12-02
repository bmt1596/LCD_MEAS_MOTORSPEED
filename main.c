#include <component/LCD/lcd_config.h>
#include <component/Sensor/sensor.h>
#include <stdio.h>
#include <math.h>

#define X_CENTER 275
#define Y_CENTER 270
#define PI 3.141596

void display_layout(void);
void display_project_information(void);
void display_number_and_line(void);
void wait(int time);
// some predefined basic colors to use with names
int colorarray[] = { 0x00000000,  //  BLACK 0
        0x00FFFFFF,     //  WHITE 1
        0x00AAAAAA,     //  GREY 2
        0x00FF0000,     //  RED 3
        0x0000FF00,     //  GREEN 4
        0x000000FF,     //  BLUE 5
        0x00FFFF00,     //  YELLOW 6
        0x0036454F,     //   Graun 7
        };

uint32_t sysClock;
uint32_t geschwindigkeit = 0;
/*
 * Funktion zum display die Geschwindigkeit und die rote Line

 */
void display_number_and_line(void)
{
    static uint16_t radius = 160;
    static uint16_t phinull = 150;
    static uint16_t phi;
    static uint16_t x, y;
    static uint16_t x_old = X_CENTER, y_old = Y_CENTER;

    char buffer[20];
    sprintf(buffer, "%3d km/h", geschwindigkeit);
    print_string1216(buffer, 400, 245, colorarray[0], colorarray[6]);

    phi = geschwindigkeit + phinull;
    x = X_CENTER + round(radius* cos((double)(phi)*2*PI/360));
    y = Y_CENTER + round(radius* sin((double)(phi)*2*PI/360));

    // remove a old line and set to background color
    if (x != x_old || y != y_old)
    {
        drawline(X_CENTER, Y_CENTER, x_old, y_old, colorarray[0]);
        drawCircle_px(x_old, y_old, 6, colorarray[0], 2);
    }

    // display new line
    drawline(X_CENTER, Y_CENTER , x, y, colorarray[3]);
    drawCircle_px(x, y, 6, colorarray[3], 2);

    // display center point
    drawCircle_px(X_CENTER, Y_CENTER, 6, colorarray[1], 4);
    drawCircle_px(X_CENTER, Y_CENTER, 30, colorarray[6], 2);
    print_string1216("Km/h", 135, 253, colorarray[1], colorarray[0]);

    x_old = x; y_old = y;
}

void display_project_information(void)
{
    set_background_color(colorarray[0]);
    drawline_H(551, 800, 170, colorarray[1], 2);
    drawline_V(0, 480, 550, colorarray[1], 2);
    // for unter line
    drawline_H(621, 738, 58, colorarray[1], 1);

    // print Header
    print_string1216(
            "                    Fahzeug-Informations-Display                   ",
            0, 0, colorarray[1], colorarray[5]);
    print_string1216("MC Projekt",
            40, 622, colorarray[1], colorarray[0]);
    print_string1216("Mitglieder:",
                80, 565, colorarray[1], colorarray[0]);
    print_string1216("1. Minh Tung Bui",
                    105, 565, colorarray[1], colorarray[0]);
    print_string1216("2. Hauke Kosmiter",
                        125, 565, colorarray[1], colorarray[0]);
}

void display_layout(void)
{
    // display information for MC project and team
    display_project_information();

    // display number 0 on circle
    drawline(84, 377, 101, 369, colorarray[3]);
    print_string1216("0", 358, 108, colorarray[6], colorarray[0]);

    // display number 30 on circle
    drawline_H(53, 72, 270, colorarray[3], 1);
    print_string1216("30", 265, 78, colorarray[6], colorarray[0]);

    // display number 60 on circle
    drawline(83, 158, 104, 166, colorarray[3]);
    print_string1216("60", 165, 112, colorarray[6], colorarray[0]);

    // display number 90 on circle
    drawline(163, 80, 171, 99, colorarray[3]);
    print_string1216("90", 106, 166, colorarray[6], colorarray[0]);

    // display number 120 on circle
    drawline_V(48, 69, 275, colorarray[3], 1);
    print_string1216("120", 75, 257, colorarray[6], colorarray[0]);

    // display number 150 on circle
    drawline(390, 80, 382, 99, colorarray[3]);
    print_string1216("150", 109, 360, colorarray[6], colorarray[0]);

    // display number 180 on circle
    drawline(465, 162, 448, 170, colorarray[3]);
    print_string1216("180", 176, 420, colorarray[6], colorarray[0]);

    // display number 210 on circle
    drawline_H(476, 496, 270, colorarray[3], 1);
    print_string1216("210", 265, 441, colorarray[6], colorarray[0]);

    // display number 240 on circle
    drawline(465, 376, 448, 368, colorarray[3]);
    print_string1216("240", 348, 420, colorarray[6], colorarray[0]);

    // display gold circle
    drawCircle_px(275, 270, 222, colorarray[6], 3);

    // unter black line
    drawline_H(0, 550, 390, colorarray[0], 100);

    // outside white circle
    drawCircle_px(275, 270, 235, colorarray[1], 4);

    // display center point
    drawCircle_px(275, 270, 6, colorarray[1], 4);

    // display gold insite circle
    drawCircle_px(275, 270, 30, colorarray[6], 2);

    // display under black line
    drawline_H(0, 550, 470, colorarray[0], 10);
    drawline_H(154, 395, 470, colorarray[1], 3);
}

void wait(int time)
{
    volatile int tmp;
    for (tmp = 0; tmp < 10800 * time; tmp++); // ~ 1ms
}


void main(void)
{
    int i = 0;
    // Set system frequency to 120 MHz
    sysClock = SysCtlClockFreqSet(SYSCTL_OSC_INT | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480, 120000000);

    // Init Port N for User LED and Port P for digital Input from Motor
    init_and_config_display();
    init_and_config_sensor();

    display_layout();

    // test funtion to display the number and red line
    while (1)
       {
        i++;
        geschwindigkeit = i;
        wait(50);
        display_number_and_line();
        if(i == 240)
        {
            i = 0;
        }
           // Test Motor and Display
           //printf("Write rectangles\n"); // for debug only
           //GPIO_PORTN_DATA_R = GPIO_PORTP_DATA_R & 0x03;
       }
}

