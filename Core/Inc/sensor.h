/**
* @file sensor.h
* @brief Sensor data handler
*
* @details
* @date 2020 Apr 06
* @version 1
* @author Ly Hon Quang
* Group: Ventus code team
*
*/

#ifndef SENSOR_H_
#define SENSOR_H_

#include "main.h"

/*******************************Definitions***********************************/

typedef struct
{
    float pressure;
    float angle;
    float ris;
    float lis;
    // float temp;
    // float humi;
} SensorData_t;

/****************************Function prototypes******************************/
void Sensor_UpdateValue(uint32_t* pui32Raw);
SensorData_t* Sensor_GetData(void);

/*****************************************************************************/
#endif /* SENSOR_H_ */

/* End of sensor.h */
