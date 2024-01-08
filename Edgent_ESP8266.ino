/*************************************************************
  Blynk is a platform with iOS and Android apps to control
  ESP32, Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build mobile and web interfaces for any
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: https://www.blynk.io
    Sketch generator:           https://examples.blynk.cc
    Blynk community:            https://community.blynk.cc
    Follow us:                  https://www.fb.com/blynkapp
                                https://twitter.com/blynk_app

  Blynk library is licensed under MIT license
 *************************************************************
  Blynk.Edgent implements:
  - Blynk.Inject - Dynamic WiFi credentials provisioning
  - Blynk.Air    - Over The Air firmware updates
  - Device state indication using a physical LED
  - Credentials reset using a physical Button
 *************************************************************/

/* Fill in information from your Blynk Template here */
/* Read more: https://bit.ly/BlynkInject */
//#define BLYNK_TEMPLATE_ID           "TMPxxxxxx"
//#define BLYNK_TEMPLATE_NAME         "Device"

#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define BLYNK_TEMPLATE_ID "TMPL6hd98Feu0"
#define BLYNK_TEMPLATE_NAME "doan2"
#define BLYNK_PRINT Serial
#define APP_DEBUG

// Uncomment your board, or configure a custom board in Settings.h
//#define USE_SPARKFUN_BLYNK_BOARD
//#define USE_NODE_MCU_BOARD
//#define USE_WITTY_CLOUD_BOARD
//#define USE_WEMOS_D1_MINI

#include "BlynkEdgent.h"
#define RX_PIN 4
#define TX_PIN 5
SoftwareSerial mySerial(RX_PIN,TX_PIN);
//#define BLYNK_DEBUG
char ssid[] = "Galaxy M21A9BD";
char pass[] = "trby1813";
String nhietdo, kcach, giatri;
 String getValue(String data, char separator, int index){
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
      if (data.charAt(i) == separator || i == maxIndex) {
          found++;
          strIndex[0] = strIndex[1] + 1;
          strIndex[1] = (i == maxIndex) ? i+1 : i;
      }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  WiFi.begin(ssid, pass);
  if (WiFi.status() != WL_CONNECTED) {
  Serial.println("WiFi not connected!");
}
  BlynkEdgent.begin();
}

void loop() {

  if (mySerial.available()) {
    String data = mySerial.readString();
    Serial.println(data);
    nhietdo = getValue(data, ';', 0);
    kcach = getValue(data, ';', 1);
    giatri = getValue(data, ';', 2);
    BlynkEdgent.run();
    Blynk.virtualWrite(V0, nhietdo.toFloat());
    Blynk.virtualWrite(V1, kcach.toFloat());
    Blynk.virtualWrite(V2, giatri.toFloat());
  }
}
