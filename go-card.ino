#include <WiFi.h>
#include <HTTPClient.h>
  
const char* ssid = "Thonilux";
const char* password =  "Internet";
int i=1;

void setup() {
  Serial.begin(115200);
  delay(1000);
  WiFi.begin(ssid, password); 
  
  while (WiFi.status() != WL_CONNECTED) { //Check for the connection
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
}
void loop() {
  
 if(WiFi.status()== WL_CONNECTED){
   checkpoint:
   HTTPClient http;
   int id = 1;
   double Lat = random(0,10);
   double Long = random(0,10);
   int Sat = random(0,9);
   int Speed = random(0,100);
   int Dir = random(0,3);
   float pwr = random (0,4);

   if(i==1){
   http.begin("http://enerma.id/gokart/api/GpsData");
   http.addHeader("Content-Type", "application/x-www-form-urlencoded");
   String dataout = "id_gokart="+String(id)+"&latitude=" + String(Lat) + "&longitude=" + String(Long) + "&satelite="+ String(Sat)+ "&speed="+ String(Speed)+ "&direction="+ String(Dir) +"&submit=enter";
   int httpResponseCode = http.POST(dataout);  
   Serial.println(httpResponseCode);
   http.end();
   i++;
   }

   else if(i==2){
   http.begin("http://enerma.id/gokart/api/BatData");
   http.addHeader("Content-Type", "application/x-www-form-urlencoded");
   String dataout = "id_gokart="+String(id)+"&power=" + String(pwr) +"&submit=enter";
   int httpResponseCode = http.POST(dataout);  
   Serial.println(httpResponseCode);
   http.end();
   i++;
   }
   else if(i==3){
   http.begin("http://enerma.id/gokart/api/Status"); //Specify the URL
   int httpCode=http.GET();
   String response = http.getString();
   http.end();
   Serial.println(response);
   i++;
   }

   
   if(i>3){
    i=1;
    Serial.println("=================================================");
    delay(3000);
   }
 }
 
  else{ 
    Serial.println("Error in WiFi connection");   
 }
}
