#include "Arduino.h"
#include "death.h"

SimpleDeath::SimpleDeath(
  int red,
  int green,
  int blue)
{
  int _red=red;
  int _green=green;
  int _blue = blue;
}
    
void SimpleDeath::Die()
{
  Serial.print("dying inside");
}

