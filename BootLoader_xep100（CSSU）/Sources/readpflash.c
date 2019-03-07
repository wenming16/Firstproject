#define PFLASH_START               0x00700000

#define PFLASH_PAGE_SIZE           0x4000
#define PFLASH_PAGE_START          0xC0
#define PFLASH_PAGE_WINDOW_START   0x8000
#define ReadWord(address)          ((UINT16*)(*(volatile UINT16 *__near)(address))

UINT16 PFlash_ReadWord (UINT16 destination) //读取2个字节
{
  UINT8   lastepage;    //用于存储PPAGE的值
  UINT8   p_page;       //用于计算PPAGE的值
  UINT16  data;         //读取出的数据
  
  lastepage = PPAGE;   //保存EPAGE的值
  p_page = (UINT8)(PFLASH_PAGE_START + (destination >> 14));   //计算PPAGE
  PPAGE = p_page;                                              //给PPAGE赋值

  data = ReadWord((destination & (PFLASH_PAGE_SIZE - 1)) + PFLASH_PAGE_WINDOW_START);  //读取页面窗口中的数据
  EPAGE= lastepage;       //恢复PPAGE的值
  return(data);
}


UINT16 PFlash_ReadAll (void)
{
  
}