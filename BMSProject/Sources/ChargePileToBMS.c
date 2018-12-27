                                              /*=======================================================================
 *Subsystem:   裸机
 *File:        ChargePileToBMS.C
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

#include  "Task_BmsAndChargePile.h"

ChargePileBMS_T         ChargePileBMS;
ChargePileBMS_State_T   ChargePileBMS_State;
/*=======================================================================
 *函数名:      ChargePile_to_Bms(pCANFRAME data)
 *功能:        充电机发送至BMS
 *参数:        无       
 *返回：       无
 *说明：       
========================================================================*/
void ChargePile_to_Bms(pCANFRAME data)
{
  uint8   Val8;
  uint16  Val;

  //0x112
  data->m_ID = 0x112;

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
     ChargePileBMS_State.Hard             = Val8&0x01; 
     ChargePileBMS_State.TempH_ChargePile = (Val8>>1)&0x01; 
     ChargePileBMS_State.VoltL_ChargePile = (Val8>>2)&0x01; 
     ChargePileBMS_State.On_Line          = (Val8>>3)&0x01; 
     ChargePileBMS_State.GetMsg           = (Val8>>4)&0x01;  
  }
  
        
      
  //Task_Flag_Counter.UpMonitor_to_BMS++;
}         

