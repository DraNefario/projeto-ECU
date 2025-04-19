#include "CrankEnrichment.h"

// Tabela de enriquecimento baseada na temperatura do motor (CLT)
const float crankEnrichmentTable[8] = {1.8, 1.6, 1.4, 1.2, 1.1, 1.05, 1.02, 1.0};
const int cltBreakpoints[8] = {-20, 0, 10, 20, 30, 40, 60, 80};

float getCrankEnrichmentFactor(float temperature) {
  if (temperature <= cltBreakpoints[0]) return crankEnrichmentTable[0];
  if (temperature >= cltBreakpoints[7]) return crankEnrichmentTable[7];

  for (int i = 0; i < 7; i++) {
    if (temperature < cltBreakpoints[i + 1]) {
      float t1 = cltBreakpoints[i];
      float t2 = cltBreakpoints[i + 1];
      float v1 = crankEnrichmentTable[i];
      float v2 = crankEnrichmentTable[i + 1];
      return v1 + (v2 - v1) * ((temperature - t1) / (t2 - t1));
    }
  }
  return 1.0; // fallback
}

bool isCranking(unsigned long millisAgora, int rpm) {
  return millisAgora < 2000 || rpm < 400;
}
