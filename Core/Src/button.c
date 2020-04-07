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

static bool _state[MAX_BUTTON];

/****************************Function definitions*****************************/

bool Button_GetState(Button_e button)
{
    return _state[button];
}

void Button_SetState(Button_e button, bool state)
{
    _state[button] = state;
}

/* End of button.c */
