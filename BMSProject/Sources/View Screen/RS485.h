/*=======================================================================
 *Subsystem:   裸机
 *File:        RS485.h
 *Author:      WenMing
 *Description: 通信：SCI2.
               接口：PJ0（RXD）；PJ1（TXD）
               波特率：
 ========================================================================
 * History:        // 修改历史记录列表，每条修改记录应包括修改日期、修改者及修改内容简述
 * 1. Date:
      Author:
      Modification:
========================================================================*/


#ifndef RS485_H
#define RS485_H

#include  "TypeDefinition.h"
#include  "MC9S12XEP100.h"
#include  "derivative.h" 

/*构建头文件及宏定义*/
  
  #define Array_couple 22 //宏定义发送双字节数组个数为22
  #define Array_single 12  //宏定义发送单字节数组个数为11

  #define RS485_Enable PORTA_PA6
  #define RS485_EnableDir DDRA_DDRA6


  /*======显示屏显示内容双字节======*/

  
  typedef union
  { 
    uint8 SCI_Content2[Array_couple];
    struct
    {       
        uint16 RunningTime;           //运行时间
        uint16 BMS_Current;           //电池组电流
      	uint16 BMS_SOC;               //SOC
      	uint16 BMS_SOH;               //SOH
      	uint16 Pack_Hightemp;         //电池组最高温度
      	uint16 Pack_Lowtemp;          //电池组最低温度
      	uint16 Pack_Volt;             //电池组总压
      	uint16 Single_Maxvolt;        //单体最高电压
      	uint16 Single_Lowvolt;        //单体最低电压
      	uint16 iso_resistance;        //绝缘电阻阻值
      	uint16 Lease_Time;            //租赁时间 
    }TX2;
  }RS485_couple;                  //双字节SCI发送   
  
  
  /*======显示屏显示内容单字节======*/
  typedef union
  {
    uint8 SCI_Content1[Array_single];
    struct
    {
      	uint8	Alam_SOC;                //SOC告警
      	uint8	Alam_Hightemp;           //电阻最高温度告警
      	uint8	Alam_Lowtemp;            //电池最低温度告警
      	uint8	Alam_Pack_highvolt;      //电池组过压告警
      	uint8	Alam_Pack_Lowvolt;       //电池组欠压告警
      	uint8 Alam_single_highvolt;    //单体过压告警
      	uint8 Alam_single_lowvolt;     //单体欠压告警
      	uint8 Alam_Charge_highcurr;    //电池组充电过流告警
      	uint8 Alam_Discharge_highcurr; //电池组放电过流告警
      	uint8 Alam_communi_fail;       //充电通信故障
      	uint8 Alam_tenancy;            //租赁信息管理
      	uint8 Acc_offline;
    }TX1;     
  }RS485_single;                   //单字节SCI发送
             
  typedef struct
  {
      RS485_couple TxData_couple;
      RS485_single TxData_single;   
  }RS485;
  extern RS485  RS485_Receive; 
      
   void RS485_DataMemset(void);
   void RS485_DataReceice(void);
   void Rs485_Transfer(void);
   
  
 #endif 

