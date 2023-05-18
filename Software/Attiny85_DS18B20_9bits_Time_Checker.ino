/*
   ATtiny85 DS18B20 9 bit time checker
   - It only shows the times in milliseconds of each sensor, to help the developer to find fake sensors.
   - Automatically detects multiple sensors connected in parallel.

   References:
   https://github.com/milesburton/Arduino-Temperature-Control-Library/blob/master/examples/Timing/Timing.ino
   https://github.com/milesburton/Arduino-Temperature-Control-Library/blob/master/examples/Multiple/Multiple.ino
   https://blog.jloh02.dev/projects/connecting-attiny85-serial-monitor/

   by RTEK1000 - May/17/2023

*/

#include <OneWire.h>
#include <DallasTemperature.h>

// Uncomment this line below to generate serial
// data output on the pin selected for TX (the RX pin is not used).
//#define serial_debug_on 1

#ifdef serial_debug_on
#include <SoftwareSerial.h>
#endif

// Pin setup

#ifdef serial_debug_on

// (Using as an input to not trigger the high temperature output)
#define rx_pin 1 // Pin 6

// (Serial monitor output, 9600 bauds, TTL)
#define tx_pin 2 // Pin 7

// (Sensor power supply)
#define ONE_WIRE_PRW 3 // Pin 2

// (Sensor signal)
#define ONE_WIRE_BUS 4 // Pin 3

// (Pulse count indication)
#define LED_INFO_OUT  0 // Pin 5

SoftwareSerial softSerial(rx_pin, tx_pin);
#endif

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

uint32_t start, stop;

#define TEMPERATURE_PRECISION 9 // Lower resolution

int number_of_sensors = 0; // all sensor pins connected in parallel

// to hold device addresses
DeviceAddress addrThermometer;

unsigned int phase_cnt = 0;
unsigned int phase_cnt2 = 0;

float avgDuration_buff = 0.0;

int temp100 = 0;
int temp10 = 0;
int temp1 = 0;

uint32_t duration = 0;
float avgDuration = 0.0;

#define tCONV 93.75 // MAX Temperature Conversion Time for 9-bit resolution

void setup()
{
#ifdef serial_debug_on
  softSerial.begin(9600);
  softSerial.print("ATtiny85 DS18B20 9 bit time checker (MAX: ");
  softSerial.print(tCONV, 2);
  softSerial.println("ms)");
#endif

  pinMode(LED_INFO_OUT, OUTPUT);
  pinMode(ONE_WIRE_PRW, OUTPUT);
}

void loop()
{
  digitalWrite(ONE_WIRE_PRW, LOW);
  pinMode(ONE_WIRE_BUS, OUTPUT);

  delay(200);

  digitalWrite(ONE_WIRE_PRW, HIGH);
  pinMode(ONE_WIRE_BUS, INPUT);
  digitalWrite(ONE_WIRE_BUS, HIGH);

  delay(200);

  // Start up the library
  sensors.begin();

  number_of_sensors = sensors.getDeviceCount();

#ifdef serial_debug_on
  softSerial.print("Locating devices...");
  softSerial.print("Found ");
  softSerial.print(number_of_sensors, DEC);
  softSerial.println(" devices.");
#endif

  phase_cnt = 0;

  while (1) {
    if (LED_info(number_of_sensors) != 0) {
      break;
    }

    delay(100); // Interval for indication pulses
  }

  delay(5000); // Interval before starting to show times

  /* (This text is part of Multiple.ino):
    method 2: search()
    search() looks for the next device. Returns 1 if a new address has been
    returned. A zero might mean that the bus is shorted, there are no devices,
    or you have already retrieved all of them. It might be a good idea to
    check the CRC to make sure you didn't get garbage. The order is
    deterministic. You will always get the same devices in the same order
  */

  oneWire.reset_search();

  int sensor_count = 0;

  while (sensor_count < number_of_sensors) {
    if (!oneWire.search(addrThermometer)) {
#ifdef serial_debug_on
      softSerial.print("Unable to find address for Thermometer ");
      softSerial.println(sensor_count, DEC);
#endif
    } else {
#ifdef serial_debug_on
      softSerial.print("Device ");
      softSerial.print(sensor_count, DEC);
      softSerial.print(":\r\nAddress: ");
      printAddress(addrThermometer);
      softSerial.println();
#endif
      if (sensors.getResolution(addrThermometer) != TEMPERATURE_PRECISION) {
        sensors.setResolution(addrThermometer, TEMPERATURE_PRECISION);
      }

      if (sensors.getResolution(addrThermometer) != TEMPERATURE_PRECISION) {
        sensors.setResolution(addrThermometer, TEMPERATURE_PRECISION);
      }

      duration = run(20, addrThermometer);
      avgDuration = duration / 20.0;

#ifdef serial_debug_on
      softSerial.print("TIME: ");
      softSerial.print(avgDuration, 2);
      softSerial.print("ms ");

      if ((avgDuration < tCONV) && (avgDuration > (tCONV * 0.75))) {
        softSerial.println("(GOOD)");
      } else {
        softSerial.println("(NOT GOOD)");
      }
#endif

      avgDuration_buff = avgDuration;

      phase_cnt = 0;

      while (1) {
        if (LED_info(avgDuration_buff) != 0) {
          break;
        }

        delay(100); // Interval for indication pulses
      }
    }

    sensor_count++;
  }

#ifdef serial_debug_on
  softSerial.println("Final break, before restart");
#endif

  delay(10000); // Final break, before restart
}

uint32_t run(int runs, DeviceAddress sensor_addr)
{
  float t;

  start = millis();
  for (int i = 0; i < runs; i++)
  {
    sensors.requestTemperatures(); // Send the command to get temperatures
    t = sensors.getTempC(sensor_addr);
  }
  stop = millis();

  return stop - start;
}

int LED_info(int data_in) {
  if (phase_cnt == 0) {
    temp100 = data_in / 100;
    temp10 = (data_in - (temp100 * 100)) / 10;
    temp1 = data_in - (temp100 * 100) - (temp10 * 10);

    phase_cnt2 = 0;

    if ((temp100 == 0) && (temp10 == 0)) {
      phase_cnt = 6;
    } else if (temp100 == 0) {
      phase_cnt = 4;
    } else {
      phase_cnt = 2;
    }
  } else if (phase_cnt == 2) {
    if (temp100 > 0) {
      LED_info_phases(temp100);
    } else {
      phase_cnt = 4; // skip delay

      phase_cnt2 = 0;
    }
  } else if (phase_cnt == 3) {
    if (phase_cnt2 == 0) {

    } else if (phase_cnt2 == 7) {

      phase_cnt++;
    }

    if (phase_cnt2 < 7) {
      phase_cnt2++;
    } else {
      phase_cnt2 = 0;
    }
  } else if (phase_cnt == 4) {
    if ((temp100 > 0) || (temp10 > 0)) {
      LED_info_phases(temp10);
    } else {
      phase_cnt = 6; // skip delay

      phase_cnt2 = 0;
    }
  } else if (phase_cnt == 5) {
    if (phase_cnt2 == 0) {

    } else if (phase_cnt2 == 7) {

      phase_cnt++;
    }

    if (phase_cnt2 < 7) {
      phase_cnt2++;
    } else {
      phase_cnt2 = 0;
    }
  } else if (phase_cnt == 6) {
    LED_info_phases(temp1);
  } else if (phase_cnt == 7) {
    if (phase_cnt2 == 0) {

    } else if (phase_cnt2 == 7) {

      return 1;
    }

    if (phase_cnt2 < 7) {
      phase_cnt2++;
    }
  }

  return 0;
}

void LED_info_phases(int& _data_in) {
  if (phase_cnt2 == 0) {
    digitalWrite(LED_INFO_OUT, HIGH);
  } if ((phase_cnt2 == 1) || (phase_cnt2 == 4)) {
    if (((phase_cnt2 == 1) && (_data_in > 0)) || ((phase_cnt2 == 4) && (_data_in == 0))) {
      digitalWrite(LED_INFO_OUT, LOW);
    }
  } if (phase_cnt2 == 5) {
    if (_data_in > 1) {
      _data_in--;
    } else {
      phase_cnt++;
    }
  }

  if (phase_cnt2 < 5) {
    phase_cnt2++;

  } else {
    phase_cnt2 = 0;
  }
}

#ifdef serial_debug_on
// (This function is part of Multiple.ino):
// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) softSerial.print("0");
    softSerial.print(deviceAddress[i], HEX);
  }
}

#endif
