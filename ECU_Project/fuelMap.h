#pragma once

extern int fuelMap[8][8];
extern const int tpsSteps[8];
extern const int rpmSteps[8];

int getIndex(float value, const int* steps, int size);

float bilinearInterpolation(float x, float y,
                            float x1, float x2,
                            float y1, float y2,
                            float Q11, float Q12,
                            float Q21, float Q22);
