//*****************************************************************************
// Name      : lab10.c
// Date	     : April 14, 2015
// Class     : CEC322, Microprocessor Systems Lab
// Authors   : Renan Vieira & Jared Johnson
// Descript. : This program implements some functions to the board DK-TM4C123G
//			   First of all, a menu of options is displayed to a terminal software
//			   which has the follow functions:
//			   M: Print Menu     - prints in the terminal the menu options
//			   =: Clear Screen   - clears in the terminal the menu options
//			   B: Toggle Buttons - able the use of on board buttons
//			   Q: Quit Program   - finishes the program
//			   In addition, this program communicates with on board accelerometer
//			   getting data from that and displaying that on the screen
//*****************************************************************************

//*****************************************************************************
// Libraries
//*****************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/rom.h"
#include "driverlib/timer.h"
#include "driverlib/adc.h"
#include "driverlib/comp.h"
#include "driverlib/pin_map.h"
#include "grlib/grlib.h"
#include "drivers/cfal96x64x16.h"
#include "drivers/buttons.h"
#include "sensorlib/hw_mpu9150.h"
#include "sensorlib/hw_ak8975.h"
#include "sensorlib/i2cm_drv.h"
#include "sensorlib/ak8975.h"
#include "sensorlib/mpu9150.h"
#include "MyQueue.h"

//*****************************************************************************
// Constants
//*****************************************************************************
#define DELAY 75000 			 //delay used to keep the on board LED on/off
#define USER_LED_BASE GPIO_PORTG_BASE //port G
#define USER_LED_PIN GPIO_PIN_2		  //pin2
#define MPU9150_I2C_ADDRESS 0x69 //accelerometer address for I2C

//*****************************************************************************
// Global Variables
//*****************************************************************************
tRectangle gsRect;  // used for displaying
tContext gsContext; // graphics context used to show text on the CSTN display.
bool gDone = false; // controls the while loop
bool gButtonsEnabled = true; // able the on board buttons
tMPU9150 sMPU9150;  	// pointer to the MPU9150 instance data.
tI2CMInstance sI2CInst; // pointer to the I2C master driver instance data.
static uint8_t g_ui8ButtonStatesLoc = ALL_BUTTONS; // watches all on board buttons
volatile bool g_bMPU9150Done; // a boolean that is set when a MPU9150 command has completed.

//*****************************************************************************
// Function Prototypes
//*****************************************************************************
int StringToUart (char *str1);
int clearBackground(void);
void MPU9150Example(void);
int process_input(void);

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
	
    // Get the interrupt status.
    ui32Status = ROM_UARTIntStatus(UART0_BASE, true);
    // Clear the asserted interrupts.
    ROM_UARTIntClear(UART0_BASE, ui32Status);
	
    // Loop while there are characters in the receive FIFO.
    while(ROM_UARTCharsAvail(UART0_BASE))
    {
		uint32_t in_char;
	  
        in_char = ROM_UARTCharGetNonBlocking(UART0_BASE);
        // Add to Queue
        if (in_char != -1)
        {
			QueuePut(in_char);
        }         
    }
	
    // Preferably NOT in an ISR, but here for diagnostic reasons
    while (!QueueEmpty())
    { 
		process_input();
    } 
}

//*****************************************************************************
// button_ISR
//*****************************************************************************
void button_ISR(void)
{
	uint32_t ui32Status;
    static unsigned butCount = 0;
    char buffer[30];
    uint32_t butState;

	// Get the interrupt status.
    ui32Status = GPIOIntStatus(BUTTONS_GPIO_BASE, true);
	// Clear the asserted interrupts.
    GPIOIntClear(BUTTONS_GPIO_BASE, ui32Status);
    butCount++;
  
    // Clear Background
    gsRect.i16XMin = 0;
    gsRect.i16YMin = 14;
    gsRect.i16XMax = GrContextDpyWidthGet(&gsContext) - 1;
    gsRect.i16YMax = GrContextDpyHeightGet(&gsContext) - 1;;
    GrContextForegroundSet(&gsContext, ClrBlack);
    GrRectFill(&gsContext, &gsRect);        
    GrContextForegroundSet(&gsContext, ClrWhite);
  
    sprintf(buffer, "Presses = %d", butCount);
    GrStringDrawCentered(&gsContext, 
                         buffer, -1, GrContextDpyWidthGet(&gsContext) / 2, 20, false);
    
    butState = ~(ROM_GPIOPinRead(BUTTONS_GPIO_BASE, ALL_BUTTONS)) & ALL_BUTTONS;
    sprintf(buffer, "State = %x", butState);
    GrStringDrawCentered(&gsContext, buffer, -1, GrContextDpyWidthGet(&gsContext) / 2, 50, false);
    {
		// Find out which button was pressed
		char *msg = "UNDEF";
		switch(butState)
		{
			case UP_BUTTON:
				msg = "UP";
			break;
			case DOWN_BUTTON:
				msg = "DOWN";
			break;
			case SELECT_BUTTON:
				msg = "Select";
			break;
			case RIGHT_BUTTON:
				msg = "RIGHT";
			break;
			case LEFT_BUTTON:
				msg = "LEFT";
			break;
		} // switch
		GrStringDrawCentered(&gsContext, msg, -1, GrContextDpyWidthGet(&gsContext) / 2, 35, false);
	} // contain msg
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
        ROM_UARTCharPutNonBlocking(UART0_BASE, *pui8Buffer++);
    }
}

//*****************************************************************************
// StringToUart() function
//*****************************************************************************
int StringToUart (char *str1)
{
    uint32_t ui32Count = strlen(str1);
    // Loop while there are more characters to send.
    while(ui32Count--)
    {
        // Write the next character to the UART.
        ROM_UARTCharPut(UART0_BASE, *str1++);
    }
    ROM_UARTCharPut(UART0_BASE, '\r');
    ROM_UARTCharPut(UART0_BASE, '\n');    
    return 0;
}

//*****************************************************************************
// Initalize the UART.
//*****************************************************************************
int init_UART(void)
{
    // Enable the peripherals used by this example.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    // Set GPIO A0 and A1 as UART pins.
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    // Configure the UART for 115,200, 8-N-1 operation.
    ROM_UARTConfigSetExpClk(UART0_BASE, ROM_SysCtlClockGet(), 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));
    // Enable the UART interrupt.
    ROM_IntEnable(INT_UART0);
    ROM_UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
    UARTIntRegister(UART0_BASE, UARTIntHandler);
    return (0);
}

//*****************************************************************************
//Initializes the GPIO pins used by the board pushbuttons.
//*****************************************************************************
void ButtonsInitLocal(void)
{
    // Enable the GPIO port to which the pushbuttons are connected.
    ROM_SysCtlPeripheralEnable(BUTTONS_GPIO_PERIPH);
    // Set each of the button GPIO pins as an input with a pull-up.
    ROM_GPIODirModeSet(BUTTONS_GPIO_BASE, ALL_BUTTONS, GPIO_DIR_MODE_IN);
    ROM_GPIOPadConfigSet(BUTTONS_GPIO_BASE, ALL_BUTTONS,
                         GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    // Initialize the debounced button state with the current state read from the GPIO bank.
    g_ui8ButtonStatesLoc = ROM_GPIOPinRead(BUTTONS_GPIO_BASE, ALL_BUTTONS);
}

//*****************************************************************************
// Initialize the GPIO.
//*****************************************************************************
int init_GPIO(void)
{
    //////////////////////// Push Buttons ////////////////////////////////
    ButtonsInitLocal();
    ROM_GPIOIntTypeSet(BUTTONS_GPIO_BASE, ALL_BUTTONS, GPIO_FALLING_EDGE);
    GPIOIntRegister(BUTTONS_GPIO_BASE, button_ISR);
  
    // Enable the GPIO interrupt.
    IntEnable(INT_GPIOM);
    GPIOIntEnable(BUTTONS_GPIO_BASE, ALL_BUTTONS);
  
    //////////////////////// Onboard LED ////////////////////////////////
    // Enable the GPIO port that is used for the on-board LED.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
    // Enable the GPIO pin for the LED (PG2).  Set the direction as output, and
    // enable the GPIO pin for digital function.
    GPIOPinTypeGPIOOutput(GPIO_PORTG_BASE, GPIO_PIN_2);
    return(0);
}

//*****************************************************************************
// clearBackground() function
//*****************************************************************************
int clearBackground()
{
    // Clear Background
    gsRect.i16XMin = 0;
    gsRect.i16YMin = 14;
    gsRect.i16XMax = GrContextDpyWidthGet(&gsContext) - 1;
    gsRect.i16YMax = GrContextDpyHeightGet(&gsContext) - 1;
    GrContextForegroundSet(&gsContext, ClrBlack);
    GrRectFill(&gsContext, &gsRect);        
    GrContextForegroundSet(&gsContext, ClrWhite);
    return(0);
}

//*****************************************************************************
// PrintMenu() function
//*****************************************************************************
void PrintMenu() 
{
    ROM_UARTCharPutNonBlocking(UART0_BASE,'\r');
    ROM_UARTCharPutNonBlocking(UART0_BASE,'\n');
  
    StringToUart("M - Print this Menu");
    StringToUart("= - Clear Screen");
    StringToUart("B - Toggle Buttons Enabled");
    StringToUart("Q - Quit Program");
}

//*****************************************************************************
// process Input
//*****************************************************************************
int process_input()
{
    uint32_t err = 0;
    int local_char = ' ';
    char *msg = NULL;
  
    err = QueueGet(&local_char);
    if ( !err && (local_char != -1))
    {
		switch (tolower(local_char))
		{    
			case 'b':
				gButtonsEnabled = !gButtonsEnabled;
				if (gButtonsEnabled) 
				{
					IntEnable(INT_GPIOM);
					msg = "Bttns Enabled";
				} 
				else 
				{
					IntDisable(INT_GPIOM);
					msg = "Bttns Dis";
				}
			break;
      
			case 'q':
				gDone = true;
				msg = "Goodbye...";
			break;
      
			case '=':
				clearBackground();
				ROM_UARTCharPutNonBlocking(UART0_BASE, 0x0c);
			break;
      
			case 'm':
				PrintMenu();
			break;
      
			default:
				StringToUart("Char Undef");
			break; 
		} // switch
    
		if (msg != NULL)
		{
			clearBackground();
		  
			GrContextForegroundSet(&gsContext, ClrWhite);
			GrStringDrawCentered(&gsContext, msg, -1, GrContextDpyWidthGet(&gsContext) / 2, 40, false);
		  
		} // msg
	} // if valid input
    return(err);
} // process input

//*****************************************************************************
// Example of using pushbutton and analog comparator interrupts
//*****************************************************************************
int main(void)
{     
    // Enable lazy stacking for interrupt handlers.
    ROM_FPULazyStackingEnable();
    // Set the clocking to run directly from the crystal.
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);
    // Disable Interrupts
    ROM_IntMasterDisable(); 
    // Initialize the display driver.
    CFAL96x64x16Init();
	
    // Initialize the graphics context.
    GrContextInit(&gsContext, &g_sCFAL96x64x16);
    // Fill the top part of the screen with blue to create the banner.
    gsRect.i16XMin = 0;
    gsRect.i16YMin = 0;
    gsRect.i16XMax = GrContextDpyWidthGet(&gsContext) - 1;
    gsRect.i16YMax = 9;
    GrContextForegroundSet(&gsContext, ClrSpringGreen);
    GrRectFill(&gsContext, &gsRect);

    // Put the application name in the middle of the banner.
    GrContextForegroundSet(&gsContext, ClrBlack);
    GrContextFontSet(&gsContext, g_psFontFixed6x8);
    GrStringDrawCentered(&gsContext, "Accel Cross", -1,
                         GrContextDpyWidthGet(&gsContext) / 2, 4, 0);

    // Initialize the display and write some instructions.
    GrContextForegroundSet(&gsContext, ClrWhite);
    GrStringDrawCentered(&gsContext, "Connect a", -1,
                         GrContextDpyWidthGet(&gsContext) / 2, 20, false);
    GrStringDrawCentered(&gsContext, "terminal", -1,
                         GrContextDpyWidthGet(&gsContext) / 2, 30, false);
    GrStringDrawCentered(&gsContext, "to UART0.", -1,
                         GrContextDpyWidthGet(&gsContext) / 2, 40, false);
    GrStringDrawCentered(&gsContext, "115000,N,8,1", -1,
                         GrContextDpyWidthGet(&gsContext) / 2, 50, false);

    // Initialize UART
    init_UART();
    // GPIO
    init_GPIO();
    // Enable processor interrupts.
    ROM_IntMasterEnable();
    // Prompt for text to be entered.
    PrintMenu();
    UARTSend((uint8_t *)"Enter text: ", 12);    
    // Contains an Infinite Loop
    MPU9150Example();
	
    // Loop forever echoing data through the UART.
    while(!gDone)
    {
        volatile uint32_t ui32Loop;

        if (!QueueEmpty())
        {
			process_input();
        }
        // Turn on the LED.
        ROM_GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2, GPIO_HIGH_LEVEL);
      
        // Delay for a bit.
        for(ui32Loop = 0; ui32Loop < DELAY; ui32Loop++) { }

        // Turn off the LED.
        ROM_GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2, GPIO_LOW_LEVEL);

        // Delay for a bit.
        for(ui32Loop = 0; ui32Loop < 19*DELAY; ui32Loop++) { }
      
    } // while
    
    UARTSend((uint8_t *)"Goodbye...", 10);
} // main()

//*****************************************************************************
// MPU9150 Sensor callback function.  Called at the end of MPU9150 sensor
// driver transactions. This is called from I2C interrupt context. Therefore,
// we just set a flag and let main do the bulk of the computations and display.
//*****************************************************************************
void MPU9150Callback(void *pvCallbackData, uint_fast8_t ui8Status)
{
    // If the transaction succeeded set the data flag to indicate to
    // application that this transaction is complete and data may be ready.
    if(ui8Status == I2CM_STATUS_SUCCESS)
    {
        g_bMPU9150Done = 1;
    }
}

//*****************************************************************************
// The MPU9150 example.
//*****************************************************************************
void MPU9150Example(void)
{
    float fAccel[3]; 
  
    // Initialize I2C3 Peripheral
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C3);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
  
    ROM_GPIOPinConfigure(GPIO_PD0_I2C3SCL);
    ROM_GPIOPinConfigure(GPIO_PD1_I2C3SDA);
  
    ROM_GPIOPinTypeI2CSCL(GPIO_PORTD_BASE, GPIO_PIN_0);
    ROM_GPIOPinTypeI2C(GPIO_PORTD_BASE, GPIO_PIN_1);
    
    // Initialize B2 as MPU9150 interrupt pin
    ROM_GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_2);
  
    ROM_GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_PIN_2, GPIO_FALLING_EDGE);
  
    GPIOIntEnable(GPIO_PORTB_BASE, GPIO_PIN_2);
  
    ROM_IntEnable(INT_GPIOB);

    // Initialize the I2C instance associated with I2C3
    I2CMInit(&sI2CInst, I2C3_BASE, INT_I2C3, 0xff, 0xff, ROM_SysCtlClockGet());
  
    // Initialize the MPU9150. This code assumes that the I2C master instance
    // has already been initialized.
    g_bMPU9150Done = false;  
    MPU9150Init(&sMPU9150, &sI2CInst, MPU9150_I2C_ADDRESS, MPU9150Callback, 0);
    while(!g_bMPU9150Done) { }
	
    // Configure the MPU9150 for +/- 4 g accelerometer range.
    g_bMPU9150Done = false;
    MPU9150ReadModifyWrite(&sMPU9150, MPU9150_O_ACCEL_CONFIG,
                           ~MPU9150_ACCEL_CONFIG_AFS_SEL_M,
                           MPU9150_ACCEL_CONFIG_AFS_SEL_4G, MPU9150Callback,
                           0);
    while(!g_bMPU9150Done) { }

    // Loop forever reading data from the MPU9150. Typically, this process
    // would be done in the background, but for the purposes of this example,
    // it is shown in an infinite loop.
    while(!gDone)
    {
		char buf[40];
		// Request another reading from the MPU9150.
		g_bMPU9150Done = false;
		MPU9150DataRead(&sMPU9150, MPU9150Callback, 0);
		while(!g_bMPU9150Done) { }
		// Get the new accelerometer, gyroscope, and magnetometer readings.
		MPU9150DataAccelGetFloat(&sMPU9150, &fAccel[0], &fAccel[1], &fAccel[2]);
		// Do something with the new accelerometer, gyroscope, and magnetometer
		// readings.
		
		GrContextForegroundSet(&gsContext, ClrWhite);

		sprintf(buf, "X = %d", (int)(fAccel[0]*1000));    
		GrStringDraw(&gsContext, buf, -1, 0, 20, true);

		sprintf(buf, "Y = %d", (int)(fAccel[1]*1000));    
		GrStringDraw(&gsContext, buf, -1, 0, 30, true);

		sprintf(buf, "Z = %d", (int)(fAccel[2]*1000));    
		GrStringDraw(&gsContext, buf, -1, 0, 40, true);    
		
	   SysCtlDelay(SysCtlClockGet() / 10);
    } // while (1)
}

//*****************************************************************************
// Called by the NVIC as a result of GPIO port B interrupt event. For this
// application GPIO port B pin 2 is the interrupt line for the MPU9150
//*****************************************************************************
void IntGPIOb(void)
{
    unsigned long ulStatus;

    ulStatus = GPIOIntStatus(GPIO_PORTB_BASE, true);

    // Clear all the pin interrupts that are set
    GPIOIntClear(GPIO_PORTB_BASE, ulStatus);

    if(ulStatus & GPIO_PIN_2)
    {
        // MPU9150 Data is ready for retrieval and processing.
        MPU9150DataRead(&sMPU9150, MPU9150Callback, &sMPU9150);
    }
}

//*****************************************************************************
// Called by the NVIC as a result of I2C3 Interrupt. I2C3 is the I2C connection
// to the MPU9150.
//*****************************************************************************
void MPU9150I2CIntHandler(void)
{
    // Pass through to the I2CM interrupt handler provided by sensor library.
    // This is required to be at application level so that I2CMIntHandler can
    // receive the instance structure pointer as an argument.
    I2CMIntHandler(&sI2CInst);
}


