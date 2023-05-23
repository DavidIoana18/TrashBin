#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "o5LLx256AA-Dg4Ikg6UPJx8DhWeOwtpI";
char ssid[] = "Ioana";
char pass[] = "password";

void setup() {
  Serial.begin(9600);
  delay(10);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }

  // Initialize Blynk
  Blynk.begin(auth, ssid, pass);

  Serial.println("Connected to Wi-Fi!");
}

void loop() {

  // Check if connected to Wi-Fi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Disconnected from Wi-Fi, reconnecting...");
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to Wi-Fi...");
    }
    Serial.println("Connected to Wi-Fi!");
  }
    
  if (Serial.available() >= 4) {
    String data1 = Serial.readStringUntil('\n');
    String data2 = Serial.readStringUntil('\n');
    String data3 = Serial.readStringUntil('\n');
    String data4 = Serial.readStringUntil('\n');
    
    Serial.println("Sending data to Blynk...");
    Blynk.virtualWrite(V0, data1.toInt());
    Blynk.virtualWrite(V1, data2.toInt());
    Blynk.virtualWrite(V2, data3.toInt());
    Blynk.virtualWrite(V3, data4.toInt());
  }

  Blynk.run();
  delay(1000);
}