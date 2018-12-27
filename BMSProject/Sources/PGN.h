#ifndef PGN_H
#define PGN_H
 
 
  
  //充电PGN宏定义  
  #define RES    		 0x000000  // 预留报文
  #define CRM    		 0x000100  // 充电机辨识报文
  #define BRM     	 0x000200  // BMS与车辆辨识报文

  #define BCP			   0x000600  // 动力蓄电池充电参数
  #define CTS			   0x000700  // 充电机发送时间同步信息
  #define CML     	 0x000800  // 充电机最大输出能力
  #define BRO   	   0x000900  // 电池充电准备就绪状态
  #define CRO   		 0x000A00  // 充电机输出准备就绪状态

  #define BCL			   0x001000  // 电池充电需求
  #define BCS		     0x001100  // 电池充电总状态
  #define CCS			   0x001200  // 充电机充电状态          
  #define BSM		     0x001300  // 动力蓄电池状态信息
  #define BMV		     0x001500  // 单体动力蓄电池电压
  #define BMT			   0x001600  // 动力蓄电池温度
  #define BSP				 0x001700  // 动力蓄电池预留报文
  #define BST				 0x001900  // BMS中止充电
  #define CST   		 0x001A00  // 充电机中止充电

  #define BSD			   0x001C00  // BMS统计数据
  #define CSD			   0x001D00  // 充电机统计数据

  #define BEM     	 0x001E00  // BMS错误报文
  #define CEM			   0x001F00  // 充电机错误报文
  
  #define DM1        0x002000  //当前故障码
  #define DM2        0x002100  //历史故障码
  #define DM3        0x002200  //诊断准备就绪
  #define DM4        0x002300  //当前故障码的清除/复位
  #define DM5        0x002400  //历史故障码的清除/复位
  #define DM6        0x002500  //停帧参数

  #define CHM        0x002600  //充电握手
  #define BHM        0x002700  //车辆握手

  #define PGN_NUM    29        //PGN过滤中PGN的数目


#endif
