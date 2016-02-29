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

#define COCKLE_NAME "meols-formby-mersey"

void setup() {

    pinMode(kLEDPin, OUTPUT);
    USE_SERIAL.begin(115200);
    digitalWrite(kLEDPin, HIGH);
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
    WiFiMulti.addAP("DoESLiverpool", "decafbad00");
    digitalWrite(kLEDPin, LOW);

    gSensors.begin();
}

void loop() {
    // wait for WiFi connection
    if((WiFiMulti.run() == WL_CONNECTED)) {

        HTTPClient http;
        digitalWrite(kLEDPin, HIGH);
        gSensors.requestTemperatures();
        USE_SERIAL.print("[HTTP] begin...\n");
        // configure traged server and url
        //http.begin("192.168.1.12", 443, "/test.html", true, "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS

        // Prepare the data we need to send to the server
        String temperature = "temperature=";
        temperature += gSensors.getTempCByIndex(0);
        //sprintf(temperature, "temperature=%f", gSensors.getTempCByIndex(0));

        String path = "/";
        path += COCKLE_NAME;
        //sprintf(path, "/%s", COCKLE_NAME);
        http.begin("192.168.0.15", 5000, path); //HTTP
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");

        USE_SERIAL.print("[HTTP] POST...\n");
        // start connection and send HTTP header
        int httpCode = http.POST(temperature);
        if(httpCode) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);
            // Flash the LED to show we're doing something
            digitalWrite(kLEDPin, LOW);
            delay(300);
            digitalWrite(kLEDPin, HIGH);
            delay(300);
            digitalWrite(kLEDPin, LOW);

            // file found at server
            if(httpCode == 200) {
                String payload = http.getString();
                USE_SERIAL.println(payload);
            }
        } else {
            digitalWrite(kLEDPin, LOW);
            USE_SERIAL.print("[HTTP] GET... failed, no connection or no HTTP server\n");
        }
    }

    delay(10000);
}

