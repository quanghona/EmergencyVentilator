/**
* @file message.c
* @brief Define messages that use in system
*
* @date 2020 Apr 11
* @version 1
* @author Ly Hon Quang
* Group: Ventus code team
*
*/

#include "support.h"

/*******************************Definitions***********************************/

/* Note currently only sutpport messages that less or equal 20 (LCD width) */
const char const MESSAGE_EMPTY[] = "                    ";
const char const MESSAGE_NOT_SET[] = "VALUE NOT CONFIRMED";
const char const MESSAGE_LOW_PRESSURE[] = "LOW PRESSURE";
const char const MESSAGE_HIGH_PRESSURE[] = "HIGH PRESSURE";
const char const MESSAGE_POSITION_ERROR[] = "POSITION ERROR";
const char const MESSAGE_ELECTRICAL_FAULT[] = "ELECTRICAL FAULT";
const char const MESSAGE_HOMING_FAULT[] = "HOMING FAULT";
const char const MESSAGE_PLATEAU_EXCEED_PIP[] = "PIP < PLATEAU";
const char const MESSAGE_PEEP_EXCEED_PLATEAU[] = "PLATEAU < PEEP";
const char const MESSAGE_PEEP_LOW[] = "PEEP LOW VALUE";
const char const MESSAGE_PLATEAU_HIGH[] = "PLATEAU HIGH VALUE";
const char const MESSAGE_PIP_HIGH[] = "PIP HIGH VALUE";
const char const MESSAGE_PLATEAU_LOW[] = "PLATEAU LOW VALUE";

/*****************************************************************************/

/* End of message.c */
