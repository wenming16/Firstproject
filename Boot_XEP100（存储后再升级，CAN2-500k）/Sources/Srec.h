/*******************************************************************************
* Freescale Semiconductor Inc.
* (c) Copyright 2004-2005 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
********************************************************************************
Services performed by FREESCALE in this matter are performed AS IS and without 
any warranty. CUSTOMER retains the final decision relative to the total design 
and functionality of the end product. FREESCALE neither guarantees nor will be 
held liable by CUSTOMER for the success of this project.
FREESCALE DISCLAIMS ALL WARRANTIES, EXPRESSED, IMPLIED OR STATUTORY INCLUDING, 
BUT NOT LIMITED TO, IMPLIED WARRANTY OF MERCHANTABILITY OR FITNESS FOR 
A PARTICULAR PURPOSE ON ANY HARDWARE, SOFTWARE ORE ADVISE SUPPLIED TO THE PROJECT
BY FREESCALE, AND OR NAY PRODUCT RESULTING FROM FREESCALE SERVICES. IN NO EVENT
SHALL FREESCALE BE LIABLE FOR INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF 
THIS AGREEMENT.

CUSTOMER agrees to hold FREESCALE harmless against any and all claims demands or 
actions by anyone on account of any damage, or injury, whether commercial, 
contractual, or tortuous, rising directly or indirectly as a result of an advise
or assistance supplied CUSTOMER in connection with product, services or goods 
supplied under this Agreement.
********************************************************************************
* File      Srec.h
* Owner     b01802
* Version   1.0 
* Date      Dec-02-2010
* Classification   General Business Information
* Brief     Srecord utilities
********************************************************************************
* Detailed Description:
********************************************************************************
Revision History:
Version  Date          Author    Description of Changes
1.0      Dec-02-2010   b01802    Initial version
*******************************************************************************/
#ifndef _UTILITIES_H 
#define _UTILITIES_H

#define DataRec 1
#define EndRec 2
#define HeaderRec 3
#define DtaLenth  120

#define MaxSRecLen 32          //S记录最大长度

typedef struct {
                UINT8 RecType;          //保存接收到S记录后的字符
                UINT8 NumBytes;
                UINT32 LoadAddr;
                UINT8 Data[MaxSRecLen];
               } SRecDataRec;
               
typedef struct 
{
  UINT8 DATA[DtaLenth];
  UINT8 Lenth;
} RecData_T;
               
extern UINT8 ww[2];
extern UINT8 DD[2];
//extern UINT8 y,o;   
//extern UINT32 ddd;            

UINT8 RcvSRecord_Upgrade(SRecDataRec *SRecData);
UINT8 RcvSRecord_Store(SRecDataRec *SRecData,RecData_T*dataptr);
void SrcdAlign4W(SRecDataRec *srcd);
//void SrcdAlign4W_2(SRecDataRec *srcd);
void Data_INIT(RecData_T*ptr);
UINT8 Data_IN(RecData_T*ptr,UINT8 data);
UINT8 Data_OUT(RecData_T*ptr);


#endif
/******************************************************************************/
