#ifndef  _TASK_BOOT_H_
#define  _TASK_BOOT_H_

  #include "TypeDefinition.h"
  //Boot升级的ID号(首次则用数据帧0x001)
  #define  Boot_ID            0xF300   //扩展帧

  typedef struct
  {
     uint16 *Boot;
     uint8  OnlineUpdateCheck;
  }Boot_Data_T;
  extern  Boot_Data_T Boot_Data;
  
  void Task_BootLoader();  
  
#endif