//************************************************************************************************
//
 Using LCD and an Accelerometer
//
//
//The Code Below Moves rolls a Ball on the Boosterpack 128x128 LCD Screen in reaction to the tilt
//of the board. The tilt is detected using the Accelerometer.
//
//The LCD is controlled using the ST7735.h Header file
//The Accelerometer readings are taken using the Tiva Board ADC. The adc.h header file is used.
//
//************************************************************************************************

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
#include "ST7735.h"
#include "PLL.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/adc.h"
#include "ball.txt"

//************************************************************************************************
//
//This is the main program of the code. It performs 3 tasks:
//1. Initializing the LCD, ADC and GPIO Ports
//2. Checking the tilt from the Accelerometer in an infinite loop
//3. Continuously rolling the ball on LCD based on tilt
//
//The Accelerometer values are read from ADC Channels AIN6 and AIN7, GPIO pins PD0 and PD1 of the
//Tiva board respectively. We are using ADC0 and ADC1 of the Tiva board.
//
//The values of x-axis and y-axis of Accelerometer are read from AIN6 and AIN7. These values are
//stored in variables adc_x  and adc_y. These values are used to determine the tilt direction of
//the board.
//
//Based on the adc_x and adc_y values, two variables "a" and "b" are incremented or decremented. These
//variables are entered into the LCD print function If "a" is incremented, the ball moves to the
//right, and if it is decremented, the ball moves to the left. Similarly, the ball moves upwards
//when "b" is decremented and downwards when it is incremented.
//
//In the infinite loop, the code checks Accelerometer values, and changes the "a" and "b" variables
//if the board is tilted 10 degrees in any direction causing the ball to roll in the corresponding
//direction. If the board isn't tilted, the values won't change and thus the ball will remain in its
//current position.
//
//************************************************************************************************


int main(void)


{
        uint32_t adc_x;         //Variable for Accelerometer X-axis
        uint32_t adc_y;         //Variable for Accelerometer Y-axis
        SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
        //Initialize LCD Screen
        ST7735_InitR(INITR_REDTAB);
        PLL_Init(Bus80MHz);
        ST7735_FillScreen(0xFFFF);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);        //Enable GPIO D
        SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);
        //Set up ADC pins for Accelerometer
        GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_3);
        //Base 0 for ADC, processor event, configuration of 0th sequencer
        ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
        ADCSequenceConfigure(ADC1_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
        //Base 0 for ADC, 0th sequencer , 1 sampling, channel 7
        ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH7);    //Channel for Accelerometer X-axis
        //Base 0 for ADC, 0th sequencer , 1 sampling, channel 7
        ADCSequenceStepConfigure(ADC1_BASE, 0, 0, ADC_CTL_CH6);    //Channel for Accelerometer Y-axis
        //Start the sequencer for ADC0 and ADC1
        ADCSequenceEnable(ADC0_BASE, 0);
        ADCSequenceEnable(ADC1_BASE, 0);
        static int a=0;         //Variable for LCD x-axis
        static int b=128;       //Variable for LCD x-axis
        //Printing Ball at the center of the screen (Initial State)
        ST7735_DrawBitmap(a,b,ball,128,128);
        int i;
        for(i=0;i<50000;i++);

        while(1)
        {
            //Clearing flag for adc, sequencer 0
            ADCIntClear(ADC0_BASE, 0);
            //Trigger for adc0, sequencer 0
            ADCProcessorTrigger(ADC0_BASE, 0);
            SysCtlDelay(20000);
            //Read value for X axis
            ADCSequenceDataGet(ADC0_BASE, 0, &adc_x);
            //Clearing flag for adc, sequencer 0
            ADCIntClear(ADC1_BASE, 0);
            //Trigger for adc1, sequencer 0
            ADCProcessorTrigger(ADC1_BASE, 0);
            SysCtlDelay(100000);
            //Read value for Y axis
            ADCSequenceDataGet(ADC1_BASE, 0, &adc_y);
            if(adc_x>2163 && adc_y>1933 && adc_y<2163)                  //Board is tilted vertically upwards
            {
                if(a<55)
                {
                    a=a+2;
                }
                for(i=0;i<50000;i++);
            }

            else if(adc_x<1933 && adc_y>1933 && adc_y<2163)            //Board is tilted vertically downwards
            {
                if(a>-55)
                {
                        a=a-2;
                }
                for(i=0;i<50000;i++);
            }
            else if(adc_y<1933 && adc_x>1933 && adc_x<2163)            //Board is tilted horizontally to the left
            {
                if(b<183)
                {
                    b=b+2;
                }
                for(i=0;i<50000;i++);
            }
            else if(adc_y>2163 && adc_x>1933 && adc_x<2163)            //Board is tilted horizontally to the right
            {
                 if(b>73)
               {
                    b=b-2;
               }
               for(i=0;i<50000;i++);
            }
            else if(adc_x>2163 && adc_y>2163)                         //Board is tilted Diagonally to the upper right
            {
                if(a<55 && b>73)
                {
                    a=a+2;
                    b=b-2;
                }
                for(i=0;i<50000;i++);
            }

            else if(adc_x<1933 && adc_y<1933)            //Board is tilted diagonally to the bottom left
            {
                if(a>-55 && b<183)
                {
                    a=a-2;
                    b=b+2;
                }
                for(i=0;i<50000;i++);
            }
            else if(adc_y<1933 && adc_x>2163)            //Board is tilted diagonally to the bottom right
            {
                if(b<183 && a<55)
                {
                    b=b+2;
                    a=a+2;
                }
                for(i=0;i<50000;i++);
            }
            else if(adc_y>2163 && adc_x<1933)            //Board is tilted diagonally to the upper left
            {
                if(b>73 && a>-55)
                {
                    b=b-2;
                    a=a-2;
                }
                for(i=0;i<50000;i++);
            }
            //Update LCD Display with new coordinate values
            ST7735_DrawBitmap(a,b,ball,128,128);
            for(i=0;i<50000;i++);
        }
    }
