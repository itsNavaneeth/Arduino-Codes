#include <dht.h>
dht DHT;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(500);
  Serial.println("DHT11 Humidity and Temperature Sensor\n\n");
  delay(1000);

}

void loop() {
  // put your main code here, to run repeatedly:
  int dht_pin = analogRead(32);
  DHT.read11(dht_pin);
  Serial.print(DHT.humidity);
  delay(2000);
}
