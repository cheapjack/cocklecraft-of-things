# Cockle Frequently Asked Questions

This page has some hints, tips and links to help with some of the common issues or questions you might have when using the Sparkfun Thing.

 * [Sparkfun Thing hookup guide](https://learn.sparkfun.com/tutorials/esp8266-thing-hookup-guide/hardware-overview) - an overview of what's what on the physical board.
 * If you get the "espcomm failed" error, try turning the Thing off and then on just after you've hit the upload button.  Getting the timing right can be a bit tricky, but persevere.
 * Once you've uploaded the code, you need to unplug the USB-Serial board from the Thing for the program to start running.
 * [DS18B20 temperature sensor data sheet](http://datasheet.octopart.com/DS18B20%2B-Dallas-Semiconductor-datasheet-11984949.pdf)

## BasicTempSender

The status LED on the BasicTempSender sketch shows different colours depending on what is happening.

When it first powers up it cycles through red, green and then blue to let you check that your wiring is all okay.  After that when it turns...

 * Blue that shows that it's connected to the WiFi and is making a request
 * Green shows that the temperature was uploaded successfully
 * Yellow means that it talked to the web server, but didn't receive a successful response
 * Red means that it failed to talk to the web server

