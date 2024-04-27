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
    // COPI on p1.7, SCLK on p1.4
    P1SEL |= BIT4 + BIT7;
    P1SEL2 |= BIT4 + BIT7;

    // Set clock frequency to 8MHz
    BCSCTL1 = CALBC1_8MHZ;
    DCOCTL = CALDCO_8MHZ;

    // Set SMCLK = DCO
    BCSCTL2 &= ~SELS;

    // Set TimerA0 = SMCLK, continuous mode, divide by 8 (1500 Hz)
    TA0CTL = TASSEL_2 + MC_2;

    UCB0CTL1 |= UCSWRST; //disable serial interface
    UCB0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC;    // data cap at 1st clk edge, MSB first, master mode, synchronous
    UCB0CTL1 |= UCSSEL_2;                           // select SMCLK
    UCB0BR0 = 0;                                    // set frequency
    UCB0BR1 = 0;
    UCB0CTL1 &= ~UCSWRST;           // Initialize USCI state machine
}

static void rgb_generate_color(int seed, uint8_t *color, int brightness) {
    switch(brightness) {
    case 3 : ; // low brightness
        color[0] = seed & 0x03;
        color[1] = (seed >> 2) & 0x03;
        color[2] = (seed >> 4) & 0x03;
        break;
    case 4 : ; // medium brightness
        color[0] = seed & 0x07;
        color[1] = (seed >> 3) & 0x07;
        color[2] = (seed >> 6) & 0x07;
        break;
    case 5 : ; // med-high brightness
        color[0] = seed & 0x07 + seed & 0x03;
        color[1] = (seed >> 3) & 0x07 + (seed >> 2) & 0x03;
        color[2] = (seed >> 6) & 0x07 + (seed >> 4) & 0x03;
        break;
    case 6 : ; // high brightness
        color[0] = seed & 0x0F;
        color[1] = (seed >> 4) & 0x0F;
        color[2] = (seed >> 8) & 0x0F;
        break;
    }
}

static void rgb_send_color(const uint8_t *color, bool wait_for_completion){
    char byte1;
    int bit1;
    for (byte1 = 0; byte1 < 3; byte1++) { // send 24 "bit" frame in 8 bit chunks
        for (bit1 = 7; bit1 >= 0; bit1--) {
            if (((color[byte1] >> bit1) & 0x01) == 0x01) {  // Bit = 1
                UCB0TXBUF = 0xFC;
                while (!(IFG2 & UCB0TXIFG));  // USCI_B0 TX buffer ready?
            }
            else { // Bit = 0
                UCB0TXBUF = 0xE0;
                while (!(IFG2 & UCB0TXIFG));  // USCI_B0 TX buffer ready?
            }
        }
    }
    if (wait_for_completion)
        while (!(IFG2 & UCB0RXIFG));  // USCI_B0 RX buffer ready? (indicates transfer complete)
}

void rgb_generate_frame(uint8_t *color, int brightness) {
    int i;
    for (i = 255; i > 0; i--) {
        rgb_generate_color(TA0R, color, brightness);
        rgb_send_color(color, false);
    }
    rgb_generate_color(TA0R, color, brightness);
    rgb_send_color(color, true);
}

/*** Code Developed for "Bad Apple Extension" ***/
/*
static void change_color(uint8_t *color, int color_id, uint8_t brightness){
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
    }
}

static void rgb_send_row(const char *row, uint8_t *color, bool reversed, bool last_row){
    int i;
    if (reversed) {
        for (i = 0; i < 8; i++) { rgb_send_color(color, (row[1] >> i) & 0x01, false); }
        for (i = 0; i < 7; i++) { rgb_send_color(color, (row[0] >> i) & 0x01, false); }
        rgb_send_color(color, ((row[0] >> 7) & 0x01) == 0x01, last_row);
    }
    else {
        for (i = 7; i >= 0; i--) { rgb_send_color(color, (row[0] >> i) & 0x01, false); }
        for (i = 7; i > 0; i--) { rgb_send_color(color, (row[1] >> i) & 0x01, false); }
        rgb_send_color(color, row[1] & 0x01, last_row);
    }
}

void rgb_send_frame(const char (*frame)[2], uint8_t *color, int color_id, uint8_t brightness){
    int i;
    change_color(color, color_id, brightness);
    for (i = 0; i < 15; i++) {
        rgb_send_row(frame[i], color, !(i & 1), false); // reverses even rows
    }
    rgb_send_row(frame[15], color, false, true);
}
*/

