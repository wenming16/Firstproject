/*=======================================================================
 *Subsystem:   裸机
 *File:        Task_CurrentLimit.C
 *Author:      Wenming
 *Description: 通信：
               接口：
               波特率：
 ========================================================================
 * History:    修改历史记录列表
 * 1. Date:
      Author:
      Modification:
========================================================================*/
 #include"TypeDefinition.h"
 #include"Task_CurrentLimit.h"
 #include"BattInfoParaStructure.h"
 
 CurrentLimit_T CurrentLimit;

 static float ChagCurrLimit_Constant(uint8 Temp);
/*=======================================================================
 *函数名:      Dis_ChargeCurrent
 *功能:        充放电电流判断
 *参数:        无                                     
 *返回：       无
 *说明：       BMS根据温度条件判断最大输出或充电电流的大小
========================================================================*/ 
void Task_CurrentLimit(void)
{
  //充电电流限制
  CurrentLimit.Current_Charge_Constant = ChagCurrLimit_Constant(TempInfo.CellTemp_Ave);
    
  //Task_Flag_Counter.Counter_ChargeCurrent++;

} 

/*=======================================================================
 *函数名:      Current_Charge_Constant()
 *功能:        充放电电流判断
 *参数:        无                                     
 *返回：       无
 *说明：       BMS根据温度条件判断最大输出或充电电流的大小
========================================================================*/ 
static
float ChagCurrLimit_Constant(uint8 Temp)
{
  static float Current;
  
  if(Temp>=0 && Temp<=55)
  {
    if(Temp>50)
    {
      Current = 100;
    }
    else if(Temp>10)
    {
      Current = 150;
    }
    else if(Temp>5)
    {
      Current = 100;
    }
    else 
    {
      Current = 50;
    }
  }
  else
  {
    Current = 0;
  }
  return Current;
}