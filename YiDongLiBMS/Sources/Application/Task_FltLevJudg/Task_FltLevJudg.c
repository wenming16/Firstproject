/*=======================================================================
 *Subsystem:   裸机
 *File:        Task_FltLevJudg.c
 *Author:      Wenming
 *Description: 
 ========================================================================
 * History:    修改历史记录列表，修改记录包括修改日期、修改者及修改内容
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#include  "Task_FltLevJudg.h"
#include  "WorkModeJudge.h"
#include  "Task_DataProcess.h"
#include  "Task_InsulDetect.h"
#include  "FltLevcfg.h"
#include  "Task_UpMonitor.h"

/*=======================================================================
                                  声明
========================================================================*/ 
Fault_DisCharge_T g_Flt_DisChg; 
Fault_Charge_T    g_Flt_Charge;
State_Offline_T   State_Offline;

 //放电故障判断
 static uint8 Fault_DisChg_VoltS(uint32 Volt,uint8 Temp); 
 static uint8 Fault_DisChg_VoltC(uint16 Volt,uint8 Temp);
 static uint8 Fault_DisChg_VoltCD(uint16 V_max,uint16 V_min);
 static uint8 Fault_DisChg_TempH(uint8 Temp);
 static uint8 Fault_DisChg_TempL(uint8 Temp);
 static uint8 Fault_DisChg_TempD(uint8 T_max,uint8 T_min);
 static uint8 Fault_DisChg_CurrH(float Current);
 static uint8 Fault_DisChg_Insul(uint16 Insul);
 //充电故障判断
 static uint8 Fault_Charge_VoltS(uint32 Volt);
 static uint8 Fault_Charge_VoltCH(uint16 Volt);
 static uint8 Fault_Charge_VoltCD(uint16 V_max,uint16 V_min);
 static uint8 Fault_Charge_TempH(uint8 Temp);
 static uint8 Fault_Charge_TempL(uint8 Temp);
 static uint8 Fault_Charge_TempD(uint8 T_max,uint8 T_min);
 static uint8 Fault_Charge_CurrH(float Current);
 static uint8 Fault_Charge_Insul(uint16 Insul);

 /*=======================================================================
                              故障判断函数0x00
 ========================================================================
 *函数名1:     Init_TaskFltLevJudg(void)
 *功能:        
 *参数:        无    
 *返回：       无
 
 *说明：       
========================================================================*/ 
void Init_TaskFltLevJudg(void)
{
  //故障等级判断变量清零
  memset(&g_Flt_DisChg,           0, sizeof(Fault_DisCharge_T));  //放电故障等级清零
  memset(&g_Flt_Charge,           0, sizeof(Fault_Charge_T));        //充电故障等级清零
  memset(&State_Offline,          0, sizeof(State_Offline_T));        //掉线故障状态清零
  memset(&HeartBeat,              0, sizeof(HeartBeat_T));        //心跳信号清零
}



/*=======================================================================
                              故障判断函数0x00
 ========================================================================
 *函数名1:     Task_FaultLevelJudge(uint8 workstate)
 *功能:        根据系统电压、温度、电流判断故障等级
 *参数:        无    
 *返回：       无
 
 *说明：       
========================================================================*/ 
void Task_FltLevJudg(uint8 workstate)
{
   switch(workstate)
   {
    case MODE_DISCHARGE: //放电状态
      //清除充电状态故障
      g_Flt_Charge.Level_Charge_SwitchOff_flag = 0; 
      g_Flt_Charge.Level_Charge_BalanceOff_Flag = 0;
      
      //判断放电状态故障
      g_Flt_DisChg.Level_Volt_Sys_Low           = Fault_DisChg_VoltS(g_DataColletInfo.SysVolt_Total, g_TempInfo.CellTemp_Ave);
      g_Flt_DisChg.Level_Volt_Cell_Low          = Fault_DisChg_VoltC(g_VoltInfo.CellVolt_Min, g_TempInfo.CellTemp_Ave);
      g_Flt_DisChg.Level_Volt_Cell_Diff_High    = Fault_DisChg_VoltCD(g_VoltInfo.CellVolt_Max, g_VoltInfo.CellVolt_Min);
      g_Flt_DisChg.Level_Temp_High              = Fault_DisChg_TempH(g_TempInfo.CellTemp_Max);
      g_Flt_DisChg.Level_Temp_Low               = Fault_DisChg_TempL(g_TempInfo.CellTemp_Min);
      g_Flt_DisChg.Level_Temp_Diff_High         = Fault_DisChg_TempD(g_TempInfo.CellTemp_Max, g_TempInfo.CellTemp_Min);
      g_Flt_DisChg.Level_Current_DisCharge_High = Fault_DisChg_CurrH(g_DataColletInfo.DataCollet_Current_Filter);
      g_Flt_DisChg.Level_Insul                  = Fault_DisChg_Insul(g_IsoDetect.insulation_resist);
      //断开继电器的二级故障标记
      if((g_Flt_DisChg.Level_Volt_Sys_Low==2) || \
         (g_Flt_DisChg.Level_Volt_Cell_Low == 2)||\
         (g_Flt_DisChg.Level_Temp_High == 2)||\
         (g_Flt_DisChg.Level_Temp_Low == 2) ||\
         (g_Flt_DisChg.Level_Current_DisCharge_High == 2) ||\
         (g_Flt_DisChg.Level_Insul == 2))
      {
        g_Flt_DisChg.Level_DisCharge_SwitchOff_flag = 1;  
      }
      
    break;
    
    case MODE_CHARGE:   //充电状态
      //清除放电状态故障
      g_Flt_DisChg.Level_Volt_Sys_Low           = 0;
      g_Flt_DisChg.Level_Volt_Cell_Low          = 0;
      g_Flt_DisChg.Level_DisCharge_SwitchOff_flag = 0;
      
      //判断充电状态故障
      g_Flt_Charge.Level_Volt_Sys_High        = Fault_Charge_VoltS(g_DataColletInfo.SysVolt_Total);
      g_Flt_Charge.Level_Volt_Cell_High       = Fault_Charge_VoltCH(g_VoltInfo.CellVolt_Max);
      g_Flt_Charge.Level_Volt_Cell_Diff_High  = Fault_Charge_VoltCD(g_VoltInfo.CellVolt_Max, g_VoltInfo.CellVolt_Min);
      g_Flt_Charge.Level_Temp_High            = Fault_Charge_TempH(g_TempInfo.CellTemp_Max);
      g_Flt_Charge.Level_Temp_Low             = Fault_Charge_TempL(g_TempInfo.CellTemp_Min);
      g_Flt_Charge.Level_Temp_Diff_High       = Fault_Charge_TempD(g_TempInfo.CellTemp_Max, g_TempInfo.CellTemp_Min);
      g_Flt_Charge.Level_Current_Charge_High  = Fault_Charge_CurrH(g_DataColletInfo.DataCollet_Current_Filter);
      g_Flt_Charge.Level_Insul                = Fault_Charge_Insul(g_IsoDetect.insulation_resist);
      //断开继电器的二级故障标记
      if((g_Flt_Charge.Level_Volt_Sys_High==2) || \
         (g_Flt_Charge.Level_Volt_Cell_High==2) || \
         (g_Flt_Charge.Level_Temp_High == 2)||\
         (g_Flt_Charge.Level_Temp_Low == 2)||\
         (g_Flt_Charge.Level_Insul == 2))
      {
        g_Flt_Charge.Level_Charge_SwitchOff_flag = 1;  
      }
      //均衡
      if((g_Flt_Charge.Level_Volt_Sys_High!=0) || \
         (g_Flt_Charge.Level_Volt_Cell_High != 0)||\
         (g_Flt_Charge.Level_Volt_Cell_Diff_High != 0)||\
         (g_Flt_Charge.Level_Temp_High != 0)||\
         (g_Flt_Charge.Level_Temp_Low != 0) ||\
         (g_Flt_Charge.Level_Temp_Diff_High != 0) ||\
         (g_Flt_Charge.Level_Current_Charge_High != 0) ||\
         (g_Flt_Charge.Level_Insul != 0))
      {
        g_Flt_Charge.Level_Charge_BalanceOff_Flag = 1;  
      }
      else
      {
        g_Flt_Charge.Level_Charge_BalanceOff_Flag = 0;
      }
      
    break;
   }
   
   //任务运行计数标记
  //Task_Flag_Cnt.F_FaultLevelJudge++;
  
}
/*============================故障判断函数===============================*/

/*=======================================================================
                              放电总压低0x01
 ========================================================================
 *函数名1:     Fault1_VoltSys_DisCharge(uint8 Temp)
 *功能:        根据温度选择放电总压1级故障阈值
 *参数:        Temp (输入温度  输入偏移量:-40)       
 *返回：       等级1电压阈值 
 
 *函数名2:     Fault2_VoltSys_DisCharge(uint8 Temp)
 *功能:        根据温度选择放电总压2级故障阈值
 *参数:        Temp (输入温度  输入偏移量:-40)       
 *返回：       等级2电压阈值 
 
 *函数名3:     Recover1_VoltSys_DisCharge(uint8 Temp)
 *功能:        根据温度选择放电总压1级恢复故障阈值
 *参数:        Temp (输入温度  输入偏移量:-40)       
 *返回：       等级1电压阈值 
 
 *函数名4:     Fault_DisChg_VoltS(uint32 Volt,uint8 Temp)
 *功能:        根据系统电压、温度判断故障等级
 *参数:        Volt(系统电压)
               Temp (输入温度  输入偏移量:-40)       
 *返回：       放电总压低的故障等级
 
 *说明：       
========================================================================*/ 
//-------------------根据温度确定故障等级以及恢复等级阈值-----------------
//1级故障函数
static
uint32 Fault1_VoltSys_DisCharge(uint8 Temp)
{
  if(Temp>=NTEMP_BOUNDARY)//常温(>=0℃)
  { 
    return (g_BMSMonitor_Volt.Volt_Sys_Low1*1000);//(28500)*(25)
  }
  else //低温
  {
    return (g_BMSMonitor_New_LT.Voll_Sys_Low1_LT*1000);
  }
}
//2级故障函数
static
uint32 Fault2_VoltSys_DisCharge(uint8 Temp)
{
  if(Temp>=NTEMP_BOUNDARY)//常温(>=0℃)
  { 
    return (g_BMSMonitor_Volt.Volt_Sys_Low2*1000);
  }
  else //低温
  {
    return (g_BMSMonitor_New_LT.Voll_Sys_Low2_LT*1000);
  }
}
//1恢复至0
static
uint32 Recover1_VoltSys_DisCharge(uint8 Temp)
{
  if(Temp>=NTEMP_BOUNDARY)//常温(>=0℃)
  { 
    return (g_BMSMonitor_Volt.Volt_Sys_Low1*1000 + 2000*SYS_SERIES_YiDongLi);
  }
  else //低温
  {
    return (g_BMSMonitor_New_LT.Voll_Sys_Low1_LT*1000 + 1000);//可换成标定变量(分辨率：0.001V/位)
  }
}

//-----------------根据输入的系统电压判断最终的故障等级------------------
static uint8 Fault_DisChg_VoltS(uint32 Volt,uint8 Temp)  //输入系统电压和环境温度
{
  static uint8 cnt[4];      //时间计数
  static uint8 FltL;
  
  if(FltL==0)           //0级故障
  {
    if(Volt<=Fault1_VoltSys_DisCharge(Temp))  //0变1
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 1;
      }
    }
    else
    {
      cnt[0] = 0;
    }
    
    if(Volt<=Fault2_VoltSys_DisCharge(Temp))  //0变2
    {
      if(++cnt[1]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[1] = 0;
        FltL = 2;
      }
    }
    else
    {
      cnt[1] = 0;
    }
    cnt[2] = 0; 
    cnt[3] = 0;
  }
  else if(FltL == 1) //1级故障
  {
    if(Volt <= Fault2_VoltSys_DisCharge(Temp))      //1变2
    {
      if(++cnt[2]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
         cnt[2] = 0;
         FltL = 2;
      }
    }
    else
    {
      cnt[2] = 0;
    }
    
    if(Volt >= Recover1_VoltSys_DisCharge(Temp))    //1变0
    {
      if(++cnt[3]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[3] = 0;
        FltL = 0;
      }
    }
    else
    {
      cnt[3] = 0;
    }
    cnt[0] = 0; 
    cnt[1] = 0;
  }
  else   //2级故障
  {
    cnt[0] = 0; 
    cnt[1] = 0;
    cnt[2] = 0;
    cnt[3] = 0; 
  }
  return(FltL);
}
/*============================放电总压低===============================*/

/*=======================================================================
                              放电单体电压低0x02
 ========================================================================
 *函数名1:     Fault1_VoltCell_DisCharge(uint8 Temp)
 *功能:        根据温度选择放电单压1级故障阈值
 *参数:        Temp (输入温度  输入偏移量:-40)       
 *返回：       等级1电压阈值 
 
 *函数名2:     Fault2_VoltCell_DisCharge(uint8 Temp)
 *功能:        根据温度选择放电单压2级故障阈值
 *参数:        Temp (输入温度  输入偏移量:-40)       
 *返回：       等级2电压阈值 
 
 *函数名3:     Recover1_VoltCell_DisCharge(uint8 Temp)
 *功能:        根据温度选择放电单压1级恢复故障阈值
 *参数:        Temp (输入温度  输入偏移量:-40)       
 *返回：       等级1电压阈值 
  
 *函数名4:     Fault_DisChg_VoltC(uint16 Volt,uint8 Temp)
 *功能:        根据单体电压、温度判断恢复后故障等级
 *参数:        Volt(单体电压)
               Temp (输入温度  输入偏移量:-40)       
 *返回：       放电单压低的故障等级
 
 *说明：       
========================================================================*/ 
//1级故障函数
static
uint16 Fault1_VoltCell_DisCharge(uint8 Temp)
{
  if(Temp>=NTEMP_BOUNDARY)//常温(>=0℃)
  { 
    return (g_BMSMonitor_Volt.Volt_Cell_Low1);
  }
  else //低温
  {
    return (g_BMSMonitor_New_LT.Volt_Cell_Low1_LT);
  }
}
//2级故障函数
static
uint16 Fault2_VoltCell_DisCharge(uint8 Temp)
{
  if(Temp>=NTEMP_BOUNDARY)//常温(>=0℃)
  { 
    return (g_BMSMonitor_Volt.Volt_Cell_Low2);
  }
  else //低温
  {
    return (g_BMSMonitor_New_LT.Volt_Cell_Low2_LT);
  }
}
//恢复至0
static
uint16 Recover1_VoltCell_DisCharge(uint8 Temp)
{
  if(Temp>=NTEMP_BOUNDARY)//常温(>=0℃)
  { 
    return (g_BMSMonitor_Volt.Volt_Cell_Low1 + 2000);
  }
  else //低温
  {
    return (g_BMSMonitor_New_LT.Volt_Cell_Low1_LT + 1000);
  }
}

//判断故障等级
static uint8 Fault_DisChg_VoltC(uint16 Volt,uint8 Temp)  //输入单体电压和环境温度
{
  static uint8 cnt[4];      //时间计数
  static uint8 FltL;
  
  if(FltL==0)           //0级故障
  {
    if(Volt<=Fault1_VoltCell_DisCharge(Temp))  //0变1
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 1;
      }
    }
    else
    {
      cnt[0] = 0;
    }
    
    if(Volt<=Fault2_VoltCell_DisCharge(Temp))  //0变2
    {
      if(++cnt[1]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[1] = 0;
        FltL = 2;
      }
    }
    else
    {
      cnt[1] = 0;
    }
    cnt[2] = 0; 
    cnt[3] = 0;
  }
  else if(FltL == 1) //1级故障
  {
    if(Volt <= Fault2_VoltCell_DisCharge(Temp))      //1变2
    {
      if(++cnt[2]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
         cnt[2] = 0;
         FltL = 2;
      }
    }
    else
    {
      cnt[2] = 0;
    }
    
    if(Volt >= Recover1_VoltCell_DisCharge(Temp))    //1变0
    {
      if(++cnt[3]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[3] = 0;
        FltL = 0;
      }
    }
    else
    {
      cnt[3] = 0;
    }
    cnt[0] = 0; 
    cnt[1] = 0;
  }
  else   //2级故障
  {
    cnt[0] = 0; 
    cnt[1] = 0;
    cnt[2] = 0;
    cnt[3] = 0; 
  }
  return(FltL);
}
/*============================放电单体电压低===========================*/

/*=======================================================================
                              放电单体压差0x03
 ========================================================================
 *函数名1:     Fault_DisChg_VoltCD(uint16 V_max,uint16 V_min)
 *功能:        根据最高/低单体电压判断压差故障等级
 *参数:        V_max(单体最高电压)
               V_min(单体最低电压)       
 *返回：       放电压差的故障等级
 
 *说明：       
========================================================================*/ 
//判断故障等级
static uint8 Fault_DisChg_VoltCD(uint16 V_max,uint16 V_min)  //输入单体最高/低电压
{
  static uint16 V_Diff;
  static uint8 cnt[2];      //时间计数
  static uint8 FltL;
  
  V_Diff = V_max - V_min;   //求最大压差
  
  //判断故障等级
  if(FltL==0)           //0级故障
  {
    if(V_Diff>=g_BMSMonitor_Volt.Volt_Cell_Diff1)  //0变1
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 1;
      }
    }
    else
    {
      cnt[0] = 0; 
    }
    cnt[1] = 0; 
  }
  else     //1级故障
  {
    if(V_Diff<=(V_Diff>=g_BMSMonitor_Volt.Volt_Cell_Diff1 - 500))    //1变0
    {
      if(++cnt[1]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[1] = 0;
        FltL = 0;
      }
    }
    else
    {
      cnt[1] = 0;
    }
    cnt[0] = 0; 
  }
  return(FltL);
}
/*============================放电单体压差============================*/

/*=======================================================================
                                放电高温0x04
 ========================================================================
 *函数名1:     Fault_DisChg_TempH(uint8 Temp)
 *功能:        根据温度判断高温故障等级
 *参数:        Temp (输入温度  输入偏移量:-40)       
 *返回：       放电单压低的故障等级
 
 *说明：       
========================================================================*/ 
//判断故障等级
static uint8 Fault_DisChg_TempH(uint8 Temp)  //输入温度
{
  static uint8 cnt[4];      //时间计数
  static uint8 FltL;
  
  if(FltL==0)           //0级故障
  {
    if(Temp>=g_BMSMonitor_Temp.Temp_DisCharge_High1)  //0变1
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 1;
      }
    }
    else
    {
      cnt[0] = 0;
    }
    
    if(Temp>=g_BMSMonitor_Temp.Temp_DisCharge_High2)  //0变2
    {
      if(++cnt[1]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[1] = 0;
        FltL = 2;
      }
    }
    else
    {
      cnt[1] = 0;
    }
    cnt[2] = 0; 
    cnt[3] = 0;
  }
  else if(FltL == 1) //1级故障
  {
    if(Temp>=g_BMSMonitor_Temp.Temp_DisCharge_High2)      //1变2
    {
      if(++cnt[2]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
         cnt[2] = 0;
         FltL = 2;
      }
    }
    else
    {
      cnt[2] = 0;
    }
    
    if(Temp<=(Temp>=g_BMSMonitor_Temp.Temp_DisCharge_High1 - 2))    //1变0
    {
      if(++cnt[3]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[3] = 0;
        FltL = 0;
      }
    }
    else
    {
      cnt[3] = 0;
    }
    cnt[0] = 0; 
    cnt[1] = 0;
  }
  else   //2级故障
  {
    cnt[0] = 0; 
    cnt[1] = 0;
    cnt[2] = 0;
    cnt[3] = 0; 
  }
  return(FltL);
}
/*==============================放电高温==============================*/

/*=======================================================================
                                放电低温0x05
 ========================================================================
 *函数名1:     Fault_DisChg_TempL(uint8 Temp)
 *功能:        根据温度判断低温故障等级
 *参数:        Temp (输入温度  输入偏移量:-40)       
 *返回：       放电单压低的故障等级
 
 *说明：       
========================================================================*/ 
//判断故障等级
static uint8 Fault_DisChg_TempL(uint8 Temp)  //输入温度
{
  static uint8 cnt[4];      //时间计数
  static uint8 FltL;
  
  if(FltL==0)           //0级故障
  {
    if(Temp<=g_BMSMonitor_Temp.Temp_DisCharge_Low1)  //0变1
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 1;
      }
    }
    else
    {
      cnt[0] = 0;
    }
    
    if(Temp<=g_BMSMonitor_Temp.Temp_DisCharge_Low2)  //0变2
    {
      if(++cnt[1]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[1] = 0;
        FltL = 2;
      }
    }
    else
    {
      cnt[1] = 0;
    }
    cnt[2] = 0; 
    cnt[3] = 0;
  }
  else if(FltL == 1) //1级故障
  {
    if(Temp<=g_BMSMonitor_Temp.Temp_DisCharge_Low2)      //1变2
    {
      if(++cnt[2]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
         cnt[2] = 0;
         FltL = 2;
      }
    }
    else
    {
      cnt[2] = 0;
    }
    
    if(Temp>=(Temp<=g_BMSMonitor_Temp.Temp_DisCharge_Low1 + 3))    //1变0
    {
      if(++cnt[3]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[3] = 0;
        FltL = 0;
      }
    }
    else
    {
      cnt[3] = 0;
    }
    cnt[0] = 0; 
    cnt[1] = 0;
  }
  else   //2级故障
  {
    cnt[0] = 0; 
    cnt[1] = 0;
    cnt[2] = 0;
    cnt[3] = 0; 
  }
  return(FltL);
}
/*==============================放电低温==============================*/

/*=======================================================================
                              放电温差0x06
 ========================================================================
 *函数名1:     Fault_DisChg_TempD(uint8 T_max,uint8 T_min)
 *功能:        根据最高/低温度判断温差故障等级
 *参数:        V_max(单体最高温度)
               V_min(单体最低温度)       
 *返回：       放电温差的故障等级
 
 *说明：       
========================================================================*/ 
//判断故障等级
static uint8 Fault_DisChg_TempD(uint8 T_max,uint8 T_min)  //输入单体最高/低电压
{
  static uint8 T_Diff;
  static uint8 cnt[2];      //时间计数
  static uint8 FltL;
  
  T_Diff = T_max - T_min;   //求最大压差
  
  //判断故障等级
  if(FltL==0)           //0级故障
  {
    if(T_Diff>=(g_BMSMonitor_Temp.Temp_DisCharge_Diff1-40))  //0变1
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 1;
      }
    }
    else
    {
      cnt[0] = 0; 
    }
    cnt[1] = 0; 
  }
  else     //1级故障
  {
    if(T_Diff<=(g_BMSMonitor_Temp.Temp_DisCharge_Diff1-40 - 2))    //1变0
    {
      if(++cnt[1]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[1] = 0;
        FltL = 0;
      }
    }
    else
    {
      cnt[1] = 0;
    }
    cnt[0] = 0; 
  }
  return(FltL);
}
/*============================放电单体温差============================*/

/*=======================================================================
                                放电过流0x07
 ========================================================================
 *函数名1:     Fault_DisChg_CurrH(float Current)
 *功能:        根据电流判断过压故障等级
 *参数:        Current (输入温度  输入偏移量:-40)       
 *返回：       放电过压的故障等级
 
 *说明：       
========================================================================*/ 
//判断故障等级
static uint8 Fault_DisChg_CurrH(float Current)//输入电流
{
  static uint8 cnt[4];      //时间计数
  static uint8 FltL;
  
  if(FltL==0)           //0级故障
  {
    if(Current>=(g_BMSMonitor_Curr.Current_DisCharge_High1*0.1-750))  //0变1
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 1;
      }
    }
    else
    {
      cnt[0] = 0;
    }
    
    if(Current>=(g_BMSMonitor_Curr.Current_DisCharge_High2*0.1-750))  //0变2
    {
      if(++cnt[1]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[1] = 0;
        FltL = 2;
      }
    }
    else
    {
      cnt[1] = 0;
    }
    cnt[2] = 0; 
    cnt[3] = 0;
  }
  else if(FltL == 1) //1级故障
  {
    if(Current>=(g_BMSMonitor_Curr.Current_DisCharge_High2*0.1-750))      //1变2
    {
      if(++cnt[2]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
         cnt[2] = 0;
         FltL = 2;
      }
    }
    else
    {
      cnt[2] = 0;
    }
    
    if(Current<=(g_BMSMonitor_Curr.Current_DisCharge_High2*0.1-750 - 20))    //1变0
    {
      if(++cnt[3]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[3] = 0;
        FltL = 0;
      }
    }
    else
    {
      cnt[3] = 0;
    }
    cnt[0] = 0; 
    cnt[1] = 0;
  }
  else   //2级故障
  {
    cnt[0] = 0; 
    cnt[1] = 0;
    cnt[2] = 0;
    cnt[3] = 0; 
  }
  return(FltL);
}
/*================================放电过流================================*/

/*=======================================================================
                                放电绝缘故障0x08
 ========================================================================
 *函数名1:     Fault_DisChg_Insul(uint16 Insul)
 *功能:        根据电流判断过压故障等级
 *参数:        Insul (输入绝缘电阻)       
 *返回：       放电绝缘故障等级
 
 *说明：       
========================================================================*/ 
//判断故障等级
static uint8 Fault_DisChg_Insul(uint16 Insul)
{
  static uint8 cnt[4];      //时间计数
  static uint8 FltL;
  
  if(FltL==0)           //0级故障
  {
    
    if(Insul<=(g_BMSMonitor_Insul.Insulation_Resis2*0.1))  //0变2
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 2;
      }
    }
    else
    {
      cnt[0] = 0;
    }
  }
  return(FltL);
}

/*================================放电绝缘故障================================*/


/*=======================================================================
                              充电总压高0x11
 ========================================================================
 *函数名1:     Fault_Charge_VoltS(uint32 Volt)
 *功能:        根据系统电压判断故障等级
 *参数:        Volt(系统电压)
 *返回：       充电总压高的故障等级
 
 *说明：       
========================================================================*/ 
//判断故障等级
static uint8 Fault_Charge_VoltS(uint32 Volt)  //输入系统电压
{
  static uint8 cnt[4];      //时间计数
  static uint8 FltL;
  
  if(FltL==0)           //0级故障
  {
    if(Volt>=g_BMSMonitor_Volt.Volt_Sys_High1*1000)  //0变1
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 1;
      }
    }
    else
    {
      cnt[0] = 0;
    }
    
    if(Volt>=g_BMSMonitor_Volt.Volt_Sys_High2*1000)  //0变2
    {
      if(++cnt[1]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[1] = 0;
        FltL = 2;
      }
    }
    else
    {
      cnt[1] = 0;
    }
    cnt[2] = 0; 
    cnt[3] = 0;
  }
  else if(FltL == 1) //1级故障
  {
    if(Volt>=g_BMSMonitor_Volt.Volt_Sys_High2*1000)      //1变2
    {
      if(++cnt[2]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
         cnt[2] = 0;
         FltL = 2;
      }
    }
    else
    {
      cnt[2] = 0;
    }
    
    if(Volt<=(Volt>=g_BMSMonitor_Volt.Volt_Sys_High1*1000 - 500*SYS_SERIES_YiDongLi))    //1变0
    {
      if(++cnt[3]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[3] = 0;
        FltL = 0;
      }
    }
    else
    {
      cnt[3] = 0;
    }
    cnt[0] = 0; 
    cnt[1] = 0;
  }
  else   //2级故障
  {
    cnt[0] = 0; 
    cnt[1] = 0;
    cnt[2] = 0;
    cnt[3] = 0; 
  }
  return(FltL);
}
/*============================充电总压高===============================*/

/*=======================================================================
                              充电单体电压高0x12
 ========================================================================
 *函数名1:     Fault_Charge_VoltCH(uint16 Volt)
 *功能:        根据单体电压判断恢复后故障等级
 *参数:        Volt(单体电压)
 *返回：       充电单体电压高的故障等级
 
 *说明：       
========================================================================*/ 
//判断故障等级
static uint8 Fault_Charge_VoltCH(uint16 Volt)  //输入单体电压和环境温度
{
  static uint8 cnt[4];      //时间计数
  static uint8 FltL;
  
  if(FltL==0)           //0级故障
  {
    if(Volt>=g_BMSMonitor_Volt.Volt_Cell_High1)  //0变1
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 1;
      }
    }
    else
    {
      cnt[0] = 0;
    }
    
    if(Volt>=g_BMSMonitor_Volt.Volt_Cell_High2)  //0变2
    {
      if(++cnt[1]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[1] = 0;
        FltL = 2;
      }
    }
    else
    {
      cnt[1] = 0;
    }
    cnt[2] = 0; 
    cnt[3] = 0;
  }
  else if(FltL == 1) //1级故障
  {
    if(Volt>=g_BMSMonitor_Volt.Volt_Cell_High2)      //1变2
    {
      if(++cnt[2]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
         cnt[2] = 0;
         FltL = 2;
      }
    }
    else
    {
      cnt[2] = 0;
    }
    
    if(Volt<=(Volt>=g_BMSMonitor_Volt.Volt_Cell_High1 - 500))    //1变0
    {
      if(++cnt[3]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[3] = 0;
        FltL = 0;
      }
    }
    else
    {
      cnt[3] = 0;
    }
    cnt[0] = 0; 
    cnt[1] = 0;
  }
  else   //2级故障
  {
    cnt[0] = 0; 
    cnt[1] = 0;
    cnt[2] = 0;
    cnt[3] = 0; 
  }
  return(FltL);
}
/*============================充电单体电压高===========================*/

/*=======================================================================
                              充电单体压差0x13
 ========================================================================
 *函数名1:     Fault_Charge_VoltCD(uint16 V_max,uint16 V_min)
 *功能:        根据单体电压判断充电单体电压低故障等级
 *参数:        V_max(单体最高电压)
               V_min(单体最低电压)      
 *返回：       充电单体压差的故障等级
 
 *说明：       
========================================================================*/ 
//判断故障等级
static uint8 Fault_Charge_VoltCD(uint16 V_max,uint16 V_min)  //输入单体最高/低电压
{
  static uint16 V_Diff;
  static uint8 cnt[2];      //时间计数
  static uint8 FltL;
  
  V_Diff = V_max - V_min;   //求最大压差
  
  //判断故障等级
  if(FltL==0)           //0级故障
  {
    if(V_Diff>=g_BMSMonitor_Volt.Volt_Cell_Diff1)  //0变1
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 1;
      }
    }
    else
    {
      cnt[0] = 0; 
    }
    cnt[1] = 0; 
  }
  else     //1级故障
  {
    if(V_Diff<=(V_Diff>=g_BMSMonitor_Volt.Volt_Cell_Diff1 - 500))    //1变0
    {
      if(++cnt[1]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[1] = 0;
        FltL = 0;
      }
    }
    else
    {
      cnt[1] = 0;
    }
    cnt[0] = 0; 
  }
  return(FltL);
}
/*=============================充电单体压差=============================*/

/*=======================================================================
                                充电高温0x14
 ========================================================================
 *函数名1:     Fault_Charge_TempH(uint8 Temp)
 *功能:        根据温度判断高温故障等级
 *参数:        Temp (输入温度  输入偏移量:-40)       
 *返回：       充电高温的故障等级

 *说明：       
========================================================================*/ 
//判断故障等级
static uint8 Fault_Charge_TempH(uint8 Temp)  //输入温度
{
  static uint8 cnt[4];      //时间计数
  static uint8 FltL;
  
  if(FltL==0)           //0级故障
  {
    if(Temp>=g_BMSMonitor_Temp.Temp_Charge_High1)  //0变1
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 1;
      }
    }
    else
    {
      cnt[0] = 0;
    }
    
    if(Temp>=g_BMSMonitor_Temp.Temp_Charge_High2)  //0变2
    {
      if(++cnt[1]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[1] = 0;
        FltL = 2;
      }
    }
    else
    {
      cnt[1] = 0;
    }
    cnt[2] = 0; 
    cnt[3] = 0;
  }
  else if(FltL == 1) //1级故障
  {
    if(Temp>=g_BMSMonitor_Temp.Temp_Charge_High2)      //1变2
    {
      if(++cnt[2]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
         cnt[2] = 0;
         FltL = 2;
      }
    }
    else
    {
      cnt[2] = 0;
    }
    
    if(Temp<=(Temp>=g_BMSMonitor_Temp.Temp_Charge_High1 - 2))    //1变0
    {
      if(++cnt[3]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[3] = 0;
        FltL = 0;
      }
    }
    else
    {
      cnt[3] = 0;
    }
    cnt[0] = 0; 
    cnt[1] = 0;
  }
  else   //2级故障
  {
    cnt[0] = 0; 
    cnt[1] = 0;
    cnt[2] = 0;
    cnt[3] = 0; 
  }
  return(FltL);
}
/*==============================充电高温==============================*/

/*=======================================================================
                                充电低温0x15
 ========================================================================
 *函数名1:     Fault_Charge_TempL(uint8 Temp)
 *功能:        根据温度判断低温故障等级
 *参数:        Temp (输入温度  输入偏移量:-40)       
 *返回：       充电低温的故障等级
 
 *说明：       
========================================================================*/ 
//判断故障等级
static uint8 Fault_Charge_TempL(uint8 Temp)  //输入温度
{
  static uint8 cnt[4];      //时间计数
  static uint8 FltL;
  
  if(FltL==0)           //0级故障
  {
    if(Temp<=g_BMSMonitor_Temp.Temp_Charge_Low1)  //0变1
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 1;
      }
    }
    else
    {
      cnt[0] = 0;
    }
    
    if(Temp<=g_BMSMonitor_Temp.Temp_Charge_Low2)  //0变2
    {
      if(++cnt[1]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[1] = 0;
        FltL = 2;
      }
    }
    else
    {
      cnt[1] = 0;
    }
    cnt[2] = 0; 
    cnt[3] = 0;
  }
  else if(FltL == 1) //1级故障
  {
    if(Temp<=g_BMSMonitor_Temp.Temp_Charge_Low2)      //1变2
    {
      if(++cnt[2]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
         cnt[2] = 0;
         FltL = 2;
      }
    }
    else
    {
      cnt[2] = 0;
    }
    
    if(Temp>=(g_BMSMonitor_Temp.Temp_Charge_Low1 + 5))    //1变0
    {
      if(++cnt[3]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[3] = 0;
        FltL = 0;
      }
    }
    else
    {
      cnt[3] = 0;
    }
    cnt[0] = 0; 
    cnt[1] = 0;
  }
  else   //2级故障
  {
    cnt[0] = 0; 
    cnt[1] = 0;
    cnt[2] = 0;
    cnt[3] = 0; 
  }
  return(FltL);
}
/*==============================充电低温==============================*/

/*=======================================================================
                              充电温差0x16
 ========================================================================
 *函数名1:     Fault_Charge_TempD(uint8 T_max,uint8 T_min)
 *功能:        根据最高/低温度判断温差故障等级
 *参数:        T_max(单体最高温度)
               T_min(单体最低温度)       
 *返回：       充电温差的故障等级
 
 *说明：       
========================================================================*/ 
//判断故障等级
static uint8 Fault_Charge_TempD(uint8 T_max,uint8 T_min)  //输入单体最高/低电压
{
  static uint8 T_Diff;
  static uint8 cnt[2];      //时间计数
  static uint8 FltL;
  
  T_Diff = T_max - T_min;   //求最大压差
  
  //判断故障等级
  if(FltL==0)           //0级故障
  {
    if(T_Diff>=g_BMSMonitor_Temp.Temp_Charge_Diff1-40)  //0变1
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 1;
      }
    }
    else
    {
      cnt[0] = 0; 
    }
    cnt[1] = 0; 
  }
  else     //1级故障
  {
    if(T_Diff<=(g_BMSMonitor_Temp.Temp_Charge_Diff1-40 - 2))    //1变0
    {
      if(++cnt[1]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[1] = 0;
        FltL = 0;
      }
    }
    else
    {
      cnt[1] = 0;
    }
    cnt[0] = 0; 
  }
  return(FltL);
}
/*================================充电温差==============================*/

/*=======================================================================
                                充电过流0x17
 ========================================================================
 *函数名1:     Fault_Charge_CurrH(float Current)
 *功能:        根据电流判断充电过流故障等级
 *参数:        Current       
 *返回：       充电过流的故障等级
 
 *说明：       
========================================================================*/ 
//判断故障等级
static uint8 Fault_Charge_CurrH(float Current)//输入温度
{
  static uint8 cnt[4];      //时间计数
  static uint8 FltL;
  
  if(FltL==0)           //0级故障
  {
    if(Current>=(g_BMSMonitor_Curr.Current_Charge_High1*0.1-750))  //0变1
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 1;
      }
    }
    else
    {
      cnt[0] = 0;
    }
    
    if(Current>=(g_BMSMonitor_Curr.Current_Charge_High2*0.1-750))  //0变2
    {
      if(++cnt[1]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[1] = 0;
        FltL = 2;
      }
    }
    else
    {
      cnt[1] = 0;
    }
    cnt[2] = 0; 
    cnt[3] = 0;
  }
  else if(FltL == 1) //1级故障
  {
    if(Current>=(g_BMSMonitor_Curr.Current_Charge_High2*0.1-750))      //1变2
    {
      if(++cnt[2]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
         cnt[2] = 0;
         FltL = 2;
      }
    }
    else
    {
      cnt[2] = 0;
    }
    
    if(Current<=(g_BMSMonitor_Curr.Current_Charge_High1*0.1-750 - 5))    //1变0
    {
      if(++cnt[3]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[3] = 0;
        FltL = 0;
      }
    }
    else
    {
      cnt[3] = 0;
    }
    cnt[0] = 0; 
    cnt[1] = 0;
  }
  else   //2级故障
  {
    cnt[0] = 0; 
    cnt[1] = 0;
    cnt[2] = 0;
    cnt[3] = 0; 
  }
  return(FltL);
}
/*================================充电过流================================*/

/*=======================================================================
                                充电绝缘故障0x18
 ========================================================================
 *函数名1:     Fault_Charge_Insul(uint16 Insul)
 *功能:        根据电流判断过压故障等级
 *参数:        Insul (输入绝缘电阻)       
 *返回：       放电绝缘故障等级
 
 *说明：       
========================================================================*/ 
//判断故障等级
static uint8 Fault_Charge_Insul(uint16 Insul)
{
  static uint8 cnt[4];      //时间计数
  static uint8 FltL;
  
  if(FltL==0)           //0级故障
  {
    
    if(Insul<=g_BMSMonitor_Insul.Insulation_Resis2*0.1)  //0变2
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 2;
      }
    }
    else
    {
      cnt[0] = 0;
    }
  }
  return(FltL);
}

/*================================充电绝缘故障================================*/


//==============================导线开路故障===================================
//直接调用g_OpenWireInfo.OpenWire_Status变量进行判断
//==============================导线开路故障===================================