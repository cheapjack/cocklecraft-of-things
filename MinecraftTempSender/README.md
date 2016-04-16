# Pushing data to Minecraft

1. Set up or find a Minecraft server with the [RaspberryJuice mod](https://github.com/zhuowei/RaspberryJuice) installed.
1. Find the x/y/z coordinates of the location where you want to show the temperature.
1. Update the MinecraftTempSender.ino code to let it know where to draw the temperature graph.  The bottom of the graph will be at the location you set in ```kFromX```, ```kFromY``` and ```kFromZ```, and the top of the graph will be at the location you set in ```kToX```, ```kToY``` and ```kToZ```.
1. [Optional] If you want, you can adjust the temperature range that the display responds to by altering ```kMinTemp``` and ```kMaxTemp```.
1. [Optional] By default it will draw the graph in glowstone, and the "undrawn" portion of the graph will be filled in with air.  If you want to change either of these then you need to set the right block type ID for ```kFilledInBlockType``` and/or ```kEmptyBlockType```.  Check the [Minecraft wiki data values page](http://www.minecraftwiki.net/wiki/Data_values_(Pocket_Edition)) to find the right ID value to use.
1. Find the ```WiFiMulti.addAP``` line and change it to the SSID and password for your WiFi network.
1. Upload the sketch to your Cockle.

It will now report the temperature to the server every 10 seconds.

## Links

 * [RaspberryJuice mod](https://github.com/zhuowei/RaspberryJuice)
 * [Minecraft wiki data values page](http://www.minecraftwiki.net/wiki/Data_values_(Pocket_Edition))

