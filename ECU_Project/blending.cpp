#include "blending.h"

// Exemplo: Curva simples de transição com breakpoints em RPM
const int rpmPoints[8] = {600, 1000, 1500, 2000, 3000, 4000, 5000, 6000};
const float blendValues[8] = {0.0, 0.15, 0.25, 0.40, 0.5, 0.65 ,0.75, 1.0};  // 0.0 = só TPS, 1.0 = só MAP

float getBlendFactor(int rpm) {
  if (rpm <= rpmPoints[0]) return blendValues[0];
  if (rpm >= rpmPoints[7]) return blendValues[8];

  for (int i = 0; i < 7; i++) {
    if (rpm < rpmPoints[i + 1]) {
      float r1 = rpmPoints[i];
      float r2 = rpmPoints[i + 1];
      float v1 = blendValues[i];
      float v2 = blendValues[i + 1];
      return v1 + (v2 - v1) * ((rpm - r1) / (r2 - r1));
    }
  }
  return 0.5;
}
