//*****************************************************************************
// Name      : lab4.c
// Date	    : February 10, 2015
// Class     : CEC322, Microprocessor Systems Lab
// Authors   : Renan Vieira & Jared Johnson
// Descript. : This program implements some functions to the board DK-TM4C123G
//			  First of all, a menu of options is displayed to a terminal software
//			  which has the follow functions:
//			  1: Splash - display for a couple of seconds the author's names
//			  2: Toggle Count - enable/disable displaying Count variable on OLED
//			  - The while loop inside the main runs blinky code;
//			  - The Timer0IntHandler interrupt runs at 1 Hz and set the frequency
//			  for the Timer1IntHandler;
//			  Timer1IntHandler runs in a variable frequency.
//*****************************************************************************

/* libraries */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/rom.h"
#include "grlib/grlib.h"
#include "drivers/cfal96x64x16.h"
#include "utils/uartstdio.h"
#include "driverlib/uart.h"
#include "driverlib/gpio.h"
#include "driverlib/adc.h"

/* constants */
#define LED_ON  1000  //amount of time in which the LED will be ON
#define LED_OFF 80000 //amount of time in which the LED will be OFF

/* global */
bool COUNT = true;    //logic which control the toggle display of counter variable
int gSecCounter = 0;      //counts how many times the the Server interrupt executes
int32_t FREQUENCY = 1;//frequency control of the served interrupt
int32_t SRV = 1;      //stores the amount of cycles served
int32_t REQ = 1;      //stores the amount of cycles requested

//*****************************************************************************
// Flags that contain the current value of the interrupt indicator as displayed
// on the CSTN display.
//*****************************************************************************
uint32_t g_ui32Flags;

//*****************************************************************************
// Graphics context used to show text on the CSTN display.
//*****************************************************************************
tContext g_sContext;

//*****************************************************************************
// The error routine that is called if the driver library encounters an error.
//*****************************************************************************
#ifdef DEBUG
void __error__(char *pcFilename, uint32_t ui32Line) { }
#endif

//*****************************************************************************
// The UART interrupt handler.
//*****************************************************************************
void
UARTIntHandler(void)
{
    uint32_t ui32Status;
    // Get the interrupt status.
    ui32Status = ROM_UARTIntStatus(UART0_BASE, true);
    // Clear the asserted interrupts.
    ROM_UARTIntClear(UART0_BASE, ui32Status);
    // Loop while there are characters in the receive FIFO.
    while(ROM_UARTCharsAvail(UART0_BASE))
    {
        // Read the next character from the UART and write it back to the UART.
        ROM_UARTCharPutNonBlocking(UART0_BASE,
                                   ROM_UARTCharGetNonBlocking(UART0_BASE));
    }
}

//*****************************************************************************
// Clears the screen given the y range position
//*****************************************************************************
void clear_bottom(int y_min, int y_max)
{
    tRectangle sRect;
    sRect.i16XMin = 0;
    sRect.i16YMin = y_min;
    sRect.i16XMax = GrContextDpyWidthGet(&g_sContext) - 1;
    sRect.i16YMax = y_max;
    GrContextForegroundSet(&g_sContext, ClrBlack);
    GrRectFill(&g_sContext, &sRect);
    GrContextForegroundSet(&g_sContext, ClrWhite);
}

//*****************************************************************************
// Toggle the COUNT global variable (based on the UART command)
//*****************************************************************************
/*
void toggle_count()
{
    if(COUNT)
    {
	COUNT = false;
        clear_bottom(46,56);
    }
    else
        COUNT = true;
}
*/
//*****************************************************************************
// Unique graphical splash from this group (commanded by the UART)
// Print the author's names and the current frequency
//*****************************************************************************
void splash(tContext sContext)
{
    // Draw a red screen, the author's names, and the frequency on the OLED
    tRectangle sRect;
    char temp[40];
    GrContextFontSet(&sContext, g_psFontFixed6x8);
    sRect.i16XMin = 0;
    sRect.i16YMin = 0;
    sRect.i16XMax = GrContextDpyWidthGet(&sContext) - 1;
    sRect.i16YMax = 63;
    GrContextForegroundSet(&sContext, ClrRed);
    GrRectFill(&sContext, &sRect);
    GrContextForegroundSet(&sContext, ClrBlue);
    GrStringDrawCentered(&sContext, "Renan & Jared", -1,
                         GrContextDpyWidthGet(&sContext) / 2, 20, false);
    sprintf(temp, "clock=%d", SysCtlClockGet());
    GrStringDrawCentered(&sContext, temp , -1,
                         GrContextDpyWidthGet(&sContext) / 2, 40, false);
    // Delay in order to wait the screen drawn
    SysCtlDelay(SysCtlClockGet());
    // Draw the standard screen 
    sRect.i16XMin = 0;
    sRect.i16YMin = 0;
    sRect.i16XMax = GrContextDpyWidthGet(&sContext) - 1;
    sRect.i16YMax = 63;
    GrContextForegroundSet(&sContext, ClrBlack);
    GrRectFill(&sContext, &sRect);
    GrContextForegroundSet(&sContext, ClrWhite);
    // Fill the top part of the screen with blue to create the banner.
    sRect.i16XMin = 0;
    sRect.i16YMin = 0;
    sRect.i16XMax = GrContextDpyWidthGet(&g_sContext) - 1;
    sRect.i16YMax = 9;
    GrContextForegroundSet(&g_sContext, ClrDarkBlue);
    GrRectFill(&g_sContext, &sRect);
    GrContextForegroundSet(&g_sContext, ClrWhite);
    GrStringDrawCentered(&g_sContext, "assembly", -1,
                         GrContextDpyWidthGet(&g_sContext) / 2, 4, 0);
}

//*****************************************************************************
// The interrupt handler for the first timer interrupt.
// It runs at 1 Hz. The functionalities are:
// Reads the UART; Reads the ADC; update the OLED screen
//*****************************************************************************
void Timer0IntHandler(void)
{
    uint32_t ui32Value[8]; //data from the ADC reading
    int32_t data;          //data from the UART reading
    char temp[40];         //temporary buffer for conversion
	
    // Clears the timer interrupt.
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    // Toggle the flag for the first timer.
    HWREGBITW(&g_ui32Flags, 0) ^= 1;
    // Disables the processor interrupt
//    ROM_IntMasterDisable();
    // Reading data from the terminal: Menu Options
    if(UARTCharsAvail(UART0_BASE))
    {
	data = UARTCharGetNonBlocking(UART0_BASE);
	if(data == '1')
	{
             splash(g_sContext);
	}
        /*
	if(data == '2')
	{
            toggle_count();
	}
        */
    }
    // Gets the current interrupt status - Verify weather data is available
    if(ADCIntStatus(ADC0_BASE, 3, false))
    {
     	// Clears sample sequence interrupt source
        ADCIntClear(ADC0_BASE, 3);
	// Gets the captured data for a sample sequence
        ADCSequenceDataGet(ADC0_BASE, 3, ui32Value);
	// Sets the frequency to the value read times a constant
        FREQUENCY = (ui32Value[0] + 1)*1000;
	// Sets the request (REQ) frequency	
        REQ = ROM_SysCtlClockGet() / FREQUENCY;
	// Update the served (SRV) based on counter from Timer1IntHandler
        SRV = gSecCounter;
	// Reinitialize the counter variable
        gSecCounter = 0;
	// Sets the timer load value
        ROM_TimerLoadSet(TIMER1_BASE, TIMER_A, REQ);   
    }
    // Causes a processor trigger for a sample sequence
    ADCProcessorTrigger(ADC0_BASE, 3);
    // Update the OLED screen with the new variables
    clear_bottom(16,44);
    GrStringDraw(&g_sContext, "ADC: ", -1, 16, 16, 0);
    GrStringDraw(&g_sContext, "SRV: ", -1, 16, 26, 0);
    GrStringDraw(&g_sContext, "REQ: ", -1, 16, 36, 0);
//    GrStringDraw(&g_sContext, "PER: ", -1, 16, 46, 0);
    sprintf(temp, "%d", ui32Value[0]);
    GrStringDraw(&g_sContext, temp, -1, 48, 16, 1);
    sprintf(temp, "%d", SRV);
    GrStringDraw(&g_sContext, temp, -1, 48, 26, 1);
    sprintf(temp, "%d", FREQUENCY);
    GrStringDraw(&g_sContext, temp, -1, 48, 36, 1);
    //sprintf(temp, "%f", 1./FREQUENCY);
    //GrStringDraw(&g_sContext, temp, -1, 48, 46, 1);
    // Enables the processor interrupt
//    ROM_IntMasterEnable();
}

//*****************************************************************************
// The interrupt handler for the second timer interrupt.
//*****************************************************************************
///*
void Timer1IntHandler(void)
{
    ROM_TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    gSecCounter ++;
}
//*/

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
// Initialization of ADC Peripheral
//*****************************************************************************
void Init_ADC()
{
    // Enables ADC peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0); 
    SysCtlDelay(SysCtlClockGet() / 12); // small delay
    // Disables a sample sequence
    ADCSequenceDisable(ADC0_BASE,  3);  // Sequence Number
    // Configures the trigger source and priority of a sample sequence
    ADCSequenceConfigure(ADC0_BASE,     // ADC0
			           3,  // Sequence Number
			           ADC_TRIGGER_PROCESSOR, // Trigger
			           0); // Priority
    // Configure a step of the sample sequencer
    ADCSequenceStepConfigure(ADC0_BASE,
                                    3, // Sequence
		                   0, // Step
		                   ADC_CTL_CH4 | ADC_CTL_IE | ADC_CTL_END);
    // Enables PORT D peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    // Configures pin(s) for use as ADC inputs
    GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_7);
    //Enables a sample sequence
    ADCSequenceEnable(ADC0_BASE, 3);
    //Clears sample sequence interrupt source
    ADCIntClear(ADC0_BASE, 3);
}

//*****************************************************************************
// This application implements the use of the timers to generate one fixed (1Hz) 
// and one variable interrupt (based on ADC reading)
//*****************************************************************************
int main(void)
{
    tRectangle sRect;
    // Enable lazy stacking for interrupt handlers
    ROM_FPULazyStackingEnable();
    // Set the clocking to run directly from the crystal: (choose only one: 16 or 66)
	
    // 16 MHz - through PLL
    //ROM_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    // 66 MHz - through PLL
    SysCtlClockSet(SYSCTL_SYSDIV_3 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
	
    // Disables the processor interrupt for draw graphics, initialize peripherals, etc
    ROM_IntMasterDisable();
    // Initialize the display driver.
    CFAL96x64x16Init();
    // Initialize the graphics context and find the middle X coordinate.
    GrContextInit(&g_sContext, &g_sCFAL96x64x16);
    // Draws the author's splash on the OLED
    splash(g_sContext);
    // Fill the top part of the screen with blue to create the banner.
    sRect.i16XMin = 0;
    sRect.i16YMin = 0;
    sRect.i16XMax = GrContextDpyWidthGet(&g_sContext) - 1;
    sRect.i16YMax = 9;
    GrContextForegroundSet(&g_sContext, ClrDarkBlue);
    GrRectFill(&g_sContext, &sRect);
    // Change foreground for white text.
    GrContextForegroundSet(&g_sContext, ClrWhite);
    // Put the application name in the middle of the banner.
    GrContextFontSet(&g_sContext, g_psFontFixed6x8);
    GrStringDrawCentered(&g_sContext, "assembly", -1,
                         GrContextDpyWidthGet(&g_sContext) / 2, 4, 0);
    // Initialize timer status display.
    GrContextFontSet(&g_sContext, g_psFontFixed6x8);
    // Draws the REQ and SRV variable on the OLED
    GrStringDraw(&g_sContext, "SRV: ", -1, 16, 26, 0);
    GrStringDraw(&g_sContext, "REQ: ", -1, 16, 36, 0);
    // Enable the peripherals used by this example.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    // Enable the peripherals used by this program.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);    // UART
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);    // UART
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);    // LED
    GPIOPinTypeGPIOOutput(GPIO_PORTG_BASE, GPIO_PIN_2); // LED
    // Set GPIO A0 and A1 as UART pins.
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    // Configure the UART for 115,200, 8-N-1 operation.
    ROM_UARTConfigSetExpClk(UART0_BASE, ROM_SysCtlClockGet(), 115200,
                             (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));
    // Prompt for text to be entered. Sends menu to terminal
    UARTSend((uint8_t *)"Menu Selection : \n\r", 19);
    UARTSend((uint8_t *)"1 - Splash: \n\r", 14);
//    UARTSend((uint8_t *)"2 - Toggle Count: \n\r", 20);
    // Initialize ADC Peripheral
    Init_ADC();
    // Causes a processor trigger for a sample sequence
    ADCProcessorTrigger(ADC0_BASE, 3);
    // Configure the two 32-bit periodic timers (initialization)
    ROM_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    ROM_TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
    ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, ROM_SysCtlClockGet());
    ROM_TimerLoadSet(TIMER1_BASE, TIMER_A, ROM_SysCtlClockGet() / 2);
    // Setup the interrupts for the timer timeouts
    ROM_IntEnable(INT_TIMER0A);
    ROM_IntEnable(INT_TIMER1A);
    ROM_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    ROM_TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    // Enable the timers.
    ROM_TimerEnable(TIMER0_BASE, TIMER_A);
    ROM_TimerEnable(TIMER1_BASE, TIMER_A);
    // Enable processor interrupts.
    ROM_IntMasterEnable();
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