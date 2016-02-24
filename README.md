# Ped101
A practice pedometer built on the Arduino 101.

What happens when you combine an Intel 32-bit Quark microcontroller with an open-source Arduino board? You get the Arduino 101. Known as the Intel Curie Compute Module, the Quark microcontroller is supported by a Bluetooth Low Energy radio and a dual combo 6-axis accelerometer and gyroscope sensor package. This combo sensor is the Bosch BMI160. Likewise, these dual sensors can be used in concert as an Inertial Monitoring Device (IMU) for measuring orientation and angular veloctiy.

See Intel for more information:
<a href=intel.com/content/www/us/en/do-it-yourself/arduino-101.html>intel.com/content/www/us/en/do-it-yourself/arduino-101.html</a>

A real-world application for this IMU technology is in a pedometer. If you're a runner or a racewalker, then you will log a lot of practice miles before each competitive event. While your training regimen can be broken down into many different phases (e.g., stamina, endurance, heart rate, etc.), the most basic form of practice will be just logging easy miles each day. In this form of practice, all you care about is distance and time. Thankfully, both of these measurements can be recorded easily with the Arduino 101's IMU.

Since we will be using the Curie Compute Module for counting steps, the IMU will only be using the accelerometer portion of the Bosch sensor. There is a special Arduino library, CurieImu that makes the initialization and use of the IMU very easy.

This project is called Ped101.

In addition to the Arduino 101 IMU, Ped101 will also display the current ambient temperature. This data is obtained from a Dallas Semiconductor DS18B20 one-wire temperature sensor. A four-digit, serial 7-segment LED display is used for conveying these data to the runner. There are four updated data displays: steps, distance (in hundredths of a mile), time (in seconds), and temperature (in degrees Fahrenheit). Finally, a 9V battery is used for powering Ped101.

##Parts
<ul>
<li>Arduino 101 (SparkFun Electronics DEV-13787)
<li>DS18B20 One-Wire temperature sensor (SFE SEN-00245)
<li>Serial four-digit, 7-segment LED display (SFE COM-11443)
<li>3 - Male-Male jumper wires (SFE PRT-12795)
<li>4.7K resistor (Mouser Yageo; mouser.com)
<li>9V battery & barrel connector (SFE PRT-10512 or, littleBits power connector)
</ul>
Three 3D printable STL files:
<ul>
<li>Box
<li>Lid
<li>Straps & washers (for mounting Ped101 on hand grip or belt)
<li>4x40 mounting hardware
</ul>
<b>NOTE:</b> The serial LED display used for Ped101 has been updated and the new version might not fit inside the 3D printed lid.

##Step-by-Step

1. Mount the serial display and the DS18B20 sensor to the Ped101 lid. Testors Cement for Plastic Models No. 3512 works.

2. Snip the jumper wires in half and solder three wires to the serial connections on the LED display.

3. Solder the 4.7K resistor between the DQ (#2) and VDD (#3) legs of the DS18B20.

4. Solder the remaining three wires to the three legs of the DS18B20.

5. Plug the male ends of each jumper wire into the Arduino 101 like so:
<ul>
<li>Arduino 3.3V -> Serial 7-Segment VCC
<li>Arduino GND -> Serial 7-Segment GND 
<li>Arduino 8 -> Serial 7-Segment RX

<li>Arduino 5V -> DS18B20 VDD (pin #3)
<Li>Arduino 10 -> DS18B20 DQ (pin #2)
<li>Arduino GND -> DS18B20 GND (pin #1)
</ul>
6. Enter your stride length in the Arduino code at the appropriate variable. This length is measured in inches.

7. Attach the Arduino 101 to your computer and upload the Ped101 code to the device.

8. Drop the Arduino 101 into the 3D printed box and secure the washers and straps on the backside of the box and through the Arduino with the 4-40 mounting hardware.

9. Mount the lid on the box. Again, Testors cement works well for this step.

10. Plug the battery into the Arduino 101 and enjoy your run.
