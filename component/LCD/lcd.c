/*
 * lcd.c
 *
 *  Created on: 15 Nov 2020
 *      Author: minht
 */


#include "lcd.h"
#include "fontscharacter.h"

int x = 0;
int y = 0;

/********************************************************************************
     Elementary output functions  => speed optimized as inline
*********************************************************************************/
void write_command(unsigned char command)
{
    GPIO_PORTM_DATA_R = command;        // Write command byte
    GPIO_PORTL_DATA_R = 0x11;           // Chip select = 0, Command mode select = 0, Write state = 0
    GPIO_PORTL_DATA_R = 0x1F;           // Initial state
}
/********************************************************************************/
inline void write_data(unsigned char data)
{

    GPIO_PORTM_DATA_R = data;           // Write data byte
    GPIO_PORTL_DATA_R = 0x15;           // Chip select = 0, Write state = 0
    GPIO_PORTL_DATA_R = 0x1F;           // Initial state
}
/********************************************************************************/
inline void window_set(int min_x,int min_y,int max_x,int max_y)
{
    write_command(0x2A);           // Set row address x-axis
    write_data(min_x >> 8);        // Set start  address           (high byte)
    write_data(min_x);             // as above                     (low byte)
    write_data(max_x >> 8);        // Set stop address             (high byte)
    write_data(max_x);             // as above                     (low byte)

    write_command(0x2B);           // Set column address (y-axis)
    write_data(min_y >> 8);        // Set start column address     (high byte)
    write_data(min_y);             // as above                     (low byte)
    write_data(max_y >> 8);        // Set stop column address      (high byte)
    write_data(max_y);             // as above                     (low byte)
}

/********************************************************************************/
void configure_display_controller_small (void) // 480 x 272 pixel
{
    GPIO_PORTL_DATA_R = INITIAL_STATE;      // Initial state of display control signals
    GPIO_PORTL_DATA_R &= ~RST;              // Display hardware reset prepare, manual p. 79
    SysCtlDelay(10000);                     // wait >1 ms
    GPIO_PORTL_DATA_R |= RST;               // Display hardware reset trigger
    SysCtlDelay(10000);                     // wait >1 ms

    write_command(SOFTWARE_RESET);          // Display Software reset, manual p. 24
    SysCtlDelay(100000);                    // wait >5 ms

    write_command(SET_PLL_MN);              // Set PLL Freq of display controller to 100MHz
    write_data(0x1D);                       // see manual p. 74
    write_data(0x02);                       //
    write_data(0x04);                       //

    write_command(START_PLL);               // Start PLL, see manual p. 73
    write_data(0x01);                       //
    SysCtlDelay(10000);                     // wait >1 ms

    write_command(LOCK_PLL);               // Lock PLL, see manual p. 73/74
    write_data(0x03);                       //
    SysCtlDelay(10000);                     // wait >1 ms

    write_command(SOFTWARE_RESET);          // Software reset, manual p. 24
    SysCtlDelay(100000);                    // wait > 10 ms

    write_command(SET_LSHIFT);              // Pixel clock frequency
    write_data(0x01);                       // set pixel Clock 9MHz, see manual p. 76
    write_data(0x70);                       //
    write_data(0xA3);                       //

    write_command(SET_LCD_MODE);            // Set LCD Panel mode, see manual p. 76
    write_data(0x20);                       // TFT panel 24bit
    write_data(0x00);                       // TFT mode
    write_data(0x01);                       // Set horizontal size = 480-1 (high byte)
    write_data(0xDF);                       // Set horizontal size = 480-1 (low byte)
    write_data(0x01);                       // Set vertical size = 272-1  (high byte)
    write_data(0x0F);                       // Set vertical size = 272-1 (low byte)
    write_data(0x00);                       // Even line RGB sequence / Odd line RGB sequence RGB

    write_command(SET_HORI_PERIOD);         // Set horizontal periods, manual  p. 47
    write_data(0x02);                       // Horizontal total period (display + non-displayed) in pixels (high byte)
    write_data(0x13);                       // as above (low byte) = total 531 pixels
    write_data(0x00);                       // Non-displayed period between the start of the horizontal sync (LLINE) signal
                                            // and the first display data  (high byte)
    write_data(0x2B);                       // as above (low byte) = total sync pulse start position  is 43 pixels
    write_data(0x0A);                       // Set the horizontal sync width = 10 pixels
    write_data(0x00);                       // Set horizontal sync pulse start (high byte)
    write_data(0x08);                       // as above (low byte) = total sync pulse start position is 8 pixels
    write_data(0x00);                       //

    write_command(SET_VERT_PERIOD);         // Set vertical periods, manual  p. 49
    write_data(0x01);                       // Vertical total period (display + non-displayed) in lines (high byte)
    write_data(0x20);                       // as above (low byte) = total 288 lines
    write_data(0x00);                       //
    write_data(0x0C);                       // The non-displayed period in lines between the start of the frame and the first
                                            // display data in line = 12 pixels.
    write_data(0x0A);                       // Set the horizontal sync width = 10 pixels
    write_data(0x00);                       // Set vertical sync pulse start position (high byte)
    write_data(0x04);                       // as above (low byte) = total sync pulse start position is 4 lines


    write_command(SET_ADRESS_MODE);         // Pixel address counting = flip display , manual p. 36
    write_data(0x03);                       // necessary to match with touch screen addressing

//  write_command(0x0A);                    // Power control mode not tested in detail
//  write_data(0x1C);

    write_command(SET_PIXEL_DATA_FORMAT);    // set pixel data format 8bit manual p. 78
    write_data(0x00);

    write_command(SET_DISPLAY_ON);           // Set display on  manual p. 78
}
/********************************************************************************/
inline void init_and_config_display(void)
{
    // Set Port M Pins 0-7: used as Output of LCD Data
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);            // enable clock-gate Port M
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOM));     // wait until clock ready
    GPIOPinTypeGPIOOutput(GPIO_PORTM_BASE, 0xFF);
    // Set Port L Pins 0-4: used as Output of LCD Control signals:
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);  // Clock Port Q
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOL));
    GPIOPinTypeGPIOOutput(GPIO_PORTL_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3| GPIO_PIN_4);

    configure_display_controller_large();  // initalize and  configuration
}
/********************************************************************************/

void configure_display_controller_large (void) // 800 x 480 pixel ???
{
//////////////////////////////////////////////////////////////////////////////////
    GPIO_PORTL_DATA_R = INITIAL_STATE;      // Initial state
    GPIO_PORTL_DATA_R &= ~RST;              // Hardware reset
    SysCtlDelay(10000);                     // wait >1 ms
    GPIO_PORTL_DATA_R |= RST;               //
    SysCtlDelay(12000);                     // wait >1 ms

    write_command(SOFTWARE_RESET);          // Software reset
    SysCtlDelay(120000);                    // wait >10 ms

    write_command(SET_PLL_MN);               // Set PLL Freq to 120 MHz
    write_data(0x24);                        //
    write_data(0x02);                        //
    write_data(0x04);                        //

    write_command(START_PLL);                // Start PLL
    write_data(0x01);                        //
    SysCtlDelay(10000);                      // wait 1 ms

    write_command(START_PLL);                // Lock PLL
    write_data(0x03);                        //
    SysCtlDelay(10000);                      // wait 1 ms

    write_command(SOFTWARE_RESET);           // Software reset
    SysCtlDelay(100000);                     // wait 10 ms

    write_command(0xe6);                    // Set pixel clock frequency
    write_data(0x01);                       // KRR Set LCD Pixel Clock 9MHz
    write_data(0x70);                       // KRR
    write_data(0xA3);                       // KRR

    write_command(SET_LCD_MODE);          // SET LCD MODE SIZE, manual p. 44
    write_data(0x20);                     // ..TFT panel 24bit
    write_data(0x00);                     // ..TFT mode
    write_data(0x03);                     // SET horizontal size = 800-1 (high byte)
    write_data(0x1F);                     // SET horizontal size = 800-1 (low byte)
    write_data(0x01);                     // Set vertical size = 480-1 (high byte)
    write_data(0xDF);                     // Set vertical size = 480-1 (low byte)
    write_data(0x00);                     // Even line RGB sequence / Odd line RGB sequence RGB

    write_command(SET_HORI_PERIOD);       // Set Horizontal Period
    write_data(0x03);                     // Horizontal total period (display + non-displayed)  (high byte)
    write_data(0x5E);                     // Horizontal total period (display + non-display) (low byte)
    write_data(0x00);                     // Non-displayed period between the start of the horizontal sync (LLINE) signal and the first displayed data.
    write_data(0x46);                     // Low byte of the non-display period between the start of the horizontal sync (LLINE) signal and the first display data
    write_data(0x09);                     // Set the vertical sync width
    write_data(0x00);                     // Set horiz.Sync pulse start    (high byte)
    write_data(0x08);                     // Set horiz.Sync pulse start    (low byte)
    write_data(0x00);                     //


    write_command(SET_VERT_PERIOD);         // Set vertical periods, manual  p. 49
    write_data(0x01);                       // Vertical total period (display + non-displayed) in lines (high byte)
    write_data(0xFE);                       // as above (low byte) = total 510  lines
    write_data(0x00);                       //
    write_data(0x0C);                       // The non-displayed period in lines between the start of the frame and the first
                                            // display data = 12 line.s
    write_data(0x0A);                       // Set the vertiacla sync width = 10 pixels
    write_data(0x00);                       // Set vertical sync pulse start position (high byte)
    write_data(0x04);                       // as above (low byte) = total sync pulse start position is 4 lines

    write_command(SET_ADRESS_MODE);         // Pixel address counting = flip display , manual p. 36
    write_data(0x03);                       // necessary to match with touch screen addressing

//  write_command(0x0A);                    // Power control mode not tested in detail
//  write_data(0x1C);

    write_command(SET_PIXEL_DATA_FORMAT);    // set pixel data format 8bit manual p. 78
    write_data(0x00);

    write_command(SET_DISPLAY_ON);           // Set display on  manual p. 78
}

void set_background_color(int color)
{
    int x, y;
    printf("Start Background Pixel by Pixel set\n"); // for debug only
    // set pixel by pixel to change the background colors
    window_set(0, 0, MAX_X - 1, MAX_Y - 1); // set single position see B.4  // to do faster ?
    write_command(0x2C); //write pixel command
    for (x = 0; x <= (MAX_X) - 1; x++)
        for (y = 0; y <= (MAX_Y) - 1; y++)
        {
            write_data((color >> 16) & 0xff);   // red
            write_data((color >> 8) & 0xff);    // green
            write_data((color) & 0xff);         // blue
        }
    printf("Background ready \n"); // for debug only
}

void drawline_H(short x1, short x2, short y, int color , int px)
{
    int x, y_i;
    window_set(x1, y, x2, y + px);
    write_command(0x2C);
    for(x = x1; x <= x2; x++)
    {
        for (y_i = y; y_i <= y + px; y_i ++)
        {
            write_data((color >> 16) & 0xff); // red
            write_data((color >> 8) & 0xff); // green
            write_data((color) & 0xff); // blue
        }
    }
}

void drawline_V(short y1, short y2, short x, int color , int px)
{
    int x_i, y;
    window_set(x, y1, x + px, y2);
    write_command(0x2C);
    for (x_i = x; x_i <= x + px; x_i ++)
    {
        for(y = y1; y <= y2; y++)
        {
            write_data((color >> 16) & 0xff); // red
            write_data((color >> 8) & 0xff);// green
            write_data((color) & 0xff);// blue
        }
    }
}

void drawCircle(int x, int y, int radius, int color)
{
    int f = 1 - radius;
    int ddF_x = 1;
    int ddF_y = -2 * radius;
    int x1 = 0;
    int y1 = radius;


    window_set(x, y + radius, x, y + radius);
    write_command(0x2C);

    write_data((color >> 16) & 0xff); // red
    write_data((color >> 8) & 0xff); // green
    write_data((color) & 0xff); // blue

    window_set(x, y - radius, x, y - radius);
    write_command(0x2C);
    write_data((color >> 16) & 0xff); // red
    write_data((color >> 8) & 0xff); // green
    write_data((color) & 0xff); // blue

    window_set(x + radius, y, x + radius, y);
    write_command(0x2C);
    write_data((color >> 16) & 0xff); // red
    write_data((color >> 8) & 0xff); // green
    write_data((color) & 0xff); // blue


    window_set(x - radius, y, x - radius, y);
    write_command(0x2C);
    write_data((color >> 16) & 0xff); // red
    write_data((color >> 8) & 0xff); // green
    write_data((color) & 0xff); // blue


    while (x1 < y1)
    {
        if (f >= 0)
        {
            y1--;
            ddF_y += 2;
            f += ddF_y;
        }
        x1++;
        ddF_x += 2;
        f += ddF_x;

        window_set(x + x1, y + y1, x + x1, y + y1);
        write_command(0x2C);
        write_data((color >> 16) & 0xff); // red
        write_data((color >> 8) & 0xff); // green
        write_data((color) & 0xff); // blue


        window_set(x - x1, y + y1, x - x1, y + y1);
        write_command(0x2C);
        write_data((color >> 16) & 0xff); // red
        write_data((color >> 8) & 0xff); // green
        write_data((color) & 0xff); // blue


        window_set(x + x1, y - y1, x + x1, y - y1);
        write_command(0x2C);
        write_data((color >> 16) & 0xff); // red
        write_data((color >> 8) & 0xff); // green
        write_data((color) & 0xff); // blue


        window_set(x - x1, y - y1, x - x1, y - y1);
        write_command(0x2C);
        write_data((color >> 16) & 0xff); // red
        write_data((color >> 8) & 0xff); // green
        write_data((color) & 0xff); // blue


        window_set(x + y1, y + x1, x + y1, y + x1);
        write_command(0x2C);
        write_data((color >> 16) & 0xff); // red
        write_data((color >> 8) & 0xff); // green
        write_data((color) & 0xff); // blue


        window_set(x - y1, y + x1, x - y1, y + x1);
        write_command(0x2C);
        write_data((color >> 16) & 0xff); // red
        write_data((color >> 8) & 0xff); // green
        write_data((color) & 0xff); // blue


        window_set(x + y1, y - x1, x + y1, y - x1);
        write_command(0x2C);
        write_data((color >> 16) & 0xff); // red
        write_data((color >> 8) & 0xff); // green
        write_data((color) & 0xff); // blue


        window_set(x - y1, y - x1, x - y1, y - x1);
        write_command(0x2C);
        write_data((color >> 16) & 0xff); // red
        write_data((color >> 8) & 0xff); // green
        write_data((color) & 0xff); // blue
    }
}

void drawCircle_px(int x, int y, int radius, int color, int px)
{
    int i;
    for ( i = radius; i >= radius - px; i--)
    {
        drawCircle(x, y, i, color);
    }
}

void write_char(int w, int color, int backcolor)
{
    int lv;
    for (lv = 0; lv < 12; lv++)
    {
        if (w & 1)
        {
            write_data((color >> 16) & 0xff); // red
            write_data((color >> 8) & 0xff); // green
            write_data((color) & 0xff); // blue
        }
        else
        {
            write_data((backcolor >> 16) & 0xff); // red
            write_data((backcolor >> 8) & 0xff); // green
            write_data((backcolor) & 0xff); // blue
        }
        w = w>>1;
    }
}

void print_string1216(char *text, int row, int column, int color, int backcolor)
{
    int w;
    int lv1, numChar;
    int length = strlen(text);
    int font_width = 12;
    int font_hight = 16;
    int columnStart = column;
    int columnStop = columnStart + font_width - 1;
    int rowStart = row;
    int rowStop = rowStart + font_hight - 1;

    for (numChar = 0; numChar < length; numChar++)
    {
        window_set(columnStart, rowStart, columnStop, rowStop);
        write_command(0x2C);
        for (lv1 = 0; lv1 < 32; lv1 = lv1 + 2)
        {
            w = (font_12_16[text[numChar]][lv1 + 1] << 4)
                    | (font_12_16[text[numChar]][lv1] >> 4);
            write_char(w, color, backcolor);
        }
        columnStart += font_width;
        columnStop += font_width;
    }
}

/******************************************************************************************************/
//draws a line from startpoint x to stoppoint y directly to the display
void drawline(short x1, short y1, short x2, short y2, int color)
{
    short old_x, old_y, x, y, i;
    int start_x, stop_x, start_y, stop_y;
    double gain;

    // 90° line:
    if (x1 == x2)
    {
        if (y1 > y2)
        {        // 90° from DOWN to UP   else: 270° from UP to DOWN
            start_y = y2;
            stop_y = y1;
        }
        else
        {
            start_y = y1;
            stop_y = y2;
        }
        window_set(x1, start_y, x2, stop_y);
        write_command(0x2C);
        for (x = start_y; x <= stop_y; x++)
        {
            write_data((color >> 16) & 0xff); // red
            write_data((color >> 8) & 0xff); // green
            write_data((color) & 0xff); // blue
        }
    }
    // 0° line:
    else if (y1 == y2)
    {
        if (x1 > x2)
        {
            start_x = x2;
            stop_x = x1;
        }
        else
        {
            start_x = x1;
            stop_x = x2;
        }
        window_set(start_x, y1, stop_x, y2);
        write_command(0x2C);
        for (x = start_x; x <= stop_x; x++)
        {
            write_data((color >> 16) & 0xff); // red
            write_data((color >> 8) & 0xff); // green
            write_data((color) & 0xff); // blue
        }
    }
/////////////////////////////////////////////////////////////////////////////////////////
    else
    {
        if (x1 > x2)
        {      // running direction is negative ! => switch start and stop
            start_x = x2;
            stop_x = x1;
            start_y = y2;
            stop_y = y1;
        }
        else
        {
            start_x = x1;
            stop_x = x2;
            start_y = y1;
            stop_y = y2;
        }
        //(stop_y - start_y) can be positive or negative
        gain = (double) (stop_y - start_y) / (stop_x - start_x);

        if (gain >= 1)
        {
            old_y = start_y;

            for (x = start_x; x <= stop_x; x++)
            {
                y = gain * (x - start_x) + start_y;
                window_set(x, old_y, x, y);
                write_command(0x2C);
                for (i = old_y; i <= y; i++)
                {
                    write_data((color >> 16) & 0xff); // red
                    write_data((color >> 8) & 0xff); // green
                    write_data((color) & 0xff); // blue
                }
                old_y = y;
            }
        }
        else if (gain > 0)
        {
            old_x = start_x;

            for (y = start_y; y <= stop_y; y++)
            {
                x = (y - start_y) / gain + start_x;
                window_set(old_x, y, x, y);
                write_command(0x2C);
                for (i = old_x; i <= x; i++)
                {
                    write_data((color >> 16) & 0xff); // red
                    write_data((color >> 8) & 0xff); // green
                    write_data((color) & 0xff); // blue
                }
                old_x = x;
            }
        }

        else if (gain <= -1)
        {
            old_y = start_y;

            for (x = start_x; x <= stop_x; x++)
            {
                y = gain * (x - start_x) + start_y;
                window_set(x, y, x, old_y);
                write_command(0x2C);
                for (i = y; i <= old_y; i++)
                {
                    write_data((color >> 16) & 0xff); // red
                    write_data((color >> 8) & 0xff); // green
                    write_data((color) & 0xff); // blue
                }
                old_y = y;
            }
        }
        // start_y > stop_y
        else if (gain < 0)
        {
            old_x = start_x;

            for (y = start_y; y >= stop_y; y--)
            {
                x = (y - start_y) / gain + start_x;
                window_set(old_x, y, x, y);
                write_command(0x2C);
                for (i = old_x; i <= x; i++)
                {
                    write_data((color >> 16) & 0xff); // red
                    write_data((color >> 8) & 0xff); // green
                    write_data((color) & 0xff); // blue
                }
                old_x = x;
            }
        }
    }
}

