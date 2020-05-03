/**
* @file motor_comm.c
* @brief Communication with external motor controller
*
* @date 2020 Apr 26
* @version 1
* @author Ly Hon Quang
* Group: Ventus code team
*
*/

#include "motor_comm.h"
#include "constants.h"
#include <stdarg.h>

/****************************Private Definitions******************************/

/* Send frame to hardware communication peripheral */
extern UART_HandleTypeDef huart3;
#define MC_Send(frame, len)                     HAL_UART_Transmit_DMA(&huart3, frame, len)
#define min(a,b)                                (((a) < (b)) ? (a) : (b))

/* Command elements */
const uint8_t MC_ELEMENT_START = 0x2A;
const uint8_t MC_ELEMENT_END = 0x3E;
const uint16_t MC_SLAVE_ID = 0xFFFF;    // Broadcast ID

const char* const FORMAT_VELOCITY = "44";
const char* const FORMAT_POSITION = "4444";

/*********************************Variables***********************************/

static uint8_t MC_frame[21];    // maximum support 16 data byte
static bool isComplete = true;  // packge transfer complete flag

/*************************Private function prototypes*************************/

static void MC_AssembleCommand(MC_Command_t command, const char* pFormat, ...);
static void MC_ui32Toui8Array(uint32_t src, uint8_t* dst);

/****************************Function definitions*****************************/

inline void MC_SetInstantVelocityFixAccelerate(uint32_t ui32Accel, int32_t i32Vel)
{
    MC_AssembleCommand(MC_COMMAND_INSTANTVELOCITYFIXACCELERATE, FORMAT_VELOCITY, ui32Accel, i32Vel);
}

inline void MC_SetInstantVelocity(uint32_t ui32Accel, int32_t i32Vel)
{
    MC_AssembleCommand(MC_COMMAND_INSTANTVELOCITY, FORMAT_VELOCITY, ui32Accel, i32Vel);
}

inline void MC_SetInstantSmoothVelocity(uint32_t ui32Accel, int32_t i32Vel)
{
    MC_AssembleCommand(MC_COMMAND_INSTANTSMOOTHVELOCITY, FORMAT_VELOCITY, ui32Accel, i32Vel);
}

inline void MC_SetInstantSmoothAbsolutePosition(uint32_t ui32Accel, uint32_t ui32Vel, uint32_t ui32Decel, int32_t i32Pos)
{
    MC_AssembleCommand(MC_COMMAND_INSTANTSMOOTHABSOLUTEPOSITION, FORMAT_POSITION, ui32Accel, ui32Vel, ui32Decel, i32Pos);
}

inline void MC_SetInstantSmoothRelativePosition(uint32_t ui32Accel, uint32_t ui32Vel, uint32_t ui32Decel, int32_t i32Pos)
{
    MC_AssembleCommand(MC_COMMAND_INSTANTSMOOTHRELATIVEPOSITION, FORMAT_POSITION, ui32Accel, ui32Vel, ui32Decel, i32Pos);
}

inline void MC_SetVelocityFixAccelerate(uint32_t ui32Accel, int32_t i32Vel)
{
    MC_AssembleCommand(MC_COMMAND_VELOCITYFIXACCELERATE, FORMAT_VELOCITY, ui32Accel, i32Vel);
}

inline void MC_SetVelocity(uint32_t ui32Accel, int32_t i32Vel)
{
    MC_AssembleCommand(MC_COMMAND_VELOCITY, FORMAT_VELOCITY, ui32Accel, i32Vel);
}

inline void MC_SetSmoothVelocity(uint32_t ui32Accel, int32_t i32Vel)
{
    MC_AssembleCommand(MC_COMMAND_SMOOTHVELOCITY, FORMAT_VELOCITY, ui32Accel, i32Vel);
}

inline void MC_SetSmoothAbsolutePosition(uint32_t ui32Accel, uint32_t ui32Vel, uint32_t ui32Decel, int32_t i32Pos)
{
    MC_AssembleCommand(MC_COMMAND_SMOOTHABSOLUTEPOSITION, FORMAT_POSITION, ui32Accel, ui32Vel, ui32Decel, i32Pos);
}

inline void MC_SetSmoothRelativePosition(uint32_t ui32Accel, uint32_t ui32Vel, uint32_t ui32Decel, int32_t i32Pos)
{
    MC_AssembleCommand(MC_COMMAND_SMOOTHRELATIVEPOSITION, FORMAT_POSITION, ui32Accel, ui32Vel, ui32Decel, i32Pos);
}

inline void MC_Execute()
{
    MC_AssembleCommand(MC_COMMAND_EXECUTE, "");
}

inline void MC_ConfigHostInterrupt(uint8_t ui8Interrupts)
{
    MC_AssembleCommand(MC_COMMAND_CONFIGHOSTINTERRUPT, "1", ui8Interrupts & 0x03);
}

inline void MC_ConfigParameters(uint32_t Kp, uint32_t Ki, uint32_t Kd, uint8_t ui8Scale)
{
    MC_AssembleCommand(MC_COMMAND_CONFIGPARAMETERS, "4441", Kp, Ki, Kd, min(ui8Scale,11));
}

inline void MC_ConfigSafety(uint32_t ui32Safety)
{
    MC_AssembleCommand(MC_COMMAND_CONFIGSAFETY, "4", ui32Safety);
}

// TODO: Get data from external controller
// uint16_t MC_GetId()
// {
//     MC_AssembleCommand(MC_COMMAND_GETID, "");
// }

// void MC_GetParameters(uint32_t* Kp, uint32_t* Ki, uint32_t* Kd, uint8_t* ui8Scale)
// {
//     MC_AssembleCommand(MC_COMMAND_GETPARAMETERS, "");
// }

// void MC_GetDeviceDescriptor(char* pcDesc)
// {
//     MC_AssembleCommand(MC_COMMAND_GETDEVICEDESCRIPTOR, "");
// }

inline void MC_Reset()
{
    MC_AssembleCommand(MC_COMMAND_RESET, "");
}

inline void MC_Homing(uint32_t ui32Accel, int32_t i32Vel, bool bWaitLogic1)
{
    MC_AssembleCommand(MC_COMMAND_HOMING, "441", ui32Accel, i32Vel, (uint8_t)bWaitLogic1);
}

inline void MC_ConfigId(uint8_t ui8ID)
{
    MC_AssembleCommand(MC_COMMAND_CONFIGID, "1", min(ui8ID,16));
}

// void MC_ConfigBaudrate(uint8_t ui8Baud)
// {
//     MC_AssembleCommand(MC_COMMAND_CONFIGBAUDRATE, "1", ui8Baud)
// }

/******************************************************************************
 * @brief Set package transfer completely flag
 * 
*****************************************************************************/
inline void MC_SetComplete(void)
{
    isComplete = true;
}

/******************************************************************************
 * @brief Assemble all data into final frame and send to hardware
 * 
 * @details Frame format. <Start><IDL><IDH><command><data>...<end>
 * 
 * @param command Command code
 * @param pFormat data order in size. Example data format: "441". 4 byte data
 * follow by 4 byte data and 1 byte data at the end. Note that data are LSB first.
 * @param ... Parameter of the sending frame. THe number of parameters must match
 * with the number of digits in format. Currently only supports 2 type of size:
 * 4 and 1 byte.
*****************************************************************************/
static void MC_AssembleCommand(MC_Command_t command, const char* pFormat, ...)
{
    va_list vaArgP;
    uint16_t len = 4;
    uint32_t value;

    while (isComplete == false);    // Need to complete old package before sending new one
    isComplete = false;
    // Assign all variables to frame array
    va_start(vaArgP, pFormat);		// Start the varargs processing.
    while (*pFormat)              // Traverse the format string
    {
        value = va_arg(vaArgP, unsigned long);
        switch (*pFormat)
        {
            // Handle 1 byte variable
            case '1':
                MC_frame[len++] = (uint8_t)value;
                break;

            // Handle 4 byte variable
            case '4':
                MC_ui32Toui8Array(value, MC_frame + len);
                len += 4;
                break;

            default:
                break;
        }
        pFormat++;
    }
    va_end(vaArgP);	// End the varargs processing.

    MC_frame[0] = MC_ELEMENT_START;
    MC_frame[1] = (uint8_t)MC_SLAVE_ID;
    MC_frame[2] = (uint8_t)(MC_SLAVE_ID >> 8);
    MC_frame[3] = (uint8_t)command;
    MC_frame[len++] = MC_ELEMENT_END;

    MC_Send(MC_frame, len);
}

/******************************************************************************
 * @brief transform a 4-byte data to a sequence of 4 1-byte data
 * 
 * @param src 4-byte data
 * @param dst 4-element array of equivalent value as src. Note the order of the 
 * data is LSB first
*****************************************************************************/
static void MC_ui32Toui8Array(uint32_t src, uint8_t* dst)
{
    for (int i = 0; i < 4; i++)
    {
        dst[i] = (uint8_t)src; // Get the LSB
        src >>= 8;  // Prepare the next LSB
    }
}

/* End of motor_comm.c */
