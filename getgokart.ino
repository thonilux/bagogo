#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
const char* ssid = "Thonilux";
const char* password =  "Internet";
int i;


void setup() {
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);
  pinMode(21, OUTPUT);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
 
}
 
void loop() {
 
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
  kontrol();
  }
  else{
    Serial.println("Network Error");
  }
  Serial.println("============================================");
}

void kontrol(){
    HTTPClient http;
    http.begin("https://enerma.id/gokart/api/getStatus"); //Specify the URL
    int httpCode=http.GET();
    String response = http.getString();
    http.end();
    int state = response[10];
    if(state==49){
      digitalWrite( 19, LOW);
      Serial.println("Gokart ON");
    }
    else if(state==48){
      digitalWrite( 19, HIGH);
      Serial.println("Gokart OFF");
    }
}
