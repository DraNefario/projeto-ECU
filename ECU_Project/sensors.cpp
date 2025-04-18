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
  pinMode(MAP_PIN, INPUT);
}

float readTPS() {
  int tpsValue = analogRead(TPS_PIN);
  return (tpsValue / 4095.0) * 100.0;
}

float lerMAP() {
  int leitura = analogRead(MAP_PIN);  // Ex: GPIO34
  // ESP32: 12 bits = 0–4095
  // Se o MAP for 0.5V a 4.5V => 620 a 3720 (por exemplo)
  float volts = leitura * (3.3 / 4095.0);

  // Vamos mapear de 0.5V (vácuo) a 4.5V (atmosfera) pra 20 a 100 kPa
  // Se for simulando com potenciômetro 0V a 3.3V, você pode fazer assim:
  int kpa = map(leitura, 0, 4095, 20, 100);
  return constrain(kpa, 20, 100);  // Protege pra não sair do range
}

float readTemperature() {
  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);
  return (isnan(temp) || temp < -55.0 || temp > 125.0) ? -99.9 : temp;
}
