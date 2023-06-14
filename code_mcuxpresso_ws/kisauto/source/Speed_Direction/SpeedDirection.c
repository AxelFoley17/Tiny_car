#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "LPC845.h"
#include "clock_config.h"
#include "sys.h"

static uint8_t u8_desiredRightSpeed = 0u;
static uint8_t u8_desiredLeftSpeed = 0u;

static float diff = 0.0f;

static uint8_t u8_actualRightSpeed = 0u;
static uint8_t u8_actualLeftSpeed = 0u;

void SetSpeed(uint8_t u8_right, uint8_t u8_left){
	u8_actualRightSpeed = u8_desiredRightSpeed = u8_right;
	u8_actualLeftSpeed = u8_desiredLeftSpeed = u8_left;
	diff = (float)u8_desiredRightSpeed / (float)u8_desiredLeftSpeed;
}

static uint8_t b_rightForward = TRUE;
static uint8_t b_leftForward = TRUE;

void SetDirection(uint8_t b_right, uint8_t b_left){
	b_rightForward = b_rightForward;
	b_leftForward = b_leftForward;
}

extern volatile uint32_t u32_RWSpeed;
extern volatile uint32_t u32_LWSpeed;

/// TODO cooldown after setspeed for currentspeed
void CorrectSpeed(){
	float wheelDiff = 0.0f;
	wheelDiff = abs(((float)u32_RWSpeed / (float)u32_LWSpeed) - diff);
	if(wheelDiff > 0.01f){
		u8_actualRightSpeed = (u8_actualRightSpeed * wheelDiff + u8_actualRightSpeed)/2;
		u8_actualLeftSpeed = (u8_actualLeftSpeed * wheelDiff + u8_actualLeftSpeed)/2;
	}
}

void CommitSpeed(){
	uint8_t left = 0u;
	if(!b_leftForward){
		left = 1u;
	}
	uint8_t right = 2u;
	if(!b_rightForward){
		right = 3u;
	}
	SCTIMER_UpdatePwmDutycycle(SCT0, left, u8_actualLeftSpeed, SCT0_pwmEvent[left]);
	SCTIMER_UpdatePwmDutycycle(SCT0, right, u8_actualRightSpeed, SCT0_pwmEvent[right]);
}