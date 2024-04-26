#include <msp430.h>
#include "msp430g2553.h"

// Notes for a major C chord
int periods[] = {1000000/261.63,
   1000000/329.63,
   1000000/392.00,
   1000000/523.5};

int which_period = 0;
int which_octave = 0;
volatile unsigned int i = 0;
void main(void) {
//    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    // Button and LED ISR setup
    P1DIR = BIT0 | BIT5 | BIT6; // green LED for interrupt check
    P1DIR &= ~BIT3;
    P1IE |= BIT3; // interrupt enable
    P1IES |= BIT3; // setting edge select
    P1IFG &= ~BIT3; // set flag to 0

//    P1DIR = BIT6;
    P2DIR = BIT5;

    P1OUT = 0;
    P2OUT = 0;

    P1OUT |= BIT5;
//    P1DIR |= BIT5;

    P1SEL = BIT1 + BIT2 + BIT4 + BIT5;
    P1SEL2 = BIT1 + BIT2 + BIT4 + BIT5;
    UCA0CTL1 = UCSWRST;                       // **Put state machine in reset**
    UCA0CTL0 |= UCMSB + UCSYNC;               // 3-pin, 8-bit SPI master
    UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    IE2 |= UCA0RXIE;

    BCSCTL3 |= LFXT1S_2;      // ACLK = VLO

    BCSCTL1 = CALBC1_1MHZ;    // Set the DCO to 1 MHz
    DCOCTL = CALDCO_1MHZ;     // And load calibration data

    P2DIR |= BIT5; // We need P2.5 to be output
    P2DIR |= BIT1; // Set P2.1 as output
    P2SEL |= BIT5; // P2.5 is TA1.2 PWM output

    TA1CCTL2 = OUTMOD_7;
    TA1CTL = TASSEL_2 + MC_1; // SMCLK, upmode

    WDTCTL = WDT_ADLY_16;  // Set Watchdog Timer to 1000 ms with VLO
    IE1 |= WDTIE;

    __enable_interrupt();

    __bis_SR_register(CPUOFF + GIE);
}

// On board button ISR to play different octaves
# pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
    // Green LED on every press
//    P1DIR ^= BIT0;
//    P1DIR &= ~BIT3;

//    if (which_octave) {
//        // which_octave is now 3rd octave
//        which_octave = 0;
////        P1IES ^= BIT3;
////        P1IFG &= ~BIT3;
//    }
//    else {
//        // which_octave is now 4th octave
//        which_octave = 1;
////        P1IES ^= BIT3;
////        P1IFG &= ~BIT3;
//    }

//    if ((P1OUT & BIT3) != BIT3)
        which_octave = 1-which_octave;
//    else
//        which_octave = 0;
//        periods[0] = 1000000/220;
//        periods[1] = 0;
//        periods[2] = 0;
//        periods[3] = 0;

    // default case to use Port 1.3 for interrupt processing
//    P1IES ^= BIT3;
    P1IFG &= ~BIT3;
    __bic_SR_register_on_exit(P1IFG);

}


__attribute__((interrupt(USCIAB0RX_VECTOR))) void USCI0RX_ISR (void)
{
  char value = UCA0RXBUF;
  if ((value == 0xA1) && ~which_octave){
      // C5
    periods[0] = 1000000/523.2;
    periods[1] = 1000000/523.2;
    periods[2] = 1000000/523.2;
    periods[3] = 1000000/523.2;
//      for(i=0; i< 20000; i++);
  } else if ((value == 0xA2) && ~which_octave){
      // D5
    periods[0] = 1000000/587.2;
    periods[1] = 1000000/587.2;
    periods[2] = 1000000/587.2;
    periods[3] = 1000000/587.2;
//      for(i=0; i< 20000; i++);
  } else if ((value == 0xA3) && ~which_octave){
      // E5
    periods[0] = 1000000/659.2;
    periods[1] = 1000000/659.2;
    periods[2] = 1000000/659.2;
    periods[3] = 1000000/659.2;
  } else if ((value == 0xA4) && ~which_octave){
      // F5
    periods[0] = 1000000/698.56;
    periods[1] = 1000000/698.56;
    periods[2] = 1000000/698.56;
    periods[3] = 1000000/698.56;
  } else if ((value == 0xA5) && ~which_octave){
      // G5
    periods[0] = 1000000/784;
    periods[1] = 1000000/784;
    periods[2] = 1000000/784;
    periods[3] = 1000000/784;
  } else if ((value == 0xA6) && ~which_octave){
      // A5
    periods[0] = 1000000/880;
    periods[1] = 1000000/880;
    periods[2] = 1000000/880;
    periods[3] = 1000000/880;
  } else if ((value == 0xA7) && ~which_octave){
      // B5
    periods[0] = 1000000/987.84;
    periods[1] = 1000000/987.84;
    periods[2] = 1000000/987.84;
    periods[3] = 1000000/987.84;
  } else if ((value == 0xA8) && ~which_octave){
      // None
    periods[0] = 0;
    periods[1] = 0;
    periods[2] = 0;
    periods[3] = 0;
  } else if ((value == 0xA1) && which_octave){
      // C4
    periods[0] = 1000000/261.6;
    periods[1] = 1000000/261.6;
    periods[2] = 1000000/261.6;
    periods[3] = 1000000/261.6;
//      for(i=0; i< 20000; i++);
  } else if ((value == 0xA2) && which_octave){
      // D4
    periods[0] = 1000000/293.7;
    periods[1] = 1000000/293.7;
    periods[2] = 1000000/293.7;
    periods[3] = 1000000/293.7;
//      for(i=0; i< 20000; i++);
  } else if ((value == 0xA3) && which_octave){
      // E4
    periods[0] = 1000000/329.6;
    periods[1] = 1000000/329.6;
    periods[2] = 1000000/329.6;
    periods[3] = 1000000/329.6;
  } else if ((value == 0xA4) && which_octave){
      // F4
    periods[0] = 1000000/349.2;
    periods[1] = 1000000/349.2;
    periods[2] = 1000000/349.2;
    periods[3] = 1000000/349.2;
  } else if ((value == 0xA5) && which_octave){
      // G4
    periods[0] = 1000000/392.0;
    periods[1] = 1000000/392.0;
    periods[2] = 1000000/392.0;
    periods[3] = 1000000/392.0;
  } else if ((value == 0xA6) && which_octave){
      // A4
    periods[0] = 1000000/440.0;
    periods[1] = 1000000/440.0;
    periods[2] = 1000000/440.0;
    periods[3] = 1000000/440.0;
  } else if ((value == 0xA7) && which_octave){
      // B4
    periods[0] = 1000000/493.92;
    periods[1] = 1000000/493.92;
    periods[2] = 1000000/493.92;
    periods[3] = 1000000/493.92;
  } else if ((value == 0xA8) && which_octave){
      // None
    periods[0] = 0;
    periods[1] = 0;
    periods[2] = 0;
    periods[3] = 0;
  }
  P1OUT = 0;
  P2OUT = 0;

  if (which_octave){
      periods[0] *= 2;
      periods[1] *= 2;
      periods[2] *= 2;
      periods[3] *= 2;
  }

}

#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void)
{
  which_period = (which_period + 1) % 4;
  TA1CCR2 = periods[which_period]>>1;
  TA1CCR0 = periods[which_period];
//  periods[0] = 1000000/(1/periods[0]*1000000*(1-0.02847*which_octave));
//  periods[3] = 1000000/(1/periods[3]*1000000*(1-0.02847*which_octave));
  periods[0] = periods[0]*which_octave;
//  P1IES |= BIT3;
//  __bic_SR_register_on_exit(CPUOFF+GIE);
}
