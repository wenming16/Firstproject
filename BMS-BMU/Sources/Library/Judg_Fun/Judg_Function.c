/*=======================================================================
 *Subsystem:   ���
 *File:        Judg_Function.C
 *Author:      WenYh
 *Description: 
 ========================================================================
 * History:    �޸���ʷ��¼�б�
 * 1. Date:   
      Author:  
      Modification:  
      
========================================================================*/

#include  "Judg_Function.h"

/*========================================================================
�ر�����:
    �ڵ������º���ʱ,��ArraySize�����ʵ��,
    �Ƽ�ʹ�� sizeof(pArray)/sizeof(pArray[0])
========================================================================*/


/*=======================================================================
 *������:      g_GetMaxArrayVal(uint16* pArray, uint16 ArraySize)
 *����:        ��ȡ�������ֵ
 *����:        pArray:����ָ��         //����Ԫ��ֻ��Ϊ��
               ArraySize:����Ԫ������  //��Χ0~255,�����÷�Χ���޸���������
               Dimen:ά��     
 *���أ�       max:���������Ԫ��
 
 *˵����       ��ȡ��������Ԫ��,λ��δ֪
========================================================================*/
uint16 g_GetMaxArrayVal(uint16* pArray, uint16 ArraySize){
  uint16  max = 0;
  uint8   i;
  if((uint16)ArraySize > 0x00FF)         
    return 0;                    //��Ч���ж�,������Ԫ�ظ�������255,����0
  for(i=0;i<ArraySize;i++){
    max=(pArray[i] > max) ? pArray[i]:max;
  }
  return (max);
}

/*=======================================================================
 *������:      g_GetMaxNode(uint16* pArray, uint16 ArraySize, uint16 max)
 *����:        ��ȡ���ֵλ��
 *����:        pArray:����ָ��
               ArraySize:����Ԫ������  //��Χ0~255,�����÷�Χ���޸���������
               max:��������Ԫ��     
 *���أ�       node:���������Ԫ�ص�λ��
 
 *˵����       ��ȡ��������Ԫ��λ��
========================================================================*/
uint16 qqq;
uint16 g_GetMaxNode(uint16* pArray, uint16 ArraySize, uint16 max){
  uint16 node;
  if((uint16)ArraySize > 0x00FF)         
    return (0xFFFF);                    //��Ч���ж�,������Ԫ�ظ�������255,����0xFF
  for(node=0;node<ArraySize;node++){   
    if(pArray[node] == max)  
      return (node);                    //��ArraySize��������Ԫ����,������δ֪
  }
}
/*=======================================================================
 *������:      g_GetMinArrayVal(uint16* Array, uint16 ArraySize)
 *����:        ��ȡ������Сֵ
 *����:        pArray:����ָ��
               ArraySize:����Ԫ������  //��Χ0~255,�����÷�Χ���޸���������     
 *���أ�       min:��������СԪ��
 
 *˵����       ��ȡ�������СԪ��,λ��δ֪
========================================================================*/
uint16 g_GetMinArrayVal(uint16* pArray, uint16 ArraySize){
  uint16  min = 0xFF;
  uint8   i;
  if((uint16)ArraySize > 0x00FF)
    return 0xFFFF;                    //��Ч���ж�,������Ԫ�ظ�������255,����0xFFFF
  for(i=0;i<ArraySize;i++){
    min=(pArray[i] < min) ? pArray[i]:min;
  }
  return (min);
}

/*=======================================================================
 *������:      g_GetMinNode(uint16* pArray, uint16 ArraySize, uint16 min)
 *����:        ��ȡ��Сֵλ��
 *����:        pArray:����ָ��
               ArraySize:����Ԫ������  //��Χ0~255,�����÷�Χ���޸���������
               min:�������СԪ��     
 *���أ�       node:��������СԪ�ص�λ��
 
 *˵����       ��ȡ�������СԪ��λ��
========================================================================*/
uint16 g_GetMinNode(uint16* pArray, uint16 ArraySize, uint16 min){
  uint16 node;
  if((uint16)ArraySize > 0x00FF)         
    return (0xFFFF);                    //��Ч���ж�,������Ԫ�ظ�������255,����0xFF
  for(node=0;node<ArraySize;node++){
    if(pArray[node] == min)
      return (node);
  }
  return (0xFF);   //���޷���λ����Сֵλ��ʱ,����0xFF
}