/******************************************
 *
 * This example works for both Industrial and STEM users.
 *
 * Developed by Jose Garcia, https://github.com/jotathebest/
 *
 * ****************************************/

/****************************************
 * Include Libraries
 ****************************************/
#include "UbidotsEsp32Mqtt.h"

/****************************************
 * Define Constants
 ****************************************/
const char *UBIDOTS_TOKEN = "BBFF-LtAIEbHEpPlRavdXFOC9Nu8SRnTN9y";  // Put here your Ubidots TOKEN
const char *WIFI_SSID = "sunag1";      // Put here your Wi-Fi SSID
const char *WIFI_PASS = "sunag1234";      // Put here your Wi-Fi password
const char *DEVICE_LABEL = "esp32";   // Put here your Device label to which data  will be published
const char *VARIABLE_LABEL = "soil-moisture"; // Put here your Variable label to which data  will be published

const int PUBLISH_FREQUENCY = 2000; // Update rate in milliseconds

unsigned long timer;
uint8_t analogPin = 35; // Pin used to read data from GPIO35 ADC_CH6.

//percentage variables
float x_new = 0.0;
int a = 0;
int b = 100;

int min_x = 900;
int max_x = 3200;
int x = 12;

Ubidots ubidots(UBIDOTS_TOKEN);

/****************************************
 * Auxiliar Functions
 ****************************************/

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

/****************************************
 * Main Functions
 ****************************************/

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  ubidots.setDebug(true);  // uncomment this to make debug messages available
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  ubidots.setCallback(callback);
  ubidots.setup();
  ubidots.reconnect();

  timer = millis();
}

void loop()
{
  // put your main code here, to run repeatedly:
  



  if (!ubidots.connected())
  {
    ubidots.reconnect();

  }
  if (abs((int)(millis() - timer)) > PUBLISH_FREQUENCY) // triggers the routine every 2 seconds
  {
    float value = analogRead(analogPin);
    Serial.print(value);


    // float percentage;
    // x_new = b - ((b-a)*(value-min_x))/(max_x-min_x) + a;

    ubidots.add(VARIABLE_LABEL, value); // Insert your variable Labels and the value to be sent
    ubidots.publish(DEVICE_LABEL);

    timer = millis();
  }
  ubidots.loop();
}