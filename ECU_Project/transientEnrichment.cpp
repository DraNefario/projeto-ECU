#include "transientEnrichment.h"

static float lastTps = 0.0;
static float enrichment = 1.0;
static unsigned long lastUpdate = 0;

const float tpsThreshold = 1.5;    // variação mínima de TPS (%) para considerar aceleração
const float maxEnrichment = 1.2;   // máximo enriquecimento (ex: 20%)
const float enrichmentDecayRate = 0.005; // quanto o enriquecimento "volta ao normal" por ms

void updateTransientEnrichment(float tps, unsigned long nowMillis) {
  float deltaTps = tps - lastTps;
  unsigned long deltaTime = nowMillis - lastUpdate;

  if (deltaTps > tpsThreshold) {
    float gain = deltaTps * 0.05;  // fator de impacto da variação
    if (gain > (maxEnrichment - 1.0)) gain = (maxEnrichment - 1.0);
    enrichment = 1.0 + gain;
  } else {
    enrichment -= enrichmentDecayRate * deltaTime;
    if (enrichment < 1.0) enrichment = 1.0;
  }

  lastTps = tps;
  lastUpdate = nowMillis;
}

float getTransientEnrichment() {
  return enrichment;
}
