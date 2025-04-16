// serial_ui.cpp - Interface simples por terminal
#include <Arduino.h>
#include "fuelMap.h"
#include "config.h"

void printFuelMap() {
  Serial.println("\n--- Fuel Map [TPS x RPM] ---");
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      Serial.printf("%5d ", fuelMap[i][j]);
    }
    Serial.println();
  }
  Serial.println("----------------------------\n");
}

void handleSerialInput() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "print map") {
      printFuelMap();
    } else if (command.startsWith("set map ")) {
      // Exemplo: set map 3 4 1450 → linha 3, coluna 4 = 1450
      int tpsIdx, rpmIdx, value;
      sscanf(command.c_str(), "set map %d %d %d", &tpsIdx, &rpmIdx, &value);
      if (tpsIdx >= 0 && tpsIdx < 8 && rpmIdx >= 0 && rpmIdx < 8) {
        fuelMap[tpsIdx][rpmIdx] = value;
        Serial.printf("Valor alterado: [%d][%d] = %d\n", tpsIdx, rpmIdx, value);
      } else {
        Serial.println("Índices fora do intervalo 0-7.");
      }
    } else {
      Serial.println("Comando inválido. Use: 'print map' ou 'set map i j valor'");
    }
  }
}