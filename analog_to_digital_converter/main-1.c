#include <msp430.h> 

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer


    int adcval;
    int i;
    int test;
    int test2=0;
    P1DIR |= 0x30;          //P1.4 and 1.5 as Output
    P2DIR |= 0xFF;          //P2 as Output
    P2SEL &= 0x00;          //Setting mode for P2.5 and 2.6
    P1SEL |= 0x03;
    P1REN |= 0x08;          //Enable pull up resistor for button P1.3
    P1DIR &= 0xF7;          //SetnP1.3 as Input
    P1OUT |= 0x08;
    ConfigureAdc();

    __enable_interrupt();
    while(1)
    {
        for(i=0;i<9999;i++)
        {
        test = P1IN;
        if(test == 0x06)        //Check for button press
            if(test2 == 0)
                test2 = 1;
            else if(test2 == 1)
                test2 = 0;
        }
        if(test2 == 0)
            ADC10CTL1 |= INCH_0;        //Set LDR Input channel
        else if(test2 == 1)
            ADC10CTL1 |= INCH_1;        //Set POT input channel

        ADC10CTL0 |= ENC + ADC10SC;        //sample and convert
        __bis_SR_register(CPUOFF + GIE);
        __delay_cycles(1000);
        ADC10CTL0 &= ~ENC;
        adcval=ADC10MEM;                //Store ADC value
        __delay_cycles(100);
        ADC10CTL1 &= ~INCH_0 + ~INCH_1;
        if(test2 == 0)
        {
        if(adcval<32)                   //Driving LED Strip for LDR
        {
            P1OUT &= 0xCF;
            P2OUT &= 0x00;
            P1OUT |= 0x00;
            P2OUT |= 0x80;
        }
        else if((adcval>=32)&&(adcval<87))
                {
                    P1OUT &= 0xCF;
                    P2OUT &= 0x00;
                    P1OUT |= 0x00;
                    P2OUT |= 0x40;
                }
        else if((adcval>=87)&&(adcval<142))
                {
                    P1OUT &= 0xCF;
                    P2OUT &= 0x00;
                    P1OUT |= 0x00;
                    P2OUT |= 0x20;
                }
        else if((adcval>=142)&&(adcval<197))
                {
                    P1OUT &= 0xCF;
                    P2OUT &= 0x00;
                    P1OUT |= 0x00;
                    P2OUT |= 0x10;
                }
        else if((adcval>=197)&&(adcval<252))
                {
                    P1OUT &= 0xCF;
                    P2OUT &= 0x00;
                    P1OUT |= 0x00;
                    P2OUT |= 0x08;
                }
        else if((adcval>=252)&&(adcval<307))
                {
                    P1OUT &= 0xCF;
                    P2OUT &= 0x00;
                    P1OUT |= 0x00;
                    P2OUT |= 0x04;
                }
        else if((adcval>=307)&&(adcval<362))
                {
                    P1OUT &= 0xCF;
                    P2OUT &= 0x00;
                    P1OUT |= 0x00;
                    P2OUT |= 0x02;
                }
        else if((adcval>=362)&&(adcval<417))
                {
                    P1OUT &= 0xCF;
                    P2OUT &= 0x00;
                    P1OUT |= 0x00;
                    P2OUT |= 0x01;
                }
        else if((adcval>=417)&&(adcval<472))
                {
                    P1OUT &= 0xCF;
                    P2OUT &= 0x00;
                    P1OUT |= 0x10;
                    P2OUT |= 0x00;
                }
        else
        {
            P1OUT &= 0xCF;
            P2OUT &= 0x00;
            P1OUT |= 0x20;
            P2OUT |= 0x00;
        }
        }
        else if(test2==1)
        {
            if(adcval<550)                      //Driving LED Strip for POT
                    {
                        P1OUT &= 0xCF;
                        P2OUT &= 0x00;
                        P1OUT |= 0x00;
                        P2OUT |= 0x80;
                    }
                    else if((adcval>=540)&&(adcval<600))
                            {
                                P1OUT &= 0xCF;
                                P2OUT &= 0x00;
                                P1OUT |= 0x00;
                                P2OUT |= 0x40;
                            }
                    else if((adcval>=600)&&(adcval<650))
                            {
                                P1OUT &= 0xCF;
                                P2OUT &= 0x00;
                                P1OUT |= 0x00;
                                P2OUT |= 0x20;
                            }
                    else if((adcval>=650)&&(adcval<700))
                            {
                                P1OUT &= 0xCF;
                                P2OUT &= 0x00;
                                P1OUT |= 0x00;
                                P2OUT |= 0x10;
                            }
                    else if((adcval>=700)&&(adcval<750))
                            {
                                P1OUT &= 0xCF;
                                P2OUT &= 0x00;
                                P1OUT |= 0x00;
                                P2OUT |= 0x08;
                            }
                    else if((adcval>=750)&&(adcval<800))
                            {
                                P1OUT &= 0xCF;
                                P2OUT &= 0x00;
                                P1OUT |= 0x00;
                                P2OUT |= 0x04;
                            }
                    else if((adcval>=800)&&(adcval<850))
                            {
                                P1OUT &= 0xCF;
                                P2OUT &= 0x00;
                                P1OUT |= 0x00;
                                P2OUT |= 0x02;
                            }
                    else if((adcval>=850)&&(adcval<900))
                            {
                                P1OUT &= 0xCF;
                                P2OUT &= 0x00;
                                P1OUT |= 0x00;
                                P2OUT |= 0x01;
                            }
                    else if((adcval>=900)&&(adcval<950))
                            {
                                P1OUT &= 0xCF;
                                P2OUT &= 0x00;
                                P1OUT |= 0x10;
                                P2OUT |= 0x00;
                            }
                    else
                    {
                        P1OUT &= 0xCF;
                        P2OUT &= 0x00;
                        P1OUT |= 0x20;
                        P2OUT |= 0x00;
                    }
        }
    }
    return 0;
}

#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{
    __bic_SR_register_on_exit(CPUOFF);        // Return to active mode
}

void ConfigureAdc(void)                                     //Initial Setup
{
    ADC10CTL1 |= INCH_1 + ADC10DIV_3 + CONSEQ_2;
    ADC10CTL0 |= SREF_0 + ADC10SHT_3 + ADC10ON + ADC10IE;
    ADC10AE0 |= 0x03;                                      //Enable channels P1.0and P1.1
}
