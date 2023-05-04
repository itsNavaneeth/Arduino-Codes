/*
  AnalogReadSerial

  Reads an analog input on pin 0, prints the result to the Serial Monitor.
  Graphical representation is available using Serial Plotter (Tools > Serial Plotter menu).
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/AnalogReadSerial
*/

// the setup routine runs once when you press reset:

#include "WiFi.h"
#include "ThingSpeak.h"

#define CHANNEL_ID 1958878
#define CHANNEL_API_KEY "KABEELQGDX1QB5WH"

WiFiClient client;

int counter = 0;

#define WIFI_NETWORK "Marvel Studios"
#define WIFI_PASSWORD "Avengers4"
#define WIFI_TIMEOUT_MS 20000






float x_new = 0.0;
int a = 0;
int b = 100;

int min_x = 900;
int max_x = 2700;
int x = 12;






void connectToWifi(){
  x = analogRead(35);
  Serial.println(x);

  Serial.print("Connecting to Wifi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_NETWORK, WIFI_PASSWORD);

  unsigned long startAttemptTime = millis();

  while(WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < WIFI_TIMEOUT_MS){
    Serial.print(x);
    delay(100);
  }

  if(WiFi.status()!= WL_CONNECTED){
    Serial.println("Failed!");
    //take action
  }else{
    x = analogRead(35);
    Serial.print("Connected! ");
    Serial.print(x);
    Serial.println(WiFi.localIP());
  }

}





void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  connectToWifi();
  // ThingSpeak.begin(client);


}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int x = analogRead(35);

  x_new = b - ((b-a)*(x-min_x))/(max_x-min_x) + a;
  // print out the value you read:
  Serial.print("Moisture % is: ");
  Serial.println(x);
  delay(200);        // delay in between reads for stability
}