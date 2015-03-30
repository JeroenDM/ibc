
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

void askUser() {
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
}

void printData() {
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
