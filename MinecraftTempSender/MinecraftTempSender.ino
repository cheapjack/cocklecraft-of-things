/**
 * MinecraftTempSender.ino
 *
 * Simple sketch to upload temperature readings taken from a DS18B20 temperature
 * sensor to a Minecraft server with the RaspberryJuice mod installed.
 * 
 * Part of the Cocklecraft project - https://github.com/mcqn/cocklecraft-of-things/
 * 
 * Expects the DS18B20 sensor to be connected to 3V3, GND and sensor pin 4
 *
 */

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>

#include <OneWire.h>
#include <DallasTemperature.h>

const int kTemperaturePin = 4;
OneWire gOneWire(kTemperaturePin);

DallasTemperature gSensors(&gOneWire);

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;

const int kLEDPin = 5;
// Pins for the RGB LED
const int kRedPin = 0;
const int kGreenPin = 12;
const int kBluePin = 13;

// Details for the Minecraft server we need to talk to
const char kMinecraftServer[] = "mc.fact.co.uk";
const int kMinecraftPort = 4711;
// Location where the temperature bar graph should be drawn
// It is drawn vertically - in the Y direction.
const int kFromX = 54;
const int kFromY = 7;
const int kFromZ = 128;
const int kToX = 54;
const int kToY = 30;
const int kToZ = 128;
// What temperature range does the graph show?
const float kMinTemp = 0.0;
const float kMaxTemp = 35.0;
// What type of blocks should we use for the "filled in" and "empty"
// parts of the graph?  0-108, see http://www.minecraftwiki.net/wiki/Data_values_(Pocket_Edition)
// for a list of block IDs to use
const int kFilledInBlockType = 89;
const int kEmptyBlockType = 0;

void setup() {

    pinMode(kLEDPin, OUTPUT);
    pinMode(kRedPin, OUTPUT);
    pinMode(kGreenPin, OUTPUT);
    pinMode(kBluePin, OUTPUT);
    USE_SERIAL.begin(115200);
    digitalWrite(kLEDPin, HIGH);
    // Check that the RGB LED is wired up right...
    // Because it's a common anode, setting a colour to HIGH will turn it off
    digitalWrite(kGreenPin, HIGH);
    digitalWrite(kBluePin, HIGH);
    digitalWrite(kRedPin, LOW);
    delay(300);
    digitalWrite(kGreenPin, LOW);
    digitalWrite(kRedPin, HIGH);
    delay(300);
    digitalWrite(kBluePin, LOW);
    digitalWrite(kGreenPin, HIGH);
    delay(300);
    digitalWrite(kBluePin, HIGH);
    
   // USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    //WiFiMulti.addAP("SSID", "PASSWORD");
    WiFiMulti.addAP("WIFI NETWORK NAME/SSID", "WIFI PASSWORD");
    digitalWrite(kLEDPin, LOW);

    gSensors.begin();
}

void loop() {
    // wait for WiFi connection
    if((WiFiMulti.run() == WL_CONNECTED)) {

        // We're connected!  Yay!  Let's show that by turning the status LED blue
        digitalWrite(kBluePin, LOW);

        digitalWrite(kLEDPin, HIGH);

        // Read the temperature
        gSensors.requestTemperatures();
        float temperature = gSensors.getTempCByIndex(0);

        // Work out how much of the bar graph we should fill in
        float constrainedTemp = constrain(temperature, kMinTemp, kMaxTemp);
        float midPoint = mapFloat(constrainedTemp, kMinTemp, kMaxTemp, kFromY, kToY);
        // Round it to the nearest integer (converting a float to an integer
        // just throws away the part after the decimal point, so adding
        // 0.5 to it beforehand is a quick trick to round it properly)
        int midY = midPoint + 0.5;
        
        WiFiClient connection;
        if (connection.connect(kMinecraftServer, kMinecraftPort))
        {
            // We're connected, send the details to draw...
            // ...first the filled in part...
            connection.print("world.setBlocks(");
            connection.print(kFromX);
            connection.print(",");
            connection.print(kFromY);
            connection.print(",");
            connection.print(kFromZ);
            connection.print(",");
            connection.print(kToX);
            connection.print(",");
            connection.print(midY);
            connection.print(",");
            connection.print(kToZ);
            connection.print(",");
            connection.print(kFilledInBlockType);
            connection.println(")");
            // ...and then the "empty" part
            if (midY < kToY)
            {
              // But only if the filled in part doesn't
              // take up the entire graph
              connection.print("world.setBlocks(");
              connection.print(kFromX);
              connection.print(",");
              connection.print(midY+1);
              connection.print(",");
              connection.print(kFromZ);
              connection.print(",");
              connection.print(kToX);
              connection.print(",");
              connection.print(kToY);
              connection.print(",");
              connection.print(kToZ);
              connection.print(",");
              connection.print(kEmptyBlockType);
              connection.println(")");
            }
            // We've successfully sent the request, turn the green LED on...
            digitalWrite(kBluePin, HIGH);
            digitalWrite(kGreenPin, LOW);
            // We're done now, close the connection
            connection.stop();
        } else {
            // Things haven't gone well.  Make the status LED red
            digitalWrite(kLEDPin, LOW);
            digitalWrite(kBluePin, HIGH);
            digitalWrite(kGreenPin, HIGH);
            digitalWrite(kRedPin, LOW);
            USE_SERIAL.print("connection failed\n");
        }
    }

    delay(4000);
    // Reset the status LED
    digitalWrite(kRedPin, HIGH);
    digitalWrite(kGreenPin, HIGH);
    digitalWrite(kBluePin, HIGH);
    // Wait for a while before taking the next reading
    delay(6000UL);
}

/*
 * mapFloat(float aVal, float aFromLow, float aFromHigh, float aToLow, float aToHigh)
 * Maps a float value (aVal) that ranges from aFromLow to aFromHigh to a new range 
 * defined by aToLow and aToHigh
 * aVal - The Value to Map
 * aFromLow - The minimum input value
 * aToLow - The maximum input value
 * aToLow - the minimum output value
 * aToHigh - the maximum output
 */
float mapFloat(float aVal, float aFromLow, float aFromHigh, float aToLow, float aToHigh)
{
  return (aVal - aFromLow) * (aToHigh - aToLow) / (aFromHigh - aFromLow) + aToLow;
}

