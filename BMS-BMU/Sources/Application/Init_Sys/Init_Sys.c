/*=======================================================================
 *Subsystem:   裸机
 *File:        Init_Sys.C
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
#include  "includes.h"  

// 整个系统的初始化函数  
LIBDEF_MemPtr  MemPtr;
SysInitState_T  g_SysInitState;

static void Physic_Init(void);
/*=======================================================================
 *函数名:      Init_Sys(void)
 *功能:        系统初始化
 *参数:        无       
 *返回：       无
 
 *说明：       
========================================================================*/
void Init_Sys(void) 
{ 
  DisableInterrupts;      
  IVBR=0x7F; //应用程序中的中断基向量地址为7F
  
  //系统初始化状态返回值清零
  MemPtr = memset(&g_SysInitState, 0, sizeof(SysInitState_T));
  
  //物理层初始化
  Physic_Init();
  
  
  //DS3231时间初始化
  #if(RESET_CLOCK == 1)
  {
    DS3231SN_INIT(0b00011000, 1, 1, 1, 0, 0b01011001); //时钟初始化(18/01/01)，while卡死的原因是IIC初始化放在读取数据之前
  }//此处用到BCD码,高4位为十进制的十位,低4位为十进制的个位
  #endif
  //获取系统时间  
  Time_Init();
  
  //数据处理数据清零
  Init_TaskDataProcess();
  
  //电流限制数据清零
  Init_TaskCurrLimit();
  
  //故障等级判断数据清零
  Init_TaskFltLevJudg(); 
  
  //故障阈值初始化
  Init_UpMonitor();
  
  //读取EEE中的值
  Get_EEprom_Value();
  
  //SOC初始化功能
  #if(RESET_SOC == 1)
  {
    g_SOCInfo.SOC_Init = 0;    
  }
  #endif
   
  
  // 初始化创建所有任务,将flag置0 
  Task_Init();                  
  
  EnableInterrupts;   
}

/*=======================================================================
 *函数名:      Physic_Init(void)
 *功能:        底层初始化,将底层对应的应用进行初始化
 *参数:        无       
 *返回：       无
 *说明：       初始化物理层、全局变量、系统时间等
========================================================================*/
static
void Physic_Init(void)
{ 
  //底层硬件初始化
  g_SysInitState.PLL    = Init_PLL();              //锁相环初始化
  g_SysInitState.EEPROM = Init_Flash();            //EEPROM初始化
  g_SysInitState.IIC    = Init_IIC();              //IIC初始化;在对时间模块进行清零之前先要初始化 
  g_SysInitState.ADC    = Init_ADC();              //AD模块初始化
  g_SysInitState.CAN1   = CAN_ToChargeInit();      //充电CAN,CAN1 
  g_SysInitState.CAN2   = CAN_UpMonitorInit();     //内网CAN,CAN2
  g_SysInitState.Relay_Positvie = Init_Relay();    //主正继电器初始化
  
  g_SysInitState.Insul  = Insulation_Init();       //绝缘检测
  g_SysInitState.Screen = Init_Screen();           //显示屏SCI1初始化  
  g_SysInitState.SPI    = LTC6804_Init();
  g_SysInitState.PIT0   = Init_PIT();             //PIT0,中断周期为10ms  
 
  //所有初始化
  if(g_SysInitState.PLL || g_SysInitState.IIC || g_SysInitState.EEPROM ||\
     g_SysInitState.ADC || g_SysInitState.CAN1 || g_SysInitState.CAN2 ||\
     g_SysInitState.Relay_Positvie || g_SysInitState.PIT0 ||\
     g_SysInitState.Screen || g_SysInitState.SPI||g_SysInitState.Insul)
  {
    g_SysInitState.AllInit_State = 1;
  }
  else
  {
    g_SysInitState.AllInit_State = 0;
  }
}












