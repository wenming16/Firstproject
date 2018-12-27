/*=======================================================================
 *Subsystem:   裸机
 *File:        ADC.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef _ADC_H_
#define _ADC_H_

  typedef enum
  {
    Init_Normal_ADC = 0
  };
  
  void   ADC_init(void);                    //AD初始化
  uint16 ADC_Value(uint8 channel);
  //float ADC_Volt_Sys(void);               //总压检测
  float  ADC_Current(void);                //霍尔器电流检测
  uint16 ADC_CC2_State(void);

#endif