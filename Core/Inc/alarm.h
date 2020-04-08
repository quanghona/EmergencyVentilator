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
    int state;                  /** A cursor for playing tone. -1 if under silent time */
} AlarmHandle_t;

/* List of tone */
extern const uint8_t DEFAULT_TONE[];

/****************************Function prototypes******************************/
void Alarm_Init(uint32_t ui32TickRate);
AlarmHandle_t* Alarm_GetHandle();
void Alarm_Enable();
void Alarm_Silence(float f32Time);
void Alarm_Disable();
void Alarm_SetTone(const uint8_t* pui8Tone, uint32_t ui32Len);

void Alarm_Toggle();
void Alarm_Off();

/*****************************************************************************/
#endif /* ALARM_H_ */

/* End of alarm.h */
