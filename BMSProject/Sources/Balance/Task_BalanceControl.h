#ifndef _TASK_PASSBALANCE_H_
#define _TASK_PASSBALANCE_H_
    
    #include   "LTC6811_ConnectType.h"
    #include   "TypeDefinition.h"
    #include   "LTC6811_PassBalance.h"
    #include   "BattInfoParaStructure.h"
    #include   "Task_FaultLevelJudge.h"
    #include   "LTC6811_VoltCollect.h"

    #define    balanceperio  500 
    
    uint8 BalanceControl_Strategy(float, uint8, uint16, uint32, uint8, float);
    void Task_BalanceControl(void);
    
    
#endif