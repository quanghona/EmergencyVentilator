/**
* @file constants.c
* @brief Define constants that use in system
*
* @date 2020 Apr 11
* @version 1
* @author Ly Hon Quang
* Group: Ventus code team
*
*/

#include "constants.h"

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

const char const MESSAGE_STARTUP[] = "State: STARTUP";
const char const MESSAGE_HOMING[] = "State: HOMING";
const char const MESSAGE_INHALE[] = "State: INHALE";
const char const MESSAGE_EXHALE[] = "State: EXHALE";

/* List of tones. Tone should read only, need to add 0x00 at the end of the array to indicate end of tone sequence */
const uint8_t const TONE_DEFAULT[] = {0x05, 0x05, 0x00};    // ON-OFF periodically
const uint8_t const TONE_NOT_SET_TIMEOUT[] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x14, 0x00};
const uint8_t const TONE_PRESSURE_OUTRANGE[] = {0x02, 0x02, 0x00};
const uint8_t const TONE_ELECTRICAL_FAULT[] = {0x01, 0x01, 0x01, 0x01, 0x0B, 0x02, 0x00};
const uint8_t const TONE_HOMING_FAULT[] = {0x0A, 0x0A, 0x00};

/* Threshold */
const float HIGH_PRESSURE = 40;                 /* Unit: cmH20 */
const float LOW_PRESSURE = 5;                   /* Unit: cmH20 */
const float CURRENT_THRESHOLD = 10;             /* Unit: A */
const float PEEP_LOW_PRESSURE = 5;              //TODO: TBD
const float PLATEAU_HIGH_PRESSURE = 30;
const float PIP_HIGH_PRESSURE = 40;
const float PLATEAU_LOW_PRESSURE = 5;

/* Motor Position */
const int POSITION_ZERO = -400;
const int POSITION_END = -1400;
const int PULSE_FACTOR = -10;    //(POSITION_END - POSITION_ZERO) / 100;

const float HOLDING_TIME = 0.2f;    // hodling time between inpiratory and expiratory phase, Unit second

/* End of constants.c */
