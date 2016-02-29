# Cockle Software Setup

## Hardware Requirements

 * A Sparkfun ESP8266 Thing board
 * A micro-USB cable
 * A USB-serial cable (e.g. FTDI or CP2102) **At the moment this has only been tested with an Arduino USB2SERIAL board**
 * A computer

## Set up your development environment

 1. Install the latest version of the Arduino IDE - [https://www.arduino.cc/en/Main/Software](https://www.arduino.cc/en/Main/Software)
 1. Plug the USB-serial board into the FTDI section of the Thing's header pins - the section marked DTR, TXO, RXI, 3V3, NC, GND.  Make sure the GND pins on both the Thing and the USB-Serial board match.
 1. Plug the other end of the USB-serial board into your computer
 1. Plug the micro-USB cable into the Thing
 1. Plug the other end of the micro-USB cable into your computer
 1. Run the Arduino IDE and follow the instructions in [Sparkfun's Thing Guide](https://learn.sparkfun.com/tutorials/esp8266-thing-hookup-guide/installing-the-esp8266-arduino-addon) to install the add-on for the ESP8266.

## Install Additional Libraries

For the temperature sensor we'll need an extra library.

 1. Run the Arduino IDE
 1. Open the library manager.  From the menu choose:
     Sketch -> Include Library -> Manage Libraries...
 1. Type "dallastemperature" into the search box in the top right corner.
 1. Click on the library by Miles Burton and others.
 1. Click on the Install button.

