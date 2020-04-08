/**
* @file button.c
* @brief Manage the buttons/switch state
*
* @date 2020 Apr 07
* @version 1
* @author Ly Hon Quang
* Group: Ventus code team
*
*/

#include "button.h"

/*********************************Variables***********************************/
static ButtonEvent_e _event[MAX_BUTTON];

/****************************Function definitions*****************************/

/******************************************************************************
 * @brief Get button event
 * 
 * @param button specify the button to get event
 * @return ButtonEvent_e event result
*****************************************************************************/
ButtonEvent_e Button_GetEvent(Button_e button)
{
    return _event[button];
}

/******************************************************************************
 * @brief Set event of a button.
 * This function has 2 purpose:
 * - Set the event that detect by hardware
 * - Clear the event after processing it
 * 
 * @param button button to set the event
 * @param event event value
*****************************************************************************/
void Button_SetState(Button_e button, ButtonEvent_e event)
{
    if (event < MAX_EVENT)
        _event[button] = event;
}

/* End of button.c */
