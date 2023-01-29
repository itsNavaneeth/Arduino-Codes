#include "WiFi.h"
#include "ThingSpeak.h"

#define CHANNEL_ID 1958878
#define CHANNEL_API_KEY "KABEELQGDX1QB5WH"

WiFiClient client;

int counter = 0;

#define WIFI_NETWORK "Airtel-Hotspot-603D"
#define WIFI_PASSWORD "3iyn4jt2"
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
    //take action
  }else{
    Serial.print("Connected!");
    Serial.println(WiFi.localIP());
  }

}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  connectToWifi();
  ThingSpeak.begin(client);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 1);
}

void loop() {
  // put your main code here, to run repeatedly:
  counter++;

  int led = ThingSpeak.readIntField(CHANNEL_ID, 4);
  if(led == 1){
    digitalWrite(LED_BUILTIN, 1);
    Serial.println("LED is ON");
  }
  else if(led == 0){
    digitalWrite(LED_BUILTIN, 0);
    Serial.println("LED is OFF");
  }


  int x = analogRead(35);
  // Serial.print("Moisture % is: ");
  // Serial.println(x);
  Serial.println(ThingSpeak.readIntField(CHANNEL_ID, 4));
  
  
  ThingSpeak.setField(1, counter);
  ThingSpeak.setField(2, WiFi.RSSI());
  ThingSpeak.setField(3, x);
  ThingSpeak.setField(4, led);
  ThingSpeak.writeFields(CHANNEL_ID, CHANNEL_API_KEY);

  delay(15000);

}
