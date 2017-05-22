//*****************************************************************************
// Name      : lab9.c
// Date	     : April 7, 2015
// Class     : CEC322, Microprocessor Systems Lab
// Authors   : Renan Vieira & Jared Johnson
// Descript. : This program implements some functions to the board DK-TM4C123G
//			   First of all, a menu of options is displayed to a terminal software
//			   which has the follow functions:
//			   0: Splash - display for a couple of seconds the author's names
//			               and clock frequency into the OLED.
//			   1: Beethoven    - uses the array of frequencies defined at songs.h
//			      to play Ode to Joy using PWM peripheral.
//			   2: Harry Potter - the same approach to play Hedwig's Theme
//*****************************************************************************

/* libraries */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/rom.h"
#include "driverlib/pin_map.h"
#include "driverlib/timer.h"
#include "grlib/grlib.h"
#include "drivers/cfal96x64x16.h"
#include "driverlib/pwm.h"
#include "songs.h"

/* constants */
#define LED_ON  1000  //amount of time in which the LED will be ON
#define LED_OFF 80000 //amount of time in which the LED will be OFF

/* global */
float Frequency = (90. / 60.); //sets the frequency of the interrupt
int Music = 1;	//keeps track of which music is being played
int Note = 0;   //runs the music's array
tContext g_sContext; // Graphics context used to show text on the CSTN display.

//*****************************************************************************
// The error routine that is called if the driver library encounters an error.
//*****************************************************************************
#ifdef DEBUG
	void __error__(char *pcFilename, uint32_t ui32Line) {}
#endif

//*****************************************************************************
// Send a string to the UART.
//*****************************************************************************
void UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count)
{
    // Loop while there are more characters to send.
    while(ui32Count--)
    {
        // Write the next character to the UART.
        UARTCharPut(UART0_BASE, *pui8Buffer++);
    }
}

//*****************************************************************************
// Unique graphical splash from this group (commanded by the UART)
// Print the author's names and the current frequency
//*****************************************************************************
void splash()
{
    // Draw a red screen, the author's names, and the frequency on the OLED
    tRectangle sRect;
    char temp[40];
    GrContextFontSet(&g_sContext, g_psFontFixed6x8);
    sRect.i16XMin = 0;
    sRect.i16YMin = 0;
    sRect.i16XMax = GrContextDpyWidthGet(&g_sContext) - 1;
    sRect.i16YMax = 63;
    GrContextForegroundSet(&g_sContext, ClrRed);
    GrRectFill(&g_sContext, &sRect);
    GrContextForegroundSet(&g_sContext, ClrBlue);
    GrStringDrawCentered(&g_sContext, "Renan & Jared", -1,
                         GrContextDpyWidthGet(&g_sContext) / 2, 20, false);
    sprintf(temp, "clock=%d", SysCtlClockGet());
    GrStringDrawCentered(&g_sContext, temp , -1,
                         GrContextDpyWidthGet(&g_sContext) / 2, 40, false);
						 
    // Delay in order to wait the screen drawn
    SysCtlDelay(SysCtlClockGet());
	
    // Draw the standard screen 
    sRect.i16XMin = 0;
    sRect.i16YMin = 0;
    sRect.i16XMax = GrContextDpyWidthGet(&g_sContext) - 1;
    sRect.i16YMax = 63;
    GrContextForegroundSet(&g_sContext, ClrBlack);
    GrRectFill(&g_sContext, &sRect);
    GrContextForegroundSet(&g_sContext, ClrWhite);
	
    // Fill the top part of the screen with blue to create the banner.
    sRect.i16XMin = 0;
    sRect.i16YMin = 0;
    sRect.i16XMax = GrContextDpyWidthGet(&g_sContext) - 1;
    sRect.i16YMax = 9;
    GrContextForegroundSet(&g_sContext, ClrDarkBlue);
    GrRectFill(&g_sContext, &sRect);
    GrContextForegroundSet(&g_sContext, ClrWhite);
    GrStringDrawCentered(&g_sContext, "PWM", -1,
                         GrContextDpyWidthGet(&g_sContext) / 2, 4, 0);
}

//*****************************************************************************
// The UART interrupt handler: selects the music and tempo based on UART
//*****************************************************************************
void UARTIntHandler(void)
{
    int32_t data; //data from the UART reading
    uint32_t ui32Status; // UARTS's status
	
    // Get the interrupt status
    ui32Status = ROM_UARTIntStatus(UART0_BASE, false);
    // Clear the asserted interrupts
    ROM_UARTIntClear(UART0_BASE, ui32Status);
    // Receives a character from the specified port
    data = UARTCharGetNonBlocking(UART0_BASE);
	
    // Reading data from the terminal: Menu Options
    switch(data)
    {
		case '0':
			splash();
        break;
        case '1':
            Music = 1;
            Frequency = (90. / 60.);
            Note = 0;
        break;
        case '2':
            Music = 2;
            Frequency = 6*(58. / 60.);
            Note = 0;
        break;
    }
	
	// Sets the timer load value
    ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, (int)(ROM_SysCtlClockGet() / Frequency));
}

//*****************************************************************************
// The Timer interrupt handler: sets the period of the music and outputs 
// a PWM frequency based on the music's array
//*****************************************************************************
void Timer0IntHandler(void)
{
    float period;
	// Clears timer interrupt sources
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	
	// Set the period based on the music chosen
    if(Music == 1)
    {
		period = (1. / beethoven9[Note]) * ROM_SysCtlClockGet();
    }
    if(Music == 2)
    {
		period = (1. / harryPotter[Note]) * ROM_SysCtlClockGet();
    }
	
	// Sets the period of a PWM generator
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, period);
	// Sets the pulse width for the specified PWM output
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, period/2);
	// Enables or disables PWM outputs
    PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT, true);
	// Enables the timer/counter for a PWM generator block
    PWMGenEnable(PWM0_BASE, PWM_GEN_0);
	
	// Stops the music based on the the last note (-1)
    if(Music == 1 && beethoven9[Note] == -1)
    {
		// Disables the timer/counter for a PWM generator block
		PWMGenDisable(PWM0_BASE, PWM_GEN_0);
		Note--;
    }
    if(Music == 2 && harryPotter[Note] == -1)
    {
		// Disables the timer/counter for a PWM generator block
		PWMGenDisable(PWM0_BASE, PWM_GEN_0);
		Note--;
    }
	
	// Increments the position inside the music's array
    Note ++;
}

//*****************************************************************************
// The main configure the peripherals used in this program and runs a blinky 
// code in background.
//*****************************************************************************
int main(void)
{
    // Enable lazy stacking for interrupt handlers.
    ROM_FPULazyStackingEnable();
    // Set the clocking to run directly from the crystal.
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);
    // Disables the processor interrupt for draw graphics, initialize peripherals, etc
    ROM_IntMasterDisable();
    // Initialize the display driver.
    CFAL96x64x16Init();
    // Initialize the graphics context.
    GrContextInit(&g_sContext, &g_sCFAL96x64x16);
	// Draws the author's splash on the OLED
    splash();
	
    // Enable the peripherals used by this example.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);   // Timer
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);    // UART
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);    // UART 
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);    // LED
    GPIOPinTypeGPIOOutput(GPIO_PORTG_BASE, GPIO_PIN_2); // LED
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);         // PWM
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);		// PWM
	
    // Set GPIO A0 and A1 as UART pins.
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    // Configure the UART for 115,200, 8-N-1 operation.
    ROM_UARTConfigSetExpClk(UART0_BASE, ROM_SysCtlClockGet(), 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));
							 
    // Prompt for text to be entered. Sends menu to terminal
    UARTSend((uint8_t *)"Menu Selection : \n\r", 19);
    UARTSend((uint8_t *)"0 - Splash: \n\r", 14);
    UARTSend((uint8_t *)"1 - Beethoven: \n\r", 17);
    UARTSend((uint8_t *)"2 - Harry Potter: \n\r", 20);
    
    // Enable the UART interrupt.
    ROM_IntEnable(INT_UART0);
    ROM_UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);

	// Sets the PWM clock configuration
    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
	// Configures the alternate function of a GPIO pin
    GPIOPinConfigure(GPIO_PH0_M0PWM0);
	// Configures pin(s) for use by the PWM peripheral
    GPIOPinTypePWM(GPIO_PORTH_BASE, GPIO_PIN_0);
	// Configures a PWM generator
    PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
    
    // Enable processor interrupts.
    ROM_IntMasterEnable();
    // Configure the two 32-bit periodic timers (initialization)
    ROM_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    //ROM_TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
    ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, (int)(ROM_SysCtlClockGet() / Frequency));
    // Setup the interrupts for the timer timeouts
    ROM_IntEnable(INT_TIMER0A);
    //ROM_IntEnable(INT_TIMER1A);
    ROM_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    // Enable the timers.
    ROM_TimerEnable(TIMER0_BASE, TIMER_A);
    
    // Runs the blinky code indefinably (LED ON, delay; LED OFF, delay)
    while(1)
    {
		volatile uint32_t ui32Loop;
		GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2, GPIO_HIGH_LEVEL);
		for(ui32Loop = 0; ui32Loop < LED_ON; ui32Loop++) {}
		GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2, GPIO_LOW_LEVEL);
		for(ui32Loop = 0; ui32Loop < LED_OFF; ui32Loop++) {}
    }
}

