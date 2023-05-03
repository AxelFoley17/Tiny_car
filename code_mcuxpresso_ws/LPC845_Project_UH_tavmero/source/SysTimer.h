/*
 * SysTimer.h
 *
 *  Created on: 2023. febr. 27.
 *      Author: benes
 */

#ifndef SYSTIMER_H_
#define SYSTIMER_H_
#include "stdint.h"
//user definition of used timeouts (max. 4)
#define timeout1
#define timeout2
#define timeout3
#define timeout4

//user definition of SysTimer cycle time [ms]
//#define SysTimCyc 5
//time = N*SysTimCyc

typedef struct Timeout
{
    unsigned char Started;
    uint8_t Tou;
    uint16_t cnt;
}Timeout;
/*
//static Timeout Tou1;
extern void SysTimer_Tou(Timeout * tou);
extern void StartTimeout(Timeout * tou, uint16 time);
extern uint8 TimeotDone(Timeout * tou);
*/
#ifdef timeout1
//extern void SysTimer_Tou1();
extern void StartTimeout1(uint16_t time);
extern uint8_t TimeoutDone1(void);
#endif

#ifdef timeout2
//extern void SysTimer_Tou2();
extern void StartTimeout2(uint16_t time);
extern uint8_t TimeoutDone2(void);
#endif

#ifdef timeout3
//extern void SysTimer_Tou3();
extern void StartTimeout3(uint16_t time);
extern uint8_t TimeoutDone3(void);
#endif

#ifdef timeout4
//extern void SysTimer_Tou4();
extern void StartTimeout4(uint16_t time);
extern uint8_t TimeoutDone4(void);
#endif

extern void TimeoutExtension(void);



#endif /* SYSTIMER_H_ */
