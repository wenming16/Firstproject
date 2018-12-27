/*=======================================================================
 *Subsystem:   裸机
 *File:        PLL.C
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
#include  "PLL.h"
#include  "TypeDefinition.h"
#include  "MC9S12XEP100.h"
#include  "derivative.h" 

/*************************************************************/
/*                      初始化锁相环                         */
/*************************************************************/
uint8 Init_PLL(void) 
{
  uint8 cnt[1];
  
  CLKSEL &= 0x7f;       //设置OSCCLK作为系统时钟
  PLLCTL &= 0x8F;       //禁止锁相环
  CRGINT &= 0xDF;
  //PLLCLK=2×OSCCLK×(SYNR+1)/(REFDV+1), fbus=PLLCLK/2
  #if(BUS_CLOCK == 120000000) 
      SYNR = 0xcd;
  #elif(BUS_CLOCK == 104000000) 
    SYNR = 0xcc;
  #elif(BUS_CLOCK == 96000000) 
    SYNR = 0xcb;
  #elif(BUS_CLOCK == 88000000) 
    SYNR = 0xca;
  #elif(BUS_CLOCK == 80000000) 
    SYNR = 0xc9;
  #elif(BUS_CLOCK == 72000000) 
    SYNR = 0xc8;
  #elif(BUS_CLOCK == 64000000) 
    SYNR = 0xc7;
  #elif(BUS_CLOCK == 56000000) 
    SYNR = 0xc6;
  #elif(BUS_CLOCK == 48000000) 
    SYNR = 0xc5;
  #elif(BUS_CLOCK == 40000000) 
    SYNR = 0x44;
  #elif(BUS_CLOCK == 32000000)
    SYNR = 0x43;     
  #elif(BUS_CLOCK == 24000000)
    SYNR = 0x42;
  #elif(BUS_CLOCK == 16000000)
    SYNR = 0x01;
 #endif 

  REFDV = 0x81;
  PLLCTL |=0x70;         //使能锁相环
  asm NOP;
  asm NOP;
  do
  {
    if(++cnt[0] == 200)
    {
      return(Init_Fault_PLL_PLLCLK);
    }
  }
  while(!(CRGFLG&0x08)); //PLLCLK锁定
  CLKSEL |= 0x80;        //设置PLLCLK为系统时钟
  
  return(Init_Normal_PLL);
}




