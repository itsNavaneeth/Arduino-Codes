#include "WiFi.h"
#include "ThingSpeak.h"
#include "WiFiManager.h"

#define CHANNEL_ID 1977132
#define CHANNEL_API_KEY "VUD24X26T3WBYF04"

WiFiClient client;

int field4 = 0;


#define WIFI_NETWORK "nava hotspot"
#define WIFI_PASSWORD "12345678"
#define WIFI_TIMEOUT_MS 20000

void connectToWifi(){
  // WiFiManager wifiManager;
  // wifiManager.autoConnect("Nava ESP32");
  // Serial.println("Connected to Nava ESP32");


  Serial.print("Connecting to Wifi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_NETWORK, WIFI_PASSWORD);

  unsigned long startAttemptTime = millis();

  while(WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < WIFI_TIMEOUT_MS){
    Serial.print(".");
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
  }

  if(WiFi.status()!= WL_CONNECTED){
    Serial.println("Failed!");
    //take action
  }else{
    Serial.print("Connected!");
    Serial.println(WiFi.localIP());
  }

}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  connectToWifi();
  ThingSpeak.begin(client);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 1);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, LOW);
  int field1 = random(20,70);
  int field2 = random(20,70);
  int field3 = random(20,70);
  field4 = field4 + random(10, 20);

  ThingSpeak.setField(1, field1);
  ThingSpeak.setField(2, field2);
  ThingSpeak.setField(3, field3);
  ThingSpeak.setField(4, field4);
  ThingSpeak.setField(5, WiFi.RSSI());
  ThingSpeak.writeFields(CHANNEL_ID, CHANNEL_API_KEY);

  delay(15000);

}
