#include "ignitionMap.h"
#include "fuelMap.h"  // Importa rpmSteps, TPS/MAP_STEPS, interpolar2D
#include "blending.h" // Para getBlendFactor se usar modo IGN_BLEND

IgnitionStrategy ignitionStrategy = IGN_TPS_BASED;

// Tabelas de ignição
float ignitionMapTPS[RPM_STEPS][TPS_STEPS] = {
  {10, 12, 14, 16, 18, 20, 22, 24},
  {12, 14, 16, 18, 20, 22, 24, 26},
  {14, 16, 18, 20, 22, 24, 26, 28},
  {16, 18, 20, 22, 24, 26, 28, 30},
  {18, 20, 22, 24, 26, 28, 30, 32},
  {20, 22, 24, 26, 28, 30, 32, 34},
  {22, 24, 26, 28, 30, 32, 34, 36},
  {24, 26, 28, 30, 32, 34, 36, 38}
};

float ignitionMapMAP[RPM_STEPS][MAP_STEPS] = {
  {5, 7, 9, 11, 13, 15, 17, 19},
  {7, 9, 11, 13, 15, 17, 19, 21},
  {9, 11, 13, 15, 17, 19, 21, 23},
  {11, 13, 15, 17, 19, 21, 23, 25},
  {13, 15, 17, 19, 21, 23, 25, 27},
  {15, 17, 19, 21, 23, 25, 27, 29},
  {17, 19, 21, 23, 25, 27, 29, 31},
  {19, 21, 23, 25, 27, 29, 31, 33}
};

// Corrige o tipo da função para evitar dependência de macro RPM_STEPS
extern float interpolar2D(float x, float y, float map[][8], int xLen, int yLen, float* xAxis, float* yAxis);

float getIgnitionAdvance(float rpm, float tps, float map) {
  float ignTPS = interpolar2D(rpm, tps, ignitionMapTPS, RPM_STEPS, TPS_STEPS, rpmSteps, tpsSteps);
  float ignMAP = interpolar2D(rpm, map, ignitionMapMAP, RPM_STEPS, MAP_STEPS, rpmSteps, mapSteps);

  switch (ignitionStrategy) {
    case IGN_TPS_BASED:
      return ignTPS;
    case IGN_MAP_BASED:
      return ignMAP;
    case IGN_BLEND: {
      float blend = getBlendFactor(rpm);
      return ignTPS * (1.0 - blend) + ignMAP * blend;
    }
    default:
      return ignTPS;
  }
}
