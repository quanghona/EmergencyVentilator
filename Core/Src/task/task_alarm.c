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

/******************************************************************************
 * @brief Handles the alarm states and timing of the tone
 * @details This task manage the ON/OFF state of the alarm. And with the given
 * frequency, it also take care of the timing of the tone assigned in the alarm
 * handle
 * 
 * @see AlarmHandle_t
*****************************************************************************/
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
                    Alarm_LED_Toggle();
                }
                break;

            default:
                /* Tone loop here */
                if (handle->tick >= (uint32_t)(handle->tone[handle->state]))
                {
                    handle->state = (handle->state + 1) % handle->tone_length;
                    handle->tick = 0;
                    Alarm_Toggle();
                    Alarm_LED_Toggle();
                }
                break;
        }
    }
}

/* End of task_alarm.c */
