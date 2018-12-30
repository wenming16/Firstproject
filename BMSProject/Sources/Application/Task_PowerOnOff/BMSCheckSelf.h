 /*=======================================================================
 *Subsystem:   Âã»ú
 *File:        BMSCheckSelf.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef _BMS_CHECK_SELF_H_
#define _BMS_CHECK_SELF_H_

  #include  "TypeDefinition.h"

  typedef struct
  {
    uint8   Check_Self_state;
    uint8   Check_Self;
  }BMSCheckSelf_T;
  extern  BMSCheckSelf_T  BMSCheckSelf;

 void BMS_WorkModeCheckself(void);
 
#endif