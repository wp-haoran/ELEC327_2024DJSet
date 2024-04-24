/**
 * @file rgb_interface.c
 * @brief A driver to operate the 16x16 WS2812B LED matrix.
 *
 * This file contains functions and their helper functions to operate the
 * 16x16 WS2812B LED matrix with synchronous serial communication (SPI
 * Protocol) through the MSP430G2553 microcontroller. Functionailty of
 * this driver includes changing the color of monochromatic displays and
 * displaying preset monochromatic frames.
 *
 * @author Bryant Huang
 * @author Giovanni Minatel
 * @author Bill Nguyen
 * @author William Pan
 */

#include "rgb_interface.h"

void rgb_init_spi(void){
    // COPI on p1.2, SCLK on p1.4
    P1SEL = BIT2 + BIT4;
    P1SEL2 = BIT2 + BIT4;

    // Set clock frequency to 16MHz
    BCSCTL1 = CALBC1_16MHZ;
    DCOCTL = CALDCO_16MHZ;

    // Set SMCLK = DCO
    BCSCTL2 &= ~SELS;

    UCA0CTL1=UCSWRST; //disable serial interface
    UCA0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC;    // data cap at 1st clk edge, MSB first, master mode, synchronous
    UCA0CTL1 |= UCSSEL_2;                           // select SMCLK
    UCA0BR0 = 0;                                    // set frequency
    UCA0BR1 = 0;                                    //
    UCA0CTL1 &= ~UCSWRST;           // Initialize USCI state machine
}

void change_color(uint8_t *color, int color_id, uint8_t brightness){
    if(brightness > 0x40) { // prevents overly high brightness
        brightness = 0x40;
    }

    switch(color_id) {
    case 0 : ;  // color = red
        color[0] = 0x00;
        color[1] = brightness;
        color[2] = 0x00;
        break;
    case 1 : ;  // color = yellow
        color[0] = brightness;
        color[1] = brightness;
        color[2] = 0x00;
        break;
    case 2 : ;  // color = green
        color[0] = brightness;
        color[1] = 0x00;
        color[2] = 0x00;
        break;
    case 3 : ;  // color = cyan
        color[0] = brightness;
        color[1] = 0x00;
        color[2] = brightness;
        break;
    case 4 : ;  // color = blue
        color[0] = 0x00;
        color[1] = 0x00;
        color[2] = brightness;
        break;
    case 5 : ;  // color = magenta
        color[0] = 0x00;
        color[1] = brightness;
        color[2] = brightness;
        break;
    case 6 : ;  // color = white
        color[0] = brightness;
        color[1] = brightness;
        color[2] = brightness;
    default : ; // color = off
        color[0] = 0x00;
        color[1] = 0x00;
        color[2] = 0x00;
    }
}

void rgb_send_color(const uint8_t *color, int light_on, bool wait_for_completion){
    unsigned int byte1;
    unsigned int bit1;
    for (byte1 = 0; byte1 < 3; byte1++){ // send 24 "bit" frame in 8 bit chunks
        for (bit1 = 7; bit1 >= 0; bit1--)
        if (light_on && ((color[byte1] >> bit1) & 0x01 == 0x01)) {  // Bit = 1
            UCA0TXBUF = 0xFF;
            while (!(IFG2 & UCA0TXIFG));  // USCI_A0 TX buffer ready?
            UCA0TXBUF = 0xE0;
            while (!(IFG2 & UCA0TXIFG));
        }
        else { // Bit = 0
            UCA0TXBUF = 0xF0;
            while (!(IFG2 & UCA0TXIFG));  // USCI_A0 TX buffer ready?
            UCA0TXBUF = 0x00;
            while (!(IFG2 & UCA0TXIFG));
        }
    }
    if (wait_for_completion)
        while (!(IFG2 & UCA0RXIFG));  // USCI_A0 RX buffer ready? (indicates transfer complete)
}

void rgb_send_row(const int *row, uint8_t *color, int reversed, bool last_row){
    unsigned int i;
    if (reversed) {
        for (i = 15; i > 0; i--) {
            rgb_send_color(color, row[i], false);
        }
        rgb_send_color(color, row[0], last_row);
    }
    else {
        for (i = 0; i < 15; i++) {
            rgb_send_color(color, row[i], false);
        }
        rgb_send_color(color, row[15], last_row);
    }
}

void rgb_send_frame(const int *frame, uint8_t *color, int color_id, uint8_t brightness){
    unsigned int i;
    int reversed;
    change_color(color, color_id, brightness);
    for (i = 0; i < 15; i++) {
        reversed = (i & 1 == 1); // reverses odd rows
        rgb_send_row(frame[i], color, reversed, false);
    }
    rgb_send_row(frame[15], color, 0, false);
}

