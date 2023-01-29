#include <String.h>
#include <Arduino.h>
#include "UbidotsEsp32Mqtt.h"
#include "WiFi.h"
#include "ThingSpeak.h"

#define RELAY_PIN 26
#define CHANNEL_ID 2019443
#define CHANNEL_API_KEY "Q2NKY5A7HHLPV3DL"

// for flow sensor
#define SENSOR 35

long currentMillis = 0;
long previousMillis = 0;
int interval = 1000;
boolean ledState = LOW;
float calibrationFactor = 4.5;
volatile byte pulseCount;
byte pulse1Sec = 0;
float flowRate;
unsigned long flowMilliLitres;
unsigned int totalMilliLitres;
float flowLitres;
float totalLitres;

int count;

/****************************************
 * Define UBIDOTS Constants
 ****************************************/
const char *UBIDOTS_TOKEN = "BBFF-LtAIEbHEpPlRavdXFOC9Nu8SRnTN9y";
const char *WIFI_SSID = "WIFI_NAME";     // Put here your Wi-Fi SSID
const char *WIFI_PASS = "WIFI_PASSWORD"; // Put here your Wi-Fi password
const char *DEVICE_LABEL = "esp32";
const char *VARIABLE_LABEL = "soil-moisture";
const char *VARIABLE_LABEL_SUBSCRIBE = "blue-light";
const int PUBLISH_FREQUENCY = 2000;
unsigned long timer;

// api command
String global_ubi = "a";

WiFiClient client;
Ubidots ubidots(UBIDOTS_TOKEN);

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

    // flow sensor
    pulseCount = 0;
    flowRate = 0.0;
    flowMilliLitres = 0;
    totalMilliLitres = 0;
    previousMillis = 0;
    attachInterrupt(digitalPinToInterrupt(SENSOR), pulseCounter, FALLING);
    // end of flow sensor

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
    if (global_ubi == "1")
    {
        // Turn the relay ON
        // Serial.println(global_ubi);
        digitalWrite(RELAY_PIN, HIGH);
        digitalWrite(LED_BUILTIN, HIGH);
    }
    else if (global_ubi == "0")
    {
        // Turn the relay OFF
        digitalWrite(RELAY_PIN, LOW);
        digitalWrite(LED_BUILTIN, LOW);
    }

    else if (global_ubi == "3")
    {
        digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
        delay(10000);                    // wait for 10 second
        digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
        delay(10000);
    }

    if (!ubidots.connected())
    {
        ubidots.reconnect();
        ubidots.subscribeLastValue("esp32", "blue-light");
    }
    if (abs((int)(millis() - timer)) > PUBLISH_FREQUENCY)
    {
        timer = millis();
    }

    // flow sensor code
    currentMillis = millis();
    if (currentMillis - previousMillis > interval)
    {

        pulse1Sec = pulseCount;
        pulseCount = 0;

        // Because this loop may not complete in exactly 1 second intervals we calculate
        // the number of milliseconds that have passed since the last execution and use
        // that to scale the output. We also apply the calibrationFactor to scale the output
        // based on the number of pulses per second per units of measure (litres/minute in
        // this case) coming from the sensor.
        flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;
        previousMillis = millis();

        // Divide the flow rate in litres/minute by 60 to determine how many litres have
        // passed through the sensor in this 1 second interval, then multiply by 1000 to
        // convert to millilitres.
        flowMilliLitres = (flowRate / 60) * 1000;
        flowLitres = (flowRate / 60);

        // Add the millilitres passed in this second to the cumulative total
        totalMilliLitres += flowMilliLitres;
        totalLitres += flowLitres;

        // Print the flow rate for this second in litres / minute
        Serial.print("Flow rate: ");
        Serial.print(float(flowRate)); // Print the integer part of the variable
        Serial.print("L/min");
        Serial.print("\t"); // Print tab space

        // Print the cumulative total of litres flowed since starting
        Serial.print("Output Liquid Quantity: ");
        Serial.print(totalMilliLitres);
        Serial.print("mL / ");
        Serial.print(totalLitres);
        Serial.println("L");
    }

    newFlowRate = float(flowRate);
    ThingSpeak.setField(4, totalLitres);
    ThingSpeak.setField(5, newFlowRate);
    ThingSpeak.setField(6, global_ubi);
    ThingSpeak.writeFields(CHANNEL_ID, CHANNEL_API_KEY);

    ubidots.loop();
}
