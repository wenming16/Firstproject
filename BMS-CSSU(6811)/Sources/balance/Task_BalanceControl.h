#ifndef _TASK_PASSBALANCE_H_
#define _TASK_PASSBALANCE_H_
    
   
    
    
    #define    BALANCEPERIO  500        
    
    

typedef struct
{
   float current;
   uint8 model_work;
   uint8 number;
   uint32 total_volt;
   uint8 flag;
}BalanceInfo_T;


extern BalanceInfo_T balance_receive;

void Task_BalanceControl_ON(void);
void Task_BalanceControl_OFF(void);
    
#endif