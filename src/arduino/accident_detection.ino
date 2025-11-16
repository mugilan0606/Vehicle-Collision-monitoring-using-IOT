#include <TinyGPSPlus.h>
#include <TinyGPS.h>
#include <SoftwareSerial.h>

TinyGPSPlus gps;
SoftwareSerial ss(9, 10); 
SoftwareSerial mySerial(7, 8);
SoftwareSerial mySerial1(4,5);

int x, y, z, bstate2;
int buzzer = 11;
int sw2 = 3;

String value, url1;
double latitude, longitude;
String condition;
int acc = 0, i = 0, j = 1;

void setup() {
  Serial.begin(115200);
  mySerial.begin(115200);
  mySerial1.begin(115200);
  ss.begin(9600);
  pinMode(sw2, INPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, HIGH);
}

void loop() {
  x = analogRead(A0);
  y = analogRead(A1);
  z = analogRead(A2);

  Serial.println("x: " + String(x));
  Serial.println("y: " + String(y));
  Serial.println("z: " + String(z));

  bstate2 = digitalRead(sw2);
  Serial.println("BUTTON STATE " + String(bstate2));

  latitude = gps.location.lat();
  longitude = gps.location.lng();

  delay(1000);
  smartDelay(1000);

  String url1 = "$";
  url1 += "&field3=";
  url1 += condition;
  url1 += "&field1=";
  url1 += String(latitude, 6);
  url1 += "&field2=";
  url1 += String(longitude, 6);
  url1 += "*";

  if (millis() > 5000 && gps.charsProcessed() < 10)
     Serial.println(F("No GPS data received: check wiring"));

  if (x >= 700 && y > 700 && z > 700) {
     Serial.println("accident is detected");
     acc = 1;
  } else {
     Serial.println("no accident is detected");
     acc = 0;
  }

  if (acc == 1) {
     Serial.println("accident happened");
     digitalWrite(buzzer, HIGH);
     delay(3000);
     digitalWrite(buzzer, LOW);

     while (j == 1) {
        bstate2 = digitalRead(sw2);
        Serial.println("BUTTON STATE " + String(bstate2));
        if (bstate2 == 0) j = 0;
        else if (i == 5) j = 0;
        i++;
        delay(500);
     }

     if (bstate2 == 0) {
        Serial.println("vehicle is safe");
        condition = "0";
        mySerial.println(url1);
        Serial.println(url1);
     }
     else if (bstate2 == 1) {
        Serial.println("vehicle is not safe");
        condition = "1";
        value = "#accident occurred at lat=" + String(latitude,6) + " lon=" + String(longitude,6);
        value += "*";
        Serial.println(value);
        mySerial1.println(value);
        delay(1000);
        mySerial.println(url1);
        Serial.println(url1);
     }
  }

  j = 1;
  i = 0;
  delay(1000);
}

static void smartDelay(unsigned long ms) {
  unsigned long start = millis();
  do {
     while (ss.available())
        gps.encode(ss.read());
  } while (millis() - start < ms);
}
