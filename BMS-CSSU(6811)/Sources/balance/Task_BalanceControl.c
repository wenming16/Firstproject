/*=======================================================================
 *Subsystem:   裸机
 *File:        Task_BalanceControl.C
 *Author:      WenM
 *Description: 通信：
               接口：
               波特率：
 ========================================================================
 * History:    修改历史记录列表
 * 1. Date:   
      Author:  
      Modification:  
      
========================================================================*/

#include  "Includes.h"

BalanceInfo_T balance_receive;
/*=======================================================================
 *函数名:      BalanceControl_Strategy(flaot, uint8, uint16, uint32, uint8, float)
 *功能:        对电池组进行被动均衡
               均衡控制策略:当最大单体大于平均电压balancevolt时进行均衡
 *参数:        curr：判断电流的大小,只当电流小于3A(静态)进行均衡
               faultflg:  在不出现故障的情况下才进行均衡
               voltmax：  最大电压
               totalvolt：25串电池的总压,用于求平均电压
               balacenod: 均衡的节点                 
 
 *返回：       uint8:     0表示正常均衡
                          1表示均衡出现错误参数并停止均衡
                          2表示未达到均衡条件不进行均衡
 *说明：       被动均衡函数
========================================================================*/
static
uint8 BalanceControl_Strategy(float curr, uint8 faultflg, uint16 voltmax, uint32 totalvolt, uint8 balacenod, uint16 balancevolt)
{
  uint8 tskstate=2;                   //返回2表示未进行均衡
  static uint16 cnt;
  uint8 balanceNum;
  
  if(curr>=10 && (faultflg == 0) && (1 == balance_receive.model_work))//MODE_CHARGE)) //只有在充电的过程中电流大于5A才开启
  {
    if((voltmax - (totalvolt/25.0)) > balancevolt)
    { 
       if(++cnt*BALANCEPERIO/1000>=3)//持续2s,连续发命令是否会出错?
       {
          
         PTT_PTT0 = 0;//均衡显示灯开启
         cnt = 3000/BALANCEPERIO;
         if(balacenod <= NUM1_Batper_true)
         {
           tskstate = LTC6811_BalanceControl(balacenod, 0x00, 0x00, 1); 
         }
         else if(balacenod <= (NUM1_Batper_true+NUM2_Batper_true))
         {
            balanceNum = balacenod-NUM1_Batper_true;
           tskstate =  LTC6811_BalanceControl(0x00, balanceNum, 0x00, 1);
         }
         else if(balacenod <= (NUM1_Batper_true+NUM2_Batper_true+NUM3_Batper_true))
         {
            balanceNum = balacenod-NUM1_Batper_true-NUM2_Batper_true;
            tskstate = LTC6811_BalanceControl(0x00, 0x00, balanceNum, 1);
         }
         else
         {
            return 1;
         } 
       }
     } 
     else
     {
        cnt = 0;
     }
  }
  else
  {
     LTC6811_BalanceControl(0x00, 0x00, 0x00, 0); //未满足条件不进行均衡 
     return 2;
  }
  return tskstate;
}

/*=======================================================================
 *函数名:      Task_BalanceControl_ON(void)
 *功能:        均衡开启控制任务
 *参数:        无       
 *返回：       无
 *说明：       根据均衡控制策略开启均衡控制,只在电压采集完毕之后开启均衡
========================================================================*/
void Task_BalanceControl_ON(void)
{
   uint8 balancestate;
          
  if(balance_receive.total_volt < 5000)
  {
      balance_receive.flag = 1;
  } 
  else
  {    
      balance_receive.flag = 0;
  }
          
   BalanceControl_Strategy(balance_receive.current, balance_receive.flag,\
                           VoltInfo.CellVolt_Max, balance_receive.total_volt,(VoltInfo.CellVolt_MaxNode+1), 500); 
   
   memset(&balance_receive,0,sizeof(balance_receive));
   
   Task_Flag_Counter.Counter_Balance_open++;                                       
}
/*=======================================================================
 *函数名:      Task_BalanceControl_OFF(void)  
 *功能:        均衡关闭控制任务
 *参数:        无       
 *返回：       无
 *说明：       在电压采集之前关闭均衡开关
========================================================================*/
void Task_BalanceControl_OFF(void)
{
   LTC6811_BalanceControl(0, 0, 0, 0);//关闭均衡功能
   PTT_PTT0 = 1;
   Task_Flag_Counter.Counter_Balance_close++;
}