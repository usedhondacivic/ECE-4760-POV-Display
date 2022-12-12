A persistence of vision display constructed from scratch using the Raspberry Pi Pico W microcontroller. Runs at 30 FPS with full RGB color and streams images over wifi.

Created by: Michael Crum (mmc323@cornell.edu), Joseph Horwitz (jah569@cornell.edu), Rabail Makhdoom (rm857@cornell.edu)

![Three examples of the POV Display running](https://github.com/usedhondacivic/ECE-4760-final-project/blob/09e4d72bde516a27118cdfb5415e74ec57157b85/photos/POV_banner.png?raw=true)

Insert write up here :)

Project Introduction (*One sentence "sound bite" that describes your project. A summary of what you did and why.*)

A persistence of vision (POV) refers to the phenomenon of the human eye in which an afterimage exists for a brief time (10 ms). It is an optical illusion in which a visual image seems to persist even when the light from it ceases to enter our eyes. Our POV display exploits this phenomena by spinning a one dimensional row of 40 LED's through a two dimensional space at such a high frequency that a two dimensional display is visible. The stream of light is repeatedly interrupted for brief instances and does not enter our eyes during those durations but by ensuring that the rotational speed of the LED's is fast enough, the human eye perceives a complete two dimensional image.

This is the basic principle behind the making of the animated movies.

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
