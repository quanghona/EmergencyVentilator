/**
* @file task_base.c
* @brief Task management functions and definitions
*
* @details 
* @date 2020 Apr 05
* @version 1
* @author Ly Hon Quang
* Group: Ventus code team
* 
*/

#include "task.h"
#include "support.h"

/*********************************Variables***********************************/
static TaskHandle_t handle;

/* Setup fixed order task list for simplification */
// TODO: define task list
typedef void (*Task_t)();
static const Task_t TaskList[TASK_CAPACITY] = {
    NULL,
    &Task_UpdateLCD,
    &Task_ReadPOTs,
    &Task_Alarm,
    &Task_ConfirmButton,
    &Task_SilenceButton,
    &Task_LimitSwitch,
    &Task_SwitchMode,
    &Task_DisplayMessage,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    &Task_CheckError
};

/****************************Function definitions*****************************/

/******************************************************************************
 * @brief Initialize Task handle structure
 * 
 * @param ui32Rate Task tick rate (Unit Hz)
*****************************************************************************/
void Task_Init(uint32_t ui32Rate)
{
    handle.enable = 0;
    memset(handle.period, 0, TASK_CAPACITY);
    memset(handle.current_tick, 0, TASK_CAPACITY);
    handle.tick_rate = ui32Rate;
    handle.sys_state = STARTUP;
}

/******************************************************************************
 * @brief get task handle instance
 * 
 * @return TaskHandle_t* pointer to instance
*****************************************************************************/
TaskHandle_t* Task_GetHandleInstance(void)
{
    return &handle;
}

/******************************************************************************
 * @brief Enable task
 * 
 * @param ui32TaskID task ID that listed in the header file
 * @param ui32Frequency task execution frequency (unit Hz)
 * Constraint: the frequency must be smaller or equal than base tick rate
*****************************************************************************/
void Task_Enable(uint32_t ui32TaskID, uint32_t ui32Frequency)
{
    if (ui32Frequency <= handle.tick_rate)
    {
        handle.enable |= 1 << ui32TaskID;
        handle.current_tick[ui32TaskID] = 0;
        handle.period[ui32TaskID] = handle.tick_rate / ui32Frequency;
    }
}

/******************************************************************************
 * @brief Halt task
 * 
 * @param ui32TaskID ID of the task to be halt
*****************************************************************************/
void Task_Disable(uint32_t ui32TaskID)
{
    uint32_t mask = ~(1 << ui32TaskID);

    handle.enable &= mask;
    handle.current_tick[ui32TaskID] = 0;
    handle.flag &= mask;
}

/******************************************************************************
 * @brief Increase tick count of all enabled tasks
 * The corresponding flag of the task will be set when the period is elapsed
 * This function should be called <b>inside<b> the timer ISR handler.
 * The calling rate of this function should synchronize with the tick rate when
 * calling Task_Init.
 * 
 * @see Task_Init
*****************************************************************************/
void Task_IncreaseTick(void)
{
    int i;
    uint32_t mask;

    for (i = 0; i < TASK_CAPACITY; i++)
    {
        mask = 1 << i;
        if (handle.enable & mask)
        {
            if (++(handle.current_tick[i]) >= handle.period[i])
            {
                handle.current_tick[i] -= handle.period[i];
                handle.flag |= mask;
            }
        }
    }    
}

/******************************************************************************
 * @brief Execute tasks that have been enabled.
 * This function should be called outside the ISR
 * 
*****************************************************************************/
void Task_Execute(void)
{
    int i;
    uint32_t mask;

    for (i = 0; i < TASK_CAPACITY; i++)
    {
        mask = 1 << i;
        if (handle.flag & mask)
        {
            handle.flag &= ~mask;
            TaskList[i]();      // Execute task here
        }
    }
}

/* End of task_base.c */
