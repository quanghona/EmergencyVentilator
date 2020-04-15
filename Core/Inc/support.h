/**
* @file support.h
* @brief Minor support functions
*
* @date 2020 Apr 06
* @version 1
* @author Ly Hon Quang
* Group: Ventus code team
*
*/

#ifndef SUPPORT_H_
#define SUPPORT_H_

#include "main.h"

/*******************************Definitions***********************************/
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

/****************************Function prototypes******************************/
void TIM4_delayus(uint16_t ui16Delay);
// uint32_t strlen(char* s);
// void memset(uint32_t* arr, uint32_t val, uint32_t len);
int32_t abs(int32_t val);

/*****************************************************************************/
#endif /* SUPPORT_H_ */

/* End of support.h */
