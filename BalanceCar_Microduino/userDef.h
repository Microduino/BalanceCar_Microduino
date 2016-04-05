
/********************姿态模块安装方向选择**********************/
#define YAW     0           //偏航方向   
#define PITCH   1           //俯仰方向
#define ROLL    2           //滚转方向
#define DIRECTION   ROLL    //姿态传感器安装方向   YAW/PITCH/ROLL

/*******************平衡车PID参数整定************************/
#define ANGLE_FIX -3.5      //角度修正参数
#define MAX_TARGET_ANGLE 12 //最大目标角度12

#define KP_ANG 0.15         //角度环比例参数
#define KI_ANG 0            //角度环积分参数          
#define KD_ANG 35           //角度环微分参数
#define KP_SPD 0.05         //速度环比例参数
#define KI_SPD 0.08         //速度环积分参数
#define KD_SPD 0            //速度环微分参数

/*******************电池电压检测参数*************************/
#define BAT_DETECT   5*84/33.0     //电池电压AD换算比例

/*******************遥控信号配置参数*************************/
#define NRF_CHANNEL 70        //nRF通道 
#define BLE_SPEED 9600        //蓝牙接口速度

#define CHANNEL_THROTTLE  3   //油门通道 
#define CHANNEL_STEERING  0   //转向通道 

#define MAX_THROTTLE 480      //最大油门
#define MAX_STEERING 120      //最大转向


