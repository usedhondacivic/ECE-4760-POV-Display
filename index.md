A persistence of vision display constructed from scratch using the Raspberry Pi Pico W microcontroller. Runs at 30 FPS with full RGB color and streams images over wifi.

Created by: Michael Crum (mmc323@cornell.edu), Joseph Horwitz (jah569@cornell.edu), Rabail Makhdoom (rm857@cornell.edu)

![Three examples of the POV Display running](https://github.com/usedhondacivic/ECE-4760-final-project/blob/09e4d72bde516a27118cdfb5415e74ec57157b85/photos/POV_banner.png?raw=true)

Project Introduction (*One sentence "sound bite" that describes your project. A summary of what you did and why.*)

A persistence of vision (POV) refers to the phenomenon of the human eye in which an image exists for a brief time (10 ms). It is an optical illusion in which a visual image seems to persist even when the light from it ceases to enter our eyes. Our POV display exploits this phenomena by spinning a one dimensional row of 40 LED's at such a high frequency that a two dimensional display is visible. By ensuring that the rotational speed of the LED's is fast enough, we can trasnmit images over Wifi and display them on our system such that the human eye perceives a complete two dimensional image. 

The overall design of this project can be grouped into three categories: mechanical, electrical, and software design. A spinning arm consisting of 40 LEDs as well as a mounting station for the motor and PCBs make up the mechanical components. On the electrical end, we used multiple components: an H-bridge motor driver and Pi Pico was used to run the motor, a Pico W tranmit images to our system, a hall sensor detected the time period of each arm rotation, and an inductive supply powered up all the required components. The actual algorithm to send images over Wifi using TCP and to display them on the LEDs was implemented in C and Python.

High level design
* *Rationale and sources of your project idea*
* *background math*
* *logical structure*
* *hardware/software tradeoffs*
* *Discuss existing patents, copyrights, and trademarks which are relevant toyour project.*

Program/hardware design
* *program details. What parts were tricky to write?*
* *hardware details. Could someone else build this based on what you have written?*
* *Be sure to specifically reference any design or code you used from someone else.*
* *Things you tried which did not work*

Results of the design
* *Any and all test data, scope traces, waveforms, etc*
* *speed of execution (hesitation, filcker, interactiveness, concurrency)*
* *accuracy (numeric, music frequencies, video signal timing, etc)*
* *how you enforced safety in the design.*
* *usability by you and other people*

Conclusions
* *Analyse your design in terms of how the results met your expectations. What might you do differently next time?*
* *How did your design conform to the applicable standards?*
* *Intellectual property considerations.*
    * *Did you reuse code or someone else's design? Did you use any of Altera's IP?*
    * *Did you use code in the public domain?*
    * *Are you reverse-engineering a design? How did you deal with patent/trademark issues?*
    * *Did you have to sign non-disclosure to get a sample part?*
    * *Are there patent opportunites for your project?*
