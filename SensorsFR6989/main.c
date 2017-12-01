#include <msp430.h>
#include <LCDDriver.h>
#include <stdint.h>

void LCDInit();

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;       // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;
    //Timers Config
    P1DIR |= BIT0;                                // Set Pin as output
    P1OUT |= BIT0;
    P8SEL1 |= BIT7;                         // Configure P1.1 for ADC
    P8SEL0 |= BIT7;

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    // Configure ADC12
    ADC12CTL0 = ADC12SHT0_2 | ADC12ON;      // Sampling time, S&H=16, ADC12 on
    ADC12CTL1 = ADC12SHP;                   // Use sampling timer
    ADC12CTL2 |= ADC12RES_2;                // 12-bit conversion results
    ADC12MCTL0 |= ADC12INCH_4;              // A1 ADC input select; Vref=AVCC

    TA0CCTL0 = CCIE;                              // TBCCR0 interrupt enabled
    TA0CCR0 = 15000;
    TA0CTL = TASSEL_1 | MC_1;                     // ACLK, up mode

    LCDInit();

    //UART Config
    // Startup clock system with max DCO setting ~8MHz
    CSCTL0_H = CSKEY >> 8;                    // Unlock clock registers
    CSCTL1 = DCOFSEL_3 | DCORSEL;             // Set DCO to 8MHz
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // Set all dividers
    CSCTL0_H = 0;                             // Lock CS registers

    // Configure USCI_A0 for UART mode
    UCA0CTLW0 = UCSWRST;                      // Put eUSCI in reset
    UCA0CTLW0 |= UCSSEL__SMCLK;               // CLK = SMCLK
    // Baud Rate calculation
    // 8000000/(16*9600) = 52.083
    // Fractional portion = 0.083
    // User's Guide Table 21-4: UCBRSx = 0x04
    // UCBRFx = int ( (52.083-52)*16) = 1
    UCA0BR0 = 52;                             // 8000000/16/9600
    UCA0BR1 = 0x00;
    UCA0MCTLW |= UCOS16 | UCBRF_1 | 0x4900;
    UCA0CTLW0 &= ~UCSWRST;                    // Initialize eUSCI
    UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt

    P4SEL0 |= BIT2 | BIT3;                    // USCI_A0 UART operation
    P4SEL1 &= ~(BIT2 | BIT3);
      ADC12CTL0 |= ADC12ENC | ADC12SC;
     __bis_SR_register(GIE);
     while(1);
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR(void)
{
    uint16_t temp1 = ADC12MEM;
    while(!(UCA0IFG&UCTXIFG));
    P1OUT ^= BIT0;
    UCA0TXBUF = (uint8_t)(temp1 >> 8);
    while(!(UCA0IFG&UCTXIFG));
    UCA0TXBUF = (uint8_t)(0x00FF & temp1);
    ADC12CTL0 |= ADC12ENC | ADC12SC;
         //UCA0TXBUF = UCA0RXBUF;    // Echo back the character received
    showChar((UCA0RXBUF & 0x00FF), 6);  // Truncate and Display Character on LCD Digit 6
}

void LCDInit() {
PJSEL0 = BIT4 | BIT5;                   // For LFXT

    // Initialize LCD segments 0 - 21; 26 - 43
    LCDCPCTL0 = 0xFFFF;
    LCDCPCTL1 = 0xFC3F;
    LCDCPCTL2 = 0x0FFF;

    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    // Configure LFXT 32kHz crystal
    CSCTL0_H = CSKEY >> 8;                  // Unlock CS registers
    CSCTL4 &= ~LFXTOFF;                     // Enable LFXT
    do
    {
      CSCTL5 &= ~LFXTOFFG;                  // Clear LFXT fault flag
      SFRIFG1 &= ~OFIFG;
    }while (SFRIFG1 & OFIFG);               // Test oscillator fault flag
    CSCTL0_H = 0;                           // Lock CS registers

    // Initialize LCD_C
    // ACLK, Divider = 1, Pre-divider = 16; 4-pin MUX
    LCDCCTL0 = LCDDIV__1 | LCDPRE__16 | LCD4MUX | LCDLP;

    // VLCD generated internally,
    // V2-V4 generated internally, v5 to ground
    // Set VLCD voltage to 2.60v
    // Enable charge pump and select internal reference for it
    LCDCVCTL = VLCD_1 | VLCDREF_0 | LCDCPEN;

    LCDCCPCTL = LCDCPCLKSYNC;               // Clock synchronization enabled

    LCDCMEMCTL = LCDCLRM;                   // Clear LCD memory
    /*showChar('1', 1);
        showChar('2', 2);
        showChar('3', 3);
        showChar('4', 4);
        showChar('5', 5);
        showChar('6', 6); */
    //Turn LCD on
    LCDCCTL0 |= LCDON;

}
