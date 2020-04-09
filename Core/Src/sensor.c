/**
* @file sensor.c
* @brief Sensor data management
*
* @date 2020 Apr 08
* @version 1
* @author Ly Hon Quang
* Group: Ventus code team
*
*/

#include "sensor.h"

/*********************************Variables***********************************/
static SensorData_t data = {0.0F, 0.0F, 0.0F, 0.0F};
static uint32_t data_raw[4] = {0, 0, 0, 0};

const float KPA_TO_CMH2O_FACTOR = 0.0980665f;

/****************************Function definitions*****************************/

/******************************************************************************
 * @brief Update obtained raw data to sensor data structure
 * @note This function use weight decay to average the data, which is follow the
 * fomula of new_data = (old_data * 0.8) + (raw_data * 0.2)
 * 
 * @param pui32Raw pointer to new raw data
*****************************************************************************/
void Sensor_UpdateValue(uint32_t* pui32Raw)
{
    int i;

    for (i = 0; i < 4; i++)
    {
        data_raw[i] = ((data_raw[i] << 2) + pui32Raw[i]) / 5;
    }
}

/******************************************************************************
 * @brief Convert and return sensor data structure instance
 * 
 * @details The pressure conversion fomula follow the Transfer funtion define in
 * MPXV7025's datasheet file.
 * @return SensorData_t* pointer to the data instance
 * 
 * @link https://www.mouser.com/datasheet/2/302/MPXV7025-1127333.pdf
*****************************************************************************/
SensorData_t* Sensor_GetData(void)
{
    /* Convert data */
    data.pressure = (22 * data_raw[0] - 68250) / 2457.0f * KPA_TO_CMH2O_FACTOR;
    data.angle = data_raw[1];
    data.ris = data_raw[2];
    data.lis = data_raw[3];

    return &data;
}

/* End of sensor.c */
