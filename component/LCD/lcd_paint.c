/*
 * @file        : lcd_paint.c
 * @author      : Minh Tung Bui
 * @copyright   : HAW-Hamburg
 * @addtogroup  : component/LCD
 * @{
 */

#include <component/LCD/lcd_paint.h>
#include <component/LCD/lcd_config.h>

/**********************************************************************************
   * @brief     function to display the layout of Tacho
   * @details   void
   * @returns   void
 */
void display_layout(void)
{
    // display information for MC project and team
    display_project_information();

    // display number 0 on circle
    drawline(84, 377, 101, 369, COLOR_RED);
    print_string1216("0", 358, 108, COLOR_YELLO, COLOR_BLACK);

    // display number 30 on circle
    drawline_H(53, 72, 270, COLOR_RED, 1);
    print_string1216("30", 265, 78, COLOR_YELLO, COLOR_BLACK);

    // display number 60 on circle
    drawline(83, 158, 104, 166, COLOR_RED);
    print_string1216("60", 165, 112, COLOR_YELLO, COLOR_BLACK);

    // display number 90 on circle
    drawline(163, 80, 171, 99, COLOR_RED);
    print_string1216("90", 106, 166, COLOR_YELLO, COLOR_BLACK);

    // display number 120 on circle
    drawline_V(48, 69, 275, COLOR_RED, 1);
    print_string1216("120", 75, 257, COLOR_YELLO, COLOR_BLACK);

    // display number 150 on circle
    drawline(390, 80, 382, 99, COLOR_RED);
    print_string1216("150", 109, 360, COLOR_YELLO, COLOR_BLACK);

    // display number 180 on circle
    drawline(465, 162, 448, 170, COLOR_RED);
    print_string1216("180", 176, 420, COLOR_YELLO, COLOR_BLACK);

    // display number 210 on circle
    drawline_H(476, 496, 270, COLOR_RED, 1);
    print_string1216("210", 265, 441, COLOR_YELLO, COLOR_BLACK);

    // display number 240 on circle
    drawline(465, 376, 448, 368, COLOR_RED);
    print_string1216("240", 348, 420, COLOR_YELLO, COLOR_BLACK);

    // display gold circle
    drawCircle_px(X_CENTER, Y_CENTER, 222, COLOR_YELLO, 3);

    // outside red circle
     drawCircle_px(X_CENTER, Y_CENTER, 245, COLOR_RED, 1);

    // unter black line
    drawline_H(0, 550, 390, COLOR_BLACK, 100);

    // outside white circle
    drawCircle_px(X_CENTER, Y_CENTER, 235, COLOR_WHITE, 4);

    // display center point
    drawCircle_px(X_CENTER, Y_CENTER, 6, COLOR_WHITE, 4);

    // display gold insite circle
    drawCircle_px(X_CENTER, Y_CENTER, 30, COLOR_YELLO, 2);

    // display under black line
    drawline_H(0, 550, 470, COLOR_BLACK, 10);
    drawline_H(154, 395, 470, COLOR_WHITE, 3);
}

/**********************************************************************************
   * @brief     function to display the information about project
   * @details   void
   * @returns   void
 */
void display_project_information(void)
{
    set_background_color(COLOR_BLACK);

    drawline_H(551, 800, 170, COLOR_WHITE, 2);
    drawline_V(0, 480, 550, COLOR_WHITE, 2);
    // for unter line
    drawline_H(621, 738, 58, COLOR_WHITE, 1);

    // print Header
    print_string1216(
            "                    Fahzeug-Informations-Display                   ",
            0, 0, COLOR_WHITE, COLOR_BLUE);
    print_string1216("MC Projekt",
            40, 622, COLOR_WHITE, COLOR_BLACK);
    print_string1216("Mitglieder:",
                80, 565, COLOR_WHITE, COLOR_BLACK);
    print_string1216("1. Minh Tung Bui",
                    105, 565, COLOR_WHITE, COLOR_BLACK);
    print_string1216("2. Hauke Kosmiter",
                        125, 565, COLOR_WHITE, COLOR_BLACK);

    print_string1216("Fahzeug", 180, 630, COLOR_WHITE, COLOR_BLACK);

    print_string1216("- Richtung", 215, 565, COLOR_WHITE, COLOR_BLACK);
    print_string1216("- Kilometerstand:", 245, 565, COLOR_WHITE, COLOR_BLACK);
    print_string1216("  in m:", 270, 565, COLOR_WHITE, COLOR_BLACK);
    print_string1216("  in km:",295, 565, COLOR_WHITE, COLOR_BLACK);

    // HAW Logo
    drawline_H(555, 795, 350, COLOR_LOGO_BACKGROUND, 80);

    drawline_H(560, 600, 360, COLOR_BLUE_HAW, 3);
    drawline_H(560, 600, 374, COLOR_BLUE_HAW, 3);
    drawline_H(560, 600, 388, COLOR_BLUE_HAW, 3);
    drawline_H(560, 600, 402, COLOR_BLUE_HAW, 3);

    drawline_H(580, 620, 367, COLOR_BLUE, 3);
    drawline_H(580, 620, 381, COLOR_BLUE, 3);
    drawline_H(580, 620, 395, COLOR_BLUE, 3);
    drawline_H(580, 620, 409, COLOR_BLUE, 3);

    print_string1216("H A W", 370, 630, COLOR_BLUE, COLOR_LOGO_BACKGROUND);
    print_string1216("H A M B U R G", 395, 630, COLOR_BLUE, COLOR_LOGO_BACKGROUND);
}



