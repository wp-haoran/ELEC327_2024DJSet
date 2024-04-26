#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>
#include "rgb_interface.h"

/**
* main.c
*/
void init_wdt(void) {
    BCSCTL3 |= LFXT1S_2; // ACLK = VLO
    BCSCTL1 |= DIVA_3; // ACLK divided by 8
    WDTCTL = WDT_ADLY_16; // WDT 16ms, ACLK, interval timer
    IE1 |= WDTIE; // Enable WDT interrupt
}

void init_buttons(void) {
    P1DIR &= ~BIT3;
    P2DIR &= ~BIT0; // P1.3, P2.0 input
    P1REN |= BIT3;
    P2REN |= BIT0; // P1.3, P2.0 enable pullup/pulldown resistor
    P1OUT |= BIT3;
    P2OUT |= BIT0; // P1.3, P2.0 pullup resistor

    P1IE |= BIT3;
    P2IE |= BIT0; // P1.3, P2.0 interrupt enabled
    P1IES |= BIT3;
    P2IES |= BIT0; // P1.3, P2.0 interrupt occurs on negedge
    P1IFG &= ~BIT3;
    P2IFG &= ~BIT0; // P1.3, P2.0 clear pending interrupts
}

void init_uart(void) {

}

// initialize color values.
// colors cycle through rainbow, beginning at red.
// brightness set to lowest value to allow full range of colors

uint8_t color[3] = {0x00, 0x00, 0x00};
volatile int color_id = 0;
volatile int flash_mode = 0;
volatile int pause_mode = 0;
char frame[16][2] = {{0b00000100, 0b00011100},
                     {0b00000100, 0b00010100},
                     {0b00011111, 0b00011100},
                     {0b00000100, 0b00000000},
                     {0b00000101, 0b01110111},
                     {0b00000110, 0b01010101},
                     {0b00000100, 0b01110111},
                     {0b00001100, 0b00000000},
                     {0b00010100, 0b00001000},
                     {0b00000100, 0b01111111},
                     {0b00000100, 0b00001000},
                     {0b00000100, 0b00011100},
                     {0b00010100, 0b00101010},
                     {0b00001100, 0b01001001},
                     {0b00000100, 0b00001000},
                     {0b00000000, 0b00000000}};

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer

    init_wdt();
    init_buttons();
    init_uart();
    rgb_init_spi();

    _enable_interrupts();

    while (1) {
        rgb_send_frame(frame, color, color_id, 0x0F);
        LPM3;
    }
    return 0;
}

// Watchdog Timer interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(WDT_VECTOR))) watchdog_timer (void)
#else
#error Compiler not supported!
#endif
{
    if(flash_mode) {
        color_id++;
        if (color_id > 6){
            color_id = 0;
        }
    }
    __bic_SR_register_on_exit(LPM3_bits); // exit LPM3 when returning to program (clear LPM3 bits)
}

// P1.3 Button ISR
#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR( void )
{
    flash_mode ^= 1;
    P1IFG &= ~BIT3;
    __bic_SR_register_on_exit(LPM3_bits);     // Clear LPM3 bits from 0(SR)
}

// P2.0 Button ISR
#pragma vector = PORT2_VECTOR
__interrupt void PORT2_ISR( void )
{
    pause_mode ^= 1;
    P2IFG &= ~BIT0;
    __bic_SR_register_on_exit(LPM3_bits);     // Clear LPM3 bits from 0(SR)
}
