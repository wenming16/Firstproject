/*=======================================================================
 *Subsystem:   裸机
 *File:        UpMonitorToBMS.C
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
#include  "Task_SOC.h"
#include  "MC9S12XEP100.h"
#include  "derivative.h"




MonitorBMS_SOC_T    MonitorBMS_SOC;
MonitorBMS_Start_T  MonitorBMS_Start;

/*=======================================================================
 *函数名:      UpMonitor_to_Bms(pCANFRAME data)
 *功能:        从0CA0开始存储；
 *参数:        无       
 *返回：       无
 *说明：       上位机发送信息给BMS，进行标定作用,用一个switch case的方式
               选择标定的数据。
========================================================================*/
void UpMonitor_to_Bms(pCANFRAME data)
{
  uint8   Return_Value,Val8;
  pCANFRAME Monitor_Biao;
  uint16  Val;
  uint32  Val32;
  uint8  Send_ID;
     
  switch(data -> m_ID) 
  { 
      //0x1910F4C0   电池电压保护阈值
      case 0x1910F4C0:
      Send_ID = 0x10;
      switch(data -> m_data[0])
      {
        //单体过压
        case 00 :
          Val = (((uint16)(data -> m_data[2]))<<8) + (data -> m_data[1]);   
          if(Val!=0xFFFF) 
          {
             *(uint16*)0x0CA0=(uint16)(Val);  
          }
          Val = (((uint16)(data -> m_data[4]))<<8) + (data -> m_data[3]);  
          if(Val!=0xFFFF) 
          {
             *(uint16*)0x0CA2=(uint16)(Val);  
          }
          //Val = (((uint16)(data -> m_data[6]))<<8) + (data -> m_data[5]); 
          //if(Val!=0xFFFF) 
          //{
          //   *(uint16*)0x0CA4=(uint16)(Val);  
          //}
          BMSMonitor_Volt.Volt_Cell_High1 =(*(uint16*)(0x0CA0));
          BMSMonitor_Volt.Volt_Cell_High2 =(*(uint16*)(0x0CA2));
          //Monitor_BMS_Volt.Single_Over3_Volt =(*(uint16*)(0x0CA4));        
        break;
        //单体欠压   
        case 01:
          Val = (((uint16)(data -> m_data[2]))<<8) + (data -> m_data[1]);   
          if(Val!=0xFFFF) 
          {
             *(uint16*)0x0CA8=(uint16)(Val);  
          }
          Val = (((uint16)(data -> m_data[4]))<<8) + (data -> m_data[3]);  
          if(Val!=0xFFFF) 
          {
             *(uint16*)0x0CAA=(uint16)(Val);  
          }
          /*Val = (((uint16)(data -> m_data[6]))<<8) + (data -> m_data[5]); 
          if(Val!=0xFFFF) 
          {
             *(uint16*)0x0CAC=(uint16)(Val);  
          } */
          BMSMonitor_Volt.Volt_Cell_Low1 =(*(uint16*)(0x0CA8));
          BMSMonitor_Volt.Volt_Cell_Low2 =(*(uint16*)(0x0CAA));
          //Monitor_BMS_Volt.Single_Under3_Volt =(*(uint16*)(0x0CAC));                               
        break;
        //系统过压  
        case 02:
          Val = (((uint16)(data -> m_data[2]))<<8) + (data -> m_data[1]);   
          if(Val!=0xFFFF) 
          {
             *(uint16*)0x0CB0=(uint16)(Val);  
          }
          Val = (((uint16)(data -> m_data[4]))<<8) + (data -> m_data[3]);   
          if(Val!=0xFFFF) 
          {
             *(uint16*)0x0CB2=(uint16)(Val);  
          }
          /*Val = (((uint16)(data -> m_data[6]))<<8) + (data -> m_data[5]);   
          if(Val!=0xFFFF) 
          {
             *(uint16*)0x0CB4=(uint16)(Val);  
          }*/
          BMSMonitor_Volt.Volt_Sys_High1 =(*(uint16*)(0x0CB0));
          BMSMonitor_Volt.Volt_Sys_High2 =(*(uint16*)(0x0CB2));
          //Monitor_BMS_Volt.Pack_Over3_Volt =(*(uint16*)(0x0CB4));
        break;
        //系统欠压
        case 03:
          Val = (((uint16)(data -> m_data[2]))<<8) + (data -> m_data[1]);   
          if(Val!=0xFFFF) 
          {
             *(uint16*)0x0CB8=(uint16)(Val);  
          }
          Val = (((uint16)(data -> m_data[4]))<<8) + (data -> m_data[3]);  
          if(Val!=0xFFFF) 
          {
             *(uint16*)0x0CBA=(uint16)(Val);  
          }
          /*Val = (((uint16)(data -> m_data[6]))<<8) + (data -> m_data[5]);  
          if(Val!=0xFFFF) 
          {
             *(uint16*)0x0CBC=(uint16)(Val);  
          } */
          BMSMonitor_Volt.Volt_Sys_Low1 =(*(uint16*)(0x0CB8));
          BMSMonitor_Volt.Volt_Sys_Low2 =(*(uint16*)(0x0CBA));
          //Monitor_BMS_Volt.Pack_Under3_Volt =(*(uint16*)(0x0CBC));
        break;
        //单体压差
        case 04:
          Val = (((uint16)(data -> m_data[2]))<<8) + (data -> m_data[1]);   
          if(Val!=0xFFFF) 
          {
             *(uint16*)0x0CC0=(uint16)(Val);  
          }
          /*Val = (((uint16)(data -> m_data[4]))<<8) + (data -> m_data[3]);    
          if(Val!=0xFFFF) 
          {
             *(uint16*)0x0CC2=(uint16)(Val);  
          }
          Val = (((uint16)(data -> m_data[6]))<<8) + (data -> m_data[5]); 
          if(Val!=0xFFFF) 
          {
             *(uint16*)0x0CC4=(uint16)(Val);  
          } */
          BMSMonitor_Volt.Volt_Cell_Diff1 =(*(uint16*)(0x0CC0));
          //Monitor_BMS_Volt.Single_Diff2_Volt =(*(uint16*)(0x0CC2));
          //Monitor_BMS_Volt.Single_Diff3_Volt =(*(uint16*)(0x0CC4));
        break;
        //系统压差
       /* case 05:
          Val = (((uint16)(data -> m_data[2]))<<8) + (data -> m_data[1]);   
          if(Val!=0xFFFF) 
          {
             *(uint16*)0x0CC8=(uint16)(Val);  
          }
          Val = (((uint16)(data -> m_data[4]))<<8) + (data -> m_data[3]);    
          if(Val!=0xFFFF) 
          {
             *(uint16*)0x0CCA=(uint16)(Val);  
          }
          Val = (((uint16)(data -> m_data[6]))<<8) + (data -> m_data[5]); 
          if(Val!=0xFFFF) 
          {
             *(uint16*)0x0CCC=(uint16)(Val);  
          }
          Monitor_BMS_Volt.System_Diff1_Volt =(*(uint16*)(0x0CC8));
          Monitor_BMS_Volt.System_Diff2_Volt =(*(uint16*)(0x0CCA));
          Monitor_BMS_Volt.System_Diff3_Volt =(*(uint16*)(0x0CCC));
        break; */ 
      }
      break;
      
      // 0x1911F4C0    电池温度保护阈值
      case 0x1911F4C0:
      Send_ID = 0x11;
      switch(data -> m_data[0])
      {
        //充电电池温度
        case 00 :
          Val8 = data -> m_data[1];   
          if(Val8!=0xFF) 
          {
             *(uint8*)0x0CD0=(uint8)(Val8);  
          }
          Val8 = data -> m_data[2];   
          if(Val8!=0xFF) 
          {
             *(uint8*)0x0CD1=(uint8)(Val8);  
          }
          /*Val8 = data -> m_data[3];   
          if(Val8!=0xFF) 
          {
             *(uint8*)0x0CD2=(uint8)(Val8);  
          }*/
          Val8 = data -> m_data[4];   
          if(Val8!=0xFF) 
          {
             *(uint8*)0x0CD3=(uint8)(Val8);  
          }
          Val8 = data -> m_data[5];   
          if(Val8!=0xFF) 
          {
             *(uint8*)0x0CD4=(uint8)(Val8);  
          }
          /*Val8 = data -> m_data[6];   
          if(Val8!=0xFF) 
          {
             *(uint8*)0x0CD5=(uint8)(Val8);  
          } */
          BMSMonitor_Temp.Temp_Charge_High1 =(*(uint8*)(0x0CD0));
          BMSMonitor_Temp.Temp_Charge_High2 =(*(uint8*)(0x0CD1));
          //Monitor_BMS_Temp.Charge_Over3_Temp =(*(uint8*)(0x0CD2));
          BMSMonitor_Temp.Temp_Charge_Low1 =(*(uint8*)(0x0CD3));
          BMSMonitor_Temp.Temp_Charge_Low2 =(*(uint8*)(0x0CD4));
          //Monitor_BMS_Temp.Charge_Under3_Temp =(*(uint8*)(0x0CD5));        
        break;
        //放电电池温度   
        case 01:
          Val8 = data -> m_data[1];   
          if(Val8!=0xFF) 
          {
             *(uint8*)0x0CD8=(uint8)(Val8);  
          }
          Val8 = data -> m_data[2];   
          if(Val8!=0xFF) 
          {
             *(uint8*)0x0CD9=(uint8)(Val8);  
          }
         /* Val8 = data -> m_data[3];   
          if(Val8!=0xFF) 
          {
             *(uint8*)0x0CDA=(uint8)(Val8);  
          } */
          Val8 = data -> m_data[4];   
          if(Val8!=0xFF) 
          {
             *(uint8*)0x0CDB=(uint8)(Val8);  
          }
          Val8 = data -> m_data[5];   
          if(Val8!=0xFF) 
          {
             *(uint8*)0x0CDC=(uint8)(Val8);  
          }
          /*Val8 = data -> m_data[6];   
          if(Val8!=0xFF) 
          {
             *(uint8*)0x0CDD=(uint8)(Val8);  
          } */
          BMSMonitor_Temp.Temp_DisCharge_High1 =(*(uint8*)(0x0CD8));
          BMSMonitor_Temp.Temp_DisCharge_High2 =(*(uint8*)(0x0CD9));
          //Monitor_BMS_Temp.DisCharge_Over3_Temp =(*(uint8*)(0x0CDA));
          BMSMonitor_Temp.Temp_DisCharge_Low1 =(*(uint8*)(0x0CDB));
          BMSMonitor_Temp.Temp_DisCharge_Low2 =(*(uint8*)(0x0CDC));
          //Monitor_BMS_Temp.DisCharge_Under3_Temp =(*(uint8*)(0x0CDD));                               
        break;
        //充/放电电池温差  
        case 02:
          Val8 = data -> m_data[1];   
          if(Val8!=0xFF) 
          {
             *(uint8*)0x0CE0=(uint8)(Val8);  
          }
          /*Val8 = data -> m_data[2];   
          if(Val8!=0xFF) 
          {
             *(uint8*)0x0CE1=(uint8)(Val8);  
          }
          Val8 = data -> m_data[3];   
          if(Val8!=0xFF) 
          {
             *(uint8*)0x0CE2=(uint8)(Val8);  
          }*/
          Val8 = data -> m_data[4];   
          if(Val8!=0xFF) 
          {
             *(uint8*)0x0CE3=(uint8)(Val8);  
          }
          /*Val8 = data -> m_data[5];   
          if(Val8!=0xFF) 
          {
             *(uint8*)0x0CE4=(uint8)(Val8);  
          }
          Val8 = data -> m_data[6];   
          if(Val8!=0xFF) 
          {
             *(uint8*)0x0CE5=(uint8)(Val8);  
          }*/
          BMSMonitor_Temp.Temp_Charge_Diff1 =(*(uint8*)(0x0CE0));
          //Monitor_BMS_Temp.Charge_Diff2_Temp =(*(uint8*)(0x0CE1));
          //Monitor_BMS_Temp.Charge_Diff3_Temp =(*(uint8*)(0x0CE2));
          BMSMonitor_Temp.Temp_DisCharge_Diff1 =(*(uint8*)(0x0CE3));
          //Monitor_BMS_Temp.DisCharge_Diff2_Temp =(*(uint8*)(0x0CE4));
          //Monitor_BMS_Temp.DisCharge_Diff3_Temp =(*(uint8*)(0x0CE5));
        break;
        //充电枪&BMS过温
       /* case 03:
          Val8 = data -> m_data[1];   
          if(Val8!=0xFF) 
          {
             *(uint8*)0x0CE8=(uint8)(Val8);  
          }
          Val8 = data -> m_data[2];   
          if(Val8!=0xFF) 
          {
             *(uint8*)0x0CE9=(uint8)(Val8);  
          }
          Val8 = data -> m_data[3];   
          if(Val8!=0xFF) 
          {
             *(uint8*)0x0CEA=(uint8)(Val8);  
          }
          Val8 = data -> m_data[4];   
          if(Val8!=0xFF) 
          {
             *(uint8*)0x0CEB=(uint8)(Val8);  
          }
          Val8 = data -> m_data[5];   
          if(Val8!=0xFF) 
          {
             *(uint8*)0x0CEC=(uint8)(Val8);  
          }
          Val8 = data -> m_data[6];   
          if(Val8!=0xFF) 
          {
             *(uint8*)0x0CED=(uint8)(Val8);  
          }
          Monitor_BMS_Temp.ChargeGun_Over1_Temp =(*(uint8*)(0x0CE8));
          Monitor_BMS_Temp.ChargeGun_Over2_Temp =(*(uint8*)(0x0CE9));
          Monitor_BMS_Temp.ChargeGun_Over3_Temp =(*(uint8*)(0x0CEA));
          Monitor_BMS_Temp.Chip_Over1_Temp =(*(uint8*)(0x0CEB));
          Monitor_BMS_Temp.Chip_Over2_Temp =(*(uint8*)(0x0CEC));
          Monitor_BMS_Temp.Chip_Over3_Temp =(*(uint8*)(0x0CED));
        break; */
      }
      break;
      
      // 0x1912F4C0      电池电流阈值
      case 0x1912F4C0:
      Send_ID = 0x12;
      switch(data -> m_data[0])
      {
        //系统放电过流
        case 00 :
          Val = (((uint16)(data -> m_data[2]))<<8) + (data -> m_data[1]);  
          if(Val!=0xFFFF) 
          {
             *(uint16*)0x0CF0=(uint16)(Val);  
          }
          Val = (((uint16)(data -> m_data[4]))<<8) + (data -> m_data[3]);    
          if(Val!=0xFFFF) 
          {
             *(uint16*)0x0CF2=(uint16)(Val);  
          }
          /*Val = (((uint16)(data -> m_data[6]))<<8) + (data -> m_data[5]);  
          if(Val!=0xFFFF) 
          {
             *(uint16*)0x0CF4=(uint16)(Val);  
          }  */
          BMSMonitor_Curr.Current_DisCharge_High1 =(*(uint16*)(0x0CF0));
          BMSMonitor_Curr.Current_DisCharge_High2 =(*(uint16*)(0x0CF2));
          //Monitor_BMS_Current.DisCharge_Over3_Current =(*(uint16*)(0x0CF4));        
        break;
        //系统充电过流  
        case 01:
          Val = (((uint16)(data -> m_data[2]))<<8) + (data -> m_data[1]); 
          if(Val!=0xFFFF) 
          {
             *(uint16*)0x0CF8=(uint16)(Val);  
          }
          Val = (((uint16)(data -> m_data[4]))<<8) + (data -> m_data[3]);   
          if(Val!=0xFFFF) 
          {
             *(uint16*)0x0CFA=(uint16)(Val);  
          }
          /*Val = (((uint16)(data -> m_data[6]))<<8) + (data -> m_data[5]); 
          if(Val!=0xFFFF) 
          {
             *(uint16*)0x0CFC=(uint16)(Val);  
          } */
          BMSMonitor_Curr.Current_Charge_High1 =(*(uint16*)(0x0CF8));
          BMSMonitor_Curr.Current_Charge_High2 =(*(uint16*)(0x0CFA));
          //BMSMonitor_Curr.Charge_Over3_Current =(*(uint16*)(0x0CFC));                               
        break;
        //SOC阈值
       /* case 02:
          Val8 = data -> m_data[1];   
          if(Val8!=0xFF) 
          {
             *(uint8*)0x0D00=(uint8)(Val8);  
          }
          Val8 = data -> m_data[2];   
          if(Val8!=0xFF) 
          {
             *(uint8*)0x0D01=(uint8)(Val8);  
          }
          Val8 = data -> m_data[3];   
          if(Val8!=0xFF) 
          {
             *(uint8*)0x0D02=(uint8)(Val8);  
          }
          Val8 = data -> m_data[4];   
          if(Val8!=0xFF) 
          {
             *(uint8*)0x0D03=(uint8)(Val8);  
          }
          Val8 = data -> m_data[5];   
          if(Val8!=0xFF) 
          {
             *(uint8*)0x0D04=(uint8)(Val8);  
          }
          Val8 = data -> m_data[6];   
          if(Val8!=0xFF) 
          {
             *(uint8*)0x0D05=(uint8)(Val8);  
          }
          Monitor_BMS_Current.SOC_Low1 =(*(uint8*)(0x0D00));
          Monitor_BMS_Current.SOC_Low2 =(*(uint8*)(0x0D01));
          Monitor_BMS_Current.SOC_Low3 =(*(uint8*)(0x0D02));
          Monitor_BMS_Current.SOC_High1 =(*(uint8*)(0x0D03));
          Monitor_BMS_Current.SOC_High2 =(*(uint8*)(0x0D04));
          Monitor_BMS_Current.SOC_High3 =(*(uint8*)(0x0D05));
        break; */
      }
      break;
      
      // 0x1913F4C0      绝缘故障阈值
      case 0x1913F4C0:
      Send_ID = 0x13;
      Val = (((uint16)(data -> m_data[2]))<<8) + (data -> m_data[1]);  
      if(Val!=0xFFFF) 
      {
         *(uint16*)0x0D08=(uint16)(Val);  
      }
      Val = (((uint16)(data -> m_data[4]))<<8) + (data -> m_data[3]);  
      if(Val!=0xFFFF) 
      {
         *(uint16*)0x0D0A=(uint16)(Val);  
      }
      Val = (((uint16)(data -> m_data[6]))<<8) + (data -> m_data[5]); 
      if(Val!=0xFFFF) 
      {
         *(uint16*)0x0D0C=(uint16)(Val);  
      }
      BMSMonitor_Insu.Insulation_Resis1 =(*(uint16*)(0x0D08));
      BMSMonitor_Insu.Insulation_Resis2 =(*(uint16*)(0x0D0A));
      BMSMonitor_Insu.Insulation_Resis3 =(*(uint16*)(0x0D0C));        
      break;
      
      // 0x1915F4C0      特殊类的信息
      case 0x1915F4C0:
      Send_ID = 0x15;
      switch(data -> m_data[0])
      {
      
        case 00 :
          Val = (((uint16)(data -> m_data[2]))<<8) + (data -> m_data[1]);   
          if(Val!=0xFFFF) 
          {
             *(uint16*)0x0D10=(uint16)(Val);  
          }
          Val = (((uint16)(data -> m_data[4]))<<8) + (data -> m_data[3]);  
          if(Val!=0xFFFF) 
          {
             *(uint16*)0x0D12=(uint16)(Val);  
          }
          Val = (((uint16)(data -> m_data[6]))<<8) + (data -> m_data[5]); 
          if(Val!=0xFFFF) 
          {
             *(uint16*)0x0D14=(uint16)(Val);  
          }
          MonitorBMS_SOC.SOC_Init =(*(uint16*)(0x0D10));
          SOCInfo.SOC_Init =  MonitorBMS_SOC.SOC_Init;
          MonitorBMS_SOC.Ref_Volt =(*(uint16*)(0x0D12));
          MonitorBMS_SOC.Ref_Current =(*(uint16*)(0x0D14));        
        break;
       
        case 01:
          Val = (((uint16)(data -> m_data[2]))<<8) + (data -> m_data[1]);   
          if(Val!=0xFFFF) 
          {
             *(uint16*)0x0D18=(uint16)(Val);  
          }
          Val32 = (((uint32)(data -> m_data[6]))<<24) + (((uint32)(data -> m_data[5]))<<16) + (((uint16)(data -> m_data[4]))<<8) + (data -> m_data[3]);  
          if(Val32!=0xFFFF) 
          {
             *(uint32*)0x0D1A=(uint32)(Val32);  
          }
          MonitorBMS_SOC.Volt_Resis =(*(uint16*)(0x0D18));
          MonitorBMS_SOC.Pack_Total_Capacity =(*(uint16*)(0x0D1A));
        break;
        
        /*case 02:
          Val = (((uint16)(data -> m_data[2]))<<8) + (data -> m_data[1]);  
          if(Val!=0xFFFF) 
          {
             *(uint16*)0x0D20=(uint16)(Val);  
          }
          MonitorBMS_SOC.BMS_Running_Time =(*(uint16*)(0x0D20)) * 60;
          Val8 = data -> m_data[3];                       //下标主正继电器
          if(Val8!=0xFFFF)
          {
            DiscFlt.RelayFault_PositiveHold = Val8;  
          }
          Val8 = data -> m_data[4];                       //下标主负继电器
          if(Val8!=0xFFFF)
          {
            DiscFlt.RelayFault_PositiveBreak = Val8;  
          }
          Val8 = data -> m_data[5];                       //下标预充继电器
          if(Val8!=0xFFFF)
          {
            DiscFlt.RelayFault_PreChag = Val8;  
          }
          Val8 = data -> m_data[6];                       //下标快充继电器
          if(Val8!=0xFFFF)
          {
            DiscFlt.RelayFault_Chag = Val8;  
          }
        break;*/
      }
      break;
     
     
     
      // 0x19FFF4C0      发送启动帧
      case 0x19FFF4C0:
      Send_ID = 0xFF;
      MonitorBMS_Start.Communication = data -> m_data[0];
      MonitorBMS_Start.CellMessageSend_judge = 0x01; 
      break;
  }
  
  if(Send_ID == 0xFE)  //版本号请求之后才发送
  {
      Monitor_Biao->m_ID = 0x18FEC0F4;       // 版本号；
    	Monitor_Biao->m_IDE = 1;
    	Monitor_Biao->m_RTR = 0;
    	Monitor_Biao->m_dataLen = 8;
    	Monitor_Biao->m_priority = 6;
    	Monitor_Biao->m_data[0] =  0;
    	Monitor_Biao->m_data[1] =  'm';
    	Monitor_Biao->m_data[2] =  1;
    	Monitor_Biao->m_data[3] =  2;
    	Monitor_Biao->m_data[4] =  3;
    	Monitor_Biao->m_data[5] = 4;
    	Monitor_Biao->m_data[6] =  5;
    	Monitor_Biao->m_data[7] =   5;
      Return_Value= BMS_CAN3SendMsg_Monitor(Monitor_Biao);         
  } 
  //标定回复帧
  else if(Send_ID != 0)   //成功
  {
      Monitor_Biao->m_ID = 0x18FFC0F4;      
      Monitor_Biao->m_IDE = 1;
      Monitor_Biao->m_RTR = 0;
      Monitor_Biao->m_dataLen = 8;
      Monitor_Biao->m_priority = 6;
      Monitor_Biao->m_data[0] = Send_ID;
      Monitor_Biao->m_data[1] = 0xFF;
    	Monitor_Biao->m_data[2] = 0xFF;
    	Monitor_Biao->m_data[3] = 0xFF;
    	Monitor_Biao->m_data[4] = 0xFF;
    	Monitor_Biao->m_data[5] = 0xFF;
    	Monitor_Biao->m_data[6] = 0xFF;
    	Monitor_Biao->m_data[7] = 0xFF;
      Return_Value= BMS_CAN3SendMsg_Monitor(Monitor_Biao);
  }
  else    //失败
  {
      Monitor_Biao->m_ID = 0x18FFC0F4;      
      Monitor_Biao->m_IDE = 1;
      Monitor_Biao->m_RTR = 0;
      Monitor_Biao->m_dataLen = 8;
      Monitor_Biao->m_priority = 6;
      Monitor_Biao->m_data[0] = 0xEE;
      Monitor_Biao->m_data[1] = 0xFF;
    	Monitor_Biao->m_data[2] = 0xFF;
    	Monitor_Biao->m_data[3] = 0xFF;
    	Monitor_Biao->m_data[4] = 0xFF;
    	Monitor_Biao->m_data[5] = 0xFF;
    	Monitor_Biao->m_data[6] = 0xFF;
    	Monitor_Biao->m_data[7] = 0xFF;
      Return_Value= BMS_CAN3SendMsg_Monitor(Monitor_Biao);
  } 
     
  //Task_Flag_Counter.UpMonitor_to_BMS++;
}         

