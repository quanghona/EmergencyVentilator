/**
* @file pots.h
* @brief Handle the POTs data and interation
*
* @details
* @date 2020 Apr 04
* @version 1
* @author Ly Hon Quang
* Group: Ventus code team
*
*/

#ifndef POTS_H_
#define POTS_H_

#include "main.h"

/*******************************Definitions***********************************/
typedef struct
{
    uint32_t rr;            /** Respiratory rate */
    uint32_t tidal;         /** Tidal volume */
    uint32_t ie10;          /** I:E ratio with the factor of 10 */
    uint32_t threshold;     /** Assist control threshold */
} POT_t;

/****************************Function prototypes******************************/
void POT_UpdateKnobValue(uint32_t* pui32Raw);
POT_t* POT_GetKnobValue(void);
void POT_ApplyValue(void);
POT_t* POT_GetAppliedValue(void);

/*****************************************************************************/

#endif /* POTS_H_ */

/* End of pots.h */
