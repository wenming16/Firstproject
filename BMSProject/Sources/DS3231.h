/*=======================================================================
 *Subsystem:   Âã»ú
 *File:        DS3231.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef _DS3231_H_
#define _DS3231_H_  

  #include"TypeDefinition.h"

  typedef struct
  {
    uint8 IIC_Read_Hour;
    uint8 IIC_Read_Day;
    uint8 IIC_Read_Month;
    uint8 IIC_Read_Year;
    uint8 IIC_Read_Minute;
    //uint8 IIC_Read_Second;
  }Read_IIC_Time_T;
  extern Read_IIC_Time_T Read_IIC_Time;

  uint8 BCD2HEX(uint8 val);
  uint8 HEX2BCD(uint8 val);
  void DS3231SN_INIT(uint8 year, uint8 month, uint8 week, uint8 day, uint8 hour, uint8 min);
  uint8 DS3231_Read_Minute(void);
  uint8 DS3231_Read_Hour(void);
  uint8 DS3231_Read_Day(void);
  uint8 DS3231_Read_Month(void); 
  uint8 DS3231_Read_Year(void);
  void DS3231_Read_Time(void);

#endif 