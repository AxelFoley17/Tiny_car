#include "stdint.h"
#include "peripherals.h"
#include "LPC845.h"

volatile uint32_t lastRightCapture = 0u;
volatile uint32_t lastLeftCapture = 0u;

volatile uint32_t rightTimeSinceLast = 0u;
volatile uint32_t leftTimeSinceLast = 0u;

#define BUFFERMAXSIZE 10u
volatile uint32_t rightBuffer[BUFFERMAXSIZE] = {0u};
volatile uint32_t leftBuffer[BUFFERMAXSIZE] = {0u};
uint8_t rightBufferCurrSize = 0u; 
uint8_t leftBufferCurrSize = 0u;
uint8_t rightBufferIndex = 0u;
uint8_t leftBufferIndex = 0u;

//calculates the average of the right buffer
uint32_t rightAverage(){
	uint32_t sum = 0u;
	if(rightBufferCurrSize == 0u)
		return 0u;
	for(uint8_t i = 0u; i < rightBufferCurrSize; i++){
		sum += rightBuffer[i];
	}
	return (uint32_t)((double)sum / (double)rightBufferCurrSize);
}

//calculates the average of the left buffer
uint32_t leftAverage(){
	uint32_t sum = 0u;
	if(leftBufferCurrSize == 0u)
		return 0u;
	for(uint8_t i = 0u; i < leftBufferCurrSize; i++){
		sum += leftBuffer[i];
	}
	return (uint32_t)((double)sum / (double)leftBufferCurrSize);
}

//callback for the right opto module
//sets rightTimeSinceLast to the average of the right buffer
void rightWheel_cb(uint32_t flags){
	uint32_t captValue = CTIMER_GetCaptureValue(CTIMER0, kCTIMER_Capture_1);
	uint32_t temp = 0u;
	if(captValue > lastRightCapture)
		temp = captValue - lastRightCapture;
	else
		temp = captValue + ((uint32_t)0xffffffff - lastRightCapture);
	if(rightBufferCurrSize < BUFFERMAXSIZE){
		rightBuffer[rightBufferCurrSize] = temp;
		rightBufferCurrSize++;
		rightBufferIndex++;
	}else {
		rightBuffer[rightBufferIndex] = temp;
		rightBufferIndex++;
	}
	
	if(rightBufferIndex >= BUFFERMAXSIZE)
		rightBufferIndex = 0u;
	
	lastRightCapture = captValue;
	rightTimeSinceLast = rightAverage();
}

//callback for the left opto module
//sets leftTimeSinceLast to the average of the left buffer
void leftWheel_cb(uint32_t flags){
	uint32_t captValue = CTIMER_GetCaptureValue(CTIMER0, kCTIMER_Capture_2);
	uint32_t temp = 0u;
	if(captValue > lastLeftCapture)
		temp = captValue - lastLeftCapture;
	else
		temp = captValue + ((uint32_t)0xffffffff - lastLeftCapture);
	if(leftBufferCurrSize < BUFFERMAXSIZE){
		leftBuffer[leftBufferCurrSize] = temp;
		leftBufferCurrSize++;
		leftBufferIndex++;
	}else {
		leftBuffer[leftBufferIndex] = temp;
		leftBufferIndex++;
	}
	
	if(leftBufferIndex >= BUFFERMAXSIZE)
		leftBufferIndex = 0u;
		
	lastLeftCapture = captValue;
	leftTimeSinceLast = leftAverage();
}