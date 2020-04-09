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
    float pressure;         /** Pressure value */
    float angle;            /** Angle measurement value */
    float ris;              /** Current of the right half of H bridge */
    float lis;              /** Current of the left half of H bridge */
    // float temp;
    // float humi;

    float pip;              /** Peak inspiratory pressure */
    float plateau;          /** Plateau pressure */
    float peep;             /** Positive end-expiratory pressure */
} SensorData_t;

/****************************Function prototypes******************************/
void Sensor_UpdateValue(uint32_t* pui32Raw);
SensorData_t* Sensor_GetData(void);

/*****************************************************************************/
#endif /* SENSOR_H_ */

/* End of sensor.h */
