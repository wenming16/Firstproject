#ifndef _CAN_H
#define _CAN_H


#define ID_BMU 	             0x0001  //ID主板为01,子板为02以此类推
#define ID_CSSU1 	           0x0002
#define ID_CSSU2 	           0x0003
#define ID_CSSU3 	           0x0004

#define ID_BootState         0x01FF  //BOOT更新状态返回值

#define RxBufSize 80

 
typedef struct                    //发送报文的结构体
{
    unsigned long id;             //ID号
    Bool RTR;                     //是否为远程贞
    unsigned char data[8];        //存放数据数组
    unsigned char len;            //can发送的数据长度为8  
    unsigned char prty;           //CAN通信传输优先级设定
} CAN_MSG;

extern CAN_MSG can_send;

extern UINT8 RxIn;               //next available location in the Rx queue
extern UINT8 RxOut;              //next character to be removed from the Rx queue

extern UINT8 RxBuff[RxBufSize];  //receive queue
extern volatile UINT8 RxBAvail;  //number of bytes left in the Rx queue

#pragma CODE_SEG RAM_CODE
extern  interrupt void CAN3RxISR(void); 
#pragma CODE_SEG DEFAULT

extern INT8 getchar(void);
extern void INIT_CAN2(void); 
extern Bool MSCAN2GetMsg(CAN_MSG *msg);
extern Bool MSCAN2SendMsg(CAN_MSG msg);
extern void CAN_SendByte(UINT8 data);
#endif