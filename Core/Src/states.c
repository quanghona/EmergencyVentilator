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
#include "lcd.h"
#include "alarm.h"
#include "pots.h"
#include "motor_comm.h"
#include "sensor.h"

/*******************************Definitions***********************************/

struct Motion_t
{
    float T;
    float Tin;
    float Tex;

    uint32_t Vin;
    uint32_t Vex;
    int32_t pos_in;
} motion_param;

/*************************Private function prototypes*************************/

static void calculateMotion(POT_t* applied_value);

/****************************Function definitions*****************************/

/******************************************************************************
 * @brief Define the necessary actions when state transition is happening
 * 
 * @param pssCurrent current system state
*****************************************************************************/
void State_Transition(SystemState_t* pssCurrent)
{
    // SensorData_t* sensor_data;

    switch (*pssCurrent)
    {
        case STARTUP:
            MC_ConfigHostInterrupt(MC_IT_TRAJECTORY);
            
            Alarm_Init(TASK_ALARM_FREQUENCY);
            TIM4_delayus(20000);  // Pressure sensor warm-up time
            
            Task_Enable(TASK_UPDATE_LCD, TASK_LCD_FREQUENCY);
            Task_Enable(TASK_ALARM, TASK_ALARM_FREQUENCY);
            Task_Enable(TASK_READ_POT, TASK_READ_POT_FREQUENCY);
            Task_Enable(TASK_CONFIRM_BUTTON, TASK_BUTTON_FREQUENCY);
            Task_Enable(TASK_SILENCE_BUTTON, TASK_BUTTON_FREQUENCY);
            Task_Enable(TASK_LIMIT_SWITCH, TASK_BUTTON_FREQUENCY);
            // Task_Enable(TASK_MODE_SWITCH, TASK_BUTTON_FREQUENCY);  // assist control not support yet
            Task_Enable(TASK_CHECK_ERROR, TASK_CHECK_ERROR_FREQUENCY);

            // MC_Homing(1000, -1000, true);
            *pssCurrent = PREHOME;
            LCD_RemoveMessage(MESSAGE_STARTUP);
            LCD_AddMessage(MESSAGE_HOMING);
            break;

        // TODO: Add actions when switch between states
        case PREHOME:
            MC_ConfigHostInterrupt(MC_IT_TRAJECTORY);
            MC_SetInstantSmoothAbsolutePosition(500, 1000, 500, POSITION_ZERO);
            *pssCurrent = HOMING;
            break;

        case HOMING:
            *pssCurrent = POSTHOME;
            break;
        
        case POSTHOME:
            MC_ConfigHostInterrupt(MC_IT_TRAJECTORY);
            POT_ApplyValue();
            calculateMotion(POT_GetAppliedValue());
            MC_SetInstantSmoothAbsolutePosition(500, motion_param.Vin, 500, motion_param.pos_in);

            *pssCurrent = INHALE;

            LCD_RemoveMessage(MESSAGE_HOMING);
            LCD_AddMessage(MESSAGE_INHALE);
            break;

        case INHALE:
            // sensor_data = Sensor_GetData();
            // sensor_data->pip = sensor_data->pressure;
            // TODO: Check inspiratory phase has ended, update pip
            *pssCurrent = INHALE_PAUSE;
            Task_StartOneShotTimer((uint32_t)(HOLDING_TIME * 1000));
            break;

        case INHALE_PAUSE:
            // sensor_data = Sensor_GetData();
            // sensor_data->plateau = sensor_data->pressure;
            // TODO: check pausing timeout is elapsed, update plateau preassure
            MC_ConfigHostInterrupt(MC_IT_TRAJECTORY);
            MC_SetInstantSmoothAbsolutePosition(500, motion_param.Vex, 500, POSITION_ZERO);

            *pssCurrent = EXHALE;

            LCD_RemoveMessage(MESSAGE_INHALE);
            LCD_AddMessage(MESSAGE_EXHALE);
            break;

        case EXHALE:
            // sensor_data = Sensor_GetData();
            // sensor_data->peep = sensor_data->pressure;
            // TODO: Check expiratory phase has ended, update peep, implement patient inhale detection

            MC_ConfigHostInterrupt(MC_IT_TRAJECTORY);
            calculateMotion(POT_GetAppliedValue());
            MC_SetInstantSmoothAbsolutePosition(500, motion_param.Vin, 500, motion_param.pos_in);
            
            *pssCurrent = INHALE;

            LCD_RemoveMessage(MESSAGE_EXHALE);
            LCD_AddMessage(MESSAGE_INHALE);
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
            // return Button_GetEvent(MOTOR_INT) == LOW_LEVEL;
            return true;
            
        case HOMING:
            // TODO: Need feedback from motor controlling whenever the position "zero" is reached
            return Button_GetEvent(MOTOR_INT) == LOW_LEVEL;
            // return true;
            
        case POSTHOME:
            return true;
            // TODO: Need feedback from motor controller whenever the hard-coded position is reached
            
        case INHALE:
            return Button_GetEvent(MOTOR_INT) == LOW_LEVEL;
            // TODO: check t >= Tin
            
        case INHALE_PAUSE:
            // TODO: check t >= Th
            return Task_IsTimerExpired();
            
        case EXHALE:
            return Button_GetEvent(MOTOR_INT) == LOW_LEVEL;
            // TODO: check t >= Tex
            
        // case EXHALE_PAUSE:
        //     break;
            
        default:
            break;
    }
    return false;
}

/******************************************************************************
 * @brief Calculate parameters needed to pass to external controller
 * 
 * @param applied_value input values from POTs
*****************************************************************************/
static void calculateMotion(POT_t* applied_value)
{
    int32_t volume = applied_value->tidal * PULSE_FACTOR;  // percent to pulse
    float fabsvolume = (float)abs(volume);
    
    motion_param.T = 60.0f / applied_value->rr;
    motion_param.Tin = motion_param.T / (1 + applied_value->ie10 / 10.0f) - HOLDING_TIME;
    motion_param.Tex = motion_param.T - motion_param.Tin - HOLDING_TIME;

    motion_param.Vin = (uint32_t)(fabsvolume / motion_param.Tin);
    motion_param.Vex = (uint32_t)(fabsvolume / motion_param.Tex);
    motion_param.pos_in = volume + POSITION_ZERO;
}

/* End of states.c */
