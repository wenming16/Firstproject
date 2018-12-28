#ifndef _TASK_POWERONOFF_H_
#define _TASK_POWERONOFF_H_
    
    #include  "TypeDefinition.h"
    #include  "Port_Control.h"
    #include  "Task_FaultLevelJudge.h"
    #include  "Task_ChargeCfg.h"
    #include  "DS3231.h"
    #include  "BattInfoParaStructure.h"
    
    //上下电判断函数的执行周期为500ms
    #define   PEWERONOFF_PERIO            500
    //1.继电器开关宏定义
    #define Relay_ON        0  //开
    #define Relay_OFF       1  //关
    
    //2.继电器引脚宏定义
    #define Relay_Positive_PORT            PA  
    #define Relay_Positive_pin              1   
    
    //#define Relay_Negative_PORT          PB
    //#define Relay_Negative_pin            1  
    
    //#define Relay_Quickcharge_PORT       PB
    //#define Relay_Quickcharge_pin         2
    
    #define Relay_CSSUPower_PORT           PA
    #define Relay_CSSUPower_pin             3 
    
    #define Relay_ScreenPower_PORT         PA
    #define Relay_ScreenPower_pin           4 
    
    typedef struct
    {
       uint8  SwitchOff_StartTime;               // 进入休眠时开始的开始计时
    }PowerOff_StartTime_T;
    
    
    void Task_PowerOnOff(void);

#endif