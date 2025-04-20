#ifndef IGNITION_MAP_H
#define IGNITION_MAP_H

enum IgnitionStrategy {
  IGN_TPS_BASED,
  IGN_MAP_BASED,
  IGN_BLEND
};

extern IgnitionStrategy ignitionStrategy;

float getIgnitionAdvance(float rpm, float tps, float map);

#endif
