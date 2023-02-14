/****************************************
 * Include Libraries
 ****************************************/
#include <String.h>
#include <Arduino.h>
#include "UbidotsEsp32Mqtt.h"
#include "WiFi.h"
#include "ThingSpeak.h"

#define RELAY_PIN 26
#define CHANNEL_ID 1958878
#define CHANNEL_API_KEY "KABEELQGDX1QB5WH"

/****************************************
 * Define Constants!
 ****************************************/
const char *UBIDOTS_TOKEN = "BBFF-LtAIEbHEpPlRavdXFOC9Nu8SRnTN9y";  // Put here your Ubidots TOKEN
const char *WIFI_SSID = "chatgpt";      // Put here your Wi-Fi SSID
const char *WIFI_PASS = "iotmans!";      // Put here your Wi-Fi password
const char *DEVICE_LABEL = "esp32";   // Put here your Device label to which data  will be published
const char *VARIABLE_LABEL = "soil-moisture"; // Put here your Variable label to which data  will be published
const char *VARIABLE_LABEL_SUBSCRIBE = "blue-light"; // Replace with your variable label to subscribe to

const int PUBLISH_FREQUENCY = 2000; // Update rate in milliseconds

String global_ubi = "a";

unsigned long timer;
uint8_t analogPin = 35; // Pin used to read data from GPIO35 ADC_CH6.
int counter = 0;
WiFiClient client;

Ubidots ubidots(UBIDOTS_TOKEN);

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

  // if ((char)payload[0]=='1'){ // Turn the relay ON
  //   digitalWrite(RELAY_PIN, HIGH);
  //   digitalWrite(LED_BUILTIN, HIGH); 
  // }
  // else{ // Turn the relay OFF
  //   digitalWrite(RELAY_PIN, LOW);
  //   digitalWrite(LED_BUILTIN, LOW);
  // }

  global_ubi = (char)payload[0];



}

/****************************************
 * Main Functions
 ****************************************/
void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);

  Serial.begin(115200);
  ubidots.setDebug(true);  // uncomment this to make debug messages available
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  ubidots.setCallback(callback);
  ubidots.setup();
  ubidots.reconnect();

  timer = millis();
  ubidots.subscribeLastValue("esp32","blue-light");

  ThingSpeak.begin(client);

}

void loop() {
  // put your main code here, to run repeatedly:
  counter++;
  // Serial.println(global_ubi);

  if(global_ubi=="1"){ // Turn the relay ON
    // Serial.println(global_ubi);
    digitalWrite(RELAY_PIN, HIGH);
    digitalWrite(LED_BUILTIN, HIGH); 
  }
  else if (global_ubi=="0"){ // Turn the relay OFF
    digitalWrite(RELAY_PIN, LOW);
    digitalWrite(LED_BUILTIN, LOW);
  }

  else if (global_ubi=="3"){
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(200);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(200);    
  }

  if (!ubidots.connected())
  {
    ubidots.reconnect();
    ubidots.subscribeLastValue("esp32","blue-light");
  }
  if (abs((int)(millis() - timer)) > PUBLISH_FREQUENCY) // triggers the routine every 2 seconds
    {
      // ubidots.add(VARIABLE_LABEL, value); // Insert your variable Labels and the value to be sent
      // ubidots.publish(DEVICE_LABEL);

      timer = millis();
    }
  
  int soilMoisture = analogRead(35);

  // float value = analogRead(35);
  // Serial.print(value);

  // float percentage;
  // x_new = b - ((b-a)*(value-min_x))/(max_x-min_x) + a;

  ThingSpeak.setField(1, counter);
  ThingSpeak.setField(2, WiFi.RSSI());
  ThingSpeak.setField(3, soilMoisture);
  // ThingSpeak.setField(4, led);
  ThingSpeak.writeFields(CHANNEL_ID, CHANNEL_API_KEY);

  ubidots.loop();
}
