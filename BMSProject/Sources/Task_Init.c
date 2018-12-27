/*=======================================================================
 *Subsystem:   裸机
 *File:        Task_Create.C
 *Author:      Wenming
 *Description: 通信：
               接口：
               波特率：
 ========================================================================
 * History:    修改历史记录列表
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#include "Task_Init.h" 

TASK tasks[ARRAY_SIZE];            //任务堆栈设计     
/*=======================================================================
 *函数名:      Task_Init(uint8 FLAGS, void (*HANDLE)())  
 *功能:        任务创建函数
 *参数:        无       
 *返回：       无
 *说明：       每个任务创建需要一个时间计时的标志位与函数指针；
========================================================================*/
TASK Task_Init(uint8 FLAGS, void (*HANDLE)())             // 创建单任务结构体函数；
{
	TASK task;
	task.flags = FLAGS;
	task.handle = HANDLE;
	return (task);
}

/*=======================================================================
 *函数名:      Task_Handle(void)  
 *功能:        任务轮询
 *参数:        无       
 *返回：       无
 *说明：       创建任务数组函数，便于轮询执行任务
========================================================================*/
void Task_Handle(void)                               // 任务执行函数，按任务的创建顺序执行任务；                     
{
  uint8 i;
  for (i = 0; i < ARRAY_SIZE; i++)
  {   
     if (tasks[i].flags==1)                    // 查询任务时间是否到；
     {             
        (*tasks[i].handle)();                  // 执行任务
        tasks[i].flags = 0;                    // 消除任务标记位              
     }        
  }  
}
   
/*=======================================================================
 *函数名:      Task_Create(void)  
 *功能:        多任务创建函数
 *参数:        无       
 *返回：       无
 *说明：       创建任务数组函数，便于轮询执行任务
========================================================================*/  
void Task_Create()                                      // 创建任务函数；
{  
   //BMS数据接收及处理任务     
   /*tasks[0] = CreateTask(0, TaskVAndTDataProcess);    //电压温度处理函数 
   tasks[1] = CreateTask(0, Task_Soccal);               //SOC计算
   tasks[2] = CreateTask(0, Dis_ChargeCurrent);         //充放电电流电任务
   tasks[3] = CreateTask(0, Task_FailLevel_Judge);      //充放电故障等级判断   
   tasks[4] = CreateTask(0, BMSCheckSelf);              //BMS自检  
   tasks[5] = CreateTask(0, InsulationDetect);          //绝缘检测 
   
   
   
   //发送给VCU的信息
   tasks[6] = CreateTask(0, Task_ToVCU_BasicInfo);      //200ms 
   tasks[7] = CreateTask(0, Task_ToVCU_Temp);           //200ms
   tasks[8] = CreateTask(0, Task_ToVCU_Volt);           //200ms
   tasks[9] = CreateTask(0, Task_ToVCU_Error);          //200ms  
   
   //发送给慢充的信息
   //tasks[11] = CreateTask(0, Task_To_SlowCharge);       //1s
   
   //充电任务
   tasks[10] = CreateTask(0, Task_Charge);              //250ms  
   //tasks[12] = CreateTask(0, Out_Queue_FromCharge);   //50ms 充电数据出队解析（取消）         
   tasks[11] = CreateTask(0, BMSToCharge_BHM);          //250ms       
   tasks[12] = CreateTask(0, BMSToCharge_BRM);          //250ms  
   tasks[13] = CreateTask(0, BMSToCharge_BCP);          //500ms 
   tasks[14] = CreateTask(0, BMSToCharge_BRO);          //250ms
   tasks[15] = CreateTask(0, BMSToCharge_BCL);          //50ms       
   tasks[16] = CreateTask(0, BMSToCharge_BCS);          //250ms  
   tasks[17] = CreateTask(0, BMSToCharge_BSM);          //250ms 
   tasks[18] = CreateTask(0, BMSToCharge_BST);          //10ms
   tasks[19] = CreateTask(0, BMSToCharge_BSD);          //250ms 
   tasks[20] = CreateTask(0, BMSToCharge_BEM);          //250ms    
   
   //与上位机通信任务        
   tasks[21] = CreateTask(0, Bms_to_Up_Monitor);        //上传数据 
   tasks[22] = CreateTask(0, Task_BootLoader);          //boot任务1s    
   tasks[23] = CreateTask(0, PowerONandOFF_Control);    //单片机上下电控制
   
   //tasks[24] = CreateTask(0, GPIO_test);    //单片机上下电控制
   */
}



