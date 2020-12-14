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
void Timer1_DisplayIntHandler(void);
void wait(int time);

uint32_t sysClock, timerScaler;
uint32_t geschwindigkeit = 0;

void Timer1_DisplayIntHandler(void)
{
    static uint16_t radius = 160;
    static uint16_t phinull = 150;
    static uint16_t phi;
    static uint16_t x, y;
    static uint16_t x_old = X_CENTER, y_old = Y_CENTER;

    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    char buffer[20];
    sprintf(buffer, "%3d km/h", geschwindigkeit);
    print_string1216(buffer, 400, 245, COLOR_BLACK, COLOR_YELLO);

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

    speed++;

    printf("%d\n",speed);

}

void Timerout_Cal_IntHandler(void)
{
    TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
    geschwindigkeit = speed;
    speed = 0;
}

void main(void)
{
    IntMasterDisable();        // as matter of principle
    // Set system frequency to 120 MHz
    sysClock = SysCtlClockFreqSet(SYSCTL_OSC_INT | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480, 120000000);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);    // Clock Gate enable TIMER0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);    // Clock Gate enable TIMER1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);    // Clock Gate enable TIMER2
    SysCtlDelay(10);

    speed = 0;
    counter = 0;

    init_and_config_display();
    init_and_config_sensor();
    // dis play complett number 0-240 and circle
    display_layout();
    Timer1_DisplayIntHandler();

    // Test
    //timerLCD();
    // Test Ende

    /*
    for (i = 0; i <= 240; i++)
    {
        geschwindigkeit = i;
        //wait(10);
        Timer1_DisplayIntHandler();
    }

    for (i = 240; i >= 0; i--)
    {
        geschwindigkeit = i;
        //wait(2);
        Timer1_DisplayIntHandler();
    }
    */


    //periphery clock enable
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOP);
    //pin setup
    GPIOPinTypeGPIOInput(GPIO_PORTP_BASE, GPIO_PIN_0);
    // Rising edge type interrupt
    GPIOIntTypeSet(GPIO_PORTP_BASE, GPIO_PIN_0, GPIO_RISING_EDGE);
    // "register" entry in  a copied IVT
    GPIOIntRegister(GPIO_PORTP_BASE, Count_IntHandler);
    GPIOIntClear(GPIO_PORTP_BASE, GPIO_PIN_0); // optional ...
    IntPrioritySet(INT_GPIOP0, 0x20); //high prio
    GPIOIntEnable(GPIO_PORTP_BASE, GPIO_PIN_0); // Allow request output from Port unit
    IntEnable(INT_GPIOP0);  // Allow request input to NVIC

    // Configure Timer0 Interrupt
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC_UP);
    TimerEnable(TIMER0_BASE, TIMER_A);

    // Configure Timer1 Interrupt
    TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER1_BASE, TIMER_A, sysClock / 20);      // fires every 50 ms
    TimerIntRegister(TIMER1_BASE, TIMER_A, Timer1_DisplayIntHandler);
    IntEnable(INT_TIMER1A);
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    TimerEnable(TIMER1_BASE, TIMER_A);

    // Configure Timer2 Interrupt
    TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER2_BASE, TIMER_A, sysClock / 2);      // fires every 500 ms
    TimerIntRegister(TIMER2_BASE, TIMER_A, Timerout_Cal_IntHandler);
    //IntPrioritySet(INT_TIMER2A, 0x80);

    IntEnable(INT_TIMER2A);
    TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
    TimerEnable(TIMER2_BASE, TIMER_A);

    IntMasterEnable();
    while (1)
       {
        //Test Motor and Display
        //printf("Write rectangles\n"); // for debug only
        // GPIO_PORTN_DATA_R = GPIO_PORTP_DATA_R & 0x03;

       }
}

