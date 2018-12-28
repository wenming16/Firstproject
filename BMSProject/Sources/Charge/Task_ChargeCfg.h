/*=======================================================================
 *Subsystem:   裸机
 *File:        Task_BmsAndChargePile.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef _TASK_BMS_AND_CHARGE_PILE_H_
#define _TASK_BMS_AND_CHARGE_PILE_H_  

  #include  "TypeDefinition.h"  
  #include  "CAN.h"  
  
  typedef struct
  {
    uint16  Volt_Max_ChargePile;
    uint16  Curr_Max_ChargePile;
    uint8   Control_ChargePile;
   
    uint16  VoltC_Max;
    uint16  VoltC_Min;
    uint8   SOC;
    uint8   Temp_Max;
    uint16  VoltS;
  }BMSChargePile_T;
  extern BMSChargePile_T BMSChargePile;
  
  typedef struct
  {
    uint8   TempH_Cell;
    uint8   TempL_Cell;
    uint8   CurrH_Cell;
    uint8   Insul;
    uint8   BMSGetMsg;
    uint8   FaultFlag;
  
  }BMSChargePile_State_T;
  extern BMSChargePile_State_T BMSChargePile_State;
  
  typedef struct
  {
    uint16  Volt_ChargePileOut;
    uint16  Curr_ChargePileOut;
   
  }ChargePileBMS_T;
  extern ChargePileBMS_T ChargePileBMS;
  
  typedef struct
  {
    uint8   Hard;
    uint8   TempH_ChargePile;
    uint8   VoltL_ChargePile;
    uint8   On_Line;
    uint8   GetMsg;
  
  }ChargePileBMS_State_T;
  extern ChargePileBMS_State_T ChargePileBMS_State;
  
  //BMS发送至充电桩
  uint8 CAN_ToChargePileInit(void);
  void Task_BmsToChargePile(void); 
  
  //充电桩发送至BMS
  void ChargePile_to_Bms(pCANFRAME data);
  
#endif