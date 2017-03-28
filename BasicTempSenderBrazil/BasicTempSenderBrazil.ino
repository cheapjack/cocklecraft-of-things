/**
 * BasicTempSender.ino
 *
 * Simple sketch to upload temperature readings taken from a DS18B20 temperature
 * sensor to the CockleRake server.
 * 
 * Part of the Cocklecraft project - https://github.com/mcqn/cocklecraft-of-things/
 * 
 * Expects the DS18B20 sensor to be connected to 3V3, GND and sensor pin 4
 *
 */

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

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

//#define COCKLE_NAME "moreton-formby-brazil"
//#define COCKLE_NAME "southport-formby-brazil"
#define COCKLE_NAME "hightown-ainsdale-brazil"

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
    
    //USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    //WiFiMulti.addAP("SSID", "PASSWORD");
    WiFiMulti.addAP("domesticscience", "raspberry");
    //WiFiMulti.addAP("Power5", "jack50n3000123");
    digitalWrite(kLEDPin, LOW);

    gSensors.begin();
}

void loop() {
    // wait for WiFi connection
    if((WiFiMulti.run() == WL_CONNECTED)) {

        // We're connected!  Yay!  Let's show that by turning the status LED blue
        digitalWrite(kBluePin, LOW);

        HTTPClient http;
        digitalWrite(kLEDPin, HIGH);
        gSensors.requestTemperatures();
        USE_SERIAL.print("[HTTP] begin...\n");

        // Prepare the data we need to send to the server
        String temperature = "temperature=";
        temperature += gSensors.getTempCByIndex(0);

        String path = "/";
        path += COCKLE_NAME;
        // Prepare the HTTP request
        //http.begin("COCKLERAKE SERVER ADDRESS", COCKLERAKE_SERVER_PORT_NUMBER, path); //HTTP
        //http.begin("domesticscience.local", 5000, path); //HTTP
        http.begin("172.24.1.1", 5000, path); //HTTP
        //http.begin("192.168.0.5", 5000, path); //HTTP
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");

        USE_SERIAL.print("[HTTP] POST...\n");
        // start connection and send HTTP header
        int httpCode = http.POST(temperature);
        if(httpCode) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] POST... code: %d\n", httpCode);
            // Flash the LED to show we're doing something
            digitalWrite(kLEDPin, LOW);
            delay(300);
            digitalWrite(kLEDPin, HIGH);
            delay(300);
            digitalWrite(kLEDPin, LOW);

            // We've successfully made a web request, turn the green LED on...
            digitalWrite(kBluePin, HIGH);
            digitalWrite(kGreenPin, LOW);
            // file found at server
            if(httpCode == 200) {
                // ...and this will just show the green LED to show success...
                String payload = http.getString();
                USE_SERIAL.println(payload);
            } else {
                // ...but if we get here we got an unexpected response from the server, so make
                // the status LED yellow
                digitalWrite(kRedPin, LOW);
            }
        } else {
            // Things haven't gone well.  Make the status LED red
            digitalWrite(kLEDPin, LOW);
            digitalWrite(kBluePin, HIGH);
            digitalWrite(kGreenPin, HIGH);
            digitalWrite(kRedPin, LOW);
            USE_SERIAL.print("[HTTP] GET... failed, no connection or no HTTP server\n");
        }
    }

    delay(4000);
    // Reset the status LED
    digitalWrite(kRedPin, HIGH);
    digitalWrite(kGreenPin, HIGH);
    digitalWrite(kBluePin, HIGH);
    // Wait for a while before taking the next reading
    delay(6000);
}

