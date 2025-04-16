#include "Afterstart.h"
#include <Arduino.h>

// Estado interno do afterstart
unsigned long afterstartBegin = 0;
bool afterstartAtivo = false;
bool afterstartInicializado = false;

void resetAfterstart() {
  afterstartBegin = millis();
  afterstartAtivo = true;
  afterstartInicializado = true;
}

void updateAfterstart(unsigned long currentMillis, bool motorLigado) {
  // Reset total se motor desligado
  if (!motorLigado) {
    afterstartAtivo = false;
    afterstartInicializado = false;
    return;
  }

  // Se o motor acabou de ligar e ainda não iniciou afterstart
  if (motorLigado && !afterstartInicializado) {
    resetAfterstart();
  }
}

float getAfterstartCorrection() {
  if (!afterstartAtivo) return 1.0;

  unsigned long elapsed = millis() - afterstartBegin;
  if (elapsed >= afterstartDuration) {
    afterstartAtivo = false;
    return 1.0;
  }

  // Decaimento linear de enriquecimento: começa em 1.2, vai para 1.0
  float progresso = (float)elapsed / afterstartDuration;
  return 1.0 + (enrichmentInicial - 1.0) * (1.0 - progresso);
}
