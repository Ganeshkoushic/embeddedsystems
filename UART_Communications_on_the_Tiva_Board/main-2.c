#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"

#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

void
UARTIntHandler(void)
{
    uint32_t ui32Status;
    ui32Status = ROM_UARTIntStatus(UART1_BASE, true);
    ROM_UARTIntClear(UART1_BASE, ui32Status);
    while(ROM_UARTCharsAvail(UART1_BASE))
    {
        ROM_UARTCharPutNonBlocking(UART1_BASE,
                                   ROM_UARTCharGetNonBlocking(UART1_BASE));
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
        SysCtlDelay(SysCtlClockGet() / (1000 * 3));
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);

    }
}

int
main(void)

{
    int data;
    int check=0;
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);        //Enable GPIO F
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);       //Set Up LED pins
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);                  //Set up switch
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);        //Enable UART1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinConfigure(GPIO_PB0_U1RX);            //Configure Pins PB0 and PB1 as UART RX and TX respectively
    GPIOPinConfigure(GPIO_PB1_U1TX);
    GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_0,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);   //Activate On-Board Switch
    UARTConfigSetExpClk(UART1_BASE, ROM_SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));  // Configure the UART for 115200, 8-N-1 operation
    while(1)
    {
        if(!GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0))            //Execute if Switch is pressed
        {
            UARTCharPut(UART1_BASE,0xAA);                       //Transmit 0xAA
            SysCtlDelay(1000000);
        }
        SysCtlDelay(1000000);
        if(UARTCharsAvail(UART1_BASE))
        {
            data = UARTCharGetNonBlocking(UART1_BASE);
        }
        if((data == 0xAA)&&(check==0))                          //Execute if 0xAA is Received
        {
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3 | GPIO_PIN_1 | GPIO_PIN_2,0x08);       //Turn on Green LED
                SysCtlDelay(100000);
                check=1;
                GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_0,1);
                UARTCharPut(UART1_BASE,0x00);
        }
        else if((data == 0xAA)&&(check==1))
        {
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3 | GPIO_PIN_1 | GPIO_PIN_2,0x04);       //Turn on Blue LED
                SysCtlDelay(100000);
                check =2;
                GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_0,1);
                UARTCharPut(UART1_BASE,0x00);
        }
        else if((data == 0xAA)&&(check==2))
                {
                        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3 | GPIO_PIN_1 | GPIO_PIN_2,0x02);   //Turn on Red LED
                        SysCtlDelay(100000);
                        check=0;
                        GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_0,1);
                        UARTCharPut(UART1_BASE,0x00);
                }
        else
        {
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_0,1);
            UARTCharPut(UART1_BASE,0x00);
        }
    }
}
