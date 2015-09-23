#include "control.h"

#define Moto_PwmMax 999

_pid_STRUCT *roll_angle_pid={0};
_pid_STRUCT *pitch_angle_pid={0};
_pid_STRUCT *yaw_angle_pid={0};

#if (PID_MODE != single_PID)	
_pid_STRUCT *roll_rate_pid={0};
_pid_STRUCT *pitch_rate_pid={0};
_pid_STRUCT *yaw_rate_pid={0};
_out_angle *_tangle;
#endif

_rc_data RC_DATA;		//accept or set vaule
//_out_geo _gGRYO;		//
//_out_angle imu;			//attitude solver
//_out_geo _gACCEL;		//

float Roll;				//angle control PWM
float Pitch;
float Yaw;


void pid_Init(void)
{
	 roll_angle_pid = ( _pid_STRUCT*)malloc(sizeof(_pid_STRUCT));
 	 yaw_angle_pid = ( _pid_STRUCT*)malloc(sizeof(_pid_STRUCT));
	 pitch_angle_pid = ( _pid_STRUCT*)malloc(sizeof(_pid_STRUCT));
 #if  (PID_MODE != single_PID)
	roll_rate_pid  = 	( _pid_STRUCT*)malloc(sizeof(_pid_STRUCT));
	 pitch_rate_pid  = ( _pid_STRUCT*)malloc(sizeof(_pid_STRUCT));
	 yaw_rate_pid  = ( _pid_STRUCT*)malloc(sizeof(_pid_STRUCT));
	  _tangle  = ( _out_angle*)malloc(sizeof(_out_angle));            //内环输入传递函数
#endif

	roll_angle_pid->Kp=10;
	roll_angle_pid->Ki=0.0;
	roll_angle_pid->Kd=0.0;
	
	pitch_angle_pid->Kp=10;
	pitch_angle_pid->Ki=0.0;
	pitch_angle_pid->Kd=0.0;

	yaw_angle_pid->Kp=0;
	yaw_angle_pid->Ki=0.0;
	yaw_angle_pid->Kd=0.0;	

#if  (PID_MODE != single_PID)
	roll_rate_pid->Kp=1;//70;
	roll_rate_pid->Ki=0;
	roll_rate_pid->Kd=0;
	
	pitch_rate_pid->Kp=1;//70;
	pitch_rate_pid->Ki=0.0;
	pitch_rate_pid->Kd=0.0;

	yaw_rate_pid->Kp=1;
	yaw_rate_pid->Ki=0.0;
	yaw_rate_pid->Kd=0.0;
#endif

	RC_DATA.pitch=0;
	RC_DATA.roll=0;
	RC_DATA.yaw=0;
	
}

//积分分离PID
void pid_Cal(float Val,_pid_STRUCT *pid,float LIMIT){
	u8 index;
    pid->SetVal=Val;
    pid->err=pid->SetVal-pid->ActualVal;	
   if(abs(pid->err)>LIMIT)
    {
    index=0;
    }else{
    index=1;
    pid->integral+=pid->err;
    }
    pid->voltage=pid->Kp*pid->err+index*pid->Ki*pid->integral+pid->Kd*(pid->err-pid->err_last);    //算法具体实现过程
//    pid->integral+=pid->err;
//    pid->voltage=pid->Kp*pid->err+pid->Ki*pid->integral+pid->Kd*(pid->err-pid->err_last);
    pid->err_last=pid->err;
    pid->ActualVal=pid->voltage*1.0;
}



#define angle_max           10.0f      
#define angle_integral_max  50.0f   
void CtrlAttiAng(void)
{

	_out_angle control_angle;    
	
	 roll_angle_pid->ActualVal=imu.roll;
	 pitch_angle_pid->ActualVal=imu.pitch;						//姿态解算出来的angle 
	

  control_angle.roll  = (float)(RC_DATA.roll) ;   
  control_angle.pitch = (float)(RC_DATA.pitch);   

	pid_Cal(control_angle.roll,roll_angle_pid,angle_integral_max);
	pid_Cal(control_angle.pitch,pitch_angle_pid,angle_integral_max);
	
	
#if (PID_MODE == single_PID)	

	Roll=roll_angle_pid->ActualVal;
	Pitch=pitch_angle_pid->ActualVal;
	
#else
	_tangle->roll=roll_angle_pid->ActualVal;					//内环的输入
	_tangle->pitch=pitch_angle_pid->ActualVal;					
#endif 

/*	  if(_tangle->roll >  angle_max)   
       _tangle->roll =  angle_max;   
    if(_tangle->roll < -angle_max)   
      _tangle->roll = -angle_max;  
		
		if(_tangle->pitch >  angle_max)   
       _tangle->pitch =  angle_max;   
    if(_tangle->pitch < -angle_max)   
      _tangle->pitch = -angle_max;  
	*/	
}

#if (PID_MODE != single_PID)	
#define gyro_max            50.0f      
#define gyro_integral_max   300.0f   
#define DMP_GYRO_SCALE 16.4f	//2000deg/s , 31276/2000=16.4f 
#define Radian_to_Angle 57.3f

void CtrlAttiRate(void)   
{   
    static  _out_angle control_gyro={0};   

/*
	gyro->roll=_gGRYO.z/DMP_GYRO_SCALE/Radian_to_Angle;   //注：这里是三轴角速度。
	gyro->pitch=_gGRYO.x/DMP_GYRO_SCALE/Radian_to_Angle;
	gyro->yaw=_gGRYO.y/DMP_GYRO_SCALE/Radian_to_Angle;
*/
	roll_rate_pid->ActualVal = imu.gyro[2]*Radian_to_Angle;		//注意正确读取! 
	pitch_rate_pid->ActualVal= imu.gyro[0]*Radian_to_Angle;     //陀螺仪的参数
	yaw_rate_pid->ActualVal  = imu.gyro[1]*Radian_to_Angle;
	
    control_gyro.roll  = _tangle->roll;        
    control_gyro.pitch = _tangle->pitch;   
    control_gyro.yaw   = (float)RC_DATA.yaw; 

	pid_Cal(control_gyro.roll,roll_rate_pid,gyro_integral_max);
	pid_Cal(control_gyro.yaw,yaw_rate_pid,gyro_integral_max);
	pid_Cal(control_gyro.pitch,pitch_rate_pid,gyro_integral_max);
 
 
    Pitch = pitch_rate_pid->ActualVal;
    Roll  = roll_rate_pid->ActualVal;
    Yaw   = yaw_rate_pid->ActualVal; 

#ifdef DEBUG_PID
	printf("***接收值打印***\n");
	printf(" RC.roll=%f    RC.yaw=%f    RC.pitch=%f\n",RC_DATA.roll,RC_DATA.yaw,RC_DATA.pitch);
	printf("***欧拉角测量值打印***\n");
	printf(" imu.roll=%f      imu.yaw=%f     imu.pitch=%f\n",imu.roll,imu.yaw,imu.pitch);
	printf("***角度外环PID输出打印***\n");
	printf(" roll=%f\n  pitch=%f\n",_tangle->roll,_tangle->pitch);
	printf("***角速度内环值输入打印***\n");
	printf(" roll_rate=%f    yaw_rate=%f    pitch_rate=%f\n ",imu.gyro[2],imu.gyro[1],imu.gyro[0]);
	printf("***角速度内环值输出打印***\n");
	printf(" roll=%f     yaw=%f    pitch=%f\n ",Roll,Yaw,Pitch);
	printf("******打印结束******\n\n");
#endif
}  

#endif 


void Ctrl_Motor(float roll,float pitch,float yaw)
{
	u16 MOTO1_PWM,MOTO2_PWM,MOTO3_PWM,MOTO4_PWM;
	MOTO1_PWM=roll-pitch-yaw;
	MOTO2_PWM=roll-pitch-yaw;
	MOTO3_PWM=roll+pitch+yaw;
	MOTO4_PWM=roll+pitch+yaw;
	
     if(MOTO1_PWM>=Moto_PwmMax)	MOTO1_PWM = Moto_PwmMax;
     if(MOTO2_PWM>=Moto_PwmMax)	MOTO2_PWM = Moto_PwmMax;
     if(MOTO3_PWM>=Moto_PwmMax)	MOTO3_PWM = Moto_PwmMax;
     if(MOTO4_PWM>=Moto_PwmMax)	MOTO4_PWM = Moto_PwmMax;
     if(MOTO1_PWM<=0)	MOTO1_PWM = 0;
     if(MOTO2_PWM<=0)	MOTO2_PWM = 0;
     if(MOTO3_PWM<=0)	MOTO3_PWM = 0;
     if(MOTO4_PWM<=0)	MOTO4_PWM = 0;//限定输入不能小于0，大于999
     
#ifdef DEBUG_PID
	printf("***MOTO_PWM值打印***\n");
	printf("MOTO1_PWM=%d\n",MOTO1_PWM);
	printf("MOTO2_PWM=%d\n",MOTO2_PWM);
	printf("MOTO3_PWM=%d\n",MOTO3_PWM);
	printf("MOTO4_PWM=%d\n",MOTO4_PWM);
	printf(" final.roll=%f\n  final.yaw=%f\n final.pitch=%f\n ",roll,yaw,pitch);
	printf("******打印结束******\n\n");
 
#else
	TIM_SetCompare1(TIM3,MOTO1_PWM);	 
	TIM_SetCompare2(TIM3,MOTO2_PWM);	
	TIM_SetCompare3(TIM3,MOTO3_PWM);	
	TIM_SetCompare4(TIM3,MOTO4_PWM);	
#endif
	
}




