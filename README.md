# ECE-4760 Final Project: Persistence of Vision Display
A persistence of vision display constructed from scratch using the Raspberry Pi Pico W microcontroller. Runs at 30 FPS with full RGB color, and streams images over wifi.

Project structure:
 - /PICO
	 - main.c
		 - Code entry point on the pico
	 - /drivers/apa102
		- Driver for the SPI based APA102 LEDs
	 - /utils/picow_tcp_client.c
		 - Driver for the TCP client
 - /PYTHON
	 - wifi.py
		 - TCP server for sending image data
 - /HARDWARE
	 - /CAD
		 - STL files for 3D printing the mechanical structure
	 - /ELECTRICAL
		 - KiCad project files for the two custom PCB's
