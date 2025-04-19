#include "fuelMap.h"
#include "blending.h"

FuelStrategy strategy = BLEND;

float rpmSteps[RPM_STEPS] = { 600, 1000, 1500, 2000, 3000, 4000, 5000, 6000 };
float tpsSteps[TPS_STEPS] = { 0, 10, 20, 40, 60, 75, 90, 100 };
float mapSteps[MAP_STEPS] = { 20, 30, 40, 50, 60, 70, 80, 90 };

float fuelMapAlphaN[RPM_STEPS][TPS_STEPS] = {
  {400, 500, 600, 700, 800, 900, 1000, 1100},
  {500, 600, 700, 800, 900, 1000, 1100, 1200},
  {600, 700, 800, 900, 1000, 1100, 1200, 1300},
  {700, 800, 900, 1000, 1100, 1200, 1300, 1400},
  {800, 900, 1000, 1100, 1200, 1300, 1400, 1500},
  {900, 1000, 1100, 1200, 1300, 1400, 1500, 1600},
  {1000, 1100, 1200, 1300, 1400, 1500, 1600, 1700},
  {1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800}
};

float fuelMapSpeedDensity[RPM_STEPS][MAP_STEPS] = {
  {300, 400, 500, 600, 700, 800, 900, 1000},  // RPM baixo
  {350, 450, 550, 650, 750, 850, 950, 1050},  // RPM médio-baixo
  {400, 500, 600, 700, 800, 900, 1000, 1100},  // RPM médio
  {450, 550, 650, 750, 850, 950, 1050, 1150},  // RPM médio-alto
  {500, 600, 700, 800, 900, 1000, 1100, 1200},  // RPM alto
  {550, 650, 750, 850, 950, 1050, 1150, 1250},  // RPM muito alto
  {600, 700, 800, 900, 1000, 1100, 1200, 1300}, // RPM muito alto
  {650, 750, 850, 950, 1050, 1150, 1250, 1350}  // RPM máximo
};

float interpolar2D(float x, float y, float map[RPM_STEPS][8], int xLen, int yLen, float* xAxis, float* yAxis) {
  int x1i = 0, y1i = 0;

  for (int i = 0; i < xLen - 1; i++) {
    if (x >= xAxis[i] && x < xAxis[i + 1]) {
      x1i = i;
      break;
    }
  }

  for (int i = 0; i < yLen - 1; i++) {
    if (y >= yAxis[i] && y < yAxis[i + 1]) {
      y1i = i;
      break;
    }
  }

  float x1 = xAxis[x1i], x2 = xAxis[x1i + 1];
  float y1 = yAxis[y1i], y2 = yAxis[y1i + 1];

  float Q11 = map[x1i][y1i];
  float Q12 = map[x1i][y1i + 1];
  float Q21 = map[x1i + 1][y1i];
  float Q22 = map[x1i + 1][y1i + 1];

  float denom = (x2 - x1) * (y2 - y1);
  if (denom == 0) return Q11;

  return (Q11 * (x2 - x) * (y2 - y) +
          Q21 * (x - x1) * (y2 - y) +
          Q12 * (x2 - x) * (y - y1) +
          Q22 * (x - x1) * (y - y1)) / denom;
}

float interpolateAlphaN(float rpm, float tps) {
  return interpolar2D(rpm, tps, fuelMapAlphaN, RPM_STEPS, TPS_STEPS, rpmSteps, tpsSteps);
}

float interpolateSpeedDensity(float rpm, float map) {
  return interpolar2D(rpm, map, fuelMapSpeedDensity, RPM_STEPS, MAP_STEPS, rpmSteps, mapSteps);
}

float getBasePulseWidth(float rpm, float tps, float map) {
  if (strategy == ALPHA_N) {
    return interpolateAlphaN(rpm, tps);
  } else if (strategy == SPEED_DENSITY) {
    return interpolateSpeedDensity(rpm, map);
  } else if (strategy == BLEND) {
    float tpsValue = interpolateAlphaN(rpm, tps);
    float mapValue = interpolateSpeedDensity(rpm, map);
    float blendFactor = getBlendFactor(rpm);  // ex: 0.0 = 100% TPS, 1.0 = 100% MAP
    return tpsValue * (1.0 - blendFactor) + mapValue * blendFactor;
  }

  return 0.0;
}
