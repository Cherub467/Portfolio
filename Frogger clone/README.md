<h1>Frogger Clone</h1>

Authors: Thanh Hien Nguyen-Mai & Lauren Mayes

Assignment provided by: Dr. Jalal Kawash

This was a project for Machine Coding 2, at the University of Calagry. We were tasked to build a game like classic frogger that runs on the RasberryPi using C language. We 
implemented getting SNES controller input by syncing the machines clock with what button was being pressed. framebuffer.o/.h, initGPIO.o/h and the make file were provided. All 
other files were edited equally between Than and I.

To run our file you will just need a rasberryPi and a SNES controller. Firstly you need download the 359-Game folder to a RasberryPi, then in the terminal/Command line navigate to 
the address where you stored the folder. Type the command "make" and press enter to run it, and then run "./myProg".

Some known bugs in our system:
 - We had to manually reset the button pressed to 0 multiple times to prevent 1 button pressed from being processed multiple times.
 - If a button is pressed to quickly not all the presses will be registered.
