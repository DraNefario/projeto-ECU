// RPMControl.cpp
#include "config.h"
#include "RPMControl.h"
#include <Arduino.h>

#define HISTERESIS 300

bool corteAtivo = false;

float controlInjection(int rpm, float fuelAmount) {
    // Se já está em corte e o RPM ainda está acima da faixa segura, continua o corte
    if (corteAtivo) {
        if (rpm > (RPM_CUT_LIMIT - HISTERESIS)) {
            Serial.println("Corte de giro mantido!");
            return 0;  // Mantém injeção desligada
        } else {
            corteAtivo = false;
            Serial.println("Corte de giro desativado.");
        }
    }

    // Se não está em corte, verifica se deve ativar
    if (!corteAtivo && rpm >= RPM_CUT_LIMIT) {
        corteAtivo = true;
        Serial.println("Corte de giro ativado!");
        return 0;
    }

    return fuelAmount;  // Caso normal, sem corte
}
