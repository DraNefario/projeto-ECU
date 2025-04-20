#pragma once

#define LED_PIN 16
#define TPS_PIN 4
#define RPM_PIN 5
#define MAP_PIN 10

#define IGNITION_PIN 11  
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
