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
#include "constants.h"

/****************************Function definitions*****************************/

/******************************************************************************
 * @brief Task that manage the actions of the confirm button
 * 
*****************************************************************************/
void Task_ConfirmButton(void)
{
    if (Button_GetEvent(BUTTON_CONFIRM) != NO_EVENT)
    {
        Button_SetEvent(BUTTON_CONFIRM, NO_EVENT);
        POT_ApplyValue();
    }
}

/******************************************************************************
 * @brief Task that manage the actions of the silence button
 * 
*****************************************************************************/
void Task_SilenceButton(void)
{
    if (Button_GetEvent(BUTTON_SILENCE) != NO_EVENT)
    {
        Button_SetEvent(BUTTON_SILENCE, NO_EVENT);
        Alarm_Silence(SILENCE_TIMEOUT);
    }
}

/******************************************************************************
 * @brief Task that manage the actions of the state transition of the Limit switch
 * 
*****************************************************************************/
void Task_LimitSwitch(void)
{
    // TODO: replace stop motor procedure
    switch (Button_GetEvent(SWITCH_LIMIT))
    {
    case LOW_LEVEL:
        
        break;

    case HIGH_LEVEL:
        
        break;
    
    default:
        break;
    }
    // Button_SetEvent(SWITCH_LIMIT, NO_EVENT);
}

/******************************************************************************
 * @brief Task that handle the switching mode actions
 * 
*****************************************************************************/
void Task_SwitchMode(void)
{
    /* TODO: define switch mode setup operation */
    switch (Button_GetEvent(SWITCH_MODE_SELECTION))
    {
    case LOW_LEVEL:
        
        break;

    case HIGH_LEVEL:
        
        break;
    
    default:
        break;
    }
    Button_SetEvent(SWITCH_MODE_SELECTION, NO_EVENT);
}

/* End of task_button.c */
