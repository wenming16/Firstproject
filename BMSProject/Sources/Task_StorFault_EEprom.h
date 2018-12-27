/*=======================================================================
 *Subsystem:   Âã»ú
 *File:        Task_StorFault_RRprom.h
 *Author:      WenMing
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef _TASK_STORFAULT_EEPROM_H_
#define _TASK_STORFAULT_EEPROM_H_

  
  typedef struct
  {
   uint8 IIC_Year;
   uint8 IIC_Month;
   uint8 IIC_Day;
   uint8 IIC_Hour;
   uint8 IIC_Minute;
  
  }EEPROM_FaultSave_T;
  extern  EEPROM_FaultSave_T  EEPROM_FaultSave;
     
  
  
  
  
  
  void Task_StorFault_EEprom(void);
  
  
#endif