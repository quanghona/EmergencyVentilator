/**
* @file motor_comm.h
* @brief Communication with external motor controller
*
* @details Definition of list of commands to control the motor via external controller
* @date 2020 Apr 26
* @version 1
* @author Ly Hon Quang
* Group: Ventus code team
*
*/

#ifndef MOTOR_COMM_H_
#define MOTOR_COMM_H_

#include "main.h"

/*******************************Definitions***********************************/

/* Command code definition */
typedef enum
{
    MC_COMMAND_INSTANTVELOCITYFIXACCELERATE = 0x00,
    MC_COMMAND_INSTANTVELOCITY = 0x01,
    MC_COMMAND_INSTANTSMOOTHVELOCITY = 0x02,
    MC_COMMAND_INSTANTSMOOTHABSOLUTEPOSITION = 0x03,
    MC_COMMAND_INSTANTSMOOTHRELATIVEPOSITION = 0x04,
    MC_COMMAND_VELOCITYFIXACCELERATE = 0x05,
    MC_COMMAND_VELOCITY = 0x06,
    MC_COMMAND_SMOOTHVELOCITY = 0x07,
    MC_COMMAND_SMOOTHABSOLUTEPOSITION = 0x08,
    MC_COMMAND_SMOOTHRELATIVEPOSITION = 0x09,
    MC_COMMAND_EXECUTE = 0x0A,
    MC_COMMAND_CONFIGHOSTINTERRUPT = 0x10,
    MC_COMMAND_CONFIGPARAMETERS = 0x17,
    MC_COMMAND_CONFIGSAFETY = 0x18,
    // MC_COMMAND_GETSTATUS = 0x0B,
    // MC_COMMAND_GETREALPOSITION = 0x0C,
    // MC_COMMAND_GETCURRENTVELOCITY = 0x0E,
    // MC_COMMAND_GETID = 0x1A,
    // MC_COMMAND_GETPARAMETERS = 0x1B,
    // MC_COMMAND_GETDEVICEDESCRIPTOR = 0x1C,
    MC_COMMAND_RESET = 0x1D,
    MC_COMMAND_HOMING = 0x1E,
    MC_COMMAND_CONFIGID = 0x1F,
    // MC_COMMAND_CONFIGBAUDRATE = 0x20
} MC_Command_t;

/* Interrupt types */
#define MC_IT_TRAJECTORY                    0x01
#define MC_IT_VELOCITY                      0x02

/* Baudrate selection */
#define MC_BAUD_115200                      0x00
#define MC_BAUD_921600                      0x01

/****************************Function prototypes******************************/

void MC_SetInstantVelocityFixAccelerate(uint32_t ui32Accel, int32_t i32Vel);
void MC_SetInstantVelocity(uint32_t ui32Accel, int32_t i32Vel);
void MC_SetInstantSmoothVelocity(uint32_t ui32Accel, int32_t i32Vel);
void MC_SetInstantSmoothAbsolutePosition(uint32_t ui32Accel, uint32_t ui32Vel, uint32_t ui32Decel, int32_t i32Pos);
void MC_SetInstantSmoothRelativePosition(uint32_t ui32Accel, uint32_t ui32Vel, uint32_t ui32Decel, int32_t i32Pos);

void MC_SetVelocityFixAccelerate(uint32_t ui32Accel, int32_t i32Vel);
void MC_SetVelocity(uint32_t ui32Accel, int32_t i32Vel);
void MC_SetSmoothVelocity(uint32_t ui32Accel, int32_t i32Vel);
void MC_SetSmoothAbsolutePosition(uint32_t ui32Accel, uint32_t ui32Vel, uint32_t ui32Decel, int32_t i32Pos);
void MC_SetSmoothRelativePosition(uint32_t ui32Accel, uint32_t ui32Vel, uint32_t ui32Decel, int32_t i32Pos);
void MC_Execute();

void MC_ConfigHostInterrupt(uint8_t ui8Interrupts);
void MC_ConfigParameters(uint32_t Kp, uint32_t Ki, uint32_t Kd, uint8_t ui8Scale);
void MC_ConfigSafety(uint32_t ui32Safety);
// uint16_t MC_GetId();
// void MC_GetParameters(uint32_t* Kp, uint32_t* Ki, uint32_t* Kd, uint8_t* ui8Scale);
// void MC_GetDeviceDescriptor(char* pcDesc);
void MC_Reset();
void MC_Homing(uint32_t ui32Accel, int32_t i32Vel, bool bWaitLogic1);
void MC_ConfigId(uint8_t ui8ID);
// void MC_ConfigBaudrate(uint8_t ui8Baud);

void MC_SetComplete(void);

/*****************************************************************************/
#endif /* MOTOR_COMM_H_ */

/* End of motor_comm.h */
