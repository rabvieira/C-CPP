//*****************************************************************************
//
// timers.c - Timers example.
//
// Copyright (c) 2011-2014 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 2.1.0.12573 of the DK-TM4C123G Firmware Package.
//
//*****************************************************************************

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
#define LED_ON  100000
#define LED_OFF 8000000
bool COUNT = false;
int counter = 0;
int FREQUENCY = 2;
int SRV = 1;
int REQ = 1;

//*****************************************************************************
//
//! \addtogroup example_list
//! <h1>Timer (timers)</h1>
//!
//! This example application demonstrates the use of the timers to generate
//! periodic interrupts.  One timer is set up to interrupt once per second and
//! the other to interrupt twice per second; each interrupt handler will toggle
//! its own indicator on the display.
//
//*****************************************************************************

//*****************************************************************************
//
// Flags that contain the current value of the interrupt indicator as displayed
// on the CSTN display.
//
//*****************************************************************************
uint32_t g_ui32Flags;

//*****************************************************************************
//
// Graphics context used to show text on the CSTN display.
//
//*****************************************************************************
tContext g_sContext;

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

void splash(tContext sContext)
{
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
    
    SysCtlDelay(SysCtlClockGet()); //delay

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
}

void clear_bottom(int y_min, int y_max)
{
    tRectangle sRect;
    //Clear the bottom of screen of the OLED
    sRect.i16XMin = 0;
    sRect.i16YMin = y_min;
    sRect.i16XMax = GrContextDpyWidthGet(&g_sContext) - 1;
    sRect.i16YMax = y_max;
    GrContextForegroundSet(&g_sContext, ClrBlack);
    GrRectFill(&g_sContext, &sRect);
    GrContextForegroundSet(&g_sContext, ClrWhite);
  
}

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

//*****************************************************************************
//
// The interrupt handler for the first timer interrupt.
//
//*****************************************************************************
void //one per sec
Timer0IntHandler(void)
{
    uint32_t ui32Value[8];
    int32_t data;
    char temp[40];
    //
    // Clear the timer interrupt.
    //
    ROM_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    //
    // Toggle the flag for the first timer.
    //
    HWREGBITW(&g_ui32Flags, 0) ^= 1;

    //
    // Update the interrupt status on the display.
    //
    ROM_IntMasterDisable();    
    // Reading data from the terminal: Menu Options
    if(UARTCharsAvail(UART0_BASE))
    {
        data = UARTCharGetNonBlocking(UART0_BASE);
	if(data == '1')
        {
          splash(g_sContext);
	}
	if(data == '2')
        {
          toggle_count();
	}
    }
    ADCProcessorTrigger(ADC0_BASE, 3);
    ADCIntClear(ADC0_BASE, 3);
    //while(!ADCIntStatus(ADC0_BASE, 3, false)) {}
    if(ADCIntStatus(ADC0_BASE, 3, false))
        ADCSequenceDataGet(ADC0_BASE, 3, ui32Value);
    FREQUENCY = ui32Value[0];
    REQ = ROM_SysCtlClockGet() / (FREQUENCY + 1);
    //SRV = counter - SRV;
    SRV = ui32Value[0];
    
    //SysCtlPeripheralDisable(SYSCTL_PERIPH_TIMER1);
    ROM_TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
    ROM_TimerLoadSet(TIMER1_BASE, TIMER_A, REQ);
    //SysCtlDelay(SysCtlClockGet() / 12); //delay
    //ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    sprintf(temp, "%d", SRV);
    //GrStringDraw(&g_sContext, (HWREGBITW(&g_ui32Flags, 0) ? "1" : "0"), -1, 68, 26, 1);
    GrStringDraw(&g_sContext, temp, -1, 68, 26, 1);
    sprintf(temp, "%d", REQ);
    GrStringDraw(&g_sContext, temp, -1, 68, 36, 1);
    
    ROM_IntMasterEnable();
}

//*****************************************************************************
//
// The interrupt handler for the second timer interrupt.
//
//*****************************************************************************
void
Timer1IntHandler(void)
{
    char temp[40];
    //
    // Clear the timer interrupt.
    //
    ROM_TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

    //
    // Toggle the flag for the second timer.
    //
    HWREGBITW(&g_ui32Flags, 1) ^= 1;

    //
    // Update the interrupt status on the display.
    //
    ROM_IntMasterDisable();
    //GrStringDraw(&g_sContext, (HWREGBITW(&g_ui32Flags, 1) ? "1" : "0"), -1, 68, 36, 1);
    if(COUNT)
    {
      sprintf(temp, "Count: %d", counter);
      clear_bottom(46,56);
      GrStringDraw(&g_sContext, temp, -1, 16, 46, 0);
    }
    
    counter ++;
    ROM_IntMasterEnable();
    
}

//*****************************************************************************
// The UART interrupt handler.
//*****************************************************************************
void
UARTIntHandler(void)
{
    uint32_t ui32Status;
    // Get the interrrupt status.
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
// Send a string to the UART.
//*****************************************************************************
void
UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count)
{
    // Loop while there are more characters to send.
    while(ui32Count--)
    {
      // Write the next character to the UART.
      UARTCharPut(UART0_BASE, *pui8Buffer++);
    }
}

void Init_ADC()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0); 
    SysCtlDelay(SysCtlClockGet() / 12); //delay
    ADCSequenceDisable(ADC0_BASE,  3); // Sequence Num
    ADCSequenceConfigure(ADC0_BASE, // ADC0
			 3, // Sequence Num
			 ADC_TRIGGER_PROCESSOR, // Trigger
			 0); // Priority
    ADCSequenceStepConfigure(ADC0_BASE,
		 3, // Sequence
		 0, // Step
		 ADC_CTL_CH4 | ADC_CTL_IE | ADC_CTL_END);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_7);
    ADCSequenceEnable(ADC0_BASE, 0);
    ADCIntClear(ADC0_BASE, 0);
}

//*****************************************************************************
//
// This example application demonstrates the use of the timers to generate
// periodic interrupts.
//
//*****************************************************************************
int
main(void)
{
    tRectangle sRect;
    //
    // Enable lazy stacking for interrupt handlers.  This allows floating-point
    // instructions to be used within interrupt handlers, but at the expense of
    // extra stack usage.
    //
    ROM_FPULazyStackingEnable();

    //
    // Set the clocking to run directly from the crystal.
    //
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);
    
        ROM_IntMasterDisable();

    //
    // Initialize the display driver.
    //
    CFAL96x64x16Init();

    //
    // Initialize the graphics context and find the middle X coordinate.
    //
    GrContextInit(&g_sContext, &g_sCFAL96x64x16);
    splash(g_sContext);
    //
    // Fill the top part of the screen with blue to create the banner.
    //
    sRect.i16XMin = 0;
    sRect.i16YMin = 0;
    sRect.i16XMax = GrContextDpyWidthGet(&g_sContext) - 1;
    sRect.i16YMax = 9;
    GrContextForegroundSet(&g_sContext, ClrDarkBlue);
    GrRectFill(&g_sContext, &sRect);

    //
    // Change foreground for white text.
    //
    GrContextForegroundSet(&g_sContext, ClrWhite);

    //
    // Put the application name in the middle of the banner.
    //
    GrContextFontSet(&g_sContext, g_psFontFixed6x8);
    GrStringDrawCentered(&g_sContext, "timers", -1,
                         GrContextDpyWidthGet(&g_sContext) / 2, 4, 0);

    //
    // Initialize timer status display.
    //
    GrContextFontSet(&g_sContext, g_psFontFixed6x8);
    GrStringDraw(&g_sContext, "SRV:", -1, 16, 26, 0);
    GrStringDraw(&g_sContext, "REQ:", -1, 16, 36, 0);

    //
    // Enable the peripherals used by this example.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);



    //
    // Configure the two 32-bit periodic timers.
    //
    ROM_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    ROM_TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
    ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, ROM_SysCtlClockGet());
    ROM_TimerLoadSet(TIMER1_BASE, TIMER_A, ROM_SysCtlClockGet() / (FREQUENCY + 1));

    //
    // Setup the interrupts for the timer timeouts.
    //
    ROM_IntEnable(INT_TIMER0A);
    ROM_IntEnable(INT_TIMER1A);
    ROM_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    ROM_TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

    //
    // Enable the timers.
    //
    ROM_TimerEnable(TIMER0_BASE, TIMER_A);
    ROM_TimerEnable(TIMER1_BASE, TIMER_A);

    // Enable the peripherals used by this program.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    // Set GPIO A0 and A1 as UART pins.
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    // Configure the UART for 115,200, 8-N-1 operation.
    ROM_UARTConfigSetExpClk(UART0_BASE, ROM_SysCtlClockGet(), 115200,
                             (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));
    // Prompt for text to be entered. Sends thius menu to terminal
    UARTSend((uint8_t *)"Menu Selection : \n\r", 20);
    UARTSend((uint8_t *)"1 - Splash: \n\r", 14);
    UARTSend((uint8_t *)"2 - Toggle Count: \n\r", 21);
    
    Init_ADC();
    
    //
    // Enable processor interrupts.
    //
    ROM_IntMasterEnable();
    

    
    while(1)
    {
	volatile uint32_t ui32Loop;
	GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2, GPIO_HIGH_LEVEL);
	for(ui32Loop = 0; ui32Loop < LED_ON; ui32Loop++) {}
	GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2, GPIO_LOW_LEVEL);
	for(ui32Loop = 0; ui32Loop < LED_OFF; ui32Loop++) {}      
    }
}