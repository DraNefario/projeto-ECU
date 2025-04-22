#pragma once

#define LED_PIN 16
#define TPS_PIN 4
#define RPM_PIN 6
#define MAP_PIN 7


#define numCoils 4
#define coilPins numCoilsPins

#define SD_CS_PIN 5

#define IGNITION_PIN 19
#define DWELL_TIME_US 3000  // Tempo de carga da bobina, ex: 3ms
#define numCylinders 1      // Você pode ajustar depois


#define INJECTOR1_PIN 18
#define INJECTOR2_PIN 8
#define INJECTOR3_PIN 3
#define INJECTOR4_PIN 9

#define ONE_WIRE_BUS 17
#define RPM_CUT_LIMIT 10000  // RPM máximo permitido antes do corte

const int idleRpm = 800;
const int visualBoost = 100;
const int intervaloSerial = 50;

const int numInjectors = 4;
const int injectors[numInjectors] = {INJECTOR1_PIN, INJECTOR3_PIN, INJECTOR4_PIN, INJECTOR2_PIN};

const int numCoilsPins[numCoils] = {20, 15, 14, 19};

// Ordem de disparo: 1-3-4-2
const int firingOrder[numCoils] = {0, 2, 3, 1};  // Índices = {cil1, cil3, cil4, cil2}
