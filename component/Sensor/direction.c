/*
 * @file        : direction.c
 * @author      : Minh Tung Bui and Hauke Kosmiter
 * @copyright   : HAW-Hamburg
 *
 * In this file the direction will be determined and print on the display
 */
#include "direction.h"

void determine_direction(uint8_t move_direction){
    // Switch Case statement with move_direction as argument
    switch (move_direction)
        {
        // Case: Not moving
        case STILL:
            print_string1216("( )", 215, 700, COLOR_BLACK, COLOR_YELLO);

            if (toggle == true)
            {
                print_string1216("  NONE  ", 450, 230, COLOR_RED, COLOR_BLACK);
            }
            else
            {
                print_string1216("        ", 450, 230, COLOR_RED, COLOR_BLACK);
            }

            break;
        // Case: Moving Forward
        case FORWARD:
            print_string1216("(V)", 215, 700, COLOR_BLACK, COLOR_YELLO);
            print_string1216(" FORWARD", 450, 230, COLOR_RED, COLOR_BLACK);
            break;
        // Case: Moving Forward
        case BACKWARD:
            print_string1216("(R)", 215, 700, COLOR_BLACK, COLOR_YELLO);
            print_string1216("BACKWARD", 450, 230, COLOR_RED, COLOR_BLACK);
            break;
        default:
            break;
        }
}
