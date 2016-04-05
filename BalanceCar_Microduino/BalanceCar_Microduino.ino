#include "RollPitch.h"      //姿态传感器库
#include "StepperDriver.h"  //步进电机库
#include <PID_v2.h>         //PID控制库
#include "Protocol.h"       //通讯协议库
#include "userDef.h"        //用户自定义

Stepper stepperL(PIN_DIRA, PIN_STEPA);              //左电机，使用stepper底板A接口
Stepper stepperR(PIN_DIRD, PIN_STEPD);              //右电机，使用stepper底板D接口
PID speedPID((double)KP_SPD, (double)KI_SPD, (double)KD_SPD, DIRECT);     //速度环控制器
PID anglePID((double)KP_ANG, (double)KI_ANG, (double)KD_ANG, DIRECT);     //角度环控制器

int16_t throttle;         //油门值
int16_t steering;         //转向值
float robotSpeedFilter;   //速度估计值
float targetSpeed;        //目标速度
float batteryValue;       //电池电压
float ypr[3];             //Yaw，Pitch，Roll三个轴的角度值

void setup(){
  Serial.begin(115200);
  dmpSetup();             //姿态传感器初始化
  protocolSetup();        //遥控接收器初始化
  
  speedPID.SetMode(AUTOMATIC);    //速度还控制器初始化
  anglePID.SetMode(AUTOMATIC);    //角度环控制器初始化
  speedPID.SetITermLimits(-10, 10);
  speedPID.SetOutputLimits(-MAX_TARGET_ANGLE, MAX_TARGET_ANGLE);
  anglePID.SetOutputLimits(-800, 800); 
  
  stepperL.begin();       //左电机初始化
  stepperR.begin();       //右电机初始化
  for (uint8_t k = 0; k < 3; k++){
    stepperR.setSpeed(3);
    stepperL.setSpeed(-3);
    delay(150);
    stepperR.setSpeed(-3);
    stepperL.setSpeed(3);
    delay(150);
  }
  Serial.println("===========start===========");
}

void loop(){
  if (protocolRead(channel_data, NUM_ROBOT)) {         //判断是否接收到遥控信号
     throttle = map(channel_data[CHANNEL_THROTTLE], 1000, 2000, -MAX_THROTTLE, MAX_THROTTLE);  //更新油门值 
     steering = map(channel_data[CHANNEL_STEERING], 1000, 2000, -MAX_STEERING, MAX_STEERING);  //更新转向值
  }
  batteryValue = (analogRead(A7)/1024.0)*BAT_DETECT;       //采集电池电压
  dmpGetYPR(ypr);         //更新姿态角
  float robotAngle = ypr[DIRECTION] + ANGLE_FIX;          //取Roll轴角度
  float robotSpeed = (stepperL.getSpeed() - stepperR.getSpeed())/2.0;     //计算小车速度
  
  robotSpeedFilter = robotSpeedFilter * 0.95 + robotSpeed * 0.05;         //小车速度滤波
  float targetAngle = speedPID.Compute(robotSpeedFilter, throttle);       //速度环计算目标角度
  
  targetSpeed += anglePID.Compute(robotAngle, targetAngle);         //角度环计算电机转速
  targetSpeed = constrain(targetSpeed, -800, 800);          // 限制最大输出转速
  
  int16_t motorR = targetSpeed + steering;    //计算右电机转速
  int16_t motorL = -targetSpeed + steering;   //计算左电机转速
  motorR = constrain(motorR, -500, 500);    // 限制右电机最大输出转速
  motorL = constrain(motorL, -500, 500);    // 限制左电机最大输出转速
  stepperR.setSpeed(motorR);        //更新右电机转速
  stepperL.setSpeed(motorL);        //更新左电机转速
}


