# ATtiny85 DS18B20 Thermostat
###### Thermostat with DS18B20 temperature sensor and ATtiny85 microcontroller:
- Watchdog activated, 2s.
- Average calculation between readings.
- Displaying the temperature value does not block the temperature reading.
- In case of communication failure with the sensor, attempts are made to reestablish the communication.
- Configurable hysteresis:
- - Output is activated when the temperature value is above the pre-established value (variable "temp_max_setpoint").
- - Output is deactivated when the temperature value is below the pre-established value (variable "temp_min_setpoint").
- Alternating warning output for when the output is activated (for active Buzzer/Beeper).
- In the event of a communication failure, after 5 attempts to reestablish the communication, the output is automatically activated.
- Output update time is dependent on the number of readings for average readings and the reading precision:
- - For 9-bit precision and 5 average readings, the time is approximately 0.5s.

###### Temperature value display by flashing LED:
- Zero value on the left is ignored, is not displayed.
- The zero value of the digit has a longer duration than for the digits between 1 and 9, and a shorter time than for the negative value signal
- Digit values between 1 and 9 are displayed with short pulses.
- Negative sign is displayed with long time.
- The duration of each pulse is dependent on the reading time of the sensor, if the reading precision is changed to more bits, the time increases.

###### Note:
- In version 2, the terminals of the 2 sensors are connected in parallel.
- R5 has been modified from 4.7k to 2.2k to slightly increase the brightness of the green color LED.
- Pay attention to the quality of the 1nF capacitor.

#### Dependencies:

###### Library:
https://github.com/milesburton/Arduino-Temperature-Control-Library

###### Board:
https://github.com/SpenceKonde/ATTinyCore

###### Notes:
- "After changing the clock source, BOD settings, or whether to save EEPROM on chip erase), you must do "Burn Bootloader" with an ISP programmer" (From: ATTinyCore)

- When the chip is blank or the chip has a pre-installed Bootloader, you must do "Burn Bootloader" as well.

- Due to the fake DS18B20 issue, the test sketch was created to help identify bad sensors: [Time_Checker (9bits)](https://github.com/rtek1000/ATtiny85_DS18B20_Thermostat/blob/main/Software/Attiny85_DS18B20_9bits_Time_Checker.ino)
- - There are sensors being sold that do not even come close to the datasheet specifications:
![img](https://raw.githubusercontent.com/rtek1000/ATtiny85_DS18B20_Thermostat/main/Hardware/DS18B20%202034C4.png)

- [(Analog.com) Guidelines for Reliable Long Line 1-Wire Networks](https://www.analog.com/en/technical-articles/guidelines-for-reliable-long-line-1wire-networks.html)

- DS18B20 Calibration? See here: [DS18B20 1-Wire calibration with Arduino: Finally nailed it!](https://thecavepearlproject.org/2016/03/05/ds18b20-calibration-we-finally-nailed-it/).

###### Arduino IDE:
https://www.arduino.cc/en/software/
- Note: Compiled with Arduino IDE 1.8.13

#### Settings:
Arduino IDE - Burn Bootloader:

(Bootloader is not used for this code, but it is necessary to burn the bootloader to reconfigure the fuses)

![image](https://raw.githubusercontent.com/rtek1000/ATtiny85_DS18B20_thermostat/main/Settings/Burn%20Bootloader.png)

IDE settings for compilation and upload:

![image](https://raw.githubusercontent.com/rtek1000/ATtiny85_DS18B20_thermostat/main/Settings/IDE%20settings.png)

Tested programming method:
- Arduino as ISP: https://www.arduino.cc/en/Tutorial/BuiltInExamples/ArduinoISP

- USBasp - USB programmer for Atmel AVR controllers: https://www.fischl.de/usbasp/

Sketch upload via programmer:

![image](https://raw.githubusercontent.com/rtek1000/ATtiny85_DS18B20_thermostat/main/Settings/Upload_Prog.png)

Schematic for ATtiny85 DS18B20 Thermostat (For version 1, only 1 sensor):

![image](https://raw.githubusercontent.com/rtek1000/ATtiny85_DS18B20_thermostat/main/Hardware/Attiny_DS18B20_schematic_v2.png)


### avrdude error: 
- avrdude: error: could not find USB device with vid=0x16c0 pid=0x5dc vendor=’www.fischl.de’ product=’USBasp’ 

On some system. A tty device is created when USBasp is connected. In my case /dev/ttyS0. However when Port is set to /dev/ttyS0 in Arduino IDE. You will see the following errors:

>avrdude: Warning: cannot open USB device: Permission denied
>avrdude: error: could not find USB device with vid=0x16c0 pid=0x5dc vendor=’www.fischl.de’ product=’USBasp’

- To Fix Permission denied error. As root edit “/etc/udev/rules.d/99-USBasp.rules” with the following:

`$ sudo nano /etc/udev/rules.d/99-USBasp.rules`

>SUBSYSTEM=="usb", ATTRS{idVendor}=="16c0", ATTRS{idProduct}=="05dc", GROUP="dialout"

- Add self to dialout user group.

`sudo usermod -aG dialout $USER`

- Then logoff and login again.
- - ---> Unplug USBasp device <--- Then restart udev.

`sudo /etc/init.d/udev restart`

- Source: [Geting USBasp v2.0 to work with Linuxmint](https://klamfx.wordpress.com/tag/avrdude-error-could-not-find-usb-device-with-vid0x16c0-pid0x5dc-vendorwww-fischl-de-productusbasp/)


#### Software License:
This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA


#### Hardware Licence:
- Released under CERN OHL 1.2: https://ohwr.org/cernohl
