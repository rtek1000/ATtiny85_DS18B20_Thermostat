// Source: https://github.com/rtek1000/Attiny85_DS18B20_thermostat

// Attiny85 pinout - Prog. ref num. - Prog. ref. name:
// 1 - RESET -
// 2 - 3/A3 - ONE_WIRE_PRW
// 3 - 4/A2 - ONE_WIRE_BUS
// 4 - GND
// 5 - 0/PWM - LED_INFO_OUT
// 6 - 1/PWM - LED_TH_OUT
// 7 - 2/A1 - LED_ERROR
// 8 - VDD

// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>
#include <avr/wdt.h>

#define ONE_WIRE_PRW 3
#define ONE_WIRE_BUS 4

#define BEEP 2
#define LED_INFO_OUT 0
#define LED_TH_OUT 1

#define temp_max_setpoint 115.0
#define temp_min_setpoint 70.0

#define TEMPERATURE_PRECISION 9 // Lower resolution

#define average_index 5

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

DeviceAddress tempDeviceAddress; // We'll use this variable to store a found device address

float tempC = 0.0;

int error_cnt = 0;

int info_cycle_cnt = 0;

int cycle_cnt = 0;

float tempC_buff = 0.0;

float tempC_buff2 = 0.0;

int temp100 = 0;
int temp10 = 0;
int temp1 = 0;

bool invert = false;

unsigned int phase_cnt = 0;
unsigned int phase_cnt2 = 0;

/*
   The setup function. We only start the sensors here
*/
void setup(void)
{
  wdt_disable();

  cli();
  WDTCR = 0xD8 | WDTO_2S;
  sei();

  wdt_reset();

  pinMode(LED_TH_OUT, OUTPUT);
  pinMode(LED_INFO_OUT, OUTPUT);
  pinMode(ONE_WIRE_PRW, OUTPUT);
  pinMode(BEEP, OUTPUT);

  digitalWrite(BEEP, LOW);
  digitalWrite(ONE_WIRE_PRW, HIGH);

  digitalWrite(LED_TH_OUT, LOW);

  for (int i = 0; i < 10; i++) {
    delay(100);

    wdt_reset();
  }

  sensors.begin();

  sensors.getAddress(tempDeviceAddress, 0);

  if (sensors.getResolution(tempDeviceAddress) != TEMPERATURE_PRECISION) {
    sensors.setResolution(tempDeviceAddress, TEMPERATURE_PRECISION);
  }
}

/*
   Main function, get and show the temperature
*/
void loop(void)
{
  float tempCtmp;
  float tempCtmpSum;

  tempCtmpSum = 0;

  for (int i = 0; i < average_index; i++) {
    sensors.requestTemperatures(); // Send the command to get temperatures

    tempCtmp = sensors.getTempC(tempDeviceAddress);
    // Check if reading was successful
    if (tempCtmp != DEVICE_DISCONNECTED_C)
    {
      tempCtmpSum += tempCtmp;
    }
    else
    {
      break;
    }

    if (info_cycle_cnt == 0) {
      if (LED_info(tempC_buff2) != 0) {
        info_cycle_cnt = 20;

        phase_cnt = 0;

        tempC_buff2 = tempC_buff;
      }
    } else {
      info_cycle_cnt--;
    }

    wdt_reset();
  }

  if (tempCtmp != DEVICE_DISCONNECTED_C) {
    tempC = tempCtmpSum / average_index;

    if (tempC > temp_max_setpoint) {
      digitalWrite(LED_TH_OUT, HIGH);
    } else if (tempC < temp_min_setpoint) {
      digitalWrite(LED_TH_OUT, LOW);
    }

    tempC_buff = tempC;

    error_cnt = 0;
  } else {
    if (error_cnt < 5) {
      error_cnt++;
    } else {
      digitalWrite(LED_TH_OUT, HIGH);
    }

    digitalWrite(ONE_WIRE_PRW, LOW);

    for (int i = 0; i < 10; i++) {
      if (digitalRead(LED_TH_OUT) == HIGH) {
        analogWrite(LED_INFO_OUT, 250 - (i * 25));
      } else {
        analogWrite(LED_INFO_OUT, 100 - (i * 5));
      }
      delay(100);

      wdt_reset();
    }

    digitalWrite(ONE_WIRE_PRW, HIGH);

    for (int i = 0; i < 10; i++) {
      if (digitalRead(LED_TH_OUT) == HIGH) {
        analogWrite(LED_INFO_OUT, i * 25);
      } else {
        analogWrite(LED_INFO_OUT, i * 5);
      }

      delay(100);

      wdt_reset();
    }

    sensors.begin();

    sensors.getAddress(tempDeviceAddress, 0);

    if (sensors.getResolution(tempDeviceAddress) != TEMPERATURE_PRECISION) {
      sensors.setResolution(tempDeviceAddress, TEMPERATURE_PRECISION);
    }
  }

  if (digitalRead(LED_TH_OUT) == HIGH) {
    digitalWrite(BEEP, !digitalRead(BEEP));
  }
}

int LED_info(int temp) {
  bool is_negative = false;

  if (phase_cnt == 0) {
    if (temp < 0) {
      temp *= -1;

      is_negative = true;
    }

    invert = digitalRead(LED_TH_OUT);

    temp100 = temp / 100;
    temp10 = (temp - (temp100 * 100)) / 10;
    temp1 = temp - (temp100 * 100) - (temp10 * 10);

    phase_cnt2 = 0;

    if (is_negative == false) {
      if ((temp100 == 0) && (temp10 == 0)) {
        phase_cnt = 6;
      } else if (temp100 == 0) {
        phase_cnt = 4;
      } else {
        phase_cnt++;
      }
    } else {
      phase_cnt++;
    }
  } else if (phase_cnt == 1) {
    if (phase_cnt2 == 0) {
      if (invert == true) {
        analogWrite(LED_INFO_OUT, 128);
      } else {
        analogWrite(LED_INFO_OUT, 64);
      }
    } if (phase_cnt2 == 8) {
      if (invert == true) {
        analogWrite(LED_INFO_OUT, 16);
      } else {
        digitalWrite(LED_INFO_OUT, LOW);
      }
    } if (phase_cnt2 == 11) {
      wdt_reset();

      if ((temp100 == 0) && (temp10 == 0)) {
        phase_cnt = 6;
      } else if (temp100 == 0) {
        phase_cnt = 4;
      } else {
        phase_cnt++;
      }
    }

    if (phase_cnt2 < 11) {
      phase_cnt2++;
    } else {
      phase_cnt2 = 0;
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
      wdt_reset();
    } else if (phase_cnt2 == 7) {
      wdt_reset();

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
      wdt_reset();
    } else if (phase_cnt2 == 7) {
      wdt_reset();

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
      wdt_reset();
    } else if (phase_cnt2 == 7) {
      wdt_reset();

      return 1;
    }

    if (phase_cnt2 < 7) {
      phase_cnt2++;
    }
  }

  return 0;
}

void LED_info_phases(int& temp_var) {
  if (phase_cnt2 == 0) {
    if (invert == true) {
      analogWrite(LED_INFO_OUT, 128);
    } else {
      analogWrite(LED_INFO_OUT, 64);
    }
  } if ((phase_cnt2 == 1) || (phase_cnt2 == 4)) {
    if (((phase_cnt2 == 1) && (temp_var > 0)) || ((phase_cnt2 == 4) && (temp_var == 0))) {
      if (invert == true) {
        analogWrite(LED_INFO_OUT, 16);
      } else {
        digitalWrite(LED_INFO_OUT, LOW);
      }
    }
  } if (phase_cnt2 == 5) {
    wdt_reset();

    if (temp_var > 1) {
      temp_var--;
    } else {
      phase_cnt++;
    }
  }

  if (phase_cnt2 < 5) {
    phase_cnt2++;

    wdt_reset();
  } else {
    phase_cnt2 = 0;
  }
}
