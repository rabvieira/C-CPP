//*****************************************************************************
// Name      : lab2.c
// Date	     : January 20, 2015
// Class     : CEC322, Microprocessor Systems Lab
// Authors   : Renan Vieira & Jared Johnson
// Descript. : This program implements some functions to the board DK-TM4C123G
//			   First of all, a menu of options is displayed to a terminal software
//			   which has the follow functions:
//			   1: turns on the LED on the board
//			   2: turns off the LED on the board
//			   3: blinks the LED on the board
//			   4: flood a character (^) in the terminal
//			   5: enable/disable the button information on the OLED display
//*****************************************************************************

/* libraries */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pge.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/rom.h"
#include "driverlib/pin_map.h"
#include "grlib/grlib.h"
#include "drivers/cfal96x64x16.h"
#include "drivers/buttons.h"
#include "utils/uartstdio.h"

/* constants */
#define LED_ON  1000
#define LED_OFF 80000

//*****************************************************************************
// The error routine that is called if the driver library encounters an error.
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

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

//*****************************************************************************
// Turn the board's LED on
//*****************************************************************************
void led_on()
{
	UARTSend((uint8_t *)"ON\n\r", 5);
	GPIO_PORTG_DATA_R |= 0x04;
}

//*****************************************************************************
// Turn the board's LED off
//*****************************************************************************
void led_off()
{
	UARTSend((uint8_t *)"OFF\n\r", 6);
	GPIO_PORTG_DATA_R &= ~(0x04);
}

//*****************************************************************************
// Blink the board's LED (on, delay + off, delay)
//*****************************************************************************
bool led_blink(bool blink)
{
	blink = true;
	volatile uint32_t ui32Loop;
	GPIO_PORTG_DATA_R |= 0x04;
	for(ui32Loop = 0; ui32Loop < LED_ON; ui32Loop++) {}
	GPIO_PORTG_DATA_R &= ~(0x04);
	for(ui32Loop = 0; ui32Loop < LED_OFF; ui32Loop++) {}
	return blink;
}

//*****************************************************************************
// Enable/Disable the flood function
//*****************************************************************************
bool toggle_flood(bool flood)
{
	if(flood)
		flood = false;
	else
		flood = true;
	return flood;
}

//*****************************************************************************
// Enable/Disable the displaying of buttons on the OLED
//*****************************************************************************
bool disable_button(bool disable )
{
    tRectangle sRect;
    tContext sContext;
    GrContextInit(&sContext, &g_sCFAL96x64x16);
    if(disable)
    {
		UARTSend((uint8_t *)"Buttons ON\n\r", 13);
		disable = false;
    }
    else
    {
		UARTSend((uint8_t *)"Buttons OFF\n\r", 14);
		disable = true;
    }
	// Clear the buttons information on the display
    sRect.i16XMin = 0;
    sRect.i16YMin = 10;
    sRect.i16XMax = GrContextDpyWidthGet(&sContext) - 1;
    sRect.i16YMax = 33;
    GrContextForegroundSet(&sContext, ClrBlack);
    GrRectFill(&sContext, &sRect);
    return disable;
}

//*****************************************************************************
// This code implements some functionalities to the board which interact with
// a terminal software
//*****************************************************************************
int
main(void)
{
    tRectangle sRect;
    tContext sContext;
    uint8_t ucDelta;
    uint8_t State = ALL_BUTTONS;
    int32_t data; // stores the char typed in the terminal
    int count=0, bp=0; // count is loop counter, bp is button pressed
    volatile uint32_t ui32Loop; //for the loop delay
    bool flood = false;
    bool blink = false;
    bool disable = false;
	char temp[40]; //buffer for OLED manipulation from in to char
	
	// Enable the peripherals used by this program.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
    GPIOPinTypeGPIOOutput(GPIO_PORTG_BASE, GPIO_PIN_2);
    // Enable lazy stacking for interrupt handlers.  This allows floating-point
    // instructions to be used within interrupt handlers, but at the expense of
    // extra stack usage.
    ROM_FPULazyStackingEnable();
    // Set the clocking to run directly from the crystal.
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);
    // Initialize the display driver.
    CFAL96x64x16Init();
    // Initialize the graphics context.
    GrContextInit(&sContext, &g_sCFAL96x64x16);
    // Fill the top part of the screen with blue to create the banner.
    sRect.i16XMin = 0;
    sRect.i16YMin = 0;
    sRect.i16XMax = GrContextDpyWidthGet(&sContext) - 1;
    sRect.i16YMax = 9;
    GrContextForegroundSet(&sContext, ClrDarkBlue);
    GrRectFill(&sContext, &sRect);
    // Change foreground for white text.
    GrContextForegroundSet(&sContext, ClrWhite);
    // Put the application name in the middle of the banner.
    GrContextFontSet(&sContext, g_psFontFixed6x8);
    // Enable the peripherals used by this program.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    // Enable processor interrupts.
    ROM_IntMasterEnable();
    // Set GPIO A0 and A1 as UART pins.
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    // Configure the UART for 115,200, 8-N-1 operation.
    ROM_UARTConfigSetExpClk(UART0_BASE, ROM_SysCtlClockGet(), 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));
    // Prompt for text to be entered. Sends thius menu to terminal
    UARTSend((uint8_t *)"Menu Selection : \n\r", 20);
    UARTSend((uint8_t *)"1 - LED ON: \n\r", 15);
    UARTSend((uint8_t *)"2 - LED OFF: \n\r", 16);
    UARTSend((uint8_t *)"3 - BLINK LED: \n\r", 18);
    UARTSend((uint8_t *)"4 - Toggle Flood: \n\r", 21);
    UARTSend((uint8_t *)"5 - Disable Buttons: \n\r", 24);
    // Initialize the Buttons function
    ButtonsInit();

    while(1)
    {
	  // Reading data from the terminal: Menu Options
		if(UARTCharsAvail(UART0_BASE))
        {
			data = UARTCharGetNonBlocking(UART0_BASE);
			if(data == '1')
            {
				blink = false;
				led_on();
			}
			if(data == '2')
			{
				blink = false;
				led_off();
			}
			if(data == '3')
				blink = led_blink(blink);
			if(data == '4')
				flood = toggle_flood(flood);
			if(data == '5')
				disable = disable_button(disable);
		}
		if(blink)
			blink = led_blink(blink);
		//Clear the top of screen of the OLED
		sRect.i16XMin = 0;
		sRect.i16YMin = 0;
		sRect.i16XMax = GrContextDpyWidthGet(&sContext) - 1;
		sRect.i16YMax = 9;
		GrContextForegroundSet(&sContext, ClrDarkBlue);
		GrRectFill(&sContext, &sRect);
		GrContextForegroundSet(&sContext, ClrWhite);
		//converts the count variable from int to char
		sprintf(temp, "count=%d", count);
		GrStringDrawCentered(&sContext, temp, -1,
							   GrContextDpyWidthGet(&sContext) / 2, 4, 0);
		//for(ui32Loop = 0; ui32Loop < 200000; ui32Loop++) {} // delay
		//Reading of the Buttons of the board
		State = ButtonsPoll(&ucDelta, 0);
		if(BUTTON_PRESSED(ALL_BUTTONS, State, ucDelta))
		{
			bp ++;
			/Clear the bottom of screen of the OLED
			sRect.i16XMin = 0;
			sRect.i16YMin = 10;
			sRect.i16XMax = GrContextDpyWidthGet(&sContext) - 1;
			sRect.i16YMax = 33;
			GrContextForegroundSet(&sContext, ClrBlack);
			GrRectFill(&sContext, &sRect);
			GrContextForegroundSet(&sContext, ClrWhite);
			//Writte on the OLED the current state of the buttons
			if(!disable)
			{
				//Write the total of pressed buttons
				sprintf(temp, "total=%d", bp);
				GrStringDrawCentered(&sContext, temp, -1,
							 GrContextDpyWidthGet(&sContext) / 2, 30, false);
				//Find out which button was pressed last
				if(BUTTON_PRESSED(UP_BUTTON, State, ucDelta))
				{
					GrStringDrawCentered(&sContext, "Last was: UP", -1,
							 GrContextDpyWidthGet(&sContext) / 2, 20, false);
				}
				if(BUTTON_PRESSED(DOWN_BUTTON, State, ucDelta))
				{
					GrStringDrawCentered(&sContext, "Last was: DOWN", -1,
							 GrContextDpyWidthGet(&sContext) / 2, 20, false);
				}
				if(BUTTON_PRESSED(SELECT_BUTTON, State, ucDelta))
				{
					GrStringDrawCentered(&sContext, "Last was: SELECT", -1,
							 GrContextDpyWidthGet(&sContext) / 2, 20, false);
				}
				if(BUTTON_PRESSED(LEFT_BUTTON, State, ucDelta))
				{
					GrStringDrawCentered(&sContext, "Last was: LEFT", -1,
							 GrContextDpyWidthGet(&sContext) / 2, 20, false);
				}
				if(BUTTON_PRESSED(RIGHT_BUTTON, State, ucDelta))
				{
					GrStringDrawCentered(&sContext, "Last was: RIGHT", -1,
							 GrContextDpyWidthGet(&sContext) / 2, 20, false);
				}
			}
		}
	  // enables the flood function
      if(flood)
		UARTCharPut(UART0_BASE, '^');
      count++;
    }
}



