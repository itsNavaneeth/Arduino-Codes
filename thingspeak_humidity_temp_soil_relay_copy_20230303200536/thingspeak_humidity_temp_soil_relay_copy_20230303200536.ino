#include "WiFi.h"
#include "ThingSpeak.h"
// #include <DHT.h>
#include <dht.h>
#define CHANNEL_ID 1958878
#define CHANNEL_API_KEY "KABEELQGDX1QB5WH"

WiFiClient client;

// DHT my_sensor(32, DHT11);
// float temperature, humidity;
dht DHT;

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
    //take action
  }else{
    Serial.print("Connected!");
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println(WiFi.localIP());
  }

}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  connectToWifi();
  ThingSpeak.begin(client);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 0);
}

void loop() {

  // int humidity = analogRead(34);
  // int temperature = analogRead(35);

  // int soil_moisture = analogRead(32);
  // Serial.print("Moisture % is: ");
  // Serial.println(soil_moisture);

  // humidity = my_sensor.readHumidity();
  // temperature = my_sensor.readTemperature();
  // Serial.print("Temperature: ");
  // Serial.print(temperature);
  // Serial.print(" C / Humidity:  ");
  // Serial.print(humidity);
  // Serial.println(" %");

  
  // ThingSpeak.setField(1, humidity);
  // ThingSpeak.setField(2, temperature);
  // ThingSpeak.setField(3, soil_moisture);
  // ThingSpeak.writeFields(CHANNEL_ID, CHANNEL_API_KEY);

  delay(3000);

}
