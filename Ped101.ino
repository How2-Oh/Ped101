/* Pedometer101
 *  
 *  Based on the following libraries:
 *  Serial 7-Segment
   SparkFun Electronics
   date: November 27, 2012
   license: This code is public domain.

  OneWire DS18B20 Thermometer

* Copyright (c) 2015 Intel Corporation.  All rights reserved.
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  * This library is distributed in the hope that it will be useful,
  * 
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  * 
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
  *
 * This sketch example demonstrates how the BMI160 accelerometer on the
 * Intel(R) Curie(TM) module can be used as a Step Counter (pedometer)

This example code shows how you could use software serial Arduino 
library to interface with a Serial 7-Segment Display.

Circuit:
Arduino -------------- Serial 7-Segment
3.3V ------------------ VCC 
GND ------------------- GND 
8 --------------------- RX

DS18B20:
Arduino --------------- DS18B20
5V -------------------- VDD (pin #3)
10 -------------------- DQ (pin #2)
GND ------------------- GND (pin #1)
5V -- 4.7K resistor --- DQ (pin #2)
*/

#include <SoftwareSerial.h>
#include "CurieImu.h"
#include <OneWire.h>

// Pedometer step count variable
int lastStepCount = 0;

// These are the Arduino pins required to create a software seiral 
// instance. Software serial is used with the 4-Segment LED display.
// We'll actually only use the TX pin.
const int softwareTx = 8;
const int softwareRx = 7;

SoftwareSerial s7s(softwareRx, softwareTx);

unsigned int counter = 0; // This variable will count up to 65k
char tempString[10]; // Will be used with sprintf to create strings

int i = 0; // An increment timing variable for use during display

OneWire  ds(10);  // Thermometer is on pin 10 (a 4.7K resistor is necessary)

void setup()
{
  // Must begin s7s software serial at the correct baud rate. 
  // The default of the s7s is 9600.
  s7s.begin(9600);

  // Set brightness value at the sketch beginning 
  setBrightness(0); // Highest brightness 

  setDecimals(0b1111111); // Turn on all decimals, colon, apos

  // Countdown to start
  for (i=0; i<500; i++) {
    s7s.print("3333");
  }
  for (i=0; i<500; i++) {
    s7s.print("2222");
  }
  for (i=0; i<500; i++) {
    s7s.print("1111");
  }

  clearDisplay();
  setDecimals(0b0000000); // Turn off all decimals, colon, apos

  // Setup the A101
  CurieImu.initialize(); // initialise the IMU
  CurieImu.setStepDetectionMode(BMI160_STEP_MODE_NORMAL); // set step detection mode to normal
  CurieImu.setStepCountEnabled(true); // enable step count

}

void loop() 
{
  // Check the step count
  updateStepCount();

  // Use the step count to find the distance
  updateDistance();

  // Display the time
  updateTime();

  // Display the temperature
  updateTemp();
  
}

// Send the clear display command (0x76)
// This will clear 
void clearDisplay() 
{
s7s.write(0x76);
}

// Sets the LED display brightness. Should receive byte with the value
// to set the brightness to
// dimmest------------->brightest 
// 255--------127--------0 
void setBrightness(byte value)
{
s7s.write(0x7A); // Set brightness command byte
s7s.write(value); // brightness data byte 
}

// Sends Content to the display.
void sendContent(byte decimals)
{
//s7s.write();
//s7s.write(decimals);
}

// Turn on any, none, or all of the decimals.
// The six lowest bits in the decimals parameter sets a decimal 
// (or colon, or apostrophe) on or off. A 1 indicates on, 0 off.
// The colon uses two bits. 
// [MSB] (X)(X)(Apos)(Colon)(Colon)(Digit 4)(Digit 3)(Digit2)(Digit1) 
void setDecimals(byte decimals)
{
s7s.write(0x77);
s7s.write(decimals);
}

// Determine step count and display total
void updateStepCount()
{
  int stepCount = CurieImu.getStepCount(); // set stepCount to read stepCount from function
  if (stepCount != lastStepCount) { // if stepCount has changed
    setDecimals(0b0000001); // Sets digit 1 decimal point on - this indicates the Step count on the display

    // Use sprintf to create a string to send to the s7s. 
    // The %4d option creates a 4-digit integer. 
    sprintf(tempString, "%4d", stepCount);

    // Output the tempString to the S7S 
    for (i=0; i<500; i++) {
      s7s.print(tempString);
    }

  lastStepCount = stepCount;

  setDecimals(0b0000000); // Turn off all decimals, colon, apos

   }
}

// Determine the distance travelled and display total
void updateDistance()
{

  int distance = 0;  // Total distance travelled
  int distanceTenths = 0;  // Tenths of a mile travelled

  int stride = 35;  // Enter your stride length in inches
  char stringTenths[4]; // Used for making printable tenths of a mile
  
/*
  Sliding Stride Scale (based on height)
  0-2 steps = height/5
  2-3 steps = height/4
  3-4 steps = height/3
  4-5 steps = height/2
  5-6 steps = height/1.2
  6-8 steps = height
  >=8 steps = height * 1.2
  lastStepCount from CurieIMU = steps
  Distance = number of steps * distance per step (stride)
 */

    setDecimals(0b0000010); // Sets digit 2 decimal point on - this indicates the Distance on the display

    distance = lastStepCount * stride; // This is distance measured in inches
    distanceTenths = (((distance) / 12) % 5280 / 100);

    // Use sprintf to create a string to send to the s7s. 
    // The %4d option creates a 4-digit integer. 
    sprintf(stringTenths, "%4d", distanceTenths);

    // Output the tempString to the S7S 
    for (i=0; i<500; i++) {
      s7s.print(stringTenths);
    }
    setDecimals(0b0000000); // Turn off all decimals, colon, apos
}

// Determine pace and display total
void updateTime()
{
  unsigned long totalSeconds = millis();

  totalSeconds = ((totalSeconds / 1000) / 60);

    setDecimals(0b0110000); // Sets colon points on - this indicates the time on the display

    // Use sprintf to create a string to send to the s7s. 
    // The %4lu option creates a 4-digit unsigned long. 
    sprintf(tempString, "%4lu", totalSeconds);

    // Output the tempString to the S7S 
    for (i=0; i<500; i++) {
      s7s.print(tempString);
    }
    setDecimals(0b0000000); // Turn off all decimals, colon, apos

}

// Grab the temperature from the sensor and display the degrees in F
void updateTemp()
{
  byte j;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;
  
  if ( !ds.search(addr)) {
    ds.reset_search();
    delay(250);
    return;
  }
  
  for( j = 0; j < 8; j++) {
  }

  if (OneWire::crc8(addr, 7) != addr[7]) {
      return;
  }
 
  // the first ROM byte indicates which chip
  switch (addr[0]) {
    case 0x10:
      //Serial.println("  Chip = DS18S20");  // or old DS1820
      type_s = 1;
      break;
    case 0x28:
      //Serial.println("  Chip = DS18B20");
      type_s = 0;
      break;
    case 0x22:
      //Serial.println("  Chip = DS1822");
      type_s = 0;
      break;
    default:
      //Serial.println("Device is not a DS18x20 family device.");
      return;
  } 

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
  
  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  for ( j = 0; j < 9; j++) {           // we need 9 bytes
    data[j] = ds.read();
  }

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }

  celsius = (float)raw / 16.0;
  fahrenheit = celsius * 1.8 + 32.0;

    setDecimals(0b1000000); // Sets apos point on - this indicates the temp on the display
    // Use sprintf to create a string to send to the s7s. 
    // The %3.1f option creates a 1-decimal float. 
    sprintf(tempString, "%3.1f", fahrenheit);

    // Output the tempString to the S7S 
    for (i=0; i<500; i++) {
      s7s.print(tempString);
    }
    setDecimals(0b0000000); // Turn off all decimals, colon, apos
}
