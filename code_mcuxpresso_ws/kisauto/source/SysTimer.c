/*
 * SysTimer.c
 *
 *  Created on: 2023. febr. 27.
 *      Author: benes
 */
#include "stdint.h"
#include "sys.h"
#include "math.h"
#include "SysTimer.h"

#ifdef timeout1
static Timeout Tou1;
#endif
#ifdef timeout2
static Timeout Tou2;
#endif
#ifdef timeout3
static Timeout Tou3;
#endif
#ifdef timeout4
static Timeout Tou4;
#endif

void SysTimer_Tou(Timeout * tou)
{
    if(tou -> Started)
    {
        if(tou->cnt > 0)
        {
            tou->cnt--;
        }
        else
        {
            tou->Tou = TRUE;
        }
    }
}

void StartTimeout(Timeout * tou, uint16_t time)
{
    tou->Tou = FALSE;
    tou->cnt = (uint16_t)(ceilf((float)time/SysTimCyc));
    tou->Started = TRUE;
    //tou->cnt = time;
}

uint8_t TimeoutDone(Timeout * tou)
{
    uint8_t TimOut = FALSE;
    if(tou->Started)
    {
        if(tou->Tou)
        {
            //CYGlobalIntDisable;
            tou->Started = FALSE;
            TimOut = TRUE;
            //CYGlobalIntEnable;
        }
    }
    return(TimOut);
}

//-----------------------------------
#ifdef timeout1
void SysTimer_Tou1()
{
    SysTimer_Tou(&Tou1);
}

void StartTimeout1(uint16_t time)
{
     StartTimeout(&Tou1, time);
}

uint8_t TimeoutDone1(void)
{
   return(TimeoutDone(&Tou1));
}
#endif
//-----------------------------------
#ifdef timeout2
void SysTimer_Tou2()
{
    SysTimer_Tou(&Tou2);
}

void StartTimeout2(uint16_t time)
{
     StartTimeout(&Tou2, time);
}

uint8_t TimeoutDone2(void)
{
   return(TimeoutDone(&Tou2));
}
#endif
//-----------------------------------
#ifdef timeout3
void SysTimer_Tou3()
{
    SysTimer_Tou(&Tou3);
}

void StartTimeout3(uint16_t time)
{
     StartTimeout(&Tou3, time);
}

uint8_t TimeoutDone3(void)
{
   return(TimeoutDone(&Tou3));
}
#endif
//-----------------------------------
#ifdef timeout4
void SysTimer_Tou4()
{
    SysTimer_Tou(&Tou4);
}

void StartTimeout4(uint16_t time)
{
     StartTimeout(&Tou4, time);
}

uint8_t TimeoutDone4(void)
{
   return(TimeoutDone(&Tou4));
}
#endif

void TimeoutExtension(void)
{
    #ifdef timeout1
    SysTimer_Tou1();
    #endif
    #ifdef timeout2
    SysTimer_Tou2();
    #endif
    #ifdef timeout3
    SysTimer_Tou3();
    #endif
    #ifdef timeout4
    SysTimer_Tou4();
    #endif
}


