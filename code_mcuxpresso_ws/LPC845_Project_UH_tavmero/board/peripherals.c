/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Peripherals v12.0
processor: LPC845
package_id: LPC845M301JBD48
mcu_data: ksdk2_0
processor_version: 13.0.1
board: LPC845BREAKOUT
functionalGroups:
- name: BOARD_InitPeripherals
  UUID: a1bf1abb-ba15-4a89-8c58-09436b4ba743
  called_from_default_init: true
  selectedCore: core0
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/

/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
component:
- type: 'system'
- type_id: 'system'
- global_system_definitions:
  - user_definitions: ''
  - user_includes: ''
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/

/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
component:
- type: 'uart_cmsis_common'
- type_id: 'uart_cmsis_common_9cb8e302497aa696fdbb5a4fd622c2a8'
- global_USART_CMSIS_common:
  - quick_selection: 'default'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/

/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
component:
- type: 'gpio_adapter_common'
- type_id: 'gpio_adapter_common_57579b9ac814fe26bf95df0a384c36b6'
- global_gpio_adapter_common:
  - quick_selection: 'default'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */

/***********************************************************************************************************************
 * Included files
 **********************************************************************************************************************/
#include "peripherals.h"

/***********************************************************************************************************************
 * BOARD_InitPeripherals functional group
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * CTIMER0 initialization code
 **********************************************************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'CTIMER0'
- type: 'ctimer'
- mode: 'Capture_Match'
- custom_name_enabled: 'false'
- type_id: 'ctimer_44573e4bbd77c18d33bceb2e7900a074'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'CTIMER0'
- config_sets:
  - fsl_ctimer:
    - ctimerConfig:
      - mode: 'kCTIMER_TimerMode'
      - clockSource: 'FunctionClock'
      - clockSourceFreq: 'ClocksTool_DefaultInit'
      - timerPrescaler: '30'
    - EnableTimerInInit: 'false'
    - matchChannels: []
    - captureChannels:
      - 0:
        - captureChannelPrefixId: 'Capture_0'
        - captureChannel: 'kCTIMER_Capture_0'
        - captureEdge: 'kCTIMER_Capture_BothEdge'
        - enableInterrupt: 'true'
      - 1:
        - captureChannelPrefixId: 'Capture_1'
        - captureChannel: 'kCTIMER_Capture_1'
        - captureEdge: 'kCTIMER_Capture_BothEdge'
        - enableInterrupt: 'true'
      - 2:
        - captureChannelPrefixId: 'Capture_2'
        - captureChannel: 'kCTIMER_Capture_2'
        - captureEdge: 'kCTIMER_Capture_BothEdge'
        - enableInterrupt: 'true'
    - interruptCallbackConfig:
      - interrupt:
        - IRQn: 'CTIMER0_IRQn'
        - enable_priority: 'true'
        - priority: '1'
      - callback: 'kCTIMER_MultipleCallback'
      - match0Callback: ''
      - match1Callback: ''
      - match2Callback: ''
      - match3Callback: ''
      - capture0Callback: 'ctimer0_callback'
      - capture1Callback: 'ctimer1_callback'
      - capture2Callback: 'ctimer2_callback'
      - capture3Callback: ''
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */
const ctimer_config_t CTIMER0_config = {
  .mode = kCTIMER_TimerMode,
  .input = kCTIMER_Capture_0,
  .prescale = 29
};
/* Multiple callback functions definition */
ctimer_callback_t CTIMER0_callback[] = {NULL, NULL, NULL, NULL, ctimer0_callback, ctimer1_callback, ctimer2_callback, NULL};

static void CTIMER0_init(void) {
  /* CTIMER0 peripheral initialization */
  CTIMER_Init(CTIMER0_PERIPHERAL, &CTIMER0_config);
  /* Interrupt vector CTIMER0_IRQn priority settings in the NVIC. */
  NVIC_SetPriority(CTIMER0_TIMER_IRQN, CTIMER0_TIMER_IRQ_PRIORITY);
  /* capture channel 0 of CTIMER0 peripheral initialization */
  CTIMER_SetupCapture(CTIMER0_PERIPHERAL, CTIMER0_CAPTURE_0_CHANNEL, kCTIMER_Capture_BothEdge, true);
  /* capture channel 1 of CTIMER0 peripheral initialization */
  CTIMER_SetupCapture(CTIMER0_PERIPHERAL, CTIMER0_CAPTURE_1_CHANNEL, kCTIMER_Capture_BothEdge, true);
  /* capture channel 2 of CTIMER0 peripheral initialization */
  CTIMER_SetupCapture(CTIMER0_PERIPHERAL, CTIMER0_CAPTURE_2_CHANNEL, kCTIMER_Capture_BothEdge, true);
  CTIMER_RegisterCallBack(CTIMER0_PERIPHERAL, CTIMER0_callback, kCTIMER_MultipleCallback);
}

/***********************************************************************************************************************
 * Initialization functions
 **********************************************************************************************************************/
void BOARD_InitPeripherals(void)
{
  /* Initialize components */
  CTIMER0_init();
}

/***********************************************************************************************************************
 * BOARD_InitBootPeripherals function
 **********************************************************************************************************************/
void BOARD_InitBootPeripherals(void)
{
  BOARD_InitPeripherals();
}
