#include "board.h"
#include "peripherals.h"
#include "LPC845.h"
#include "Sys.h"
#include "SysTimer.h"
#include "Dist/UH_HCSR-04.h"
#include "fsl_debug_console.h"
#include "letsdothis.h"
#include "timeBetween.h"

const uint16_t MIN_DISTANCE = 40u; //cm
const uint16_t MAX_DISTANCE = 400u; //cm

//defines
#define DISTANCES_SIZE 30


//types
typedef enum DistState{
	good = 0,
	bad = 1,
	unknown = 2,
	beingMeasured = 3
}DistState;

typedef struct Dist{
	uint8_t distance;
	uint8_t measuredTimes;
	DistState distState;
}Dist;


//variables
Dist distances[DISTANCES_SIZE];

int16_t weFace = 0;

int16_t desiredDirection = 0;

const int16_t ALPHA = 5;


//functions
void resetDistances(){
	for(int16_t i = 0; i < DISTANCES_SIZE; i++){
		distances[i].distance = 0;
		distances[i].measuredTimes = 0;
		distances[i].distState = unknown;
	}
}

int16_t inRangeIndex(int16_t index){
	while(index >= DISTANCES_SIZE){
		index -= DISTANCES_SIZE;
	}
	while(index < 0){
		index += DISTANCES_SIZE;
	}
	return index;
}
extern volatile uint32_t g_systickCounter;
void stop(){
	SCTIMER_UpdatePwmDutycycle(SCT0, 0, (uint8_t) 100, SCT0_pwmEvent[0]);
	SCTIMER_UpdatePwmDutycycle(SCT0, 1, (uint8_t) 100, SCT0_pwmEvent[1]);
	SCTIMER_UpdatePwmDutycycle(SCT0, 2, (uint8_t) 100, SCT0_pwmEvent[2]);
	SCTIMER_UpdatePwmDutycycle(SCT0, 3, (uint8_t) 100, SCT0_pwmEvent[3]);
	const uint32_t start = g_systickCounter;
	const uint32_t end = start + 150;
	while(	g_systickCounter < end ||
			(start > end &&
			g_systickCounter > start)
		){
		__asm("NOP");//busy wait
	}
}

void set_wheels(int8_t leftPercent, int8_t rightPercent){
	if(leftPercent <= -100) leftPercent = -95;
	else if(leftPercent >= 100) leftPercent = 95;
	
	if(rightPercent <= -100) rightPercent = -95;
	else if(rightPercent >= 100) rightPercent = 95;
	
	if(leftPercent == 0){
		SCTIMER_UpdatePwmDutycycle(SCT0, 0, (uint8_t) 0, SCT0_pwmEvent[0]);
		SCTIMER_UpdatePwmDutycycle(SCT0, 1, (uint8_t) 0, SCT0_pwmEvent[1]);
	}else if(leftPercent > 0){
		SCTIMER_UpdatePwmDutycycle(SCT0, 0, (uint8_t) 0, SCT0_pwmEvent[0]);
		SCTIMER_UpdatePwmDutycycle(SCT0, 1, (uint8_t) leftPercent-1, SCT0_pwmEvent[1]);
	}else if(leftPercent < 0){
		SCTIMER_UpdatePwmDutycycle(SCT0, 1, (uint8_t) 0, SCT0_pwmEvent[1]);
		SCTIMER_UpdatePwmDutycycle(SCT0, 0, (uint8_t) (-leftPercent)-1, SCT0_pwmEvent[0]);
	}
	
	
	if(rightPercent == 0){
		SCTIMER_UpdatePwmDutycycle(SCT0, 2, (uint8_t) 0, SCT0_pwmEvent[2]);
		SCTIMER_UpdatePwmDutycycle(SCT0, 3, (uint8_t) 0, SCT0_pwmEvent[3]);
	}else if(rightPercent > 0){
		SCTIMER_UpdatePwmDutycycle(SCT0, 3, (uint8_t) 0, SCT0_pwmEvent[3]);
		SCTIMER_UpdatePwmDutycycle(SCT0, 2, (uint8_t) rightPercent+1, SCT0_pwmEvent[2]);
	}else if(rightPercent < 0){
		SCTIMER_UpdatePwmDutycycle(SCT0, 2, (uint8_t) 0, SCT0_pwmEvent[2]);
		SCTIMER_UpdatePwmDutycycle(SCT0, 3, (uint8_t) (-rightPercent)+1, SCT0_pwmEvent[3]);
	}
}

void rotateOne(uint8_t direction){//false = left, true = right
	const uint32_t ROT_DURATION = 52u;
	const int8_t ROTSPEED = 90u;
	uint32_t start = g_systickCounter;
	uint32_t end = start + ROT_DURATION;
	if(direction) set_wheels(ROTSPEED-1, -ROTSPEED+1);
	else set_wheels((-ROTSPEED)-2, ROTSPEED+2);

	while( between(g_systickCounter, start, end)){//this is only a little bit of waiting
		__asm("NOP");//busy wait
	}

	stop();
}


void rotateTo(int16_t index){//itt mar nem kell inrange be lennie, konyebb szamolas miatt ki is szedjuk onnan
	index = inRangeIndex(index);
	weFace = inRangeIndex(weFace);
	uint8_t turnLeft = FALSE;
	if(index < weFace && abs(weFace - index) < abs(weFace - (index + DISTANCES_SIZE))) turnLeft = TRUE;
	if(index > weFace && abs(weFace - index) > abs(weFace - (index - DISTANCES_SIZE))) turnLeft = TRUE;
	while(index != weFace){
		if(turnLeft == FALSE){
			rotateOne(FALSE);
			weFace++;
		}else if(turnLeft == TRUE){
			rotateOne(TRUE);
			weFace--;
		}
		weFace = inRangeIndex(weFace);
	}
	return;
}

void scan(int16_t index){
	const uint16_t measureTimes = 5;
	index = inRangeIndex(index);
	rotateTo(index);

	if(distances[index].distState != beingMeasured && distances[index].measuredTimes < measureTimes){//currently not measuring, but we need more
		UH_measure(TRUE);
		distances[index].distState = beingMeasured;
	}
	if(distances[index].distState == beingMeasured && UH_measure(FALSE).ready && TimeoutDone1()){//measuring done
		if(UH_measure(FALSE).valid){
			distances[index].distance += UH_measure(FALSE).value / measureTimes;//we only need the average
		}
		else{
			distances[index].distance = MAX_DISTANCE / measureTimes;//we only need the average;
		}
		distances[index].measuredTimes++;
		distances[index].distState = unknown;
	}
}

DistState isFarEnough(int16_t index){
	index = inRangeIndex(index);
	if(distances[index].distState == bad){//just to be safe
		return bad;
	}
	if(distances[index].distState == good){//just to be safe
		return good;
	}

	if(distances[index].measuredTimes < 5){//we havent yet measured this direction fully
		scan(index);
	}
	if(distances[index].measuredTimes == 5){//we are done measuring, lets evaulate
		if( distances[index].distance > (int) MIN_DISTANCE ){
			distances[index].distState = good;
			return good;
		}
		distances[index].distState = bad;
		return bad;
	}
	return unknown;//we arent done measuring so its unknown
}

DistState directionIsGood(int16_t index){
	index = inRangeIndex(index);
	for(int16_t i = 1; i <= ALPHA; i++){
		DistState res = isFarEnough(index - i);
		if( res == bad ){//this section is bad we cant go in this direction
			return bad;
		}else if(res == unknown){//we arent yet done scanning this section
			return unknown;
		}else if(res == good){
			continue;
		}
	}
	for(int16_t i = 1; i <= ALPHA; i++){
		DistState res = isFarEnough(index + i);
		if( res == bad ){//this section is bad we cant go in this direction
			return bad;
		}else if(res == unknown){//we arent yet done scanning this section
			return unknown;
		}else if(res == good){
			continue;
		}
	}

	return good;//all the directions were good -> we are good to go
}

uint8_t canGo(){
	if(desiredDirection >= DISTANCES_SIZE){//we have done a full rotation
		desiredDirection = 0;
		resetDistances();
	}
	DistState res = directionIsGood(desiredDirection);
	if(res == bad){//we need to go to the next section
		desiredDirection += ALPHA * 2;
		return FALSE;
	}
	if(res == unknown){//we arent yet done scanning this section
		return FALSE;
	}

	if(res == good){
		rotateTo(inRangeIndex(desiredDirection));
		resetDistances();
		weFace = 0;
		desiredDirection = 0;
		return TRUE;
	}
}
