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
#include <scanning.h>
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "LPC845.h"
#include "fsl_debug_console.h"
#include "string.h"
#include "sys/sys.h"
#include "sys/SysTimer.h"
#include "Dist/UH_HCSR-04.h"
#include "fixSpeed/fixSpeed.h"
#include "timeBetween/timeBetween.h"

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
	while(1){
		switch(carState){
			case scanning:{//scanning for the good direction
				if(canGo()){//we found it
					carState = going;
					goingStartTime = g_systickCounter;
				}
				break;
			}
			case going:{//going forward for a set amount of time
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
	return 0;
}
