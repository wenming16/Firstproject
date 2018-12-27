#ifndef BMS_TO_UP_MONITOR
#define BMS_TO_UP_MONITOR


/********BMS to Up Monitor information*******/

#define    BMS_Send_Information1   0x18FF9700
#define    BMS_Send_Information2   0x18FF9710
#define    BMS_Send_Information3   0x18FF9800
#define    BMS_Send_Information4   0x18FF9810
#define    BMS_Send_Information5   0x18ff9900
#define    BMS_Send_Information6   0x19FF9900




;


extern CANFRAME BMS_to_Upmonitor;


void Bms_to_Up_Monitor(void);
//uint8 UpMonitor_to_Bms(uint32 ID);


#endif