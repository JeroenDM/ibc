/* This code reads the torque sensor and sends it to the pc
   for real time plotting in scilab to ease calibration
*/

float referenceVoltage = 5;
float torqueOffset = 0;
float torqueScaling = 202.9;

void setup() {
  Serial.begin(9600);

}

void loop() {
  float voltage = analogRead(A2)/1023.0*referenceVoltage;
  voltage = analogRead(A2)/1023.0*referenceVoltage;
  
  float torque = (voltage - 2.5 + torqueOffset)*torqueScaling;
  
  Serial.print(voltage);
  Serial.print("\t");
  Serial.println(torque);
  delay(200);  
}
