#include <msp430.h> 

//I am adding a new line here
/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	P1DIR|=0x41;
	P1REN|=0x08;
	long unsigned int i;
	while(1)
	{
	    if((P1IN & 0x08))
	    {
	        P1OUT|=0x01;             //Toggle LED
	        for(i=0;i<400000;i++);//delay
	        P1OUT&=0xFE;
	        for(i=0;i<400000;i++);
	    }
	    else
	        {
	            P1OUT|=0x40;             //Toggle LED
	            for(i=0;i<400000;i++);     //delay
	            P1OUT&=0xBF;
	            for(i=0;i<400000;i++);
	        }

	    }
	return 0;
}
