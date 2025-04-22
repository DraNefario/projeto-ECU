#include <Arduino.h>
#include "config.h"
#include "ignitionControl.h"

static int currentCoilIndex = 0;
static unsigned long lastSparkTime = 0;

void setupIgnition() {
  for (int i = 0; i < numCoils; i++) {
    pinMode(coilPins[i], OUTPUT);
    digitalWrite(coilPins[i], LOW);
  }
}

void handleIgnition(int rpm, float advance, unsigned long currentMicros) {
  if (rpm <= 0 || advance < 0) return;

  float usPerRev = 60000000.0 / rpm;
  float usPerDegree = usPerRev / 360.0;
  float sparkAdvanceTime = advance * usPerDegree;
  float ignitionInterval = usPerRev / numCoils;

  if ((long)(currentMicros - lastSparkTime) >= (long)(ignitionInterval - sparkAdvanceTime)) {
    int coilToFire = firingOrder[currentCoilIndex];

    digitalWrite(coilPins[coilToFire], HIGH);
    delayMicroseconds(DWELL_TIME_US);
    digitalWrite(coilPins[coilToFire], LOW);

    lastSparkTime = currentMicros;
    currentCoilIndex = (currentCoilIndex + 1) % numCoils;
  }
}
