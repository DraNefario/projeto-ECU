#pragma once

#include <Arduino.h>

// ENUMERADOR DE FALHAS
enum FaultCode {
  NO_FAULT = 0,
  TPS_SIGNAL_LOST,
  MAP_SIGNAL_LOST,
  TEMP_SENSOR_INVALID,
  RPM_SIGNAL_LOST,
  INJECTOR_FAULT,
  NUM_FAULT_CODES
};

// ESTRUTURA QUE ARMAZENA QUAIS FALHAS ESTÃO ATIVAS
struct FaultStatus {
  bool active[NUM_FAULT_CODES];
};

// FUNÇÕES DE DIAGNÓSTICO
void checkDiagnostics(float tps, float map, float temp, int rpm);
void printFaults();
void clearFaults();
bool isFaultActive(FaultCode code);
