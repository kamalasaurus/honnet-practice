#include "Wire.h"
#include <MPU6050_tockn.h>
TwoWire i2c;
MPU6050 mpu6050(i2c);

//  From the Open Sound Control (OSC) library for the ESP8266/ESP32
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>

char ssid[] = "nihao";                // your network SSID (name)
char pass[] = "drixdrix";             // your network password

WiFiUDP Udp;                          // A UDP instance to let us send and receive packets over UDP
const IPAddress outIp(192,168,43,58); // remote IP of your computer
const unsigned int localPort = 8888;  // local port to listen for OSC packets (not used for sending)
const unsigned int outPort = 9999;    // remote port to receive OSC


void setup() {
    Serial.begin(115200);
    Serial.println("Hello!");

    i2c.begin(D3, D4);
    mpu6050.begin();
}

void loop() {
    // handle IMU
    mpu6050.update();
    Serial.print(mpu6050.getAccX()); Serial.print('\t');
    Serial.print(mpu6050.getAccY()); Serial.print('\t');
    Serial.print(mpu6050.getAccZ()); Serial.print('\n');

    // handle OSC
    OSCMessage msg("/IMU");
    msg.add(mpu6050.getAccX());
    msg.add(mpu6050.getAccY());
    msg.add(mpu6050.getAccZ());
    Udp.beginPacket(outIp, outPort);
    msg.send(Udp);
    Udp.endPacket();
    msg.empty();
    delay(30);
}

