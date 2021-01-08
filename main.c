/*
 * @file        : lcd_paint.c
 * @author      : Minh Tung Bui and Hauke Kosmiter
 * @copyright   : HAW-Hamburg
 * @addtogroup  : component/LCD
 * @{
 */

#include <component/LCD/lcd_config.h>
#include <component/LCD/lcd_paint.h>
#include <component/Sensor/sensor.h>
#include <stdio.h>
#include <math.h>


#include "inc/tm4c1294ncpdt.h" // Macros fpr Registers and Bitfields
#include "inc/hw_memmap.h" //  Tivaware Macros ...BASE as base address for peripheral modules
#include "driverlib/systick.h"   //  Tivaware functions: SysTick...
#include "driverlib/interrupt.h" //  Tivaware functions: Int...
#include "driverlib/sysctl.h"    //  Tivaware functions: SysCtl...  + Macros  SYSCTL_...
#include "driverlib/gpio.h"      //  Tivaware functions: GPIO... + Macros  GPIO_...
#include "driverlib/timer.h"

typedef float float32_t;


uint32_t sysCLK;
uint32_t sensor_count_impuls = 0;
uint8_t move_direction;

float32_t veclocity_tacho = 0;
float32_t distance_in_m = 0;
float32_t distance_in_km = 0;


enum direction{FORWARD, BACKWARD, STATIONARY };

void Timer1_DisplayIntHandler(void)
{
    static uint16_t radius = 160;
    static uint16_t phinull = 150;
    static uint16_t phi;
    static uint16_t x, y;
    static uint16_t x_old = X_CENTER, y_old = Y_CENTER;
    char buffer1[20], buffer2[20], buffer3[20];

    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

    veclocity_tacho = (double) (sensor_count_impuls * 1.91);
    if (veclocity_tacho >= 240)
    {
        veclocity_tacho = 240;
    }
    distance_in_m = distance_in_m + (double) (veclocity_tacho / 18);
    distance_in_km = (double) (distance_in_m / 1000);

    sprintf(buffer1, "%3.2lf", veclocity_tacho);
    print_string1216("  km/h  ", 380, 225, COLOR_BLACK, COLOR_YELLO);
    print_string1216(buffer1, 410, 238, COLOR_BLACK, COLOR_YELLO);


    sprintf(buffer2, "%3.2lf", distance_in_m);
    print_string1216(buffer2, 270, 700, COLOR_BLACK, COLOR_YELLO);

    sprintf(buffer3, "%3.2lf", distance_in_km);
    print_string1216(buffer3, 295, 700, COLOR_BLACK, COLOR_YELLO);

    phi = veclocity_tacho + phinull;
    x = X_CENTER + round(radius * cos((double) (phi) * 2 * PI / 360));
    y = Y_CENTER + round(radius * sin((double) (phi) * 2 * PI / 360));

    // remove a old line and set to background color
    if (x != x_old || y != y_old)
    {
        drawline(X_CENTER, Y_CENTER, x_old, y_old, COLOR_BLACK);
        drawCircle_px(x_old, y_old, 6, COLOR_BLACK, 2);
    }

    // display new line
    drawline(X_CENTER, Y_CENTER, x, y, COLOR_RED);
    drawCircle_px(x, y, 6, COLOR_RED, 2);

    // display center point
    drawCircle_px(X_CENTER, Y_CENTER, 6, COLOR_WHITE, 4);
    drawCircle_px(X_CENTER, Y_CENTER, 30, COLOR_YELLO, 2);
    print_string1216("Km/h", 135, 253, COLOR_WHITE, COLOR_BLACK);

    x_old = x;
    y_old = y;
    if (sensor_count_impuls == 0)
    {
        print_string1216("( )", 215, 700, COLOR_BLACK, COLOR_YELLO);
    }
    if (move_direction == FORWARD)
    {
        print_string1216("(V)", 215, 700, COLOR_BLACK, COLOR_YELLO);
    }
    else if (move_direction == BACKWARD)
    {
        print_string1216("(R)", 215, 700, COLOR_BLACK, COLOR_YELLO);
    }

    sensor_count_impuls = 0;
}

void Count_IntHandler(void)
{
    printf("%d\n", sensor_count_impuls);

    GPIOIntClear(GPIO_PORTP_BASE, GPIO_PIN_0); // finially not needed, but done as a matter of principle
    SysTickDisable();

    sensor_count_impuls++;
    GPIO_PORTN_DATA_R = GPIO_PORTP_DATA_R & 0x03;

    printf("%d\n", sensor_count_impuls);

    if (GPIO_PORTN_DATA_R == 0x03)
    {
        move_direction = FORWARD;
    }
    else if (GPIO_PORTN_DATA_R == 0x02)
    {
        move_direction = BACKWARD;
    }
    else if (GPIO_PORTN_DATA_R == 0x00 || GPIO_PORTN_DATA_R == 0x01 )
    {
        move_direction = STATIONARY;
    }
    printf("%d\n", sensor_count_impuls);
}

void init_peripherals(void)
{
    // CLOCK GATES ENABLE
    /********************************************************************************/
    IntMasterDisable();

    // PERIPHERY CLOCK ENABLE
    /********************************************************************************/

    // Set Port P Pins 0-1: used as Interrupts
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOP);            // Clock Gate enable GPIO/P
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOP));    // wait until Port P ready

    // Set Port N Pins 0-1: used for LED signals
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);            // Clock Gate enable GPIO/N
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION));    // wait until Port N ready

    // Set Port M Pins 0-7: used as Output of LCD Data
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);            // enable clock-gate Port M
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOM));    // wait until clock ready

    // Set Port L Pins 0-4: used as Output of LCD Control signals:
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);            // enable clock-gate Port L
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOL)) ;   // wait until clock ready

    // Port N: LED signals output
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1); // PortN
    // Set Port M Pins 0-7: used as Output of LCD Data
    GPIOPinTypeGPIOOutput(GPIO_PORTM_BASE, 0xFF);
    // Set Port L Pins 0-4: used as Output of LCD Control signals:
    GPIOPinTypeGPIOOutput(GPIO_PORTL_BASE, 0x1F);

    // INTERRUPTS
    /********************************************************************************/
    GPIOPinTypeGPIOInput(GPIO_PORTP_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    GPIO_PORTP_DEN_R = 0x03;
    // Rising edge type interrupt
    GPIOIntTypeSet(GPIO_PORTP_BASE, GPIO_PIN_0, GPIO_RISING_EDGE | GPIO_DISCRETE_INT);
    // "register" entry in  a copied IVT
    GPIOIntRegister(GPIO_PORTP_BASE, Count_IntHandler);
    GPIOIntClear(GPIO_PORTP_BASE, GPIO_PIN_0);              // optional ...
    //IntPrioritySet(INT_GPIOP0, 0x20);                     //high prio
    GPIOIntEnable(GPIO_PORTP_BASE, GPIO_PIN_0);             // Allow request output from Port unit
    IntEnable(INT_GPIOP0);                                  // Allow request input to NVIC

    // TIMER
    /********************************************************************************/
    // Configure Timer1 Interrupt
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);           // Clock Gate enable TIMER1
    TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER1_BASE, TIMER_A, sysCLK / 5);         // fires every 200 ms
    TimerIntRegister(TIMER1_BASE, TIMER_A, Timer1_DisplayIntHandler);
    IntEnable(INT_TIMER1A);
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    TimerEnable(TIMER1_BASE, TIMER_A);
}

void main(void)
{
    // Set system frequency to 120 MHz
    sysCLK = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);

    init_peripherals();
    configure_display_controller_large();

    display_layout();
    IntMasterEnable();
    while (1)
        ;
}

