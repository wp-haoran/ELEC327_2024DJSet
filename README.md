# ELEC327_2024DJSet
This repository holds the code for our (Minatel, Huang, Nguyen, Pan) final project at Rice University's ELEC 327 course. 

We sought to construct a DJ set, consisting of a theremin operated with ultrasonic distance sensors and a LED matrix that plays animations determined by the DJ. 

The link to a demonstration of the project is provided [here](https://rice.box.com/s/wwp57yu2822f0gqisk5gadgmjqpktd3a).

The functionalities of the DJ set are outlined below.

## Block Diagram
The diagram in our files illustrates how the components in our system are connected.

## Pitch Control and Output
Pitch is controlled by one of the distance sensors, i.e. the farther the distance sensor detects an object, the higher the pitch is set in the resulting output sound. This pitch is set by pulse-width modulation. The output is discretized into notes varying between two octaves, specifically C3 and C5, and played through piezo buzzers. We are using SPI to establish a master and slave connection between two MSP430 boards. This was done to double the amount of timers we had access to, but also to address how there were overlapping pin connections required by the ultrasonic distance sensor and piezo buzzer. The master MSP430 board takes the distance sensor reading and assigns it to an 8-bit message based on the range it falls under, which then gets sent as a transmit buffer to the slave. The slave MSP430 board takes the receive buffer and plays an output frequency on the buzzer based on the message. 

The button connected to the slave changes the octave in which the note is being played. On button press, we toggle between (default) 3rd octave and pulsating 5th octave notes. The choice to pulsate and skip the 4th octave was deliberately done to make the change more obvious to perceive, which helped in debugging as well and contributed to a better DJ rave atmosphere. 

Files used for pitch control and output: master_ultrasonic.c, slave_buzzer.c

## Volume and Brightness Control
Volume is controlled by a potentiometer, i.e. the smaller the set resistance, the larger the volume is set in the resulting output sound. This volume is set by pulse-width modulation and dicretized into various levels. The brightness of the LED matrix is scaled with the detected volume.

The following buttons add additional functionality:
- File used for brightness control: master_led_matrix.c

## LED Display Control
The LED Matrix plays a pattern of our choosing and changes brightness in response to the distance closest to the ultrasonic sensor hooked to the respective Master MSP430. We coded 4 levels of brightness. We also set up P1.3 to be able to pause and continue the LED matrix animation upon button press. We also implemented a button to pause the current LED matrix animation, in case there is any pattern we like.

## Trials And Tribulations
- We wanted to play the Bad Apple video on the LED matrix, but the MSP430 has limited storage, so we had to figure out a way to store data on the laptop and pass it through to the MSP. We decided on a UART to USB cable, but our cable was not compatible hardware-wise with the RX function of the MSP, resulting in successful transmission but inability for the MSP to relay the information further. As a result, we decided to generate our own frames instead of relying on stored video frames.
- At one point we postulated that button interrupts would cause difficulties when operating concurrently with watchdog timer and a large amount of serial info being sent across the MSPs, so we experimented with moving the button functionality to the master, where the strain on the WDT was not as heavy. However, this turned out to be a non-issue and so our button functionality remained in the slave code.
- 
## Datasheets for components
- WS2812B 50mm x 50mm LEDs https://cdn-shop.adafruit.com/datasheets/WS2812B.pdf
- FT232RL USB-UART Converter IC https://ftdichip.com/wp-content/uploads/2020/08/DS_FT232R.pdf
- Ultrasonic Distance Sensor https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf
