#ifndef _COMM_PC_H
#define _COMM_PC_H

#include "sys.h"
#include "UART1.h"

#define M_PI_F 3.1415926
//only send data
typedef union int16un
{
uint8_t b[2];
int16_t val;
}int16_un;
typedef union int32un
{
uint8_t b[4];
int32_t val;
}int32_un;

typedef struct HawkerPacket_tt 
{
uint8_t header[2];
uint8_t cmd;
uint8_t len;
	
int16_un roll;
int16_un pitch;
int16_un yaw;
int32_un alti;
int16_un temp;
int32_un pres;
int16_un speed;
	
uint8_t sum;
}HawkerPacket_t;

typedef struct IMU_tt
{
uint8_t caliPass;
uint8_t ready;
int16_t accADC[3];
int16_t gyroADC[3];
int16_t magADC[3];
float 	accRaw[3];		//m/s^2
float 	gyroRaw[3];		//rad/s 
float 	magRaw[3];		//
float   accOffset[3];		//m/s^2
float   gyroOffset[3]; 
float   accb[3];		//filted, in body frame
float   accg[3];
float   gyro[3];
float   DCMgb[3][3];
float   q[4];
float   roll;				//deg
float   pitch;
float 	yaw;
float   rollRad;				//rad
float   pitchRad;
float 	yawRad;
}imu_t;
//
extern HawkerPacket_t up;
extern uint8_t pcCmdFlag;
extern uint16_t rcData[4];
extern imu_t imu;

void CommPCUploadHandle(void);
void CommPCProcessCmd(void);
void CommPC(uint8_t c);
void testCommPC(void);
void ReturnPIDHead(uint8_t pidType);

#ifdef DEBUG_COMMPC
extern uint8_t  loop50HzFlag;
void test_SendPC(void);
#endif

//
#define UartSendInt16(_x)  UartSendBuffer((uint8_t *)(&_x ),2)
#define UartSendInt32(_x)  UartSendBuffer((uint8_t *)(&_x ),4)
/*
#ifdef DEBUG_COMMPC
	void COMMPC_TEST(void);
#endif
*/
#endif

