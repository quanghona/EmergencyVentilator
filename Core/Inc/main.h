/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LCD_DB4_Pin GPIO_PIN_0
#define LCD_DB4_GPIO_Port GPIOC
#define LCD_DB5_Pin GPIO_PIN_1
#define LCD_DB5_GPIO_Port GPIOC
#define LCD_DB6_Pin GPIO_PIN_2
#define LCD_DB6_GPIO_Port GPIOC
#define LCD_DB7_Pin GPIO_PIN_3
#define LCD_DB7_GPIO_Port GPIOC
#define ENCODER_A_Pin GPIO_PIN_0
#define ENCODER_A_GPIO_Port GPIOA
#define ENCODER_B_Pin GPIO_PIN_1
#define ENCODER_B_GPIO_Port GPIOA
#define PRESSURE_Pin GPIO_PIN_2
#define PRESSURE_GPIO_Port GPIOA
#define ANGLE_Pin GPIO_PIN_3
#define ANGLE_GPIO_Port GPIOA
#define MOTOR_RIS_Pin GPIO_PIN_4
#define MOTOR_RIS_GPIO_Port GPIOA
#define MOTOR_LIS_Pin GPIO_PIN_5
#define MOTOR_LIS_GPIO_Port GPIOA
#define BPM_Pin GPIO_PIN_6
#define BPM_GPIO_Port GPIOA
#define TIDAL_Pin GPIO_PIN_7
#define TIDAL_GPIO_Port GPIOA
#define LCD_RS_Pin GPIO_PIN_4
#define LCD_RS_GPIO_Port GPIOC
#define LCD_E_Pin GPIO_PIN_5
#define LCD_E_GPIO_Port GPIOC
#define IE_Pin GPIO_PIN_0
#define IE_GPIO_Port GPIOB
#define THRESHOLD_Pin GPIO_PIN_1
#define THRESHOLD_GPIO_Port GPIOB
#define MOTOR_N_Pin GPIO_PIN_9
#define MOTOR_N_GPIO_Port GPIOA
#define LCD_RW_Pin GPIO_PIN_6
#define LCD_RW_GPIO_Port GPIOC
#define MOTOR_P_Pin GPIO_PIN_8
#define MOTOR_P_GPIO_Port GPIOA
#define LED_Pin GPIO_PIN_12
#define LED_GPIO_Port GPIOC
#define MOTOR_ENABLE_Pin GPIO_PIN_2
#define MOTOR_ENABLE_GPIO_Port GPIOD
#define CONFIRM_Pin GPIO_PIN_5
#define CONFIRM_GPIO_Port GPIOB
#define CONFIRM_EXTI_IRQn EXTI9_5_IRQn
#define MODE_Pin GPIO_PIN_6
#define MODE_GPIO_Port GPIOB
#define MODE_EXTI_IRQn EXTI9_5_IRQn
#define LIMIT_SWITCH_Pin GPIO_PIN_7
#define LIMIT_SWITCH_GPIO_Port GPIOB
#define LIMIT_SWITCH_EXTI_IRQn EXTI9_5_IRQn
#define SILENCE_Pin GPIO_PIN_8
#define SILENCE_GPIO_Port GPIOB
#define SILENCE_EXTI_IRQn EXTI9_5_IRQn
#define ALARM_Pin GPIO_PIN_9
#define ALARM_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define bool uint32_t
#define true 1
#define false 0

#define SYSTICK_FREQUENCY                       1000

/* Task */
#define TASK_ALARM_FREQUENCY                    10
#define TASK_LCD_FREQUENCY                      21
#define TASK_READ_POT_FREQUENCY                 40
#define TASK_BUTTON_FREQUENCY                   20
#define TASK_CHECK_ERROR_FREQUENCY              20
#define TASK_DISPLAY_MESSAGE_FREQUENCY          1

/* Timeout. Unit: tick of Task alarm */
#define SILENCE_TIMEOUT                         600.0
#define PRESSURE_OUTRANGE_TIMEOUT               0.0
#define ELECTRICAL_FAULT_TIMEOUT                0.0
#define HOMING_FAULT_TIMEOUT                    0.0
#define POT_CHANGE_TIMEOUT                      50.0
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
