/*
 * @file        : init.c
 * @author      : Minh Tung Bui and Hauke Kosmiter
 * @copyright   : HAW-Hamburg
 *
 * In this file there are the functions for initiate the peripherals and variables
 */
#include <component/Initiation/init.h>

void init_peripherals(void)
{
    // CLOCK GATES ENABLE
    /********************************************************************************/
    int i = 0;
    //Configurations of Port Pin as interrupts source
    SYSCTL_RCGCGPIO_R = 0x2000; // switch clock on for Port P

    i++; // wait for clock stable at periphery
    GPIO_PORTP_DEN_R |= 0x03;       // enable P0 & P1
    GPIO_PORTP_DIR_R &= ~0x03;      // input P0 & P1
    GPIO_PORTP_IS_R &= ~0x03;       // both pins generates edge sensitive requests
    GPIO_PORTP_IBE_R &= ~0x03;      // not use both edges
    GPIO_PORTP_IEV_R |= 0x03;       // interrupt events are rising edges
    GPIO_PORTP_ICR_R |= 0x03;       // clear interrupt requests
    GPIO_PORTP_IM_R |= 0x03;        // unmask P0 and P1 as I Sources
    GPIO_PORTP_SI_R |= 0x01;        // speciality of Port P and Q
    // Here set P0 and P1 as seperate source <===== Each pin has its own interrupt vector. page 750 and 791 manual
    NVIC_EN2_R |= 0x3000; // enable Port P I Requests in the NVIC

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
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);                       // Clock Gate enable TIMER1
    TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER1_BASE, TIMER_A, sysCLK / 2.5);                   // fires every 200 ms
    TimerIntRegister(TIMER1_BASE, TIMER_A, Timer1_DisplayIntHandler);
    IntEnable(INT_TIMER1A);
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

}

void init_var(void){
    // Variables for interrupt handler
    S1counter = 0;
    S2counter = 0;

    // Variables for calculation in the timer function
    veclocity_tacho = 0;
    distance_in_m = 0;
    distance_in_km = 0;

    // Toggle bool for direction determining
    toggle = true;
}

