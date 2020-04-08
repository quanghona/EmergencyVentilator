/**
* @file alarm.c
* @brief Control the buzzer
*
* @date 2020 Apr 5
* @version 1
* @author Ly Hon Quang
* Group: Ventus code team
*
*/

#include "alarm.h"
#include "main.h"

/*******************************Definitions***********************************/
#define _Alarm_TOGGLE()          HAL_GPIO_TogglePin(ALARM_GPIO_Port, ALARM_Pin)
#define _Alarm_OFF()             HAL_GPIO_WritePin(ALARM_GPIO_Port, ALARM_Pin, GPIO_PIN_RESET)

/* List of tones. Tone should read only */
const uint8_t DEFAULT_TONE[] = {0x05};    // ON-OFF periodically

/*********************************Variables***********************************/
static AlarmHandle_t handle;

/****************************Function definitions*****************************/

/******************************************************************************
 * @brief Initialize alarm library.
 * @details This function ask the system to give the alarm task frequency
 *
 * @param ui32TickRate tick rate (unit: Hz)
*****************************************************************************/
void Alarm_Init(uint32_t ui32TickRate)
{
    handle.tick_rate = ui32TickRate;
    handle.state = -1;
    handle.tone = DEFAULT_TONE;
    handle.tone_length = sizeof(DEFAULT_TONE);
    handle.enable = false;
}

/******************************************************************************
 * @brief Get alarm handle instance
 *
 * @return AlarmHandle_t* handle instance
*****************************************************************************/
AlarmHandle_t* Alarm_GetHandle()
{
    return &handle;
}

/******************************************************************************
 * @brief Enable alarm
 * @note This function may not turn on the alarm yet due to the silent period.
 * After the silent period has expired, the alarm start its duty with the
 * tone defined in the handle instance
 *
 * @see AlarmHandle_t
*****************************************************************************/
void Alarm_Enable()
{
    handle.enable = true;
}

/******************************************************************************
 * @brief Shut the alarm in an amount of time
 *
 * @param f32Time time to silence (unit second)
*****************************************************************************/
void Alarm_Silence(float f32Time)
{
    handle.silence_time = (uint32_t)(f32Time * handle.tick_rate);   // Convert to alarm tick base
    handle.tick = 0;
    handle.state = -1;
    _Alarm_OFF();
}

/******************************************************************************
 * @brief Completely disable the alarm
 *
*****************************************************************************/
void Alarm_Disable()
{
    handle.enable = false;
    handle.silence_time = 0;
    handle.tick = 0;
    handle.state = -1;
    _Alarm_OFF();
}

/******************************************************************************
 * @brief Set alarm tone
 *
 * @param pui8Tone a constant pointer to tone array
 * @param ui32Len Length of the tone, this param is needed because the length
 * information will be loss when pass tone as pointer to the function
*****************************************************************************/
void Alarm_SetTone(const uint8_t* pui8Tone, uint32_t ui32Len)
{
    handle.tone = pui8Tone;
    handle.tone_length = ui32Len;
}

/******************************************************************************
 * @brief Toggle alarm state
 *
*****************************************************************************/
inline void Alarm_Toggle()
{
    _Alarm_TOGGLE();
}

/******************************************************************************
 * @brief Turn off the alarm
 *
*****************************************************************************/
inline void Alarm_Off()
{
    _Alarm_OFF();
}

/*****************************************************************************/

/* End of alarm.c */
