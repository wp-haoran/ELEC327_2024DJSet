#include <msp430.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

char printdist[] = "Distance to the nearest object is: ";
char centimeter[] = " cm";
char dot[] = ".";
char zerro[] = "0";
char newline[] = " \r\n";
volatile int temp[2];
volatile float diff;
volatile unsigned int i=0;
int dst_int;
int dst_flt;
float tmp_flt;
char dst_char[5];
char dst_flt_char[5];
volatile float distance;
void ser_output(char *str);

volatile char received_ch = 0;

void main(void)

{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    BCSCTL1= CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;



    //pins spi
    P1OUT |= BIT5;
    P1DIR |= BIT5 | BIT6;
    P1SEL = BIT1 | BIT2 | BIT4 | BIT6;
    P1SEL2 = BIT1 | BIT2 | BIT4;


    //pins ultrasonic//
       //P1DIR = BIT6;
        P2SEL = BIT1;
        //P1SEL = BIT1|BIT2|BIT6;
        //P1SEL2 = BIT1|BIT2;
        //


    //spi
    UCA0CTL1 = UCSWRST;
    UCA0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC; // 3-pin, 8-bit SPI master
    UCA0CTL1 |= UCSSEL_2; // SMCLK
    UCA0BR0 |= 0x02; // /2
    UCA0BR1 = 0; //
    UCA0MCTL = 0; // No modulation
    UCA0CTL1 &= ~UCSWRST; // **Initialize USCI state machine**
    //

    //uart
//    UCA0CTL1 |= UCSWRST+UCSSEL_2;
//    UCA0BR0 = 52;
//    UCA0BR1 = 0;
//    UCA0MCTL = UCBRS_0;
//    UCA0CTL1 &= ~UCSWRST;
    //

    //ultrasonic
    TA0CTL = TASSEL_2|MC_1 ;
    TA0CCR0 = 0xFFFF;
    TA0CCR1 = 0x000A;
    TA0CCTL1 = OUTMOD_7;
    TA1CTL = TASSEL_2|MC_2 ;
    TA1CCTL1 = CAP | CCIE | CCIS_0 | CM_3 | SCS ;
    //

    _enable_interrupts();

    while(1){
        distance = diff/58;
        dst_int = floor(distance);

        P1OUT &= (~BIT5); // Select Device

        while (!(IFG2 & UCA0TXIFG)); // USCI_A0 TX buffer ready?

        if (dst_int < 15){
            UCA0TXBUF = 0xA1;
        } else if ((dst_int > 15) && (dst_int < 30)) {
            UCA0TXBUF = 0xA2;
        } else if ((dst_int > 30) && (dst_int < 45)) {
            UCA0TXBUF = 0xA3;
        } else if ((dst_int > 45) && (dst_int < 60)) {
            UCA0TXBUF = 0xA4;
        } else



            if ((dst_int > 60) && (dst_int < 75)) {
            UCA0TXBUF = 0xA5;
        } else if ((dst_int > 75) && (dst_int < 90)) {
            UCA0TXBUF = 0xA6;
        } else if ((dst_int > 90) && (dst_int < 105)) {
            UCA0TXBUF = 0xA7;
        } else {
            UCA0TXBUF = 0xA8;
        }
         // Send 0xAA over SPI to Slave
        while (!(IFG2 & UCA0RXIFG)); // USCI_A0 RX Received?
        received_ch = UCA0RXBUF; // Store received data

        P1OUT |= (BIT5); // Unselect Device



        __delay_cycles(100000);  //0.5 second delay


    }
}
#pragma vector = TIMER1_A1_VECTOR
__interrupt void Timer_A(void){
        temp[i] = TA1CCR1;
        i += 1;
        TA1CCTL1 &= ~CCIFG ;
        if (i==2) {
            diff=temp[i-1]-temp[i-2];
            i=0;
        }

}
