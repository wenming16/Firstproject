
//文明事业大成
/*=======================================================================
 *Subsystem:   裸机
 *File:        CAN.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef _CAN_H_
#define _CAN_H_
  
  #include  "TypeDefinition.h"
   
  
  //CAN初始化
  enum Init_CAN
  {
    //初始化位置正常为0
    Init_Normal_CAN = 0,
    //初始化位置失败的对应
    Init_Fault_CAN_BaudRate,
    Init_Fault_CAN_EnterState,
    Init_Fault_CAN_Unready,
    Init_Fault_CAN_Synchr
  };
  
  //CAN发送数据
  enum SendMsg_CAN
  {
    SendMsg_Normal = 0,
    SendMsg_Fault_Lenth,
    SendMsg_Fault_Synch,
    SendMsg_Fault_NoEmptyNode
  };
  
  //CAN接收数据
  enum GetMsg_CAN
  {
    GetMsg_Normal = 0,
    GetMsg_Fault_RFLG_RXF
  };
  
  //CAN报文结构体
  typedef struct 
  {
    uint32 m_ID;      // msg发送方ID
    uint8 m_IDE;      // 扩展帧为1，标准帧为0
    uint8 m_RTR;      // 远程帧为1，数据帧为0
    uint8 m_data[8];  // 帧数据
    uint8 m_dataLen;  // 帧数据长度
    uint8 m_priority; // 发送优先级 
  }CANFRAME, *pCANFRAME; 

  

  //CAN0
  uint8 INIT_CAN0(uint16 Baud_Rate);
  uint8 SendMsg_CAN0(pCANFRAME sendFrame);
  uint8 GetMsg_CAN0(pCANFRAME receiveFrame);

  //CAN1  充电
  uint8 INIT_CAN1(uint16 Baud_Rate);
  uint8 SendMsg_CAN1(pCANFRAME sendFrame);
  uint8 GetMsg_CAN1(pCANFRAME receiveFrame);
  
  //CAN2  内网 
  uint8 INIT_CAN2(uint16 Baud_Rate);
  uint8 SendMsg_CAN2(pCANFRAME sendFrame);
  uint8 GetMsg_CAN2(pCANFRAME receiveFrame);
  
  //CAN3
  uint8 INIT_CAN3(uint16 Baud_Rate);
  uint8 SendMsg_CAN3(pCANFRAME sendFrame);
  uint8 GetMsg_CAN3(pCANFRAME receiveFrame);

#endif  




