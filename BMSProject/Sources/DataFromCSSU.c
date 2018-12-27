/*=======================================================================
 *Subsystem:   裸机
 *File:        DataFromCSSU.c
 *Author:      WenYuhao
 *Description: 通信：
               接口：CAN3
               波特率：500kb
 ========================================================================
 * History:    修改历史记录列表
 * 1. Date:
      Author:
      Modification:
========================================================================*/
  
  #include"TypeDefinition.h"
  #include"Task_DataProcess.h"
  #include "CAN.h"
  #include "BattInfoParaStructure.h"
  #include "BattInfoConfig.h"
  
  Module_VoltInfo_T   Module_VoltInfo[SYS_NUMBER_MODULE];
  Module_TempInfo_T   Module_TempInfo[SYS_NUMBER_MODULE];
  Module_StateInfo_T  Module_StateInfo[SYS_NUMBER_MODULE];
/*=======================================================================
 *函数名:      DataFromCSSU(pCANFRAME data)  
 *功能:        接收内网数据
 *参数:        ID：报文ID号       
 *返回：       无
 *说明：       接收包括子板，绝缘检测板数据
========================================================================*/
void DataFromCSSU(pCANFRAME data)
{
  uint8 i=0,j=0;  
  switch(data->m_ID) 
  { 
    //电池模组单体电压信息
    case 0x18FF9700:                                      //子板采集的单体电压信息，从第25节开始存储
      j = data->m_data[1];                                //6804采集电压编号
      for(i=0; i<3&&(3*j+i<(SYS_SERIES-25)); i++) 
      {
         VoltInfo.CellVolt[25+(j*3+i)] = data->m_data[2+i*2] + (((uint16)data->m_data[3+i*2])<<8);
      }
      break;
    
    //电池模组最大最小电压信息  
    case 0x18FF9710:
      Module_VoltInfo[1].Max_VoltC      = data->m_data[0]+(((uint16)data->m_data[1])<<8);
      Module_VoltInfo[1].Max_VoltC_Node = data->m_data[2];
      Module_VoltInfo[1].Min_VoltC      = data->m_data[3]+(((uint16)data->m_data[4])<<8);
      Module_VoltInfo[1].Min_VoltC_Node = data->m_data[5];
      break;
    
    //每个模组单体温度信息
    case 0x18FF9800:
      for(i=0; i<SYS_NUMBER_MODULE_TEMP; i++) 
      {
         TempInfo.CellTemp[5 + (SYS_NUMBER_MODULE_TEMP*0+i)] = data->m_data[i+1];
      }
      break; 
    
    //每个模组最大最小温度信息  
    case 0x18FF9810:
      Module_TempInfo[1].Max_Temp     = (data->m_data[0]);
      Module_TempInfo[1].Max_TempNode = data->m_data[1];
      Module_TempInfo[1].Min_Temp     = (data->m_data[2]);
      Module_TempInfo[1].Min_TempNode = data->m_data[3];
      Module_TempInfo[1].Total_Temp   = (data->m_data[4] + (((uint16)data->m_data[5])<<8));
      Module_TempInfo[1].Ave_Temp     = Module_TempInfo[1].Total_Temp / SYS_NUMBER_MODULE_TEMP;
      break;  
    
    //每个模组总压信息
    case 0x18FF9900:
      Module_StateInfo[1].CSSU_TempH        = data -> m_data[0];
      Module_StateInfo[1].OpenWire_Status   = data -> m_data[1];
      Module_StateInfo[1].ModuleVolt_Total  = (data -> m_data[2]+(((uint16)data -> m_data[3])<<8)+(((uint32)data -> m_data[4])<<16));
      Module_StateInfo[1].InsulVolt_Total   = (data -> m_data[5]+(((uint16)data -> m_data[6])<<8)+(((uint32)data -> m_data[7])<<16));
      DataColletInfo.ModuleVolt_Total[1]    = Module_StateInfo[1].ModuleVolt_Total;
      break;
  } 
}
  
  
  
  
  
  
  
  
  
  
  
  
  
  