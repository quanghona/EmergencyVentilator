/**
* @file states.h
* @brief System state definitions
*
* @details Define a list of state necessary for the system
* @date 2020 Apr 04
* @version 1
* @author Ly Hon Quang
* Group: Ventus code team
* 
*/

#ifndef STATES_H_
#define STATES_H_

#include "main.h"

/*******************************Definitions***********************************/

typedef enum
{
    STARTUP,
    PREHOME,
    HOMING,
    POSTHOME,
    INHALE,
    INHALE_PAUSE,
    EXHALE,
    EXHALE_PAUSE
} SystemState_t;

/****************************Function prototypes******************************/

void State_Transition(SystemState_t* pssCurrent);
bool State_CheckTransition(SystemState_t ssCurrent);

/*****************************************************************************/

#endif /* STATES_H_ */
