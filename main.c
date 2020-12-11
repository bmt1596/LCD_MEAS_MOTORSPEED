#include <component/LCD/lcd_config.h>
#include <component/LCD/lcd_paint.h>
#include <component/Sensor/int_handler.h>
#include <component/Sensor/sensor.h>
#include <stdio.h>
#include <math.h>

//void display_layout(void);
//void display_project_information(void);
void display_number_and_line(void);
void wait(int time);
//void edgeCountTimer(void);
void tt_handler(void);
void init_clock(void);

uint32_t sysClock;
uint32_t geschwindigkeit = 0;
uint32_t global_count = 0;

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
    print_string1216(buffer, 400, 245, COLOR_BLACK, COLOR_YELLO);

    phi = geschwindigkeit + phinull;
    x = X_CENTER + round(radius* cos((double)(phi)*2*PI/360));
    y = Y_CENTER + round(radius* sin((double)(phi)*2*PI/360));

    // remove a old line and set to background color
    if (x != x_old || y != y_old)
    {
        drawline(X_CENTER, Y_CENTER, x_old, y_old, COLOR_BLACK);
        drawCircle_px(x_old, y_old, 6, COLOR_BLACK, 2);
    }

    // display new line
    drawline(X_CENTER, Y_CENTER , x, y, COLOR_RED);
    drawCircle_px(x, y, 6, COLOR_RED, 2);

    // display center point
    drawCircle_px(X_CENTER, Y_CENTER, 6, COLOR_WHITE, 4);
    drawCircle_px(X_CENTER, Y_CENTER, 30, COLOR_YELLO, 2);
    print_string1216("Km/h", 135, 253, COLOR_WHITE, COLOR_BLACK);

    x_old = x; y_old = y;
}

void wait(int time)
{
    volatile int tmp;
    for (tmp = 0; tmp < 10800 * time; tmp++); // ~ 1ms
}


// Function to calculate the kilometers of the day
float clacKM(void){
    // Wheels = 20'' = 50,8cm --> circumference = 159.593
    float circumference = 159.593;
    return circumference * global_count;
}

// Function to calculate the actual speed
int calcSpeed(void){
    // The x value should be the max cycle you can have so that ....
    int x = 1;
    return x/240;
}
void main(void)
{
    int i = 0;
    // Set system frequency to 120 MHz
    // Ist jetzt in timer.c
    init_and_config_display();
    init_and_config_sensor();
    init_clock();
    tt_handler();

    // Test
    //timerLCD();
    // Test Ende

    // dis play complett number 0-240 and circle
    display_layout();

    // test funtion to display the number and red line
    for (i = 0; i <= 240; i++)
    {
        geschwindigkeit = i;
        wait(10);
        display_number_and_line();
    }

    for (i = 240; i >= 0; i--)
    {
        geschwindigkeit = i;
        wait(2);
        display_number_and_line();
    }


    while (1)
       {
        //Test Motor and Display
        //printf("Write rectangles\n"); // for debug only
        GPIO_PORTN_DATA_R = GPIO_PORTP_DATA_R & 0x03;

       }
}

