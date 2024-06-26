// This is a test code we used for the ultrasonic distance sensor

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
void main(void)

{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    BCSCTL1= CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
    P1DIR = BIT6;
    P2SEL = BIT1;
    P1SEL = BIT1|BIT2|BIT6;
    P1SEL2 = BIT1|BIT2;
    UCA0CTL1 |= UCSWRST+UCSSEL_2;
    UCA0BR0 = 52;
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS_0;
    UCA0CTL1 &= ~UCSWRST;
    TA0CTL = TASSEL_2|MC_1 ;
    TA0CCR0 = 0xFFFF;
    TA0CCR1 = 0x000A;
    TA0CCTL1 = OUTMOD_7;
    TA1CTL = TASSEL_2|MC_2 ;
    TA1CCTL1 = CAP | CCIE | CCIS_0 | CM_3 | SCS ;
    _enable_interrupts();
    while(1){
        distance = diff/58;
        dst_int = floor(distance);
        tmp_flt = distance - dst_int;



        ltoa(dst_int, dst_char, 10);

        ser_output(dst_char); //added line

        /*
        if (tmp_flt < 0.01) {
        dst_flt = floor(tmp_flt * 1000);


        ltoa(dst_flt,dst_flt_char, 10);
        ser_output(printdist); ser_output(dst_char); ser_output(dot); ser_output(zerro); ser_output(zerro); ser_output(dst_flt_char); ser_output(centimeter);
        }
        else if (tmp_flt < 0.1) {
            dst_flt = floor(tmp_flt * 100);
            ltoa(dst_flt,dst_flt_char, 10);
            ser_output(printdist); ser_output(dst_char); ser_output(dot); ser_output(zerro); ser_output(dst_flt_char); ser_output(centimeter);
        }
        else {
            dst_flt = floor(tmp_flt * 100);
            ltoa(dst_flt,dst_flt_char, 10);
            ser_output(printdist); ser_output(dst_char); ser_output(dot); ser_output(dst_flt_char); ser_output(centimeter);
        }
        */
        ser_output(newline);
        __delay_cycles(500000);  //0.5 second delay
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
void ser_output(char *str){
    while(*str != 0) {
        while (!(IFG2&UCA0TXIFG));
        UCA0TXBUF = *str++;
        }
}
