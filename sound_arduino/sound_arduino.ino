// const int ledpin=13; // ledpin and soundpin are not changed throughout the process
const int soundpin=A0;
const int threshold=2000; // sets threshold value for sound sensor

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(soundpin,INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  int soundsens=analogRead(soundpin); // reads analog data from sound sensor
  Serial.println(soundsens);
  delay(1000);
  
  if (soundsens>=threshold) {
  digitalWrite(LED_BUILTIN, HIGH); //turns led on
  Serial.println(soundsens);
  delay(1000);
  }
  else{
  digitalWrite(LED_BUILTIN, LOW);
  }

}
