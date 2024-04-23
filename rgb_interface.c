/*
 * rgb_interface.c
 */

#include "rgb_interface.h"

uint8_t end_frame[] = {0x00,0x00,0x00};

void rgb_init_spi(void){
    // COPI on p1.2, SCLK on p1.4
    P1SEL = BIT2 + BIT4;
    P1SEL2 = BIT2 + BIT4;

    UCA0CTL1=UCSWRST; //disable serial interface
    UCA0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC;    // data cap at 1st clk edge, MSB first, master mode, synchronous
    UCA0CTL1 |= UCSSEL_2;                           // select SMCLK
    UCA0BR0 = 0;                                    // set frequency
    UCA0BR1 = 0;                                    //
    UCA0CTL1 &= ~UCSWRST;           // Initialize USCI state machine

}
//writes a 24 bit frame to the spi buffer
void rgb_send_frame(const uint8_t *frame1, bool wait_for_completion){
    int byte1;
    int bit1;
    for (byte1 = 0; byte1 < 3; byte1++){ // send 24 "bit" frame in 8 bit chunks
        for (bit1 = 7; bit1 >= 0; bit1--)
        if ((frame1[byte1] >> bit1) & 0x01 == 0x01) {  // Bit = 1
            UCA0TXBUF = 0xFF;
            while (!(IFG2 & UCA0TXIFG));  // USCI_A0 TX buffer ready?
            UCA0TXBUF = 0xE0;
            while (!(IFG2 & UCA0TXIFG));  // USCI_A0 TX buffer ready?
        }
        else { // Bit = 0
            UCA0TXBUF = 0xF0;
            while (!(IFG2 & UCA0TXIFG));  // USCI_A0 TX buffer ready?
            UCA0TXBUF = 0x00;
            while (!(IFG2 & UCA0TXIFG));  // USCI_A0 TX buffer ready?
        }
    }
    if (wait_for_completion)
        while (!(IFG2 & UCA0RXIFG));  // USCI_A0 RX buffer ready? (indicates transfer complete)
}

