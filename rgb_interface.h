/**
 * @file rgb_interface.h
 * @brief Function prototypes for the 16x16 WS2812 LED matrix driver.
 *
 * This file contains prototypes for functions needed to drive the
 * 16x16 WS2812B LED matrix with the MSP430G2553 micro-controller
 * to function as a DJ set.
 *
 * @author Bryant Huang
 * @author Giovanni Minatel
 * @author Bill Nguyen
 * @author William Pan
 */

#ifndef RGB_INTERFACE_H_
#define RGB_INTERFACE_H_
#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Initializes the MSP430G2553 microcontroller for communication with
 *        the 16x16 WS2812B LED matrix.
 *
 * Initializes I/O ports (P1.2, P1.4) needed for communication to LED
 * matrix, the sub-main clock (as the 16MHz digitally controlled oscillator),
 * and the serial interface for operation in SPI protocol.
 *
 * @return Void.
 */
void rgb_init_spi(void);

/**
 * @brief Sets the color to be passed into the LED matrix.
 *
 * The color is stored as a uint8_t array of length 3 in GRB format.
 * The value of color_id corresponds to the following colors: 0-red,
 * 1-yellow, 2-green, 3-cyan, 4-blue, 5-magenta, 6-white. Any other
 * value passed to color_id turns the LED off.
 *
 * @param color An array of length 3 allocated for storing the color value.
 * @param color_id An integer 0-6 (inclusive) indicating the color.
 * @param brightness Brightness value of the color
 * @return Void.
 */
void change_color(uint8_t *color, int color_id, uint8_t brightness); // TODO: make static after testing

/**
 * @brief Sends color to serial buffer for sending to each LED.
 *
 * Writes each bit of the color value as a square wave with an appropriate duty
 * cycle corresponding to how the WS2812B interprets the bit.
 *
 * @param color An array of uint8_t values in GRB format specifying color.
 * @param light_on Integer dictating whether LED should be on.
 * @param wait_for_completion Boolean indicating whether color is last to be sent.
 * @return Void.
 */
void rgb_send_color(const uint8_t *color, int light_on, bool wait_for_completion); // TODO: make static after testing

/**
 * @brief Processes row of binary values to be displayed on LED matrix.
 *
 * Helper function for rgb_send_frame.
 *
 * @param row An array of 16 binary values indicating whether LED should be on.
 * @param color An array of uint8_t values in GRB format specifying color.
 * @param reversed Integer dictating whether row should be read in reverse.
 * @param last_row Boolean indicating whether row is last of LED matrix.
 * @return Void.
 */
void rgb_send_row(const int *row, uint8_t *color, int reversed, bool last_row); // TODO: make static after testing

/**
 * @brief Processes frame of binary values to be displayed on LED matrix.
 *
 * Primary function for interfacing with the LED matrix.
 * The value of color_id corresponds to the following colors: 0-red,
 * 1-yellow, 2-green, 3-cyan, 4-blue, 5-magenta, 6-white. Any other
 * value passed to color_id turns the LED off.
 *
 * @param frame A 16x16 array of binary values illustrating the image to be displayed.
 * @param color An array of length 3 allocated for storing the color value.
 * @param color_id An integer 0-6 (inclusive) indicating the color.
 * @return Void.
 */
void rgb_send_frame(const int *frame, uint8_t *color, int color_id, uint8_t brightness);

#endif /* RGB_INTERFACE_H_ */
