# ELEC327_2024DJSet
This repository holds the code for our (Minatel, Huang, Nguyen, Pan) final project at Rice University's ELEC 327 course. 

We sought to construct a DJ set, consisting of a theremin operated with ultrasonic distance sensors and a LED matrix that plays animations determined by the DJ. 

The link to a demonstration of the project is provided [here](https://www.youtube.com/watch?v=dQw4w9WgXcQ).

The functionalities of the DJ set are outlined below.

## Block Diagram
The diagram in our files illustrates how the components in our system are connected.

## Pitch Control and Output
Pitch is controlled by one of the distance sensors, i.e. the farther the distance sensor detects an object, the higher the pitch is set in the resulting output sound. This pitch is set by pulse-width modulation. The output is discretized into notes varying between two octaves, specifically C3 and C5, and played through piezo buzzers. We are using SPI to establish a master and slave connection between two MSP430 boards. This was done to double the amount of timers we had access to, but also to address how there were overlapping pin connections required by the ultrasonic distance (https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf) sensor and piezo buzzer. The master MSP430 board takes the distance sensor reading and assigns it to an 8-bit message based on the range it falls under, which then gets sent as a transmit buffer to the slave. The slave MSP430 board takes the receive buffer and plays an output frequency on the buzzer based on the message. 

The button connected to the slave changes the octave in which the note is being played.

Files used for pitch control and output: master_ultrasonic.c, slave_buzzer.c

## Volume and Brightness Control
Volume is controlled by a potentiometer, i.e. the smaller the set resistance, the larger the volume is set in the resulting output sound. This volume is set by pulse-width modulation and dicretized into various levels. The brightness of the LED matrix is scaled with the detected volume.

The following buttons add additional functionality:
- Mute Button: on press, turns off the output sound.

- File used for brightness control: master_led_matrix.c

## LED Display Control
The LED Matrix plays a pattern of our choosing and changes brightness in reponse to the distance closent to the ultrasonic sensor hooked to the respective Master MSP430. We coded 4 levels of brightness. 

## Trials And Tribulations
- We wanted to play the Bad Apple video on the LED matrix, but the MSP430 has limited storage, so we had to figure out a way to store data on the laptop and pass it through to the MSP. We decided on a UART to USB cable, but our cable was not compatible hardware-wise with the RX function of the MSP, resulting in successful transmission but inability for the MSP to relay the information further. As a result, we decided to generate our own frames instead of relying on stored video frames. 
