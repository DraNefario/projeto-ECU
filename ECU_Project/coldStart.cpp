#include "coldStart.h"

// Tabela de correção [temperatura °C, fator de correção]
const int coldStartTableSize = 6;
float coldStartTable[coldStartTableSize][2] = {
  {0, 1.6},
  {10, 1.4},
  {20, 1.2},
  {30, 1.1},
  {40, 1.05},
  {60, 1.0}
};

float getColdStartCorrection(float temperature) {
  // Se o valor não é confiável, retorna sem correção
  if (temperature == -99.9) return 1.0;

  // Antes do primeiro ponto
  if (temperature <= coldStartTable[0][0]) {
    return coldStartTable[0][1];
  }

  // Depois do último ponto
  if (temperature >= coldStartTable[coldStartTableSize - 1][0]) {
    return coldStartTable[coldStartTableSize - 1][1];
  }

  // Interpolação linear entre os pontos
  for (int i = 0; i < coldStartTableSize - 1; i++) {
    float t1 = coldStartTable[i][0];
    float t2 = coldStartTable[i + 1][0];
    float f1 = coldStartTable[i][1];
    float f2 = coldStartTable[i + 1][1];

    if (temperature >= t1 && temperature <= t2) {
      float factor = f1 + (f2 - f1) * ((temperature - t1) / (t2 - t1));
      return factor;
    }
  }

  return 1.0; // fallback, não deveria chegar aqui
}
