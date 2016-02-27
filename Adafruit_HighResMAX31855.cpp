#include <Adafruit_HighResMAX31855.h>

void Adafruit_HighResMAX31855::ready() {
  if (!isReady) {
    uint32_t t=millis();
    if (t < 200) { delay(200-t); } // 200ms to prep
    lowRes.readCelsius(); // first reading may be wrong
    isReady=true;
  }
}

Adafruit_HighResMAX31855::Adafruit_HighResMAX31855(int8_t SCLK, int8_t CS, int8_t MISO)
  : isReady(false), lowRes(SCLK,CS,MISO) {}

Adafruit_HighResMAX31855::Adafruit_HighResMAX31855(int8_t CS)
  : isReady(false), lowRes(CS) {}

double Adafruit_HighResMAX31855::readInternal() {
  ready();
  return lowRes.readInternal();
}


double Adafruit_HighResMAX31855::readCelsius() {
  ready();

  // Code from jh421797 (slightly modified to avoid multiple reads,
  // and account for error codes by wmacevoy).

  float thermocoupleTemperature = lowRes.readCelsius(); 
  if (isnan(thermocoupleTemperature)) return NAN;

  float coldJunctionTemperature = lowRes.readInternal();  

   // MAX31855 thermocouple voltage reading in mV
  float thermocoupleVoltage = (thermocoupleTemperature-coldJunctionTemperature)*0.041276;
   
  float coldJunctionVoltage = -0.176004136860E-01 +
      0.389212049750E-01  * coldJunctionTemperature +
      0.185587700320E-04  * pow(coldJunctionTemperature, 2.0) +
     -0.994575928740E-07  * pow(coldJunctionTemperature, 3.0) +
      0.318409457190E-09  * pow(coldJunctionTemperature, 4.0) +
     -0.560728448890E-12  * pow(coldJunctionTemperature, 5.0) +
      0.560750590590E-15  * pow(coldJunctionTemperature, 6.0) +
     -0.320207200030E-18  * pow(coldJunctionTemperature, 7.0) +
      0.971511471520E-22  * pow(coldJunctionTemperature, 8.0) +
     -0.121047212750E-25  * pow(coldJunctionTemperature, 9.0) +
      0.118597600000E+00  * exp(-0.118343200000E-03 * 
                                pow((coldJunctionTemperature-0.126968600000E+03), 2.0) );
                        
   // cold junction voltage + thermocouple voltage         
   float voltageSum = thermocoupleVoltage + coldJunctionVoltage;
   
   // calculate corrected temperature reading based on coefficients for 3 different ranges   
   float b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10;
   if(thermocoupleVoltage < 0){
      b0 = 0.0000000E+00;
      b1 = 2.5173462E+01;
      b2 = -1.1662878E+00;
      b3 = -1.0833638E+00;
      b4 = -8.9773540E-01;
      b5 = -3.7342377E-01;
      b6 = -8.6632643E-02;
      b7 = -1.0450598E-02;
      b8 = -5.1920577E-04;
      b9 = 0.0000000E+00;
   }
   
   else if(thermocoupleVoltage < 20.644){
      b0 = 0.000000E+00;
      b1 = 2.508355E+01;
      b2 = 7.860106E-02;
      b3 = -2.503131E-01;
      b4 = 8.315270E-02;
      b5 = -1.228034E-02;
      b6 = 9.804036E-04;
      b7 = -4.413030E-05;
      b8 = 1.057734E-06;
      b9 = -1.052755E-08;
   }
   
   else if(thermocoupleVoltage < 54.886){
      b0 = -1.318058E+02;
      b1 = 4.830222E+01;
      b2 = -1.646031E+00;
      b3 = 5.464731E-02;
      b4 = -9.650715E-04;
      b5 = 8.802193E-06;
      b6 = -3.110810E-08;
      b7 = 0.000000E+00;
      b8 = 0.000000E+00;
      b9 = 0.000000E+00;
   }
   
   else {
      return NAN;
   }
   
   return b0 + 
      b1 * voltageSum +
      b2 * pow(voltageSum, 2.0) +
      b3 * pow(voltageSum, 3.0) +
      b4 * pow(voltageSum, 4.0) +
      b5 * pow(voltageSum, 5.0) +
      b6 * pow(voltageSum, 6.0) +
      b7 * pow(voltageSum, 7.0) +
      b8 * pow(voltageSum, 8.0) +
      b9 * pow(voltageSum, 9.0);
}

double Adafruit_HighResMAX31855::readFahrenheit() {
  double C = readCelsius();
  if (isnan(C)) return NAN;
  return (9.0/5.0)*C+32.0;
}

double Adafruit_HighResMAX31855::readKelvin() {
  double C = readCelsius();
  if (isnan(C)) return NAN;
  return C+273.16;
}
