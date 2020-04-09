/**
* @file task_lcd.c
* @brief Update the display information
*
* @details
* @date 2020 Apr 4
* @version 1
* @author Ly Hon Quang
* Group: Ventus code team
*
*/

#include "task.h"
#include "pots.h"
#include "lcd.h"
#include "sensor.h"

/****************************Function definitions*****************************/

/******************************************************************************
 * @brief Task that update the display
 * @details Update the parameters such as tidal volume, respiratory rate, I:E ratio
 * and pressure values
 * 
*****************************************************************************/
void Task_UpdateLCD(void)
{
    POT_t* pot_value = POT_GetKnobValue();
    SensorData_t* sensor_data = Sensor_GetData();

    LCD_printf(0, 1, "V=%3d%%max", pot_value->tidal);
    LCD_printf(0, 2, "RR=%2d/min", pot_value->rr);
    LCD_printf(0, 3, "I:E=1:%1d.%1d", pot_value->ie10 / 10, pot_value->ie10 % 10);

    LCD_printf(11, 1, "peak=%2d", sensor_data->pip);
    LCD_printf(11, 2, "plat=%2d", sensor_data->plateau);
    LCD_printf(11, 3, "PEEP=%2d", sensor_data->peep);
    
    // TODO: Display the message at the first line of the display
}

/*****************************************************************************/

/* End of task_lcd.c */
