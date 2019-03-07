/*=======================================================================
 *Subsystem:   Bootloader
 *File:        main.C
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
#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */
#include "Types.h"
#include "Config.h"
#include "CAN.h"
#include "Errors.h"
#include "PFlash.h"
#include "Srec.h"
#include "PartID.h"
#include "eeprom.h"
#include "Srec.h"
#include "PIT.h"

#pragma MESSAGE DISABLE C5909
#pragma MESSAGE DISABLE C12056

#define RESET_VEC_SRC       0x7FFFE0        //0x7FFFE0
#define RESET_VEC_DST       0x7FDFF8        //0x7FEFF8

#define CPU12IVBR           0xFF00          //bootloader基地址
#define BUS_CLOCK           32000000

#define __SEG_START_REF(a)  __SEG_START_ ## a
#define __SEG_END_REF(a)    __SEG_END_ ## a
#define __SEG_SIZE_REF(a)   __SEG_SIZE_ ## a
#define __SEG_START_DEF(a)  extern INT8 __SEG_START_REF (a) []
#define __SEG_END_DEF(a)    extern INT8 __SEG_END_REF   (a) []
#define __SEG_SIZE_DEF(a)   extern INT8 __SEG_SIZE_REF  (a) []

__SEG_START_DEF (RAM_CODE);
__SEG_END_DEF   (RAM_CODE);
__SEG_SIZE_DEF  (RAM_CODE);

volatile  ErrType FlashErr;
UINT16 *boot;       //进入BootLoader的确认字存储地址
UINT16 *DataIVBR;   //是否需要升级的确认字存储地址
/*=======================================================================
 *函数名:      static UINT8 EraseFlash_Store(void)
 *功能:        擦除Flash
 *参数:        无       
 *返回：       无
 *说明：       在代码存储的过程中擦除存储区的flash空间，
               代码存储的区间为：0x70_0000~0x77_0000
========================================================================*/
static UINT8 EraseFlash_Store(void)
{
  UINT8 Error;
  
  switch (PARTID)
  {
    //S12XE 1024k flash
    case MASK_0M48H:  //Falling to next case
    case MASK_1M48H:  //Falling to next case
    case MASK_2M48H:  //Falling to next case
    case MASK_3M48H:  //Falling to next case
    case MASK_4M48H_5M48H:
    case MASK_0N35H_1N35H:
      if(Error = PFlash_EraseBlock(0x740000UL))
      {
        return(Error);
      }
      if(Error = PFlash_EraseBlock(0x700000UL))
      {
        return(Error);
      }
      return(noErr);
    
    default:
      return(UnknownPartID);
  }      
}

/*=======================================================================
 *函数名:      static UINT8 EraseFlash_Upgrade(void)
 *功能:        擦除Flash
 *参数:        无       
 *返回：       无
 *说明：       在代码升级的过程中擦除源代码存储区的flash空间，
               代码存储的区间为：0x78_0000~0x7FDFDF
               此处为区块擦除
========================================================================*/
static UINT8 EraseFlash_Upgrade(void)
{
  UINT8 Error;
  
  switch (PARTID)
  {
    //S12XE 1024k flash
    case MASK_0M48H:  //Falling to next case
    case MASK_1M48H:  //Falling to next case
    case MASK_2M48H:  //Falling to next case
    case MASK_3M48H:  //Falling to next case
    case MASK_4M48H_5M48H:
    case MASK_0N35H_1N35H:
      //erase whole P-Flash block 0 except bootloader area
      if(Error = PFlash_EraseSectorBySector(0x7C0000UL, 0x7FDFDFUL))
      {
        return(Error); 
      }
      //and erase all remaining blocks
      if(Error = PFlash_EraseBlock(0x7A0000UL))
      {
        return(Error);  
      }
      if(Error = PFlash_EraseBlock(0x780000UL))
      {
        return(Error); 
      }
      return(noErr);
    
    default:
      return(UnknownPartID);
  }      
}

/*=======================================================================
 *函数名:      static UINT8 ProgramFlash_Upgrade(void)
 *功能:        代码写入Flash
 *参数:        无       
 *返回：       无
 *说明：       在代码的升级过程中将存储区(0x70_0000~0x77_0000)的代码写入源代码区，
               此处为代码编程
========================================================================*/
static UINT8 ProgramFlash_Upgrade(void)
{
  SRecDataRec ProgSRec;
  UINT8 Error = noErr; 
  
  for(;;)
  {
    //go get an S-Record, return if there was an error
    if (Error = RcvSRecord_Upgrade(&ProgSRec)) 
    {
      return(0x81);
    }
    
    __asm nop;
    SrcdAlign4W(&ProgSRec);
    __asm nop;
    
    if (ProgSRec.RecType == EndRec) // S7, S* or S9 record?
    {
      break;                       // yes. return
    }
      
    else if (ProgSRec.RecType == HeaderRec) //S0 record?
      continue;             //yes. just ignore it
      
    else                    //a data record was received
    {
      if (ProgSRec.LoadAddr == RESET_VEC_SRC)//复位向量的地址
      {
        if(Error = PFlash_Program(RESET_VEC_DST, (UINT16 *)&ProgSRec.Data[24]))
        {
          return(0x82);
        }
      }   
      else
      {
        switch (PARTID)
        {       
          //S12XE 1024k flash
          case MASK_0M48H:  //Falling to next case
          case MASK_1M48H:  //Falling to next case
          case MASK_2M48H:  //Falling to next case
          case MASK_3M48H:  //Falling to next case
          case MASK_4M48H_5M48H:
          case MASK_0N35H_1N35H:
            if(Error = PFlash_Program(ProgSRec.LoadAddr, (UINT16 *)&ProgSRec.Data))
              return(0x83);
        
            if(Error = PFlash_Program(ProgSRec.LoadAddr+8, (UINT16 *)&ProgSRec.Data[8]))
              return(0x83);
      
            if(Error = PFlash_Program(ProgSRec.LoadAddr+16, (UINT16 *)&ProgSRec.Data[16]))
              return(0x83);
      
            if(Error = PFlash_Program(ProgSRec.LoadAddr+24, (UINT16 *)&ProgSRec.Data[24])) 
              return(0x83);
              break;
              
          default:
            return(UnknownPartID);
        } 
      }
      CAN_SendByte(0x02);
    }
  }
  return(Error);
}

/*=======================================================================
 *函数名:      static UINT8 ProgramFlash_Store(void)
 *功能:        远程将代码存储到pFlash(0x70_0000~0x77_0000)中
 *参数:        无       
 *返回：       无
 *说明：       远程将代码存储到pFlash(0x70_0000~0x77_0000)中
               此处为代码存储
========================================================================*/
static RecData_T RecData;
static UINT8 ProgramFlash_Store(void)
{
  SRecDataRec ProgSRec;
  UINT8 Error = noErr; 
  UINT32 LoadAddr = PFLASH_START; 
  RecData_T*dataptr = &RecData;
  UINT8 addrcount,k,j;
  UINT16 *Address=NULL;
  
  Data_INIT(dataptr);//初始化接收缓存
  for(;;)
  {
    if (Error = RcvSRecord_Store(&ProgSRec,dataptr)) 
    {
      return(0x11);
    } 
      
    if (ProgSRec.RecType == HeaderRec) //S0 record
    {
      continue;
    }
    else                    
    {
      addrcount = (dataptr->Lenth)%8;         //计算每行S19文件中的字符数量(每次存储8字节)      
      Address = (UINT16 *)(&dataptr->DATA[0]);//每行S19字符的首地址
      switch (PARTID)
      {       
        case MASK_0M48H:  //Falling to next case
        case MASK_1M48H:  //Falling to next case
        case MASK_2M48H:  //Falling to next case
        case MASK_3M48H:  //Falling to next case
        case MASK_4M48H_5M48H:
        case MASK_0N35H_1N35H:
            if(addrcount != 0)//每行S19的字节数非8个倍数(每次发送存储8字节)
            {
               for(k=0; k<8-addrcount; k++)//将少于8字节的包填满0xFF
               {
                 if(Data_IN(dataptr,0xFF) == 1)//填0xFF
                 {
                    return (0x12); 
                 }
               }
               addrcount = (dataptr->Lenth-1)/8;//计算每行S19数据发送的次数(每次8字节)
               for(k=0; k<addrcount+1; k++)
               {
                  if(Error = PFlash_Program(LoadAddr, Address)) //将Address中的数据存储到flash地址(LoadAddr)中
                  {
                    return(0x13); 
                  }
                  Address =  Address + 4;  //每次存储2个字节(Address为UINT16的指针)
                  LoadAddr = LoadAddr + 8; //flash地址每次加8
                  for(j=0; j<8; j++)
                  {
                    Data_OUT(dataptr);     //每次取出8字节
                  }
               }
            }
            else//每行S19的字节数是8个倍数
            {
               addrcount = (dataptr->Lenth-1)/8; //发送次数
               for(k=0; k<addrcount+1; k++)
               {
                  if(Error = PFlash_Program(LoadAddr, Address))
                  {
                    return(0x14); 
                  }
                  Address =  Address + 4;   //每次存储2个字节(Address为UINT16的指针)
                  LoadAddr = LoadAddr + 8;  //flash地址每次加8 
                  for(j=0; j<8; j++)
                  {
                    Data_OUT(dataptr);      //每次取出8字节
                  }               
               }
            }
            //Data_INIT(dataptr); //每行S19文件发送完之后，清除缓存(有隐患,中断中部分数据被清除)
            break;
              
        default:
          return(UnknownPartID);
         
      }
      if (ProgSRec.RecType == EndRec)         
      {
         return (noErr);
      }
      CAN_SendByte(0x01);
    }
  }
  return(Error);
}

/*=======================================================================
 *函数名:      static void CopyCodeToRAM(void)
 *功能:        对flash进行读写的程序需要将其复制至RAM中进行运行
 *参数:        无       
 *返回：       无
 *说明：       复制至RAM的起始地址必须与Prm文件中的relocate_to地址一致
               运用了relocate_to就必须使用此段代码,否则系统崩溃
========================================================================*/
static void CopyCodeToRAM(void)           
{
  UINT8 *Src;
  UINT8 *Dst;
  UINT16  SegSize;
  UINT16  x;
  
  //RAM code resides in Flash from 0xfc00 - 0xfeff
  Src = (UINT8 *)__SEG_START_REF(RAM_CODE);
  //code will be copied to RAM at address 0x3C00
  Dst = (UINT8 *)0x3C00;//一定要和Prm文件中的relocate_to地址一致
  SegSize = (UINT16)__SEG_SIZE_REF(RAM_CODE);
  
  for (x = 0; x < SegSize; x++)   //just copy a byte at a time
  {
    *Dst++ = *Src++; 
  }
}

/*=======================================================================
 *函数名:      static void InterruptModuleSetup(void)
 *功能:        设置BootLoader程序中的中断基向量地址
 *参数:        无       
 *返回：       无
 *说明：       此基地址为BootLoader的中断向量基地址
 
========================================================================*/
static void InterruptModuleSetup(void)
{
  IVBR = (CPU12IVBR >> 8);
}

/*=======================================================================
 *函数名:      static void PLL_Init(void)
 *功能:        初始化锁相环
 *参数:        无       
 *返回：       无
 *说明：       晶振频率:16MHz
               Bus频率:32MHz 
========================================================================*/
static void PLL_Init(void) 
{
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
    while(!(CRGFLG&0x08)); //PLLCLK锁定
    CLKSEL |= 0x80;        //设置PLLCLK为系统时钟
}


/*=======================================================================
 *函数名:      static void EraseFlash_Status(void)
 *功能:        执行擦除存储区还是擦除代码区 
 *参数:        data：CAN发送数据
               fun：回调函数        
 *返回：       无
 *说明：       执行擦除存储区还是擦除代码区                 
========================================================================*/
static void EraseFlash_Status(UINT8 data, UINT8 (*fun)(void))
{
   UINT8 flashsta=0;
   UINT8 i,j;
   flashsta = fun();
   
   if(flashsta != NoErr)//如果擦除有错,发送出错信息
   {
     while(1)
     {
       CAN_SendByte(data);
       for(i=0; i<100; i++)
       {
         for(j=0; j<200; j++);
       }
     }
   }
}
/*=======================================================================
 *函数名:      static Boot_Delay (void)
 *功能:        Boot的延时函数
 *参数:        ts:延时的节拍数       
 *返回：       无
 *说明：       晶振频率:16MHz
               Bus频率:32MHz 
========================================================================*/
static
void Boot_Delay(unsigned int ts)
{
   unsigned int i,j;
   for(i=0; i<500; i++)
   {
      for(j=0; j<ts; j++);
   }
}

/*=======================================================================
 *函数名:      static void PLL_Init(void)
 *功能:        初始化锁相环
 *参数:        无       
 *返回：       无
 *说明：       晶振频率:16MHz
               Bus频率:32MHz 
========================================================================*/
UINT16 QQ[5]={0,0,0,0,0};//调试标记
UINT32 Flashstart_Add;
//UINT16 dd;
void main(void) 
{
  UINT8 caseflag=0,i,j;
  UINT8 flashsta=0,state;
  //UINT16 datast;
  memset(QQ, 0, 5*sizeof(UINT16));

  DisableInterrupts;
  /*主板需要控制CAN的收发器开关(有休眠功能)
  DDRS_DDRS2 = 1;//使能CAN收发器
  PTS_PTS2 = 0; 
  
  DDRK_DDRK5 = 0;//控制CAN的电源 
  PORTK_PK5 = 1;*/    
  
  Flashstart_Add = PFLASH_START; //boot的全局变量必须在初始化中清零,否则会出错
  
  //单片机初始化
  PLL_Init(); //BusClk:32MHz  
  INIT_CAN2();//CAN3初始化,波特率:500kbps
  
  CopyCodeToRAM();//将对flash读写的代码复制至RAM中运行
  InterruptModuleSetup();//设置Boot的中断向量基地址
  //对Pflash和Dflash的初始化
  Init_Eeprom(&FlashErr); //设定flash的时钟
  //部分分区(单片机的首次使用时最好进行全分区和擦除flash所有的块)
  FlashErr = LaunchFlashCommand(2 ,PARTITION_D_FLASH, 0, DFPART, EEE_RAM, 0, 0, 0, 0, 0); 
  ErrorCheck(FlashErr,(accerr|fpviol|mgstat1|mgstat0), (erserif|pgmerif|epviolif|ersvif1|ersvif0|dfdif|sfdif));
  if(FlashErr.ErrCode == NoErr) 
  {
    FlashErr = LaunchFlashCommand(0 ,EEPROM_QUERY, 0, 0, 0, 0, 0, 0, 0, 0); 
    ErrorCheck(FlashErr,(accerr|fpviol|mgstat1|mgstat0), (erserif|pgmerif|epviolif|ersvif1|ersvif0|dfdif|sfdif));
    ChecPartErr(&FlashErr);   
  }  
  if(FlashErr.ErrCode == NoErr) //无错误使能EEPROM
  {
    FlashErr = LaunchFlashCommand(0 ,ENABLE_EEPROM_EMULATION, 0, 0, 0, 0, 0, 0, 0, 0);//BuffRam模拟EEprom(EEE)
    ErrorCheck(FlashErr, (accerr|fpviol|mgstat1|mgstat0), (erserif|pgmerif|epviolif|ersvif1|ersvif0|dfdif|sfdif));    
  }
  CAN_SendByte(0x55);
  //EEPROM的值初始化
  boot = (UINT16*)0x0C00;
  DataIVBR = (UINT16*)0x0C02;
  (*boot) = 0x66;
  /*
  if((*DataIVBR) == 0x77)//需要再应用程序中增加一个地址变量进入BOOT或是升级被打断后进入原先的AAP中
  {
    (*DataIVBR) = 0x11;
  }
  else if((*DataIVBR) == 0x55)//若在升级过程中掉线,那么程序重新启动后直接进行重新升级(不需要加载)
  {
    if((*(UINT16*)0xC000) != 0xFFFF) //如果APP存储区有数据则直接进行升级
    {
      caseflag = 0;   //caseflag == 1;则不需要加载S19文件直接进行升级
    }
  }*/
  EnableInterrupts;       //enable interrupts for the CAN

  for(;;)
  { 
    switch(caseflag)
    {
      case 0:
        QQ[0]++;
        CAN_SendByte(0xC0);   //进入BOOT程序并开始进行擦除
        EraseFlash_Status(0x10, EraseFlash_Store); //擦除存储区flash(此处掉电进入boot程序，需要重新更新S19文件)
        CAN_SendByte(0xC1);  //Flash擦除完毕
        QQ[1]++;
        
        CAN2RIER_RXFIE = 1;   //开启CAN2接收中断  
        if(state = ProgramFlash_Store())
        {
           while(state != 0)
           {
             CAN_SendByte(state);//代码存储错误,需要重启或重新更新Boot程序
             for(i=0; i<250; i++)
             {
               for(j=0; j<250; j++);
             }
           }
        }
        CAN2RIER_RXFIE = 0;   //代码存储完毕则关闭中断
        
        CAN_SendByte(0xC2);
        
        RxIn = RxOut= 0;      //需要清除接收缓存,(因为TXT文件结尾含有2个结尾符,需要将其清掉)
        RxBAvail = RxBufSize; //the receive buffer is empty
        
        (*DataIVBR) = 0x55;   //代码存储完整标记位,或可升级标记位
        caseflag = 1;
        QQ[2]++;
      break;
      
      case 1:
        CAN_SendByte(0xC3);   //进入程序升级
        EraseFlash_Status(0x80, EraseFlash_Upgrade);//擦除代码段flash
        CAN_SendByte(0xC4);   //代码下载完毕
        QQ[3]++;
        
        init_PIT0();//中断频率:10ms(此处中断未开启,由升级过程中开启)
        if(state = ProgramFlash_Upgrade())
        {
           while(state != 0)
           {
             CAN_SendByte(state);//代码升级错误,需要重启或重新更新Boot程序
             for(i=0; i<250; i++)
             {
               for(j=0; j<250; j++);
             }
           }
        }
        PITINTE_PINTE0 = 0;   //关断中断避免继续读取Flash进行编程
        
        (*boot) = 0;          //只有更新完之后才置回标记位
        (*DataIVBR) = 0x66;   //此处标记位可引导Boot进入APP程序
        
        CAN_SendByte(0xC5);   //代码升级完毕
        
        caseflag = 2;
        QQ[4]++;
        //while(1);             //设计升级完成后需要重新启动
        
        Boot_Delay(1000);
        for(i=0;i<250;i++)
        {
          for(i=0;i<250;i++);  
        }
        COPCTL = 0x01;       //重新启动     
        ARMCOP = 0x00;
      break;
      
      default:
        CAN_SendByte(0xC6);//代码更新故障
      break;
    }  
  }   
}
/******************************************************************************/
#pragma CODE_SEG RAM_CODE
interrupt void isrPIT0()//PIT0定时中断函数,周期为5ms,时间再短会出错(可以大于5ms)
{  
   UINT8 i;
   UINT16 data;
   if (PITTF_PTF0 ==1) 
   {   
     PITTF_PTF0 = 1;
   } 
   for(i=0; i<16; i++)//此处最大为16个个循环,改大会导致队列存储溢出
   {
     data = PFlash_ReadWord(Flashstart_Add);
     if (RxBAvail != 0)            //if there are bytes available in the Rx buffer
     {
        RxBAvail--;
        RxBuff[RxIn++] = (UINT8)(data>>8);//高字节
        RxBAvail--;
        RxBuff[RxIn++] = (UINT8)data;
        if (RxIn == RxBufSize)      //低字节
        { 
          RxIn = 0;               //yes. wrap around to the start
        }                 
     }
     Flashstart_Add = Flashstart_Add + 2; 
   }
}
#pragma CODE_SEG DEFAULT
