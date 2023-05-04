/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef _PERIPHERALS_H_
#define _PERIPHERALS_H_

/***********************************************************************************************************************
 * Included files
 **********************************************************************************************************************/
#include "fsl_common.h"
#include "fsl_ctimer.h"
#include "fsl_clock.h"
#include "fsl_sctimer.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/
/* Definitions for BOARD_InitPeripherals functional group */
/* Definition of peripheral ID */
#define CTIMER0_PERIPHERAL CTIMER0
/* Timer tick frequency in Hz (input frequency of the timer) */
#define CTIMER0_TICK_FREQ 1000000UL
/* Timer tick period in ns (input period of the timer) */
#define CTIMER0_TICK_PERIOD 1000UL
/* Definition of PWM period channel. */
#define CTIMER0_PWM_PERIOD_CH kCTIMER_Match_0
/* Definition of channel 0 ID */
#define CTIMER0_CAPTURE_0_CHANNEL kCTIMER_Capture_0
/* Definition of channel 1 ID */
#define CTIMER0_CAPTURE_1_CHANNEL kCTIMER_Capture_1
/* Definition of channel 2 ID */
#define CTIMER0_CAPTURE_2_CHANNEL kCTIMER_Capture_2
/* CTIMER0 interrupt vector ID (number). */
#define CTIMER0_TIMER_IRQN CTIMER0_IRQn
/* CTIMER0 interrupt vector priority. */
#define CTIMER0_TIMER_IRQ_PRIORITY 1
/* BOARD_InitPeripherals defines for SCT0 */
/* Definition of peripheral ID */
#define SCT0_PERIPHERAL SCT0
/* Definition of clock source frequency */
#define SCT0_CLOCK_FREQ 30000000UL
/* SCT0 interrupt vector ID (number). */
#define SCT0_IRQN SCT0_IRQn
/* SCT0 interrupt handler identifier. */
#define SCT0_IRQHANDLER SCT0_IRQHandler

/***********************************************************************************************************************
 * Global variables
 **********************************************************************************************************************/
extern const ctimer_config_t CTIMER0_config;
extern const sctimer_config_t SCT0_initConfig;
extern const sctimer_pwm_signal_param_t SCT0_pwmSignalsConfig[4];
extern uint32_t SCT0_pwmEvent[4];

/***********************************************************************************************************************
 * Callback functions
 **********************************************************************************************************************/
/* Multiple callback functions declaration */
extern void ctimer0_callback(uint32_t flags);
extern void ctimer1_callback(uint32_t flags);
extern void ctimer2_callback(uint32_t flags);
extern ctimer_callback_t CTIMER0_callback[];

/***********************************************************************************************************************
 * Initialization functions
 **********************************************************************************************************************/

void BOARD_InitPeripherals(void);

/***********************************************************************************************************************
 * BOARD_InitBootPeripherals function
 **********************************************************************************************************************/
void BOARD_InitBootPeripherals(void);

#if defined(__cplusplus)
}
#endif

#endif /* _PERIPHERALS_H_ */