int pin = 4;
int oldMil;
int newMil;
int rrpm = 0;
int s2 = 0;
int adc = 0;
float tegangan = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(pin, INPUT);

}

void loop() {
  rpm();
  delay(100);
}

void rpm() {
  int s = 0;
  int x = digitalRead(pin);
//  Serial.print("Initial Pin State = ");
//  Serial.println(x);
  if (x == 0) {
    s2 = 1;                           // Sensor Ready
  }
  else {
    rrpm = 0;
      adc=analogRead(A0);
      tegangan=(0.0567*adc)+0.5913;
      String dataout = "id_gokart=1&voltage="+String(tegangan)+"&rpm="+String(rrpm)+"&submit=enter";
      Serial.println(dataout);
      delay(1000);
//    Serial.print("RPM = ");
//    Serial.println(rpm);
  }

  while (s2 != 0 && s < 2) {
    x = digitalRead(pin);
    //    Serial.print("Semaphore = ");
    //    Serial.println(s);
    //    Serial.print("Pin State = ");
    //    Serial.println(x);
    if (x == 1) {
      if (s == 0) {
        oldMil = millis();
//        Serial.print("Old Millis = ");
//        Serial.println(oldMil);
      }
      s = 1;           // s = 1
      //      Serial.print("Semaphore = ");
      //      Serial.println(s);
      //      Serial.print("Pin State = ");
      //      Serial.println(x);
    }
    if (x == 0 && s == 1) {
      newMil = millis();
      s = 2;                    // s = 2
      s2 = 0;
//      Serial.print("Semaphore = ");
//      Serial.println(s);
//      Serial.print("Pin State = ");
//      Serial.println(x);
//      Serial.print("New Millis = ");
//      Serial.println(newMil);
      int timeMil = newMil - oldMil;  // time 1 rotasi
      int rrpm = 60000 / (timeMil*8);
      adc=analogRead(A0);
      tegangan=(0.0567*adc)+0.5913;
      String dataout = "id_gokart=1&voltage="+String(tegangan)+"&rpm="+String(rrpm)+"&submit=enter";
      Serial.println(dataout);
//      Serial.print("Time Millis = ");
//      Serial.println(timeMil);
//      Serial.print("RPM = ");
//      Serial.println(rpm);
    }
  }
}
