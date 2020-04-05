/**
* @file task.h
* @brief Systam task definition
*
* @details 
* @date 2020 Apr 05
* @version 1
* @author Ly Hon Quang
* Group: Ventus code team
*
*/

#ifndef TASK_H_
#define TASK_H_

/*******************************Definitions***********************************/

const uint32_t TASK_CAPACITY = 32;

typedef struct
{
    uint32_t enable;                        /** Task enable bits. Maximum support 32 tasks */
    uint32_t period[TASK_CAPACITY];         /** Task period (base unit: tick rate) */
    uint32_t current_tick[TASK_CAPACITY];   /** Tasks's tick counter */
    uint32_t tick_rate;                     /** Task tick rate (Hz) */
} TaskHandle_t;

/* Task ID and distribution */
#define TASK_CONTROL_MOTOR          1
#define TASK_UPDATE_LCD             5
#define TASK_READ_POT               6
#define TASK_READ_SENSOR            7
#define TASK_ALARM                  8
#define TASK_CHECK_ERROR            9
#define TASK_READ_TEMP              10

/****************************Function prototypes******************************/
void Task_Init(uint32_t ui32Rate);
TaskHandle_t* Task_GetHandleInstance(void);
void Task_Enable(uint32_t ui32TaskID, uint32_t ui32Frequency);
void Task_Disable(uint32_t ui32TaskID);
void Task_Execute(void);

void Task_ConfirmButton(void);
void Task_UpdateLCD(void);

/*****************************************************************************/

#endif /* TASK_H_ */

/* End of task.h */
