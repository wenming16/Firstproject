/*=======================================================================
 *Subsystem:   裸机
 *File:        Judg_Function.C
 *Author:      WenYh
 *Description: 
 ========================================================================
 * History:    修改历史记录列表
 * 1. Date:   
      Author:  
      Modification:  
      
========================================================================*/

#include  "Judg_Function.h"

/*========================================================================
特别声明:
    在调用以下函数时,向ArraySize输入的实参,
    推荐使用 sizeof(pArray)/sizeof(pArray[0])
========================================================================*/


/*=======================================================================
 *函数名:      g_GetMaxArrayVal(uint16* pArray, uint16 ArraySize)
 *功能:        获取数组最大值
 *参数:        pArray:数组指针         //数组元素只能为正
               ArraySize:数组元素数量  //范围0~255,超过该范围请修改数据类型
               Dimen:维度     
 *返回：       max:数组中最大元素
 
 *说明：       获取数组的最大元素,位置未知
========================================================================*/
uint16 g_GetMaxArrayVal(uint16* pArray, uint16 ArraySize){
  uint16  max = 0;
  uint8   i;
  if((uint16)ArraySize > 0x00FF)         
    return 0;                    //有效性判断,若数组元素个数大于255,返回0
  for(i=0;i<ArraySize;i++){
    max=(pArray[i] > max) ? pArray[i]:max;
  }
  return (max);
}

/*=======================================================================
 *函数名:      g_GetMaxNode(uint16* pArray, uint16 ArraySize, uint16 max)
 *功能:        获取最大值位置
 *参数:        pArray:数组指针
               ArraySize:数组元素数量  //范围0~255,超过该范围请修改数据类型
               max:数组的最大元素     
 *返回：       node:数组中最大元素的位置
 
 *说明：       获取数组的最大元素位置
========================================================================*/
uint16 qqq;
uint16 g_GetMaxNode(uint16* pArray, uint16 ArraySize, uint16 max){
  uint16 node;
  if((uint16)ArraySize > 0x00FF)         
    return (0xFFFF);                    //有效性判断,若数组元素个数大于255,返回0xFF
  for(node=0;node<ArraySize;node++){   
    if(pArray[node] == max)  
      return (node);                    //若ArraySize大于数组元素数,返回数未知
  }
}
/*=======================================================================
 *函数名:      g_GetMinArrayVal(uint16* Array, uint16 ArraySize)
 *功能:        获取数组最小值
 *参数:        pArray:数组指针
               ArraySize:数组元素数量  //范围0~255,超过该范围请修改数据类型     
 *返回：       min:数组中最小元素
 
 *说明：       获取数组的最小元素,位置未知
========================================================================*/
uint16 g_GetMinArrayVal(uint16* pArray, uint16 ArraySize){
  uint16  min = 0xFF;
  uint8   i;
  if((uint16)ArraySize > 0x00FF)
    return 0xFFFF;                    //有效性判断,若数组元素个数大于255,返回0xFFFF
  for(i=0;i<ArraySize;i++){
    min=(pArray[i] < min) ? pArray[i]:min;
  }
  return (min);
}

/*=======================================================================
 *函数名:      g_GetMinNode(uint16* pArray, uint16 ArraySize, uint16 min)
 *功能:        获取最小值位置
 *参数:        pArray:数组指针
               ArraySize:数组元素数量  //范围0~255,超过该范围请修改数据类型
               min:数组的最小元素     
 *返回：       node:数组中最小元素的位置
 
 *说明：       获取数组的最小元素位置
========================================================================*/
uint16 g_GetMinNode(uint16* pArray, uint16 ArraySize, uint16 min){
  uint16 node;
  if((uint16)ArraySize > 0x00FF)         
    return (0xFFFF);                    //有效性判断,若数组元素个数大于255,返回0xFF
  for(node=0;node<ArraySize;node++){
    if(pArray[node] == min)
      return (node);
  }
  return (0xFF);   //当无法定位到最小值位置时,返回0xFF
}