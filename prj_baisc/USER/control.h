#ifndef _CONTROL_H
#define _CONTROL_H
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include <stdlib.h>
#include "led.h"
//#include <stdio.h>
#include "CommPc.h"

typedef struct {
	float SetVal;   //期望值
	float ActualVal;//实际输出
	float err;
	float err_last;
	float Kp,Ki,Kd;
	float voltage;
	float integral;
	}_pid_STRUCT;


typedef struct 
{ 	
	float roll;
	float pitch;
	float yaw;
	
}_out_angle;

typedef struct 
{ 	
	float roll;
	float pitch;
	float yaw;
}_rc_data;

typedef struct
{
	float x;
	float y;
	float z;
}_out_geo;

typedef struct
{
float x;
float y;
float z;
float vx;
float vy;
float vz;
float ax;
float ay;
float az;
}nav_t;

//extern _out_geo _gGRYO;
//extern _out_geo _gACCEL;
//extern _out_angle imu;
extern float Roll;
extern float Pitch;
extern float Yaw;

extern _pid_STRUCT *roll_angle_pid;
extern _pid_STRUCT *roll_rate_pid;

extern _pid_STRUCT *pitch_angle_pid;
extern _pid_STRUCT *pitch_rate_pid;

extern _pid_STRUCT *yaw_angle_pid;
extern _pid_STRUCT *yaw_rate_pid;


void pid_Init(void);
void pid_Cal(float Val,_pid_STRUCT *pid,float LIMIT);
void CtrlAttiRate(void);
void CtrlAttiAng(void);
void Ctrl_Motor(float roll,float pitch,float yaw);


#endif

