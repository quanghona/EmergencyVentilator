/**
* @file task_pots.c
* @brief Trigger POT reading task
*
* @date 2020 Apr 07
* @version 1
* @author Ly Hon Quang
* Group: Ventus code team
*
*/

#include "task.h"
#include "main.h"

/*********************************Variables***********************************/
extern ADC_HandleTypeDef hadc2;

/****************************Function definitions*****************************/

inline void Task_ReadPOTs(void)
{
    HAL_ADCEx_InjectedStart_IT(&hadc2);
}

/* End of task_pots.c */
