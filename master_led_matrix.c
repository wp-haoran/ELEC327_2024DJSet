#include <msp430.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// code remains from serial debugging for testing
char printdist[] = "Distance to the nearest object is: ";
char centimeter[] = " cm";
char dot[] = ".";
char zerro[] = "0";
char newline[] = " \r\n";
char dst_char[5];
char dst_flt_char[5];
void ser_output(char *str);

// variables to track iterations and distance
volatile int temp[2];
volatile float diff;
volatile unsigned int i=0;
int dst_int;
int dst_flt;
float tmp_flt;
volatile float distance;

// flag to make sure transmitted message is received by slave
volatile char received_ch = 0;

void main(void)

{
    //clock configurations
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    BCSCTL1= CALBC1_1MHZ; // use 1 Mhz clock
    DCOCTL = CALDCO_1MHZ;

    //pins for spi communication -- based on which GPIO pins are connected to other hardware
    P1OUT |= BIT5;
    P1DIR |= BIT5 | BIT6;
    P1SEL = BIT1 | BIT2 | BIT4 | BIT6;
    P1SEL2 = BIT1 | BIT2 | BIT4;


    //pin selection for ultrasonic sensor
    P2SEL = BIT1;
    //pin selection for LED brightness toggle
    P2SEL |= BIT3 | BIT4;
    P2DIR |= BIT3 | BIT4;

    //spi setup
    UCA0CTL1 = UCSWRST;
    UCA0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC; // 3-pin, 8-bit SPI master
    UCA0CTL1 |= UCSSEL_2; // SMCLK
    UCA0BR0 |= 0x02; // /2
    UCA0BR1 = 0; //
    UCA0MCTL = 0; // No modulation
    UCA0CTL1 &= ~UCSWRST; // **Initialize USCI state machine**

    //ultrasonic sensor setup -- setting up timers
    TA0CTL = TASSEL_2|MC_1 ;
    TA0CCR0 = 0xFFFF;
    TA0CCR1 = 0x000A;
    TA0CCTL1 = OUTMOD_7;
    TA1CTL = TASSEL_2|MC_2 ;
    TA1CCTL1 = CAP | CCIE | CCIS_0 | CM_3 | SCS ;

    _enable_interrupts();

    while(1){
        distance = diff/58; //calibration neccary (from detasheet)
        dst_int = floor(distance);

        P1OUT &= (~BIT5); // Select Device

        while (!(IFG2 & UCA0TXIFG)); // USCI_A0 TX buffer ready?

        //sends the correspondednt byte based on the distance recorded
        if (dst_int < 20){
            UCA0TXBUF = 0xA4;
        } else if ((dst_int > 20) && (dst_int < 30)) {
            UCA0TXBUF = 0xA2;
        } else if ((dst_int > 30) && (dst_int < 40)) {
            UCA0TXBUF = 0xA3;
        } else {
            UCA0TXBUF = 0xA1;
        }



        //message is echoes back by the slave
        while (!(IFG2 & UCA0RXIFG)); // USCI_A0 RX Received?
        received_ch = UCA0RXBUF; // Store received data

        P1OUT |= (BIT5); // Unselect Device



        __delay_cycles(1000);  //0.5 second delay between sends, this gives time for message to be processed by the slave (give time for the buzzer to play the note)


    }
}

//timer A1 interrupt for distance detection using the ultrasonic sensor
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
