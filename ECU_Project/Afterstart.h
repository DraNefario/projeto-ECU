#ifndef AFTERSTART_H
#define AFTERSTART_H

// Duração do enriquecimento após partida (ms)
const unsigned long afterstartDuration = 10000;

// Enriquecimento inicial (ex: 1.2 = 20% a mais de combustível)
const float enrichmentInicial = 1.2;

void resetAfterstart();
void updateAfterstart(unsigned long currentMillis, bool motorLigado);
float getAfterstartCorrection();

#endif
