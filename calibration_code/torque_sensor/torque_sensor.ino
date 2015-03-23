/* This code reads the torque sensor and sends it to the pc
   for real time plotting in scilab to ease calibration
*/

void setup() {
  Serial.begin(9600);

}

void loop() {
  float voltage = analogRead(A0)/1023.0*5;
  float voltage2 = analogRead(A1)/1023.0*3;
  Serial.print(voltage);
  Serial.print("\t");
  Serial.println(voltage2);
  delay(200);  
}
