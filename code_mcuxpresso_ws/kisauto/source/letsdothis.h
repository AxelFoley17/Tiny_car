#ifndef LETSDOTHIS_H
#define LETSDOTHIS_H


#include "stdint.h"
uint8_t canGo();//checks if the car can go
void resetDistances();//resets the distances
void stop();//stops the car
void set_wheels(int8_t leftPercent, int8_t rightPercent);//sets the pwm for the wheels

#endif // LETSDOTHIS_H