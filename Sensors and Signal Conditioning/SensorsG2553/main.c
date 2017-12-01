#include <msp430.h>
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;       // stop watchdog timer
    P1DIR |= BIT0;                                // Set Pin as output

    //ADC INIT
    ADC10CTL0 = ADC10SHT_2 + ADC10ON;         // ADC10ON, interrupt enabled
    ADC10CTL1 = INCH_4;                       // input A1
    ADC10AE0 |= 0x10;                         // PA.1 ADC option select
    P1DIR |= 0x01;                            // Set P1.0 to output direction

    TA0CCTL0 = CCIE;                              // TACCR0 interrupt enabled
    TA0CCR0 = 15000;                              // TA0CCR0 15 kHz
    TA0CTL = TASSEL_1 | MC_1;                     // ACLK, up mode
    //UART Config
    if (CALBC1_1MHZ==0xFF)                  // If calibration constant erased
      {
        while(1);                               // do not load, trap CPU!!
      }
    //UART INIT
      DCOCTL = 0;                               // Select lowest DCOx and MODx settings
      BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
      DCOCTL = CALDCO_1MHZ;
      P1SEL |= BIT1 + BIT2 + BIT5;                     // P1.1 = RXD, P1.2=TXD
      P1SEL2 = BIT1 + BIT2 ;                    // P1.1 = RXD, P1.2=TXD
      UCA0CTL1 |= UCSSEL_2;                     // SMCLK
      UCA0BR0 = 104;                            // 1MHz 9600
      UCA0BR1 = 0;                              // 1MHz 9600
      UCA0MCTL = UCBRS0;                        // Modulation UCBRSx = 1
      UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
      IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt

         ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
         __bis_SR_register(GIE);        // LPM0, ADC10_ISR will force exit
         while(1);
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR(void)
{
    while (!(IFG2&UCA0TXIFG));          //TRANSMIT READY....   RU READY KIDS?
    UCA0TXBUF = ADC10MEM >> 8;          //NEED TO BIT SHIFT... AYE AYE CAPTAIN
    while (!(IFG2&UCA0TXIFG));          //RU READY?
    UCA0TXBUF = ADC10MEM;               //Read rest of ADC
    ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
}

