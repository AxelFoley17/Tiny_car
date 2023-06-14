#include "fixSpeed.h"
#include "letsdothis.h"

uint32_t desiredEdgeDelayLeft = 0;
uint32_t desiredEdgeDelayRight = 0;

extern volatile uint32_t leftTimeSinceLast;
extern volatile uint32_t rightTimeSinceLast;

int8_t leftSpeed = 70;
int8_t rightSpeed = 70;

void regulateSpeed(){
	if(desiredEdgeDelayLeft < leftTimeSinceLast){//these are times between the pings, less means faster speed
		leftSpeed += 1;
	}else if(desiredEdgeDelayLeft > leftTimeSinceLast){
		leftSpeed -= 1;
	}

	if(desiredEdgeDelayRight < rightTimeSinceLast){
		rightSpeed += 1;
	}else if(desiredEdgeDelayRight > rightTimeSinceLast){
		rightSpeed -= 1;
	}
	set_wheels(leftSpeed, rightSpeed);
}

void setDesiredEdgeDelay(uint32_t left, uint32_t right){
	desiredEdgeDelayLeft = left;
	desiredEdgeDelayRight = right;
}
