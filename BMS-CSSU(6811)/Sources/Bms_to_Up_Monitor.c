/*=======================================================================
 *Subsystem:   裸机
 *File:        Bms_to_Up_Monitor.C
 *Author:      ZWB
 *Description: 通信：
               接口：
               波特率：
/*========================================================================
 * History:    修改历史记录列表
 * 1. Date:    
      Author: 
      Modification: 
/*=======================================================================*/

#include "Includes.h"


/*=======================================================================
 *函数名:      Bms_to_Up_Monitor(void) 
 *功能:        BMS information to UpMonitor
 *参数:        无       
 *返回：       无
 *说明：       BMS发送信息给上位机；
========================================================================*/
void Bms_to_Up_Monitor(void) 
{
    uint8  i,j,Return_Value;
    uint16  BalanceVolt;
    CANFRAME  BMS_to_Upmonitor;
    uint8 batt,batt1; 
    
    /*
    for(i=0; i<25; i++)
    {
      VoltInfo.CellVolt[i] = 34500+i;
      VoltInfo.CellVolt_Total = VoltInfo.CellVolt_Total+VoltInfo.CellVolt[i];
    }
     VoltInfo.CellVolt_Max = 34524; 
     VoltInfo.CellVolt_Min = 34500;
     VoltInfo.CellVolt_MaxNode = 24;
     VoltInfo.CellVolt_MinNode =0;            
    
    for(i=0; i<5; i++)
    {
      TempInfo.CellTemp[i] = 22+i;
      TempInfo.CellTemp_tatoltemp = TempInfo.CellTemp_tatoltemp+TempInfo.CellTemp[i];
    }
     TempInfo.CellTemp_Max = 22;
     TempInfo.CellTemp_Min = 22;
     TempInfo.CellTemp_MaxNode = 4;
     TempInfo.CellTemp_MinNode = 0;
    */ 
    
    
    batt=(NUM1_Batper_true+NUM2_Batper_true+NUM3_Batper_true+NUM4_Batper_true+NUM5_Batper_true)/3;
    batt1=(NUM1_Batper_true+NUM2_Batper_true+NUM3_Batper_true+NUM4_Batper_true+NUM5_Batper_true)%3;
       
    BMS_to_Upmonitor.m_ID = BMS_Send_Information1+NUM_pack;       
  	BMS_to_Upmonitor.m_IDE = 1;
  	BMS_to_Upmonitor.m_RTR = 0;
  	BMS_to_Upmonitor.m_dataLen = 8;
  	BMS_to_Upmonitor.m_priority = 6;
    for(i = 0; i <batt ; i++) 
    {
      BMS_to_Upmonitor.m_data[0] = (uint8)(ToBMU_BalanceState.CSSUBalanceNode);             //均衡开启状态
      BMS_to_Upmonitor.m_data[1] = (uint8)(i);            //每个6804采集电压的编号  0-3  
      BMS_to_Upmonitor.m_data[2] = (uint8)VoltInfo.CellVolt[i*3];
      BMS_to_Upmonitor.m_data[3] = (VoltInfo.CellVolt[i*3]>>8)&0X00FF;
      BMS_to_Upmonitor.m_data[4] = (uint8)VoltInfo.CellVolt[i*3+1];
      BMS_to_Upmonitor.m_data[5] = (VoltInfo.CellVolt[i*3+1]>>8)&0X00FF;
      BMS_to_Upmonitor.m_data[6] = (uint8)VoltInfo.CellVolt[i*3+2];
      BMS_to_Upmonitor.m_data[7] = (VoltInfo.CellVolt[i*3+2]>>8)&0X00FF;
      Return_Value= MSCAN2SendMsg(&BMS_to_Upmonitor);
      delay_time(100);
    }  
    switch(batt1) 
    {
      case 1:
      BMS_to_Upmonitor.m_data[0] = (uint8)(i/NUM_IC);
      BMS_to_Upmonitor.m_data[1] = (uint8)(i);                             //每个6804采集电压的编号 
      BMS_to_Upmonitor.m_data[2] = (uint8)VoltInfo.CellVolt[i*3];
      BMS_to_Upmonitor.m_data[3] = (VoltInfo.CellVolt[i*3]>>8)&0X00FF;
      BMS_to_Upmonitor.m_data[4] = 0xFF;
      BMS_to_Upmonitor.m_data[5] = 0xFF;
      BMS_to_Upmonitor.m_data[6] = 0xFF;
      BMS_to_Upmonitor.m_data[7] = 0xFF;
       Return_Value= MSCAN2SendMsg(&BMS_to_Upmonitor);
      break;
      
      case 2:
      BMS_to_Upmonitor.m_data[0] = (uint8)(i/NUM_IC);
      BMS_to_Upmonitor.m_data[1] = (uint8)(i);                             //每个6804采集电压的编号 
      BMS_to_Upmonitor.m_data[2] = (uint8)VoltInfo.CellVolt[i*3];
      BMS_to_Upmonitor.m_data[3] = (VoltInfo.CellVolt[i*3]>>8)&0X00FF;
      BMS_to_Upmonitor.m_data[4] = (uint8)VoltInfo.CellVolt[i*3+1];
      BMS_to_Upmonitor.m_data[5] = (VoltInfo.CellVolt[i*3+1]>>8)&0X00FF;
      BMS_to_Upmonitor.m_data[6] = 0xFF;
      BMS_to_Upmonitor.m_data[7] = 0xFF;
       Return_Value= MSCAN2SendMsg(&BMS_to_Upmonitor);
      break;
      
      default:
      break; 
    }      
    
    BMS_to_Upmonitor.m_ID = BMS_ture_battery+NUM_pack;       
  	BMS_to_Upmonitor.m_IDE = 1;
  	BMS_to_Upmonitor.m_RTR = 0;
  	BMS_to_Upmonitor.m_dataLen = 8;
  	BMS_to_Upmonitor.m_data[0] = NUM1_Batper_true;
    BMS_to_Upmonitor.m_data[1] = NUM2_Batper_true;
    BMS_to_Upmonitor.m_data[2] = NUM3_Batper_true;
    BMS_to_Upmonitor.m_data[3] = 0xFF;
    BMS_to_Upmonitor.m_data[4] = 0xFF;
    BMS_to_Upmonitor.m_data[5] = 0xFF;
    BMS_to_Upmonitor.m_data[6] = 0xFF;  
    BMS_to_Upmonitor.m_data[7] = 0xFF;       
    Return_Value= MSCAN2SendMsg(&BMS_to_Upmonitor);
    
    BMS_to_Upmonitor.m_ID = BMS_Send_Information2+NUM_pack;       
  	BMS_to_Upmonitor.m_IDE = 1;
  	BMS_to_Upmonitor.m_RTR = 0;
  	BMS_to_Upmonitor.m_dataLen = 8;
  	BMS_to_Upmonitor.m_data[0] = (uint8)VoltInfo.CellVolt_Max;
    BMS_to_Upmonitor.m_data[1] = (VoltInfo.CellVolt_Max>>8)&0x00FF;
    BMS_to_Upmonitor.m_data[2] = VoltInfo.CellVolt_MaxNode;
    BMS_to_Upmonitor.m_data[3] = (uint8)VoltInfo.CellVolt_Min;
    BMS_to_Upmonitor.m_data[4] = (VoltInfo.CellVolt_Min>>8)&0x00FF;
    BMS_to_Upmonitor.m_data[5] = VoltInfo.CellVolt_MinNode;
    BMS_to_Upmonitor.m_data[6] = 0xFF;  
    BMS_to_Upmonitor.m_data[7] = 0xFF;       
    Return_Value= MSCAN2SendMsg(&BMS_to_Upmonitor); 
   
    BMS_to_Upmonitor.m_ID = BMS_Send_Information3+NUM_pack;      
  	BMS_to_Upmonitor.m_IDE = 1;
  	BMS_to_Upmonitor.m_RTR = 0;
  	BMS_to_Upmonitor.m_dataLen = 8;
  	BMS_to_Upmonitor.m_priority = 6;
  	for( i=0; i< ((NUM_Tem+6) / 7) ;i++)         //ly 按照协议修改
    {
      BMS_to_Upmonitor.m_data[0] = i;
      //memset( &pMsgTran.data,0xFF,8 );       /* 8个data设置为40 */
      if( i < 1 )                      //对于扩展可修改此处
      {
          for(j=1; j < NUM_IC*2; j++) 
          {
              BMS_to_Upmonitor.m_data[j] = TempInfo.CellTemp[j-1+i*7] + 40;
          }   
      } 
      else 
      {
           for( j = 1 ; j <= (NUM_Tem% 7);j++ ) 
           {
              BMS_to_Upmonitor.m_data[j] = TempInfo.CellTemp[j-1+i*7] + 40; 
           }
      }
       Return_Value= MSCAN2SendMsg(&BMS_to_Upmonitor); 
    }
  
    BMS_to_Upmonitor.m_ID = BMS_Send_Information4+NUM_pack;       // 电池基本信息1;
  	BMS_to_Upmonitor.m_IDE = 1;
  	BMS_to_Upmonitor.m_RTR = 0;
  	BMS_to_Upmonitor.m_dataLen = 8;
  	BMS_to_Upmonitor.m_priority = 6;
    BMS_to_Upmonitor.m_data[0] = TempInfo.CellTemp_Max + 40;
  	BMS_to_Upmonitor.m_data[1] = TempInfo.CellTemp_MaxNode;   
  	BMS_to_Upmonitor.m_data[2] = TempInfo.CellTemp_Min + 40;
  	BMS_to_Upmonitor.m_data[3] = TempInfo.CellTemp_MinNode;   
  	BMS_to_Upmonitor.m_data[4] = TempInfo.CellTemp_tatoltemp;
  	BMS_to_Upmonitor.m_data[5] = TempInfo.CellTemp_tatoltemp>>8;   
  	BMS_to_Upmonitor.m_data[6] = 0xFF;
  	BMS_to_Upmonitor.m_data[7] = 0xFF;       	 
    Return_Value= MSCAN2SendMsg(&BMS_to_Upmonitor); 
   
   /* BMS_to_Upmonitor.m_ID = BMS_Send_Single_Volt;       
  	BMS_to_Upmonitor.m_IDE = 1;
  	BMS_to_Upmonitor.m_RTR = 0;
  	BMS_to_Upmonitor.m_dataLen = 8;
  	BMS_to_Upmonitor.m_priority = 6;
	  for(j = 0; j < Monitor_Stand.LTC_Chip_Num; j++) 
	  {
    	  for(i = 0;i<4;i++) 
    	  {  
          	BMS_to_Upmonitor.m_data[0] = j+1;
          	BMS_to_Upmonitor.m_data[1] = i;
          	BMS_to_Upmonitor.m_data[2] = Volt_Data_T.Signal_Vol[j][i*3];   
          	BMS_to_Upmonitor.m_data[3] = Volt_Data_T.Signal_Vol[j][i*3]>>8;
          	BMS_to_Upmonitor.m_data[4] = Volt_Data_T.Signal_Vol[j][i*3+1];
          	BMS_to_Upmonitor.m_data[5] = Volt_Data_T.Signal_Vol[j][i*3+1]>>8;
          	BMS_to_Upmonitor.m_data[6] = Volt_Data_T.Signal_Vol[j][i*3+2];
          	BMS_to_Upmonitor.m_data[7] = Volt_Data_T.Signal_Vol[j][i*3+2]>>8;       
            Return_Value= MSCAN1SendMsg(&BMS_to_Upmonitor); 
    	  } 
	  } */
  
   /* BMS_to_Upmonitor.m_ID = BMS_Send_Pack_Temp;             //芯片温度、导线开路、总压
  	BMS_to_Upmonitor.m_IDE = 1;
  	BMS_to_Upmonitor.m_RTR = 0;
  	BMS_to_Upmonitor.m_dataLen = 8;
  	BMS_to_Upmonitor.m_priority = 6;
    for(i =0;i<Monitor_Stand.LTC_Chip_Num;i++)
    {
        BMS_to_Upmonitor.m_data[0] = i+1;
      	BMS_to_Upmonitor.m_data[1] = 0;	
      	BMS_to_Upmonitor.m_data[2] = Temp_Data_T.Pack_Temper[i][0]+40;   
      	BMS_to_Upmonitor.m_data[3] = Temp_Data_T.Pack_Temper[i][1]+40;
      	BMS_to_Upmonitor.m_data[4] = Temp_Data_T.Pack_Temper[i][2]+40;
      	BMS_to_Upmonitor.m_data[5] = Temp_Data_T.Pack_Temper[i][3]+40;
      	BMS_to_Upmonitor.m_data[6] = 40;
      	BMS_to_Upmonitor.m_data[7] = 40;       
        Return_Value= MSCAN2SendMsg(&BMS_to_Upmonitor); 
    } */

    BMS_to_Upmonitor.m_ID = BMS_Send_Information5+NUM_pack;             //导线开路
  	BMS_to_Upmonitor.m_IDE = 1;
  	BMS_to_Upmonitor.m_RTR = 0;
  	BMS_to_Upmonitor.m_dataLen = 8;
  	BMS_to_Upmonitor.m_priority = 6;
    BMS_to_Upmonitor.m_data[0] = TempInfo.CellTemp_over_ic;
  	BMS_to_Upmonitor.m_data[1] = VoltInfo.Openwire_error;	
  	BMS_to_Upmonitor.m_data[2] = VoltInfo.CellVolt_Total;   
  	BMS_to_Upmonitor.m_data[3] = VoltInfo.CellVolt_Total>>8;
  	BMS_to_Upmonitor.m_data[4] = VoltInfo.CellVolt_Total>>16;
  	BMS_to_Upmonitor.m_data[5] = IsoDetect.insulation_TotalVolt;
  	BMS_to_Upmonitor.m_data[6] = IsoDetect.insulation_TotalVolt>>8;
  	BMS_to_Upmonitor.m_data[7] = IsoDetect.insulation_TotalVolt>>16;       
    Return_Value= MSCAN2SendMsg(&BMS_to_Upmonitor); 
     
    BMS_to_Upmonitor.m_ID = BMS_Send_Information6+NUM_pack;             //导线开路
  	BMS_to_Upmonitor.m_IDE = 1;
  	BMS_to_Upmonitor.m_RTR = 0;
  	BMS_to_Upmonitor.m_dataLen = 6;
  	BMS_to_Upmonitor.m_priority = 6;
  	for(i = 0; i < NUM_IC ; i++)
  	{
      BMS_to_Upmonitor.m_data[i*2] = Openwire_flag[i];                   // 导线开路
    	BMS_to_Upmonitor.m_data[i*2 + 1] = (uint8)((Openwire_flag[i]>>8)&0x00FF);	       
  	}
  	Return_Value= MSCAN2SendMsg(&BMS_to_Upmonitor);
 
    Task_Flag_Counter.BMS_to_UpMonitor++;
      
}

