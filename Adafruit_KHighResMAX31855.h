#pragma once

#include <Adafruit_MAX31855.h>
//  https://learn.adafruit.com/calibrating-sensors/maxim-31855-linearization


class  Adafruit_KHighResMAX31855 {
 private: bool isReady;
 private: void ready();
 public: Adafruit_MAX31855 lowRes;
 public: Adafruit_KHighResMAX31855(int8_t SCLK, int8_t CS, int8_t MISO);
 public: Adafruit_KHighResMAX31855(int8_t CS);
 public: double readCelsius();
 public: double readFahrenheit();
 public: double readKelvin();  

 public: inline double readFarenheit() // mispell as original
  { return readFahrenheit(); } 

 public: inline uint8_t readError() { return lowRes.readError(); }
 public: inline double readInternal() { return lowRes.readInternal(); }
};
