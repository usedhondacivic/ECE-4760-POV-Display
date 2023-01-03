# ECE-4760 Final Project: Persistence of Vision Display
A persistence of vision display constructed from scratch using the Raspberry Pi Pico W microcontroller. Runs at 30 FPS with full RGB color and streams images over wifi.

Created by: Michael Crum (mmc323@cornell.edu), Joseph Horwitz (jah569@cornell.edu), Rabail Makhdoom (rm857@cornell.edu)

Read the write up: https://michael-crum.com/ECE-4760-final-project/

![Three examples of the POV Display running](https://github.com/usedhondacivic/ECE-4760-final-project/blob/09e4d72bde516a27118cdfb5415e74ec57157b85/photos/POV_banner.png)

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
- /new_python
	- New experimental python setup, including a browser based interface and support for most image types, gif's, and mp4 video.
	- web_server.py
		- Entry point for the program, launches the webserver which serves the web interface on localhost port 8080 (localhost:8080 in your web browser)
	- tcp_server.py
		- Handles connecting to the display over TCP, and sending arbitry data over the connection
	- file_translation.py
		- Translates image data to a radial RGB format used by the display
	- display_driver.py
		- Handles the transition from a file name to the proper byte format, then sends packets to the display using functions from tcp_server.py
	-  index.html
		- The html file served by the browser client in web_server.py

 - /HARDWARE
	 - /CAD
		 - STL files for 3D printing the mechanical structure
	 - /ELECTRICAL
		 - KiCad project files for the rotor and control board PCBs
