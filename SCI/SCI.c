/*=======================================================================
 *Subsystem:   裸机
 *File:        SCI.C
 *Author:      WenMing
 *Description: 通信：SCI2.
               接口：PJ0（RXD）；PJ1（TXD）
               波特率：
 ========================================================================
 * History:        // 修改历史记录列表，每条修改记录应包括修改日期、修改者及修改内容简述
 *1. Date:         2018 - 5 - 29
     Author:       ZWB
     Modification: GPRSrxdata接受任务进行修改，增加了标定功能的接受。
========================================================================*/

#include  "SCI.h"    //头文件包含      

#include  "MC9S12XEP100.h"
#include  "derivative.h"  

//uint8 p=0,s,d=0,ss,dd=0;
//uint8 jilu[6];

/*=======================================================================
 *函数名:      SCI_init
 *功能:        初始化SCI
 *参数:        SCINo:SCI2
               sysclk：时间总线时钟:32MHz
               baud：SCI通信波特率       
 *返回：       无
 *说明：       
========================================================================*/
uint8 Init_SCI2(void) 
{    
  unsigned int SCI_temp;
  
  SCI_temp = 32000000/16/9600;
  SCI1BD = SCI_temp;              // 设置SCI0波特率为12500（BAUD），BUS_CLOCK=32MHz
                                                 
  SCI1CR1 = 0x00;                 // 设置SCI0为正常模式，八位数据位，无奇偶校验( 是否需要读状态来确认？)
  SCI1CR2 = 0x08;                 // 允许发送数据，禁止中断功能
  
  return(Init_Normal_SCI);   
}


/*=======================================================================
 *函数名:      SCI2_send_byte
 *功能:        SACI发送一个字节
 *参数:        data:要发送的字节       
 *返回：       无
 *说明：       
========================================================================*/
static
uint8 SCI2_send_byte(uint8 data) 
{ 
    uint8 cnt[1];
    do
    {
      if(++cnt[0] == 200)
      {
        return (SendMsg_Fault_SCI_SCI1SR1_TDRE);
      }
    }
    while(!SCI1SR1_TDRE);             // 等待发送数据寄存器（缓冲器）为空    
    SCI1DRL = data;  
    return(SendMsg_SCI);   
}


/*=======================================================================
 *函数名:      SCI2_send_Nbyte
 *功能:        SACI发送多个字节
 *参数:        string:要发送的字节指针       
 *返回：       无
 *说明：       
========================================================================*/
uint8 SCI2_send_Nbyte(uint8 n,uint8 string[]) 
{ 
  uint8 i;
  uint8 SCI_state; 
  for(i = 0; i < n; i++)         //发送时间限制
  { 
    
    SCI_state = SCI2_send_byte(string[i]);
  } 
  return(SCI_state); 
}

