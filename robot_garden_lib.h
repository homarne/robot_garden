#ifndef robot_garden_lib_h
#define robot_garden_lib_h

//#include "Arduino.h"
#include <Adafruit_NeoPixel.h>

class SimpleChase
{
  Adafruit_NeoPixel _strip;
  int _red;
  int _green;
  int _blue;
  float _fade_factor;
  
  public:
    SimpleChase(
      Adafruit_NeoPixel strip,
      int red,
      int green,
      int blue,
      float fade_factor);
    
    void Chase();
};

#endif
