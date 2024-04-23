# ELEC327_2024DJSet
This repository holds the code for our (Cerqueira, Huang, Nguyen, Pan) final project at Rice University's ELEC 327 course. 

We sought to construct a DJ set, consisting of a theremin operated with ultrasonic distance sensors and a LED matrix that plays animations determined by the DJ. One ultrasonic distance sensor of the theremin controls pitch, and the other sensor controls volume. The sound is produced by pulse-width modulation and output through piezo buzzers. The buttons control whether the sound output is a single note, a major triad, or a minor triad. The latter two chords are output through multiple piezo buzzers.

For the LED matrix, the default state of the animation is an infinite loop of preset frames. The volume set by the DJ controls the brightness of the LEDs in the LED matrix. The buttons also allow the DJ the ability to pause the animation, control what frame is shown by the LED matrix by adjusting pitch, as well as switch between multiple different preset animations.

*EDIT* We will use the ultrasonic distance sensor to control the buzzer pitch and LED matrix. The buzzer volume can be adjusted with a potentiometer. 
