#include <String.h>
#include <Arduino.h>
#include "UbidotsEsp32Mqtt.h"
#include <WiFi.h>
#include "ThingSpeak.h"

#define RELAY_PIN 26
#define CHANNEL_ID 2028983
#define CHANNEL_API_KEY "M39WL405TXHKKLT8"


unsigned long previousMillis = 0;
unsigned long interval = 30000;


// for flow sensor
#define SENSOR 35

long currentMillis2 = 0;
long previousMillis2 = 0;
int interval2 = 1000;

float calibrationFactor = 4.5;
volatile byte pulseCount;
byte pulse1Sec = 0;
float flowRate;
unsigned long flowMilliLitres;
unsigned int totalMilliLitres;
float flowLitres;
float totalLitres;

int count;

const char* ssid = "BMS-IoT";
const char* password = "IoT_Project1";

/****************************************
 * Define UBIDOTS Constants
 ****************************************/
const char* WIFI_SSID = "BMS-IoT";
const char* WIFI_PASS = "IoT_Project1";
const char *UBIDOTS_TOKEN = "BBFF-LtAIEbHEpPlRavdXFOC9Nu8SRnTN9y";
const char *DEVICE_LABEL = "esp32";
const char *VARIABLE_LABEL_SUBSCRIBE = "blue-light";
const int PUBLISH_FREQUENCY = 2000;
unsigned long timer;

// api command
String global_ubi = "a";

WiFiClient client;
Ubidots ubidots(UBIDOTS_TOKEN);

void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info){
  // Serial.println("Connected to AP successfully!");
  if (digitalRead(RELAY_PIN) == HIGH) {
    Serial.println("Relay is OFF (HIGH) [inside WiFiStationConnected]");
  }
  else {
    Serial.println("Relay is ON (LOW) [inside WiFiStationConnected]");
  }

  // ubidots.subscribeLastValue("esp32", "blue-light");
  
}

void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info){
  // Serial.println("WiFi connected");
  // Serial.println("IP address: ");
  // Serial.println(WiFi.localIP());

  if (digitalRead(RELAY_PIN) == HIGH) {
    Serial.println("Relay is OFF (HIGH) [inside WiFiGotIP]");
  }
  else {
    Serial.println("Relay is ON (LOW) [inside WiFiGotIP]");
  }

  // ubidots.subscribeLastValue("esp32", "blue-light");

}

void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info){

  // Blink the LED
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  //end of blink


  // Serial.println("Disconnected from WiFi access point");

  digitalWrite(RELAY_PIN, HIGH);
  // digitalWrite(LED_BUILTIN, HIGH);

  if (digitalRead(RELAY_PIN) == HIGH) {
    Serial.println("Relay is OFF (HIGH) [inside WiFiStationDisconnected]");
  }
  else {
    Serial.println("Relay is ON (LOW) [inside WiFiStationDisconnected]");
  }

  // Serial.print("WiFi lost connection. Reason: ");
  // Serial.println(info.wifi_sta_disconnected.reason);
  // Serial.println("Trying to Reconnect");

  // WiFi.begin(ssid, password);
  // Serial.println("Before 5000 delay [inside WiFiStationDisconnected]");
  // delay(5000);
  // Serial.println("After 5000 delay [inside WiFiStationDisconnected]");
  // ESP.restart();
}

// flow sensor function
void IRAM_ATTR pulseCounter()
{
    pulseCount++;
}

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
    global_ubi = (char)payload[0];
}

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(RELAY_PIN, OUTPUT);
    pinMode(SENSOR, INPUT_PULLUP);

    Serial.begin(115200);

    // delete old config
    WiFi.disconnect(true);
    delay(1000);

    WiFi.onEvent(WiFiStationConnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
    WiFi.onEvent(WiFiGotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
    WiFi.onEvent(WiFiStationDisconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

    // turn valve OFF and LED ON by default
    digitalWrite(RELAY_PIN, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
    if (digitalRead(RELAY_PIN) == HIGH) {
      Serial.println("Relay is OFF (HIGH) [from setup]");
    }
    else {
      Serial.println("Relay is ON (LOW) [from setup]");
    }
    //end of valve OFF

    // flow sensor
    pulseCount = 0;
    flowRate = 0.0;
    flowMilliLitres = 0;
    totalMilliLitres = 0;
    previousMillis2 = 0;
    attachInterrupt(digitalPinToInterrupt(SENSOR), pulseCounter, FALLING);
    // end of flow sensor


    // WiFi.begin(ssid, password);
    
    Serial.println();
    Serial.println();
    Serial.println("Wait for WiFi... ");




    ubidots.setDebug(true); // uncomment this to make debug messages available
    ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
    ubidots.setCallback(callback);
    ubidots.setup();
    ubidots.reconnect();
    timer = millis();
    ubidots.subscribeLastValue("esp32", "blue-light");

    ThingSpeak.begin(client);
}

void loop()
{
  unsigned long currentMillis = millis();
  /*if condition to check wifi reconnection*/
  if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >=interval)) {
    Serial.print(millis());
    Serial.println("Reconnecting to WiFi...");
    WiFi.disconnect();
    WiFi.reconnect();
    previousMillis = currentMillis;
  }

  // if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >=interval)) {
  //   Serial.print(millis());
  //   Serial.println("Reconnecting to WiFi...");
    
  //   // Blink the LED
  //   digitalWrite(LED_BUILTIN, HIGH);
  //   delay(500);
  //   digitalWrite(LED_BUILTIN, LOW);
  //   delay(500);
  //   //end of blink

  //   WiFi.disconnect();
  //   WiFi.reconnect();
  //   previousMillis = currentMillis;
  //   ubidots.subscribeLastValue("esp32", "blue-light");
  // }


    if (global_ubi == "1")
    {
        // Turn the relay OFF
        // Serial.println(global_ubi);
        digitalWrite(RELAY_PIN, HIGH);
        digitalWrite(LED_BUILTIN, HIGH);
        if (digitalRead(RELAY_PIN) == HIGH) {
          Serial.println("Relay is OFF (HIGH) [from global_ubi == 1]");
        }
    }
    else if (global_ubi == "0")
    {
        // Turn the relay ON
        digitalWrite(RELAY_PIN, LOW);
        digitalWrite(LED_BUILTIN, LOW);
        if (digitalRead(RELAY_PIN) == LOW) {
          Serial.println("Relay is ON (LOW) [from global_ubi == 0]");
        }
    }

    if (!ubidots.connected())
    {
        // turn valve OFF and LED ON by default
        digitalWrite(RELAY_PIN, HIGH);
        digitalWrite(LED_BUILTIN, HIGH);
        if (digitalRead(RELAY_PIN) == HIGH) {
          Serial.println("Relay is OFF (HIGH) [from !ubidots.connected()");
        }
        //end of valve OFF


        // Blink the LED
        digitalWrite(LED_BUILTIN, HIGH);
        delay(500);
        digitalWrite(LED_BUILTIN, LOW);
        delay(500);
        //end of blink


        ubidots.reconnect();
        ubidots.subscribeLastValue("esp32", "blue-light");
    }

    if (abs((int)(millis() - timer)) > PUBLISH_FREQUENCY)
    {
        timer = millis();
    }

    // flow sensor code
    currentMillis2 = millis();
    if (currentMillis2 - previousMillis2 > interval2)
    {

        pulse1Sec = pulseCount;
        pulseCount = 0;

        // Because this loop may not complete in exactly 1 second intervals we calculate
        // the number of milliseconds that have passed since the last execution and use
        // that to scale the output. We also apply the calibrationFactor to scale the output
        // based on the number of pulses per second per units of measure (litres/minute in
        // this case) coming from the sensor.
        flowRate = ((1000.0 / (millis() - previousMillis2)) * pulse1Sec) / calibrationFactor;
        previousMillis2 = millis();

        // Divide the flow rate in litres/minute by 60 to determine how many litres have
        // passed through the sensor in this 1 second interval, then multiply by 1000 to
        // convert to millilitres.
        flowMilliLitres = (flowRate / 60) * 1000;
        flowLitres = (flowRate / 60);

        // Add the millilitres passed in this second to the cumulative total
        totalMilliLitres += flowMilliLitres;
        totalLitres += flowLitres;

        // Print the flow rate for this second in litres / minute
        // Serial.print("Flow rate: ");
        // Serial.print(float(flowRate)); // Print the integer part of the variable
        // Serial.print("L/min");
        // Serial.print("\t"); // Print tab space

        // Print the cumulative total of litres flowed since starting
        // Serial.print("Output Liquid Quantity: ");
        // Serial.print(totalMilliLitres);
        // Serial.print("mL / ");
        // Serial.print(totalLitres);
        // Serial.println("L");
    }

    int newFlowRate = float(flowRate);
    ThingSpeak.setField(1, global_ubi);
    ThingSpeak.setField(2, totalLitres);
    ThingSpeak.setField(3, newFlowRate);
    ThingSpeak.writeFields(CHANNEL_ID, CHANNEL_API_KEY);

    ubidots.loop();
}
