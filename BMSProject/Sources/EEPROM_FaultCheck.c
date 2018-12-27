#include  "EEPROM.h"

uint8 EEPORM_FaultCheck(void)
{ 
  volatile  ErrType FlashErr;

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
  return(FlashErr.ErrCode);
}