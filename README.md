# ELEC327_2024DJSet
This repository holds the code for our (Minatel, Huang, Nguyen, Pan) final project at Rice University's ELEC 327 course. 

We sought to construct a DJ set, consisting of a theremin operated with ultrasonic distance sensors and a LED matrix that plays animations determined by the DJ. 

The link to a demonstration of the project is provided [here](https://www.youtube.com/watch?v=dQw4w9WgXcQ).

The functionalities of the DJ set are outlined below.

## Block Diagram
The diagram below illustrates how the components in our system are connected.

(insert image here)

## Pitch Control and Output
Pitch is controlled by one of the distance sensors, i.e. the farther the distance sensor detects an object, the higher the pitch is set in the resulting output sound. This pitch is set by pulse-width modulation. The output is discretized into notes varying between two octaves, specifically C3 and C5, and played through piezo buzzers.

The following buttons add additional functionality:
- Minor Triad Button: when held, sets output sound to minor triad. (if we have time)
- Major Triad Button: when held, sets output sound to major triad.
The latter two chords are output through multiple piezo buzzers.

## Volume and Brightness Control
Volume is controlled by a potentiometer, i.e. the smaller the set resistance, the larger the volume is set in the resulting output sound. This volume is set by pulse-width modulation and dicretized into various levels. The brightness of the LED matrix is scaled with the detected volume.

The following buttons add additional functionality:
- Mute Button: on press, turns off the output sound.

## LED Display Control
When the device is on, the LED matrix enters the default state where an animation based on the music video for _[Bad Apple!!](https://www.youtube.com/watch?v=9lNZ_Rnr7Jc)_ plays indefinitely. The volume set by the potentiometer controls the brightness of the LEDs in the LED matrix. 

The following buttons add additional functionality:
- Animation Control Button: on press, enters or exits a state in which the animation is paused and the frame can be controlled by adjusting pitch. (if we have time)
- Color Button: on press, changes the color of the LEDs in the animation in a set order.
