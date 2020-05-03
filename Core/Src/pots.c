/**
* @file pots.c
* @brief Handle the POTs data and its interaction with outside
*
* @date 2020 Apr 04
* @version 1
* @author Ly Hon Quang
* Group: Ventus code team
*
*/

#include "pots.h"

/*********************************Variables***********************************/
static uint32_t _POT_raw[4] = {0,0,0,0};
static POT_t _knob_value = {0,0,0,0};
static POT_t _applied_value = {0,0,0,0};

/*************************Private function prototypes*************************/

/******************************************************************************
 * @brief Convert raw values to standard unit.
 *
 * @details With the 12-bit ADC resolution. The fomula are:
 * Respiratory rate = raw * 32 / 4096 + 8 (0-4095 to 8-40 bpm)
 * Tidal = raw * 100 / 4096 (0-4095 to 0-100%)
 * IE = raw * 30 / 4096 + 10 (0-4095 to 1-4 * 10)
 * Threshold = raw * 100 / 4096 (0-4095 to 0-100)
 * @note The denominator of all fomula should be 4095 but we make it 4096 to
 * simplify the calculation
 *
*****************************************************************************/
static void POT_ConvertRawToKnobValue()
{
    _knob_value.rr = (_POT_raw[0] >> 7) + 8;
    _knob_value.tidal = (_POT_raw[1] * 100) >> 12;
    _knob_value.ie10 = ((_POT_raw[2] * 30) >> 12) + 10;
    _knob_value.threshold = (_POT_raw[3] * 100) >> 12;
}

/****************************Function definitions*****************************/

/******************************************************************************
 * @brief Update the knob values
 * In this function, the value will be average with ratio old : new = 0.8 : 0.2
 *
 * @param pui32Raw new raw value from ADC. Constraint: the array must have 4 elements
 * and follow the order of: rr, tidal, ie, threshold
 *
 * @return none
*****************************************************************************/
void POT_UpdateKnobValue(uint32_t* pui32Raw)
{
    int i;

    for (i = 0; i < 4; i++)
    {
        _POT_raw[i] = ((_POT_raw[i] << 2) + pui32Raw[i]) / 5;
    }
}

/******************************************************************************
 * @brief Retrieve current knob value that store in the system
 *
 * @return POT_t* pointer to the knob data structure
*****************************************************************************/
POT_t* POT_GetKnobValue(void)
{
    POT_ConvertRawToKnobValue();
    return (&_knob_value);
}

/******************************************************************************
 * @brief Retrieve the applied value
 *
 * @return POT_t* pointer to the applied data structure
*****************************************************************************/
POT_t* POT_GetAppliedValue(void)
{
    return (&_applied_value);
}

/******************************************************************************
 * @brief Apply the knob value to the value use for processing
 *
*****************************************************************************/
void POT_ApplyValue(void)
{
    POT_ConvertRawToKnobValue();

    _applied_value.rr = _knob_value.rr;
    _applied_value.tidal = _knob_value.tidal;
    _applied_value.ie10 = _knob_value.ie10;
    _applied_value.threshold = _knob_value.threshold;
}

/*****************************************************************************/

/* End of pots.c */
