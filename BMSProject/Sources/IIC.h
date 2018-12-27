/*=======================================================================
 *Subsystem:   Âã»ú
 *File:        IIC.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef _IIC_H_
#define _IIC_H_  

  #include"TypeDefinition.h"

  #define SCL     PTJ_PTJ7
  #define SDA     PTJ_PTJ6
  #define SCL_dir DDRJ_DDRJ7
  #define SDA_dir DDRJ_DDRJ6

  typedef enum
  {
    Init_Normal_IIC = 0
  };
  
  typedef enum
  {
    Normal_IICWrite = 0,
    Fault_IICWrite_IBIF1,
    Fault_IICWrite_RXAK1,
    Fault_IICWrite_IBIF2,
    Fault_IICWrite_RXAK2,
    Fault_IICWrite_IBIF3,
    Fault_IICWrite_RXAK3
  };
  
  uint8 Init_IIC(void);
  uint8 IIC_write(unsigned char addr,unsigned char writeaddr,unsigned char data);
  unsigned char IIC_read(unsigned char addr,unsigned char readaddr); 

#endif                                                                           
