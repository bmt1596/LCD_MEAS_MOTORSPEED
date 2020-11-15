#include <stdint.h>
#include <stdbool.h> // type bool for giop.h
#include "inc/hw_types.h"
#include "inc/tm4c1294ncpdt.h"
#include <stdio.h>   // Debug only
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>     // GPIO_PIN_X
#include <inc/hw_memmap.h>      // GPIO_PORTX_BASE

// import LCD Header form Folder
#include "component/LCD/lcd.h"
#include "component/Sensor/sensor.h"

// some predefined basic colors to use with names
enum colors
{
    BLACK = 0x00000000,
    WHITE = 0x00FFFFFF,
    GREY = 0x00AAAAAA,
    RED = 0x00FF0000,
    GREEN = 0x0000FF00,
    BLUE = 0x000000FF,
    YELLOW = 0x00FFFF00,
};

// same values as array for indexed colors
int colorarray[] = { 0x00000000,  //  BLACK
        0x00FFFFFF,  //  WHITE
        0x00AAAAAA,  //  GREY
        0x00FF0000,  //  RED
        0x0000FF00,  //  GREEN
        0x000000FF,  //  BLUE
        0x00FFFF00,  //  YELLOW
        };

// Variable zum Auslesen des Systemtaktes
uint32_t sysClock;

void main(void)
{
   int i,j,x,y;
   enum colors color;

   // Set system frequency to 120 MHz
   sysClock = SysCtlClockFreqSet(SYSCTL_OSC_INT |SYSCTL_USE_PLL |SYSCTL_CFG_VCO_480,120000000);

   init_ports_display(); // Init Port L for Display Control and Port M for Display Data
   configPorts();        // Init Port N for User LED and Port P for digital Input from Motor

   // Display initialization
    #ifdef LARGE_DISPLAY
        configure_display_controller_large();  // initalize and  configuration
    #endif /* LARGE_DISPLAY */
    #ifdef SMALL_DISPLAY
        configure_display_controller_small();  // initalize and  configuration
    #endif /* SMALL_DISPLAY */

    configure_display_set_background_color(colorarray[0]);


    j=0;
    // Start endless loop
    while(1)
    {
        // Test Motor and Display
        printf("Write rectangles\n"); // for debug only
        for (x=0;x<=(MAX_X)-1;x=x+30)
            for (y=0;y<=(MAX_Y)-1;y=y+30)
             {
                // Copy Digital Input PP0 (S1), PP1 (S2) to Port PN0 and PN1
                GPIO_PORTN_DATA_R = GPIO_PORTP_DATA_R & 0x03;

                window_set(x,y,x+14,y+14);
                write_command(0x2C); //write pixel command
                color=colorarray[(j)%7];
                j++; // change color
                for(i=0;i<(15*15);i++) // set pixels
                {
                    write_data((color>>16)&0xff); // red
                    write_data((color>>8)&0xff); // green
                    write_data((color)&0xff); // blue
                }
             }
    }

}
