/* Dit programma leest en verwerkt de data van verschillende sensoren en stuurt deze
   naar de computer over de seriële poort voor real time testen in het labo.
   
   Auteur: Jeroen De Maeyer
   
*/

#include <SD.h>

// Variabelen die gelogd worden:
float time = 0;
float Tas = 0;    // Koppel gemeten in trapas
float Ibat = 0;   // Stroom gemeten aan batterij
float Vbat = 0;   // Spanning gemeten aan batterij
float Ras = 0;    // Trapsnelheid gemeten aan trapas
float Vbike = 0;  // Snelheid van de fiets
float Temp = 0;   // Omgevingstemperatuur
const int ma_length = 20;

// Vectoren voor lopend gemiddelde
float Tas_array[ma_length];
float Tas_sum;
float Ibat_array[ma_length];
float Ibat_sum;
float Vbat_array[ma_length];
float Vbat_sum;

// Functionele variabelen
// Zet deze variable op true als je output over de seriele poort wilt
// NIET aanzetten als er geen usb kabel is aangesloten en je wil dat het programma werkt
boolean serialOutput = false;
unsigned long time1;
unsigned long time2;
unsigned long timeOld1;
unsigned long timeOld2;
unsigned long previousTime;
unsigned long currentTime;

// Constanten
const float theta = 2*3.1415/6; // Hoek in rad tussen twee pulsen bij trapas
const float wheelCirc = 2.1; // Omtrek van het fietswiel met snelheidsensor
// change this to match your SD shield or module;
// Arduino Ethernet shield: pin 4
// Adafruit SD shields and modules: pin 10
// Sparkfun SD shield: pin 8
const int chipSelect = 10;
const float referenceVoltage = 5.00;
const float currentOffset = 0.3;
const float torqueOffset = 0;  // IN VOLT

// Interrupt flags, 1 voor trapsnelheid, 2 voor snelheid feits
volatile boolean flag1 = false;
volatile boolean flag2 = false;

// Interrupt service routines, actief wanneer magneet voorbij sensor passeerd
void isr1() {
  flag1 = true;
}

void isr2() {
  flag2 = true;
}

//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
void setup() {
  
  Serial.begin(9600);
  serialOutput = true;
  
  // Stel de interrupts en in- en uitgangen in
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(0, isr1, FALLING);
  pinMode(3, INPUT_PULLUP);
  attachInterrupt(1, isr2, FALLING);
  
  // Initialiseer enkele variabelen
  previousTime = millis();
  currentTime = millis();
}

//-----------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
void loop() {
  for (int i=0; i<ma_length; i++) {
    previousTime = currentTime;
    while((currentTime - previousTime) < 50) {
      // check of er pulsen zijn geweest van de rotatiesensoren
      ckeckPulses();
      currentTime = millis();
    }
    
    // Lees de analoge sensoren in
    readAnalogSensors2(i);  
  }
  
  Tas = Tas_sum/ma_length;
  Ibat = Ibat_sum/ma_length;
  Vbat = Vbat_sum/ma_length;
  time = millis()/1000.0;
  
  
  // Schrijf een regel met data over seriele poort indien gewenst
  if(serialOutput) {
    printData();
  }
  
}
