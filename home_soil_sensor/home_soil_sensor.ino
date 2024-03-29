#include "WiFi.h"
#include "ThingSpeak.h"

#define CHANNEL_ID 2028981
#define CHANNEL_API_KEY "IKU02NTFSIL2ADDQ"

WiFiClient client;

#define WIFI_NETWORK "Marvel Studios"      // Enter your WiFi network name
#define WIFI_PASSWORD "Avengers4" // Enter your WiFi password
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
    Serial.println(soil_moisture_1);

    // delay(1000);

    ThingSpeak.setField(1, soil_moisture_1);
    ThingSpeak.writeFields(CHANNEL_ID, CHANNEL_API_KEY);

    delay(15000);
}
