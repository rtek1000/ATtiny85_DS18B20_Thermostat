### Binary files:

Only 1 sensor:

- Attiny85_DS18B20_thermostat_v1.ino.hex


For 2 sensors:

- Attiny85_DS18B20_thermostat_v2.ino.hex


### Atiny85 fuses:

- Install AvrDude on [Ubuntu](https://medium.com/@ppatil/avr-programing-using-avrdude-in-ubuntu-93734c26ad19)

- Reading fuses:

$ sudo avrdude -c usbasp -p t85

>avrdude: AVR device initialized and ready to accept instructions
>
>Reading | ################################################## | 100% 0.00s
>
>avrdude: Device signature = 0x1e930b (probably t85)
>
>avrdude: safemode: Fuses OK (E:FF, H:D7, L:F1)
>
>avrdude done.  Thank you.

- If it's necessary, burn it.

- - Avoid burning unnecessarily, if programmed wrong it can block the microcontroller.

- - If you enable external clock, you must supply external clock.

- - Fuses are not bundled with binary files.

- Burning fuses (No Bootloader; 16MHz PLL; Internal clock):

$ sudo avrdude -p t85 -c usbasp -U efuse:w:0xFF:m -U hfuse:w:0xD7:m -U lfuse:w:0xF1:m

> avrdude: AVR device initialized and ready to accept instructions
> 
> Reading | ################################################## | 100% 0.00s
> 
> avrdude: Device signature = 0x1e930b (probably t85)
> avrdude: reading input file "0xFF"
> avrdude: writing efuse (1 bytes):
> 
> Writing | ################################################## | 100% 0.00s
> 
> avrdude: 1 bytes of efuse written
> avrdude: verifying efuse memory against 0xFF:
> avrdude: load data efuse data from input file 0xFF:
> avrdude: input file 0xFF contains 1 bytes
> avrdude: reading on-chip efuse data:
> 
> Reading | ################################################## | 100% 0.00s
> 
> avrdude: verifying ...
> avrdude: 1 bytes of efuse verified
> avrdude: reading input file "0xD7"
> avrdude: writing hfuse (1 bytes):
> 
> Writing | ################################################## | 100% 0.00s
> 
> avrdude: 1 bytes of hfuse written
> avrdude: verifying hfuse memory against 0xD7:
> avrdude: load data hfuse data from input file 0xD7:
> avrdude: input file 0xD7 contains 1 bytes
> avrdude: reading on-chip hfuse data:
> 
> Reading | ################################################## | 100% 0.00s
> 
> avrdude: verifying ...
> avrdude: 1 bytes of hfuse verified
> avrdude: reading input file "0xF1"
> avrdude: writing lfuse (1 bytes):
> 
> Writing | ################################################## | 100% 0.01s
> 
> avrdude: 1 bytes of lfuse written
> avrdude: verifying lfuse memory against 0xF1:
> avrdude: load data lfuse data from input file 0xF1:
> avrdude: input file 0xF1 contains 1 bytes
> avrdude: reading on-chip lfuse data:
> 
> Reading | ################################################## | 100% 0.00s
> 
> avrdude: verifying ...
> avrdude: 1 bytes of lfuse verified
> 
> avrdude: safemode: Fuses OK (E:FF, H:D7, L:F1)
> 
> avrdude done.  Thank you.



