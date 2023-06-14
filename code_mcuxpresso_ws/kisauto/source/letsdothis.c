#include "board.h"
#include "peripherals.h"
#include "LPC845.h"
#include "Sys.h"
#include "SysTimer.h"
#include "Dist/UH_HCSR-04.h"
#include "fsl_debug_console.h"
#include "letsdothis.h"

const uint16_t MIN_DISTANCE = 40u; //cm
const uint16_t MAX_DISTANCE = 400u; //cm


#define DISTANCES_SIZE 30
int distances[DISTANCES_SIZE];

void resetDistances(){
	for(uint8_t i = 0; i < DISTANCES_SIZE; i++){
		distances[i] = -1;
	}
}

const int16_t ALPHA = 5;

int16_t weFace = 0;

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
	
	while(g_systickCounter < end ||
		(start > end && // tulfolyt
		g_systickCounter > start)//de a g_systickCounter meg nem folyt tul
		){
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
	const uint16_t measureTime = 5;
	index = inRangeIndex(index);
	rotateTo(index);
	
	uint16_t result = 0;
	for(uint16_t i = 0; i < measureTime; i++){
		UH_measure(TRUE);
		while(!UH_measure(FALSE).ready && !TimeoutDone1()){
			__asm("NOP");//busy wait
		}
		
		const uint32_t start = g_systickCounter;
		const uint32_t end = start + 50;
		while(	g_systickCounter < end ||
				(start > end &&
				g_systickCounter > start)
			){
			__asm("NOP");//busy wait
		}
		if(UH_measure(FALSE).valid){
			result += UH_measure(FALSE).value;
		}
		else{
			result += MAX_DISTANCE;
		}
	}
	distances[index] = (int)(result / measureTime);
}

uint8_t isFarEnough(int16_t index){
	index = inRangeIndex(index);
	if(distances[index] == -1){
		scan(index);
	}
	
	if(distances[index] > (int)MIN_DISTANCE){//this hurts but we arent using stdbool
		return TRUE;
	}
	return FALSE;
}

uint8_t directionIsGood(int16_t index){
	index = inRangeIndex(index);
	if(!isFarEnough(index)){
		return FALSE;
	}
	for(int16_t i = 1; i <= ALPHA; i++){
		if(!isFarEnough(index - i)){
			return FALSE;
		}
	}
	for(int16_t i = 1; i <= ALPHA; i++){
		if(!isFarEnough(index + i)){
			return FALSE;
		}
	}
	return TRUE;
}

uint8_t carNewDir(){
	PRINTF("carNewDir\r\n");
	resetDistances();
	weFace = 0;
	int16_t dir = 0;
	
	while(!directionIsGood(dir) && dir < DISTANCES_SIZE){
		dir += ALPHA * 2;
	}
	if(dir < DISTANCES_SIZE){
		rotateTo(inRangeIndex(dir));
		return TRUE;
	}
	rotateTo(inRangeIndex(dir));//visszaforduljon az eredeti állapotba
	return FALSE;
}

void corregateSpeed(uint32_t desiredRisingEdgeRate[], uint32_t currentRisingEdgeRate[], uint8_t *speeds[]){
	if(desiredRisingEdgeRate[0] < currentRisingEdgeRate[0]){//these are times between the pings, less means faster speed
		*(speeds[0]) += 1;
	}else if(desiredRisingEdgeRate[0] > currentRisingEdgeRate[0]){
		*(speeds[0]) -= 1;
	}
	
	if(desiredRisingEdgeRate[1] < currentRisingEdgeRate[1]){
		*(speeds[1]) += 1;
	}else if(desiredRisingEdgeRate[1] > currentRisingEdgeRate[1]){
		*(speeds[1]) -= 1;
	}
}

extern volatile uint32_t rightTimeSinceLast;
extern volatile uint32_t leftTimeSinceLast;

extern volatile uint32_t u32_pwmCounter;//csak bizonyos időnként állítjuk a pwm-et

void go(){
	PRINTF("go\r\n");
	const uint32_t GO_DURATION = 500u; // 0.7sec
	uint8_t goRightSpeed = 65u;
	uint8_t goLeftSpeed = 65u;
	uint32_t start = g_systickCounter;
	uint32_t end = start + GO_DURATION;
	uint32_t desiredRisingEdgeRate[] = {9000, 9000};//left, right
	while(g_systickCounter < end ||
		(start > end && // tulfolyt
		g_systickCounter > start)//de a g_systickCounter meg nem folyt tul
		){
		uint32_t currentRisingEdgeRate[] = {leftTimeSinceLast, rightTimeSinceLast};
		uint8_t *speeds[] = {&goLeftSpeed, &goRightSpeed};
		if(u32_pwmCounter == 0U){
			u32_pwmCounter = PWM_CHANGE_TIME;
			corregateSpeed(desiredRisingEdgeRate, currentRisingEdgeRate, speeds);
			set_wheels(goLeftSpeed, goRightSpeed);//TODO, lehessen majd korrigalni
		}
		__asm("NOP");//busy wait //TODO is it stopped
	}
	
	stop();
}
