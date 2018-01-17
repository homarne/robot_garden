#include <Adafruit_NeoPixel.h>
#include "robot_garden_lib.h"

SimpleChase::SimpleChase(
    Adafruit_NeoPixel strip,
    int red,
    int green,
    int blue,
    float fade_factor)
{
  _strip = strip;
  _red = red;
  _green = green;
  _blue = blue;
  _fade_factor = fade_factor;
}
  
//void SimpleChase::Chase()
//{
//  int _start_num = 0;
//  int _end_num = 144;
//  int _tail_length = 0;
////  for(uint16_t i=0; i<_strip.numPixels(); i++)
//  // run the chase with tail 
//  for(uint16_t i=_start_num; i<_end_num; i++) 
//  {
//    _strip.setPixelColor(i, _strip.Color(_red,_green,_blue));
//    int _fade_count=i;
//    int _scaled_red=255;
//
//    _tail_length = 0;
//    while(_fade_count>0 && _scaled_red>0)
//    {
//      _scaled_red=(float)_scaled_red*_fade_factor;
//      _fade_count-=1;
//      _strip.setPixelColor(_fade_count, _strip.Color(_scaled_red,0,0));
//      _tail_length += 1
//    }
//    _strip.show();
//    delay(1);
//  }
//}

void SimpleChase::Chase()
{
  int _start_num = 0; // starting pixel number
  int _end_num = 144; // ending pixel number
//  int _tail_length = 0;
//  for(uint16_t i=0; i<_strip.numPixels(); i++)
  // run the chase with tail 
  int _head_num = _start_num;// head of the chase
  int _tail_num = _start_num;// tail of the chase
  while(_tail_num < _end_num) 
  {
    if (_head_num < _end_num) // dont update pixels beyond the end  
    {
      _strip.setPixelColor(_head_num, _strip.Color(_red,_green,_blue));
    }
    int _fade_count=_head_num;
    int _scaled_red=255;

    // walk back from the head and program dimmer pixels
    // until either _start_num is reached or brightness reaches zero
    while(_fade_count>_start_num && _scaled_red>0) 
    {
      _scaled_red=(float)_scaled_red*_fade_factor;
      _fade_count-=1;
      if (_fade_count < _end_num) // dont update pixels beyond the end
      {
        _strip.setPixelColor(_fade_count, _strip.Color(_scaled_red,0,0));
      }
    }
    _tail_num = _fade_count; // update the tail position
    _head_num += 1; // move the head forward
    
    _strip.show();
    delay(1);
  }
}

