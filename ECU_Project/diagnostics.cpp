#include "diagnostics.h"

static FaultStatus faultStatus;

bool isFaultActive(FaultCode code) {
  return faultStatus.active[code];
}

void checkDiagnostics(float tps, float map, float temp, int rpm) {
  // Verifica limites válidos para sensores
  faultStatus.active[TPS_SIGNAL_LOST] = (tps < 0.0 || tps > 100.0);
  faultStatus.active[MAP_SIGNAL_LOST] = (map < 10 || map > 110);
  faultStatus.active[TEMP_SENSOR_INVALID] = (temp < -40 || temp > 150);

  // Verifica se o RPM está "travado" (sem pulso novo)
  static unsigned long lastRpmTime = 0;
  static int lastRpm = 0;
  unsigned long now = millis();

  if (rpm == lastRpm) {
    if (now - lastRpmTime > 1500) {
      faultStatus.active[RPM_SIGNAL_LOST] = true;
    }
  } else {
    faultStatus.active[RPM_SIGNAL_LOST] = false;
    lastRpmTime = now;
    lastRpm = rpm;
  }

  // Placeholder de falha de injetor (futura expansão com feedback real)
  faultStatus.active[INJECTOR_FAULT] = false;
}

void printFaults() {
  Serial.println("== DIAGNOSTICO DE FALHAS ==");
  for (int i = 0; i < NUM_FAULT_CODES; i++) {
    if (faultStatus.active[i]) {
      Serial.print("Falha ativa: ");
      switch (i) {
        case TPS_SIGNAL_LOST: Serial.println("TPS_SIGNAL_LOST"); break;
        case MAP_SIGNAL_LOST: Serial.println("MAP_SIGNAL_LOST"); break;
        case TEMP_SENSOR_INVALID: Serial.println("TEMP_SENSOR_INVALID"); break;
        case RPM_SIGNAL_LOST: Serial.println("RPM_SIGNAL_LOST"); break;
        case INJECTOR_FAULT: Serial.println("INJECTOR_FAULT"); break;
        default: break;
      }
    }
  }
  Serial.println("==========================");
}

void clearFaults() {
  for (int i = 0; i < NUM_FAULT_CODES; i++) {
    faultStatus.active[i] = false;
  }
  Serial.println("Todos os códigos de falha foram apagados.");
}
