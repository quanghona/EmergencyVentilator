/**
* @file button.h
* @brief Manage the buttons/switch state
*
* @date 2020 Apr 06
* @version 1
* @author Ly Hon Quang
* Group: Ventus code team
*
*/

#ifndef BUTTON_H_
#define BUTTON_H_

#include "main.h"

/*******************************Definitions***********************************/

typedef enum
{
    BUTTON_CONFIRM,
    BUTTON_SILENCE,
    SWITCH_MODE_SELECTION,
    SWITCH_LIMIT,
    MAX_BUTTON
} Button_e;

/****************************Function prototypes******************************/
bool Button_GetState(Button_e button);
void Button_SetState(Button_e button, bool state);

/*****************************************************************************/
#endif /* BUTTON_H_ */

/* End of button.h */
