/*
 * @file        : lcd_paint.h
 * @author      : Minh Tung Bui
 * @copyright   : HAW-Hamburg
 * @addtogroup  : component/LCD
 * @{
 */

//<! defines color codes
enum color
{
    COLOR_BLACK             = 0x00000000,
    COLOR_WHITE             = 0x00FFFFFF,
    COLOR_GREY              = 0x00AAAAAA,
    COLOR_RED               = 0x00FF0000,
    COLOR_GREEN             = 0x0000FF00,
    COLOR_BLUE              = 0x000000FF,
    COLOR_YELLO             = 0x00FFFF00,
    COLOR_GOUND             = 0x0036454F,
    COLOR_LOGO_BACKGROUND   = 0x00F2F3F4,
    COLOR_BLUE_HAW          = 0x00A1CAF1
};

//<! midpoint coordinates of tacho
#define X_CENTER 275
#define Y_CENTER 270
#define PI 3.141596

/**********************************************************************************
   * @brief     function to display the layout of Tacho
   * @details   void
   * @returns   void
 */
void display_layout(void);

/**********************************************************************************
   * @brief     function to display the information about project
   * @details   void
   * @returns   void
 */
void display_project_information(void);
