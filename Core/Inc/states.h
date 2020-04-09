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

typedef enum
{
    STARTUP,
    PREHOME,
    HOMING,
    POSTHOME,
    SET_VALUE,
    INHALE,
    EXHALE
} SystemState_t;

#endif /* STATES_H_ */
