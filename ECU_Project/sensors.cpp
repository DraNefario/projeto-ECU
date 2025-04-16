#include "config.h"
#include <OneWire.h>
#include <DallasTemperature.h>

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void initSensors() {
  sensors.begin();
  pinMode(LED_PIN, OUTPUT);
  for (int i = 0; i < numInjectors; i++) {
    pinMode(injectors[i], OUTPUT);
    digitalWrite(injectors[i], LOW);
  }
  pinMode(TPS_PIN, INPUT);
}

float readTPS() {
  int tpsValue = analogRead(TPS_PIN);
  return (tpsValue / 4095.0) * 100.0;
}

float readTemperature() {
  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);
  return (isnan(temp) || temp < -55.0 || temp > 125.0) ? -99.9 : temp;
}
