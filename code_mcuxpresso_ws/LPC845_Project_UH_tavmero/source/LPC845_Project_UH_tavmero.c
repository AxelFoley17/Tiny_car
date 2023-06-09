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

int main(void) {
	uint16_t ui16_distance;
	uint8_t b_valid = FALSE;
	uint8_t b_canWrite = FALSE;
	
	Init();

	PRINTF("\r\n\r\n----------FIRST MEASURE-------------\r\n\r\n");
	LED_RED_OFF();
	FirstMeasure();

	/* Enter an infinite loop, just incrementing a counter. */
	while(1){
		Measure(&ui16_distance, &b_valid, &b_canWrite);
		if(b_canWrite){
			if(b_valid){
				PRINTF("%u cm\r\n", ui16_distance);   //kiíratás az USBUART-on
				LED_RED_TOGGLE() ; 		// LED villogtatás
			}else{
				PRINTF("No echo!\r\n");
			}
		}
		b_canWrite = FALSE;
		b_valid = FALSE;
	}
	return 0;
}
