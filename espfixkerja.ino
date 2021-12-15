#include <WiFi.h>

#include <HTTPClient.h>
#include <TinyGPS++.h> //Libary TinyGPS
#include <Wire.h>

float Lat = 0;
float Latt = 0;
float Long = 0;
float Longg = 0;
int voltage = 42;
int Speed = 0;
int Dir = 0;
int i =1;
const int relay = 26;
const int pin = 4;

int rpmPin = 13; 
volatile byte rpmcount;
unsigned int rpm;
unsigned long timeold;
unsigned long timeMil;
unsigned long oldMil;
unsigned long newMil;
String dataout;
String seri;

const char* ssid = "GoKart";
const char* password =  "GoKart123";
// Choose two Arduino pins to use for software serial
#define RXD2 16
#define TXD2 17
HardwareSerial neogps(1);


// Membuat objek TinyGPS++
TinyGPSPlus gps;

void setup()
{
  //Memulai koneksi serial pada baudrate 9600
  Serial.begin(115200);

  //Memulai koneksi serial dengan sensor
  neogps.begin(9600, SERIAL_8N1, RXD2, TXD2);

  delay(1000);
  WiFi.begin(ssid, password); 
  
  while (WiFi.status() != WL_CONNECTED) { //Check for the connection
    delay(1000);
//    Serial.println("Connecting to WiFi..");
  }
//  Serial.println("Connected to the WiFi network");

//  pinMode(ledPin, OUTPUT);
//  digitalWrite(ledPin, HIGH);
  pinMode(relay, OUTPUT);
  rpmcount = 0;
  rpm = 0;
  timeold = 0;
  digitalWrite(relay, LOW);
}

void loop()
{
if(WiFi.status()== WL_CONNECTED){
  //Membuat tampilan data ketika terdapat koneksi
  while (neogps.available() > 0)
    if (gps.encode(neogps.read()))
     if(i==1){
      kontrol();
      
      gpsdata();
      i++;
     }
     else if(i==2){
      kontrol();
      gokart();
      i=1;
     }
  // Jika dalam 5 detik tidak ada koneksi, maka akan muncul error "No GPS detected"
  // Periksa sambungan dan reset arduino
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println("No GPS detected");
    while(true);
  }
}
}

void gpsdata()
{
    Serial.println(gps.location.lat(),6);
    Serial.println(gps.location.lng(),6);
    if (gps.location.isValid()){
      Latt = (gps.location.lat(),6);
      Longg = (gps.location.lng(),6);
      HTTPClient http;
      http.begin("https://enerma.id/gokart/api/GpsData");
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      String dataout = "id_gokart=1&latitude=" + String(gps.location.lat(),6)+"&longitude=" + String(gps.location.lng(),6)+"&submit=enter";
      int httpResponseCode = http.POST(dataout);  
//      Serial.println(dataout);
//      Serial.println(httpResponseCode);
      http.end();
    }
    delay(5000);
  
}
void gokart(){
     while(Serial.available()>0){
      char c=Serial.read();
       seri += c;
      }
      dataout=String(seri);
      HTTPClient http;
      http.begin("https://enerma.id/gokart/api/BatData");
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      int httpResponseCode = http.POST(dataout);  
//      Serial.println(dataout);
      Serial.println(httpResponseCode);
      http.end();
     
      delay(5000);
}
void kontrol(){
      HTTPClient http;
      http.begin("https://enerma.id/gokart/api/getStatus");
      int httpCode=http.GET();
      String response = http.getString();
      http.end();
      int stat = response[10];
      if(stat==49){
        digitalWrite(relay, LOW);
//        Serial.println("gokart on");
      }
      else if(stat==48){
        digitalWrite(relay, HIGH);
//        Serial.println("gokart off");
      }
}
