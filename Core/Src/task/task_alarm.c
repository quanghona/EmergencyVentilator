/**
* @file task_alarm.c
* @brief Handle alarm sound
*
* @date 2020 Apr 06
* @version 1
* @author Ly Hon Quang
* Group: Ventus code team
*
*/

#include "task.h"
#include "alarm.h"

/****************************Function definitions*****************************/

void Task_Alarm(void)
{
    AlarmHandle_t* handle = Alarm_GetHandle();

    if (handle->enable)
    {
        handle->tick++;
        /* Reserved for other uncommon cases in future */
        switch (handle->state)
        {
            case -1:
                /* Alarm silence time */
                if (handle->tick >= handle->silence_time)
                {
                    handle->state++;
                    handle->tick = 0;
                    handle->silence_time = 0;
                    Alarm_Toggle();
                }
                break;

            default:
                /* Tone loop here */
                if (handle->tick >= handle->tone[state])
                {
                    handle->state = (state + 1) % sizeof(handle->tone);
                    handle->tick = 0;
                    Alarm_Toggle();
                }
                break;
        }
    }
}

/* End of task_alarm.c */
