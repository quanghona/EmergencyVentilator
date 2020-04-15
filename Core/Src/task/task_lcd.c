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
#include "support.h"
#include "hash.h"
#include <string.h>

/*********************************Variables***********************************/
static Hash_t hmessages;

/*************************Private function prototypes*************************/
static void LCD_SetMessage(const char* message);

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
}

/******************************************************************************
 * @brief Display messages on first line of LCD in a cycle loop behavior
 * 
 * @see LCD_AddMessage, LCD_RemoveMessage
*****************************************************************************/
void Task_DisplayMessage(void)
{
    do
    {
        hmessages.current_index = (hmessages.current_index + 1) % MAX_TABLE_SIZE;
    }
    while (hmessages.table[hmessages.current_index] == NULL);
    LCD_SetMessage((const char*)(hmessages.table[hmessages.current_index]));
}

/******************************************************************************
 * @brief Add a message to message table
 * @details This function use hashing to allocate the space to store the message
 * pointer. After adding message, if there is only 1 message in the table, the
 * message will be display directly on LCD, no task enable. Otherwise, the display
 * task will be enable to start the displya loop
 * 
 * @param message message to be added
 * @return true if message sucessfully added to the table
 * @return false if the table is full
 * 
 * @see hash_func
*****************************************************************************/
bool LCD_AddMessage(const char* pi8Message)
{
    if(Hash_Add(&hmessages, (const uint8_t*)pi8Message))
    {
        /* If there is only 1 message to display, no task will be enable */
        if (hmessages.load == 1)
        {
            LCD_SetMessage(pi8Message);
        }
        else if (hmessages.load == 2)
        {
            Task_Enable(TASK_DISPLAY_MESSAGE, TASK_DISPLAY_MESSAGE_FREQUENCY);
        }
        return true;
    }
    else
    {
        return false;
    }
}

/******************************************************************************
 * @brief Remove a message from a message table
 * @details After removing message successfully, if onlly 1 message in the table,
 * the behavior in this case si similar with the AddMessage function.
 * And if the last message is removed, the LCD's message display line will be cleared
 * 
 * @param message pointer to the message to be removed
 * @return true if message was successfully removed
 * @return false if the message not exist in the table
 * 
 * @see hash_func
*****************************************************************************/
bool LCD_RemoveMessage(const char* pi8Message)
{
    if (Hash_Remove(&hmessages, (const uint8_t*)pi8Message))
    {
        if (hmessages.load == 0)
        {
            LCD_SetMessage(MESSAGE_EMPTY);
        }
        else if (hmessages.load == 1)
        {
            /* Same as AddMessage, no task will be enabled if only have 1 message in the table*/
            Task_Disable(TASK_DISPLAY_MESSAGE);
            Task_DisplayMessage();
        }
        return true;
    }
    else
    {
        return false;
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

/* End of task_lcd.c */
