#include "fuelMap.h"

int fuelMap[8][8] = {
  {400, 500, 600, 700, 800, 900, 1000, 1100},
  {500, 600, 700, 800, 900, 1000, 1100, 1200},
  {600, 700, 800, 900, 1000, 1100, 1200, 1300},
  {700, 800, 900, 1000, 1100, 1200, 1300, 1400},
  {800, 900, 1000, 1100, 1200, 1300, 1400, 1500},
  {900, 1000, 1100, 1200, 1300, 1400, 1500, 1600},
  {1000, 1100, 1200, 1300, 1400, 1500, 1600, 1700},
  {1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800}
};

const int tpsSteps[8] = {0, 15, 30, 45, 60, 75, 90, 100};
const int rpmSteps[8] = {0, 1000, 2000, 3000, 4000, 5000, 6000, 7000};

int getIndex(float value, const int* steps, int size) {
  for (int i = 0; i < size - 1; i++) {
    if (value >= steps[i] && value < steps[i + 1]) return i;
  }
  return size - 1;
}

float bilinearInterpolation(float x, float y,
                            float x1, float x2,
                            float y1, float y2,
                            float Q11, float Q12,
                            float Q21, float Q22) {
  float denom = (x2 - x1) * (y2 - y1);
  if (denom == 0) return Q11;
  return (
    Q11 * (x2 - x) * (y2 - y) +
    Q21 * (x - x1) * (y2 - y) +
    Q12 * (x2 - x) * (y - y1) +
    Q22 * (x - x1) * (y - y1)
  ) / denom;
}
