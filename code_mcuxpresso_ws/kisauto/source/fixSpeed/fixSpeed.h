#ifndef FIXSPEED_H
#define FIXSPEED_H

#include "stdint.h"

void regulateSpeed();//tries to maintain the set edge delay
void setDesiredEdgeDelay(uint32_t left, uint32_t right);//sets the desired edge delay

#endif //FIXSPEED_H
