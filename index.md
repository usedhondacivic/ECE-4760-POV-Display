A persistence of vision display constructed from scratch using the Raspberry Pi Pico W microcontroller. Runs at 30 FPS with full RGB color and streams images over wifi.

Created by: Michael Crum (mmc323@cornell.edu), Joseph Horwitz (jah569@cornell.edu), Rabail Makhdoom (rm857@cornell.edu)

![Three examples of the POV Display running](https://github.com/usedhondacivic/ECE-4760-final-project/blob/09e4d72bde516a27118cdfb5415e74ec57157b85/photos/POV_banner.png?raw=true)

Project Introduction (*One sentence "sound bite" that describes your project. A summary of what you did and why.*)

We built a persistance of vision display which can simulate any image with a rotating strip of LEDs.

A persistence of vision (POV) refers to the phenomenon of the human eye in which an image exists for a brief time (10 ms). It is an optical illusion in which a visual image seems to persist even when the light from it ceases to enter our eyes. Our POV display exploits this phenomena by spinning a one dimensional row of 40 LED's at such a high frequency that a two dimensional display is visible. By ensuring that the rotational speed of the LED's is fast enough, we can trasnmit images over Wifi and display them on our system such that the human eye perceives a complete two dimensional image. 

The overall design of this project can be grouped into three categories: mechanical, electrical, and software design. A spinning arm consisting of 40 light emitting diodes (LEDs) as well as a mounting station for the motor and PCBs make up the mechanical components. On the electrical end, we used multiple components: an H-bridge motor driver and Pi Pico was used to run the motor, a Pico W tranmit images to our system, a hall sensor detected the time period of each arm rotation, and an inductive supply powered up all the required components. The actual algorithm to send images over Wifi using TCP and to display them on the LEDs was implemented in C and Python.

High level design
* *Rationale and sources of your project idea*
* *background math*
* *logical structure*
* *hardware/software tradeoffs*
* *Discuss existing patents, copyrights, and trademarks which are relevant toyour project.*

The original idea for this project came from a video that Rabail saw of a similar POV display on Youtube.com. We approached our professor, Hunter Adams, to discuss whether this would actually be a viable project idea and on his confirmation we started brainstorming about the POV display. Initially, we thought of implementing the display on a propeller fan as we had seen on Youtube, but eventually we chose to build the rotor and arm of the POV display ourselves for a cleaner and more lightweight project. 

To display an image, we first convert a bitmap image to a polar coordinate system, where we define a theoretical "pixel" to be the image's color at a certain radius from the center of the image at a certain angle. The resolution of the image in this system is determined by the number of LEDs we can use (radius) and how many times we can change the LEDs in one rotation (angle). 

The logical structure of our project is relatively straightforward. First, we  Our software calculates the time duration to display each "pixel" on the display by simply measuring the period of each full rotation using a Hall Effect sensor and a magnet, then dividing by the number of rotation angles we sampled from the source image. By assigning the LEDs to a certain color for just that duration of time, we can then display the pixels and ensure proper image orientation at each rotation.

The nature of our design allows the hardware and software tradeoffs to be independent of each other. The more robust we make our software (i.e. optimized code, error-free LED mapping), the more optimized and error free our project becomes. Similarly, by making our hardware more robust (i.e. using heat sinks, tying down wires, using bigger motors), we get a higher resolution of our system and also make the project safer. 

Our project does not include any known existing patents, copyrights, or trademarks. We designed all the hardware from scratch. The software is also our original work, with some basis in the open-source example code published by Raspberry Pi.

Program/hardware design
* *program details. What parts were tricky to write?*
* *hardware details. Could someone else build this based on what you have written?*
* *Be sure to specifically reference any design or code you used from someone else.*
* *Things you tried which did not work*

The main components of our mechanical design are a motor, a spinning arm with 2 PCBs attached, and two 3D printed mounts for the motor as well as for the spinning arm which were designed and 3D printed. The mount acts as the main support for the arm as it is connected to the spinning shaft of the motor. Since the motor did not have its own self-supporting component, we also built a mount to ensure that it stays upright. This mount provides great stability while the motor is spinning at high speeds and as an additional precaution, we also clamped the main mounting bracket to a table during the demonstration. Furthermore, to ensure that we don't get too much swaying in the arm itself at the high speeds we were testing at, we positioned the electrical components on the spinning arm to provide good counterbalance on both sides and used nuts to balance the weights on both sides.

Our program had several challenges we had to overcome. First, we developed a Python script to converted bitmap images into a custom data format which could be used to display images on the POV display. Then, we established a TCP exchange to transfer the image data to the microcontroller operating the display. Wifi was our chosen communication method because it allowed us to update the image while the microcontroller was rotating, which would be impossible with a wired setup. On the microcontroller's side, we had one thread poll for incoming data and handle processing that data, while another thread maintained the display. The data came through as a stream of bytes but had to be formed into a three-dimensional array of color data: angle of rotation * LED number (0 at center) * RGB triple. This structure made displaying the image a simple continuous loop through the array. The main challenge in the program for displaying the image was in timing. The thread to change the LEDs had to run exactly as quickly as needed to loop through the entire image during one spin of the blade. This was accomplished by timing one rotational period using a hall effect sensor and magnet. The hall effect sensor attached to the spinning microcontroller would trigger an interrupt every time it passed a magnet fastened to a stationary base. By timing the interrupts and dividing by the target number of rotations, the timing of LED changes could be adjusted on the fly to guarantee a complete and steady image.

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
