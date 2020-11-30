#include <component/LCD/lcd.h>
#include <component/Sensor/sensor.h>
#include <stdio.h>

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

void init_and_paintbackground(void);
void displayzahl_and_kreis(void);
void display_number(void);



// same values as array for indexed colors
// Variable zum Auslesen des Systemtaktes
uint32_t sysClock;

void display_information(void);


void init_and_paintbackground(void)
{
    // Init Port L for Display Control and Port M for Display Data
    init_ports_display();
    // Display initialization
    displayzahl_and_kreis();

    //number on display

    display_number();
}

void display_information(void)
{
    set_background_color(colorarray[0]);
    drawline_H(551, 800, 170, colorarray[1], 2);
    drawline_V(0, 480, 550, colorarray[1], 2);
    // for unter line
    drawline_H(621, 738, 58, colorarray[1], 1);

    // Header
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

void displayzahl_and_kreis(void)
{


    display_information();
    // gach do va so
    // void write_line(short x1, short y1, short x2, short y2, int color)
    drawline(84, 377, 101, 369, colorarray[3]);
    print_string1216("0", 358, 108, colorarray[6], colorarray[0]);

    drawline_H(53, 72, 270, colorarray[3], 1);
    print_string1216("30", 265, 78, colorarray[6], colorarray[0]);

    drawline(83, 158, 104, 166, colorarray[3]);
    print_string1216("60", 165, 112, colorarray[6], colorarray[0]);

    drawline(163, 80, 171, 99, colorarray[3]);
    print_string1216("90", 106, 166, colorarray[6], colorarray[0]);

    drawline_V(48, 69, 275, colorarray[3], 1);
    print_string1216("120", 75, 257, colorarray[6], colorarray[0]);

    drawline(390, 80, 382, 99, colorarray[3]);
    print_string1216("150", 109, 360, colorarray[6], colorarray[0]);

    drawline(465, 162, 448, 170, colorarray[3]);
    print_string1216("180", 176, 420, colorarray[6], colorarray[0]);

    drawline_H(476, 496, 270, colorarray[3], 1);
    print_string1216("210", 265, 437, colorarray[6], colorarray[0]);

    drawline(465, 376, 448, 368, colorarray[3]);
    print_string1216("240", 348, 420, colorarray[6], colorarray[0]);

    print_string1216("Km/h", 135, 253, colorarray[1], colorarray[0]);

    // vong tron mau vang
    drawCircle_px(275, 270, 222, colorarray[6], 3);

    // unter black line
    drawline_H(0, 550, 390, colorarray[0], 100);

    // ausser Kreis
    drawCircle_px(275, 270, 235, colorarray[1], 4);

    // mitteln Punk
    drawCircle_px(275, 270, 6, colorarray[1], 4);

    // vong tron mau vang ben trong
    drawCircle_px(275, 270, 30, colorarray[6], 2);

    // unter black line
    drawline_H(0, 550, 470, colorarray[0], 10);
    drawline_H(154, 395, 470, colorarray[1], 3);
}

void display_number(void)
{
    print_string1216("000,00", 400, 245, colorarray[0], colorarray[6]);
}

void wait(int time){
    volatile int tmp;

    for(tmp = 0; tmp < 10800*time; tmp++); // ~ 1ms
}

void main(void)
{
    int i, j, x, y;

    // Set system frequency to 120 MHz
    sysClock = SysCtlClockFreqSet(SYSCTL_OSC_INT | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480, 120000000);

    configPorts(); // Init Port N for User LED and Port P for digital Input from Motor
    init_and_paintbackground();

    drawline(275, 270 ,120, 350, colorarray[3]);

    // Test Area //

    // --------- //

    while (1)
       {
           // Test Motor and Display
           //printf("Write rectangles\n"); // for debug only
           //GPIO_PORTN_DATA_R = GPIO_PORTP_DATA_R & 0x03;
        /*if (i % 1 == 0)
            {
            configPorts();
            }
           i++;*/
       }

}

