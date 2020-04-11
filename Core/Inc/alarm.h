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
    int32_t silence_time;      /** Silence time, tick based.
                                   Positive values: both LED and alarm are off,
                                   negative values: only alarm is off */
    uint32_t tick;              /** Tick counter, use for timing in task */
    int cursor;                 /** A cursor for playing tone. -1 if under silent time */
    union AlarmStatus_u         /** Alarm status bits. For managing the various error type status */
    {
        struct
        {
            uint32_t high_pressure : 1;
            uint32_t low_pressure : 1;
            uint32_t plateau_exceed_pip : 1;
            uint32_t peep_exceed_plateau : 1;
            uint32_t peep_low : 1;
            uint32_t plateau_high : 1;
            uint32_t pip_high : 1;
            uint32_t plateau_low : 1;
            uint32_t electrical : 1;
            uint32_t position_error : 1;
            uint32_t homing_fault : 1;
            uint32_t not_set : 1;
            uint32_t assist_a : 1;
        };
        uint32_t ui32Status;
    } status;
} AlarmHandle_t;

/* List of tone */
extern const uint8_t const DEFAULT_TONE[];
extern const uint8_t const NOT_SET_TIMEOUT_TONE[];
extern const uint8_t const PRESSURE_OUTRANGE_TONE[];
extern const uint8_t const ELECTRICAL_FAULT_TONE[];
extern const uint8_t const HOMING_FAULT_TONE[];

/****************************Function prototypes******************************/
void Alarm_Init(uint32_t ui32TickRate);
AlarmHandle_t* Alarm_GetHandle();
void Alarm_Enable();
void Alarm_Silence(float f32Time);
void Alarm_Disable();
bool Alarm_AddTone(const uint8_t* pui8Tone);
bool Alarm_RemoveTone(const uint8_t* pui8Tone);

void Alarm_SynchronizeWithLED();
void Alarm_Toggle();
void Alarm_Off();
void Alarm_LED_Toggle();
void Alarm_LED_Off();

/*****************************************************************************/
#endif /* ALARM_H_ */

/* End of alarm.h */
