/*
 * sys.c
 *
 *  Created on: 2023. febr. 27.
 *      Author: benes
 */
#include "SysTimer.h"
volatile uint32_t u32_pwmCounter = PWM_CHANGE_TIME;
volatile uint32_t g_systickCounter = 0U;
void SysTick_Handler(void)
{
	if (u32_pwmCounter != 0U){
		u32_pwmCounter--;
	}
    //if (g_systickCounter != 0U)
    //{
        g_systickCounter++;
    //}
    TimeoutExtension();//számoljuk az időt a távolság méréshez
}

