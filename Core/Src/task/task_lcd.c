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

/****************************Function definitions*****************************/

void Task_UpdateLCD(void)
{
    uint32_t peak = 0;
    uint32_t plat = 0;
    uint32_t peep = 0;
    POT_t* ppot_value = POT_GetKnobValue();

    LCD_printf(0, 1, "V=%3d%%max", ppot_value->tidal);
    LCD_printf(0, 2, "RR=%2d/min", ppot_value->rr);
    LCD_printf(0, 3, "I:E=1:%1d.%1d", ppot_value->ie10 / 10, ppot_value->ie10 % 10);

    // TODO: replace with real pressure values
    LCD_printf(11, 1, "peak=%2d", peak);
    LCD_printf(11, 2, "plat=%2d", plat);
    LCD_printf(11, 3, "PEEP=%2d", peep);
}

/*****************************************************************************/

/* End of task_lcd.c */
