#include <Arduino.h>
#include "config.h"
#include "ignitionControl.h"

static unsigned long lastIgnitionTime = 0;

void setupIgnition() {
  pinMode(IGNITION_PIN, OUTPUT);
  digitalWrite(IGNITION_PIN, LOW);
}

void handleIgnition(int rpm, float ignitionAdvance, unsigned long currentMicros) {
  if (rpm <= 0 || ignitionAdvance < 0) return;

  float usPerRev = 60000000.0 / rpm;
  float usPerDegree = usPerRev / 360.0;
  float sparkTimeBeforeTDC = ignitionAdvance * usPerDegree;

  unsigned long timeBetweenSparks = usPerRev / numCylinders;
  unsigned long sparkTriggerTime = timeBetweenSparks - sparkTimeBeforeTDC;

  if ((long)(currentMicros - lastIgnitionTime) >= (long)sparkTriggerTime) {
    digitalWrite(IGNITION_PIN, HIGH);
    delayMicroseconds(DWELL_TIME_US);
    digitalWrite(IGNITION_PIN, LOW);

    lastIgnitionTime = currentMicros;
  }
}
