#include <stdint.h>
#include <stdbool.h> // type bool for giop.h
#include "inc/hw_types.h"
#include "inc/tm4c1294ncpdt.h"
#include <stdio.h>   // Debug only
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>     // GPIO_PIN_X
#include <inc/hw_memmap.h>      // GPIO_PORTX_BASE


#define MAX_X 800
#define MAX_Y 480



// constants for display initialization
#define RST 0x10
#define INITIAL_STATE (0x1F)
#define SOFTWARE_RESET (0x01)
#define SET_PLL_MN (0xE2)
#define START_PLL (0xE0)
#define LOCK_PLL (0xE0)  // same as START_PLL
#define SET_LSHIFT (0xE6)
#define SET_LCD_MODE (0xB0)
#define SET_HORI_PERIOD (0xB4)
#define SET_VERT_PERIOD (0xB6)
#define SET_ADRESS_MODE (0x36)
#define SET_PIXEL_DATA_FORMAT (0xF0)
#define SET_DISPLAY_ON (0x29)
#define SET_DISPLAY_OFF (0x29) // not tested ??
// Size of Display define
#define LARGE_DISPLAY
// #define SMALL_DISPLAY

/********************************************************************************/
inline void init_ports_display(void);
/********************************************************************************/
void write_command(unsigned char command);
/********************************************************************************/
inline void write_data(unsigned char data);
/********************************************************************************/
inline void window_set(int min_x,int min_y,int max_x,int max_y);
/********************************************************************************/
void configure_display_controller_small (void); // 480 x 272 pixel
/********************************************************************************/
void configure_display_controller_large (void); // 800 x 480 pixel ???
/********************************************************************************/
void configure_display_set_background_color(int color);
/********************************************************************************/
void configure_display_paint_line_horizontal(short startx, short stopx, short y, int color);
/********************************************************************************/
void configure_display_paint_line_vertical(short starty, short stopy, short x, int color);



