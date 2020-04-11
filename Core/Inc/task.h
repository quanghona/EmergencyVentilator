/**
* @file task.h
* @brief Systam task definition
* @note These tasks are only run in low speed (<= 1kHz) and no priority. Other special
* high speed tasks such as motor controlling, it is separated into other type of
* task.
*
* @date 2020 Apr 05
* @version 1
* @author Ly Hon Quang
* Group: Ventus code team
*
*/

#ifndef TASK_H_
#define TASK_H_

#include "main.h"
#include "states.h"

/*******************************Definitions***********************************/

#define TASK_CAPACITY               32

typedef struct
{
    uint32_t enable;                        /** Task enable bits. Maximum support 32 tasks */
    uint32_t period[TASK_CAPACITY];         /** Task period (base unit: tick rate) */
    uint32_t current_tick[TASK_CAPACITY];   /** Tasks's tick counter */
    uint32_t flag;                          /** Task execution flags */
    uint32_t tick_rate;                     /** Task tick rate (Hz) */
    SystemState_t sys_state;                /** System state */
} TaskHandle_t;

/* Task ID and distribution */
/* Note: The task for motor is handling in a separated higher speed timer */
#define TASK_UPDATE_LCD             1
#define TASK_READ_POT               2
#define TASK_ALARM                  3
#define TASK_CONFIRM_BUTTON         4
#define TASK_SILENCE_BUTTON         5
#define TASK_LIMIT_SWITCH           6
#define TASK_MODE_SWITCH            7
#define TASK_DISPLAY_MESSAGE        8
#define TASK_READ_TEMP              30
#define TASK_CHECK_ERROR            31

/****************************Function prototypes******************************/
/* Task management */
void Task_Init(uint32_t ui32Rate);
TaskHandle_t* Task_GetHandleInstance(void);
void Task_Enable(uint32_t ui32TaskID, uint32_t ui32Frequency);
void Task_Disable(uint32_t ui32TaskID);
void Task_IncreaseTick(void);
void Task_Execute(void);

/* Task functions */
void Task_ConfirmButton(void);
void Task_SilenceButton(void);
void Task_LimitSwitch(void);
void Task_SwitchMode(void);
void Task_UpdateLCD(void);
void Task_Alarm(void);
void Task_ReadPOTs(void);
void Task_CheckError(void);
void Task_DisplayMessage(void);

/* Other functions */
bool LCD_AddMessage(const char* pi8Message);
bool LCD_RemoveMessage(const char* pi8Message);

/*****************************************************************************/

#endif /* TASK_H_ */

/* End of task.h */
