/*
 * @file        : lcd_config.d
 * @author      : Minh Tung Bui
 * @copyright   : HAW-Hamburg
 * @addtogroup  : component/LCD
 * @{
 */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>                // type bool for giop.h
#include "inc/hw_types.h"
#include "inc/tm4c1294ncpdt.h"
#include <stdio.h>                  // Debug only
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>         // GPIO_PIN_X
#include <inc/hw_memmap.h>          // GPIO_PORTX_BASE

#define MAX_X 800
#define MAX_Y 480

// constants for display initialization
#define RST 0x10
#define INITIAL_STATE (0x1F)
#define SOFTWARE_RESET (0x01)
#define SET_PLL_MN (0xE2)
#define START_PLL (0xE0)
#define LOCK_PLL (0xE0)             // same as START_PLL
#define SET_LSHIFT (0xE6)
#define SET_LCD_MODE (0xB0)
#define SET_HORI_PERIOD (0xB4)
#define SET_VERT_PERIOD (0xB6)
#define SET_ADRESS_MODE (0x36)
#define SET_PIXEL_DATA_FORMAT (0xF0)
#define SET_DISPLAY_ON (0x29)
#define SET_DISPLAY_OFF (0x29)      // not tested ??
// Size of Display define
#define LARGE_DISPLAY


/**********************************************************************************
   * @brief     function to config Port M, L and init for LCD display
   * @details   void
   * @returns   void
 */
inline void init_and_config_display(void);

/**********************************************************************************
   * @brief     function to write command byte on Port M and select chip
   * @details   void
   * @returns   void
 */

void write_command(unsigned char command);

/**********************************************************************************
   * @brief     function to write data byte on Port M and select chip
   * @details   void
   * @returns   void
 */
inline void write_data(unsigned char data);

/**********************************************************************************
   * @brief     function to set the frame to be displayed on the LCD
   * @details   int min_x,int min_y,int max_x,int max_y
   * @returns   void
 */
inline void window_set(int min_x,int min_y,int max_x,int max_y);

/**********************************************************************************
   * @brief     function to set configure for small display 480 x 272 pixel
   * @details   void
   * @returns   void
 */
void configure_display_controller_small (void);

/**********************************************************************************
   * @brief     function to set configure for large display 480 x 800 pixel
   * @details   void
   * @returns   void
 */
void configure_display_controller_large (void);

/**********************************************************************************
   * @brief     function to set the color for background
   * @details   the frame is 480 x 800 pixel
   * @returns   void
 */
void set_background_color(int color);

/**********************************************************************************
   * @brief     function to draw the horizontal line with color
   * @details   x1 (x-start), x2 (x-stop), y , color , px (size)
   * @returns   void
 */
void drawline_H(short x1, short x2, short y, int color , int px);

/**********************************************************************************
   * @brief     function to draw the vertical line with color
   * @details   y1 (y-start), y2 (y-stop), x , color , px (size)
   * @returns   void
 */
void drawline_V(short y1, short y2, short x, int color , int px);

/**********************************************************************************
   * @brief     function to write the character on screen
   * @details   w (character), color1 (color for character), color2 (background color)
   * @returns   void
 */
void write_char(int w, int color1, int color2);

/**********************************************************************************
   * @brief     function to write the string on screen
   * @details   *text (text), row,  column,  color,  backcolor)
   * @returns   void
 */
void print_string1216(char *text, int row, int column, int color, int backcolor);

/**********************************************************************************
   * @brief     function to write a cicle on screen
   * @details   x (center x), y (center y ), radius, color)
   * @returns   void
 */
void drawCircle(int x, int y, int radius, int color);

/**********************************************************************************
   * @brief     function to write a cicle with size on screen
   * @details   x (center x), y (center y ), radius, color) , px (size)
   * @returns   void
 */
void drawCircle_px(int x, int y, int radius, int color, int px);

/**********************************************************************************
   * @brief     function to write a normal line
   * @details   x1 (x start),  y1 (y start),  x2 (stop), y2 (stop), color
   * @returns   void
 */
void drawline(short x1, short y1, short x2, short y2, int color);



