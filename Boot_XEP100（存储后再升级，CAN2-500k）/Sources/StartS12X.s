;********************************************************************************
;* Freescale Semiconductor Inc.
;* (c) Copyright 2004-2005 Freescale Semiconductor, Inc.
;* ALL RIGHTS RESERVED.
;********************************************************************************
;Services performed by FREESCALE in this matter are performed AS IS and without 
;any warranty. CUSTOMER retains the final decision relative to the total design 
;and functionality of the end product. FREESCALE neither guarantees nor will be 
;held liable by CUSTOMER for the success of this project.
;FREESCALE DISCLAIMS ALL WARRANTIES, EXPRESSED, IMPLIED OR STATUTORY INCLUDING, 
;BUT NOT LIMITED TO, IMPLIED WARRANTY OF MERCHANTABILITY OR FITNESS FOR 
;A PARTICULAR PURPOSE ON ANY HARDWARE, SOFTWARE OR ADVISE SUPPLIED TO THE PROJECT
;BY FREESCALE, AND OR NAY PRODUCT RESULTING FROM FREESCALE SERVICES. IN NO EVENT
;SHALL FREESCALE BE LIABLE FOR INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF 
;THIS AGREEMENT.
;
;CUSTOMER agrees to hold FREESCALE harmless against any and all claims demands or 
;actions by anyone on account of any damage, or injury, whether commercial, 
;contractual, or tortuous, rising directly or indirectly as a result of an advise
;or assistance supplied CUSTOMER in connection with product, services or goods 
;supplied under this Agreement.
;********************************************************************************
;* File      StartS12X.s
;* Owner     b01802
;* Version   1.0   
;* Date      Dec-02-2010
;* Classification   General Business Information
;* Brief     Startup file
;********************************************************************************
;* Detailed Description:
;*
;*    Tested on: DEMO9S12XSFAME, EVB9S12XEP100
;*    
;*    The bootloder handles all reset vectors. This startup file makes a decision
;*    whether the bootloader or user application is executed. 
;********************************************************************************
;Revision History:
;Version   Date          Author    Description of Changes
;1.0       Dec-02-2010  b01802    Initial version
;********************************************************************************
;
  xref  main            ;xref表示本子程序的定义在其他文件中
;
  xdef  _BootStart      ;xdef表示在本文件中定义了可以被其他文件中的程序调用的子程序
;

AppResetVect:   equ     $dffe ;equ，把该地址赋给AppResetVect标号 here is stored reset vector of user application
StackTop:       equ     $3900 ;栈顶地址
BootCtrWord:    equ     $0C00 ;EEPROM的0C00位置用于储存BootLoader控制字
APPCtrWord:     equ     $0C02 ;确认下载地址
APPStartUp:     equ     $C000 ;APP的Startup地址

_BootStart:
  
  ldd   APPStartUp      
  cpd   #$ffff            
  beq   GoBoot  
  ;程序下载时BDM会擦除PFLASH地址中的全部值,若APP中无程序则进入Boot程序中
  ;此处处于针对当BOOT第2次下载时,地址0x0C02上次存储的值为0x11，此时下载时会进入无程序的APP中
  ; -----------------------------------------------------------------------------
  
  ldd   APPCtrWord      
  cpd   #$11            
  beq   GoApp           
  ;说明:首次下载BOOT时,此地之中的值为0XFF,Pfalsh中无APP程序,若升级不成功,则程序一直进入BOOT中
  ;若从APP程序中进入BOOT状态时,应用程序中将0x0C02地址中的值改为0x77,0x0C00地址中的值改为0x66,
  ;此时则不进入APP中,进入下条指令(即:进入BOOT中), 在BOOT程序加载不成功则0x0C02中的值一直为0x11,
  ;那么下一次重新启动进入的是原来的APP中,若加载成功那么0x0C02地址中的值改为0x55,下次重新启动时,
  ;不需要加载APP程序可以直接进行升级(即进入case2中).
  ; -----------------------------------------------------------------------------
  
  ldd   BootCtrWord
  cpd   #$66
  beq   GoBoot         
  ;说明:如果应用程序中需要进入BOOT中,则在应用程序中将0x0C00的字节改为0x66和将0x0C02中的值改为0x77
  ;那么重新启动后就可以进入BOOT的startup中              
  ; -----------------------------------------------------------------------------
  
  ldd   AppResetVect    
  cpd   #$ffff          
  beq   GoBoot          
  ;说明:单片机首次使用时Flash擦除后的原始值为0xFF;此种情况则今天BOOT的程序中                     
  ; -----------------------------------------------------------------------------
 
  ldd   APPCtrWord      
  cpd   #$66            
  beq   GoApp           
  ;说明:进入此指令时,说明加载和升级已经成功则进入新的APP程序中
  ; -----------------------------------------------------------------------------
  
GoBoot:
  lds  #StackTop
  jmp  main                        ;then start the bootloader
  
GoApp:  
  ldx   AppResetVect    ;将App地址加载到累加器X中
  jmp    0,x            ;jump to the application
  
;********************************************************************************

