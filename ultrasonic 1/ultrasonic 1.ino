#include <NewPing.h>

NewPing sonar(10,11,20);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(50);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("The distance is: ");
  Serial.print(sonar.ping_cm());
  Serial.println(" cm");
  delay(1000);

}
