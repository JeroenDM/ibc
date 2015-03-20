
// Originele funtie 20 maart 2015
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

// aangepaste functie om lopend gemiddelde te kunnen doen
void readAnalogSensors2(int index) {
  
  // AnalogRead wordt telkens twee keer uitgevoerd om interferentie van vorige metingen te vermijden.
  // Koppel inlezen [Nm]
  analogRead(A2);
  Tas_sum -= Tas_array[index];
  Tas_array[index] = (analogRead(A2)/1023.0*referenceVoltage - 2.50 + torqueOffset)*202.9; //0.006V per Nm
  Tas_sum += Tas_array[index];
  
  // Stroom inlezen
  analogRead(A1);
  Ibat_sum -= Ibat_array[index];
  Ibat_array[index] = -24*(analogRead(A1) - 512)/1023.0*referenceVoltage + currentOffset;  // [A]
  Ibat_sum += Ibat_array[index];
  
  // Spanningmeter op basis van spanningdeler met Vmax = 60V->5V op arduino ingang
  analogRead(A0);
  Vbat_sum -= Vbat_array[index];
  Vbat_array[index] = analogRead(A0)/1023.0*referenceVoltage*11.675;  // [V]
  Vbat_sum += Vbat_array[index]; 
}

void ckeckPulses() {
  // Pulstreinen omzetten in snelheid als er een  puls is
    time1 = millis();
    // Min trapfrequentie 10 rpm -> 1/6 rps dus min 2 pulsen per s -> Tmax = 0.5s = 500ms
    if ((time1 - timeOld1) < 500){
    if (flag1) {
      // Max trapfrequentie 120 rpm -> 2 rps dus max 24 puslen per s -> Tmin ~ 0.04s = 40ms
      if ((time1 - timeOld1) > 20) {
        Ras = (time1 - timeOld1)/1000.0;
        Ras = theta/Ras;
        Ras = Ras*60/2/3.1415; // Convert to rpm
        timeOld1 = time1;
        }
        flag1 = false; // Eventueel via hulpvariabele en interrupts uitzetten als fouten
      }
    }
    else {
      Ras = 0;
      if (flag1){
        timeOld1 = time1;
        flag1 = false;
      }
     }
    
    time2 = millis();
    // Min snelheid 3.6 km/h ~ 1m/s ~ 1/2 pulsen per s Tmin = 2000ms
    if ((time2 - timeOld2) < 2000){
      if (flag2) {
        // Max snelheid 40 km/h ~ 11m/s ~ 5.3 pulsen per s Tmin = 190ms
        if ((time2 - timeOld2) > 100) {
          Vbike = (time2 - timeOld2)/1000.0;
          Vbike = wheelCirc/Vbike*3.6;
          timeOld2 = time2;
        }
        flag2 = false; // Eventueel via hulpvariabele en interrupts uitzetten als fouten
      }
    }
    else{
      Vbike = 0;
      if(flag2){
        timeOld2 = time2;
        flag2 = false;
      }
    }
}
