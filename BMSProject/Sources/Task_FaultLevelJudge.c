/*=======================================================================
 *Subsystem:   裸机
 *File:        Task_FaultLevelJudge.c
 *Author:      Wenming
 *Description: 通信：
               接口：  CAN2
               波特率：250k
               调试：
 ========================================================================
 * History:    修改历史记录列表，修改记录包括修改日期、修改者及修改内容
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#include  "TypeDefinition.h"
#include  "BattInfoConfig.h"
#include  "Task_FaultLevelJudge.h"
#include  "FaultLevelConfig.h"        
#include  "Roll_Task.h"  
#include  "BattInfoParaStructure.h" 
#include  "Task_DataProcess.h"                                  //后续修改
#include  "WorkModeJudge.h"
/*=======================================================================
                                  声明
========================================================================*/ 
Fault_DisCharge_T Fault_DisCharge; 
Fault_Charge_T    Fault_Charge;

VoltInfo_T VoltInfo;
TempInfo_T TempInfo;
State_Offline_T State_Offline;

FltThreshold_DisChg_T FltThreshold_DisChg;
FltThreshold_Charge_T FltThreshold_Charge;

 //放电故障判断
 static uint8 Fault_DisChg_VoltS(uint32 Volt,uint8 Temp); 
 static uint8 Fault_DisChg_VoltC(uint16 Volt,uint8 Temp);
 static uint8 Fault_DisChg_VoltCD(uint16 V_max,uint16 V_min);
 static uint8 Fault_DisChg_TempH(uint8 Temp);
 static uint8 Fault_DisChg_TempL(uint8 Temp);
 static uint8 Fault_DisChg_TempD(uint8 T_max,uint8 T_min);
 static uint8 Fault_DisChg_CurrH(float Current);
 //充电故障判断
 static uint8 Fault_VoltSys_Charge(uint32 Volt);
 static uint8 Fault_VoltCell_High_Charge(uint16 Volt);
 static uint8 Fault_VoltCell_Low_Charge(uint16 Volt,uint8 Temp);
 static uint8 Fault_VoltCell_Diff_Charge(uint16 V_max,uint16 V_min);
 static uint8 Fault_Temp_High_Charge(uint8 Temp);
 static uint8 Fault_Temp_Low_Charge(uint8 Temp);
 static uint8 Fault_Temp_Diff_Charge(uint8 T_max,uint8 T_min);
 static uint8 Fault_Current_Charge(float Current);

/*=======================================================================
 *函数名1:     FltThresholdGet(void)
 *功能:        获取故障阈值
 *参数:        无    
 *返回：       无
 
 *说明：       
========================================================================*/ 
/*static
void FltThresholdGet(void)
{
  //放电
  FltThreshold_DisChg.F2_DisChg_VoltSL_NT = ;
  FltThreshold_DisChg.F1_DisChg_VoltSL_NT = ;
  FltThreshold_DisChg.F2_DisChg_VoltSL_LT = ;
  FltThreshold_DisChg.F1_DisChg_VoltSL_LT = ;
  
  FltThreshold_DisChg.F2_DisChg_VoltCL_NT = ;
  FltThreshold_DisChg.F1_DisChg_VoltCL_NT = ;
  FltThreshold_DisChg.F2_DisChg_VoltCL_LT = ;
  FltThreshold_DisChg.F1_DisChg_VoltCL_LT = ;
  
  FltThreshold_DisChg.F1_DisChg_VoltCD = ;
  
  FltThreshold_DisChg.F2_DisChg_TempH = ;
  FltThreshold_DisChg.F1_DisChg_TempH = ;
  
  FltThreshold_DisChg.F2_DisChg_TempL = ;
  FltThreshold_DisChg.F1_DisChg_TempL = ;
  
  FltThreshold_DisChg.F1_DisChg_TempD = ;
  
  FltThreshold_DisChg.F2_DisChg_CurrH = ;
  FltThreshold_DisChg.F1_DisChg_CurrH = ;
  
  FltThreshold_DisChg.F2_DisChg_Insul = ;
//充电
  FltThreshold_Charge.F2_Charge_VoltSL = ;
  FltThreshold_Charge.F1_Charge_VoltSL = ;
  
  FltThreshold_Charge.F2_Charge_VoltCL = ;
  FltThreshold_Charge.F1_Charge_VoltCL = ;
  
  FltThreshold_Charge.F1_Charge_VoltCD = ;
  
  FltThreshold_Charge.F2_Charge_TempH = ;
  FltThreshold_Charge.F1_Charge_TempH = ;
  
  FltThreshold_Charge.F2_Charge_TempL = ;
  FltThreshold_Charge.F1_Charge_TempL = ;
  
  FltThreshold_Charge.F1_Charge_TempD = ;
  
  FltThreshold_Charge.F2_Charge_CurrH = ;
  FltThreshold_Charge.F1_Charge_CurrH = ;
  
  FltThreshold_Charge.F2_Charge_Insul = ;
} */

/*=======================================================================
                              故障判断函数0x00
 ========================================================================
 *函数名1:     Task_FaultLevelJudge(uint8 workstate)
 *功能:        根据系统电压、温度、电流判断故障等级
 *参数:        无    
 *返回：       无
 
 *说明：       
========================================================================*/ 
void Task_FaultLevelJudge(uint8 workstate)
{
   switch(workstate)
   {
    case MODE_DISCHARGE: //放电状态
      //清除充电状态故障
      Fault_Charge.Level_Charge_SwitchOff_flag = 0; 
      //判断放电状态故障
      Fault_DisCharge.Level_Volt_Sys_Low           = Fault_DisChg_VoltS(DataColletInfo.SysVolt_Total, TempInfo.CellTemp_Ave);
      Fault_DisCharge.Level_Volt_Cell_Low          = Fault_DisChg_VoltC(VoltInfo.CellVolt_Min, TempInfo.CellTemp_Ave);
      Fault_DisCharge.Level_Volt_Cell_Diff_High    = Fault_DisChg_VoltCD(VoltInfo.CellVolt_Max, VoltInfo.CellVolt_Min);
      Fault_DisCharge.Level_Temp_High              = Fault_DisChg_TempH(TempInfo.CellTemp_Max);
      Fault_DisCharge.Level_Temp_Low               = Fault_DisChg_TempL(TempInfo.CellTemp_Min);
      Fault_DisCharge.Level_Temp_Diff_High         = Fault_DisChg_TempD(TempInfo.CellTemp_Max, TempInfo.CellTemp_Min);
      Fault_DisCharge.Level_Current_DisCharge_High = Fault_DisChg_CurrH(DataColletInfo.DataCollet_Current_Filter);
      
      //断开继电器的二级故障标记
      if((Fault_DisCharge.Level_Volt_Sys_Low==2) || \
         (Fault_DisCharge.Level_Volt_Cell_Low == 2)||\
         (Fault_DisCharge.Level_Temp_High == 2)||\
         (Fault_DisCharge.Level_Temp_Low == 2) ||\
         (Fault_DisCharge.Level_Current_DisCharge_High == 2))
      {
        Fault_DisCharge.Level_DisCharge_SwitchOff_flag = 1;  
      }
      
    break;
    
    case MODE_CHARGE:   //充电状态
      //清除放电状态故障
      Fault_DisCharge.Level_Volt_Sys_Low           = 0;
      Fault_DisCharge.Level_Volt_Cell_Low          = 0;
      Fault_DisCharge.Level_DisCharge_SwitchOff_flag = 0;
      //判断充电状态故障
      Fault_Charge.Level_Volt_Sys_High        = Fault_VoltSys_Charge(DataColletInfo.SysVolt_Total);
      Fault_Charge.Level_Volt_Cell_High       = Fault_VoltCell_High_Charge(VoltInfo.CellVolt_Max);
      Fault_Charge.Level_Volt_Cell_Diff_High  = Fault_VoltCell_Diff_Charge(VoltInfo.CellVolt_Max, VoltInfo.CellVolt_Min);
      Fault_Charge.Level_Temp_High            = Fault_Temp_High_Charge(TempInfo.CellTemp_Max);
      Fault_Charge.Level_Temp_Low             = Fault_Temp_Low_Charge(TempInfo.CellTemp_Min);
      Fault_Charge.Level_Temp_Diff_High       = Fault_Temp_Diff_Charge(TempInfo.CellTemp_Max, TempInfo.CellTemp_Min);
      Fault_Charge.Level_Current_Charge_High  = Fault_Current_Charge(DataColletInfo.DataCollet_Current_Filter);
      
      //断开继电器的二级故障标记
      if((Fault_Charge.Level_Volt_Sys_High==2) || \
         (Fault_Charge.Level_Volt_Cell_High==2) || \
         (Fault_Charge.Level_Volt_Cell_Low == 2)||\
         (Fault_Charge.Level_Temp_High == 2)||\
         (Fault_Charge.Level_Temp_Low == 2))
      {
        Fault_Charge.Level_Charge_SwitchOff_flag = 1;  
      }
    break;
   }
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
    return (F1_DISCHG_VOLTSL_NT);//可换成标定变量(分辨率：0.001V/位)
  }
  else //低温
  {
    return (F1_DISCHG_VOLTSL_LT);//可换成标定变量(分辨率：0.001V/位)
  }
}
//2级故障函数
static
uint32 Fault2_VoltSys_DisCharge(uint8 Temp)
{
  if(Temp>=NTEMP_BOUNDARY)//常温(>=0℃)
  { 
    return (F2_DISCHG_VOLTSL_NT);//可换成标定变量(分辨率：0.001V/位)
  }
  else //低温
  {
    return (F2_DISCHG_VOLTSL_LT);//可换成标定变量(分辨率：0.001V/位)
  }
}
//1恢复至0
static
uint32 Recover1_VoltSys_DisCharge(uint8 Temp)
{
  if(Temp>=NTEMP_BOUNDARY)//常温(>=0℃)
  { 
    return (R1_DISCHG_VOLTSL_NT);//可换成标定变量(分辨率：0.001V/位)
  }
  else //低温
  {
    return (R1_DISCHG_VOLTSL_LT);//可换成标定变量(分辨率：0.001V/位)
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
    return (F1_DISCHG_VOLTCL_NT);//可换成标定变量(分辨率：0.001V/位)
  }
  else //低温
  {
    return (F1_DISCHG_VOLTCL_LT);//可换成标定变量(分辨率：0.001V/位)
  }
}
//2级故障函数
static
uint16 Fault2_VoltCell_DisCharge(uint8 Temp)
{
  if(Temp>=NTEMP_BOUNDARY)//常温(>=0℃)
  { 
    return (F2_DISCHG_VOLTCL_NT);//可换成标定变量(分辨率：0.001V/位)
  }
  else //低温
  {
    return (F2_DISCHG_VOLTCL_LT);//可换成标定变量(分辨率：0.001V/位)
  }
}
//恢复至0
static
uint16 Recover1_VoltCell_DisCharge(uint8 Temp)
{
  if(Temp>=NTEMP_BOUNDARY)//常温(>=0℃)
  { 
    return (R1_DISCHG_VOLTCL_NT);//可换成标定变量(分辨率：0.001V/位)
  }
  else //低温
  {
    return (R1_DISCHG_VOLTCL_LT);//可换成标定变量(分辨率：0.001V/位)
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
    if(V_Diff>=F1_DISCHG_VOLTCD)  //0变1
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
    if(V_Diff<=R1_DISCHG_VOLTCD)    //1变0
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
    if(Temp>=F1_DISCHG_TEMPH)  //0变1
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
    
    if(Temp>=F2_DISCHG_TEMPH)  //0变2
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
    if(Temp>=F2_DISCHG_TEMPH)      //1变2
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
    
    if(Temp<=R1_DISCHG_TEMPH)    //1变0
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
    if(Temp<=F1_DISCHG_TEMPL)  //0变1
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
    
    if(Temp<=F2_DISCHG_TEMPL)  //0变2
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
    if(Temp<=F2_DISCHG_TEMPL)      //1变2
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
    
    if(Temp>=R1_DISCHG_TEMPL)    //1变0
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
    if(T_Diff>=F1_DISCHG_TEMPD)  //0变1
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
    if(T_Diff<=R1_DISCHG_TEMPD)    //1变0
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
    if(Current>=F1_DISCHG_CURRH)  //0变1
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
    
    if(Current>=F2_DISCHG_CURRH)  //0变2
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
    if(Current>=F2_DISCHG_CURRH)      //1变2
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
    
    if(Current<=R1_DISCHG_CURRH)    //1变0
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
static Fault_DisChg_Insul(uint16 Insul)
{
  static uint8 cnt[4];      //时间计数
  static uint8 FltL;
  
  if(FltL==0)           //0级故障
  {
    
    if(Insul<=F2_DISCHG_INSUL)  //0变2
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
 *函数名1:     Fault_VoltSys_Charge(uint32 Volt)
 *功能:        根据系统电压判断故障等级
 *参数:        Volt(系统电压)
 *返回：       充电总压高的故障等级
 
 *说明：       
========================================================================*/ 
//判断故障等级
static uint8 Fault_VoltSys_Charge(uint32 Volt)  //输入系统电压
{
  static uint8 cnt[4];      //时间计数
  static uint8 FltL;
  
  if(FltL==0)           //0级故障
  {
    if(Volt>=F1_CHARGE_VOLTSH)  //0变1
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
    
    if(Volt>=F2_CHARGE_VOLTSH)  //0变2
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
    if(Volt>=F2_CHARGE_VOLTSH)      //1变2
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
    
    if(Volt<=R1_CHARGE_VOLTSH)    //1变0
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
 *函数名1:     Fault_VoltCell_High_Charge(uint16 Volt)
 *功能:        根据单体电压判断恢复后故障等级
 *参数:        Volt(单体电压)
 *返回：       充电单体电压高的故障等级
 
 *说明：       
========================================================================*/ 
//判断故障等级
static uint8 Fault_VoltCell_High_Charge(uint16 Volt)  //输入单体电压和环境温度
{
  static uint8 cnt[4];      //时间计数
  static uint8 FltL;
  
  if(FltL==0)           //0级故障
  {
    if(Volt>=F1_CHARGE_VOLTCH)  //0变1
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
    
    if(Volt>=F2_CHARGE_VOLTCH)  //0变2
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
    if(Volt>=F2_CHARGE_VOLTCH)      //1变2
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
    
    if(Volt<=R1_CHARGE_VOLTCH)    //1变0
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
 *函数名1:     Fault_VoltCell_Diff_Charge(uint16 V_max,uint16 V_min)
 *功能:        根据单体电压判断充电单体电压低故障等级
 *参数:        V_max(单体最高电压)
               V_min(单体最低电压)      
 *返回：       充电单体压差的故障等级
 
 *说明：       
========================================================================*/ 
//判断故障等级
static uint8 Fault_VoltCell_Diff_Charge(uint16 V_max,uint16 V_min)  //输入单体最高/低电压
{
  static uint16 V_Diff;
  static uint8 cnt[2];      //时间计数
  static uint8 FltL;
  
  V_Diff = V_max - V_min;   //求最大压差
  
  //判断故障等级
  if(FltL==0)           //0级故障
  {
    if(V_Diff>=F1_CHARGE_VOLTCD)  //0变1
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
    if(V_Diff<=R1_CHARGE_VOLTCD)    //1变0
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
 *函数名1:     Fault_Temp_High_Charge(uint8 Temp)
 *功能:        根据温度判断高温故障等级
 *参数:        Temp (输入温度  输入偏移量:-40)       
 *返回：       充电高温的故障等级

 *说明：       
========================================================================*/ 
//判断故障等级
static uint8 Fault_Temp_High_Charge(uint8 Temp)  //输入温度
{
  static uint8 cnt[4];      //时间计数
  static uint8 FltL;
  
  if(FltL==0)           //0级故障
  {
    if(Temp>=F1_CHARGE_TEMPH)  //0变1
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
    
    if(Temp>=F2_CHARGE_TEMPH)  //0变2
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
    if(Temp>=F2_CHARGE_TEMPH)      //1变2
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
    
    if(Temp<=R1_CHARGE_TEMPH)    //1变0
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
 *函数名1:     Fault_Temp_Low_Charge(uint8 Temp)
 *功能:        根据温度判断低温故障等级
 *参数:        Temp (输入温度  输入偏移量:-40)       
 *返回：       充电低温的故障等级
 
 *说明：       
========================================================================*/ 
//判断故障等级
static uint8 Fault_Temp_Low_Charge(uint8 Temp)  //输入温度
{
  static uint8 cnt[4];      //时间计数
  static uint8 FltL;
  
  if(FltL==0)           //0级故障
  {
    if(Temp<=F1_CHARGE_TEMPL)  //0变1
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
    
    if(Temp<=F2_CHARGE_TEMPL)  //0变2
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
    if(Temp<=F2_CHARGE_TEMPL)      //1变2
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
    
    if(Temp>=R1_CHARGE_TEMPL)    //1变0
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
 *函数名1:     Fault_Temp_Diff_Charge(uint8 T_max,uint8 T_min)
 *功能:        根据最高/低温度判断温差故障等级
 *参数:        T_max(单体最高温度)
               T_min(单体最低温度)       
 *返回：       充电温差的故障等级
 
 *说明：       
========================================================================*/ 
//判断故障等级
static uint8 Fault_Temp_Diff_Charge(uint8 T_max,uint8 T_min)  //输入单体最高/低电压
{
  static uint8 T_Diff;
  static uint8 cnt[2];      //时间计数
  static uint8 FltL;
  
  T_Diff = T_max - T_min;   //求最大压差
  
  //判断故障等级
  if(FltL==0)           //0级故障
  {
    if(T_Diff>=F1_CHARGE_TEMPD)  //0变1
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
    if(T_Diff<=R1_CHARGE_TEMPD)    //1变0
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
 *函数名1:     Fault_Current_Charge(float Current)
 *功能:        根据电流判断充电过流故障等级
 *参数:        Current       
 *返回：       充电过流的故障等级
 
 *说明：       
========================================================================*/ 
//判断故障等级
static uint8 Fault_Current_Charge(float Current)//输入温度
{
  static uint8 cnt[4];      //时间计数
  static uint8 FltL;
  
  if(FltL==0)           //0级故障
  {
    if(Current>=F1_CHARGE_CURRH)  //0变1
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
    
    if(Current>=F2_CHARGE_CURRH)  //0变2
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
    if(Current>=F2_CHARGE_CURRH)      //1变2
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
    
    if(Current<=R1_CHARGE_CURRH)    //1变0
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

