/*=======================================================================
 *Subsystem:   裸机
 *File:        ADC.c
 *Author:      WenMing
 *Description: 接口
               总压检测PAD0
               霍尔传感器：PAD03
               分流器：PAD10
               采样频率：2MHz
 ========================================================================
 * History:        // 修改历史记录列表，每条修改记录应包括修改日期、修改者及修改内容简述
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#include"TypeDefinition.h"
#include"MC9S12XEP100.h"
#include"derivative.h"
#include"ADC_Config.h"
#include"Filter_Function.h"
#include"ADC.h"

/*=======================================================================
 *函数名:      ADC_init
 *功能:        初始化ADC
 *参数:        无         
                      
 *返回：       无
 *说明：       ADC 时钟频率：2MHz
========================================================================*/
uint8 Init_ADC(void) 
{
  ATD0CTL2 = 0x40;  //启动A/D模块,快速清零,禁止中断
  ATD0CTL1_SRES=2;  //选用12位模数转换
  ATD0CTL3 = 0x88;   //每次只转换一个通道
  ATD0CTL4 = 0xe1;   //AD模块时钟频率为2MHz
  
  return(Init_Normal_ADC);
} 

/*=======================================================================
 *函数名:      ADC_Value()
 *功能:        1路A/D转换函数，获取通道channel的转换结果
 *参数:        channel：通道号（0~15）         
 *返回：       ADdata:channel通道的转换结果 
 
 *说明：       ADC 时钟频率：2MHz
========================================================================*/
uint16 ADC_Value(uint8 channel) 
{
  uint16 ADdata;
  ATD0CTL5 = channel;
  for(;;)
  {
     if((ATD0STAT0 & (1 << 7)) != 0)
     {          
        ADdata = ATD0DR0; 
        break;
     }
  }
  return ADdata;
}
/*=======================================================================
 *函数名:      ADC_Volt_Sys
 *功能:        电池包总电压的检测
 *参数:        无         
 *返回：       Volt_Sys:channel通道的转换结果
 
 *说明：       ADC 时钟频率：2MHz
========================================================================*/
/*
float ADC_Volt_Sys(void) 
{
   uint16 Voltemp;
   float Volt_Sys;  
   Voltemp = ADC_Value(VoltageChannel9);  //记得显示
   Volt_Sys = (Voltemp / 4096.0) * Stand_Volt * Trans_Volt;  // Stand_Volt,Trans_Volt为宏定义，基准电压和总压转换
   return Volt_Sys;     
}  
*/

/*=======================================================================
 *函数名:      ADC_Current
 *功能:        电池包霍尔传感器电流的检测
 *参数:        无         
                      
 *返回：       Current:霍尔传感器通道的转换结果
 *说明：       ADC 时钟频率：2MHz
========================================================================*/
float ADC_Current(void) 
{
  uint16 Currtemp;
  float Hall_voltage;
  Currtemp = ADC_Value(HallChannel); 
  Hall_voltage = (Currtemp/4095.0*Stand_Volt-2.5)*Hall_Range/2.0;  //根据传感器再做转换
  return Hall_voltage;  
}
/*=======================================================================
 *函数名:      ADC_CC2_State
 *功能:        电池包霍尔传感器电流的检测
 *参数:        无         
                      
 *返回：       Current:霍尔传感器通道的转换结果
 *说明：       ADC 时钟频率：2MHz
========================================================================*/
uint16 ADC_CC2_State(void) 
{
  uint16 cc2_state;
  cc2_state = ADC_Value(QCharge_CC2); 
  return cc2_state;  
}
