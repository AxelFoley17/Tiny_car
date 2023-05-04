/*
 * UH_HCSR-04.c
 *
 *  Created on: 2023. febr. 26.
 *      Author: benes
 */
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "LPC845.h"
#include "sys.h"
#include "SysTimer.h"
#include "UH_HCSR-04.h"
#include "fsl_debug_console.h"

// a trigger impulzus szélessége 1-2 msec (specifikáció szetrint > 10us)
#define TRG_WITH 2
//#define Timer_UH_ALL_IT_DIS 0x00
//Maximális mérhető távolság
#define DistanceMax 400
#define us_pro_cm (float) 55.6

static DistMeasResult DistResult; // a static változóra garantált, hogy megmarad az értéke
static uint32_t DistMeasRdy = 0;
//static volatile uint32_t pulse_done, result_done, pulse_count;
static volatile uint32_t cap0, cap1;
volatile static uint8_t cnt = 0;
void ctimer0_callback(uint32_t flags)
{	if(cnt == 0)
	{
   		cap0 = CTIMER_GetCaptureValue(CTIMER0, kCTIMER_Capture_0);
   		cnt++;
	}
	else
	{
   	    cap1 = CTIMER_GetCaptureValue(CTIMER0, kCTIMER_Capture_0);
   	    if(cap1 >= cap0)
   	    {
   	    	DistResult.value = cap1 - cap0;
   	    }
   	    else
   	    {
   	    	DistResult.value = cap1 + ((uint32_t)0xffffffff - cap0);
   	    }
   	    DistMeasRdy = TRUE;
   	    cnt = 0;
	}
}

uint8_t bumm1 = FALSE;
uint8_t bumm2 = FALSE;

volatile uint32_t u32_RWSpeed = 0u;
volatile uint32_t u32_LWSpeed = 0u;
volatile uint32_t u32_lastR = 0u;
volatile uint32_t u32_lastL = 0u;

const float wheelPerimeter = 1.065f; //cm, not very exact
const uint32_t wholes = 20u;
const uint32_t freq = 30u * 1000000u;
const uint32_t cmToM = 100u;

//const float combinedNum = (float)freq * wheelPerimeter / (float)wholes / (float)cmToM;
const float combinedNum = (float)freq * wheelPerimeter / (float)wholes;


void rightWheel_cb(uint32_t flags){
	uint32_t u32_captValue = CTIMER_GetCaptureValue(CTIMER0, kCTIMER_Capture_1);
	uint32_t diff = u32_captValue - u32_lastR;
	u32_lastR = u32_captValue;
	u32_RWSpeed = (volatile uint32_t)(combinedNum / (float) diff);
}
void leftWheel_cb(uint32_t flags){
	uint32_t u32_captValue = CTIMER_GetCaptureValue(CTIMER0, kCTIMER_Capture_2);
	uint32_t diff = u32_captValue - u32_lastL;
	u32_lastL = u32_captValue;
	u32_LWSpeed = (volatile uint32_t)(combinedNum / (float) diff);
}


#define TRG_Time 1
#define EchoTimeout 20 // > 1000*4/330m/s

void StartUH_TRG(void)
{
	PIO0_17_ON(); //Trigger ON
	StartTimeout4(TRG_Time); //start 1ms timeout
}

#define UH_start 1U
#define UH_TRG_wait 2U
#define UH_ECHO_wait 3U
#define UH_stop 4U

DistMeasResult UH_measure(uint8_t rst)
{
    static uint16_t UH_state;

    if(rst)
    {
        UH_state = UH_start;
        DistResult.ready = FALSE;
        DistResult.valid = TRUE;
        DistMeasRdy = FALSE;
    }
    switch(UH_state)
    {
    case UH_start:
        StartUH_TRG();
        UH_state = UH_TRG_wait;
        break;
    case UH_TRG_wait:
        if(TimeoutDone4())
        {
        	PIO0_17_OFF();
        	UH_state = UH_ECHO_wait;
            StartTimeout4(EchoTimeout);

        }
        break;
   case UH_ECHO_wait:
	    if(TimeoutDone4())
	    {
	    	DistMeasRdy = TRUE;
	    	DistResult.valid = FALSE;
	    }
        if(DistMeasRdy)
        {
            DistMeasRdy = FALSE;
            if(DistResult.valid)
            {
                DistResult.value = (uint32_t)((float)DistResult.value/us_pro_cm);  // konvertálás cm-be
                if(DistResult.value > DistanceMax)
                {
                    DistResult.valid = FALSE;
                }
            }
            DistResult.ready= TRUE;
            UH_state = UH_stop;
        }
        break;
    case UH_stop:           //ebből az állapotból az rst=1 (új mérés indítás) hozza ki.
        break;
    }
    return(DistResult);
}

