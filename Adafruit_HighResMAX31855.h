#pragma once

#include <Adafruit_MAX31855.h>
//  https://learn.adafruit.com/calibrating-sensors/maxim-31855-linearization


class  Adafruit_HighResMAX31855 {
 private: bool isReady;
 private: void ready();
 public: Adafruit_MAX31855 lowRes;
 public: Adafruit_HighResMAX31855(int8_t SCLK, int8_t CS, int8_t MISO);
 public: Adafruit_HighResMAX31855(int8_t CS);
 public: double readCelsius();
 public: double readFahrenheit();
 public: double readKelvin();  

 public: inline double readFarenheit() // mispell as original
  { return readFahrenheit(); } 

 public: uint8_t readError();
 public: double readInternal();
};
