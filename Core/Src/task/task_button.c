/**
* @file task_button.c
* @brief Define task that related to buttons
*
* @details
* @date 2020 Apr 4
* @version 1
* @author Ly Hon Quang
* Group: Ventus code team
*
*/

#include "task.h"
#include "button.h"
#include "alarm.h"
#include "pots.h"

/****************************Function definitions*****************************/

void Task_ConfirmButton(void)
{
    if (Button_GetState(BUTTON_CONFIRM) != NO_EVENT)
    {
        Button_SetState(BUTTON_CONFIRM, NO_EVENT);
        POT_ApplyValue();
        Alarm_Disable();
    }
}

void Task_SilenceButton(void)
{
    if (Button_GetState(BUTTON_SILENCE) != NO_EVENT)
    {
        Button_SetState(BUTTON_SILENCE, NO_EVENT);
        Alarm_Silence(SILENCE_TIMEOUT);
    }
}

void Task_LimitSwitch(void)
{
    // TODO: replace stop motor procedure
    switch (Button_GetState(SWITCH_LIMIT))
    {
    case LOW_LEVEL:
        
        break;

    case HIGH_LEVEL:
        
        break;
    
    default:
        break;
    }
    Button_SetState(SWITCH_LIMIT, NO_EVENT);
}

void Task_SwitchMode(void)
{
    /* TODO: define switch mode setup operation */
    switch (Button_GetState(SWITCH_MODE_SELECTION))
    {
    case LOW_LEVEL:
        
        break;

    case HIGH_LEVEL:
        
        break;
    
    default:
        break;
    }
    Button_SetState(SWITCH_MODE_SELECTION, NO_EVENT);
}

/* End of task_button.c */
