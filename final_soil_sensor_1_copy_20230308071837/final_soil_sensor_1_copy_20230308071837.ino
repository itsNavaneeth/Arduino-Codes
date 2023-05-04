#include "WiFi.h"
#include "ThingSpeak.h"

#define CHANNEL_ID 2028980
#define CHANNEL_API_KEY "13QMZOZW5LLP1PS7"

WiFiClient client;

#define WIFI_NETWORK "BMS-IoT"      // Enter your WiFi network name
#define WIFI_PASSWORD "IoT_Project1" // Enter your WiFi password
#define WIFI_TIMEOUT_MS 20000

void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info){
  // Serial.println("Connected to AP successfully!");

  // ubidots.subscribeLastValue("esp32", "home-valve");
  
}

void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info){
  // Serial.println("WiFi connected");
  // Serial.println("IP address: ");
  // Serial.println(WiFi.localIP());

  // ubidots.subscribeLastValue("esp32", "home-valve");

}

void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info){

  // Blink the LED
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  //end of blink


  // Serial.println("Disconnected from WiFi access point");

  // Serial.print("WiFi lost connection. Reason: ");
  // Serial.println(info.wifi_sta_disconnected.reason);
  // Serial.println("Trying to Reconnect");
  WiFi.begin(ssid, password);

  Serial.println("Before 5000 delay [inside WiFiStationDisconnected]");
  delay(5000);
  Serial.println("After 5000 delay [inside WiFiStationDisconnected]");

  ESP.restart();
}

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

    WiFi.onEvent(WiFiStationConnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
    WiFi.onEvent(WiFiGotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
    WiFi.onEvent(WiFiStationDisconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

    ThingSpeak.begin(client);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, 1);
}

void loop()
{
    int soil_moisture_1 = analogRead(35);

    ThingSpeak.setField(1, soil_moisture_1);
    ThingSpeak.writeFields(CHANNEL_ID, CHANNEL_API_KEY);

    delay(15000);
}
