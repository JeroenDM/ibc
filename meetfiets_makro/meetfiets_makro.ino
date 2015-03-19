/* Dit programma leest en verwerkt de data van verschillende sensoren en schrijft deze naar de seriële poort
   Daar kan ze dan worden uitgelezen door een computer, in dit geval de raspberry pi waar een python script
   op loopt.
   
   Auteur: Jeroen De Maeyer
   Versie 2 (13/01/2015)
   
   Nog doen: overflow protection voor millis()
              minimum rotatiesnelheid, anders 0
   v5: sd card functionaliteit zit in aparte functies zodat gemakkelijk zonder kan gewerkt worden.
   v7 tussntijds herstelpunt aanmaken
   v8 alles op orde stellen en consistent maken
      functie connect to pi is verwijderd
      readSD om data van sd kaart te halen via usb
   v9 debugfunctie writeSD om op de sd kaart debug strings te schrijven
      USB check is nu met wachttijd want if(Serial) werkt alleen voor leonardo
   
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
const float theta = 2*3.1415/12; // Hoek in rad tussen twee pulsen bij trapas
const float wheelCirc = 2.1; // Omtrek van het fietswiel met snelheidsensor
const int chipSelect = 8;
const float referenceVoltage = 5.00;
const float currentOffset = 0.2;
const float torqueOffset = -0.1;  // IN VOLT

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
  
  // Kijk of er een usb is aangesloten
  serialOutput = checkUSB(5000);
  
  // Stel de interrupts en in- en uitgangen in
  pinMode(2, INPUT);
  attachInterrupt(0, isr1, FALLING);
  pinMode(3, INPUT_PULLUP);
  attachInterrupt(1, isr2, FALLING);
  
  // Initialiseer enkele variabelen
  previousTime = millis();
  currentTime = millis();
  
  // Initialiseer sd-kaart
  initSD();
  
  // Vraag of de data ingelezen moet worden en voer daarna gewoon loop() uit
  if(serialOutput) {
    Serial.println("Type r if you want to read the data on the sd card.");
    Serial.println("Type n if you want to continue with measuring.");
    Serial.println("Type d to delete data from SD card.");
    boolean wait = true;
    while(wait) {
      if (Serial.available() > 0) {
        char antwoord = Serial.read();
        if (antwoord == 'r') {
          readSD();
          wait = true;
          Serial.println("Type r if you want to read the data on the sd card.");
          Serial.println("Type n if you want to continue with measuring.");
          Serial.println("Type d to delete data from SD card.");
        }
        else if (antwoord == 'd') {
          clearData();
          wait = true;
          Serial.println("Type r if you want to read the data on the sd card.");
          Serial.println("Type n if you want to continue with measuring.");
          Serial.println("Type d to delete data from SD card.");
        }
        else if (antwoord == 'n') {
          Serial.println("You don't want to read sd so we continue");
          wait = false;
        }
        else {
          Serial.println("You have to type 'r' or 'n'.");
        }
      }
    }
  }
  
  if(serialOutput) {    
    // Print labels voor data
    Serial.print("Time\tTorque\tIbat\tVoltage\t");
    Serial.println("PedalSpeed\tSpeed");
  }
  writeSD("End of setup");
}

//-----------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
void loop() {
  // Deze lus word elke seconde (1000ms) verlaten om de data weg te schrijven
  while((currentTime - previousTime) < 1000) {
    // check of er pulsen zijn geweest van de rotatiesensoren
    ckeckPulses();
    
    currentTime = millis();
  }
  previousTime = currentTime;
  
  // Lees de analoge sensoren in
  readAnalogSensors();
  
  // Stuur een regel met data naar SD card
  saveDataToSD();
  
  // Schrijf een regel met data over seriele poort indien gewenst
  if(serialOutput) {
    sendData();
  }
  
}

//-----------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------

void initSD() {
  // Kijk of er een SD kaart is en andere setup
  pinMode(chipSelect, OUTPUT);
  if (!SD.begin(chipSelect)) {
    if(serialOutput) Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  if(serialOutput) Serial.println("Card initialized.");
  
  //Schrijf een startregel in het databestand op de SD kaart
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  if(dataFile) {
    dataFile.println("Hier begint een nieuwe reeks metingen.");
  }
  else {
    if(serialOutput) Serial.println("Error in opening file on SD card");
  }
  dataFile.close();
}

void sendData() {
  Serial.print(time, 4);
  Serial.print("\t");
  Serial.print(Tas, 4);
  Serial.print("\t");
  Serial.print(Ibat, 1);
  Serial.print("\t");
  Serial.print(Vbat, 2);
  Serial.print("\t");
  Serial.print(Ras, 2);
  Serial.print("\t");
  Serial.print(Vbike, 2);
Serial.println();
}

void readAnalogSensors() {
  
  // AnalogRead wordt telkens twee keer uitgevoerd om interferentie van vorige metingen te vermijden.
  // Koppel inlezen [Nm]
  Tas = (analogRead(A2)/1023.0*referenceVoltage - 2.50 + torqueOffset)*202.9; //0.006V per Nm
  Tas = (analogRead(A2)/1023.0*referenceVoltage - 2.50 + torqueOffset)*202.9; //0.006V per Nm
  
  // Stroom inlezen
  Ibat = -24*(analogRead(A1) - 512)/1023.0*referenceVoltage + currentOffset;  // [A]
  Ibat = -24*(analogRead(A1) - 512)/1023.0*referenceVoltage + currentOffset;  // [A]
  
  // Spanningmeter op basis van spanningdeler met Vmax = 60V->5V op arduino ingang
  Vbat = analogRead(A0)/1023.0*referenceVoltage*11.675;  // [V]
  Vbat = analogRead(A0)/1023.0*referenceVoltage*11.675;  // [V]
  
  // Tijdstip van opnemen data opslaan, een kleine onnauwkeurigheid
  // door de eindige tijd dat het in beslag neemt om de data in te lezen
  time = millis()/1000.0;
}

void ckeckPulses() {
  // Pulstreinen omzetten in snelheid als er een  puls is
    if (flag1) {
      time1 = millis();
      // Max trapfrequentie 120 rpm -> 2 rps dus max 24 puslen per s -> Tmin ~ 0.04s = 40ms
      if ((time1 - timeOld1) > 20) {
        Ras = (time1 - timeOld1)/1000.0;
        Ras = theta/Ras;
        Ras = Ras*60/2/3.1415; // Convert to rpm
        timeOld1 = time1;
      }
      flag1 = false; // Eventueel via hulpvariabele en interrupts uitzetten als fouten
    }
    
    if (flag2) {
      time2 = millis();
      // Om debouncing te vermijden is 100ms aangeraden
      if ((time2 - timeOld2) > 100) {
        Vbike = (time2 - timeOld2)/1000.0;
        Vbike = wheelCirc/Vbike*3.6;
        timeOld2 = time2;
      }
      flag2 = false; // Eventueel via hulpvariabele en interrupts uitzetten als fouten
    }
}

boolean saveDataToSD() {
  // Open (of maak) het bestand op de SD kaart
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  if(dataFile) {
    dataFile.print(time, 4);
    dataFile.print("\t");
    dataFile.print(Tas, 2);
    dataFile.print("\t");
    dataFile.print(Ibat, 1);
    dataFile.print("\t");
    dataFile.print(Vbat, 2);
    dataFile.print("\t");
    dataFile.print(Ras, 2);
    dataFile.print("\t");
    dataFile.print(Vbike, 2);
    dataFile.println();
    }
  else {
    if(serialOutput) Serial.println("Error in opening file on SD card");
  }
}

void readSD() {
  // Code from http://arduino.cc/en/Tutorial/ReadWrite
  // re-open the file for reading:
  File myFile = SD.open("datalog.txt");
  if (myFile) {
    Serial.println("datalog.txt:");
   
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
        Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
    Serial.println();
    Serial.println("End of file ------------------------");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

void writeSD(String debugString) {
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  if(dataFile) {
    dataFile.println(debugString);
  }
  else {
    if(serialOutput) Serial.println("Error in writing debug string on SD card");
  }
  dataFile.close();
}

boolean checkUSB(unsigned int waitTime) {
  unsigned int startTime = millis();
  Serial.begin(9600);
  while((millis() - startTime) < waitTime) {
    if (Serial.available() > 0) {
      char antwoord = Serial.read();
      if (antwoord == 'c') {
        Serial.print("Connection established!");
        return true;
      }
    }
  }
  return false;
}

void clearData() {
  if(SD.remove("datalog.txt")) Serial.println("Data deleted from sd card.");
}
  
