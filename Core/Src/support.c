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

/******************************************************************************
 * @brief Create micro second delay using TIM4
 * 
 * @param ui16Delay delay time in micro second, maximum 65535
*****************************************************************************/
void TIM4_delayus(uint16_t ui16Delay)
{
    TIM4->CNT = 0;
    TIM4->CR1 |= (TIM_CR1_CEN);     // Enable timer
    while (TIM4->CNT < (uint32_t)ui16Delay);
    TIM4->CR1 &= ~(TIM_CR1_CEN);    // Disable timer
}

/******************************************************************************
 * @brief DIY get string length function
 * 
 * @param s pointer to string
 * @return uint32_t length of the string
*****************************************************************************/
uint32_t strlen(char* s)
{
    uint32_t len = 0;

    while (*(s++) != 0) len++;
    return len;
}

/******************************************************************************
 * @brief Mimic memset function with type uint32_t
 * 
 * @param arr source array
 * @param val value to be set
 * @param len number of member of the array
*****************************************************************************/
void memset(uint32_t* arr, uint32_t val, uint32_t len)
{
    while ((len--) > 0) *(arr++) = val;
}

/******************************************************************************
 * @brief Return absolute value
 * 
 * @param val value
 * @return int32_t absolute value
*****************************************************************************/
int32_t abs(int32_t val)
{
    return val < 0 ? -val : val;
}

/* End of support.c */
