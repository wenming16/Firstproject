/*=======================================================================
 *Subsystem:   裸机
 *File:        Init_CAN.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef _INIT_CAN_H_
#define _INIT_CAN_H_

  #include  "TypeDefinition.h"
  
 /*===================================================================
                              CAN波特率设置
  ====================================================================*/
 
  typedef struct 
  {
    uint8 InitResult_CAN0;
    uint8 InitResult_CAN1;
    uint8 InitResult_CAN2;
    uint8 InitResult_CAN3;
      
  }InitResult_CAN_T;
  extern InitResult_CAN_T InitResult_CAN; 

  void Init_CAN(void);





#endif  




