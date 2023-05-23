
#include <NewPing.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include <math.h>

// WIFI RX & TX=============================================
SoftwareSerial espSerial(19, 18); // RX, TX


//Sonars====================================================

//sonar1  - prima trapa
const int trigPin1 = 4;
const int echoPin1 = 5;
int maxDistance1 = 400; // limita maxima de detectare pentru senzorul HC-SR04, in cm
NewPing sonar1(trigPin1, echoPin1, maxDistance1);


//sonar2 - a doua trapa
const int trigPin2 = 6;
const int echoPin2 = 7;
int maxDistance2 = 400; // limita maxima de detectare pentru senzorul HC-SR04, in cm
NewPing sonar2(trigPin2, echoPin2, maxDistance2);


//sonar3 - a doua trapa
const int trigPin3 = 8;
const int echoPin3 = 9;
int maxDistance3 = 400; // limita maxima de detectare pentru senzorul HC-SR04, in cm
NewPing sonar3(trigPin3, echoPin3, maxDistance3);


//ServoMotors===============================================

Servo servoMotor1;
Servo servoMotor2;


// Capacitive & Inductive sensors===========================
int inductiveSensor = 2;
int capacitiveSensor = 3;


void setup() {
  
  //Inductive & capacitive
  pinMode(inductiveSensor, INPUT);
  pinMode(capacitiveSensor, INPUT);
  
  Serial.begin(9600);
  
  //wifi
  espSerial.begin(9600);  // Set the baud rate of ESP8266
}



const int numSamples = 50;
int sensorValues[numSamples];
int state = LOW;

int filter(int sensorName){

  int value = digitalRead(sensorName);

  
  // muta fiecare valoare la stanga in vector, lasand primul element liber pentru a stoca valoarea curenta
  for (int i = 0; i < numSamples - 1; i++) { 
    sensorValues[i] = sensorValues[i+1];
  }
  // adauga valoarea curenta la ultimul element din vector
  sensorValues[numSamples-1] = value;

  // calculeaza media valorilor din vector
  int sum = 0;
  for (int i = 0; i < numSamples; i++) {
    sum += sensorValues[i];
  }
  int average = sum / numSamples;

  
  if (average > 0){
    state = HIGH;
  } else {
    state = LOW;
  }

  
  Serial.print("Sensor value: ");
  Serial.println(state);
 
  return state;
}



bool maxBin (int max){
  int maxMetal, maxPlastic = max + 1;  
   maxMetal = sonar2.ping_cm(); 
   maxPlastic = sonar3.ping_cm(); 

  bool fullBin = false;
  if (maxMetal < max || maxPlastic < max){
    fullBin = true;
  }
  return fullBin;
}


void motor1(Servo &servoName, int pinMotor, int angle, int thresholdDistance, int delayMotor) {

  int distance = sonar1.ping_cm();
  
  if (distance > 1 && distance < thresholdDistance){ 
    servoName.attach(pinMotor);
    servoName.write(0); 
    delay(delayMotor);
  }

}

struct Bin {
  int counterPlastic;
  int counterMetal;
  int levelPlastic;
  int levelMetal;
};
Bin binData={0,0,0,0};


  struct Bin motor2(Servo &servoName1, Servo &servoName2,  int pinMotor1, int pinMotor2, int angle, int thresholdDistance, int delayMotors) { // 0 - LEFT / 90 - center / 180 right

  int capacitive = -1;
  int inductive = -1;

  for (int i = 0; i<50; i++){
   capacitive = filter(capacitiveSensor);
  }
 
  for (int i = 0; i<50; i++){
   inductive = filter(inductiveSensor);
   
  }

  int distance = sonar1.ping_cm();
  
 if (capacitive == HIGH && inductive == HIGH) { 
    Serial.println("[STATUS]: METAL");
    binData.counterMetal++;
    binData.levelMetal+=33; 
    if (binData.levelMetal == 99){
      binData.levelMetal+= 1;   
    } 
     
    servoName2.attach(pinMotor2);
    servoName2.write(0); 
    delay(delayMotors); 
    servoName2.write(angle);
    delay(delayMotors);    
    servoName2.detach();

    if (distance >= thresholdDistance){
    delay(delayMotors); 
    servoName1.write(angle); 
    delay(delayMotors); 
    servoName1.detach();
    }

  } else if (capacitive == HIGH && inductive == LOW) { 
    Serial.println("[STATUS]: PLASTIC");
    binData.counterPlastic++;
    binData.levelPlastic+=33;   
    if (binData.levelPlastic == 99){
      binData.levelPlastic+= 1;
      }   
    
    servoName2.attach(pinMotor2);
    servoName2.write(180); 
    delay(delayMotors);
    servoName2.write(180 - angle);
    delay(delayMotors);      
    servoName2.detach();

    if (distance >= thresholdDistance){
    delay(delayMotors);
    servoName1.write(angle); 
    delay(delayMotors); 
    servoName1.detach();
    }
  } else {
    Serial.println("[STATUS]: WAIT");
  }
  return binData;
}


 void sendDataToWiFi(Bin binData){
   
   Serial.println("Sending data to ESP8266...");
   
   espSerial.println(binData.counterMetal);
   espSerial.println(binData.counterPlastic);
   espSerial.println(binData.levelMetal);
   espSerial.println(binData.levelPlastic);

 }

bool isFull = true;

void loop() {
  isFull = maxBin(11); 
  Serial.println(isFull == false ? "GOL" : "PLIN");
  if (isFull == false){
    Serial.println("[STATUS]: START");  

    motor1(servoMotor1, 12, 90, 30, 2000);

    struct Bin binData = motor2(servoMotor1, servoMotor2, 12, 13, 90, 30, 2000);
    sendDataToWiFi(binData); 
 
  }
}    
