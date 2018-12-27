/*=======================================================================
 *Subsystem:   裸机
 *File:        Task_BmsToUpMonitor.C
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

#include  "Task_BmsAndUpMonitor.h"
#include  "BattInfoConfig.h"
#include  "FaultLevelConfig.h"
#include  "DS3231.h"
#include  "Roll_Task.h"
#include  "EEDFLASH.h"
#include  "Task_BMSCheckSelf.h"
#include  "BattInfoParaStructure.h"
#include  "Task_CurrentLimit.h"
#include  "Task_SOC.h"
#include  "Task_FaultLevelJudge.h"
#include  "WorkModeJudge.h"

BMSMonitor_Volt_T BMSMonitor_Volt;
BMSMonitor_Temp_T BMSMonitor_Temp;
BMSMonitor_Curr_T BMSMonitor_Curr;
BMSMonitor_SOC_T  BMSMonitor_SOC;
BMSMonitor_Insu_T BMSMonitor_Insu; 
BMSMonitor_SOH_T  BMSMonitor_SOH;


InsulInfo_T InsulInfo;


/*=======================================================================
 *函数名:      CAN_UpMonitorInit(void) 
 *功能:        上位机使用的CAN通道初始化
 *参数:        无       
 *返回：       无
 
 *说明：       
========================================================================*/ 
uint8 CAN_UpMonitorInit(void)
{
  uint8 state;
  state = INIT_CAN2(500);
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
uint8 CAN_ToUpMonitor(pCANFRAME canptr)
{
  if(!SendMsg_CAN2(canptr))
  {
     return 0; 
  }  
  return 1;
}

/*=======================================================================
 *函数名:      Task_BmsToUpMonitor(void) 
 *功能:        BMS information to UpMonitor
 *参数:        无       
 *返回：       无
 *说明：       BMS发送信息给上位机；
========================================================================*/  
void Task_BmsToUpMonitor(void)
{  
  uint8 i;
  
  pCANFRAME BMS_to_Upmonitor;
  
  BMS_to_Upmonitor->m_IDE = 1;
	BMS_to_Upmonitor->m_RTR = 0;
	BMS_to_Upmonitor->m_dataLen = 8;
	BMS_to_Upmonitor->m_priority = 6; 
  
  if(MonitorBMS_Start.CellMessageSend_judge == 0x01)
  {
    //电池类信息
    //0x1800C0F4
    for(i=0;i<3;i++)
    { 
      BMS_to_Upmonitor->m_ID = 0x1800C0F4;     	
      switch(i)
      {
        case 0:
        	BMS_to_Upmonitor->m_data[0] = i;     //编号 0x00
        	BMS_to_Upmonitor->m_data[1] = CELL_TYPE;                                //电池类型 
        	BMS_to_Upmonitor->m_data[2] = (uint8)(CELL_RESIS_INTERNAL*10);          //电池内阻，单位是0.1mΩ/Bit  
        	BMS_to_Upmonitor->m_data[3] = ((uint16)(CELL_RESIS_INTERNAL*10)) >> 8; 
        	BMS_to_Upmonitor->m_data[4] = (uint8)(CELL_CAPACITY*100);               //电池容量，单位是0.01AH/Bit
        	BMS_to_Upmonitor->m_data[5] = ((uint16)(CELL_CAPACITY*100)) >> 8;
        	BMS_to_Upmonitor->m_data[6] = (uint8)CELL_LIFE_CYCLE;                   //循环寿命，1次/Bit
        	BMS_to_Upmonitor->m_data[7] = ((uint16)(CELL_LIFE_CYCLE)) >> 8;       
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
          delayus(100);
        break;
        
        case 1:
          BMS_to_Upmonitor->m_data[0] = i;     //编号 0x01
          BMS_to_Upmonitor->m_data[1] = (uint8)(CELL_VOLT_NOMINAL*10000);     //单体标称电压，单位是0.0001V/Bit  
          BMS_to_Upmonitor->m_data[2] = ((uint16)(CELL_VOLT_NOMINAL*10000)) >> 8;          
          BMS_to_Upmonitor->m_data[3] = (uint8)(CELL_VOLT_MAX*10000);    //最高单体电压，单位是0.0001V/Bi
          BMS_to_Upmonitor->m_data[4] = ((uint16)(CELL_VOLT_MAX*10000)) >> 8;             
          BMS_to_Upmonitor->m_data[5] = (uint8)(CELL_VOLT_MIN*10000);        //最低单体电压，单位是0.0001V/Bit
          BMS_to_Upmonitor->m_data[6] = ((uint16)(CELL_VOLT_MIN*10000)) >> 8;                  
          BMS_to_Upmonitor->m_data[7] = CELL_TEMP_MAX_DISCHARGE;   //电池最高保护温度
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
          delayus(100);
        break;
        
        case 2:
          BMS_to_Upmonitor->m_data[0] = i;     //编号 0x02
          BMS_to_Upmonitor->m_data[1] = (uint8)(F1_DISCHG_VOLTCD);          //最大单体压差 
          BMS_to_Upmonitor->m_data[2] = (uint16)(F1_DISCHG_VOLTCD) >> 8;            
          BMS_to_Upmonitor->m_data[3] = (uint8)F2_DISCHG_TEMPL;             //电池最低使用温度
          BMS_to_Upmonitor->m_data[4] = (uint8)SYS_SERIES_YiDongLi;                  //电池系统总串数   
          BMS_to_Upmonitor->m_data[5] = (uint8)SYS_NUMBER_MODULE;           //电池系统模组数
          BMS_to_Upmonitor->m_data[6] = (uint8)SYS_NUMBER_BOX;              //电池系统箱数
          BMS_to_Upmonitor->m_data[7] = (uint8)SYS_NUMBER_TEMP;             //电池系统温度检测总数量
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
          delayus(100);
        break;
      }
    }

    //电池故障等级
    //0x1810C0F4      电池电压保护阈值信息
    for(i=0;i<6;i++)
    {
      BMS_to_Upmonitor->m_ID = 0x1810C0F4;
      switch(i)
      {
        //单体过压
        case 0 :
        	BMS_to_Upmonitor->m_data[0] = i;     //编号 0x00
        	BMS_to_Upmonitor->m_data[1] = (uint8)(BMSMonitor_Volt.Volt_Cell_High1);         //单体电压过高阈值1级 
        	BMS_to_Upmonitor->m_data[2] = ((uint16)(BMSMonitor_Volt.Volt_Cell_High1)) >> 8;             
        	BMS_to_Upmonitor->m_data[3] = (uint8)(BMSMonitor_Volt.Volt_Cell_High2);         //单体电压过高阈值2级
        	BMS_to_Upmonitor->m_data[4] = ((uint16)(BMSMonitor_Volt.Volt_Cell_High2)) >> 8;       
        	BMS_to_Upmonitor->m_data[5] = 0xFF;//(uint8)(BMSMonitor_Volt.Single_Over3_Volt);         //单体电压过高阈值3级
        	BMS_to_Upmonitor->m_data[6] = 0xFF;//((uint16)(BMSMonitor_Volt.Single_Over3_Volt)) >> 8;       
        	BMS_to_Upmonitor->m_data[7] = 0xFF;       
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
          delayus(100); 
        break;
        //单体欠压   
        case 1:
          BMS_to_Upmonitor->m_data[0] = i;     //编号 0x01
         	BMS_to_Upmonitor->m_data[1] = (uint8)(BMSMonitor_Volt.Volt_Cell_Low1);         //单体电压过低阈值1级 
        	BMS_to_Upmonitor->m_data[2] = ((uint16)(BMSMonitor_Volt.Volt_Cell_Low1)) >> 8;             
        	BMS_to_Upmonitor->m_data[3] = (uint8)(BMSMonitor_Volt.Volt_Cell_Low2);         //单体电压过低阈值2级
        	BMS_to_Upmonitor->m_data[4] = ((uint16)(BMSMonitor_Volt.Volt_Cell_Low2)) >> 8;       
        	BMS_to_Upmonitor->m_data[5] = 0xFF;//(uint8)(BMSMonitor_Volt.Single_Under3_Volt);         //单体电压过低阈值3级
        	BMS_to_Upmonitor->m_data[6] = 0xFF;//((uint16)(BMSMonitor_Volt.Single_Under3_Volt)) >> 8;                  
          BMS_to_Upmonitor->m_data[7] = 0xFF;                               
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
          delayus(100);
        break;
        //系统过压  
        case 2:
          BMS_to_Upmonitor->m_data[0] = i;     //编号 0x02
        	BMS_to_Upmonitor->m_data[1] = (uint8)(BMSMonitor_Volt.Volt_Sys_High1);         //系统电压过高阈值1级 
        	BMS_to_Upmonitor->m_data[2] = ((uint16)(BMSMonitor_Volt.Volt_Sys_High1)) >> 8;             
        	BMS_to_Upmonitor->m_data[3] = (uint8)(BMSMonitor_Volt.Volt_Sys_High2);         //系统电压过高阈值2级
        	BMS_to_Upmonitor->m_data[4] = ((uint16)(BMSMonitor_Volt.Volt_Sys_High2)) >> 8;       
        	BMS_to_Upmonitor->m_data[5] = 0xFF;//(uint8)(Monitor_BMS_Volt.Pack_Over3_Volt);         //系统电压过高阈值3级
        	BMS_to_Upmonitor->m_data[6] = 0xFF;//((uint16)(Monitor_BMS_Volt.Pack_Over3_Volt)) >> 8;       
        	BMS_to_Upmonitor->m_data[7] = 0xFF;
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
          delayus(100);
        break;
        //系统欠压
        case 3:
          BMS_to_Upmonitor->m_data[0] = i;     //编号 0x03
        	BMS_to_Upmonitor->m_data[1] = (uint8)(BMSMonitor_Volt.Volt_Sys_Low1);         //系统电压过低阈值1级 
        	BMS_to_Upmonitor->m_data[2] = ((uint16)(BMSMonitor_Volt.Volt_Sys_Low1)) >> 8;             
        	BMS_to_Upmonitor->m_data[3] = (uint8)(BMSMonitor_Volt.Volt_Sys_Low2);         //系统电压过低阈值2级
        	BMS_to_Upmonitor->m_data[4] = ((uint16)(BMSMonitor_Volt.Volt_Sys_Low2)) >> 8;       
        	BMS_to_Upmonitor->m_data[5] = 0xFF;//(uint8)(Monitor_BMS_Volt.Pack_Under3_Volt);         //系统电压过低阈值3级
        	BMS_to_Upmonitor->m_data[6] = 0xFF;//((uint16)(Monitor_BMS_Volt.Pack_Under3_Volt)) >> 8;       
        	BMS_to_Upmonitor->m_data[7] = 0xFF;
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
          delayus(100);
        break;
        //单体压差
        case 4:
          BMS_to_Upmonitor->m_data[0] = i;     //编号 0x04
        	BMS_to_Upmonitor->m_data[1] = (uint8)(BMSMonitor_Volt.Volt_Cell_Diff1);         //单体电压压差阈值1级 
        	BMS_to_Upmonitor->m_data[2] = ((uint16)(BMSMonitor_Volt.Volt_Cell_Diff1)) >> 8;             
        	BMS_to_Upmonitor->m_data[3] = 0xFF;//(uint8)(Monitor_BMS_Volt.Single_Diff2_Volt);         //单体电压压差阈值2级
        	BMS_to_Upmonitor->m_data[4] = 0xFF;//((uint16)(Monitor_BMS_Volt.Single_Diff2_Volt)) >> 8;       
        	BMS_to_Upmonitor->m_data[5] = 0xFF;//(uint8)(Monitor_BMS_Volt.Single_Diff3_Volt);         //单体电压压差阈值3级
        	BMS_to_Upmonitor->m_data[6] = 0xFF;//((uint16)(Monitor_BMS_Volt.Single_Diff3_Volt)) >> 8;       
        	BMS_to_Upmonitor->m_data[7] = 0xFF;
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
          delayus(100);
        break; 
        //系统压差
        case 5:
          BMS_to_Upmonitor->m_data[0] = i;     //编号 0x05
        	BMS_to_Upmonitor->m_data[1] = 0xFF;//(uint8)(Monitor_BMS_Volt.System_Diff1_Volt);         //系统电压压差阈值1级 
        	BMS_to_Upmonitor->m_data[2] = 0xFF;//((uint16)(Monitor_BMS_Volt.System_Diff1_Volt)) >> 8;             
        	BMS_to_Upmonitor->m_data[3] = 0xFF;//(uint8)(Monitor_BMS_Volt.System_Diff2_Volt);         //系统电压压差阈值2级
        	BMS_to_Upmonitor->m_data[4] = 0xFF;//((uint16)(Monitor_BMS_Volt.System_Diff2_Volt)) >> 8;       
        	BMS_to_Upmonitor->m_data[5] = 0xFF;//(uint8)(Monitor_BMS_Volt.System_Diff3_Volt);         //系统电压压差阈值3级
        	BMS_to_Upmonitor->m_data[6] = 0xFF;//((uint16)(Monitor_BMS_Volt.System_Diff3_Volt)) >> 8;       
        	BMS_to_Upmonitor->m_data[7] = 0xFF;
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
          delayus(100);
        break; 
      }
    }

    //0x1811C0F4      电池温度保护阈值信息
    for(i=0;i<4;i++)
    {
      BMS_to_Upmonitor->m_ID = 0x1811C0F4;
      switch(i)
      {
        //充电电芯温度 
        case 0:
        	BMS_to_Upmonitor->m_data[0] = i;     //编号 0x00
        	BMS_to_Upmonitor->m_data[1] = (uint8)(BMSMonitor_Temp.Temp_Charge_High1);         //充电电芯温度过高 
        	BMS_to_Upmonitor->m_data[2] = (uint8)(BMSMonitor_Temp.Temp_Charge_High2);              
        	BMS_to_Upmonitor->m_data[3] = 0xFF;//(uint8)(Monitor_BMS_Temp.Charge_Over3_Temp);         
        	BMS_to_Upmonitor->m_data[4] = (uint8)(BMSMonitor_Temp.Temp_Charge_Low1);        //充电电芯温度过低 
        	BMS_to_Upmonitor->m_data[5] = (uint8)(BMSMonitor_Temp.Temp_Charge_Low2);         
        	BMS_to_Upmonitor->m_data[6] = 0xFF;//(uint8)(Monitor_BMS_Temp.Charge_Under3_Temp);        
        	BMS_to_Upmonitor->m_data[7] = 0xFF;       
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
          delayus(100); 
        break;
        //放电电芯温度   
        case 1:
          BMS_to_Upmonitor->m_data[0] = i;     //编号 0x01
         	BMS_to_Upmonitor->m_data[1] = (uint8)(BMSMonitor_Temp.Temp_DisCharge_High1);         //放电电芯温度过高 
        	BMS_to_Upmonitor->m_data[2] = (uint8)(BMSMonitor_Temp.Temp_DisCharge_High2);             
        	BMS_to_Upmonitor->m_data[3] = 0xFF;//(uint8)(Monitor_BMS_Temp.DisCharge_Over3_Temp);         
        	BMS_to_Upmonitor->m_data[4] = (uint8)(BMSMonitor_Temp.Temp_DisCharge_Low1);        //放电电芯温度过低 
        	BMS_to_Upmonitor->m_data[5] = (uint8)(BMSMonitor_Temp.Temp_DisCharge_Low2);         
        	BMS_to_Upmonitor->m_data[6] = 0xFF;//(uint8)(Monitor_BMS_Temp.DisCharge_Under3_Temp);                  
          BMS_to_Upmonitor->m_data[7] = 0xFF;                               
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
          delayus(100);
        break;
        //电池温差   
        case 2:
          BMS_to_Upmonitor->m_data[0] = i;     //编号 0x02
        	BMS_to_Upmonitor->m_data[1] = (uint8)(BMSMonitor_Temp.Temp_Charge_Diff1);         //充电电池温差过大
        	BMS_to_Upmonitor->m_data[2] = 0xFF;//(uint8)(Monitor_BMS_Temp.Charge_Diff2_Temp);             
        	BMS_to_Upmonitor->m_data[3] = 0xFF;//(uint8)(Monitor_BMS_Temp.Charge_Diff3_Temp);         
        	BMS_to_Upmonitor->m_data[4] = (uint8)(BMSMonitor_Temp.Temp_DisCharge_Diff1);      //放电电池温差过大 
        	BMS_to_Upmonitor->m_data[5] = 0xFF;//(uint8)(Monitor_BMS_Temp.DisCharge_Diff2_Temp);       
        	BMS_to_Upmonitor->m_data[6] = 0xFF;//(uint8)(Monitor_BMS_Temp.DisCharge_Diff3_Temp);       
        	BMS_to_Upmonitor->m_data[7] = 0xFF;
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
          delayus(100);
        break;
        //充电枪&BMS过温阈值
        case 3:
          BMS_to_Upmonitor->m_data[0] = i;     //编号 0x03
        	BMS_to_Upmonitor->m_data[1] = 0xFF;//(uint8)(Monitor_BMS_Temp.ChargeGun_Over1_Temp);         //充电枪过温
        	BMS_to_Upmonitor->m_data[2] = 0xFF;//(uint8)(Monitor_BMS_Temp.ChargeGun_Over2_Temp);             
        	BMS_to_Upmonitor->m_data[3] = 0xFF;//(uint8)(Monitor_BMS_Temp.ChargeGun_Over3_Temp);         
        	BMS_to_Upmonitor->m_data[4] = 0xFF;//(uint8)(Monitor_BMS_Temp.Chip_Over1_Temp);              //BMS过温
        	BMS_to_Upmonitor->m_data[5] = 0xFF;//(uint8)(Monitor_BMS_Temp.Chip_Over2_Temp);         
        	BMS_to_Upmonitor->m_data[6] = 0xFF;//(uint8)(Monitor_BMS_Temp.Chip_Over3_Temp);       
        	BMS_to_Upmonitor->m_data[7] = 0xFF;
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
          delayus(100);
        break;
      }
    }

    //0x1812C0F4      电池电流阈值信息
    for(i=0;i<3;i++)
    {
      BMS_to_Upmonitor->m_ID = 0x1812C0F4;
      switch(i)
      {
        //系统放电过流 
        case 0:
        	BMS_to_Upmonitor->m_data[0] = i;     //编号 0x00
        	BMS_to_Upmonitor->m_data[1] = (uint8)(BMSMonitor_Curr.Current_DisCharge_High1);          
        	BMS_to_Upmonitor->m_data[2] = ((uint16)(BMSMonitor_Curr.Current_DisCharge_High1)) >> 8;              
        	BMS_to_Upmonitor->m_data[3] = (uint8)(BMSMonitor_Curr.Current_DisCharge_High2);         
        	BMS_to_Upmonitor->m_data[4] = ((uint16)(BMSMonitor_Curr.Current_DisCharge_High2)) >> 8;       
        	BMS_to_Upmonitor->m_data[5] = 0xFF;//(uint8)(Monitor_BMS_Current.DisCharge_Over3_Current);         
        	BMS_to_Upmonitor->m_data[6] = 0xFF;//((uint16)(Monitor_BMS_Current.DisCharge_Over3_Current)) >> 8;       
        	BMS_to_Upmonitor->m_data[7] = 0xFF;       
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
          delayus(100); 
        break;
        //系统充电过流 
        case 1:
          BMS_to_Upmonitor->m_data[0] = i;     //编号 0x01
         	BMS_to_Upmonitor->m_data[1] = (uint8)(BMSMonitor_Curr.Current_Charge_High1);         
        	BMS_to_Upmonitor->m_data[2] = ((uint16)(BMSMonitor_Curr.Current_Charge_High1)) >> 8;             
        	BMS_to_Upmonitor->m_data[3] = (uint8)(BMSMonitor_Curr.Current_Charge_High2);         
        	BMS_to_Upmonitor->m_data[4] = ((uint16)(BMSMonitor_Curr.Current_Charge_High2)) >> 8;         
        	BMS_to_Upmonitor->m_data[5] = 0xFF;//(uint8)(Monitor_BMS_Current.Charge_Over3_Current);         
        	BMS_to_Upmonitor->m_data[6] = 0xFF;//((uint16)(Monitor_BMS_Current.Charge_Over3_Current)) >> 8;                  
          BMS_to_Upmonitor->m_data[7] = 0xFF;                               
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
          delayus(100);
        break;
        //SOC阈值
        case 2:
          BMS_to_Upmonitor->m_data[0] = i;     //编号 0x02
        	BMS_to_Upmonitor->m_data[1] = 0xFF;//(uint8)(Monitor_BMS_Current.SOC_Low1);         //低SOC
        	BMS_to_Upmonitor->m_data[2] = 0xFF;//(uint8)(Monitor_BMS_Current.SOC_Low2);             
        	BMS_to_Upmonitor->m_data[3] = 0xFF;//(uint8)(Monitor_BMS_Current.SOC_Low3);         
        	BMS_to_Upmonitor->m_data[4] = 0xFF;//(uint8)(Monitor_BMS_Current.SOC_High1);        //高SOC
        	BMS_to_Upmonitor->m_data[5] = 0xFF;//(uint8)(Monitor_BMS_Current.SOC_High2);         
        	BMS_to_Upmonitor->m_data[6] = 0xFF;//(uint8)(Monitor_BMS_Current.SOC_High3);       
        	BMS_to_Upmonitor->m_data[7] = 0xFF;
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
          delayus(100);
        break;
      }
    }

    //0x1813C0F4     绝缘故障阈值信息
    BMS_to_Upmonitor->m_ID = 0x1813C0F4;     	
    BMS_to_Upmonitor->m_data[0] = 0x00;     //编号  0x00
    BMS_to_Upmonitor->m_data[1] = 0xFF;//(uint8)(Monitor_BMS_Insulation.Insulation_Resis1);         
    BMS_to_Upmonitor->m_data[2] = 0xFF;//((uint16)(Monitor_BMS_Insulation.Insulation_Resis1)) >> 8;             
    BMS_to_Upmonitor->m_data[3] = 0xFF;//(uint8)(Monitor_BMS_Insulation.Insulation_Resis2);         
    BMS_to_Upmonitor->m_data[4] = 0xFF;//((uint16)(Monitor_BMS_Insulation.Insulation_Resis2)) >> 8;         
    BMS_to_Upmonitor->m_data[5] = 0xFF;//(uint8)(Monitor_BMS_Insulation.Insulation_Resis3);         
    BMS_to_Upmonitor->m_data[6] = 0xFF;//((uint16)(Monitor_BMS_Insulation.Insulation_Resis3)) >> 8;                  
    BMS_to_Upmonitor->m_data[7] = 0xFF;                               
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
    delayus(100);

    MonitorBMS_Start.CellMessageSend_judge = 0x00;
  }
  
  //实时更新数据信息 
  //0x1820C0F4    电池系统自检及时间信息
  for(i=0;i<3;i++)
  {
    BMS_to_Upmonitor->m_ID = 0x1820C0F4;
    switch(i)
    {
      case 0 :     //系统实时时间
      	BMS_to_Upmonitor->m_data[0] = i;     //编号 0x00
      	BMS_to_Upmonitor->m_data[1] = Read_IIC_Time.IIC_Read_Year;     //系统实时时间:年 
      	BMS_to_Upmonitor->m_data[2] = Read_IIC_Time.IIC_Read_Month;    //系统实时时间:月         
      	BMS_to_Upmonitor->m_data[3] = Read_IIC_Time.IIC_Read_Day;      //系统实时时间:日
      	BMS_to_Upmonitor->m_data[4] = Read_IIC_Time.IIC_Read_Hour;     //系统实时时间:时  
      	BMS_to_Upmonitor->m_data[5] = Read_IIC_Time.IIC_Read_Minute;   //系统实时时间:分
      	BMS_to_Upmonitor->m_data[6] = 0xFF;
      	BMS_to_Upmonitor->m_data[7] = 0xFF;       
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
        delayus(100);
      break;
      
      case 1:    //系统运行状态及时长
        BMS_to_Upmonitor->m_data[0] = i;     //编号 0x01
        BMS_to_Upmonitor->m_data[1] = g_WorkStateJudge.WorkState;    //系统运行状态：00 放电，01 快充，02慢充  
        BMS_to_Upmonitor->m_data[2] = (uint8)(EEprom_Data.BmsRunTime_min/60);    //BMS系统运行时间（小时）       
        BMS_to_Upmonitor->m_data[3] = (uint16)(EEprom_Data.BmsRunTime_min/60) >> 8;    
        BMS_to_Upmonitor->m_data[4] = 0xFF;    //BMS系统单次运行时长
        BMS_to_Upmonitor->m_data[5] = 0xFF;        
        BMS_to_Upmonitor->m_data[6] = 0xFF;    //BMS系统上次运行时长              
        BMS_to_Upmonitor->m_data[7] = 0xFF;   
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
        delayus(100);
      break;
      
      case 2:      //电池自检状态及自检失败原因
      
        BMS_to_Upmonitor->m_data[0] = i;     //编号 0x02
        BMS_to_Upmonitor->m_data[1] = BMSCheckSelf.Check_Self_state;   //电池自检状态  00 自检中，01 成功，02 失败
        if(BMSCheckSelf.Check_Self_state == 1)
        {
          BMS_to_Upmonitor->m_data[2] = 0xFF;     //自检失败原因      
          BMS_to_Upmonitor->m_data[3] = 0xFF;
          BMS_to_Upmonitor->m_data[4] = 0xFF; 
          BMS_to_Upmonitor->m_data[5] = 0xFF;
          BMS_to_Upmonitor->m_data[6] = 0xFF;
          BMS_to_Upmonitor->m_data[7] = 0xFF;
        }
        else if(BMSCheckSelf.Check_Self_state == 2)
        {
          BMS_to_Upmonitor->m_data[2] = (BMSCheckSelf.Check_Self & 0x01);     
          BMS_to_Upmonitor->m_data[3] = (BMSCheckSelf.Check_Self & 0x02);     
          BMS_to_Upmonitor->m_data[4] = (BMSCheckSelf.Check_Self & 0x04);     
          BMS_to_Upmonitor->m_data[5] = 0xFF;
          BMS_to_Upmonitor->m_data[6] = 0xFF;
          BMS_to_Upmonitor->m_data[7] = 0xFF;
        }
        
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
        delayus(100);
      break;
    }
  }
  
  //0x1830C0F4    电池系统总压信息
  for(i=0;i<2;i++)
  {
    BMS_to_Upmonitor->m_ID = 0x1830C0F4; 
    switch(i)
    {
      case 0:    //电池总压及压差
      	BMS_to_Upmonitor->m_data[0] = i;     //编号 0x00
      	BMS_to_Upmonitor->m_data[1] = (uint8)(DataColletInfo.SysVolt_Total*0.001);     //电池总压（单体累加）   0.1V分辨率
      	BMS_to_Upmonitor->m_data[2] = ((uint16)(DataColletInfo.SysVolt_Total*0.001))>>8;             
      	BMS_to_Upmonitor->m_data[3] = 0xFF; //(uint8)(SOC_DATA.BMSU_Volt*10);     //电池总压（总压模块总压）  0.1V分辨率
      	BMS_to_Upmonitor->m_data[4] = 0xFF; //((uint16)(SOC_DATA.BMSU_Volt*10)) >> 8;        
      	BMS_to_Upmonitor->m_data[5] = 0xFF; //(uint8)(abs(VoltInfo.TotalVolt_V-SOC_DATA.BMSU_Volt)*10);  //压差（abs(累加电压-测试电压)）    0.1V分辨率
      	BMS_to_Upmonitor->m_data[6] = 0xFF; //((uint16)(abs(VoltInfo.TotalVolt_V-SOC_DATA.BMSU_Volt)*10)) >> 8;
      	BMS_to_Upmonitor->m_data[7] = 0xFF;       
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
        delayus(100);
      break;
      
      case 1:    //单体最高/最低电压及压差
        BMS_to_Upmonitor->m_data[0] = i;     //编号 0x01
        BMS_to_Upmonitor->m_data[1] = (uint8)(VoltInfo.CellVolt_Max);          //单体最高电压（分辨率：0.0001V）
        BMS_to_Upmonitor->m_data[2] = ((uint16)(VoltInfo.CellVolt_Max)) >> 8;   
        BMS_to_Upmonitor->m_data[3] = (uint8)(VoltInfo.CellVolt_Min);          //单体最低电压（分辨率：0.0001V)   
        BMS_to_Upmonitor->m_data[4] = ((uint16)(VoltInfo.CellVolt_Min)) >> 8;
        BMS_to_Upmonitor->m_data[5] = (uint8)(VoltInfo.CellVolt_Diff);                //单体压差（分辨率：0.0001V) 
        BMS_to_Upmonitor->m_data[6] = ((uint16)(VoltInfo.CellVolt_Diff)) >> 8;             
        BMS_to_Upmonitor->m_data[7] = 0xFF;   
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
        delayus(100);
      break;
    }
  }
   
  //0x1840C0F4    电池系统温度信息
  for(i=0;i<2;i++)
  {
    BMS_to_Upmonitor->m_ID = 0x1840C0F4;
    switch(i)
    {
      case 0:     //单体最高/最低温度及温差
      	BMS_to_Upmonitor->m_data[0] = i;     //编号 0x00
      	BMS_to_Upmonitor->m_data[1] = (uint8)(TempInfo.CellTemp_Max);     //单体电池最高温度   1℃分辨率
      	BMS_to_Upmonitor->m_data[2] = (uint8)(TempInfo.CellTemp_Min);     //单体电池最低温度   1℃分辨率             
      	BMS_to_Upmonitor->m_data[3] = (uint8)(TempInfo.CellTemp_Diff + 40);    //单体电池温差  1℃分辨率
      	BMS_to_Upmonitor->m_data[4] = 0xFF;        
      	BMS_to_Upmonitor->m_data[5] = 0xFF;  
      	BMS_to_Upmonitor->m_data[6] = 0xFF;
      	BMS_to_Upmonitor->m_data[7] = 0xFF;       
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
        delayus(100);
      break;
      
      case 1:    //主板温度及慢充/快充充电枪温度
        BMS_to_Upmonitor->m_data[0] = i;     //编号 0x01
        BMS_to_Upmonitor->m_data[1] = 0xFF;  //(uint8)(ChageAndBMU_Temp.BMU_Temp);           //主板温度   1℃分辨率
        BMS_to_Upmonitor->m_data[2] = 0xFF;  //慢充充电枪温度1 
        BMS_to_Upmonitor->m_data[3] = 0xFF;  //慢充充电枪温度2
        #if(Chag_Temp_Enable == 1)            
        BMS_to_Upmonitor->m_data[4] = ChageAndBMU_Temp.Chage_Temp1;  //快充充电枪温度1
        BMS_to_Upmonitor->m_data[5] = ChageAndBMU_Temp.Chage_Temp2;  //快充充电枪温度2
        #else
        BMS_to_Upmonitor->m_data[4] = 0xFF;  //快充充电枪温度1
        BMS_to_Upmonitor->m_data[5] = 0xFF;  //快充充电枪温度2
        #endif               
        BMS_to_Upmonitor->m_data[6] = 0xFF;  //快充充电枪温度3           
        BMS_to_Upmonitor->m_data[7] = 0xFF;  //快充充电枪温度4 
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
        delayus(100);
      break;
    }
  }
  
  //0x1850C0F4    电池系统电流信息
  BMS_to_Upmonitor->m_ID = 0x1850C0F4;     	
	BMS_to_Upmonitor->m_data[0] = (uint8)((DataColletInfo.DataCollet_Current_Filter + 750)*10);        //电流测量值   0.1A分辨率  偏移量：-750
	BMS_to_Upmonitor->m_data[1] = ((uint16)((DataColletInfo.DataCollet_Current_Filter + 750)*10))>>8;
	BMS_to_Upmonitor->m_data[2] = (uint8)((CurrentLimit.Current_Charge_Constant + 750)*10);     //充电电流限制值   0.1A分辨率  偏移量：-750  （未写）          
	BMS_to_Upmonitor->m_data[3] = ((uint16)((CurrentLimit.Current_Charge_Constant + 750)*10))>>8;
	BMS_to_Upmonitor->m_data[4] = 0xFF;  //(uint8)((Current_Limit.ConstantDischargeCurrent + 750)*10);     //放电电流限制值   0.1A分辨率  偏移量：-750   （未写）
	BMS_to_Upmonitor->m_data[5] = 0xFF;  //((uint16)((Current_Limit.ConstantDischargeCurrent + 750)*10))>>8;  
	BMS_to_Upmonitor->m_data[6] = 0xFF;  //(uint8)((Current_Limit.FeedbackCurrent + 750)*10);          //最大反馈电流限制值   0.1A分辨率  偏移量：-750   （未写）
	BMS_to_Upmonitor->m_data[7] = 0xFF;  //((uint16)((Current_Limit.FeedbackCurrent + 750)*10))>>8;       
  while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
  delayus(100);

  //0x1860C0F4    电池系统绝缘信息
  BMS_to_Upmonitor->m_ID = 0x1860C0F4;     	
  BMS_to_Upmonitor->m_data[0] = 0xFF;//(uint8)(InsulInfo.insulation_Vposit*10);    //正对地电压   0.1V分辨率
  BMS_to_Upmonitor->m_data[1] = 0xFF;//((uint16)(InsulInfo.insulation_Vposit*10))>>8;           
  BMS_to_Upmonitor->m_data[2] = 0xFF;//(uint8)(InsulInfo.insulation_Vnegt*10);           //负对地电压   0.1V分辨率
  BMS_to_Upmonitor->m_data[3] = 0xFF;//((uint16)(InsulInfo.insulation_Vnegt*10))>>8;              
  BMS_to_Upmonitor->m_data[4] = (uint8)(InsulInfo.Insul_Resis_Pos*10);           //正对地电阻值   0.1V分辨率
  BMS_to_Upmonitor->m_data[5] = ((uint16)(InsulInfo.Insul_Resis_Pos*10))>>8;               
  BMS_to_Upmonitor->m_data[6] = (uint8)(InsulInfo.Insul_Resis_Neg*10);            //负对地电阻值  0.1V分辨率（未写）
  BMS_to_Upmonitor->m_data[7] = ((uint16)(InsulInfo.Insul_Resis_Neg*10))>>8;
  while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
  delayus(100);
     
  //0x1870C0F4    电池SOC、SOH信息
  for(i=0;i<2;i++)
  {
    BMS_to_Upmonitor->m_ID = 0x1870C0F4;
    switch(i)
    {
      case 0:     //SOC,SOC,单次充/放电量
      	BMS_to_Upmonitor->m_data[0] = i;     //编号 0x00
      	BMS_to_Upmonitor->m_data[1] = (uint8)(SOCInfo.SOC_ValueRead*100);      //SOC_Read      1%分辨率
      	BMS_to_Upmonitor->m_data[2] = (uint8)(SOCInfo.SOC_ValueVoltGet*100);      //SOC_Volt      1℃分辨率             
      	BMS_to_Upmonitor->m_data[3] = (uint8)(BMSMonitor_SOH.SOH*100);     //SOH           1%分辨率
      	BMS_to_Upmonitor->m_data[4] = 0xFF;                                     //单次充电量
      	BMS_to_Upmonitor->m_data[5] = 0xFF;  
      	BMS_to_Upmonitor->m_data[6] = (uint8)(EnergyInfo.Energy_Once_DisCharge*10);    //单次放电量
      	BMS_to_Upmonitor->m_data[7] = ((uint16)(EnergyInfo.Energy_Once_DisCharge*10))>>8;        
        while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
        delayus(100);
      break;
      
      case 1:    //主板温度及慢充/快充充电枪温度
        BMS_to_Upmonitor->m_data[0] = i;      //编号 0x01
        BMS_to_Upmonitor->m_data[1] = (uint8)(EnergyInfo.Energy_Total_Charge*10);            //累计充电电量   0.1kWh分辨率
        BMS_to_Upmonitor->m_data[2] = ((uint16)(EnergyInfo.Energy_Total_Charge*10)) >> 8; 
        BMS_to_Upmonitor->m_data[3] = ((uint32)(EnergyInfo.Energy_Total_Charge*10)) >> 16;             
        BMS_to_Upmonitor->m_data[4] = (uint8)(EnergyInfo.Energy_Total_DisCharge*10);           //累计放电电量   0.1kWh分辨率
        BMS_to_Upmonitor->m_data[5] = ((uint16)(EnergyInfo.Energy_Total_DisCharge*10)) >> 8;              
        BMS_to_Upmonitor->m_data[6] = ((uint32)(EnergyInfo.Energy_Total_DisCharge*10)) >> 16;         
        BMS_to_Upmonitor->m_data[7] = 0xFF;
        while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
        delayus(100);
      break;
    }
  }
  
  //0x1880C0F4    电池故障信息
  for(i=0;i<2;i++)
  {
    BMS_to_Upmonitor->m_ID = 0x1880C0F4;
    switch(i)
    {
      case 0:     
      	BMS_to_Upmonitor->m_data[0] = i;     //编号  0x00
      	#if(RelayJudge_Enale == 1)    //继电器判断功能使能
      	  BMS_to_Upmonitor->m_data[1] = (DiscFlt.RelayFault_PositiveHold & 0x03) + ((DiscFlt.RelayFault_PositiveBreak << 2) & 0x0C) + (((DiscFlt.RelayFault_PreChag = 0x01) << 4) & 0x30) + (((DiscFlt.RelayFault_Chag = 0x01) << 6) & 0xC0);      //继电器状态 0 打开，1关闭
      	  BMS_to_Upmonitor->m_data[2] = 0xFF;              
        	BMS_to_Upmonitor->m_data[3] = (DiscFlt.CellOverVolt & 0x03) + ((DiscFlt.CellUnderVolt << 2) & 0x0C) + ((DiscFlt.CellDiffVolt << 4) & 0x30) + ((DiscFlt.PackOverVolt << 6) & 0xC0);     //单体电池过压/欠压/压差过大，总压过高
        	BMS_to_Upmonitor->m_data[4] = (DiscFlt.PackUnderVolt & 0x03) + ((DiscFlt.JuyuanFault << 2) & 0x0C) + ((DiscFlt.PointHighTemp << 4) & 0x30) + ((DiscFlt.PointLowTemp << 6) & 0xC0);     //总压过低，绝缘故障，放电温度过高/过低
        	BMS_to_Upmonitor->m_data[5] = (DiscFlt.PointDiffTemp & 0x03) + ((DiscFlt.PointHighTemp << 2) & 0x0C) + ((DiscFlt.PointLowTemp << 4) & 0x30) + ((DiscFlt.PointDiffTemp << 6) & 0xC0);   //放电温差过大，充电温度过高/过低，充电温差过大
        	BMS_to_Upmonitor->m_data[6] = (DiscFlt.HighSOC & 0x03) + ((DiscFlt.LowSOC << 2) & 0x0C) + ((DiscFlt.OverCurnt << 4) & 0x30) + ((DiscFlt.OverCurnt << 6) & 0xC0);   //SOC高，SOC低，充电电流过大，放电电流过大                                     
        	BMS_to_Upmonitor->m_data[7] = (0x00 & 0x03) + ((0x00 << 2) & 0x0C) + ((DiscFlt.PackCSSUDiffVolt << 4) & 0x30) + (0b11 << 6) ;  //充电枪温度过高（未写），慢充接触器温度过高（未写），总压测量故障       
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
          delayus(100);
        #else                         //继电器判断功能未使能
          BMS_to_Upmonitor->m_data[1] = 0x00;
      	  BMS_to_Upmonitor->m_data[2] = 0xFF;              
        	BMS_to_Upmonitor->m_data[3] = 0x00;  //单体电池过压/欠压/压差过大，总压过高
        	BMS_to_Upmonitor->m_data[4] = 0x00;  //总压过低，绝缘故障，放电温度过高/过低
        	BMS_to_Upmonitor->m_data[5] = 0x00;  //放电温差过大，充电温度过高/过低，充电温差过大
        	BMS_to_Upmonitor->m_data[6] = 0x00;  //SOC高，SOC低，充电电流过大，放电电流过大                                     
        	BMS_to_Upmonitor->m_data[7] = 0x00;  //充电枪温度过高，慢充接触器温度过高，总压测量故障       
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
          delayus(100);
        #endif
      	
      break;
      
      case 1:    
        BMS_to_Upmonitor->m_data[0] = i;      //编号 0x01
        BMS_to_Upmonitor->m_data[1] = 0xFF;   //BMS芯片温度过高
        BMS_to_Upmonitor->m_data[2] = 0xFF;//(DiscFlt.HIVL_ECT0_Fault & 0x01) + ((DiscFlt.HIVL_ECT1_Fault << 1) & 0x02) + ((DiscFlt.HIVL_ECT2_Fault << 2) & 0x04) + ((DiscFlt.HIVL_ECT3_Fault << 3) & 0x08) + (0b11111 << 4) ;      //互锁故障
        #if(SysModuleNum<2)
          BMS_to_Upmonitor->m_data[3] = ((State_Offline.CSSU1) & 0x01) + ((State_Offline.VCU << 1) & 0x02) + ((State_Offline.HVU << 2) & 0x04) + ((0x01 << 3) & 0x08) + (0b11111 << 4);     //通信故障  0000 正常，0001 CSSU掉线，0010 VUC掉线，0100 HVU掉线，1000 TBOX掉线(暂时没有设为0x01)          
        #elif(SysModuleNum<3)
          BMS_to_Upmonitor->m_data[3] = ((State_Offline.CSSU1 | State_Offline.CSSU2) & 0x01) + ((State_Offline.VCU << 1) & 0x02) + ((State_Offline.HVU << 2) & 0x04) + ((0x01 << 3) & 0x08) + (0b11111 << 4);     //通信故障  0000 正常，0001 CSSU掉线，0010 VUC掉线，0100 HVU掉线，1000 TBOX掉线(暂时没有设为0x01)          
        #elif(SysModuleNum<4)
          BMS_to_Upmonitor->m_data[3] = ((State_Offline.CSSU1 | State_Offline.CSSU2 | State_Offline.CSSU3) & 0x01) + ((State_Offline.VCU << 1) & 0x02) + ((State_Offline.HVU << 2) & 0x04) + ((0x01 << 3) & 0x08) + (0b11111 << 4);     //通信故障  0000 正常，0001 CSSU掉线，0010 VUC掉线，0100 HVU掉线，1000 TBOX掉线(暂时没有设为0x01)          
        #elif(SysModuleNum<5)
          BMS_to_Upmonitor->m_data[3] = ((State_Offline.CSSU1 | State_Offline.CSSU2 | State_Offline.CSSU3 | State_Offline.CSSU4) & 0x01) + ((State_Offline.VCU << 1) & 0x02) + ((State_Offline.HVU << 2) & 0x04) + ((0x01 << 3) & 0x08) + (0b11111 << 4);     //通信故障  0000 正常，0001 CSSU掉线，0010 VUC掉线，0100 HVU掉线，1000 TBOX掉线(暂时没有设为0x01)          
        #elif(SysModuleNum<6)
          BMS_to_Upmonitor->m_data[3] = ((State_Offline.CSSU1 | State_Offline.CSSU2 | State_Offline.CSSU3 | State_Offline.CSSU4 | State_Offline.CSSU5) & 0x01) + ((State_Offline.VCU << 1) & 0x02) + ((State_Offline.HVU << 2) & 0x04) + ((0x01 << 3) & 0x08) + (0b11111 << 4);     //通信故障  0000 正常，0001 CSSU掉线，0010 VUC掉线，0100 HVU掉线，1000 TBOX掉线(暂时没有设为0x01)          
        #elif(SysModuleNum<7)
          BMS_to_Upmonitor->m_data[3] = ((State_Offline.CSSU1 | State_Offline.CSSU2 | State_Offline.CSSU3 | State_Offline.CSSU4 | State_Offline.CSSU5 | State_Offline.CSSU6) & 0x01) + ((State_Offline.VCU << 1) & 0x02) + ((State_Offline.HVU << 2) & 0x04) + ((0x01 << 3) & 0x08) + (0b11111 << 4);     //通信故障  0000 正常，0001 CSSU掉线，0010 VUC掉线，0100 HVU掉线，1000 TBOX掉线(暂时没有设为0x01)          
        #endif 
        BMS_to_Upmonitor->m_data[4] = 0xFF;           
        BMS_to_Upmonitor->m_data[5] = 0xFF;              
        BMS_to_Upmonitor->m_data[6] = 0xFF;         
        BMS_to_Upmonitor->m_data[7] = 0xFF;
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
        delayus(100);
      break;
    }
  }
  /*
  //1890C0F4 BMS与充电机信息
  for(i=0;i<2;i++)
  {
    BMS_to_Upmonitor->m_ID = 0x1890C0F4;
    switch(i)
    {
      case 0:     
        //BMS
      	BMS_to_Upmonitor->m_data[0] = i;     //编号 0x00
      	//充电中止原因
      	BMS_to_Upmonitor->m_data[1] = (Charge_Error.BMS_ChagInf.Upto_GivenSOC & 0x03) + ((Charge_Error.BMS_ChagInf.Upto_GivenSysVolt << 2) & 0x0C) + ((Charge_Error.BMS_ChagInf.Upto_GivenCellVolt << 4) & 0x30) + ((Charge_Error.BMS_ChagInf.Charger_Suspend << 6) & 0xC0);  
      	//故障原因    
      	BMS_to_Upmonitor->m_data[2] = (Charge_Error.BMS_ChagInf.Check_Self & 0x03) + ((Charge_Error.BMS_ChagInf.Cur_Mismatch << 2) & 0x0C) + ((Charge_Error.BMS_ChagInf.Volt_Mismatch << 4) & 0x30) + ((Charge_Error.BMS_ChagInf.Jueyuan_Fault << 6) & 0xC0);                    
      	BMS_to_Upmonitor->m_data[3] = (Charge_Error.BMS_ChagInf.Connect_Overtemp & 0x03) + ((Charge_Error.BMS_ChagInf.BMS_Overtemp << 2) & 0x0C) + ((Charge_Error.BMS_ChagInf.Sys_Overtime << 4) & 0x30) + ((Charge_Error.BMS_ChagInf.Relay_Off << 6) & 0xC0); 
      	BMS_to_Upmonitor->m_data[4] = (Charge_Error.BMS_ChagInf.Connect_CC2_Fault & 0x03) + ((Charge_Error.BMS_ChagInf.Other_Fault << 2) & 0x0C) + ((Charge_Error.BMS_ChagInf.Chag_OverCur << 4) & 0x30) + ((Charge_Error.BMS_ChagInf.Volt_Error << 6) & 0xC0);  
        //超时
      	BMS_to_Upmonitor->m_data[5] = (Charge_Error.BMS_Connect_Overtime.CRM00_Overtime & 0x03) + ((Charge_Error.BMS_Connect_Overtime.CRMAA_Overtime << 2) & 0x0C) + ((Charge_Error.BMS_Connect_Overtime.CTSCML_Overtime << 4) & 0x30) + ((Charge_Error.BMS_Connect_Overtime.CRO_Overtime << 6) & 0xC0);   
      	BMS_to_Upmonitor->m_data[6] = (Charge_Error.BMS_Connect_Overtime.CCS_Overtime & 0x03) + ((Charge_Error.BMS_Connect_Overtime.CST_Overtime << 2) & 0x0C) + ((Charge_Error.BMS_Connect_Overtime.CSD_Overtime << 4) & 0x30) + ((0b00 << 6) & 0xC0);
      	BMS_to_Upmonitor->m_data[7] = 0xFF;       
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
        delayus(100);
      break;
      
      case 1:   
        //充电机
      	BMS_to_Upmonitor->m_data[0] = i;     //编号 0x01
      	//充电中止原因
      	BMS_to_Upmonitor->m_data[1] = (Charge_Error.Charger_ChagInf.Normal_Suspend & 0x03) + ((Charge_Error.Charger_ChagInf.Artificial_Suspend << 2) & 0x0C) + ((Charge_Error.Charger_ChagInf.Fault_Suspend << 4) & 0x30) + ((Charge_Error.Charger_ChagInf.BMS_Suspend << 6) & 0xC0);  
      	//故障原因    
      	BMS_to_Upmonitor->m_data[2] = (Charge_Error.Charger_ChagInf.Charger_Overtemp & 0x03) + ((Charge_Error.Charger_ChagInf.Chag_Connect << 2) & 0x0C) + ((Charge_Error.Charger_ChagInf.Charger_Inner_Overtemp << 4) & 0x30) + ((Charge_Error.Charger_ChagInf.Energy_Transfer << 6) & 0xC0);                    
      	BMS_to_Upmonitor->m_data[3] = (Charge_Error.Charger_ChagInf.Charger_Emergency_Stop & 0x03) + ((Charge_Error.Charger_ChagInf.Other_Fault << 2) & 0x0C) + ((Charge_Error.Charger_ChagInf.Cur_Error << 4) & 0x30) + ((Charge_Error.Charger_ChagInf.Volt_Error << 6) & 0xC0); 
      	BMS_to_Upmonitor->m_data[4] = 0xFF;  
        //超时
      	BMS_to_Upmonitor->m_data[5] = (Charge_Error.Charger_Connect_Overtime.BRM_Overtime & 0x03) + ((Charge_Error.Charger_Connect_Overtime.BCP_Overtime << 2) & 0x0C) + ((Charge_Error.Charger_Connect_Overtime.BRO_Overtime << 4) & 0x30) + ((Charge_Error.Charger_Connect_Overtime.BCS_Overtime << 6) & 0xC0);   
      	BMS_to_Upmonitor->m_data[6] = (Charge_Error.Charger_Connect_Overtime.BCL_Overtime & 0x03) + ((Charge_Error.Charger_Connect_Overtime.BST_Overtime << 2) & 0x0C) + ((Charge_Error.Charger_Connect_Overtime.BSD_Overtime << 4) & 0x30) + ((0b00 << 6) & 0xC0);
      	BMS_to_Upmonitor->m_data[7] = 0xFF; 
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
        delayus(100);
      break;
    }
  }
  */
   /*
  //修改后
  //以下两帧报文CANpro验收码：0xC4CE07A0，屏蔽码：000F0000
  //0x1899C0F4（数据测试报文）,CANpro验收码：0xC4CE07A0,屏蔽码：0x00000000
  BMS_to_Upmonitor.m_ID = 0x1899C0F4;
	BMS_to_Upmonitor.m_data[0] = (uint8)(EnergyInfo.Energy_Total_Charge*10);       //子板总压值（子板总压总和）0.1V分辨率
	BMS_to_Upmonitor.m_data[1] = ((uint16)(EnergyInfo.Energy_Total_Charge*10))>>8;
	BMS_to_Upmonitor.m_data[2] = (uint8)(EnergyInfo.Energy_Total_DisCharge*10);                //总压模块总电压值， 0.1V分辨率
	BMS_to_Upmonitor.m_data[3] = ((uint16)(EnergyInfo.Energy_Total_DisCharge*10)) >> 8; 
	BMS_to_Upmonitor.m_data[4] = (uint8)g_Batt_VoltMesg.Single_Max_Volt;        //单体最高电压（分辨率：0.0001V）
	BMS_to_Upmonitor.m_data[5] = (g_Batt_VoltMesg.Single_Max_Volt>>8); 
	BMS_to_Upmonitor.m_data[6] = (uint8)(g_Batt_VoltMesg.Single_Min_Volt);      //单体最低电压（分辨率：0.0001V)
	BMS_to_Upmonitor.m_data[7] = (g_Batt_VoltMesg.Single_Min_Volt>>8);
  Return_Value = BMS_CAN3SendMsg_Monitor(&BMS_to_Upmonitor); 
      
  //0x1898C0F4（数据测试报文）,CANpro验收码：0xC4C607A0,屏蔽码：0x00000000
  BMS_to_Upmonitor.m_ID = 0x1898C0F4;
	BMS_to_Upmonitor.m_data[0] = (uint8)((SOC_DATA.Filter_Current+400)*10);       //总电流（分辨率：0.1A，偏移量：400）
	BMS_to_Upmonitor.m_data[1] = ((uint16)((SOC_DATA.Filter_Current+400)*10))>>8;
	BMS_to_Upmonitor.m_data[2] = SOC_LEP_DATA.SOC_t;                             //进入SOC初始化标记位,1:SOC初始化;2:SOC查表
	BMS_to_Upmonitor.m_data[3] = SOC_LEP_DATA.EE_Value;                          //EEprom错误标记位,1:EEEprom正常读取;2:EEE存在错误值      //备用
	BMS_to_Upmonitor.m_data[4] = (uint8)(SOCInfo.SOC_ValueRead*100);                 //输出SOC值（分辨率：1%）
	BMS_to_Upmonitor.m_data[5] = (uint8)(SOCInfo.SOC_ValueVoltGet*100);                 //按电压计算的SOC值（分辨率：1%）
	BMS_to_Upmonitor.m_data[6] = (uint8)(SOCInfo.SOC_Init);                          //程序错误1:错误;0:无错
	BMS_to_Upmonitor.m_data[7] = ((uint16)(SOCInfo.SOC_Init))>>8;                                           //备用
  Return_Value = BMS_CAN3SendMsg_Monitor(&BMS_to_Upmonitor);  
  */
  //Task_Flag_Counter.BMS_to_UpMonitor++;  
}




