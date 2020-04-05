/**
* @file task_base.c
* @brief 
*
* @details 
* @date 2020 Apr 05
* @version 1
* @author Ly Hon Quang
* Group: Ventus code team
* 
*/

#include "task.h"

/*********************************Variables***********************************/
static TaskHandle_t handle;

/* Setup fixed order task list for simplification */
// TODO: define task list
const void (*TaskList[TASK_CAPACITY])(void) = {
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    Task_UpdateLCD,
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
    NULL,
    NULL,
    NULL,
    NULL
};

/****************************Function definitions*****************************/

/******************************************************************************
 * @brief Initialize Task handle structure
 * 
 * @param ui32Rate Task tick rate
*****************************************************************************/
void Task_Init(uint32_t ui32Rate)
{
    handle.enable = 0;
    memset(handle.period, 0, TASK_CAPACITY);
    memset(handle.current_tick, 0, TASK_CAPACITY);
    handle.tick_rate = ui32Rate;
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
void Task_SetEnable(uint32_t ui32TaskID, uint32_t ui32Frequency)
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
    handle.enable &= ~(1 << ui32TaskID);
    handle.current_tick[ui32TaskID] = 0;
}

/******************************************************************************
 * @brief Execute tasks that have been enabled.
 * This function should be called periodically and synchronize with the tick rate
 * set in the Task_Init function.
 * The function will increase the counter of each task and will execute the task
 * if its period is elapsed
 * 
 * @see Task_Init
*****************************************************************************/
void Task_Execute(void)
{
    int i;

    for (i = 0; i < 32; i++)
    {
        if (handle.enable & (1 << i))
        {
            if (++handle.current_tick[i] >= handle.period[i])
            {
                handle.current_tick[i] = 0;

                // TODO: assert null function call
                TaskList[i]();
            }
        }
    }
}

/* End of task_base.c */
