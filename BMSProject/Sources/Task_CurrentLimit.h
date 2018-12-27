/*=======================================================================
 *Subsystem:   裸机
 *File:        Task_CurrentLimit.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef _TASK_CURRENT_LIMIT_H_
#define _TASK_CURRENT_LIMIT_H_

  typedef struct
  {
    float Current_Charge_Constant;        //充电持续电流大小
  }CurrentLimit_T;
  extern CurrentLimit_T CurrentLimit;   
    
  void Task_CurrentLimit(void);  
    
   
#endif