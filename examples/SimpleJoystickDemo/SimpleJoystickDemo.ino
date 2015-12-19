
/*

Arduino Leonardo Joystick Library Demo.
No input, just a demo that moves everything virtually.

See Joystick.h for ackgnowlegments.
*/

// define which axes we want to use
#define LEOJOY_AXIS_X 1
#define LEOJOY_AXIS_Y 1
#define LEOJOY_AXIS_Z 0
#define LEOJOY_AXIS_Rx 0
#define LEOJOY_AXIS_Ry 0
#define LEOJOY_AXIS_Rz 0
#define LEOJOY_AXIS_Slider 1
#define LEOJOY_AXIS_Dial 1
#define LEOJOY_AXIS_Wheel 1
#define LEOJOY_AXIS_Vx 0
#define LEOJOY_AXIS_Vy 0
#define LEOJOY_AXIS_Vz 0
// define resolution in bit (up to 15 bit, 16 bit currently has issues with singedness but don't care - 15 is more than enough)
#define LEOJOY_AXIS_BITS 10

// number of buttons
#define LEOJOY_NUMBUTTONS 16

// number of hats
#define LEOJOY_NUMHATS 1

// enabling debugging will write a ton of low level info on Serial
//#define LEOJOY_DEBUG

// define a pin that will mute Joystick USB Reports to allow safe uploading of new sketch (have an ISP ready if you don't do this)
#define LEOJOY_MUTE_PIN 3

// note that the LEOJOY_* defines need to happen before the include
#include <Joystick.h>

#define LED 13

void setup()
{
#ifdef LEOJOY_DEBUG
    pinMode(LED, OUTPUT);

    // Enable Serial port for basic console debugging. 
    Serial.begin(115200);

    // blink for 15 seconds (60*0,25) to allow serial monitor to attach
    for (int i = 0 ; i < 60; i++) {
      digitalWrite(LED, ((i % 2) ==0));
      delay(250);
    }
#endif

    Joystick.begin();
        
}// end setup


void loop()
{
  // turn on all buttons
  for (int i =0; i < LEOJOY_NUMBUTTONS; i++) {
  	Joystick.press(i);
    delay(50);
  }   
  // turn off all buttons
  for (int i =0; i < LEOJOY_NUMBUTTONS; i++) {
    Joystick.release(i);
    delay(50);
  }   

  // flip the hats in all directions
  for (int i =0; i < LEOJOY_NUMHATS; i++) {
    for (int j =0; j < 9; j++) {
      Joystick.hat(i,j);
      delay(125);
    }
    // disable hat
    Joystick.hat(i,0);
  }   

  // move all axes, then center at middle
  long maxvalue = (1l<<LEOJOY_AXIS_BITS);
  for (int i =0; i < LEOJOY_NUMAXIS; i++) {
    for (long j =0; j < maxvalue; j+=(maxvalue/100)) {
      Joystick.axis(i,j);
      delay(10);
    }
    Joystick.axis(i,maxvalue/2);
  }   

}// end main program loop
