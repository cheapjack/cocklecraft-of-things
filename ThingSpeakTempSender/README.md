# Pushing data to ThingSpeak

1. Sign up for an account on [ThingSpeak.com](http://thingspeak.com)
1. Follow the steps to [create a new channel](https://uk.mathworks.com/help/thingspeak/collect-data-in-a-new-channel.html) to hold the data.  The important fields to fill in are *name* and *field 1*.  Feel free to fill in as many of the others as you like - the *tags* field is good to help other people (or future you) know more about the data being recorded (if you include a 'cockle' tag then we can all see which channels are recording data from Cockles); ticking the *make public* option will let other people see the data you're recording; and the *latitude* and *longitude* are good to show where your Cockle is located.  To get an idea of values to use, here's what we used for our [test Cockle temperature channel](https://thingspeak.com/channels/96166):
  * **Name** Cockle temperature feed
  * **Description** Test channel to receive data from a Cockle
  * **Field 1** Temperature (C)
  * **Tags** temperature, Celsius, Ambient, cockle
  * **Make Public** Yes
  * **URL** https://github.com/mcqn/cocklecraft-of-things
  * **Show Location** Yes
  * **Latitude** 53.405
  * **Longitude** -2.985
1. Once your channel is created, click on the *API Keys* view and copy the *Write API Key* to the ```THINGSPEAK_API_KEY``` value in the sketch.
1. Find the ```WiFiMulti.addAP``` line and change it to the SSID and password for your WiFi network.

## Links

 * [ThingSpeak documentation](https://uk.mathworks.com/help/thingspeak/index.html)

