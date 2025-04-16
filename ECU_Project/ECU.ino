// ecu.ino - Loop principal do simulador ECU
#include <Arduino.h>
#include "fuelMap.h"
#include "sensors.h"
#include "config.h"
#include "serial.h"
#include "coldStart.h"

unsigned long lastRpm = 0;
bool corteInjecao = false;

volatile int pulsos = 0;
unsigned long ultimaLeitura = 0;
int rpm = 0;

void IRAM_ATTR contarPulso() {
  pulsos++;
}

int currentInjector = 0;
bool injectorOn = false;
unsigned long injectorStartMicros = 0;
unsigned long ultimaAtualizacaoSerial = 0;

void setup() {
  Serial.begin(115200);
  initSensors();
  pinMode(RPM_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(RPM_PIN), contarPulso, FALLING);
}

void loop() {
  unsigned long currentMicros = micros();
  float tpsPercentage = readTPS();
  float temperature = readTemperature();

  // Atualiza RPM a cada segundo
  unsigned long agora = millis();
  if (agora - ultimaLeitura >= 1000) {
    noInterrupts();
    rpm = pulsos * 60;
    pulsos = 0;
    interrupts();
    ultimaLeitura = agora;

    if (millis() < 2000) rpm = 1200;
    else if (rpm < idleRpm) rpm = idleRpm;
  }

  // Lógica de corte de injeção
  corteInjecao = (tpsPercentage < 1.0 && rpm > (idleRpm + 600)) ? true : false;

  int tpsIdx = getIndex(tpsPercentage, tpsSteps, 8);
  int rpmIdx = getIndex(rpm, rpmSteps, 8);
  int tpsIdx2 = min(tpsIdx + 1, 7);
  int rpmIdx2 = min(rpmIdx + 1, 7);

  float x1 = tpsSteps[tpsIdx];
  float x2 = tpsSteps[tpsIdx2];
  float y1 = rpmSteps[rpmIdx];
  float y2 = rpmSteps[rpmIdx2];

  float Q11 = fuelMap[tpsIdx][rpmIdx];
  float Q12 = fuelMap[tpsIdx][rpmIdx2];
  float Q21 = fuelMap[tpsIdx2][rpmIdx];
  float Q22 = fuelMap[tpsIdx2][rpmIdx2];

  float interpolatedInjection = bilinearInterpolation(tpsPercentage, rpm, x1, x2, y1, y2, Q11, Q12, Q21, Q22);

  float injectionTimeReal = 0;
  if (!corteInjecao) {
    injectionTimeReal = interpolatedInjection;
    float correction = getColdStartCorrection(temperature);
    injectionTimeReal *= correction;
  }

  float injectionTime = injectionTimeReal * visualBoost;
  unsigned long maxInjectionTime = (rpm > 0) ? (60000000UL / rpm) / numInjectors : 0;
  if (maxInjectionTime > 0 && injectionTime > maxInjectionTime) {
    injectionTime = maxInjectionTime;
  }

  if (!injectorOn) {
    digitalWrite(injectors[currentInjector], HIGH);
    injectorStartMicros = currentMicros;
    injectorOn = true;
  } else if ((long)(currentMicros - injectorStartMicros) >= (long)injectionTime) {
    digitalWrite(injectors[currentInjector], LOW);
    currentInjector = (currentInjector + 1) % numInjectors;
    injectorOn = false;
  }

  if (agora - ultimaAtualizacaoSerial >= intervaloSerial) {
    Serial.printf(
      "TPS: %5.1f%% | RPM: %4d | InjTime: %5.0fus | Temp: %6.1fC| Corte Injecao: %s\n",
      tpsPercentage, rpm, injectionTimeReal, temperature,
      corteInjecao ? "ON" : "off"
    );
    ultimaAtualizacaoSerial = agora;
  }
  handleSerialInput();
}
