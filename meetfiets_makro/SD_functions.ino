
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
    dataFile.close();
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

void clearData() {
  if(SD.remove("datalog.txt")) Serial.println("Data deleted from sd card.");
}
