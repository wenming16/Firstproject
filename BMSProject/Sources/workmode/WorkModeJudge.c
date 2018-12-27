/*=======================================================================
 *Subsystem:   裸机
 *File:        Task_BMSCheckSelf.C
 *Author:      WenYuhao
 *Description: 通信：
               接口：
               波特率：
               只有自检成功后才能闭合响应的开关
/* ========================================================================
 * History:    修改历史记录列表；
 * 1. Date:    
      Author:  
      Modification: 
========================================================================*/
#include "WorkModeJudge.h"  

WorkStateJudge_T  g_WorkStateJudge;
/*=======================================================================
 *函数名:      ChargeJudgment 
 *功能:        判断充电还是放电状态
 *参数:        无       
 *返回：       无
 *说明：       任务时间到标志位给1，10ms执行一次
========================================================================*/
uint8 WokeModeJudgment(void)                                
{  
  //判断车的状态(充电or放电)
  uint8  i=0, q=0;
  uint16 cc2state;
  for(i=0; i<10; i++)
  {
     cc2state = ADC_CC2_State();
     if(cc2state <= 3000)
     {
       q++;
     }
  }   
  if(q == 10)//连续10次是相同的状态
  {
    return MODE_CHARGE;   // ChagMode == 1
  }
  else
  {
    return MODE_DISCHARGE;
  }
}