#ifndef  TASK_CREATE_
#define  TASK_CREATE_

  #include  "TypeDefinition.h"
  
  #define ARRAY_SIZE   24                // 任务堆栈大小； 

  typedef struct                         // 任务结构体；
  {
      uint8 flags;                       // 任务标识；  
      void (*handle)();                  // 任务处理函数的函数指针； 
  }TASK;   

  void Task_Handle(void);                //任务轮询
  void Task_Init(void);                  //函数表的创建

#endif