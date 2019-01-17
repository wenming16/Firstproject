/*=======================================================================
 *Subsystem:   ���
 *File:        Init_Sys.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/
#ifndef _INIT_SYS_H_
#define _INIT_SYS_H_  

  #include  "TypeDefinition.h" 

  typedef struct//�ײ��ʼ�����λ 00:�ɹ�;1:ʧ��
  {
    uint8 ADC;
    uint8 IIC;
    uint8 PIT0;
    uint8 PLL;
    uint8 Relay_Positvie; //�����̵���
    uint8 EEPROM;
    //uint8 CAN0;
    uint8 CAN1;
    uint8 CAN2;
    //uint8 CAN3;
    uint8 Screen;
    uint8 SPI;
    uint8 Insul;//��Ե��� 
    uint8 AllInit_State;   
  }SysInitState_T;
  extern SysInitState_T g_SysInitState;
  
  void Init_Sys(void);
  
#endif  