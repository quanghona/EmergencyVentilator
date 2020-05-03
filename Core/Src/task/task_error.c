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
#include "constants.h"
#include "lcd.h"

/*******************************Definitions***********************************/

struct ErrorResolvingData
{
    uint32_t mask;
    float timeout;
    const uint8_t* tone;
    const char* message;
};

// Please refer to the order of the alarm status bits in the alarm handle structure to define the mask value
static const struct ErrorResolvingData ERD[] = {
    {.mask = 0x00000001, .timeout = PRESSURE_OUTRANGE_TIMEOUT, .tone = TONE_PRESSURE_OUTRANGE, .message = MESSAGE_HIGH_PRESSURE},
    {.mask = 0x00000002, .timeout = PRESSURE_OUTRANGE_TIMEOUT, .tone = TONE_PRESSURE_OUTRANGE, .message = MESSAGE_LOW_PRESSURE},
    {.mask = 0x00000004, .timeout = PRESSURE_OUTRANGE_TIMEOUT, .tone = TONE_PRESSURE_OUTRANGE, .message = MESSAGE_PLATEAU_EXCEED_PIP},
    {.mask = 0x00000008, .timeout = PRESSURE_OUTRANGE_TIMEOUT, .tone = TONE_PRESSURE_OUTRANGE, .message = MESSAGE_PEEP_EXCEED_PLATEAU},
    {.mask = 0x00000010, .timeout = PRESSURE_OUTRANGE_TIMEOUT, .tone = TONE_PRESSURE_OUTRANGE, .message = MESSAGE_PEEP_LOW},
    {.mask = 0x00000020, .timeout = PRESSURE_OUTRANGE_TIMEOUT, .tone = TONE_PRESSURE_OUTRANGE, .message = MESSAGE_PLATEAU_HIGH},
    {.mask = 0x00000040, .timeout = PRESSURE_OUTRANGE_TIMEOUT, .tone = TONE_PRESSURE_OUTRANGE, .message = MESSAGE_PIP_HIGH},
    {.mask = 0x00000080, .timeout = PRESSURE_OUTRANGE_TIMEOUT, .tone = TONE_PRESSURE_OUTRANGE, .message = MESSAGE_PLATEAU_LOW},
    {.mask = 0x00000100, .timeout = ELECTRICAL_FAULT_TIMEOUT, .tone = TONE_ELECTRICAL_FAULT, .message = MESSAGE_ELECTRICAL_FAULT},
    {.mask = 0x00000400, .timeout = HOMING_FAULT_TIMEOUT, .tone = TONE_HOMING_FAULT, .message = MESSAGE_HOMING_FAULT},
    {.mask = 0x00000800, .timeout = POT_CHANGE_TIMEOUT, .tone = TONE_NOT_SET_TIMEOUT, .message = MESSAGE_NOT_SET}
};
static const int ERD_SIZE = sizeof(ERD) / sizeof(ERD[0]);   // precalculate to save resources

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
    uint32_t old_status = alarm_handle->status.ui32Status;
    uint32_t status_rising, status_falling;
    int i;

    /* Status update */
    alarm_handle->status.not_set = ((knob_value->rr != applied_value->rr) ||
                                   (knob_value->tidal != applied_value->tidal) ||
                                   (knob_value->ie10 != applied_value->ie10));
                                //    (knob_value->threshold != applied_value->threshold));
    // alarm_handle->status.high_pressure = (sensor_data->pressure >= HIGH_PRESSURE);
    // alarm_handle->status.low_pressure = (sensor_data->pressure <= LOW_PRESSURE);
    // TODO: add other electrical fault or define other type of electrical fault in status struct
    // alarm_handle->status.electrical = ((sensor_data->lis >= CURRENT_THRESHOLD) ||
    //                                   (sensor_data->ris >= CURRENT_THRESHOLD));
    // alarm_handle->status.plateau_exceed_pip = sensor_data->plateau > sensor_data->pip;
    // alarm_handle->status.peep_exceed_plateau = sensor_data->peep > sensor_data->plateau;
    // alarm_handle->status.peep_low = sensor_data->peep < PEEP_LOW_PRESSURE;
    // alarm_handle->status.plateau_high = sensor_data->plateau > PLATEAU_HIGH_PRESSURE;
    // alarm_handle->status.pip_high = sensor_data->pip > PIP_HIGH_PRESSURE;
    // alarm_handle->status.plateau_low = sensor_data->plateau < PLATEAU_LOW_PRESSURE;
    // alarm_handle->status.homing_fault = (task_handle->sys_state > HOMING) &&
    //                                     Button_GetEvent(SWITCH_LIMIT) != LOW_LEVEL;
    //TODO: add home switch not touching when homing case
    // TODO: Add other errors which may happen in real life

    /* Catch status rising or falling edges */
    status_rising = (alarm_handle->status.ui32Status) & ~old_status;
    status_falling = ~(alarm_handle->status.ui32Status) & old_status;

    for (i = 0; i < ERD_SIZE; i++)
    {
        if (status_rising & ERD[i].mask)
        {
            Alarm_Silence(ERD[i].timeout);
            Alarm_AddTone(ERD[i].tone);
            LCD_AddMessage(ERD[i].message);
        }
        else if (status_falling & ERD[i].mask)
        {
            // This is a hack, with the pressure alarm, only disable these alarm when all pressure is in normal condition
            // TODO: Need more safe approach in the future
            if (((ERD[i].mask & 0x000000FF) == 0) || (ERD[i].mask > 0x000000FF))
            {
                Alarm_RemoveTone(ERD[i].tone);
            }
            LCD_RemoveMessage(ERD[i].message);
        }
    }
}

/* End of task_error.c */
