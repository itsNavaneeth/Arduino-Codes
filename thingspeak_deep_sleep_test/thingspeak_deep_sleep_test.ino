#include "WiFi.h"
#include "ThingSpeak.h"

#define CHANNEL_ID 2028981
#define CHANNEL_API_KEY "IKU02NTFSIL2ADDQ"

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  300        /* Time ESP32 will go to sleep (in seconds) */

WiFiClient client;

int counter = 0;

#define WIFI_NETWORK "Marvel Studios"
#define WIFI_PASSWORD "Avengers4"
#define WIFI_TIMEOUT_MS 20000

void connectToWifi(){
  Serial.print("Connecting to Wifi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_NETWORK, WIFI_PASSWORD);

  unsigned long startAttemptTime = millis();

  while(WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < WIFI_TIMEOUT_MS){
    Serial.print(".");
    delay(100);
  }

  if(WiFi.status()!= WL_CONNECTED){
    Serial.println("Failed!");
    digitalWrite(LED_BUILTIN, 1);
    delay(500);
    digitalWrite(LED_BUILTIN, 0);
    //take action
  }else{
    Serial.print("Connected!");
    digitalWrite(LED_BUILTIN, 1);
    Serial.println(WiFi.localIP());
    digitalWrite(LED_BUILTIN, 1);
  }

}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115000);
  connectToWifi();
  ThingSpeak.begin(client);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 1);

  ThingSpeak.setField(2, WiFi.RSSI());
  ThingSpeak.writeFields(CHANNEL_ID, CHANNEL_API_KEY);


  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) +
  " Seconds");

  Serial.println("Going to sleep now");
  // delay(1000);
  // Serial.flush(); 
  esp_deep_sleep_start();


}

void loop() {

}
