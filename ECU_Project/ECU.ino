// ecu.ino - Loop principal do simulador ECU
#include <Arduino.h>
#include "fuelMap.h"
#include "sensors.h"
#include "config.h"
#include "serial.h"
#include "coldStart.h"
#include "Afterstart.h"
#include "RPMControl.h"

unsigned long lastRpm = 0;
bool corteInjecao = false;

volatile int pulsos = 0;
unsigned long ultimaLeitura = 0;
int rpm = 0;

void IRAM_ATTR contarPulso() {
  pulsos++;
}

extern bool corteAtivo;  // Importa a variável de RPMControl.cpp


int currentInjector = 0;
bool injectorOn = false;
unsigned long injectorStartMicros = 0;
unsigned long ultimaAtualizacaoSerial = 0;

void setup() {
  Serial.begin(115200);
  initSensors();
  pinMode(RPM_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(RPM_PIN), contarPulso, FALLING);
  strategy = ALPHA_N;
}

void loop() {
  unsigned long currentMicros = micros();
  unsigned long agora = millis();

  // Leitura real dos sensores
  float tpsPercentage = readTPS();        // Leitura do TPS real
  float mapPressure = lerMAP();           // Leitura do MAP real
  float temperature = readTemperature();  // Leitura da temperatura real (se necessário)

  // Correção baseada no MAP (suaviza a interpolação do TPS)
  float correcaoMAP = mapPressure / 100.0;  // Pressão normalizada (por exemplo: 100kPa)

  // Atualiza RPM a cada segundo (como no seu código original)
  if ((long)(agora - ultimaLeitura) >= 1000) {
    noInterrupts();
    rpm = pulsos * 60;
    pulsos = 0;
    interrupts();
    ultimaLeitura = agora;

    if (agora < 2000 && rpm == 0) rpm = 1200;  // Durante o cranking
    else if (rpm < idleRpm) rpm = idleRpm;     // RPM em marcha lenta

    // Simula desaceleração se estiver em corte de giro
    if (corteAtivo) {
      rpm -= 800;  // Ajuste esse valor conforme a "velocidade" de queda desejada
      if (rpm < idleRpm) rpm = idleRpm;  // Evita cair abaixo do mínimo
    }
  }

  int rpmDisplay = corteAtivo ? 9700 : rpm;


  // Atualiza o estado do Afterstart
  updateAfterstart(agora, rpm > 400);


  // Atualiza corteInjecao para refletir tanto TPS quanto corte de giro real
  corteInjecao = corteAtivo || (tpsPercentage < 1.0 && rpm > (idleRpm + 600));


  // Chama a interpolação para calcular o tempo de injeção
  float interpolatedInjection = getBasePulseWidth(rpm, tpsPercentage, mapPressure);

  Serial.printf("interpolated: %.2f | correcaoMAP: %.2f | tempCorr: %.2f | afterCorr: %.2f\n",
                interpolatedInjection, correcaoMAP, getColdStartCorrection(temperature), getAfterstartCorrection());

  float injectionTimeReal = interpolatedInjection * correcaoMAP;

  if (!corteInjecao) {
    float correction = getColdStartCorrection(temperature);
    float afterstart = getAfterstartCorrection();
    injectionTimeReal *= correction * afterstart;
  }

  // Chama a função de controle de injeção com corte de giro
  injectionTimeReal = controlInjection(rpm, injectionTimeReal);

  // Limita o tempo de injeção
  float injectionTime = injectionTimeReal; //* visualBoost;
  unsigned long maxInjectionTime = (rpm > 0) ? (60000000UL / rpm) / numInjectors : 0;
  if (maxInjectionTime > 0 && injectionTime > maxInjectionTime) {
    injectionTime = maxInjectionTime;
  }

  // Controla os injetores
  if (injectionTimeReal > 0) {
    if (!injectorOn) {
      digitalWrite(injectors[currentInjector], HIGH);
      injectorStartMicros = currentMicros;
      injectorOn = true;
    } else if ((long)(currentMicros - injectorStartMicros) >= (long)injectionTime) {
      digitalWrite(injectors[currentInjector], LOW);
      currentInjector = (currentInjector + 1) % numInjectors;
      injectorOn = false;
    }
  }

  // Envio de dados para o terminal serial
  if ((long)(agora - ultimaAtualizacaoSerial) >= intervaloSerial) {
    Serial.printf(
      "TPS: %5.1f%% | RPM: %4d | InjTime: %5.0fus | Temp: %6.1fC | Afterstart: %.2f | Corte Injecao: %s\n\n",
      tpsPercentage, rpmDisplay, injectionTimeReal, temperature,
      getAfterstartCorrection(), corteInjecao ? "ON" : "off"
    );
    ultimaAtualizacaoSerial = agora;
  }

  handleSerialInput();  // Lê e processa entradas do serial (se necessário)
}