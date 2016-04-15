/* Arduino code to record reed contact events and store them in the internal memory
    Fast and no sd-card module needed, but limited data points can be saved (255 bytes)
    I yet have to draw a scematic of the hardware unit
    
  Jeroen De Maeyer
  15/04/2016
  A big thank you to the online Arduino community for the infinite amout of recources out there!
*/

#include <Wire.h>       // Library to communicate over i2c bus
#include <EEPROMex.h>   // Extended eeprom library with more functions
#include <EEPROMVar.h>  // Library for internal eeprom



#define disk1 0x50 //Address on the i2c bus for eeprom 24LC64

const byte buttonPinRed = 5;  // "stop measuring" button
const byte sensorPin = 2;     // "reed switch" input
const byte buttonPin = 3;     // "start measuring / send data to pc" button
const byte switchPin = 4;     // select "measuring / pc" mode
const byte ledPinRed = 9;     // "reed contact closed" red led
const byte ledPin = 8;        // "measuring active" green led

boolean sensor = 0;
boolean button = 0;
boolean buttonRed = 0;
boolean sensorOld = 0;
boolean buttonOld = 0;
boolean buttonOldRed = 0;

unsigned int dataLength = 255;
unsigned long time;
unsigned long timeOld;

unsigned short timeStored;
unsigned short timeData[255];

unsigned int counter = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  pinMode(sensorPin, INPUT_PULLUP); // reed contact directly connected to Arduino
  pinMode(buttonPin, INPUT);        // external pull down resistor
  pinMode(buttonPinRed, INPUT);     // external pull down resistor
  pinMode(switchPin, INPUT);        // external pull down resistor
  
  pinMode(ledPinRed, OUTPUT);
  pinMode(ledPin, OUTPUT);
  
  digitalWrite(ledPin, LOW);
}

void loop() {
  if (digitalRead(switchPin) == 0) {   
    button = digitalRead(buttonPin);
    if (button == 1 && buttonOld == 0) {
      digitalWrite(ledPin, HIGH);
      measure();
      digitalWrite(ledPin, LOW);
      Serial.println("Measuring done, start of saving data");
      EEPROM.writeBlock(0, timeData);
      Serial.println("Data saved in eeprom");
    }
    buttonOld = button;
    delay(100);
    
  }
  else {
    digitalWrite(ledPin, LOW);
    counter = 0;
    button = digitalRead(buttonPin);
    if (button == 1 && buttonOld == 0) {
      sendDataToPC();
    }
    buttonOld = button;
    delay(100);
  }
}

void measure() {
  Serial.println("Time  ");
  boolean stopMeasurement = 0;
  while(counter < dataLength && stopMeasurement == 0) {
    
    sensor = digitalRead(sensorPin);
    buttonRed = digitalRead(buttonPinRed);
    
    if (sensor == 0 && sensorOld == 1) {
      digitalWrite(ledPinRed, HIGH);
      time = millis();
      timeStored = time;
      Serial.print(counter+1);
      Serial.print("  ");
      Serial.println(timeStored/1000.0, 3);
      timeData[counter] = timeStored;
      
      timeOld = time;
      counter++;
      delay(10); // Delay to avoid to much noise infuence
      
      digitalWrite(ledPinRed, LOW);
    }
    
    if (buttonRed == 1 && buttonOldRed == 0) {
        stopMeasurement = 1;
        timeData[counter] = 0;
        counter++;   
    }
      
    buttonOldRed = buttonRed;
    sensorOld = sensor;
  }

  delay(10);
}

void sendDataToPC() {
  EEPROM.readBlock(0, timeData);
  delay(10);
  Serial.println("start");
  for(int i=0; i<dataLength; i++) {
    if (timeData [i] == 0) {
      Serial.println("stop");
      Serial.println("start");
    }
    else {
      Serial.println(timeData[i]/1000.0,3);
    }
  }
  Serial.println("stop");
}
