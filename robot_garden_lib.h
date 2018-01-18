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

  int _start_num; // starting pixel number
  int _end_num; // ending pixel number

  int _rate; // step drop rate
  int _drop_count; // numer of steps until one is dropped

  int _head_num; // first lit pixel of the chase
  int _tail_num; // last lit pixel of the chase
  
  public:
    SimpleChase(
      Adafruit_NeoPixel strip,
      int first,
      int last,
      int red,
      int green,
      int blue,
      float fade_factor,
      int rate);
    
    void ChaseLoop();
    void Chase();
    void ChaseStep();
    void _ChaseStep();
};

#endif
