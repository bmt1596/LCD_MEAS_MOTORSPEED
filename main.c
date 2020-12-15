#include <component/LCD/lcd_config.h>
#include <component/LCD/lcd_paint.h>
#include <component/Sensor/int_handler.h>
#include <component/Sensor/sensor.h>
#include <component/Sensor/Timer/timer.h>
#include <component/LCD/display_init.h>
#include <stdio.h>
#include <math.h>

#include "driverlib/systick.h"   //  Tivaware functions: SysTick...
#include "driverlib/interrupt.h" //  Tivaware functions: Int...
#include "driverlib/sysctl.h"    //  Tivaware functions: SysCtl...  + Macros  SYSCTL_...
#include "driverlib/gpio.h"      //  Tivaware functions: GPIO... + Macros  GPIO_...
#include "driverlib/timer.h"

//void display_layout(void);
//void display_project_information(void);
//void Timer1_DisplayIntHandler(void);
// test comment
void wait(int time);

uint32_t sysClock, timerScaler;
double geschwindigkeit = 0;
double ge = 0;
int richtung = 0;

void Timer1_DisplayIntHandler(void)
{
    static uint16_t radius = 160;
    static uint16_t phinull = 150;
    static uint16_t phi;
    static uint16_t x, y;
    static uint16_t x_old = X_CENTER, y_old = Y_CENTER;

    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    if (richtung == 0)
    {
    }
    else if (richtung == 1)
    {
        print_string1216("(V)", 215, 700, COLOR_BLACK, COLOR_YELLO);
    }
    else if (richtung == 2)
    {
        print_string1216("(R)", 215, 700, COLOR_BLACK, COLOR_YELLO);
    }

    char buffer[20];
    sprintf(buffer, "%3.2lf km/h", geschwindigkeit);
    print_string1216(buffer, 400, 225, COLOR_BLACK, COLOR_YELLO);

    phi = geschwindigkeit + phinull;
    x = X_CENTER + round(radius* cos((double)(phi)*2*PI/360));
    y = Y_CENTER + round(radius* sin((double)(phi)*2*PI/360));

    // remove a old line and set to background color
    if (x != x_old || y != y_old)
    {
        drawline(X_CENTER, Y_CENTER, x_old, y_old, COLOR_BLACK);
        drawCircle_px(x_old, y_old, 6, COLOR_BLACK, 2);
    }

    // display new line
    drawline(X_CENTER, Y_CENTER , x, y, COLOR_RED);
    drawCircle_px(x, y, 6, COLOR_RED, 2);

    // display center point
    drawCircle_px(X_CENTER, Y_CENTER, 6, COLOR_WHITE, 4);
    drawCircle_px(X_CENTER, Y_CENTER, 30, COLOR_YELLO, 2);
    print_string1216("Km/h", 135, 253, COLOR_WHITE, COLOR_BLACK);

    x_old = x; y_old = y;
}

void wait(int time)
{
    volatile int tmp;
    for (tmp = 0; tmp < 10800 * time; tmp++); // ~ 1ms
}

void Count_IntHandler(void)
{
    GPIOIntClear(GPIO_PORTP_BASE,GPIO_PIN_0); // finially not needed, but done as a matter of principle
    SysTickDisable(); // stop  systick
    ge++;
    GPIO_PORTN_DATA_R = GPIO_PORTP_DATA_R & 0x03;

    if (GPIO_PORTP_DATA_R == 0x03)
    {
        richtung = 1;
    }
    else if (GPIO_PORTP_DATA_R == 0x02)
    {
        richtung = 2;
    }
    else if (GPIO_PORTP_DATA_R == 0x00)
    {
        richtung = 0;
    }
}

void Timerout_Cal_IntHandler(void)
{
    TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
    geschwindigkeit = ge * 2.71;
    ge = 0;
    richtung = 0;
    print_string1216("( )", 215, 700, COLOR_BLACK, COLOR_YELLO);
    if (geschwindigkeit > 240)
    {
        geschwindigkeit = 240;
    }
}

void init_peripherals (void) {

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);    // Clock Gate enable TIMER0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);    // Clock Gate enable TIMER1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);    // Clock Gate enable TIMER2
    SysCtlDelay(10);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOP);                        //periphery clock enable
    GPIOPinTypeGPIOInput(GPIO_PORTP_BASE, GPIO_PIN_0);                  //pin setup
    GPIOIntTypeSet(GPIO_PORTP_BASE, GPIO_PIN_0, GPIO_RISING_EDGE);      // Rising edge type interrupt
    GPIOIntRegister(GPIO_PORTP_BASE, Count_IntHandler);
    GPIOIntClear(GPIO_PORTP_BASE, GPIO_PIN_0);                          // optional ...
    IntPrioritySet(INT_GPIOP0, 0x20);                                   // high prio
    GPIOIntEnable(GPIO_PORTP_BASE, GPIO_PIN_0);                         // Allow request output from Port unit
    IntEnable(INT_GPIOP0);                                              // Allow request input to NVIC

    // Configure Timer0 Interrupt
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC_UP);
    TimerEnable(TIMER0_BASE, TIMER_A);

    // Configure Timer1 Interrupt
    TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER1_BASE, TIMER_A, sysClock / 10);      // fires every 50 ms
    TimerIntRegister(TIMER1_BASE, TIMER_A, Timer1_DisplayIntHandler);
    IntEnable(INT_TIMER1A);
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    TimerEnable(TIMER1_BASE, TIMER_A);

    // Configure Timer2 Interrupt
    TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER2_BASE, TIMER_A, sysClock / 5);      // fires every 200 ms
    TimerIntRegister(TIMER2_BASE, TIMER_A, Timerout_Cal_IntHandler);
    //IntPrioritySet(INT_TIMER2A, 0x20);

    IntEnable(INT_TIMER2A);
    TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
    TimerEnable(TIMER2_BASE, TIMER_A);
}

void main(void)
{
    IntMasterDisable();        // as matter of principle
    // Set system frequency to 120 MHz
    sysClock = SysCtlClockFreqSet(SYSCTL_OSC_INT | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480, 120000000);
    init_peripherals();
    init_and_config_display();
    display_layout();
    init_and_config_sensor();

    IntMasterEnable();
    while (1);
}

