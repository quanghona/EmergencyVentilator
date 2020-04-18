/**
* @file states.c
* @brief System state definitions and interaction
*
* @date 2020 Apr 09
* @version 1
* @author Ly Hon Quang
* Group: Ventus code team
*
*/

#include "states.h"
#include "support.h"
#include "constants.h"
#include "button.h"
#include "task.h"

/****************************Function definitions*****************************/

/******************************************************************************
 * @brief Define the necessary actions when state transition is happening
 * 
 * @param pssCurrent current system state
*****************************************************************************/
void State_Transition(SystemState_t* pssCurrent)
{
    switch (*pssCurrent)
    {
        case STARTUP:
            LCD_RemoveMessage(MESSAGE_STARTUP);
            LCD_AddMessage(MESSAGE_HOMING);
            
            Alarm_Init(10);
            TIM4_delayus(20000);  // Pressure sensor warm-up time
            
            Task_Enable(TASK_UPDATE_LCD, TASK_LCD_FREQUENCY);
            Task_Enable(TASK_ALARM, TASK_ALARM_FREQUENCY);
            Task_Enable(TASK_READ_POT, TASK_READ_POT_FREQUENCY);
            Task_Enable(TASK_CONFIRM_BUTTON, TASK_BUTTON_FREQUENCY);
            Task_Enable(TASK_SILENCE_BUTTON, TASK_BUTTON_FREQUENCY);
            Task_Enable(TASK_LIMIT_SWITCH, TASK_BUTTON_FREQUENCY);
            // Task_Enable(TASK_MODE_SWITCH, TASK_BUTTON_FREQUENCY);  // assist control not support yet
            Task_Enable(TASK_CHECK_ERROR, TASK_CHECK_ERROR_FREQUENCY);

            *pssCurrent = PREHOME;
            break;

        // TODO: Add actions when switch between states
        case PREHOME:
            *pssCurrent = HOMING;
            break;

        case HOMING:
            *pssCurrent = POSTHOME;
            break;
        
        case POSTHOME:
            LCD_RemoveMessage(MESSAGE_HOMING);
            LCD_AddMessage(MESSAGE_INHALE);
            *pssCurrent = INHALE;
            break;

        case INHALE:
            // TODO: Check inspiratory phase has ended, update pip
            *pssCurrent = INHALE_PAUSE;
            break;

        case INHALE_PAUSE:
            LCD_RemoveMessage(MESSAGE_INHALE);
            LCD_AddMessage(MESSAGE_EXHALE);
            // TODO: check pausing timeout is elapsed, update plateau preassure
            *pssCurrent = EXHALE;
            break;

        case EXHALE:
            // TODO: Check expiratory phase has ended, update peep, implement patient inhale detection
            LCD_AddMessage(MESSAGE_INHALE);
            *pssCurrent = INHALE;
            break;

        // case EXHALE_PAUSE:
        
        //     break;

        default:
            // Unexpected states
            // TODO: perform actions to get back to STARTUP state
            LCD_clear();
            LCD_AddMessage(MESSAGE_STARTUP);
            // LCD_RemoveAllMessages();
            *pssCurrent = STARTUP;
            break;
    }
}

/******************************************************************************
 * @brief Check if transition constraints are satisfied
 * 
 * @param ssCurrent current system state
 * @return true transfer to new state
 * @return false keep in current state
*****************************************************************************/
bool State_CheckTransition(SystemState_t ssCurrent)
{
    switch (ssCurrent)
    {
        case STARTUP:
            return true;
        
        case PREHOME:
            return Button_GetEvent(SWITCH_MODE_SELECTION) == HIGH_LEVEL;
            
        case HOMING:
            // TODO: Need feedback from motor controlling whenever the position "zero" is reached
            return Button_GetEvent(SWITCH_MODE_SELECTION) == LOW_LEVEL;
            
        case POSTHOME:
            // TODO: Need feedback from motor controller whenever the hard-coded position is reached
            break;
            
        case INHALE:
            // TODO: check t >= Tin
            break;
            
        case INHALE_PAUSE:
            // TODO: check t >= Th
            break;
            
        case EXHALE:
            // TODO: check t >= Tex
            break;
            
        // case EXHALE_PAUSE:
        //     break;
            
        default:
            break;
    }
    return false;
}

/* End of states.c */
