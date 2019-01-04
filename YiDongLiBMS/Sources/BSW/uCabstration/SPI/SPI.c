/*=======================================================================
 *Subsystem:   ���
 *File:        SPI.C
 *Author:      Wenming
 *Description: ͨ�ţ�
               �ӿڣ�
               �����ʣ�
 ========================================================================
 * History:    �޸���ʷ��¼�б�
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#include "SPI.h"

/*=======================================================================
 *������:      SPI1_Init(void) 
 *����:        SPI1��ʼ��
 *����:        ��       
 *���أ�       ��
 *˵����       ͨ�Ų�����:1Mhz
               SPI��ʼ��������ҪƬѡ
========================================================================*/
uint8 SPI1_Init(void) 
{
   SPI1CR1=0b01011100;
   SPI1CR2=0x00;
   SPI1BR=0x71; //1MHz������
 
   return 0;
}
                           
/*=======================================================================
 *������:      SPI1_Write(uint8) 
 *����:        SPI1д���ݺ���
 *����:        data��Ҫд��8λ����       
 *���أ�       ��
 *˵����       SPI1д���ݺ���
========================================================================*/
void SPI1_Write(uint8 data) 
{
  while(!SPI1SR_SPTEF);                // ÿ��ֻ�ܷ���һ���ֽڵ����ݣ�
  SPI1DRL = data;
}

/*=======================================================================
 *������:      SPI1_read(void) 
 *����:        SPI1�����ݺ���
 *����:        ��       
 *���أ�       ��
 *˵����       SPI1�����ݺ���
========================================================================*/
uint8 SPI1_read(void) 
{
  uint8 temp2,returndata;
 
  while(!SPI1SR_SPIF) 
  temp2 = SPI1SR;   
  
  returndata = SPI1DRL;           //��ȡ����
  return(returndata); 
}


    
 