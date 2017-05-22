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
#include "driverlib/comp.h"
#include "driverlib/rom.h"
#include "driverlib/pin_map.h"
#include "driverlib/timer.h"
#include "driverlib/i2c.h"
#include "grlib/grlib.h"
#include "drivers/cfal96x64x16.h"
#include "drivers/buttons.h"

#define LED_ON  1000  //amount of time in which the LED will be ON
#define LED_OFF 80000 //amount of time in which the LED will be OFF
#define I2C_SLAVE_ADDRESS 0x60
int waveForm = 0;
int g_Counter = 0;
//char sawTooth[4095] = 

//*****************************************************************************
// The error routine that is called if the driver library encounters an error.
//*****************************************************************************
#ifdef DEBUG
void __error__(char *pcFilename, uint32_t ui32Line) {}
#endif

//*****************************************************************************
// Graphics context used to show text on the CSTN display.
//*****************************************************************************
tContext g_sContext;

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
    GrStringDrawCentered(&g_sContext, "Interrupts", -1,
                         GrContextDpyWidthGet(&g_sContext) / 2, 4, 0);
}

//*****************************************************************************
//
// Send a string to the UART.
//
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
// The UART interrupt handler.
//*****************************************************************************
void UARTIntHandler(void)
{
    uint32_t ui32Status;
    //
    // Get the interrrupt status.
    //
    ui32Status = ROM_UARTIntStatus(UART0_BASE, false);

    //
    // Clear the asserted interrupts.
    //
    ROM_UARTIntClear(UART0_BASE, ui32Status);
    
    // Reading data from the terminal: Menu Options
    if(UARTCharsAvail(UART0_BASE))
    {
	waveForm = UARTCharGetNonBlocking(UART0_BASE);
        if(waveForm == '0')
        {
            splash();
        }
        /*
	if(data == '0')
	{
            splash();
	}
	if(data == '1')
	{
            sawToothWave();
	}
	if(data == '2')
	{
            squareWave();
	}
        */
    }
}

//*****************************************************************************
// WaitI2CDone
//*****************************************************************************
static unsigned long WaitI2CDone( unsigned int long ulBase){
   // Wait until done transmitting
   while( I2CMasterBusy(ulBase));
   // Return I2C error code
   return I2CMasterErr(ulBase);
}

//*****************************************************************************
// I2C_write
//*****************************************************************************
int i2c_write(uint32_t i2c_base_addr,
 unsigned long addr,
 unsigned num_char,
 unsigned char *write_buf)
{
 int rv = 0;
 if (num_char == 0)
 return rv;

 // Make sure
 WaitI2CDone(i2c_base_addr);
 if (num_char == 1)
 {
 // Set the slave address
 I2CMasterSlaveAddrSet(i2c_base_addr, addr , false);
 I2CMasterDataPut(i2c_base_addr, write_buf[0]);

 // Send the command to initiate the read
 I2CMasterControl(i2c_base_addr, I2C_MASTER_CMD_SINGLE_SEND);
 // Wait for that transmission to finish
 WaitI2CDone( i2c_base_addr);
 }
 else
 {
 int burst_cont = (num_char - 2);
 int index = 0;

 // Set the slave address
 // recieve = false
 I2CMasterSlaveAddrSet(i2c_base_addr, addr , false);
 // Tell the slave to start reading
 I2CMasterDataPut(i2c_base_addr, write_buf[index++]);
 // Start Burst
 I2CMasterControl(i2c_base_addr, I2C_MASTER_CMD_BURST_SEND_START);
 // Wait for that transmission to finish
 WaitI2CDone(i2c_base_addr);

 while (burst_cont > 0)
 {
 // Tell the slave to start reading
 I2CMasterDataPut(i2c_base_addr, write_buf[index++]);
 // Continue Burst
 I2CMasterControl(i2c_base_addr, I2C_MASTER_CMD_BURST_SEND_CONT);
 // Wait for that transmission to finish
 WaitI2CDone( i2c_base_addr);
 burst_cont--;
 }

 // Tell the slave to start reading
 I2CMasterDataPut(i2c_base_addr, write_buf[index++]);
 // Finish Burst
 I2CMasterControl(i2c_base_addr, I2C_MASTER_CMD_BURST_SEND_FINISH);
 // Wait for that transmission to finish
 WaitI2CDone( i2c_base_addr);
 } // case num_char > 1

 return(rv);
}

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

void slowsawToothWave()
{
    char temp[40];
    int volt;
    unsigned char buf[8];
    for(volt=0 ; volt<4096 ; volt++)
    {
      buf[0] = (volt & 0xF00) >> 8;
      buf[1] = (volt & 0xFF);
      clear_bottom(16,36);
      sprintf(temp, "%x", buf[0]);
      GrStringDraw(&g_sContext, temp, -1, 26, 26, 0);
      sprintf(temp, "%x", buf[1]);
      GrStringDraw(&g_sContext, temp, -1, 31, 26, 0);
      i2c_write(I2C2_BASE, I2C_SLAVE_ADDRESS, 2, buf);
    }
}

void sawToothWave()
{
    int volt;
    unsigned char buf[8];
    for(volt=0 ; volt<4096 ; volt++)
    {
      buf[0] = (volt & 0xF00) >> 8;
      buf[1] = (volt & 0xFF);
      i2c_write(I2C2_BASE, I2C_SLAVE_ADDRESS, 2, buf);
    }
}

void squareWave()
{
    int volt;
    unsigned char buf[8];
    for(volt=0 ; volt<4096 ; volt++)
    {
      if(volt<= 2048)
      {
      //put top 4 bits of volt (12 bit rep) into unsigned char buf
      buf[0] = (0xFFF & 0xF00) >> 8;
      //put bottom 8 bits of volt (12 bit rep) into buf
      buf[1] = (0xFFF & 0x0FF);
      //i2c_write(I2C2_BASE, I2C_SLAVE_ADDRESS, 2, buf);
      }
      else
      {
        buf[0] =0x0;
        buf[1] = 0x0;
        
      }
      
      //clear_bottom(16,36);
      //sprintf(temp, "%x", buf);
      //GrStringDraw(&g_sContext, temp, -1, 16, 26, 0);
      i2c_write(I2C2_BASE, I2C_SLAVE_ADDRESS, 2, buf);
    }
}

//---------------------------------------------------------------------------

void Timer0IntHandler(void)
{
    //unsigned char temp[40];
    //sprintf(temp, "%x", g_Counter);
    //unsigned char write_buf[1] = {'A'};
    switch(waveForm)
    {
      case '0':
          i2c_write(I2C2_BASE, I2C_SLAVE_ADDRESS, 2, (unsigned char*)g_Counter); //random
      break;
      case '1':
          slowsawToothWave();
      break;
      case '2':
          sawToothWave();
      break;
       case '3':
          squareWave();
      break;
    }
    
    //i2c_write(I2C2_BASE, I2C_SLAVE_ADDRESS, 2, (unsigned char*)g_Counter); //random
    g_Counter ++;
    if(g_Counter >= 4095)
        g_Counter = 0;
}

//*****************************************************************************
//
// This example demonstrates how to send a string of data to the UART.
//
//*****************************************************************************

int main(void)
{
    //tRectangle sRect;
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
    // Disables the processor interrupt for draw graphics, initialize peripherals, etc
    ROM_IntMasterDisable();
    //
    // Initialize the display driver.
    //
    CFAL96x64x16Init();

    //
    // Initialize the graphics context.
    //
    GrContextInit(&g_sContext, &g_sCFAL96x64x16);
    splash();
    //
    // Enable the peripherals used by this example.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);    
    
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);    // LED
    GPIOPinTypeGPIOOutput(GPIO_PORTG_BASE, GPIO_PIN_2); // LED

    //
    // Set GPIO A0 and A1 as UART pins.
    //
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Configure the UART for 115,200, 8-N-1 operation.
    //
    ROM_UARTConfigSetExpClk(UART0_BASE, ROM_SysCtlClockGet(), 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));
    // Prompt for text to be entered. Sends menu to terminal
    UARTSend((uint8_t *)"Menu Selection : \n\r", 19);
    UARTSend((uint8_t *)"0 - Splash: \n\r", 14);
    UARTSend((uint8_t *)"1 - Slow Sawtooth: \n\r", 21);
     UARTSend((uint8_t *)"2 - Sawtooth: \n\r", 16);
    UARTSend((uint8_t *)"3 - Square: \n\r", 14);
    //
    // Enable the UART interrupt.
    //
    ROM_IntEnable(INT_UART0);
    ROM_UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
    
    
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C2);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeI2C(GPIO_PORTF_BASE, GPIO_PIN_7);
    GPIOPinTypeI2CSCL(GPIO_PORTF_BASE, GPIO_PIN_6);
    GPIOPinConfigure(GPIO_PF6_I2C2SCL);
    GPIOPinConfigure(GPIO_PF7_I2C2SDA);
    I2CMasterInitExpClk(I2C2_BASE, SysCtlClockGet(), true);   
    I2CMasterSlaveAddrSet(I2C2_BASE, I2C_SLAVE_ADDRESS, false);

    // Enable processor interrupts.
    ROM_IntMasterEnable();
    // Configure the two 32-bit periodic timers (initialization)
    ROM_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    //ROM_TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
    ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, ROM_SysCtlClockGet() / 10);
    // Setup the interrupts for the timer timeouts
    ROM_IntEnable(INT_TIMER0A);
    //ROM_IntEnable(INT_TIMER1A);
    ROM_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    // Enable the timers.
    ROM_TimerEnable(TIMER0_BASE, TIMER_A);
    
    while(1)
    {
	volatile uint32_t ui32Loop;
	GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2, GPIO_HIGH_LEVEL);
	for(ui32Loop = 0; ui32Loop < LED_ON; ui32Loop++) {}
	GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2, GPIO_LOW_LEVEL);
	for(ui32Loop = 0; ui32Loop < LED_OFF; ui32Loop++) {}
    }
}