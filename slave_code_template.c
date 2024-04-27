// This is a test code we used for setting up a slave in SPI communication

#include <msp430.h>


volatile unsigned int i = 0;
void main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    P1DIR = BIT6;
    P2DIR = BIT5;

    P1OUT = 0;
    P2OUT = 0;

    P1OUT |= BIT5;
    P1DIR |= BIT5;

    P1SEL = BIT1 + BIT2 + BIT4 + BIT5;
    P1SEL2 = BIT1 + BIT2 + BIT4 + BIT5;
    UCA0CTL1 = UCSWRST;                       // **Put state machine in reset**
    UCA0CTL0 |= UCMSB + UCSYNC;               // 3-pin, 8-bit SPI master
    UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    IE2 |= UCA0RXIE;



    __bis_SR_register(GIE);


    while(1);
}
__attribute__((interrupt(USCIAB0RX_VECTOR))) void USCI0RX_ISR (void)
{

  char value = UCA0RXBUF;

  if (value == 0xAA){
//      for (;;)
//          {
              P1OUT ^= BIT6;

              for(i=0; i< 20000; i++);


//          }
  } else if (value == 0xAB){

      P2OUT ^= BIT5;
      for(i=0; i< 20000; i++);


  }
  P1OUT = 0;
  P2OUT = 0;


}
