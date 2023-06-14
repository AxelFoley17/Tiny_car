#ifndef LETSDOTHIS_H
#define LETSDOTHIS_H


#include "stdint.h"
void rotateOne(uint8_t direction);
void go();
uint8_t carNewDir();
void corregateSpeed(uint32_t desiredRisingEdgeRate[], uint32_t currentRisingEdgeRate[], uint8_t *speeds[]);

#endif // LETSDOTHIS_H