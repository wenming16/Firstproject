/*=======================================================================
 *Subsystem:   裸机
 *File:        SCI.h
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


#ifndef _SCI_H_
#define _SCI_H_
  
  #include  "TypeDefinition.h"
  
  typedef enum
  {
    Init_Normal_SCI = 0
  };
  
  typedef enum
  {
    SendMsg_SCI = 0,
    SendMsg_Fault_SCI_SCI1SR1_TDRE
  };
  
  uint8 Init_SCI2(void);
  uint8 SCI2_send_Nbyte(uint8 n,uint8 string[]);  
     

#endif