#include <Adafruit_NeoPixel.h>
#include "robot_garden_lib.h"

SimpleChase::SimpleChase(
    Adafruit_NeoPixel strip,
    int first,
    int last,
    int red,
    int green,
    int blue,
    float fade_factor,
    int rate)
{
  _strip = strip;
  _red = red;
  _green = green;
  _blue = blue;
//  _fade_factor = fade_factor;
  _fade_factor = fade_factor;
  _rate = rate;

  _start_num = first; // starting pixel number
  _end_num = last; // ending pixel number

  _head_num = _start_num; // initialize head to start
  _tail_num = _start_num; // initilize tail to start

  _drop_count = _rate;
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

void SimpleChase::ChaseLoop()
{
  int _head_num = _start_num;// head of the chase
  int _tail_num = _start_num;// tail of the chase
  while(_tail_num < _end_num) 
  {
    if (_head_num < _end_num) // dont update pixels beyond the end  
    {
      _strip.setPixelColor(_head_num, _strip.Color(_red,_green,_blue));
    }
    int _fade_count=_head_num;
    
    int _scaled_red = _red;
    int _scaled_green = _green;
    int _scaled_blue = _blue;
    int _total_color = _scaled_red + _scaled_green + _scaled_blue;

    // walk back from the head and program dimmer pixels
    // until either _start_num is reached or brightness reaches zero
    while(_fade_count>_start_num && _total_color>0) 
    {
      _scaled_red=(float)_scaled_red * _fade_factor;
      _scaled_green=(float)_scaled_green * _fade_factor;
      _scaled_blue=(float)_scaled_blue * _fade_factor;
      _total_color = _scaled_red + _scaled_green + _scaled_blue;
      
      _fade_count-=1;
      if (_fade_count < _end_num) // dont update pixels beyond the end
      {
        _strip.setPixelColor(_fade_count, _strip.Color(_scaled_red,_scaled_green,_scaled_blue));
      }
    }
    _tail_num = _fade_count; // update the tail position
    _head_num += 1; // move the head forward
    
    _strip.show();
    delay(1);
  }
}

void SimpleChase::Chase()
{
  while (1)
  {
    ChaseStep();
    _strip.show();
    delay(1);
  }
}

void SimpleChase::ChaseStep()
{
  if (_rate == 10)
  {
    _ChaseStep();
  }
  else if (_drop_count > 0)
  {
    _ChaseStep();
    _drop_count -= 1;
  }
  else 
  {
    _drop_count = _rate;
  }
}

void SimpleChase::_Set_Random_Color()
{
//    _red = random(0, 255);
//    _green = random(0,255);
//    _blue = random(0,255);

    int _color_count = sizeof(robo_colors)/(sizeof(int)*3); // number of rgb colors in color array
    int _new_color = random(0,_color_count);

    _red = robo_colors[_new_color][0];
    _green = robo_colors[_new_color][1];
    _blue = robo_colors[_new_color][2];
}

int SimpleChase::_Direction(int pixel_number)
{
  if (_direction == 0){
    return pixel_number;
  }
  else{
    return _start_num + _end_num - pixel_number - 1;
  }
}

void SimpleChase::_Set_Random_Tail()
{
  int tail_factor = random(75,95);
  _fade_factor = (float)tail_factor/100;
  Serial.println("tail factor: ");
  Serial.println(_direction);
}


void SimpleChase::_ChaseStep()
{
  if(_tail_num >= _end_num)
  {
    _Set_Random_Color();
    _direction = random(0,2);
    Serial.println("direction: ");
    Serial.println(_direction);
    _Set_Random_Tail();
    _head_num = _start_num;
    _tail_num = _start_num;
  }
  
  if (_head_num < _end_num) // dont update pixels beyond the end  
  {
    _strip.setPixelColor(_Direction(_head_num), _strip.Color(_red,_green,_blue));
  }
  int _fade_count=_head_num;

  int _scaled_red = _red;
  int _scaled_green = _green;
  int _scaled_blue = _blue;
  int _total_color = _scaled_red + _scaled_green + _scaled_blue;

  // walk back from the head and program dimmer pixels
  // until either _start_num is reached or brightness reaches zero
  while(_fade_count>_start_num && _total_color>0) 
  {
    _scaled_red=(float)_scaled_red * _fade_factor;
    _scaled_green=(float)_scaled_green * _fade_factor;
    _scaled_blue=(float)_scaled_blue * _fade_factor;
    _total_color = _scaled_red + _scaled_green + _scaled_blue;
    
    _fade_count-=1;
    if (_fade_count < _end_num) // dont update pixels beyond the end
    {
      _strip.setPixelColor(_Direction(_fade_count), _strip.Color(_scaled_red,_scaled_green,_scaled_blue));
    }
  }
  _tail_num = _fade_count; // update the tail position
  _head_num += 1; // move the head forward
  
}

