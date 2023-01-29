/*
FetchCommandFromTalkBack

Description: Checks a TalkBack queue every 60 seconds and set the state of the build in LED according
             to the latest command fetched. Turn the LED on and off by using the commands TURN_ON and TURN_OFF.
             The TalkBack documentation can be found at https://www.mathworks.com/help/thingspeak/talkback-app.html.
             
Hardware: ESP32-based boards

Notes:
- Requires installation of EPS32 core.
- Select the target hardware from the Tools > Board menu

Copyright 2018, The MathWorks, Inc.
*/

#include <WiFi.h>

char ssid[] = "nava hotspot";   // Your network SSID (name) 
char pass[] = "12345678";   // Your network password

WiFiClient  client;

unsigned long myTalkBackID = 47509;
const char * myTalkBackKey = "F54INUNM1M3QNYPI";


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);  // Set up LED
  Serial.begin(115200);          // Initialize serial
  WiFi.mode(WIFI_STA);
}

void loop() {

  // Connect or reconnect to Wi-Fi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(String(ssid));
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }

  // Create the TalkBack URI
  String tbURI = String("/talkbacks/") + String(myTalkBackID) + String("/commands/execute");
  
  // Create the message body for the POST out of the values
  String postMessage =  String("api_key=") + String(myTalkBackKey);                      
                       
   // Make a string for any commands that might be in the queue
  String newCommand = String();

  // Make the POST to ThingSpeak
  int x = httpPOST(tbURI, postMessage, newCommand);
  client.stop();
  
  // Check the result
  if(x == 200){
    Serial.println("checking queue..."); 
    // check for a command returned from TalkBack
    if(newCommand.length() != 0){

      Serial.print("  Latest command from queue: ");
      Serial.println(newCommand);
      
      if(newCommand == "hello"){
        digitalWrite(LED_BUILTIN, HIGH);  
      }

      if(newCommand == "bye"){
        digitalWrite(LED_BUILTIN, LOW);
      }
    }
    else{
      Serial.println("  Nothing new.");  
    }
    
  }
  else{
    Serial.println("Problem checking queue. HTTP error code " + String(x));
  }

  
  delay(10000); // Wait 10 seconds to check queue again
}

// General function to POST to ThingSpeak
int httpPOST(String uri, String postMessage, String &response){

  bool connectSuccess = false;
  connectSuccess = client.connect("api.thingspeak.com",80);

  if(!connectSuccess){
      return -301;   
  }
  
  postMessage += "&headers=false";
  
  String Headers =  String("POST ") + uri + String(" HTTP/1.1\r\n") +
                    String("Host: api.thingspeak.com\r\n") +
                    String("Content-Type: application/x-www-form-urlencoded\r\n") +
                    String("Connection: close\r\n") +
                    String("Content-Length: ") + String(postMessage.length()) +
                    String("\r\n\r\n");

  client.print(Headers);
  client.print(postMessage);

  long startWaitForResponseAt = millis();
  while(client.available() == 0 && millis() - startWaitForResponseAt < 5000){
      delay(100);
  }

  if(client.available() == 0){       
    return -304; // Didn't get server response in time
  }

  if(!client.find(const_cast<char *>("HTTP/1.1"))){
      return -303; // Couldn't parse response (didn't find HTTP/1.1)
  }
  
  int status = client.parseInt();
  if(status != 200){
    return status;
  }

  if(!client.find(const_cast<char *>("\n\r\n"))){
    return -303;
  }

  String tempString = String(client.readString());
  response = tempString;
  
  return status;
}