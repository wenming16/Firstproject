#include  "Task_BmsAndChargePile.h"
#include  "BattInfoConfig.h"
#include  "Task_CurrentLimit.h"
#include  "Task_SOC.h"
#include  "BattInfoParaStructure.h"
#include  "Task_FaultLevelJudge.h"

static uint8 ChargeEndJudge(uint8,float,uint16,uint8,float);
/*=======================================================================
 *函数名:      BmsToChargePileInfo(void) 
 *功能:        搜集发送至充电机的信息
 *参数:        无       
 *返回：       无
 
 *说明：       配置需要发送给充电机的信息,提高程序可移植性
========================================================================*/ 
void BmsToChargePileInfo(void)
{
  BMSChargePile.Volt_Max_ChargePile = CELL_VOLT_NOMINAL * SYS_SERIES_YiDongLi + 5;
  BMSChargePile.Curr_Max_ChargePile = CurrentLimit.Current_Charge_Constant;
  
  BMSChargePile.Control_ChargePile = ChargeEndJudge(SOCInfo.SOC_ValueRead,ChargePileBMS.Curr_ChargePileOut,VoltInfo.CellVolt_Max,TempInfo.CellTemp_Max,DataColletInfo.DataCollet_Current_Filter);
  
  BMSChargePile.VoltC_Max = VoltInfo.CellVolt_Max ;
  BMSChargePile.VoltC_Min = VoltInfo.CellVolt_Min;
  BMSChargePile.SOC = SOCInfo.SOC_ValueRead;
  BMSChargePile.Temp_Max = TempInfo.CellTemp_Max;
  BMSChargePile.VoltS = DataColletInfo.SysVolt_Total;
  
  BMSChargePile_State.TempH_Cell = Fault_Charge.Level_Temp_High;
  BMSChargePile_State.TempL_Cell = Fault_Charge.Level_Temp_Low;
  BMSChargePile_State.CurrH_Cell = Fault_Charge.Level_Current_Charge_High;
  BMSChargePile_State.Insul = 0;
  BMSChargePile_State.BMSGetMsg = 0;
  BMSChargePile_State.FaultFlag = 0;
}

/*=======================================================================
 *函数名:      ChargeEndJudge(void) 
 *功能:        充电中止判断
 *参数:        无       
 *返回：       无
 
 *说明：       
========================================================================*/ 
static
uint8 ChargeEndJudge(uint8 soc,float curr_out,uint16 voltc,uint8 temph,float curr_adc)
{
  if(soc+0.002>=1)        //SOC达到100%
  {
    return (1);
  }
  else if(curr_out < 0.03*SYS_CAPACITY)
  {
    return (1);
  }
  else if(voltc > CELL_VOLT_MAX)
  {
    return (1);
  }
  else if(temph > CELL_TEMP_MAX_CHARGE + 40)
  {
    return (1);
  }
  else if(abs(curr_adc - curr_out)>=5)
  {
    return (1);
  }
  else 
  {
    return (0);
  }
}
