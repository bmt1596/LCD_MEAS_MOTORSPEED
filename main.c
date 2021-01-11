/*
 * @file        : lcd_paint.c
 * @author      : Minh Tung Bui and Hauke Kosmiter
 * @copyright   : HAW-Hamburg
 * @addtogroup  : component/LCD
 * @{
 */

#include <component/Initiation/init.h>

void main(void)
{
    // Set system frequency to 120 MHz
    sysCLK = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);
    init_peripherals();
    init_var();

    configure_display_controller_large();
    display_layout();

    TimerEnable(TIMER1_BASE, TIMER_A);

    while (1);
}

