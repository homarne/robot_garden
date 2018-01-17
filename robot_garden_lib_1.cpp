#include <Adafruit_NeoPixel.h>
#include "robot_garden_lib_1.h"

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
  
void SimpleChase::Chase()
{
  for(uint16_t i=0; i<_strip.numPixels(); i++) 
  {
    _strip.setPixelColor(i, _strip.Color(_red,_green,_blue));
    int _fade_count=i;
    int _scaled_red=255;
    while(_fade_count>0 && _scaled_red>0)
    {
      _scaled_red=(float)_scaled_red*_fade_factor;
      _fade_count-=1;
      _strip.setPixelColor(_fade_count, _strip.Color(_scaled_red,0,0));
    }
    _strip.show();
    delay(1);
  }
}

