/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f1xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_it.h"
#include "motor_comm.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "task.h"
#include "button.h"
#include "pots.h"
#include "sensor.h"
#include "alarm.h"
#include "support.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
 
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_adc1;
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern TIM_HandleTypeDef htim3;
extern DMA_HandleTypeDef hdma_usart3_tx;
/* USER CODE BEGIN EV */
extern uint32_t raw_sensor[4];
extern UART_HandleTypeDef huart3;
/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M3 Processor Interruption and Exception Handlers          */ 
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    for (int i = 0; i < 2; i++)
    {
      TIM4_delayus(50000);
    }
    HAL_GPIO_TogglePin(MOTOR_ENABLE_GPIO_Port, MOTOR_ENABLE_Pin);
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    for (int i = 0; i < 2; i++)
    {
      TIM4_delayus(50000);
    }
    HAL_GPIO_TogglePin(MOTOR_ENABLE_GPIO_Port, MOTOR_ENABLE_Pin);
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
      for (int i = 0; i < 2; i++)
      {
        TIM4_delayus(50000);
      }
      HAL_GPIO_TogglePin(MOTOR_ENABLE_GPIO_Port, MOTOR_ENABLE_Pin);

    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
      for (int i = 0; i < 2; i++)
      {
        TIM4_delayus(50000);
      }
      HAL_GPIO_TogglePin(MOTOR_ENABLE_GPIO_Port, MOTOR_ENABLE_Pin);

    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */
  /* USER CODE END SysTick_IRQn 0 */
  /* USER CODE BEGIN SysTick_IRQn 1 */
  Task_IncreaseTick();
  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 channel1 global interrupt.
  */
void DMA1_Channel1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */
  uint32_t source_it = hdma_adc1.Instance->CCR;
  /* USER CODE END DMA1_Channel1_IRQn 0 */
  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */
  if ((source_it & DMA_IT_HT) != RESET)
  {
    __HAL_DMA_DISABLE_IT(&hdma_adc1, DMA_IT_HT);
    __HAL_DMA_CLEAR_FLAG(&hdma_adc1, __HAL_DMA_GET_HT_FLAG_INDEX(&hdma_adc1));
  }
  else if ((source_it & DMA_IT_TC) != RESET)
  {
    Sensor_UpdateValue(raw_sensor);
    hdma_adc1.State = HAL_DMA_STATE_READY;
    __HAL_DMA_DISABLE_IT(&hdma_adc1, DMA_IT_TC | DMA_IT_TE);
    __HAL_DMA_CLEAR_FLAG(&hdma_adc1, __HAL_DMA_GET_TC_FLAG_INDEX(&hdma_adc1));
    __HAL_UNLOCK(&hdma_adc1);
    // HAL_ADC_Stop_DMA(&hadc1);
  }
  /* USER CODE END DMA1_Channel1_IRQn 1 */
}

/**
  * @brief This function handles DMA1 channel2 global interrupt.
  */
void DMA1_Channel2_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel2_IRQn 0 */
  uint32_t source_it = hdma_usart3_tx.Instance->CCR;
  /* USER CODE END DMA1_Channel2_IRQn 0 */
  /* USER CODE BEGIN DMA1_Channel2_IRQn 1 */
  if ((source_it & DMA_IT_HT) != RESET)
  {
    __HAL_DMA_DISABLE_IT(&hdma_usart3_tx, DMA_IT_HT);
    __HAL_DMA_CLEAR_FLAG(&hdma_usart3_tx, __HAL_DMA_GET_HT_FLAG_INDEX(&hdma_usart3_tx));
  }
  else if ((source_it & DMA_IT_TC) != RESET)
  {
    hdma_usart3_tx.State = HAL_DMA_STATE_READY;
    __HAL_DMA_DISABLE_IT(&hdma_usart3_tx, DMA_IT_TC | DMA_IT_TE);
    __HAL_DMA_CLEAR_FLAG(&hdma_usart3_tx, __HAL_DMA_GET_TC_FLAG_INDEX(&hdma_usart3_tx));
    __HAL_UNLOCK(&hdma_usart3_tx);
    HAL_UART_DMAStop(&huart3);
    MC_SetComplete();
  }
  /* USER CODE END DMA1_Channel2_IRQn 1 */
}

/**
  * @brief This function handles ADC1 and ADC2 global interrupts.
  */
void ADC1_2_IRQHandler(void)
{
  /* USER CODE BEGIN ADC1_2_IRQn 0 */
  int channel_index;
  uint32_t raw[4];

  if (__HAL_ADC_GET_IT_SOURCE(&hadc2, ADC_IT_JEOC))
  {
    for (channel_index = 0; channel_index < 4; channel_index++)
      raw[channel_index] = HAL_ADCEx_InjectedGetValue(&hadc2, ADC_INJECTED_RANK_1 + channel_index);

    POT_UpdateKnobValue(raw);
    __HAL_ADC_DISABLE_IT(&hadc2, ADC_IT_JEOC);
    __HAL_ADC_CLEAR_FLAG(&hadc2, (ADC_FLAG_JSTRT | ADC_FLAG_JEOC));
    CLEAR_BIT(hadc2.State, HAL_ADC_STATE_INJ_BUSY);
    HAL_ADCEx_InjectedStop_IT(&hadc2);
  }
  /* USER CODE END ADC1_2_IRQn 0 */
  /* USER CODE BEGIN ADC1_2_IRQn 1 */

  /* USER CODE END ADC1_2_IRQn 1 */
}

/**
  * @brief This function handles EXTI line[9:5] interrupts.
  */
void EXTI9_5_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI9_5_IRQn 0 */
  if (__HAL_GPIO_EXTI_GET_IT(MODE_Pin) != RESET)
  {
    Button_SetEvent(SWITCH_MODE_SELECTION, HAL_GPIO_ReadPin(MODE_GPIO_Port, MODE_Pin) + LOW_LEVEL);
  }

  if (__HAL_GPIO_EXTI_GET_IT(LIMIT_SWITCH_Pin) != RESET)
  {
    Button_SetEvent(SWITCH_LIMIT, HAL_GPIO_ReadPin(MODE_GPIO_Port, LIMIT_SWITCH_Pin) + LOW_LEVEL);
  }

  if (__HAL_GPIO_EXTI_GET_IT(SILENCE_Pin) != RESET)
  {
    Button_SetEvent(BUTTON_SILENCE, EDGE_DETECTED);
  }

  if (__HAL_GPIO_EXTI_GET_IT(CONFIRM_Pin) != RESET)
  {
    Button_SetEvent(BUTTON_CONFIRM, EDGE_DETECTED);
  }
  
  if (__HAL_GPIO_EXTI_GET_IT(MOTOR_INT_Pin) != RESET)
  {
    Button_SetEvent(MOTOR_INT, HAL_GPIO_ReadPin(MOTOR_INT_GPIO_Port, MOTOR_INT_Pin) + LOW_LEVEL);
  }

  __HAL_GPIO_EXTI_CLEAR_IT(MODE_Pin | LIMIT_SWITCH_Pin | SILENCE_Pin | CONFIRM_Pin | MOTOR_INT_Pin);
  /* USER CODE END EXTI9_5_IRQn 0 */
  /* USER CODE BEGIN EXTI9_5_IRQn 1 */

  /* USER CODE END EXTI9_5_IRQn 1 */
}

/**
  * @brief This function handles TIM3 global interrupt.
  */
void TIM3_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */
  __HAL_TIM_CLEAR_IT(&htim3, TIM_IT_UPDATE);
  /* USER CODE END TIM3_IRQn 0 */
  /* USER CODE BEGIN TIM3_IRQn 1 */

  /* USER CODE END TIM3_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
