/**
* @file constants.h
* @brief Define constants use in system
*
* @date 2020 Apr 18
* @version 1
* @author Ly Hon Quang
* Group: Ventus code team
*
*/

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

/*******************************Definitions***********************************/

/* Task */
#define TASK_ALARM_FREQUENCY                    10
#define TASK_LCD_FREQUENCY                      5
#define TASK_READ_POT_FREQUENCY                 40
#define TASK_BUTTON_FREQUENCY                   20
#define TASK_CHECK_ERROR_FREQUENCY              20
#define TASK_DISPLAY_MESSAGE_FREQUENCY          1

/* Timeout. Unit: tick of Task alarm */
#define SILENCE_TIMEOUT                         600.0f
#define PRESSURE_OUTRANGE_TIMEOUT               0.0f
#define ELECTRICAL_FAULT_TIMEOUT                0.0f
#define HOMING_FAULT_TIMEOUT                    0.0f
#define POT_CHANGE_TIMEOUT                      50.0f

/* Messages */
extern const char const MESSAGE_EMPTY[];
extern const char const MESSAGE_NOT_SET[];
extern const char const MESSAGE_LOW_PRESSURE[];
extern const char const MESSAGE_HIGH_PRESSURE[];
extern const char const MESSAGE_POSITION_ERROR[];
extern const char const MESSAGE_ELECTRICAL_FAULT[];
extern const char const MESSAGE_HOMING_FAULT[];
extern const char const MESSAGE_PLATEAU_EXCEED_PIP[];
extern const char const MESSAGE_PEEP_EXCEED_PLATEAU[];
extern const char const MESSAGE_PEEP_LOW[];
extern const char const MESSAGE_PLATEAU_HIGH[];
extern const char const MESSAGE_PIP_HIGH[];
extern const char const MESSAGE_PLATEAU_LOW[];

extern const char const MESSAGE_STARTUP[];
extern const char const MESSAGE_HOMING[];
extern const char const MESSAGE_INHALE[];
extern const char const MESSAGE_EXHALE[];

/* List of tone */
extern const uint8_t const TONE_DEFAULT[];
extern const uint8_t const TONE_NOT_SET_TIMEOUT[];
extern const uint8_t const TONE_PRESSURE_OUTRANGE[];
extern const uint8_t const TONE_ELECTRICAL_FAULT[];
extern const uint8_t const TONE_HOMING_FAULT[];

/* Alarm Threshold */
extern const float HIGH_PRESSURE;
extern const float LOW_PRESSURE;
extern const float CURRENT_THRESHOLD;
extern const float PEEP_LOW_PRESSURE;
extern const float PLATEAU_HIGH_PRESSURE;
extern const float PIP_HIGH_PRESSURE;
extern const float PLATEAU_LOW_PRESSURE;

/*****************************************************************************/
#endif /* CONSTANTS_H_ */

/* End of constants.h */
