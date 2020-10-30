# Attiny85_DS18B20_thetmostat
Thermostat with DS18B20 temperature sensor and Attiny85 microcontroller.

Temperature value display by flashing LED:
- Zero value on the left is ignored, is not displayed.
- Digit value zero has an average duration.
- Digit values between 1 and 9 are displayed with short pulses.
- Negative sign is displayed with long time.


Dependencies:

Library:
https://github.com/milesburton/Arduino-Temperature-Control-Library

Board:
https://github.com/SpenceKonde/ATTinyCore

(Compiled with Arduino IDE 1.8.13)

Schematic:
![alt text](https://github.com/rtek1000/Attiny85_DS18B20_thetmostat/blob/main/Attiny_DS18B20_schem.png?raw=true)

License
This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
