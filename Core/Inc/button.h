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
    BUTTON_CONFIRM,             /** Confirm/set button */
    BUTTON_SILENCE,             /** Silence button */
    SWITCH_MODE_SELECTION,      /** Switch mode */
    SWITCH_LIMIT,               /** Switch limit */
    MOTOR_INT,                  /**  Motor external interrupt */
    MAX_BUTTON
} Button_e;

typedef enum
{
    NO_EVENT,                   /** No event detected */
    EDGE_DETECTED,              /** Either edge is detected */
    LOW_LEVEL,                  /** Low level or falling edge detected */
    HIGH_LEVEL,                 /** High level or rising edge detected */
    MAX_EVENT
} ButtonEvent_e;

/****************************Function prototypes******************************/

ButtonEvent_e Button_GetEvent(Button_e button);
void Button_SetEvent(Button_e button, ButtonEvent_e event);

/*****************************************************************************/
#endif /* BUTTON_H_ */

/* End of button.h */
