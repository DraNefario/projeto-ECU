#pragma once

#define LED_PIN 16
#define TPS_PIN 4
#define RPM_PIN 5

#define INJECTOR1_PIN 18
#define INJECTOR2_PIN 8
#define INJECTOR3_PIN 3
#define INJECTOR4_PIN 9

#define ONE_WIRE_BUS 17

const int idleRpm = 800;
const int visualBoost = 250;
const int intervaloSerial = 50;

const int numInjectors = 4;
const int injectors[numInjectors] = {INJECTOR1_PIN, INJECTOR3_PIN, INJECTOR4_PIN, INJECTOR2_PIN};
