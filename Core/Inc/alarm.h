/**
* @file alarm.h
* @brief Control the buzzer
*
* @details
* @date 2020 Apr 5
* @version 1
* @author Ly Hon Quang
* Group: Ventus code team
*
*/

#ifndef ALARM_H_
#define ALARM_H_

#include "main.h"

/*******************************Definitions***********************************/

typedef struct
{
    uint32_t tick_rate;         /** Alarm task frequency */
    bool enable;                /** Alarm enable state */
    const uint8_t* tone;        /** Constant pointer to tone */
    uint32_t tone_length;       /** Tone length */
    uint32_t silence_time;      /** Silence time, tick based */
    uint32_t tick;              /** Tick counter, use for timing in task */
    int cursor;                 /** A cursor for playing tone. -1 if under silent time */
    struct AlarmStatus               /** Alarm status bits. For managing the various error type status */
    {
        uint32_t high_pressure : 1;
        uint32_t low_pressure : 1;
        uint32_t electrical : 1;
        uint32_t position_error : 1;
        uint32_t homing_fault : 1;
        uint32_t not_set : 1;
        uint32_t assist_a : 1;
    } status;
} AlarmHandle_t;

/* List of tone */
extern const uint8_t DEFAULT_TONE[1];
extern const uint8_t NOT_SET_TIMEOUT_TONE[5];
extern const uint8_t PRESSURE_OUTRANGE_TONE[2];
extern const uint8_t ELECTRICAL_FAULT_TONE[7];
extern const uint8_t HOMING_FAULT_TONE[1];

/****************************Function prototypes******************************/
void Alarm_Init(uint32_t ui32TickRate);
AlarmHandle_t* Alarm_GetHandle();
void Alarm_Enable();
void Alarm_Silence(float f32Time);
void Alarm_Disable();
void Alarm_SetTone(const uint8_t* pui8Tone, uint32_t ui32Len);

void Alarm_Toggle();
void Alarm_Off();
void Alarm_LED_Toggle();
void Alarm_LED_Off();

/*****************************************************************************/
#endif /* ALARM_H_ */

/* End of alarm.h */
