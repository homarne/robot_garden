#include <Adafruit_NeoPixel.h>
#include "robot_garden_lib.h"
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 4

#define STOP_REQ 6
#define STOP_ACK 5

#define MASTER 1
#define SLAVE 0

#define UNASSERT false
#define ASSERT true

#define RUN_COUNT 1000

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(3*144, PIN, NEO_GRB + NEO_KHZ800);

SimpleChase strand_01 = SimpleChase(strip,0,144,255,0,0,0.8,10);
SimpleChase strand_02 = SimpleChase(strip,144,288,0,255,0,0.8,7);
SimpleChase strand_03 = SimpleChase(strip,288,432,0,0,255,0.8,4);
//SimpleChase strand_04 = SimpleChase(strip,432,576,255,255,255,0.8,1);
//
int status_1 = 0;
int status_2 = 0;
int status_3 = 0;
//int status_4 = 0;

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

int ms_mode = MASTER;
bool stop_request = false;

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code


  Serial.begin(9600);
  Serial.println("setup");
  
  strip.begin();
  strip.setBrightness(255);
  strip.show(); // Initialize all pixels to 'off'

  if (ms_mode == MASTER)
  {
    initialize_master_io();
  }
  else
  {
    initialize_slave_io();
  }

}

void loop() {
      
    Serial.println("running");
    
    int i = 0;
    bool run = true; 
    while (run)
    {
      status_1 = strand_01.ChaseStep();
      status_2 = strand_02.ChaseStep();
      status_3 = strand_03.ChaseStep();
//      status_4 = strand_04.ChaseStep();
      
      strip.show();

      // if slave, check for stop request
      if (ms_mode == SLAVE)
      {
        run = slave_check_for_stop();
      }
      else // mode == MASTER
      {
        i++;
        if (i >= RUN_COUNT)
        {
          run = false;
        }
      }
    }

    Serial.println("stopping");

    if (ms_mode == MASTER)
    {
      master_stop_request(ASSERT);
    }
    
//    Serial.println((status_1 || status_2 || status_3 || status_4));
//    Serial.println((status_1 + status_2 + status_3 + status_4));

    while (status_1 == 1 || status_2 == 1 || status_3 == 1) // || status_4 == 1)
    {    
      //Serial.println("waiting");
      if (status_1 == 1)
      {
        status_1 = strand_01.ChaseStep();
      }
      if (status_2 == 1)
      {
        status_2 = strand_02.ChaseStep(); 
      }
      if (status_3 == 1)
      {
        status_3 = strand_03.ChaseStep();
      }
//      if (status_4 == 1)
//      {
//        status_4 = strand_04.ChaseStep(); 
//      }

      strip.show();
    }

    Serial.println("stopped");
    
    if (ms_mode == SLAVE)
    {
      slave_stop_ack(ASSERT);
      slave_wait_for_run();
    }
    else // ms_mode = MASTER
    {
      master_wait_for_all_stopped();
      
      Serial.println("master pausing");
      
      delay(5000);
    }
      
    Serial.println("resuming");

    if (ms_mode == MASTER)
    {
      master_stop_request(UNASSERT);
    }
    else // ms_mode = SLAVE
    {
      slave_stop_ack(UNASSERT);
    }


//  // Some example procedures showing how to display to the pixels:
//  colorWipe(strip.Color(255, 0, 0), 50); // Red
//  colorWipe(strip.Color(0, 255, 0), 50); // Green
//  colorWipe(strip.Color(0, 0, 255), 50); // Blue

}

void initialize_master_io()
{
  pinMode(STOP_ACK, INPUT_PULLUP);     // stop ack is input
  pinMode(STOP_REQ, OUTPUT);          // stop request is output
  digitalWrite(STOP_REQ, HIGH);      // stop request unasserted
}

void initialize_slave_io()
{
  pinMode(STOP_ACK, OUTPUT);          // stop acknowledge is output
  digitalWrite(STOP_ACK, LOW);       // stop acknowledge is unasserted
  pinMode(STOP_REQ, INPUT_PULLUP);  // stop request is input
}

void master_stop_request(bool state)
{
  if (state) // ASSERT
  {
    digitalWrite(STOP_REQ, LOW);  // assert stop request
  }
  else // UNASSERT
  {
    digitalWrite(STOP_REQ, HIGH);  // unassert stop request
  }
}

void master_wait_for_all_stopped()
{
  Serial.println("master waiting for all stopped");
  
  bool all_acked = false;
  while (all_acked == false)
  {
    all_acked = digitalRead(STOP_ACK);
  }
}

bool slave_check_for_stop()
{
  return digitalRead(STOP_REQ);
}

void slave_wait_for_run()
{
  Serial.println("slave waiting for run");
  
  bool run = false;
  while (run != true)
    {
      run = digitalRead(STOP_REQ);
    }
}

void slave_stop_ack(bool state)
{
  if (state) // ASSERT
  {
    pinMode(STOP_ACK, INPUT_PULLUP); // acknowledge stop request by placing STOP_ACK pin in pull-up state
                                    // when all slave STOP_ACK pins are acknowleded, 
                                   // the STOP_ACK pin on MASTER will go high
  }
  else // UNASSERT
  {
    pinMode(STOP_ACK, OUTPUT);      // set STOP_ACK pin as output
    digitalWrite(STOP_ACK, LOW);   // and set output low
  }
}


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}


