/*
 * Copyright 2016-2023 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    LPC845_Project_UH_tavmero.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "LPC845.h"
#include "fsl_debug_console.h"
#include "string.h"
#include "sys.h"
#include "SysTimer.h"
#include "Dist/UH_HCSR-04.h"
#include "Dist/Measure.h"
#include "letsdothis.h"
#include "fixSpeed.h"
#include "timeBetween.h"

/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

void Init(){
	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();

#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
	/* Init FSL debug console. */
	BOARD_InitDebugConsole();
#endif

	/* Set systick reload value to generate 1ms interrupt */
	if (SysTick_Config(SystemCoreClock / 1000U)){
		while (1){}
	}
	CTIMER_StartTimer(CTIMER0);
}

/*
 * @brief   Application entry point.
 */

extern volatile uint32_t u32_pwmCounter;//csak bizonyos időnként állítjuk a pwm-et

#if 0
extern volatile uint32_t u32_RWSpeed;//jobb kerék sebessége
extern volatile uint32_t u32_LWSpeed;//bal kerék sebessége
#endif

extern volatile uint32_t rightTimeSinceLast;
extern volatile uint32_t leftTimeSinceLast;

volatile uint8_t isGoing = FALSE;
///////////////////////////////////////////
typedef enum CarState {scanning = 0, going = 1}CarState; //State of the car
extern volatile uint32_t g_systickCounter;

int main(void) {
	Init();

	PRINTF("\r\n\r\n----------FIRST MEASURE-------------\r\n\r\n");
	LED_RED_OFF();
	
	uint8_t goingStartTime = 0;
	const uint8_t GOINGDUR = 10;
	CarState carState = scanning;
	
	resetDistances();
	setDesiredEdgeDelay(9000, 9000);


#if 0
	//int16_t i16_speedPercentRight = 35u;//kiadott pwm érték a jobb kerékhez
	//int16_t i16_speedPercentLeft = 35u;//kiadott pwm érték a bal kerékhez
	//const int16_t desiredSpeed = 40u;//a kerekek átlagos sebessége ennyi legyen
	uint16_t u16_distance;
	uint16_t u16_lastValidDistance = 0;
	uint8_t b_valid = FALSE;
	uint8_t b_canWrite = FALSE;
	uint32_t u32_whichLeft = 1u;
	uint32_t u32_whichRight = 2u;
	
	uint8_t up = TRUE;
	uint8_t lSpeed = 70u;
	uint8_t rSpeed = 70u;
	uint32_t u32_lastTime = 0u;
	const uint32_t speedChangeCD = 5000u;
	/* Enter an infinite loop, just incrementing a counter. */
	uint32_t desiredRisingEdgeRate[] = {8500, 8500};//left, right
	while(1){
	//PWM
		if(u32_pwmCounter == 0U){
			u32_pwmCounter = PWM_CHANGE_TIME;
			uint32_t currentRisingEdgeRate[] = {leftTimeSinceLast, rightTimeSinceLast};
			uint8_t *speeds[] = {&lSpeed, &rSpeed};
			corregateSpeed(desiredRisingEdgeRate, currentRisingEdgeRate, speeds);
			
			if(u16_lastValidDistance > 30u){//semmi se legyen közelebb 30 cm-nél

				//kezdetleges sebesség állítás
				/*if(u32_RWSpeed > u32_LWSpeed){
					PRINTF("Right is faster\r\n");
					i16_speedPercentLeft++;
					i16_speedPercentRight--;
				}
				if(u32_LWSpeed > u32_RWSpeed){
					PRINTF("Left is faster\r\n");
					i16_speedPercentLeft--;
					i16_speedPercentRight++;
				}
				int16_t atlag = (i16_speedPercentLeft + i16_speedPercentRight) / 2;
				if(atlag > desiredSpeed){
					i16_speedPercentLeft -= atlag - desiredSpeed;
					i16_speedPercentRight -= atlag - desiredSpeed;
				}
				else if(atlag < desiredSpeed){
					i16_speedPercentLeft += desiredSpeed - atlag;
					i16_speedPercentRight += desiredSpeed - atlag;
				}*/
				
				PRINTF("LEFTTime: %d RIGHTTime: %d\r\nLeft: %d, Right: %d \r\n", leftTimeSinceLast, rightTimeSinceLast, lSpeed, rSpeed);

				SCTIMER_UpdatePwmDutycycle(SCT0, u32_whichLeft, (uint8_t) lSpeed, SCT0_pwmEvent[u32_whichLeft]);
				SCTIMER_UpdatePwmDutycycle(SCT0, u32_whichRight, (uint8_t) rSpeed, SCT0_pwmEvent[u32_whichRight]);
			}else
			if(u16_lastValidDistance < 30u){// ha közelebb van mint 30 akkor álljunk meg
				SCTIMER_UpdatePwmDutycycle(SCT0, u32_whichLeft, 0u, SCT0_pwmEvent[u32_whichLeft]);
				SCTIMER_UpdatePwmDutycycle(SCT0, u32_whichRight, 0u, SCT0_pwmEvent[u32_whichRight]);
			}

		}

	//Megmérjük a távolságot
		Measure(&u16_distance, &b_valid, &b_canWrite);
		if(b_canWrite){
			if(b_valid){
				u16_lastValidDistance = u16_distance;
				PRINTF("%u cm\r\n", u16_distance);   //kiíratás az USBUART-on
				//LED_RED_TOGGLE() ; 		// LED villogtatás
			}else{
				u16_lastValidDistance = 150u;
				PRINTF("No echo!\r\n");
			}
		}
		b_canWrite = FALSE;//reseteljük az értékeket
		b_valid = FALSE;
	}

#endif	
#if 1
	while(1){
		switch(carState){
			case scanning:{
				if(canGo()){
					carState = going;
					goingStartTime = g_systickCounter;
				}
				break;
			}
			case going:{
				if(between(g_systickCounter, goingStartTime, goingStartTime + GOINGDUR)){
					regulateSpeed();
				}
				else{
					stop();
					carState = scanning;
				}
				break;
			}
		}
	}
#endif	
#if 0
	while(1){
	for(int i = 0; i < 30; i++)
		rotateOne(FALSE);
		
	for(int i = 0; i < 30; i++)
		rotateOne(TRUE);
	}
#endif
#if 0
	uint16_t u16_distance;
	uint16_t u16_lastValidDistance = 0;
	uint8_t b_valid = FALSE;
	uint8_t b_canWrite = FALSE;
	while(1){
		if(u32_pwmCounter == 0U){
			u32_pwmCounter = PWM_CHANGE_TIME;
			if(u16_lastValidDistance > 40u){
				SCTIMER_UpdatePwmDutycycle(SCT0, 1, (uint8_t) 80, SCT0_pwmEvent[1]);
				SCTIMER_UpdatePwmDutycycle(SCT0, 2, (uint8_t) 80, SCT0_pwmEvent[2]);
			}else{
				SCTIMER_UpdatePwmDutycycle(SCT0, 1, (uint8_t) 0, SCT0_pwmEvent[1]);
				SCTIMER_UpdatePwmDutycycle(SCT0, 2, (uint8_t) 0, SCT0_pwmEvent[2]);
			}
			PRINTF("LEFT: %d RIGHT: %d\r\n", leftTimeSinceLast, rightTimeSinceLast);
		}
		Measure(&u16_distance, &b_valid, &b_canWrite);
		if(b_canWrite){
			if(b_valid){
				u16_lastValidDistance = u16_distance;
				//LED_RED_TOGGLE() ; 		// LED villogtatás
			}else{
				u16_lastValidDistance = 150u;
			}
		}
		b_canWrite = FALSE;//reseteljük az értékeket
		b_valid = FALSE;
	}
#endif
	return 0;
}
