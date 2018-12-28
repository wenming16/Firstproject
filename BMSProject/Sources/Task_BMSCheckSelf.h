 /*=======================================================================
 *Subsystem:   Âã»ú
 *File:        Task_BMSCheckSelf.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef _TASK_BMS_CHECK_SELF_H_
#define _TASK_BMS_CHECK_SELF_H_

  #include  "TypeDefinition.h"
  #include  "WorkModeJudge.h"
  #include  "BattInfoConfig.h"
  #include  "Sys_Init.h"
  #include  "PIT.h"

  typedef struct
  {
    uint8   Check_Self_state;
    uint8   Check_Self;
  }BMSCheckSelf_T;
  extern  BMSCheckSelf_T  BMSCheckSelf;

 void BMS_WorkModeCheckself(void);
 
#endif