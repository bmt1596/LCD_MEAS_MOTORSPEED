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
#include <stdint.h>
#include <math.h>


#include "inc/tm4c1294ncpdt.h" // Macros fpr Registers and Bitfields
#include "inc/hw_memmap.h" //  Tivaware Macros ...BASE as base address for peripheral modules
#include "driverlib/systick.h"   //  Tivaware functions: SysTick...
#include "driverlib/interrupt.h" //  Tivaware functions: Int...
#include "driverlib/sysctl.h"    //  Tivaware functions: SysCtl...  + Macros  SYSCTL_...
#include "driverlib/gpio.h"      //  Tivaware functions: GPIO... + Macros  GPIO_...
#include "driverlib/timer.h"

typedef float float32_t;

// global testvariables
uint32_t sysCLK;
uint32_t S1counter = 0;
uint32_t S2counter = 0;

uint8_t move_direction;

float32_t veclocity_tacho = 0;
float32_t distance_in_m = 0;
float32_t distance_in_km = 0;

bool toggle = true;

enum direction{FORWARD, BACKWARD, STILL };

void Timer1_DisplayIntHandler(void);
void init_peripherals(void);
void S1Handler(void);
void S2Handler(void);

void S1Handler(void)
{
    GPIO_PORTP_ICR_R |= 0x01; //clear interrupt p0
    S1counter++; // count of runs of this handler
    GPIO_PORTP_ICR_R |= 0x01; //clear interrupt p0
}

void S2Handler(void)
{
    GPIO_PORTP_ICR_R |= 0x02; //clear interrupt p1
    S2counter++; // count the runs of this handler

    if (GPIO_PORTP_DATA_R & 0x02 == 0x02)
    {
        move_direction = BACKWARD;
    }
    else
    {
        move_direction = FORWARD;
    }
}

void Timer1_DisplayIntHandler(void)
{
    static uint16_t radius = 160;
    static uint16_t phinull = 150;
    static uint16_t phi;
    static uint16_t x, y;
    static uint16_t x_old = X_CENTER, y_old = Y_CENTER;
    char buffer1[20], buffer2[20], buffer3[20];

    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

    veclocity_tacho = (double) (S2counter * 1.151);

    if(veclocity_tacho > 180)
    {
        if (toggle == true)
        {
            print_string1216("TOO FAST!!", 350, 220, COLOR_RED, COLOR_BLACK);
        }else
        {
            print_string1216("          ", 350, 220, COLOR_BLACK, COLOR_BLACK);
        }

    }
    else if (veclocity_tacho >= 240)
    {
        veclocity_tacho = 240;
    }
    else
    {
        print_string1216("          ", 350, 220, COLOR_BLACK, COLOR_BLACK);
    }

    distance_in_m = distance_in_m + (double) (veclocity_tacho / 9);
    distance_in_km = (double) (distance_in_m / 1000);

    if (veclocity_tacho < 11)
    {
        sprintf(buffer1, " %3.2lf   ", veclocity_tacho);
    }
    else if (veclocity_tacho < 101)
    {
        sprintf(buffer1, " %3.2lf   ", veclocity_tacho);
    }
    else
    {
        sprintf(buffer1, "%3.2lf   ", veclocity_tacho);
    }

    print_string1216(buffer1, 410, 238, COLOR_WHITE, COLOR_BLACK);

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

    switch (move_direction)
    {
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
    case FORWARD:
        print_string1216("(V)", 215, 700, COLOR_BLACK, COLOR_YELLO);
        print_string1216(" FORWARD", 450, 230, COLOR_RED, COLOR_BLACK);
        break;
    case BACKWARD:
        print_string1216("(R)", 215, 700, COLOR_BLACK, COLOR_YELLO);
        print_string1216("BACKWARD", 450, 230, COLOR_RED, COLOR_BLACK);
        break;
    default:
        break;
    }

    S2counter = 0;
    move_direction = STILL;

    toggle = !toggle;

}

void init_peripherals(void)
{
    // CLOCK GATES ENABLE
    /********************************************************************************/
    int i = 0;
    //Configurations of Port Pin as interrupts source
    SYSCTL_RCGCGPIO_R = 0x2000; // switch clock on for Port P

    i++; // wait for clock stable at periphery
    GPIO_PORTP_DEN_R |= 0x03; // enable P0 & P1
    GPIO_PORTP_DIR_R &= ~0x03; // input P0 & P1
    GPIO_PORTP_IS_R &= ~0x03; // boht pins generates edge−sensitive requests
    GPIO_PORTP_IBE_R &= ~0x03; // not use both edges
    //GPIO_PORTP_IBE_R |= 0x03; // alternative use both edges
    GPIO_PORTP_IEV_R |= 0x03; //interrupt events are rising edges
    GPIO_PORTP_ICR_R |= 0x03; //clear interrupt requests
    GPIO_PORTP_IM_R |= 0x03; // unmask P0 and P1 as I−Sources
    GPIO_PORTP_SI_R |= 0x01; // speciality of Port P and Q
    // here sete P0 and P1 as seperate source <===== Each pin has its own interrupt vector. page 750 and 791 manua l
    // GPIO_PORTP_SI_R &= ~0x01; // alternative : summary interrupt for ALL Pins of Port P
    NVIC_EN2_R |= 0x3000; // enable Port P I−Requests in the NVIC
    //printf("Count Interrupt Requests and Levels at P0 und P1 \n");

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

    // TIMER
    /********************************************************************************/
    // Configure Timer1 Interrupt
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);    // Clock Gate enable TIMER1
    TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER1_BASE, TIMER_A, sysCLK / 2.5);      // fires every 200 ms
    TimerIntRegister(TIMER1_BASE, TIMER_A, Timer1_DisplayIntHandler);
    IntEnable(INT_TIMER1A);
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
}


void main(void)
{
    // Set system frequency to 120 MHz
    sysCLK = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);
    init_peripherals();

    configure_display_controller_large();
    display_layout();

    TimerEnable(TIMER1_BASE, TIMER_A);

    while (1);
}

