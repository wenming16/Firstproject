#ifndef _J1939_H_
#define _J1939_H_ 

  /*--------------------定义常量------------------*/   
  #define NUMBER_PDU_BUFFERS       8        //PDU报文最大字节数,一般为8
  #define NUMBER_TRANS_RX_BUFFERS  128      //网络层大于8字节后最大接收字节数，可按BMS需求
  #define NUMBER_TRANS_TX_BUFFERS  128      //网络层大于8字节后最大发送字节数，可按BMS需求

  #define IN_BUFFER_SIZE 20        //接收缓存区最大数据报文量
  #define OUT_BUFFER_SIZE 20       //发送缓存区最大数据报文量

  #define CAN_MAX_BYTE_COUNT 8

  #define NOTPRIMED 0              //可以取出数据
  #define PRIMED    1
  
  

  /*-----------------控制器节点地址宏定义---------------*/
  #define NODEADDR         0xF4              //本控制器节点地址
  #define GLOBADDR         0xFF              //全局节点地址
  #define NULLADDR         0xFE
  #define VCUADDRESS       0x27              //VCU节点地址
  #define CHARGEADDRESS    0x56              //充电节点地址
  #define CSSUADDRESS      0x97              //子板节点地址
  #define INSULATIONADD    0x1A              //绝缘板节点地址
  

  /*-----------------传输层数据接收状态---------------*/
  #define WAIT_FOR_MESSAGE               0         // State Zero: Waiting for a BAM or RTS data packet to arrive
  #define INIT_REASSEMBLE_STRUCTURE      1         // State One: Message reassemble structure and message timer are initialized.
  #define CHECK_PACKET                   2         // State Two: The incoming message is for this node.
  #define SEND_ABORT                     3         // State Three:  No more buffers to reassemble incoming message, Abort is sent.
  #define SEND_CTS_WITH_COUNT            4         // State Four: Buffers are available to reassemble incoming message, send CTS back to sender to indicate number of packets between next CTS (TRANSPORT_PACKET_COUNT). 
  #define WAIT_FOR_DATA                  5         // State Five: Waiting for DATA type packet from source.
  #define CHECK_TIMER                    6         // State Six:  Message timer is checked for expiration.
  #define RESET_REASSEMBLY_STRUCTURE     7         // State Seven: Message reassemble structure is reset.
  #define CHECK_DATA_PACKET              8         // State Eight: A DATA type packet has arrived and checked.
  #define SAVE_DATA                      9         // State Nine: The data is saved in the message reassemble buffer
  #define SEND_EOM                       10        // State Ten:  EOM message is sent.
  #define SEND_CTS                       11        // State Eleven: CTS is sent to trigger more data from the sender. 
  #define FILL_USER_MESSAGE              12        // State Twelve: The J1939_RX_MESSAGE_T data structure is filled for user application.


  /*-----------------传输层数据发送状态---------------*/
  #define SEND_TP_CM                     0         //发送TP_CM命令来进行发送连接,发送到目的地发送RTS;发送广播发送BAM 
  #define WAIT_FOR_CTS                   1         //等待接收方CTS ,确认接下里发送的字节报文数量
  #define SEND_TP_DT                     2         //发送数据报文
  #define CHECK_TIMER1                   3         //检测时间间隔是否超时
  #define SEND_ABORT1                    4         //发送放弃连接报文
  #define WAIT_FOR_EOM                   5         //等待结束连接报文
  #define WAIT_AFTER_END                 6         //
  #define CHECK_TIMER2                   7  
  
  /*-----------------传输层传输协议宏定义---------------*/
  #define TP_CM                          0x00EC00  // 0x00EC00=60416,传输连接PGN
  #define TP_CM_RTS                      16
  #define TP_CM_CTS                      17         
  #define TP_CM_END_OF_MSG_ACK           19
  #define TP_CM_CONN_ABORT               255
  #define TP_CM_BAM                      32
  #define TP_DT                          0x00EB00  // 0x00EB00=60160
  #define TP_NONE                        0          
  
  /*-----------------数据传输层等待超时定义---------------*/  
  #define TICK                           30        // 判断多字节接收是否超时，20ms  
  
  
  /*--------------------队列结构体-------------------*/ 
  
  typedef struct 
  {
    CANFRAME *pBase;
    uint8 Front;
    uint8 Rear;
    uint8 BufferLen;
    uint8 DataLen;
  }QUEUE_T, *pQUEUE_T;
  
  
   
  /*--------------------大于8字节的数据结构体-------------------*/ 
  typedef struct//底层到传输层
  {
   PGN_T PGN;
   vuint8 data[NUMBER_TRANS_RX_BUFFERS]; //128
   vuint16 byte_count;
   vuint8 source_addr;
   vuint8 dest_addr;
  } J1939_RX_NETWork_T; //传输层到网络层的数据接口（大于8字节），无传输协议
  //转化成PDU格式进行发送
  typedef struct
  {
   PGN_T     PGN;
   vuint8    data[8];  
   vuint16   byte_count;
   vuint8    priority;
   vuint8    dest_addr;
   vint8     status;
  }J1939_TX_PDU_T;   //创建发送PDU格式，数据链层到传输层的数据转化接口
  
  
  //数据接收
  typedef struct
  {
   PGN_T  PGN;
   vuint8 data[8];  
   vuint8 byte_count;
   vuint8 source_addr;
   vuint8 dest_addr;
  }J1939_RX_PDU_T;   //创建接收PDU格式，数据链层到传输层的数据转化接口
  
  typedef struct
  {
   uint8  CTS_Flag;
   PGN_T  PGN;
   vuint8 data[8];  
   vuint8 byte_count;
   vuint8 source_addr;
   vuint8 dest_addr;
  }J1939_CTS_T;   //接收CTS结构体
  
  typedef struct//包括CTS和ENDofMsgACK帧
  {
   uint8  ENDofMsgACK_Flag;
   PGN_T  PGN;
   vuint8 data[8];  
   vuint8 byte_count;
   vuint8 source_addr;
   vuint8 dest_addr;
  }J1939_CENDofMsgACK_T;   //接收ENDofMsgACK结构体
  
  //传输到应用层
  typedef struct
  {
   PGN_T  PGN;
   vuint8 status;
   vuint8 packet_number;
   vuint8 total_packet_number;
   vuint8 byte_count;
   vuint8 timer_counter;
   vuint8 source_addr;
   vuint8 dest_addr;
   vuint8 TP;
  }J1939_RX_STATE_MACHINE_T;     //传输层数据传输处理结构形式（大于8字节），传输协议结构数据
  
  /*-----------------数据链层队列结构体---------------*/

  
  
  /*-----------------传输层数据传输协议内部结构体---------------*/
   //传输层数据传输处理结构形式（大于8字节），传输协议结构数据



  typedef struct
  {
     PGN_T  PGN;
     vuint8 status;
     vuint8 packet_number;
     vuint8 total_packet_number;
     vuint8 total_rxpacket_number;
     vuint8 byte_count;
     vuint8 timer_counter;
     vuint16 timer1_counter;
     vuint8 source_addr;
     vuint8 dest_addr;
     vuint8 TP;
  }J1939_TX_STATE_MACHINE_T;     //传输层数据传输处理结构形式（大于8字节），传输协议结构数据
   
   
  /*-----------------传输层到应用层数据传输结构体---------------*/
 
  typedef struct
  {
     PGN_T PGN;
     vuint8 data[NUMBER_TRANS_TX_BUFFERS]; //128
     vuint16 byte_count;
     vuint8 priority;
     vuint8 dest_addr;
     vint8 status;
  }J1939_TX_MESSAGE_T;  //传输层到网络层的数据接口（小于8字节），无传输协议  

#endif
