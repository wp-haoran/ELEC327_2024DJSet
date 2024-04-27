#include <msp430.h>
#include "msp430g2553.h"

// Notes for a major C chord, default configuration
int periods[] = {1000000/261.63,
   1000000/329.63,
   1000000/392.00,
   1000000/523.5};

// counter to keep track of index in array
int which_period = 0;
// flag that keeps track of which octave we are in,
// starts at default of 0 which is 5th octave, 1 is 4th/3rd octave depending on user choice
int which_octave = 0;
//
//volatile unsigned int i = 0;
void main(void) {

//    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    // Button and LED ISR setup
    P1DIR = BIT0 | BIT5 | BIT6; // green LED for interrupt check
    P1DIR &= ~BIT3; // set up pin for button
    P1IE |= BIT3; // interrupt enable
    P1IES |= BIT3; // setting edge select
    P1IFG &= ~BIT3; // set flag to 0

    // Sets up piezo buzzer output
    P2DIR = BIT5;

    // Initialize outputs to be 0
    P1OUT = 0;
    P2OUT = 0;

    // Establish GPIO pin as slave select for SPI
    P1OUT |= BIT5;

    // SPI setup needs 4 pins to be connected, 8-bits received from master.
    // Pins are for slave select, shared SCLK for synchronization, MOSI, and MISO
    P1SEL = BIT1 + BIT2 + BIT4 + BIT5;
    P1SEL2 = BIT1 + BIT2 + BIT4 + BIT5;
    UCA0CTL1 = UCSWRST;                       // **Put state machine in reset**
    UCA0CTL0 |= UCMSB + UCSYNC;               // 3-pin, 8-bit SPI master
    UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    IE2 |= UCA0RXIE;                          // enable interrupt

    BCSCTL3 |= LFXT1S_2;      // ACLK = VLO

    BCSCTL1 = CALBC1_1MHZ;    // Set the DCO to 1 MHz
    DCOCTL = CALDCO_1MHZ;     // And load calibration data

    // Piezo buzzer
    P2DIR |= BIT5; // We need P2.5 to be output
    P2DIR |= BIT1; // Set P2.1 as output
    P2SEL |= BIT5; // P2.5 is TA1.2 PWM output

    TA1CCTL2 = OUTMOD_7;
    TA1CTL = TASSEL_2 + MC_1; // SMCLK, upmode

    WDTCTL = WDT_ADLY_16;  // Set Watchdog Timer to 1000 ms with VLO
    IE1 |= WDTIE;

    // enable interrupts
    __enable_interrupt();
    __bis_SR_register(CPUOFF + GIE);
}

// On board button ISR to play different octaves
# pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
    // Change which_octave to other binary value to indicate current mode of sound playing
    which_octave = 1-which_octave;
    // Clear button IFG flag
    P1IFG &= ~BIT3;
    __bic_SR_register_on_exit(P1IFG);
}

// This ISR is for playing the appropriate sound based on the received byte from master board and button press
__attribute__((interrupt(USCIAB0RX_VECTOR))) void USCI0RX_ISR (void)
{
  // This is the byte received from the master board
  char value = UCA0RXBUF;
  // The sounds played depends on the byte received from the master board and current which_octave value, which
  // changes based on button press. Since we compare two variables, case statements were not used.
  if ((value == 0xA1) && ~which_octave){
      // C5
    periods[0] = 1000000/523.2;
    periods[1] = 1000000/523.2;
    periods[2] = 1000000/523.2;
    periods[3] = 1000000/523.2;
  } else if ((value == 0xA2) && ~which_octave){
      // D5
    periods[0] = 1000000/587.2;
    periods[1] = 1000000/587.2;
    periods[2] = 1000000/587.2;
    periods[3] = 1000000/587.2;
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

    // Case for nothing detected within 105 cm of ultrasonic sensor, play no sound
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
  } else if ((value == 0xA2) && which_octave){
      // D4
    periods[0] = 1000000/293.7;
    periods[1] = 1000000/293.7;
    periods[2] = 1000000/293.7;
    periods[3] = 1000000/293.7;
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

    // Case for nothing detected within 105 cm of ultrasonic sensor, play no sound
  } else if ((value == 0xA8) && which_octave){
      // None
    periods[0] = 0;
    periods[1] = 0;
    periods[2] = 0;
    periods[3] = 0;
  }
  // Set outputs to 0 at end of interrupt to reset for next time interrupt called.
  P1OUT = 0;
  P2OUT = 0;

  // Multiply period by two to shift down one octave. It was initially 4th octave, but this optional code
  // makes it 3rd octave so user can easily hear difference in notes.
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
  // Cycle through periods array by incrementing which_period by 1 and taking mod 4 each time we enter WDT ISR
  which_period = (which_period + 1) % 4;
  TA1CCR2 = periods[which_period]>>1;
  TA1CCR0 = periods[which_period];

  // Multiply by which_octave, a flag with binary value, so that the output sound is either continuous or pulsing
  periods[0] = periods[0]*which_octave;
}
