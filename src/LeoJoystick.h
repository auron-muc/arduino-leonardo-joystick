/*
  LeoJoystick.h

  Copyright (c) 2015, Tobias Meyer
  Based on code from 
  Arduino LLC,
  helm (see helmpcb.com/electronics/usb-joystick),
  drake250 and RamjetX( Mark Gallon ) (see http://forum.freetronics.com/viewtopic.php?f=27&t=734) and
  Peter Barrett

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef LEOJOY_h
#define LEOJOY_h

#include "HID.h"
#include <stdint.h>
#include <Arduino.h>

#include "BitArrayTemplate.h"

#if !defined(_USING_HID)

#warning "Using legacy HID core (non pluggable)"

#else

//================================================================================
//================================================================================
//  Joystick

// definition of the axes in the Joystick object
#ifndef LEOJOY_AXIS_X
#define LEOJOY_AXIS_X 1
#endif
#ifndef LEOJOY_AXIS_Y
#define LEOJOY_AXIS_Y 1
#endif
#ifndef LEOJOY_AXIS_Z
#define LEOJOY_AXIS_Z 0
#endif
#ifndef LEOJOY_AXIS_Rx
#define LEOJOY_AXIS_Rx 0
#endif
#ifndef LEOJOY_AXIS_Ry
#define LEOJOY_AXIS_Ry 0
#endif
#ifndef LEOJOY_AXIS_Rz
#define LEOJOY_AXIS_Rz 1
#endif
#ifndef LEOJOY_AXIS_Slider
#define LEOJOY_AXIS_Slider 1
#endif
#ifndef LEOJOY_AXIS_Dial
#define LEOJOY_AXIS_Dial 0
#endif
#ifndef LEOJOY_AXIS_Wheel
#define LEOJOY_AXIS_Wheel 0
#endif
#ifndef LEOJOY_AXIS_Vx
#define LEOJOY_AXIS_Vx 0
#endif
#ifndef LEOJOY_AXIS_Vy
#define LEOJOY_AXIS_Vy 0
#endif
#ifndef LEOJOY_AXIS_Vz
#define LEOJOY_AXIS_Vz 0
#endif

// resolution of all the axes, in Bit. Max is 15 bit (values from 0-32767) default is 10 (values 0-1023)
// anything beyond 10 bit is probably too fine anyways
#ifndef LEOJOY_AXIS_BITS
#define LEOJOY_AXIS_BITS 0x0a
#endif

//count the number of axis
#define LEOJOY_NUMAXIS (LEOJOY_AXIS_X+LEOJOY_AXIS_Y+LEOJOY_AXIS_Z+LEOJOY_AXIS_Rx+LEOJOY_AXIS_Ry+LEOJOY_AXIS_Rz+LEOJOY_AXIS_Slider+LEOJOY_AXIS_Dial+LEOJOY_AXIS_Wheel+LEOJOY_AXIS_Vx+LEOJOY_AXIS_Vy+LEOJOY_AXIS_Vz)

// Windows seems to seee 32 Buttons max
#ifndef LEOJOY_NUMBUTTONS
#define LEOJOY_NUMBUTTONS 8
#endif

// up to 8 hats are defined in the Descriptor, but windows only seems to see 4
#ifndef LEOJOY_NUMHATS
#define LEOJOY_NUMHATS 1
#endif

// Buttons and hats do not need to be byte-aligned, but it might help when debugging
//#define LEOJOY_AXIS_ALIGN (8-((LEOJOY_NUMAXIS*LEOJOY_AXIS_BITS) & 7))
//#define LEOJOY_BUTTONS_ALIGN (8-((LEOJOY_NUMAXIS*LEOJOY_AXIS_BITS+LEOJOY_AXIS_ALIGN+LEOJOY_NUMBUTTONS) & 7))
#define LEOJOY_AXIS_ALIGN 0
#define LEOJOY_BUTTONS_ALIGN 0

// calculate the number of bits in the report
#define LEOJOY_REPORT_BITS (LEOJOY_NUMAXIS*LEOJOY_AXIS_BITS+LEOJOY_AXIS_ALIGN+LEOJOY_NUMBUTTONS+LEOJOY_BUTTONS_ALIGN+LEOJOY_NUMHATS*4)

class LeoJoystick_
{
private:
  BitArrayTemplate<LEOJOY_REPORT_BITS> _state;
  void send();
  
public:
  LeoJoystick_(void);
  void begin(void);
  void end(void);
  
  /**
    * Press a button. 
	* @param button the button number to press.
	*/
  void press(uint8_t button);

  /**
    * Release (unpress) a button. 
	* @param button the button number to release.
	*/
  void release(uint8_t button);
  
  /**
    * Check wheter a button is pressed.
	* @param button the button number to check.
	* @return true if pressed.
	*/
  bool isPressed(uint8_t button);
  
  /** 
   * Set the Axis to a specific value.
   *
   * @param axis The numer of the axis. Axes are in the order defined in the HID Descriptor, but only the ones defined do count.
   *             X would usually be 0, Y be 1, etc.  
   * @param value The value to set the axis to. Will be truncated to LEOJOY_AXIS_BITS resolution.
   */
  void axis(uint8_t axis, uint16_t value);

  /**
   * Set the Hat direction.
   * @param hat The Hat number to set. 0 < hat < LEOJOY_NUMHATS
   * @param value The direction to look. In 45 Degree steps. 1 is top, 3 is right, 5 is bottom, 7 is left. 0 is center. 
   */
  void hat(uint8_t hat, uint8_t value);
  
#ifdef LEOJOY_DEBUG  
  /**
   * Dump the current state to Serial
   */
  void dumpstate();
#endif
  
};

// *****************************************************************************************
// Implelemtation in Header File because the generated object code would not use the defines 
// from the sketch otherwise (the cpp file is compiled separately and would use the default defines)
// *****************************************************************************************

// calculate the maximum value possible with LEOJOY_AXIS_BITS bits [ (1<<LEOJOY_AXIS_BITS)-1 ]
// and split it into low and high byte
#define LEOJOY_AXIS_MAX_LOWBYTE (((1l<<LEOJOY_AXIS_BITS)-1)&0xFF)
#define LEOJOY_AXIS_MAX_HIGHBYTE ((((1l<<LEOJOY_AXIS_BITS)-1)>>8)& 0xFF)


// The glorious Joystick descriptor
// Check out www.usb.org/developers/hidpage/ for more than you'll ever need to know about USB HID
// HID descriptor initially created using the HID descriptor tool from www.usb.org/developers/hidpage/dt2_4.zip (win32)
static const uint8_t _hidReportDescriptor[] PROGMEM = {
  
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x04,                    // USAGE (Joystick)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x09, 0x01,                    //   USAGE (Pointer)
    0xa1, 0x00,                    //   COLLECTION (Physical)
    0x85, 0x03,                    //     REPORT_ID (1) -- used when sending the report

#if LEOJOY_NUMAXIS >0
	// some LEOJOY_AXIS_BITS-bit axis
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
#if LEOJOY_AXIS_X == 1
    0x09, 0x30,                    //     USAGE (X)
#endif	
#if LEOJOY_AXIS_Y == 1
    0x09, 0x31,                    //     USAGE (Y)
#endif	
#if LEOJOY_AXIS_Z == 1
    0x09, 0x32,                    //     USAGE (Z)
#endif	
#if LEOJOY_AXIS_Rx == 1	
    0x09, 0x33,                    //     USAGE (Rx)
#endif	
#if LEOJOY_AXIS_Ry == 1	
    0x09, 0x34,                    //     USAGE (Ry)
#endif	
#if LEOJOY_AXIS_Rz == 1	
    0x09, 0x35,                    //     USAGE (Rz)
#endif	
#if LEOJOY_AXIS_Slider == 1
    0x09, 0x36,                    //     USAGE (Slider)
#endif	
#if LEOJOY_AXIS_Dial == 1
    0x09, 0x37,                    //     USAGE (Dial)
#endif	
#if LEOJOY_AXIS_Wheel == 1
    0x09, 0x38,                    //     USAGE (Wheel)
#endif	
#if LEOJOY_AXIS_Vx == 1
    0x09, 0x40,                    //     USAGE (Vx)
#endif	
#if LEOJOY_AXIS_Vy == 1
    0x09, 0x41,                    //     USAGE (Vy)
#endif	
#if LEOJOY_AXIS_Vz == 1
    0x09, 0x42,                    //     USAGE (Vz)
#endif	
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x26, LEOJOY_AXIS_MAX_LOWBYTE, LEOJOY_AXIS_MAX_HIGHBYTE, // LOGICAL_MAXIMUM (1023) 
    0x75, LEOJOY_AXIS_BITS,      //     REPORT_SIZE (10)
    0x95, LEOJOY_NUMAXIS,        //     REPORT_COUNT (LEOJOY_NUMAXIS)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
#endif


#if LEOJOY_AXIS_ALIGN > 0
// alignment to byte boundary
    0x75, LEOJOY_AXIS_ALIGN,     // 
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x81, 0x01,                    //     INPUT (Cnst,Ary,Abs)
#endif
	
#if LEOJOY_NUMBUTTONS >0
// 40 Buttons	
    0x05, 0x09,                    //     USAGE_PAGE (Button)
    0x19, 0x01,                    //     USAGE_MINIMUM (Button 1) ( USAGE_MIN and _MAX replace the need for defining USAGE tags)
    0x29, LEOJOY_NUMBUTTONS,       //     USAGE_MAXIMUM (Button X) ( so this is the same as USAGE 1, USAGE 2, USAGE 3,... USAGE X)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //     REPORT_SIZE (1)
    0x95, LEOJOY_NUMBUTTONS,       //     REPORT_COUNT (X)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
#endif

#if LEOJOY_BUTTONS_ALIGN > 0
// alignment to byte boundary
    0x75, LEOJOY_BUTTONS_ALIGN,     // 
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x81, 0x01,                    //     INPUT (Cnst,Ary,Abs)
#endif

#if LEOJOY_NUMHATS > 0
// Up to eight hat switches (4 bit)
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    0x09, 0x39,                    //     USAGE (Hat switch)
    // turns out that according to USB spec the last specified usage will be used for further inputs
	// so no need to add other USAGE (Hat switch)es
    0x15, 0x01,                    //     LOGICAL_MINIMUM (1)
    0x25, 0x08,                    //     LOGICAL_MAXIMUM (8)
    0x35, 0x00,                    //     PHYSICAL_MINIMUM (0)
    0x46, 0x3b, 0x01,              //     PHYSICAL_MAXIMUM (315)
    0x65, 0x14,                    //     UNIT (Eng Rot:Angular Pos)
    0x75, 0x04,                    //     REPORT_SIZE (4)
    0x95, LEOJOY_NUMHATS,          //     REPORT_COUNT (X)
    0x81, 0x42,                    //     INPUT (Data,Var,Abs,Null)
#endif
	
#if (LEOJOY_REPORT_BITS& 7) > 0
// alignment to byte boundary
    0x75, 8-(LEOJOY_REPORT_BITS & 7),  // REPORT_SIZE ()  - Note that &7 is the same as %8
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x81, 0x01,                    //     INPUT (Cnst,Ary,Abs)
#endif
    0xc0,                          //   END_COLLECTION
    0xc0                           // END_COLLECTION

};

//================================================================================
//================================================================================
//	Debugging
#ifdef LEOJOY_DEBUG  
void LeoJoystick_::dumpstate()
{
	char cbuf[5];
	
	for (int i=0; i < _state.getBufferSize(); i++) {
		sprintf(cbuf, "%02x", _state.getBuffer()[i]);
		Serial.print(cbuf);
    }
	Serial.print(" == ");
	for (int i =0; i < LEOJOY_REPORT_BITS; i++) {
		if (i % 8 ==0 ) Serial.print(" ");
		Serial.print(_state.getBit(8*(i>>3) + (7-(i&7)) ));
	}

	Serial.println();
}
#endif

//================================================================================
//================================================================================
//	Joystick

LeoJoystick_::LeoJoystick_(void)
{
#ifdef LEOJOY_MUTE_PIN
    pinMode(LEOJOY_MUTE_PIN, INPUT_PULLUP);
#endif	
	static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
	HID().AppendDescriptor(&node);
}

void LeoJoystick_::begin(void) 
{
#ifdef LEOJOY_DEBUG	
	char cbuf[10];
	Serial.print("Joystick with ");
	Serial.print(LEOJOY_NUMAXIS);
	Serial.print(" axes à ");
	Serial.print(LEOJOY_AXIS_BITS);
	Serial.print(" bit, ");
	Serial.print(LEOJOY_NUMBUTTONS);
	Serial.print(" buttons, ");
	Serial.print(LEOJOY_NUMHATS);
	Serial.print(" hats. Report size is ");
	Serial.print(LEOJOY_REPORT_BITS);
	Serial.print(" bits in ");
	Serial.print(_state.getBufferSize());
	Serial.println(" bytes");
	
	Serial.print("Descriptor is ");
	Serial.print(sizeof(_hidReportDescriptor));
	Serial.print(" bytes:");
	for (int i=0; i < sizeof(_hidReportDescriptor); i++) {
		uint8_t byte = pgm_read_byte_near(_hidReportDescriptor+i);
		sprintf(cbuf,"%02x ", byte);
		Serial.print(cbuf);
    }
	Serial.println();
#endif	
}

void LeoJoystick_::end(void) 
{
}

void LeoJoystick_::press(uint8_t b) 
{
	const uint8_t offset = LEOJOY_NUMAXIS*LEOJOY_AXIS_BITS+
	                       LEOJOY_AXIS_ALIGN;
	_state.set(offset+b,1);
	send();
}

void LeoJoystick_::release(uint8_t b)
{
	const uint8_t offset = LEOJOY_NUMAXIS*LEOJOY_AXIS_BITS+
	                       LEOJOY_AXIS_ALIGN;
	_state.set(offset+b,0);
	send();
}

bool LeoJoystick_::isPressed(uint8_t b)
{
	const uint8_t offset = LEOJOY_NUMAXIS*LEOJOY_AXIS_BITS+
	                       LEOJOY_AXIS_ALIGN;
	return _state.getBit(offset+b);
}

void LeoJoystick_::axis(uint8_t axis, uint16_t value)
{
	_state.set(LEOJOY_AXIS_BITS*axis,LEOJOY_AXIS_BITS,value);
	send();
}

void LeoJoystick_::hat(uint8_t hat, uint8_t value)
{
	const uint8_t offset = LEOJOY_NUMAXIS*LEOJOY_AXIS_BITS+
	                       LEOJOY_AXIS_ALIGN+
						   LEOJOY_NUMBUTTONS+
						   LEOJOY_BUTTONS_ALIGN;
	_state.set(offset+hat*4,4,value);
	send();
}


void LeoJoystick_::send()
{
#ifdef LEOJOY_MUTE_PIN
    // allow for muting of joystick USB functions to allow upload of new sketch
	// otherwise it might get messy, and you might have to resort to using an ISP (I did...)
	if (digitalRead(LEOJOY_MUTE_PIN)) {
#endif
	#ifdef LEOJOY_DEBUG	
		digitalWrite(13, !digitalRead(13));
		dumpstate();
	#endif	
		HID().SendReport(3,_state.getBuffer(),_state.getBufferSize());
#ifdef LEOJOY_MUTE_PIN
	}
#endif
}

// define the Joystick singleton
LeoJoystick_ LeoJoystick;


#endif
#endif