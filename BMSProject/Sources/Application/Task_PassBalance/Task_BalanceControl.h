#ifndef _TASK_PASSBALANCE_H_
#define _TASK_PASSBALANCE_H_
    
    #include   "LTC6811_ConnectType.h"
    #include   "TypeDefinition.h"
    #include   "LTC6811_PassBalance.h"
    #include   "Task_DataProcess.h"
    #include   "Task_FltLevJudg.h"
    #include   "LTC6811_VoltCollect.h"

    #define    balanceperio  500 
    
    void Task_BalanceControl_ON(void);
    void Task_BalanceControl_OFF(void);
    
#endif