
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
