#ifndef FUELMAP_H
#define FUELMAP_H

enum FuelStrategy {
  ALPHA_N,
  SPEED_DENSITY,
  BLEND
};

extern FuelStrategy strategy;

#define RPM_STEPS 8
#define TPS_STEPS 8
#define MAP_STEPS 8

extern float rpmSteps[RPM_STEPS];
extern float tpsSteps[TPS_STEPS];
extern float mapSteps[MAP_STEPS];

extern float fuelMapAlphaN[RPM_STEPS][TPS_STEPS];
extern float fuelMapSpeedDensity[RPM_STEPS][MAP_STEPS];

float getBasePulseWidth(float rpm, float tps, float map);
float interpolateAlphaN(float rpm, float tps);
float interpolateSpeedDensity(float rpm, float map);

#endif
