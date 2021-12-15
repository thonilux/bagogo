// Deklarasi Library ESP32 yang digunakan
#include <WiFi.h>             // Library WiFi
#include <HTTPClient.h>       // Library komunikasi data melalui HTTP
#include <TinyGPS++.h>        // Library sensor TinyGPS
#include <Wire.h>             // Library koneksi

// Deklarasi Variabel
// variabel data GPS
float Lat = 0;
float Latt = 0;
float Long = 0;
float Longg = 0;
int voltage = 42;
int Speed = 0;
int Dir = 0;

// Deklarasi pin
int i = 1;
const int relay = 26;
const int pin = 4;

// variabel rpm
int rpmPin = 13;
volatile byte rpmcount;
unsigned int rpm;
unsigned long timeold;
unsigned long timeMil;
unsigned long oldMil;
unsigned long newMil;
String dataout;
String seri;

// Menentukan wifi yang digunakan
// Masukan ssid/nama wifi dan pasword
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
  }
  pinMode(relay, OUTPUT);
  rpmcount = 0;
  rpm = 0;
  timeold = 0;
  digitalWrite(relay, LOW);
}

void loop()
{
  if (WiFi.status() == WL_CONNECTED) {
    //Membuat tampilan data ketika terdapat koneksi
    while (neogps.available() > 0)
      if (gps.encode(neogps.read()))
        if (i == 1) {
          kontrol();

          gpsdata();
          i++;
        }
        else if (i == 2) {
          kontrol();
          gokart();
          i = 1;
        }
    // Jika dalam 5 detik tidak ada koneksi, maka akan muncul error "No GPS detected"
    // Periksa sambungan dan reset arduino
    if (millis() > 5000 && gps.charsProcessed() < 10)
    {
      Serial.println("No GPS detected");
      while (true);
    }
  }
}

void gpsdata() {
  // Validasi data lokasi dari sensor GPS
  if (gps.location.isValid()) {
    // Mengambil data longitude dan latitude dari lokasi sensor GPS
    Latt = (gps.location.lat(), 6);
    Longg = (gps.location.lng(), 6);
    // Mengirim data GPS ke database melalui API
    HTTPClient http;
    http.begin("https://enerma.id/gokart/api/GpsData");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    String dataout = "id_gokart=1&latitude=" + String(gps.location.lat(), 6) + "&longitude=" + String(gps.location.lng(), 6) + "&submit=enter";
    int httpResponseCode = http.POST(dataout);
    http.end();
    // Selesai mengirim data GPS
  }
  delay(5000);
}

void gokart() {
  // Membaca data rpm dan tegangan baterai gokart dari arduino UNO
  while (Serial.available() > 0) {
    char c = Serial.read();
    seri += c;
  }
  dataout = String(seri);
  // Mengirim data rpm dan tegangan baterai gokart
  HTTPClient http;
  http.begin("https://enerma.id/gokart/api/BatData");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpResponseCode = http.POST(dataout);
  Serial.println(httpResponseCode);
  http.end();
  // Selesai mengirim data

  delay(5000);
}
void kontrol() {
  // Mengambil data status gokart dari web
  HTTPClient http;
  http.begin("https://enerma.id/gokart/api/getStatus");
  int httpCode = http.GET();
  String response = http.getString();
  http.end();
  // Selesai mengambil data
  // Set relay ON/OFF berdasarkan data yang diambil sebelumnya
  int stat = response[10];
  if (stat == 49) {
    digitalWrite(relay, LOW);
 }
  else if (stat == 48) {
    digitalWrite(relay, HIGH);
 }
}
