/**
* @file support.c
* @brief Other support functions
*
* @date 2020 Apr 06
* @version 1
* @author Ly Hon Quang
* Group: Ventus code team
*
*/

#include "support.h"
#include "main.h"

/****************************Function definitions*****************************/

void TIM4_delayus(uint16_t ui16Delay)
{
    TIM4->CNT = 0;
    TIM4->CR1 |= (TIM_CR1_CEN);     // Enable timer
    while (TIM4->CNT < (uint32_t)ui16Delay);
    TIM4->CR1 &= ~(TIM_CR1_CEN);    // Disable timer
}

/* End of support.c */
