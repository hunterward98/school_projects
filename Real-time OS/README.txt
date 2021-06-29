This class was focused on developing real-time OS applications in C.
These were two large group projects that we did, with the motor controller
project being our final project of the semester that built upon
previous projects in the course (so they aren't included). This
includes thread scheduling, multi-threading, and other basic RTOS
functions.

These were developed using Keil uVision IDE and a Texas Instruments
TM4C123GH6PM ARM 32-bit microcontroller with an additional board
that contained an LCD, keypad, and other nifty devices. The project files
have been included if you wish to look at the projects in the IDE.
However, the directories may be wacky, so it may take some work to
get these working properly. The files you likely want to examine are
any .c and .s files, as these are the ones we worked on. You may
notice that some of these files were provided for us, but this
is largely because the projects built on each other. Once we
created the files for the first project we did, we were then provided
with starter files by the professor that required us to implement
more functions.

These were all group projects - the File System project was done
with one other person (technically two, but one person slacked heavily - 
this required us to meet with the prof to have him help us) and
the motor controller was done with five total people and took three weeks.

The File System implements basic functions to manage the disk.
This includes writing, erasing, and directory management. The majority
of the work for the project that we performed is in OS_File_System.c.
FlashPorgraam.c and Test_File_System.c were provided to us by the
professor, along with startup.s and any header files.

The Motor Controller utilizes pulse-width modulation to control a
10V DC motor. This project requires an external circuit with an
ADC to read the motor RPM. The way the project functions is quite
simple - the user enters the desired RPM [400-2400 RPM] and
the desired RPM and measured RPM are displayed on the LCD screen.
If the RPM is not within the desired range, then it adjusts the duty
cycle of the puslse-width modulator to adjust motor speed.
The motor controller is incremental - meaning it does not use any fancy
math or constants that have been derived from testing. If it's too
slow, it increments the duty cycle by one and vice versa. However,
a PID controller is included (User.c) but is commented out because
we chose to go with incremental initially and stuck with it. It should
be noted that the PID controller did perform much better at getting the
motor to the target RPM much faster. We had a lot of problems with
the project - circuit failures, forgetting to check for errors,
and initializing ports incorrectly to name a few, but we managed
to complete the whole thing as desired.