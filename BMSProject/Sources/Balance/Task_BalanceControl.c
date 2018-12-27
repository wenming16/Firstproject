/*=======================================================================
 *Subsystem:   裸机
 *File:        Task_BalanceControl.C
 *Author:      WenM
 *Description: 通信：
               接口：
               波特率：
 ========================================================================
 * History:    修改历史记录列表
 * 1. Date:   
      Author:  
      Modification:  
      
========================================================================*/
#include   "Task_BalanceControl.h"
/*=======================================================================
 *函数名:      BalanceControl_Strategy(flaot, uint8, uint16, uint32, uint8, float)
 *功能:        对电池组进行被动均衡
               均衡控制策略:当最大单体大于平均电压balancevolt时进行均衡
 *参数:        curr：判断电流的大小,只当电流小于3A(静态)进行均衡
               faultflg:  在不出现故障的情况下才进行均衡
               voltmax：  最大电压
               totalvolt：25串电池的总压,用于求平均电压
               balacenod: 均衡的节点                 
 
 *返回：       uint8:     0表示正常均衡
                          1表示均衡出现错误参数并停止均衡
                          2表示未达到均衡条件不进行均衡
 *说明：       被动均衡函数
========================================================================*/
static
uint8 BalanceControl_Strategy(float curr, uint8 faultflg, uint16 voltmax, uint32 totalvolt, uint8 balacenod, float balancevolt)
{
  uint8 tskstate=2; //返回2表示未进行均衡
  static uint16 cnt;
  uint8 balanceNum;
  
  if(abs(curr)<3 && (faultflg == 0)) //使其保持在静态的工作状态并且没有出现故障下才进行均衡
  {
    if((voltmax - (totalvolt/25.0)) > balancevolt)
    {
       if(++cnt*balanceperio/1000>5)//持续5s
       {
         if(balacenod <= NUM1_Batper_true)
         {
            tskstate   = LTC6811_BalanceControl(balacenod, 0x00, 0x00, 1); 
         }
         else if(balacenod <= (NUM1_Batper_true+NUM2_Batper_true))
         {
            balanceNum = balacenod-NUM1_Batper_true;
            tskstate   = LTC6811_BalanceControl(0x00, balanceNum, 0x00, 1);
         }
         else if(balacenod <= (NUM1_Batper_true+NUM2_Batper_true+NUM3_Batper_true))
         {
            balanceNum = balacenod-NUM1_Batper_true-NUM2_Batper_true;
            tskstate   = LTC6811_BalanceControl(0x00, 0x00, balanceNum, 1);
         }
         else
         {
            return 1;
         }
       }
    }
  }
  else
  {
     tskstate = LTC6811_BalanceControl(0x00, 0x00, 0x00, 1); //未满足条件不进行均衡 
     return 2;
  }
  return tskstate;
}

/*=======================================================================
 *函数名:      Task_BalanceControl(void)
 *功能:        均衡控制任务
 *参数:        无       
 *返回：       无
 *说明：       均衡控制任务
========================================================================*/
void Task_BalanceControl(void)
{
   uint8 balancestate;
   balancestate = BalanceControl_Strategy(DataColletInfo.DataCollet_Current_Filter, Fault_Charge.Level_Charge_BalanceOff_Flag|Fault_DisCharge.Level_DisCharge_BalanceOff_Flag,\
                                          g_LTC6811_VoltInfo.CellVolt_Max, g_LTC6811_VoltInfo.CellVolt_Total, g_LTC6811_VoltInfo.CellVolt_MaxNode, 0.1);
}

