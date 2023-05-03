/*
 * sys.c
 *
 *  Created on: 2023. febr. 27.
 *      Author: benes
 */
#include "SysTimer.h"

volatile static uint32_t g_systickCounter;
void SysTick_Handler(void)
{
    if (g_systickCounter != 0U)
    {
        g_systickCounter--;
    }
    TimeoutExtension();
}

