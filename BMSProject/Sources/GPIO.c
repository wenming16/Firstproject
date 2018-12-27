/*=======================================================================
 *Subsystem:   裸机
 *File:        GPIO.C
 *Author:      WENMING
 *Description: 通信：
               接口：
               波特率：
 ========================================================================
 * History:    修改历史记录列表
 * 1. Date:
      Author:
      Modification:
========================================================================*/ 

//头文件包含
#include  "GPIO.h"             //普通IO口构件头文件
#include  "TypeDefinition.h"
#include  "derivative.h"              
    //2个内部函数
static uint8 ABEK_Bit(uint16 port);
static uint8 ADEK_AddrChg(uint16 port,uint8 reg);
    
//构件函数实现     
    //----------------------------------------------------------------------*
    //函数名: GPIO_Init                                                     *
    //功  能: 初始化GPIO（先初始化端口某引脚方向寄存器，再判断方向是否为输出，若为输出则需要再设置输出高或低电平，若为输入则不需要再设置）* 
    //参  数: port:端口名                                                   *
    //        pin:指定端口引脚                                              *
    //        direction:引脚方向,0=输入,1=输出                              * 
    //        state:初始状态,0=低电平,1=高电平（数据寄存器）                *
    //返  回: 无                                                            * 
    //----------------------------------------------------------------------*
    uint8 Init_GPIO(uint16 port,uint8 pin,uint8 direction,uint8 state)
    {
        GPIO_Set(port,DDR,pin,direction);
        //1.引脚方向为输出时,设置引脚状态（引脚方向为输出时，要写该引脚对应数据寄存器位；引脚方向为输入时，读） 
        if(direction==1) 
          GPIO_Set(port,PRT,pin,state);
        
        return (Init_Normal_GPIO);
    }

    //----------------------------------------------------------------------* 
    //函数名: GPIO_Get                                                      *
    //功  能: 获得引脚状态                                                  *
    //参  数: port: 端口名                                                  *
    //        reg:  指定端口寄存器                                          *
    //        pin:  指定端口引脚                                            *
    //返  回: state:得到引脚状态(ABEK上拉或低功耗,效果=0xFF或=0x00)         * 
    //----------------------------------------------------------------------* 
    uint8 GPIO_Get(uint16 port,uint8 reg,uint8 pin)
    {
        uint8 reval=0,bit=0;
        //获取A、B、E、K的上拉允许寄存器和低功耗驱动设置值
        if((reg==PER)||(reg==RDR))
        {
          bit=ABEK_Bit(port);
          if((reg==PER)&&(bit!=8))
          {
            reval=BGET(bit,PUCR );
            return reval;
          }
          else if((reg==RDR)&&(bit!=8))
          {
            reval=BGET(bit,RDRIV);
            return reval;
          }
        }
        //调整方向寄存器偏移量
        reg=ADEK_AddrChg(port,reg);
        //获取寄存器的设置值
        reval = BGET(pin,(*(vuint8 *)(port+reg)));    //得到引脚的状态
        return reval;
    }

    //----------------------------------------------------------------------* 
    //函数名: GPIO_Set                                                      *
    //功  能: 设置引脚状态                                                  *
    //参  数: port :端口名                                                  *
    //        reg  :指定端口寄存器                                          *
    //        pin  :指定端口引脚                                            *
    //        state:0=低电平,1=高电平(ABEK上拉或低功耗,效果=0xFF或=0x00)    *
    //返  回: 无                                                            *
    //----------------------------------------------------------------------*
    void GPIO_Set(uint16 port,uint8 reg,uint8 pin,uint8 state)
    {
        uint8 bit=0;
        //A、B、E、K的上拉允许寄存器和低功耗驱动寄存器设置
        if((reg==PER)||(reg==RDR))  
        {
          bit=ABEK_Bit(port);
          if((reg==PER)&&(bit!=8))
          {
            if(state == 1) 
              BSET(bit,PUCR);
            else  
              BCLR(bit,PUCR);
            return;
          }
          else if((reg==RDR)&&(bit!=8))
          {
            if(state == 1)
              BSET(bit,RDRIV);
            else 
              BCLR(bit,RDRIV);
            return;
          }
        }
        //调整方向寄存器偏移量
        reg=ADEK_AddrChg(port,reg);
        //设置寄存器的值
        if(state == 1)
          BSET(pin,(*(vuint8 *)(port+reg)));   //输出高电平(1)
        else                                  
          BCLR(pin,(*(vuint8 *)(port+reg)));   //输出低电平(0)
    } 
    
    //----------------------------------------------------------------------* 
    //函数名: GPPort_Get                                                    *
    //功  能: 获得获取端口各引脚状态                                        *
    //参  数: port:  端口名                                                 *
    //        reg:   指定端口寄存器                                         *
    //返  回: 得到端口状态(对于ABEK上拉或低功耗,效果=0xFF或=0x00)           * 
    //----------------------------------------------------------------------* 
    uint8 GPPort_Get(uint16 port,uint8 reg)
    {
        uint8 reval=0,bit=0;
        //获取A、B、E、K的上拉允许寄存器和低功耗驱动设置值
        if((reg==PER)||(reg==RDR))
        {
          bit=ABEK_Bit(port);
          if((reg==PER)&&(bit!=8))
          {
            reval=BGET(bit,PUCR );
            if(reval==1) reval=0xFF;
            else reval=0x00;
            return reval;
          }
          else if((reg==RDR)&&(bit!=8))
          {
            reval=BGET(bit,RDRIV);
            if(reval==1) reval=0xFF;
            else reval=0x00;
            return reval;
          }
        }
        //调整方向寄存器偏移量
        reg=ADEK_AddrChg(port,reg);
        //获取寄存器的设置值
        reval = (*(vuint8 *)(port+reg));    //得到引脚的状态
        return reval;
    }
    
    //----------------------------------------------------------------------* 
    //函数名: GPPort_Set                                                    *
    //功  能: 设置端口各引脚状态                                            *
    //参  数: port   :  端口名                                              *
    //        reg    :  指定寄存器                                          *
    //        setFlag:  =0时, bValue所有=0的位，寄存器对应位清0；           *
    //                  =1时, bValue所有=1的位，寄存器对应位置1；           *
    //                  =其它值,寄存器=bValue                               *
    //        bValue:指定值(对于ABEK上拉或低功耗,效果=0xFF或=0x00)          *
    //返  回: 无                                                            *
    //----------------------------------------------------------------------*
    void GPPort_Set(uint16 port,uint8 reg,uint8 setFlag,uint8 bValue)
    {
        uint8 bit=0;
        //A、B、E、K的上拉允许寄存器和低功耗驱动寄存器设置
       if((reg==PER)||(reg==RDR))
        {
          bit=ABEK_Bit(port);
          if((reg==PER)&&(bit!=8))
          {
            if(bValue == 0xFF) 
              BSET(bit,PUCR);
            else if(bValue == 0x00) 
              BCLR(bit,PUCR);
            return;
          }
          else if((reg==RDR)&&(bit!=8))
          {
            if(bValue == 0xFF) 
              BSET(bit,RDRIV);
            else if(bValue == 0x00)
              BCLR(bit,RDRIV);
            return;
          }
        }
        //调整方向寄存器偏移量
        reg=ADEK_AddrChg(port,reg);        
        //设置寄存器的值
        if(setFlag==1)      (*(vuint8 *)(port+reg)) |= bValue;          
        else if(setFlag==0) (*(vuint8 *)(port+reg)) &= bValue;          
        else                (*(vuint8 *)(port+reg))  = bValue;          
    }         
    
    //2个内部函数的实现
    //----------------------------------------------------------------------*
    //函数名: ABEK_Bit                                                      *
    //功  能: A、B、E、K的上拉允许寄存器和低功耗驱动寄存器的位确定          * 
    //参  数: port:端口名                                                   *
    //返  回: 寄存器对应的位值                                              * 
    //----------------------------------------------------------------------*
    static
    uint8 ABEK_Bit(uint16 port)
    {
        uint8 bit=0;
        switch(port)
        {
          case PA:bit=0;
          break;
          case PB:bit=1;
          break;
          case PE:bit=4;
          break;
          case PK:bit=7;
          break;
          default:bit=8;
        }
        return bit;
    }
    
    //----------------------------------------------------------------------*
    //函数名: ADEK_AddrChg                                                  *
    //功  能: 调整方向寄存器偏移量                                          * 
    //参  数: port:端口名                                                   *
    //        reg :指定端口寄存器                                           *
    //返  回: 寄存器地址偏移量修正值                                        * 
    //----------------------------------------------------------------------*
    static
    uint8 ADEK_AddrChg(uint16 port,uint8 reg)
    {
        if((port==PE)||(port==PK))
        {
          if(reg==DDR) 
            reg=DDR-1;  //此时reg=PTI 输入寄存器
        }
        else if((port==PAD0)||(port==PAD1))
        {
          if(reg==RDR) 
            reg=RDR+1;
          else if(reg==PER) 
            reg=PER+2; 
        }
        return reg;
    }
   
   

