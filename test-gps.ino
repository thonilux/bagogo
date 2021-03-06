#include <WiFi.h>
#include <HTTPClient.h>
#include <TinyGPS++.h> //Libary TinyGPS
#include <Wire.h>

float Lat = 0;
float Long = 0;
int Sat = 0;
int Speed = 0;
int Dir = 0;


const char* ssid = "Thonilux";
const char* password =  "Internet";
// Choose two Arduino pins to use for software serial
#define RXD2 16
#define TXD2 17
HardwareSerial neogps(1);


// Membuat objek TinyGPS++
TinyGPSPlus gps;

void setup()
{
  //Memulai koneksi serial pada baudrate 9600
  Serial.begin(9600);

  //Memulai koneksi serial dengan sensor
  neogps.begin(9600, SERIAL_8N1, RXD2, TXD2);

  delay(1000);
  WiFi.begin(ssid, password); 
  
  while (WiFi.status() != WL_CONNECTED) { //Check for the connection
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
}

void loop()
{
if(WiFi.status()== WL_CONNECTED){
  //Membuat tampilan data ketika terdapat koneksi
  while (neogps.available() > 0)
    if (gps.encode(neogps.read()))
      displayInfo();

  // Jika dalam 5 detik tidak ada koneksi, maka akan muncul error "No GPS detected"
  // Periksa sambungan dan reset arduino
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println("No GPS detected");
    while(true);
  }
}
}

void displayInfo()
{
  if (gps.location.isValid()){
    float Lat = gps.location.lat();
    float Long = gps.location.lng();
    Serial.println(gps.location.lat(),6);
    Serial.println(gps.location.lng(),6);
    int Sat = gps.satellites.value();
    int Speed = gps.speed.kmph();
    int Dir = 1;
  HTTPClient http;
   int id = 1;
  http.begin("http://enerma.id/gokart/api/GpsData");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  String dataout = "id_gokart="+String(id)+"&latitude=" + String(gps.location.lat(),6) + "&longitude=" + String(gps.location.lng(),6) + "&satelite="+ String(gps.satellites.value(),6)+ "&speed="+ "0"+ "&direction="+ "0" +"&submit=enter";
  int httpResponseCode = http.POST(dataout);  
  Serial.println(dataout);
  Serial.println(httpResponseCode);
  http.end();
  delay(1000);
  }

  
  Serial.print("Date: ");
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print("/");
    Serial.print(gps.date.day());
    Serial.print("/");
    Serial.println(gps.date.year());
  }
  else
  {
    Serial.println("Not Available");
  }

  Serial.print("Time: ");
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour()+7);
    Serial.print(":");
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(":");
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(".");
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.println(gps.time.centisecond());
  }
  else
  {
    Serial.println("Not Available");
  }

  Serial.println();
  Serial.println();
  delay(1000);
}
