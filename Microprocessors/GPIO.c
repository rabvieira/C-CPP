//*****************************************************************************
// Name      : lab3v2.c
// Date	     : January 30, 2015
// Class     : CEC322, Microprocessor Systems Lab
// Authors   : Renan Vieira & Jared Johnson
// Descript. : This program implements some functions to the board DK-TM4C123G
//			   First of all, a menu of options is displayed to a terminal software
//			   which has the follow functions:
//			   1: turns on the LED on the board
//			   2: turns off the LED on the board
//			   3: blinks the LED on the board
//			   4: splash a message on the screen for a few seconds
//			   5: enables numerical displaying for ADC reading on the screen
//			   6: enables graphical displaying for ADC reading on the screen
//			   7: turns off the ADC displaying
//			   a: toggle the displaying of the channel 4
//			   b: toggle the displaying of the channel 5
//			   c: toggle the displaying of the channel 6
//			   q: quit the program
//*****************************************************************************

/* libraries */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/adc.h"
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
void __error__(char *pcFilename, uint32_t ui32Line) {}
#endif

//*****************************************************************************
// The UART interrupt handler.
//*****************************************************************************
void UARTIntHandler(void)
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
// Turn the board's LED on
//*****************************************************************************
void led_on()
{
	UARTSend((uint8_t *)"ON\n\r", 5);
	GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2, GPIO_HIGH_LEVEL);
}

//*****************************************************************************
// Turn the board's LED off
//*****************************************************************************
void led_off()
{
	UARTSend((uint8_t *)"OFF\n\r", 6);
	GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2, GPIO_LOW_LEVEL);
}

//*****************************************************************************
// Blink the board's LED (on, delay + off, delay)
//*****************************************************************************
bool led_blink(bool blink)
{
	blink = true;
	volatile uint32_t ui32Loop;
	GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2, GPIO_HIGH_LEVEL);
	for(ui32Loop = 0; ui32Loop < LED_ON; ui32Loop++) {}
	GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2, GPIO_LOW_LEVEL);
	for(ui32Loop = 0; ui32Loop < LED_OFF; ui32Loop++) {}
	return blink;
}

//********************************************************************************
// enable displaying of all three channels available on the OLED at numerical mode
//********************************************************************************
bool numerical_funct(bool numerical, uint32_t ui32Value[8],tContext sContext, tRectangle sRect)
{
	char temp[40];
    numerical = true;
    sprintf(temp, "ADC0=%d", ui32Value[0]);
	GrStringDrawCentered(&sContext, temp, -1, GrContextDpyWidthGet(&sContext) / 2, 20, false);
	sprintf(temp, "ADC0=%d", ui32Value[1]);
	GrStringDrawCentered(&sContext, temp, -1, GrContextDpyWidthGet(&sContext) / 2, 40, false);
	sprintf(temp, "ADC0=%d", ui32Value[2]);
	GrStringDrawCentered(&sContext, temp, -1, GrContextDpyWidthGet(&sContext) / 2, 60, false);
	return numerical;
}

//********************************************************************************
// enable displaying of all three channels available on the OLED at graphical mode
//********************************************************************************
bool graphical_funct(bool graphical, uint32_t ui32Value[8],tContext sContext, tRectangle sRect)
{
	graphical = true;
	//channel 4
	sRect.i16XMin = 0;
	sRect.i16YMin = 10;
	sRect.i16XMax = (ui32Value[0]/4095.0)*(GrContextDpyWidthGet(&sContext) - 1);
	sRect.i16YMax = 23;
	GrContextForegroundSet(&sContext, ClrOrange);
	GrRectFill(&sContext, &sRect);
	//channel 5
	sRect.i16XMin = 0;
	sRect.i16YMin = 24;
	sRect.i16XMax = (ui32Value[1]/4095.0)*(GrContextDpyWidthGet(&sContext) - 1);
	sRect.i16YMax = 44;
	GrContextForegroundSet(&sContext, ClrGreen);
	GrRectFill(&sContext, &sRect);
	//channel 6
	sRect.i16XMin = 0;
	sRect.i16YMin = 45;
	sRect.i16XMax = (ui32Value[2]/4095.0)*(GrContextDpyWidthGet(&sContext) - 1);
	sRect.i16YMax = 63;
	GrContextForegroundSet(&sContext, ClrYellow);
	GrRectFill(&sContext, &sRect);
	return graphical;
}

//*****************************************************************************
// enable displaying channel 4 available on the OLED at graphical mode
//*****************************************************************************
void channel4(bool ch4, uint32_t ui32Value[8],tContext sContext, tRectangle sRect)
{
	char temp[40];
    if(ch4)
    {
		sRect.i16XMin = 0;
		sRect.i16YMin = 10;
		sRect.i16XMax = (ui32Value[0]/4095.0)*(GrContextDpyWidthGet(&sContext) - 1);
		sRect.i16YMax = 23;
		GrContextForegroundSet(&sContext, ClrOrange);
		GrRectFill(&sContext, &sRect);
    }
    else
    {
        sprintf(temp, "ADC0=%d", ui32Value[0]);
        GrStringDrawCentered(&sContext, temp, -1, 
							GrContextDpyWidthGet(&sContext) / 2, 20, false);
    }
}

//*****************************************************************************
// enable displaying channel 5 available on the OLED at graphical mode
//*****************************************************************************
void channel5(bool ch5, uint32_t ui32Value[8],tContext sContext, tRectangle sRect)
{
    char temp[40];
    if(ch5)
    {
		sRect.i16XMin = 0;
		sRect.i16YMin = 24;
		sRect.i16XMax = (ui32Value[1]/4095.0)*(GrContextDpyWidthGet(&sContext) - 1);
		sRect.i16YMax = 44;
		GrContextForegroundSet(&sContext, ClrGreen);
		GrRectFill(&sContext, &sRect);
    }
    else
    {
        sprintf(temp, "ADC0=%d", ui32Value[1]);
        GrStringDrawCentered(&sContext, temp, -1,
							GrContextDpyWidthGet(&sContext) / 2, 40, false);
    }
}

//*****************************************************************************
// enable displaying channel 6 available on the OLED at graphical mode
//*****************************************************************************
void channel6(bool ch6, uint32_t ui32Value[8],tContext sContext, tRectangle sRect)
{
    char temp[40];
    if(ch6)
    {
		sRect.i16XMin = 0;
		sRect.i16YMin = 45;
		sRect.i16XMax = (ui32Value[2]/4095.0)*(GrContextDpyWidthGet(&sContext) - 1);
		sRect.i16YMax = 63;
		GrContextForegroundSet(&sContext, ClrYellow);
		GrRectFill(&sContext, &sRect);
    }
    else
    {
        sprintf(temp, "ADC0=%d", ui32Value[2]);
        GrStringDrawCentered(&sContext, temp, -1,
							GrContextDpyWidthGet(&sContext) / 2, 60, false);
    }
}

//*****************************************************************************
// shows on the OLED the message "Renan & Jared" for a few seconds
//*****************************************************************************
void splash(tContext sContext, tRectangle sRect)
{
    sRect.i16XMin = 0;
    sRect.i16YMin = 0;
    sRect.i16XMax = GrContextDpyWidthGet(&sContext) - 1;
    sRect.i16YMax = 63;
    GrContextForegroundSet(&sContext, ClrRed);
    GrRectFill(&sContext, &sRect);
    GrContextForegroundSet(&sContext, ClrBlue);
    GrStringDrawCentered(&sContext, "Renan & Jared", -1,
						GrContextDpyWidthGet(&sContext) / 2, 20, false);
    SysCtlDelay(SysCtlClockGet());
    sRect.i16XMin = 0;
    sRect.i16YMin = 0;
    sRect.i16XMax = GrContextDpyWidthGet(&sContext) - 1;
    sRect.i16YMax = 63;
    GrContextForegroundSet(&sContext, ClrBlack);
    GrRectFill(&sContext, &sRect);
    GrContextForegroundSet(&sContext, ClrWhite);
}

//*****************************************************************************
// This code implements some functionalities to the board which interact with
// a terminal software. These involves (O)LED's control + 3 ADC inputs
//*****************************************************************************
int main(void)
{
	tRectangle sRect;				  // Rectangle variable
    tContext sContext;				  // graphics context
    int32_t data;                     // stores the char typed in the terminal
    int count=0;                      // count is loop counter
    volatile uint32_t ui32Loop;       //for the loop delay
    bool blink = false;               //logic contro of the blink functionality
    bool ch4 = false, ch5 = false, ch6 = false; //control of each channel
    bool turn_off_ADC = true; //enables/disable displaying of ADC on the screen
    char temp[40];            //buffer for OLED manipulation from int to char
    uint32_t ui32Value[8];    //stores the data catch from the ADC

    // Allows floating-point instructions to be used within interrupt handlers
    ROM_FPULazyStackingEnable();
    // Set the clocking to run directly from the crystal.
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);
	// Enable the peripherals used by this program.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
    GPIOPinTypeGPIOOutput(GPIO_PORTG_BASE, GPIO_PIN_2);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    // Initialize the display driver.
    CFAL96x64x16Init();
    // Initialize the graphics context.
    GrContextInit(&sContext, &g_sCFAL96x64x16);
    // Change foreground for white text.
    GrContextForegroundSet(&sContext, ClrWhite);
    // Put the application name in the middle of the banner.
    GrContextFontSet(&sContext, g_psFontFixed6x8);
    // Enable processor interrupts.
    ROM_IntMasterEnable();
    // Set GPIO A0 and A1 as UART pins.
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    // Configure the UART for 115,200, 8-N-1 operation.
    ROM_UARTConfigSetExpClk(UART0_BASE, ROM_SysCtlClockGet(), 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));
    // Prompt for text to be entered. Sends this menu to terminal
    UARTSend((uint8_t *)"Menu Selection : \n\r", 20);
    UARTSend((uint8_t *)"1 - LED ON: \n\r", 15);
    UARTSend((uint8_t *)"2 - LED OFF: \n\r", 16);
    UARTSend((uint8_t *)"3 - BLINK LED: \n\r", 18);
    UARTSend((uint8_t *)"4 - Splash: \n\r", 16);
    UARTSend((uint8_t *)"5 - Numerical: \n\r", 18);
    UARTSend((uint8_t *)"6 - Graphical: \n\r", 18);
    UARTSend((uint8_t *)"7 - Turn Off:  \n\r", 18);
    UARTSend((uint8_t *)"a - Chan4:  \n\r", 15);
    UARTSend((uint8_t *)"b - Chan5:  \n\r", 15);
    UARTSend((uint8_t *)"c - Chan6:  \n\r", 15);
    UARTSend((uint8_t *)"q - Quit:  \n\r", 14);
    // Enable ADC peripherals
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0); 
	// Small delay required
    SysCtlDelay(SysCtlClockGet() / 12);
	//Selects the Sequence Number. 0 implies all channels are available
    ADCSequenceDisable(ADC0_BASE,  0);
	//Configure the Sequence chosen
    ADCSequenceConfigure(ADC0_BASE, // ADC0
			 0,         			// Sequence Number
			 ADC_TRIGGER_PROCESSOR, // Trigger
			 0); 					// Priority
	//Configure the Sequence Step chosen
    ADCSequenceStepConfigure(ADC0_BASE,
			 0, 				    // Sequence Number
			 0, 				    // Step
			 ADC_CTL_CH4);			// Channel
    ADCSequenceStepConfigure(ADC0_BASE,
		     0, 				    // Sequence Number
		     1,       				// Step
		     ADC_CTL_CH5);			// Channel
    ADCSequenceStepConfigure(ADC0_BASE,
		     0, 				    // Sequence
		     2, 					// Step
		     ADC_CTL_CH6 | ADC_CTL_IE | ADC_CTL_END); // Channel Setup
	// Enable ADC Input peripherals		 
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	// Specify the pins which will be used as input
    GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_7);
    GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_6);
    GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_5);
	// Enable ADC Sequence	
    ADCSequenceEnable(ADC0_BASE, 0);
	// Clear buffer
    ADCIntClear(ADC0_BASE, 0);
    // Splash on the screen a message for a few seconds
    splash(sContext, sRect);
	//run this loop until 'q' is pressed
    while(1)
    {
		//Causes a processor trigger for a sample sequence.
		ADCProcessorTrigger(ADC0_BASE, 0);
		//Wait until the sample sequence has completed
		while(!ADCIntStatus(ADC0_BASE, 0, false)) {}
		//Clears sample sequence interrupt source
		ADCIntClear(ADC0_BASE, 0);
		//Gets the captured data for a sample sequence
		ADCSequenceDataGet(ADC0_BASE, 0, ui32Value);
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
				splash(sContext, sRect);
			if(data == '5')
			{
				ch4 = false;
				ch5 = false;
				ch6 = false;
				turn_off_ADC = false;
			}
			if(data == '6')
			{
				ch4 = true;
				ch5 = true;
				ch6 = true;
				turn_off_ADC = false;
			}
			if(data == '7')
				turn_off_ADC = true;
			if(data == 'q')
			{
				break;
			}
			if(data == 'a')
			{
				if(ch4)
					ch4 = false;
				else
					ch4 = true;
				channel4(ch4, ui32Value,sContext, sRect);
				turn_off_ADC = false;
			}
			if(data == 'b')
			{
				if(ch5)
					ch5 = false;
				else
					ch5 = true;
				channel5(ch5, ui32Value,sContext, sRect);
				turn_off_ADC = false;
			}
			if(data == 'c')
			{
				if(ch6)
					ch6 = false;
				else
					ch6 = true;
				channel6(ch6, ui32Value,sContext, sRect);
				turn_off_ADC = false;
			}
		}
		//disable ADC reading
		if(!turn_off_ADC)
		{
			channel4(ch4, ui32Value,sContext, sRect);
			channel5(ch5, ui32Value,sContext, sRect);
			channel6(ch6, ui32Value,sContext, sRect);
		}
		//toggle blink		
		if(blink)
			blink = led_blink(blink);
		//Cleans the top of screen
		sRect.i16XMin = 0;
		sRect.i16YMin = 0;
		sRect.i16XMax = GrContextDpyWidthGet(&sContext) - 1;
		sRect.i16YMax = 9;
		GrContextForegroundSet(&sContext, ClrDarkBlue);
		GrRectFill(&sContext, &sRect);
		// Change foreground for white text.
		GrContextForegroundSet(&sContext, ClrWhite);
		//converts the count variable from int to char
		sprintf(temp, "count=%d", count);
		GrStringDrawCentered(&sContext, temp, -1,
							 GrContextDpyWidthGet(&sContext) / 2, 4, 0);
		//Clean the bottom of screen
		sRect.i16XMin = 0;
		sRect.i16YMin = 10;
		sRect.i16XMax = GrContextDpyWidthGet(&sContext) - 1;
		sRect.i16YMax = 63;
		// Change foreground for black text.
		GrContextForegroundSet(&sContext, ClrBlack);
		GrRectFill(&sContext, &sRect);
		// Change foreground for white text.
		GrContextForegroundSet(&sContext, ClrWhite);
		count++;
	}
}
