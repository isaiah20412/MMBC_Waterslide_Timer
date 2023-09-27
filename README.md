# MMBC Waterslide Timer
## Project Writeup
>[!Note]
The following is **not** a proper Readme, but an excerpt of this project's documentation as found in the *Mountain Meadows Bible Camp Facilities Handbook*.


### Summary
The basic principle is relatively simple; a laser beam firing into a sensor at the top of the slide is broken when the rider passes through it. When the sensor registers the momentary lack of the laser, a stopwatch style timer is started. Another laser/sensor assembly is located at the bottom of the slide, just before the last section of slide, and it stops the timer when the rider passes through the final beam.

### Components
The brain of the operation is an Arduino Nano. (technically it is an Elegoo Nano - a much cheaper knockoff - but the two are essentially the same) The Arduino receives analog signals from the sensors, and when the values of those signals exceeds a given threshold, it starts or stops the timer, depending on which sensor the signal came from. A two part assembly consisting of a laser and a light sensor is responsible for detecting the presence of a rider at the start and end points of the slide. The lasers are small and inexpensive red laser diodes. The sensors are Light Detection Sensor modules that use a LDR (Light Dependent Resistor) to generate either an analog or digital signal. (I am using the analog output, as it allows for a higher amount of accuracy in determining whether the laser beam is or is not blocked by the body of a rider.)

### Operation
The top laser, top sensor, bottom laser, and bottom sensor are all enclosed in small black plastic project boxes (see figure #3). The Arduino itself is enclosed in a modified conduit termination box, which also houses a small OLED display and a pushbutton used to reset the timer after each run. Another black box acts as a junction box and merges the contents of three CAT5e cables into one. The entire system is powered  by an adjustable power supply, set to 9V DC, connected to yet another black box, which houses a Buck DC-DC Step-down Power Converter  that regulates the 9V to 5V. This box also contains a rocker switch used to turn the system's power on/off. Communication between all of these boxes is accomplished by varying lengths of CAT5e cable, with the cables that run to the sensors placed inside a run of 1/2" conduit.

### Additional Notes
In addition to all this, there is a modified Thermacell© bug repelling lamp (which no longer repels bugs) that now houses a cluster of three green LED's that will glow brightly when the timer has been reset and is currently stopped, and then dim once the timer has started. This allows a method of easy and reliable method of communication between the lifeguard and the people launching the rider.

The Arduino runs a relatively simple sketch (the official name for an Arduino code file), broken down into three major sections. Section #1 - Define variables. Section #2 - Setup (initialize display and go/no go light). Section #3 Main program: this is a three part loop, which continuously checks for a need to either start the timer, end it, or calculate the current time.
