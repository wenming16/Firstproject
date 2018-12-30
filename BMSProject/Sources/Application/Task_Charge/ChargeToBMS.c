                                              /*=======================================================================
 *Subsystem:   裸机
 *File:        ChargeToBMS.C
 *Author:      Wen Yuhao
 *Description: 通信：
               接口：
               波特率：
 ========================================================================
 * History:    修改历史记录列表
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#include  "Task_Charge.h"
#include  "Task_DataProcess.h"
#include  "BattInfoConfig.h"
#include  "LTC6811_ConnectType.h"

ChargePileBMS_T         ChargePileBMS;
Charge_State_T   g_Charge_State;
/*=======================================================================
 *函数名:      ChargePile_to_Bms(pCANFRAME data)
 *功能:        充电机发送至BMS
 *参数:        无       
 *返回：       无
 *说明：       
========================================================================*/
void CAN_ChargetoBMS(pCANFRAME data)
{
  uint8   Val8;
  uint16  Val;

  Val = (((uint16)(data -> m_data[0]))<<8) + (data -> m_data[1]);   
  if(Val!=0xFFFF) 
  {
     ChargePileBMS.Volt_ChargePileOut = (uint16)(Val);  
  }
  
  Val = (((uint16)(data -> m_data[2]))<<8) + (data -> m_data[3]);  
  if(Val!=0xFFFF) 
  {
     ChargePileBMS.Curr_ChargePileOut = (uint16)(Val);  
  }
  
  Val8 = (data -> m_data[5]); 
  if(Val8!=0xFF) 
  {
     g_Charge_State.Hard             = Val8&0x01; 
     g_Charge_State.TempH_ChargePile = (Val8>>1)&0x01; 
     g_Charge_State.VoltL_ChargePile = (Val8>>2)&0x01; 
     g_Charge_State.On_Line          = (Val8>>3)&0x01; 
     g_Charge_State.GetMsg           = (Val8>>4)&0x01;  
  }
  if(g_Charge_State.Hard || g_Charge_State.TempH_ChargePile ||\
    g_Charge_State.VoltL_ChargePile || g_Charge_State.On_Line ||\
    g_Charge_State.GetMsg)
  {
    g_Charge_State.FltState = 1;
  }
  else
  {
    g_Charge_State.FltState = 0;
  }
   
}         

