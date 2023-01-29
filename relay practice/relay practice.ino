#include <Arduino.h>

#define RELAY_PIN 26

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(RELAY_PIN, HIGH);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("high");
  delay(1000);
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("low");
  delay(1000);
}
