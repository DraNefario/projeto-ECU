#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

#include "fuelMap.h"
#include "sensors.h"
#include "config.h"
#include "serial.h"
#include "coldStart.h"
#include "Afterstart.h"
#include "RPMControl.h"
#include "CrankEnrichment.h"
#include "transientEnrichment.h"
#include "ignitionMap.h"
#include "ignitionControl.h"
#include "diagnostics.h"

unsigned long lastRpm = 0;
bool corteInjecao = false;

static unsigned long lastInjectionTime = 0;


volatile int pulsos = 0;
unsigned long ultimaLeitura = 0;
int rpm = 0;

void IRAM_ATTR contarPulso() {
  pulsos++;
}

extern bool corteAtivo;

int currentInjector = 0;
bool injectorOn = false;
unsigned long injectorStartMicros = 0;
unsigned long ultimaAtualizacaoSerial = 0;

// ============================
// 🔄 Leitura do mapa via SD
// ============================
bool loadFuelMapFromCSV(const char* filename) {
  File file = SD.open(filename);
  if (!file) {
    Serial.println("Erro ao abrir arquivo de mapa.");
    return false;
  }

  int row = 0;
  while (file.available() && row < RPM_STEPS) {
    String line = file.readStringUntil('\n');
    line.trim();
    if (line.length() == 0) continue;

    int col = 0;
    int index = 0;
    while (col < TPS_STEPS + 1 && index < line.length()) {
      int nextComma = line.indexOf(',', index);
      if (nextComma == -1) nextComma = line.length();
      String valStr = line.substring(index, nextComma);
      valStr.trim();
      float value = valStr.toFloat();

      if (col == 0) rpmSteps[row] = value;
      else fuelMapAlphaN[row][col - 1] = value;

      index = nextComma + 1;
      col++;
    }
    row++;
  }

  file.close();
  Serial.println("Mapa de combustível carregado do SD.");
  return true;
}

void setup() {
  Serial.begin(115200);
  initSensors();

  pinMode(RPM_PIN, INPUT_PULLUP);  // ✅ Configure RPM antes do SD
  attachInterrupt(digitalPinToInterrupt(RPM_PIN), contarPulso, FALLING);

  strategy = ALPHA_N;
  setupIgnition();

  Serial.println("Inicializando cartão SD...");
  if (SD.begin(SD_CS_PIN)) {
    Serial.println("SD detectado.");
    loadFuelMapFromCSV("/fuelMap.csv");
  } else {
    Serial.println("Falha ao inicializar SD.");
  }
}

void loop() {
  unsigned long currentMicros = micros();
  unsigned long agora = millis();

  float tpsPercentage = readTPS();
  updateTransientEnrichment(tpsPercentage, agora);
  float mapPressure = lerMAP();
  float temperature = readTemperature();

  checkDiagnostics(tpsPercentage, mapPressure, temperature, rpm);
  float correcaoMAP = mapPressure / 100.0;

  if ((long)(agora - ultimaLeitura) >= 1000) {
    noInterrupts();
    rpm = pulsos * 60;
    pulsos = 0;
    interrupts();
    ultimaLeitura = agora;

    if (agora < 2000 && rpm == 0) rpm = 1200;
    else if (rpm < idleRpm) rpm = idleRpm;

    if (corteAtivo) {
      rpm -= 800;
      if (rpm < idleRpm) rpm = idleRpm;
    }
  }

  int rpmDisplay = corteAtivo ? 9700 : rpm;

  updateAfterstart(agora, rpm > 400);
  corteInjecao = corteAtivo || (tpsPercentage < 1.0 && rpm > (idleRpm + 600));

  float interpolatedInjection = getBasePulseWidth(rpm, tpsPercentage, mapPressure);
  float injectionTimeReal = interpolatedInjection * correcaoMAP;

  if (!corteInjecao) {
    float correction = getColdStartCorrection(temperature);
    float afterstart = getAfterstartCorrection();
    float crankEnrichment = isCranking(agora, rpm) ? getCrankEnrichmentFactor(temperature) : 1.0;
    float transient = getTransientEnrichment();
    injectionTimeReal *= correction * afterstart * crankEnrichment * transient;
  }

  float ignitionAdvance = getIgnitionAdvance(rpm, tpsPercentage, mapPressure);
  handleIgnition(rpm, ignitionAdvance, currentMicros);

  injectionTimeReal = controlInjection(rpm, injectionTimeReal);
  float injectionTime = injectionTimeReal;

  unsigned long maxInjectionTime = (rpm > 0) ? (60000000UL / rpm) / numInjectors : 0;
  if (maxInjectionTime > 0 && injectionTime > maxInjectionTime) {
    injectionTime = maxInjectionTime;
  }

  unsigned long timePerCycle = (rpm > 0) ? (60000000UL / rpm) : 0;  // 1 rotação em micros

  if ((rpm > 0) && (currentMicros - lastInjectionTime >= timePerCycle / numInjectors)) {
    // Simula o pulso de injeção para o cilindro atual
    digitalWrite(injectors[currentInjector], HIGH);
    delayMicroseconds(injectionTime);  // Tempo de injeção calculado
    digitalWrite(injectors[currentInjector], LOW);

    currentInjector = (currentInjector + 1) % numInjectors;
    lastInjectionTime = currentMicros;
  }


  if ((long)(agora - ultimaAtualizacaoSerial) >= intervaloSerial) {
    Serial.printf("TPS: %5.1f%% | RPM: %4d | InjTime: %5.0fus | Temp: %6.1fC | Afterstart: %.2f | Corte Injecao: %s\n\n",
                  tpsPercentage, rpmDisplay, injectionTimeReal, temperature,
                  getAfterstartCorrection(), corteInjecao ? "ON" : "off");
    Serial.printf("Transient: %.2f\n", getTransientEnrichment());
    ultimaAtualizacaoSerial = agora;
  }

  // === SERIAL MENU: TESTES MANUAIS DE COMPONENTES ===
  if (Serial.available()) {
    char cmd = Serial.read();

    if (cmd == 'i') {
      Serial.println("🔧 Testando injetores...");
      for (int i = 0; i < numInjectors; i++) {
        digitalWrite(injectors[i], HIGH);
        delay(100);
        digitalWrite(injectors[i], LOW);
        delay(200);
      }
    }

    if (cmd == 'b') {
      Serial.println("🔧 Testando bobinas...");
      for (int i = 0; i < numCoils; i++) {
        digitalWrite(coilPins[i], HIGH);
        delay(100);
        digitalWrite(coilPins[i], LOW);
        delay(200);
      }
    }
  }

  // === VERIFICAÇÃO DE SENSORES ===
  if (tpsPercentage < 0 || tpsPercentage > 100) Serial.println("⚠️ TPS fora de range!");
  if (mapPressure < 10 || mapPressure > 120) Serial.println("⚠️ MAP fora de range!");
  if (temperature < -20 || temperature > 150) Serial.println("⚠️ Temperatura fora de range!");


  handleSerialInput();
}
