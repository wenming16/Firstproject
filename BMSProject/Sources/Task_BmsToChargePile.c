  /*=======================================================================
 *Subsystem:   裸机
 *File:        Task_BmsToChargePile.C
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
 #include  "Task_CurrentLimit.h"
 #include  "BattInfoParaStructure.h"
 #include  "Task_SOC.h"
 #include  "Task_FaultLevelJudge.h"
 
 BMSChargePile_T        BMSChargePile;
 BMSChargePile_State_T  BMSChargePile_State;
 
 
/*=======================================================================
 *函数名:      CAN_ToChargePileInit(void) 
 *功能:        与充电桩通信的CAN通道初始化
 *参数:        无       
 *返回：       无
 
 *说明：       
========================================================================*/ 
uint8 CAN_ToChargePileInit(void)
{
  uint8 state;
  state = INIT_CAN1(250);
  return(state);
}

/*=======================================================================
 *函数名:      CAN_ToUpMonitor(pCANFRAME canptr) 
 *功能:        BMS information to UpMonitor
 *参数:        无       
 *返回：       无
 *说明：       BMS发送信息给上位机的CAN通道选择；
========================================================================*/ 
static
uint8 CAN_ToChargePile(pCANFRAME canptr)
{
  if(!SendMsg_CAN1(canptr))
  {
     return 0;  //正常
  }  
  return 1;   //报错
}

/*=======================================================================
 *函数名:      Task_BmsToChargePile(void) 
 *功能:        BMS information to ChargePile
 *参数:        无       
 *返回：       无
 *说明：       BMS发送信息给充电机
========================================================================*/ 
void Task_BmsToChargePile(void) 
{
  pCANFRAME BMS_to_ChargePile;
  
  BmsToChargePileInfo();
  
  BMS_to_ChargePile->m_IDE = 0;    //标准帧
	BMS_to_ChargePile->m_RTR = 0;
	BMS_to_ChargePile->m_dataLen = 8;
	BMS_to_ChargePile->m_priority = 6; //?? 
  
  //0x111
  BMS_to_ChargePile->m_ID = 0x111;
 
  BMS_to_ChargePile->m_data[0] = (BMSChargePile.Volt_Max_ChargePile*10) >> 8;       //最高允许充电端电压高字节   0.1V/bit
	BMS_to_ChargePile->m_data[1] = (uint8)(BMSChargePile.Volt_Max_ChargePile*10);                              
	BMS_to_ChargePile->m_data[2] = (BMSChargePile.Curr_Max_ChargePile*10)>> 8;        //最高允许充充电电流高字节   0.1A/bit   
	BMS_to_ChargePile->m_data[3] = (uint8)(BMSChargePile.Curr_Max_ChargePile*10); 
	BMS_to_ChargePile->m_data[4] = BMSChargePile.Control_ChargePile;                  //控制充电桩    0:开启    1:关闭
	BMS_to_ChargePile->m_data[5] = (((0x01)&0x01) + \
	                                ((BMSChargePile_State.TempH_Cell<<1)&0x02) + \
	                                ((BMSChargePile_State.TempL_Cell<<2)&0x04) + \
	                                ((BMSChargePile_State.CurrH_Cell<<3)&0x08) + \
	                                ((BMSChargePile_State.Insul<<4)&0x10) + \
	                                ((BMSChargePile_State.BMSGetMsg<<5)&0x20) + \
	                                ((BMSChargePile_State.FaultFlag<<6)&0x40) + \
	                                ((0x01<<7)&0x80));
	BMS_to_ChargePile->m_data[6] = 0xFF;                 
	BMS_to_ChargePile->m_data[7] = 0xFF;       
  while(CAN_ToChargePile(BMS_to_ChargePile));
  delayus(100);
 
 //0x115
  BMS_to_ChargePile->m_ID = 0x115;
 
  BMS_to_ChargePile->m_data[0] = (BMSChargePile.VoltC_Max*1000) >> 8;      //单体最高电压高字节   0.001V/bit
	BMS_to_ChargePile->m_data[1] = (uint8)(BMSChargePile.VoltC_Max*1000);                              
	BMS_to_ChargePile->m_data[2] = (BMSChargePile.VoltC_Min*1000)>> 8;       //单体最低电压高字节   0.001V/bit 
	BMS_to_ChargePile->m_data[3] = (uint8)(BMSChargePile.VoltC_Min*1000); 
	BMS_to_ChargePile->m_data[4] = BMSChargePile.SOC*250;                    //SOC   0.4%/bit
	BMS_to_ChargePile->m_data[5] = BMSChargePile.Temp_Max;                   //最高温度     1℃/bit  偏移量:-40℃
	BMS_to_ChargePile->m_data[6] = (BMSChargePile.VoltS*10)>>8;              //电池组电压高字节     0.1V/bit
	BMS_to_ChargePile->m_data[7] = (uint8)(BMSChargePile.VoltS*10);       
  while(CAN_ToChargePile(BMS_to_ChargePile));
  delayus(100);
}






