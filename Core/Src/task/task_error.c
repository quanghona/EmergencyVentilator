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

/*******************************Definitions***********************************/
const float HIGH_PRESSURE = 40;     /* Unit: cmH20 */
const float LOW_PRESSURE = 5;       /* Unit: cmH20 */
const float CURRENT_THRESHOLD = 15; /* Unit: A */

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
    // TODO: Add other errors which may happen in real life

    if (!alarm_handle->enable && alarm_handle->status)
    {
        // TODO: setup alarm trigger scheme
        /* Order of below code segment will determine the priority (descending) of the alarm */
        if (alarm_handle->status.high_pressure || alarm_handle->status.low_pressure)
        {
            Alarm_Silence(PRESSURE_OUTRANGE_TIMEOUT);
            Alarm_SetTone(PRESSURE_OUTRANGE_TONE);
        }
        else if (alarm_handle->status.electrical)
        {
            Alarm_Silence(ELECTRICAL_FAULT_TIMEOUT);
            Alarm_SetTone(ELECTRICAL_FAULT_TONE);
        }
        else if (alarm_handle->status.not_set)
        {
            Alarm_Silence(POT_CHANGE_TIMEOUT);
            Alarm_SetTone(NOT_SET_TIMEOUT_TONE);
        }
        
        Alarm_Enable();
    }
    else if (!alarm_handle->status && alarm_handle->enable)
    {
        Alarm_Disable();
    }
}

/* End of task_error.c */
