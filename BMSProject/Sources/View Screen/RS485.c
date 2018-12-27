/*=======================================================================
 *Subsystem:   裸机
 *File:        RS485.C
 *Author:      WenMing
 *Description: 通信：SCI2.
               接口：PJ0（RXD）；PJ1（TXD）
               波特率：
               调试：观察RS485_Receive数据与显示屏数据是否一致,以及IIC模块时间的显示；
 ========================================================================
 * History:           修改历史记录列表，每条修改记录应包括修改日期、修改者及修改内容简述
 * 1. Date:           
      Author:         
      Modification:   
========================================================================*/
#include "RS485.h"  
#include "EEDFLASH.h" 
#include "BattInfoParaStructure.h"  
#include "Task_SOC.h"
#include "Task_FaultLevelJudge.h"
     
RS485  RS485_Receive;         
      
/*=======================================================================
 *函数名:      RS485_DataMemset(void)
 *功能:        初始化485数据
 *参数:        无       
 *返回：       无
 *说明：       
========================================================================*/
void RS485_DataMemset(void) 
{
    RS485_EnableDir = 1;
    RS485_Enable = 1; 
}

/*=======================================================================
 *函数名:      RS485_DataReceive
 *功能:        SCI数据接收
 *参数:        无       
 *返回：       无
 *说明：       
========================================================================*/
void RS485_DataReceice(void) 
{ 

 /*----------------------------------------双字节接收-----------------------------------*/

    RS485_Receive.TxData_couple.TX2.BMS_SOH = 0; //无  
    RS485_Receive.TxData_couple.TX2.RunningTime = (uint16)(EEprom_Data.BmsRunTime_min/60.0);           
    RS485_Receive.TxData_couple.TX2.BMS_Current = (uint16)(DataColletInfo.DataCollet_Current_Filter*10);                  
    RS485_Receive.TxData_couple.TX2.BMS_SOC = (uint16)(SOCInfo.SOC_ValueRead*100);                                                    
    RS485_Receive.TxData_couple.TX2.Pack_Hightemp = TempInfo.CellTemp_Max-20;               
    RS485_Receive.TxData_couple.TX2.Pack_Lowtemp = TempInfo.CellTemp_Min-20;                
    RS485_Receive.TxData_couple.TX2.Pack_Volt = DataColletInfo.SysVolt_Total/10;                 
    RS485_Receive.TxData_couple.TX2.Single_Maxvolt = (VoltInfo.CellVolt_Max +5)/10;      
    RS485_Receive.TxData_couple.TX2.Single_Lowvolt = (VoltInfo.CellVolt_Min +5)/10;        
    RS485_Receive.TxData_couple.TX2.iso_resistance = VoltInfo.CellVolt_MaxNode+1;    //最高单体节点号   
    RS485_Receive.TxData_couple.TX2.Lease_Time = VoltInfo.CellVolt_MinNode+1;        //最低单体节点号
                          
/*------------------------------------------单字节接收---------------------------------*/       
   
      
    RS485_Receive.TxData_single.TX1.Alam_Hightemp = (uint8)Fault_DisCharge.Level_Temp_High;
    RS485_Receive.TxData_single.TX1.Alam_Lowtemp = (uint8)Fault_DisCharge.Level_Temp_Low;     
    
    //RS485_Receive.TxData_single.TX1.Alam_SOC = (uint8)DiscFlt.LowSOC;  
    RS485_Receive.TxData_single.TX1.Alam_Pack_highvolt =  (uint8)Fault_Charge.Level_Volt_Sys_High;                        
    RS485_Receive.TxData_single.TX1.Alam_Pack_Lowvolt = (uint8)Fault_DisCharge.Level_Volt_Sys_Low;
    RS485_Receive.TxData_single.TX1.Alam_single_highvolt =  (uint8)Fault_Charge.Level_Volt_Cell_High;                      
    RS485_Receive.TxData_single.TX1.Alam_single_lowvolt = (uint8)Fault_DisCharge.Level_Volt_Cell_Low;
    RS485_Receive.TxData_single.TX1.Alam_Charge_highcurr = (uint8)Fault_Charge.Level_Current_Charge_High;                       
    RS485_Receive.TxData_single.TX1.Alam_Discharge_highcurr = (uint8)Fault_DisCharge.Level_Current_DisCharge_High;
    RS485_Receive.TxData_single.TX1.Alam_communi_fail = 0; //导线开路
    RS485_Receive.TxData_single.TX1.Alam_tenancy = 0;  //未用  
    RS485_Receive.TxData_single.TX1.Acc_offline = 0;   //未用
    
/*--------------------------------------------------------------------------------------*/
    //Task_Flag_Counter.Counter_485Receive++; 
 }    

/*=======================================================================
 *函数名:      Rs485_Transfer
 *功能:        SACI发送处理
 *参数:        rs:要发送结构体的字节指针       
 *返回：       无
 *说明：       
========================================================================*/
void Rs485_Transfer() 
{          
   uint8 i;
   PTT_PTT1 = ~PTT_PTT1;
   RS485_DataReceice();
   delay(10);
   SCI2_send_Nbyte(Array_couple, RS485_Receive.TxData_couple.SCI_Content2);  
   SCI2_send_Nbyte(Array_single, RS485_Receive.TxData_single.SCI_Content1); 

   //Task_Flag_Counter.Counter_485Send++;   
   return;
}