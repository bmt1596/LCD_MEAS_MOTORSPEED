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

// same values as array for indexed colors
// Variable zum Auslesen des Systemtaktes
uint32_t sysClock;

void init_and_paintbackground(void)
{
    // Init Port L for Display Control and Port M for Display Data
    init_ports_display();
    // Display initialization
    set_background_color(colorarray[0]);


    // drawline_H(0, 800, 200, colorarray[0], 2);

    drawline_V(0, 480, 550, colorarray[1], 2);

    // Header
    print_string1216("                    Fahzeug-Informations-Display                   ", 0, 0, colorarray[1], colorarray[5]);

    // vong tron mau vang
    drawCircle_px(275, 270, 225, colorarray[6], 2);

    // unter black line
    drawline_H(0, 550, 390, colorarray[0], 100);

    // ausser Kreis
    drawCircle_px(275, 270, 235, colorarray[1], 2);
    // mitteln Punk
    drawCircle_px(275, 270, 5, colorarray[1], 3);

    drawCircle_px(275, 270, 30, colorarray[6], 1);

    // unter black line
    drawline_H(0, 550, 470, colorarray[0], 10);
    drawline_H(154, 395, 470, colorarray[1], 2);

    // gach vang va so
    drawline_V(48, 65, 275, colorarray[3], 1);
    print_string1216("120", 75, 257, colorarray[6], colorarray[0]);

    drawline_H(53,70, 270, colorarray[3], 1);
    print_string1216("30", 265, 78 , colorarray[6], colorarray[0]);

    drawline_H(479,496, 270, colorarray[3], 1);
    print_string1216("210", 265, 437 , colorarray[6], colorarray[0]);

    // void write_line(short x1, short y1, short x2, short y2, int color)
    drawline(88, 390,105, 382, colorarray[3]);
    print_string1216("0", 368, 108, colorarray[6], colorarray[0]);


    //number on display
    print_string1216("Km/h", 380, 253, colorarray[1], colorarray[0]);

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
    j = 0;

    // Start endless loop
    /*
    while (1)
    {
        // Test Motor and Display
        printf("Write rectangles\n"); // for debug only
        for (x = 0; x <= (MAX_X) - 1; x = x + 30)
            for (y = 0; y <= (MAX_Y) - 1; y = y + 30)
            {
                // Copy Digital Input PP0 (S1), PP1 (S2) to Port PN0 and PN1
                GPIO_PORTN_DATA_R = GPIO_PORTP_DATA_R & 0x03;

                window_set(x, y, x + 14, y + 14);
                write_command(0x2C); //write pixel command
                color = colorarray[(j) % 7];
                j++; // change color

                for (i = 0; i < (15 * 15); i++) // set pixels
                {
                    write_data((color >> 16) & 0xff); // red
                    write_data((color >> 8) & 0xff); // green
                    write_data((color) & 0xff); // blue
                }
            }
    }
    */

    drawline(275, 270 ,120, 350, colorarray[3]);
    float f = 0;

    while (1)
       {
           // Test Motor and Display
           printf("Write rectangles\n"); // for debug only
           GPIO_PORTN_DATA_R = GPIO_PORTP_DATA_R & 0x03;
       }

}
