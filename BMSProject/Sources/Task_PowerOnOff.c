/*=======================================================================
 *Subsystem:   裸机
 *File:        Task_PowerOnOff.c
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:  
========================================================================*/

 #include   "Task_PowerOnOff.h"
/*=======================================================================
 *函数名:      PositiveRelay_OFF
 *功能:        主正继电器的控制
 *参数:        wokemode:工作模式
               FltLevel:闭合继电器的2级故障
                      
 *返回：       无
 *说明：       ADC 时钟频率：2MHz
========================================================================*/
void PositiveRelay_OFF(uint8 FltLevel)
{
  if(FltLevel&&(Relay_StateGet(Relay_Positive_PORT, Relay_Positive_pin)==Relay_ON))
  {
     Relay_Control(Relay_Positive_PORT, Relay_Positive_pin, Relay_OFF);
  }

} 

void PositiveRelay_ON(void)
{
  if(Relay_StateGet(Relay_Positive_PORT, Relay_Positive_pin)==Relay_OFF)
  {
    Relay_Control(Relay_Positive_PORT, Relay_Positive_pin, Relay_ON); 
  } 
}


/*=======================================================================
 *函数名:      PositiveRelay_OFF
 *功能:        主正继电器的控制
 *参数:        wokemode:工作模式
               FltLevel:闭合继电器的2级故障
                      
 *返回：       无
 *说明：       ADC 时钟频率：2MHz
========================================================================*/
/*void Task_PowerOnOff(uint8 FltLevel, uint8 selfchecksate)
{
   if(selfchecksate == 1)
   {
      PositiveRelay_ON();
   }
   
   if(FltLevel == 1)
   {
      PositiveRelay_OFF();
   }
}*/