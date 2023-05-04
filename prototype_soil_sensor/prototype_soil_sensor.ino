#include "WiFi.h"
#include "ThingSpeak.h"

#define CHANNEL_ID 1958878
#define CHANNEL_API_KEY "KABEELQGDX1QB5WH"

WiFiClient client;

#define WIFI_NETWORK "BMS-IoT"      // Enter your WiFi network name
#define WIFI_PASSWORD "IoT_Project1" // Enter your WiFi password
#define WIFI_TIMEOUT_MS 20000

void connectToWifi()
{
    Serial.print("Connecting to Wifi");
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_NETWORK, WIFI_PASSWORD);

    unsigned long startAttemptTime = millis();

    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < WIFI_TIMEOUT_MS)
    {
        Serial.print(".");
        // digitalWrite(LED_BUILTIN, 1);
        delay(100);
        // digitalWrite(LED_BUILTIN, 0);
    }

    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Failed!");
        digitalWrite(LED_BUILTIN, 1);
        // take action
    }
    else
    {
        Serial.print("Connected!");
        Serial.println(WiFi.localIP());
        digitalWrite(LED_BUILTIN, 0);
    }
}

void setup()
{
    Serial.begin(9600); // cross check baud rate
    connectToWifi();
    ThingSpeak.begin(client);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, 1);
}

void loop()
{
    int soil_moisture_1 = analogRead(35);
    int soil_moisture_2 = analogRead(34);

    /*
    Soil Moisture Reference
    Air = 0%
    Really dry soil = 10%
    Probably as low as you'd want = 20%
    Well watered = 50%
    Cup of water = 100%
    */

    Serial.println(soil_moisture_1);
    Serial.println(soil_moisture_2);

    ThingSpeak.setField(1, soil_moisture_1);
    ThingSpeak.setField(2, soil_moisture_2);
    // ThingSpeak.setField(2, soil_moisture_1);
    ThingSpeak.writeFields(CHANNEL_ID, CHANNEL_API_KEY);

    delay(15000);
}
