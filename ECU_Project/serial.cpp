// serial_ui.cpp - Interface simples por terminal
#include <Arduino.h>
#include "fuelMap.h"
#include "config.h"

void printFuelMap() {
  if (strategy == ALPHA_N) {
    // Imprimir o mapa do Alpha-N
    Serial.println("\n--- Fuel Map [TPS x RPM] ---");
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        Serial.printf("%5d ", fuelMapAlphaN[i][j]);
      }
      Serial.println();
    }
    Serial.println("----------------------------\n");
  } else if (strategy == SPEED_DENSITY) {
    // Imprimir o mapa do Speed-Density
    Serial.println("\n--- Fuel Map [RPM x MAP] ---");
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        Serial.printf("%5d ", fuelMapSpeedDensity[i][j]);
      }
      Serial.println();
    }
    Serial.println("----------------------------\n");
  }
}

void handleSerialInput() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "print map") {
      printFuelMap();  // Imprimir o mapa atual baseado na estratégia
    } else if (command.startsWith("set map ")) {
      // Exemplo: set map 3 4 1450 → linha 3, coluna 4 = 1450
      int idx1, idx2, value;
      sscanf(command.c_str(), "set map %d %d %d", &idx1, &idx2, &value);

      if (strategy == ALPHA_N) {
        if (idx1 >= 0 && idx1 < 8 && idx2 >= 0 && idx2 < 8) {
          fuelMapAlphaN[idx1][idx2] = value;
          Serial.printf("Valor alterado no Alpha-N: [%d][%d] = %d\n", idx1, idx2, value);
        } else {
          Serial.println("Índices fora do intervalo 0-7.");
        }
      } else if (strategy == SPEED_DENSITY) {
        if (idx1 >= 0 && idx1 < 8 && idx2 >= 0 && idx2 < 8) {
          fuelMapSpeedDensity[idx1][idx2] = value;
          Serial.printf("Valor alterado no Speed-Density: [%d][%d] = %d\n", idx1, idx2, value);
        } else {
          Serial.println("Índices fora do intervalo 0-7.");
        }
      }
    } else if (command == "set strategy ALPHA_N") {
      strategy = ALPHA_N;
      Serial.println("Estratégia alterada para: ALPHA_N");
    } else if (command == "set strategy SPEED_DENSITY") {
      strategy = SPEED_DENSITY;
      Serial.println("Estratégia alterada para: SPEED_DENSITY");
    } else if (command == "set strategy BLEND") {
      strategy = BLEND;
      Serial.println("Estratégia alterada para: BLEND");
    } else {
      Serial.println("Comando inválido. Use: 'print map', 'set map i j valor', ou 'set strategy'");
    }
  }
}
