/**
* @file task_error.c
* @brief Task for checking operation error
*
* @details A list of error define in this link: https://e-vent.mit.edu/controls/operation/
* There may be other types of error in the future when project go further
* @date 2020 Apr 08
* @version 1
* @author Ly Hon Quang
* Group: Ventus code team
*
*/

#include "task.h"
#include "sensor.h"
#include "pots.h"
#include "alarm.h"
#include "button.h"
#include "support.h"
#include "lcd.h"

/*******************************Definitions***********************************/
const float HIGH_PRESSURE = 40;     /* Unit: cmH20 */
const float LOW_PRESSURE = 5;       /* Unit: cmH20 */
const float CURRENT_THRESHOLD = 15; /* Unit: A */
const float PEEP_LOW_PRESSURE = 0;  //TODO: TBD
const float PLATEAU_HIGH_PRESSURE = 30;
const float PIP_HIGH_PRESSURE = 40;
const float PLATEAU_LOW_PRESSURE = 5;

const char* MESSAGE_EMPTY = "                    ";
const char* MESSAGE_NOT_SET = "VALUE NOT CONFIRMED";
const char* MESSAGE_LOW_PRESSURE = "LOW PRESSURE";
const char* MESSAGE_HIGH_PRESSURE = "HIGH PRESSURE";
const char* MESSAGE_POSITION_ERROR = "POSITION ERROR";
const char* MESSAGE_ELECTRICAL_FAULT = "ELECTRICAL FAULT";
const char* MESSAGE_HOMING_FAULT = "HOMING FAULT";
const char* MESSAGE_PLATEAU_EXCEED_PIP = "PIP < PLATEAU";
const char* MESSAGE_PEEP_EXCEED_PLATEAU = "PLATEAU < PEEP";
const char* MESSAGE_PEEP_LOW = "PEEP LOW VALUE";
const char* MESSAGE_PLATEAU_HIGH = "PLATEAU HIGH VALUE";
const char* MESSAGE_PIP_HIGH = "PIP HIGH VALUE";
const char* MESSAGE_PLATEAU_LOW = "PLATEAU LOW VALUE";

/*************************Private function prototypes*************************/
static void LCD_SetMessage(const char* message);

/****************************Function definitions*****************************/

/******************************************************************************
 * @brief Check various type of fault operation of the system.
 * 
*****************************************************************************/
void Task_CheckError(void)
{
    POT_t* knob_value = POT_GetKnobValue();
    POT_t* applied_value = POT_GetAppliedValue();
    AlarmHandle_t* alarm_handle = Alarm_GetHandle();
    SensorData_t* sensor_data = Sensor_GetData();
    TaskHandle_t* task_handle = Task_GetHandleInstance();

    /* Status update */
    alarm_handle->status.not_set = ((knob_value->rr != applied_value->rr) ||
                                   (knob_value->tidal != applied_value->tidal) ||
                                   (knob_value->ie10 != applied_value->ie10) ||
                                   (knob_value->threshold != applied_value->threshold));
    alarm_handle->status.high_pressure = (sensor_data->pressure >= HIGH_PRESSURE);
    alarm_handle->status.low_pressure = (sensor_data->pressure <= LOW_PRESSURE);
    // TODO: add other electrical fault or define other type of electrical fault in status struct
    alarm_handle->status.electrical = ((sensor_data->lis >= CURRENT_THRESHOLD) ||
                                      (sensor_data->ris >= CURRENT_THRESHOLD));
    alarm_handle->status.plateau_exceed_pip = sensor_data->plateau > sensor_data->pip;
    alarm_handle->status.peep_exceed_plateau = sensor_data->peep > sensor_data->plateau;
    alarm_handle->status.peep_low = sensor_data->peep < PEEP_LOW_PRESSURE;
    alarm_handle->status.plateau_high = sensor_data->plateau > PLATEAU_HIGH_PRESSURE;
    alarm_handle->status.pip_high = sensor_data->pip > PIP_HIGH_PRESSURE;
    alarm_handle->status.plateau_low = sensor_data->plateau < PLATEAU_LOW_PRESSURE;
    alarm_handle->status.homing_fault = (task_handle->sys_state > POSTHOME) &&
                                        Button_GetEvent(SWITCH_LIMIT) != LOW_LEVEL;
    //TODO: add home switch not touching when homing case
    // TODO: Add other errors which may happen in real life

    if (!(alarm_handle->enable) && alarm_handle->status.ui32Status)
    {
        // TODO: setup alarm trigger scheme
        /* Order of below code segment will determine the priority (descending) of the alarm */
        if (alarm_handle->status.high_pressure)
        {
            Alarm_Silence(PRESSURE_OUTRANGE_TIMEOUT);
            Alarm_SetTone(PRESSURE_OUTRANGE_TONE, sizeof(PRESSURE_OUTRANGE_TONE));
            LCD_SetMessage(MESSAGE_HIGH_PRESSURE);
        }
        else if (alarm_handle->status.low_pressure)
        {
            Alarm_Silence(PRESSURE_OUTRANGE_TIMEOUT);
            Alarm_SetTone(PRESSURE_OUTRANGE_TONE, sizeof(PRESSURE_OUTRANGE_TONE));
            LCD_SetMessage(MESSAGE_LOW_PRESSURE);
        }
        else if (alarm_handle->status.plateau_exceed_pip)
        {
            Alarm_Silence(PRESSURE_OUTRANGE_TIMEOUT);
            Alarm_SetTone(PRESSURE_OUTRANGE_TONE, sizeof(PRESSURE_OUTRANGE_TONE));
            LCD_SetMessage(MESSAGE_PLATEAU_EXCEED_PIP);
        }
        else if (alarm_handle->status.peep_exceed_plateau)
        {
            Alarm_Silence(PRESSURE_OUTRANGE_TIMEOUT);
            Alarm_SetTone(PRESSURE_OUTRANGE_TONE, sizeof(PRESSURE_OUTRANGE_TONE));
            LCD_SetMessage(MESSAGE_PEEP_EXCEED_PLATEAU);
        }
        else if (alarm_handle->status.peep_low)
        {
            Alarm_Silence(PRESSURE_OUTRANGE_TIMEOUT);
            Alarm_SetTone(PRESSURE_OUTRANGE_TONE, sizeof(PRESSURE_OUTRANGE_TONE));
            LCD_SetMessage(MESSAGE_PEEP_LOW);
        }
        else if (alarm_handle->status.plateau_high)
        {
            Alarm_Silence(PRESSURE_OUTRANGE_TIMEOUT);
            Alarm_SetTone(PRESSURE_OUTRANGE_TONE, sizeof(PRESSURE_OUTRANGE_TONE));
            LCD_SetMessage(MESSAGE_PLATEAU_HIGH);
        }
        else if (alarm_handle->status.pip_high)
        {
            Alarm_Silence(PRESSURE_OUTRANGE_TIMEOUT);
            Alarm_SetTone(PRESSURE_OUTRANGE_TONE, sizeof(PRESSURE_OUTRANGE_TONE));
            LCD_SetMessage(MESSAGE_PIP_HIGH);
        }
        else if (alarm_handle->status.plateau_low)
        {
            Alarm_Silence(PRESSURE_OUTRANGE_TIMEOUT);
            Alarm_SetTone(PRESSURE_OUTRANGE_TONE, sizeof(PRESSURE_OUTRANGE_TONE));
            LCD_SetMessage(MESSAGE_PLATEAU_LOW);
        }
        else if (alarm_handle->status.electrical)
        {
            Alarm_Silence(ELECTRICAL_FAULT_TIMEOUT);
            Alarm_SetTone(ELECTRICAL_FAULT_TONE, sizeof(ELECTRICAL_FAULT_TONE));
            LCD_SetMessage(MESSAGE_ELECTRICAL_FAULT);
        }
        else if (alarm_handle->status.homing_fault)
        {
            Alarm_Silence(HOMING_FAULT_TIMEOUT);
            Alarm_SetTone(HOMING_FAULT_TONE, sizeof(HOMING_FAULT_TONE));
            LCD_SetMessage(MESSAGE_HOMING_FAULT);
        }
        else if (alarm_handle->status.not_set)
        {
            Alarm_Silence(POT_CHANGE_TIMEOUT);
            Alarm_SetTone(NOT_SET_TIMEOUT_TONE, sizeof(NOT_SET_TIMEOUT_TONE));
            LCD_SetMessage(MESSAGE_NOT_SET);
        }
        
        Alarm_Enable();
    }
    else if (!(alarm_handle->status.ui32Status) && (alarm_handle->enable))
    {
        Alarm_Disable();
        LCD_SetMessage(MESSAGE_EMPTY);
    }
}

/******************************************************************************
 * @brief Set LCD alarm message at the first line
 * 
 * @param message message string to be displayed. Currently only support less than
 * or equal 20 characters string
*****************************************************************************/
static void LCD_SetMessage(const char* message)
{
    uint32_t len = strlen((char*)message);

    LCD_printf(0, 0, message);

    if (len < 20)
    {
        LCD_printf(len, 0, MESSAGE_EMPTY + len);
    }

    // TODO: Support string longer than 20 characters
}

/* End of task_error.c */
