# Attiny85 DS18B20 Thermostat
Thermostat with DS18B20 temperature sensor and Attiny85 microcontroller.
- Watchdog activated, 2s.
- Average calculation between readings, configurable.
- Displaying the temperature value does not block the temperature reading.
- In case of communication failure with the sensor, an attempt is made to reestablish the communication.
- Configurable hysteresis:
- - Output is activated when the temperature value is above the pre-established value in the variable temp_max_setpoint.
- - Output is deactivated when the temperature value is below the pre-established value in the variable temp_min_setpoint.
- Alternating audible warning output for when the output is activated (for active Buzzer/Beeper).
- In the event of a communication failure, after 5 attempts to reestablish the communication, the output is automatically activated.
- Output update time is dependent on the number of readings for average readings and the reading precision:
- - For 9-bit precision and 5 average readings, the time is approximately 0.5s.

Temperature value display by flashing LED:
- Zero value on the left is ignored, is not displayed.
- The zero value of the digit has a longer duration than for the digits between 1 and 9, and a shorter time than for the negative value signal
- Digit values between 1 and 9 are displayed with short pulses.
- Negative sign is displayed with long time.
- The duration of each pulse is dependent on the reading time of the sensor, if the reading precision is changed to more bits, the time increases.


Dependencies:

Library:
https://github.com/milesburton/Arduino-Temperature-Control-Library

Board:
https://github.com/SpenceKonde/ATTinyCore

- "After changing the clock source, BOD settings, or whether to save EEPROM on chip erase), you must do "Burn Bootloader" with an ISP programmer"

- When the chip is blank or the chip has a pre-installed Bootloader, you must do "Burn Bootloader" as well.

- Bootloader is not used for this code, but it is necessary to burn the bootloader to reconfigure the fuses.

![alt text](https://github.com/rtek1000/Attiny85_DS18B20_thermostat/blob/main/Burn%20Bootloader.png?raw=true)


Compiled with Arduino IDE 1.8.13:
Settings:
![alt text](https://github.com/rtek1000/Attiny85_DS18B20_thermostat/blob/main/IDE%20settings.png?raw=true)


Tested programming method:
- Arduino as ISP: https://www.arduino.cc/en/Tutorial/BuiltInExamples/ArduinoISP

- USBasp - USB programmer for Atmel AVR controllers: https://www.fischl.de/usbasp/

![alt text](https://github.com/rtek1000/Attiny85_DS18B20_thermostat/blob/main/Upload_Prog.png?raw=true)

Schematic for Attiny85 DS18B20 thermostat:
![alt text](https://github.com/rtek1000/Attiny85_DS18B20_thermostat/blob/main/Attiny_DS18B20_schem.png?raw=true)


License
This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
