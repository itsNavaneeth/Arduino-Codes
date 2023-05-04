void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int soilMoisture = analogRead(35);
  Serial.println(soilMoisture);
  delay(1000);
}