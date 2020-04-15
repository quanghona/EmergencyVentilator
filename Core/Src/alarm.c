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
#include "support.h"
#include "hash.h"

/*******************************Definitions***********************************/
#define _Alarm_TOGGLE()         HAL_GPIO_TogglePin(ALARM_GPIO_Port, ALARM_Pin)
#define _Alarm_OFF()            HAL_GPIO_WritePin(ALARM_GPIO_Port, ALARM_Pin, GPIO_PIN_RESET)
#define _LED_TOGGLE()           HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin)
#define _LED_OFF()              HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET)
#define _LED_WRITE(State)       HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, !State)
#define _Alarm_Synchronize()    HAL_GPIO_WritePin(ALARM_GPIO_Port, ALARM_Pin, !HAL_GPIO_ReadPin(LED_GPIO_Port, LED_Pin))

/* List of tones. Tone should read only, need to add 0x00 at the end of the array to indicate end of tone sequence */
const uint8_t const DEFAULT_TONE[] = {0x05, 0x05, 0x00};    // ON-OFF periodically
const uint8_t const NOT_SET_TIMEOUT_TONE[] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x14, 0x00};
const uint8_t const PRESSURE_OUTRANGE_TONE[] = {0x02, 0x02, 0x00};
const uint8_t const ELECTRICAL_FAULT_TONE[] = {0x01, 0x01, 0x01, 0x01, 0x0B, 0x02, 0x00};
const uint8_t const HOMING_FAULT_TONE[] = {0x0A, 0x0A, 0x00};

/*********************************Variables***********************************/
static AlarmHandle_t handle;
static Hash_t htones;

static int32_t SILENCE_TIMEOUT_TICK;

/*************************Private function prototypes*************************/
static void Alarm_SetTone(const uint8_t* pui8Tone);

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
    handle.cursor = -1;
    handle.enable = false;
    handle.silence_time = 0;
    SILENCE_TIMEOUT_TICK = ui32TickRate * SILENCE_TIMEOUT;
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
 * @note If silence button is pressed or under going, all new alarms will have no effect
 *
 * @param f32Time time to silence (unit second)
*****************************************************************************/
void Alarm_Silence(float f32Time)
{
    if (abs(handle.silence_time) == SILENCE_TIMEOUT_TICK) return;

    handle.silence_time = (int32_t)(f32Time * handle.tick_rate);   // Convert to alarm tick base
    if (handle.enable == false)
    {
        handle.tick = 0;
        handle.cursor = -1;
    }
    else
    {
        handle.silence_time = -handle.silence_time;
    }
    _Alarm_OFF();
    // _LED_OFF();
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
    handle.cursor = -1;
    _Alarm_OFF();
    _LED_OFF();
}

/******************************************************************************
 * @brief Set alarm tone
 *
 * @param pui8Tone a constant pointer to tone array
 * @param ui32Len Length of the tone, this param is needed because the length
 * information will be loss when pass tone as pointer to the function
*****************************************************************************/
static inline void Alarm_SetTone(const uint8_t* pui8Tone)
{
    handle.tone = pui8Tone;
}

/******************************************************************************
 * @brief Add a tone to the tone table
 * 
 * @param pui8Tone pointer to tone
 * @return true if tone add successfully
 * @return false if the table is full
*****************************************************************************/
bool Alarm_AddTone(const uint8_t* pui8Tone)
{
    if (Hash_Add(&htones, pui8Tone))
    {
        if (htones.load == 1)
        {
            Alarm_SetTone(pui8Tone);
            Alarm_Enable();
        }
        return true;
    }
    else
    {
        return false;
    }
}

/******************************************************************************
 * @brief Remove a tone from the tone table
 * 
 * @param pui8Tone tone that need to remove
 * @return true if tone was successfully removed
 * @return false if the tone not exist in the table
*****************************************************************************/
bool Alarm_RemoveTone(const uint8_t* pui8Tone)
{
    if (Hash_Remove(&htones, pui8Tone))
    {
        if (htones.load == 0)
        {
            Alarm_Disable();
        }
        else
        {
            /* Find the next tone to play */
            do
            {
                htones.current_index = (htones.current_index + 1) % MAX_TABLE_SIZE;
            }
            while (htones.table[htones.current_index] == NULL);
            Alarm_SetTone(htones.table[htones.current_index]);
            handle.cursor = 0;
            handle.tick = 0;
            _LED_OFF();
        }
        return true;
    }
    else
    {
        return false;
    }
}

/******************************************************************************
 * @brief Synchronize the ON/OFF state of alarm with LED
 * 
*****************************************************************************/
inline void Alarm_SynchronizeWithLED()
{
    _Alarm_Synchronize();
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

/******************************************************************************
 * @brief Toggle led state
 * 
*****************************************************************************/
inline void Alarm_LED_Toggle()
{
    _LED_TOGGLE();
}

/******************************************************************************
 * @brief Turn LED off
 * 
*****************************************************************************/
inline void Alarm_LED_Off()
{
    _LED_OFF();
}

/******************************************************************************
 * @brief Write LED state
 * 
 * @param bOn is LED ON
*****************************************************************************/
void Alarm_LED_Write(bool bOn)
{
    _LED_WRITE(bOn);
}

/* End of alarm.c */
