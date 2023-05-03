/*
 * UH_HCSR-04.h
 *
 *  Created on: 2023. febr. 26.
 *      Author: benes
 */

#ifndef UH_HCSR_04_H_
#define UH_HCSR_04_H_

#include "stdint.h"

typedef struct DistMeasResult
{
	uint16_t value;
	uint8_t valid;
	uint8_t ready;
}DistMeasResult;

//Maximális mérhető távolság
#define DistanceMax 400

//extern void UH_init(void);
//extern int8 UH_measure(uint8 rst, DistMeasResult *value);
extern DistMeasResult UH_measure(uint8_t rst);


#endif /* UH_HCSR_04_H_ */
