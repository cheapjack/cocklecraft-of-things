# Pushing data to a CockleRake server

1. Find out the IP name/address and port number for the CockleRake server that you're going to use.  It is often something like *192.168.0.1* with a port number of either *80* or *5000*.
1. Register your Cockle with the CockleRake server by going to *http://CockleRake-server-address:port-number* in a web browser and following the *register a new Cockle* link.
1. Once your Cockle is registered, copy the registration code to the ```COCKLE_NAME``` value in the sketch.
1. Next, find the ```WiFiMulti.addAP``` line and change it to the SSID and password for your WiFi network.
1. Finally, replace the ```COCKLERAKE SERVER ADDRESS``` text with the CockleRake server address, and likewise the ```COCKLERAKE_SERVER_PORT_NUMBER``` text with the port number.

## Links

 * [ThingSpeak documentation](https://uk.mathworks.com/help/thingspeak/index.html)

