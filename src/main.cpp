#include <Arduino.h>
#include <TimerOne.h>
#include <Thermistor.h>
#include <NTC_Thermistor.h>
#include <AverageThermistor.h>

const int fanPin = 9;

#define SENSOR_PIN A1
#define REFERENCE_RESISTANCE 10000
#define NOMINAL_RESISTANCE 100000
#define NOMINAL_TEMPERATURE 25
#define B_VALUE 3950

#define READINGS_NUMBER 10
#define DELAY_TIME 10

Thermistor *thermistor = NULL;

void setup(void)
{
  Timer1.initialize(40); // 40 us = 25 kHz
  Serial.begin(115200);

  Thermistor *originThermistor = new NTC_Thermistor(
      SENSOR_PIN,
      REFERENCE_RESISTANCE,
      NOMINAL_RESISTANCE,
      NOMINAL_TEMPERATURE,
      B_VALUE);

  thermistor = new AverageThermistor(
      originThermistor,
      READINGS_NUMBER,
      DELAY_TIME);
}

void loop(void)
{
  // slowly increase the PWM fan speed
  //
  // Reads temperature
  const double celsius = thermistor->readCelsius();
  const double duty = (celsius - 25) * 5;
  const double dutyCycle = max(min(duty, 100.0), 0.0);
  Timer1.pwm(fanPin, (dutyCycle / 100) * 1023);

  // Output of information
  Serial.print(" Temperature: ");
  Serial.print(celsius);
  Serial.print(" C, Duty Cycle: ");
  Serial.print(dutyCycle);
  Serial.println("");
  //delay(50);
}