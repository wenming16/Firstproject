/*=======================================================================
 *Subsystem:   裸机
 *File:        PIT.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef _PIT_H_
#define _PIT_H_  

  #include"TypeDefinition.h"

//开放或禁止PIT0溢出中断
    #define EnablePIT(channel)           PITINTE |= (1<<channel)    //开放定时器溢出中断
    #define DisablePIT(channel)          PITINTE &= ~(1<<channel)  //禁止定时器溢出中断

  //PIT初始化
  enum Init_PIT
  {
    //PIT初始化正常
    Init_Normal_PIT = 0,
    Init_Fault_PIT_Channel,
    Init_Fault_PIT_Period
  };

  typedef struct 
  {
    uint16 Period_10ms;       // 10ms任务时间标志;
    uint16 Period_50ms;       // 50ms任务时间标志;
    uint16 Period_100ms;      // 100ms任务时间标志;
    uint16 Period_200ms;      // 200ms任务时间标志;
    uint16 Period_250ms;      // 250ms任务时间标志;
    uint16 Period_500ms;      // 500ms任务时间标志;
    uint16 Period_1s;         // 1s任务时间标志; 
  }PIT_Time_10ms_T;
  extern PIT_Time_10ms_T PIT_Time_10ms;  



//构件函数声明区

    //TimerInit:定时器初始化函数--------------------------------------------*
    //功  能:定时器初始化，中断一次时间为1/2秒                              *
    //参  数:channel==0~7, MUXSEL:0-选择微时基0；1-选择微时基1
    // MTLD:填入微定时器加载寄存器的值， LD：填入通道加载寄存器的值         *                                                             *
    //返  回:无                                                             *
    //----------------------------------------------------------------------*
    void PITInit(uint8 channel,uint8 MUXSEL,uint8 MTLD,uint16 LD);
    uint8 Init_PIT(uint8 channel, uint8 perio);
   // void init_PIT0(void);
   // void init_PIT1(void);
    //----------------------------------------------------------------------*
    //函数名: SecAdd1(计时函数)                                             *
    //功  能: 以秒为最小单位递增时,分,秒缓冲区的值(00:00:00-23:59:59)       *
    //参  数: *p:计数变量的首地址                                           *
    //返  回: 无                                                            *
    //----------------------------------------------------------------------*

#endif                                                                           
