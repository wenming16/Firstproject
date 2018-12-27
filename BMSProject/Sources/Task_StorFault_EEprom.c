/* ========================================================================
 *Author:      ZWB
 *Description: 通信：
               接口：
               波特率：
 ========================================================================
 * History:    修改历史记录列表
 * 1. Date:    
      Author:  
      Modification: 
      
========================================================================*/
#define Eeprom_Communication_ID  0x1610C0F4  

#include  "TypeDefinition.h"
#include  "Task_StorFault_EEprom.h"
#include  "MC9S12XEP100.h"
#include  "derivative.h"
#include  "stddef.h"
#include  "CAN.h" 
#include  "hidef.h"
#include  "IIC.h" 

EEPROM_FaultSave_T  EEPROM_FaultSave;
/*=========================================================================================================================
 *函数名:      static uint8 VoltSH( uint8 pack_volt_over,uint8 year,uint8 month,uint8 day,uint8 hour,uint8 minute ) 
 *功能:       
 *参数:        无       
 *返回：       无
 *说明：       1 总压过压保护值存储
=========================================================================================================================*/
static 
uint8 VoltSH( uint8 pack_volt_over,uint8 year,uint8 month,uint8 day,uint8 hour,uint8 minute ) 
{
    uint8 error = 0,temp;
    static uint8 first_fault;
    uint16 write_val,read_val;
       
    if( pack_volt_over != 0 )                 
    {
        
        EPAGE = 0xFC;
        
        temp = *(uint8*)(0x0826);
        
        temp = temp & 0x03;
        
        if( temp != pack_volt_over || first_fault == 0 ) 
        {
            error = 1;
             
            write_val = pack_volt_over; 
            write_val = 0x0300 | write_val;
            first_fault = 1;
            
            *(uint8*)0x0820 = year;
            *(uint8*)0x0821 = month;
            *(uint8*)0x0822 = day;
            *(uint8*)0x0823 = hour;
            *(uint8*)0x0824 = minute;
            *(uint16*)0x0825 = write_val; 
            
            read_val = *(uint16*)(0x0825);
            
            if( write_val == read_val ) 
            {
                error = 0; 
            }
             
        }   
    }
    
    return error;    
}

/*==========================================================================================================================
 *函数名:      static uint8 VoltSL( uint8 pack_volt_under,uint8 year,uint8 month,uint8 day,uint8 hour,uint8 minute ) 
 *功能:       
 *参数:        无       
 *返回：       无
 *说明：       2总压欠压保护值存储
===========================================================================================================================*/
static 
uint8 VoltSL( uint8 pack_volt_under,uint8 year,uint8 month,uint8 day,uint8 hour,uint8 minute ) 
{
    uint8 error = 0,temp;
    static uint8 first_fault;
    uint16 write_val,read_val;
       
    if( pack_volt_under != 0 )                 
    {
        
        EPAGE = 0xFC;
        
        temp = *(uint8*)(0x082d);
        
        temp = temp & 0x03;
        
        if( temp != pack_volt_under || first_fault == 0 ) 
        {
            error = 1;
             
            write_val = pack_volt_under; 
            write_val = 0x0304 | write_val;
            first_fault = 1;
            
            *(uint8*)0x0827 = year;
            *(uint8*)0x0828 = month;
            *(uint8*)0x0829 = day;
            *(uint8*)0x082a = hour;
            *(uint8*)0x082b = minute;
            *(uint16*)0x082c = write_val; 
            
            read_val = *(uint16*)(0x082c);
            
            if( write_val == read_val ) 
            {
                error = 0; 
            }
             
        }   
    }
    
    return error;    
}

/*=========================================================================================================================
 *函数名:      static uint8 VoltCH( uint8 cell_volt_over,uint8 year,uint8 month,uint8 day,uint8 hour,uint8 minute )   
 *功能:       
 *参数:        无       
 *返回：       无
 *说明：       3单体过压保护值存储
=========================================================================================================================*/
static 
uint8 VoltCH( uint8 cell_volt_over,uint8 year,uint8 month,uint8 day,uint8 hour,uint8 minute ) 
{
    uint8 error = 0,temp;
    static uint8 first_fault;
    uint16 write_val,read_val;
       
    if( cell_volt_over != 0 )                 
    {
        
        EPAGE = 0xFC;
        
        temp = *(uint8*)(0x0834);
        
        temp = temp & 0x03;
        
        if( temp != cell_volt_over || first_fault == 0 ) 
        {
            error = 1;
             
            write_val = cell_volt_over; 
            write_val = 0x0100 | write_val;
            first_fault = 1;
            
            *(uint8*)0x082e = year;
            *(uint8*)0x082f = month;
            *(uint8*)0x0830 = day;
            *(uint8*)0x0831 = hour;
            *(uint8*)0x0832 = minute;
            *(uint16*)0x0833 = write_val; 
            
            read_val = *(uint16*)(0x0833);
            
            if( write_val == read_val ) 
            {
                error = 0; 
            }
             
        }   
    }
    
    return error;    
}

/*===========================================================================================================================
 *函数名:      static uint8 VoltCL( uint8 cell_volt_under,uint8 year,uint8 month,uint8 day,uint8 hour,uint8 minute ) 
 *功能:       
 *参数:        无       
 *返回：       无
 *说明：       4单体欠压保护值存储
============================================================================================================================*/
static 
uint8 VoltCL( uint8 cell_volt_under,uint8 year,uint8 month,uint8 day,uint8 hour,uint8 minute ) 
{
    uint8 error = 0,temp;
    static uint8 first_fault;
    uint16 write_val,read_val;
       
    if( cell_volt_under != 0 )                 
    {
        
        EPAGE = 0xFC;
        
        temp = *(uint8*)(0x083b);
        
        temp = temp & 0x03;
        
        if( temp != cell_volt_under || first_fault == 0 ) 
        {
            error = 1;
             
            write_val = cell_volt_under; 
            write_val = 0x0104 | write_val;
            first_fault = 1;
            
            *(uint8*)0x0835 = year;
            *(uint8*)0x0836 = month;
            *(uint8*)0x0837 = day;
            *(uint8*)0x0838 = hour;
            *(uint8*)0x0839 = minute;
            *(uint16*)0x083a = write_val; 
            
            read_val = *(uint16*)(0x083a);
            
            if( write_val == read_val ) 
            {
                error = 0; 
            }
             
        }   
    }
    
    return error;    
}

/*===========================================================================================================================
 *函数名:      static uint8 VoltCD( uint8 cell_volt_delta,uint8 year,uint8 month,uint8 day,uint8 hour,uint8 minute ) 
 *功能:       
 *参数:        无       
 *返回：       无
 *说明：       5单体压差故障值保存
============================================================================================================================*/
static 
uint8 VoltCD( uint8 cell_volt_delta,uint8 year,uint8 month,uint8 day,uint8 hour,uint8 minute ) 
{
    uint8 error = 0,temp;
    static uint8 first_fault;
    uint16 write_val,read_val;
       
    if( cell_volt_delta != 0 )                 
    {
        
        EPAGE = 0xFC;
        
        temp = *(uint8*)(0x0842);
        
        temp = temp & 0x03;
        
        if( temp != cell_volt_delta || first_fault == 0 ) 
        {
            error = 1;
             
            write_val = cell_volt_delta; 
            write_val = 0x0108 | write_val;
            first_fault = 1;
            
            *(uint8*)0x083c = year;
            *(uint8*)0x083d = month;
            *(uint8*)0x083e = day;
            *(uint8*)0x083f = hour;
            *(uint8*)0x0840 = minute;
            *(uint16*)0x0841 = write_val; 
            
            read_val = *(uint16*)(0x0841);
            
            if( write_val == read_val ) 
            {
                error = 0; 
            }
             
        }   
    }
    
    return error;    
}

/*===========================================================================================================================
 *函数名:      static uint8 CurrH_Charge( uint8 charge_cur_over,uint8 year,uint8 month,uint8 day,uint8 hour,uint8 minute ) 
 *功能:       
 *参数:        无       
 *返回：       无
 *说明：       6充电过流故障值保存
============================================================================================================================*/
static 
uint8 CurrH_Charge( uint8 charge_cur_over,uint8 year,uint8 month,uint8 day,uint8 hour,uint8 minute ) 
{
    uint8 error = 0,temp;
    static uint8 first_fault;
    uint16 write_val,read_val;
       
    if( charge_cur_over != 0 )                 
    {
        
        EPAGE = 0xFC;
        
        temp = *(uint8*)(0x0849);
        
        temp = temp & 0x03;
        
        if( temp != charge_cur_over || first_fault == 0 ) 
        {
            error = 1;
             
            write_val = charge_cur_over; 
            write_val = 0x2000 | write_val;
            first_fault = 1;
            
            *(uint8*)0x0843 = year;
            *(uint8*)0x0844 = month;
            *(uint8*)0x0845 = day;
            *(uint8*)0x0846 = hour;
            *(uint8*)0x0847 = minute;
            *(uint16*)0x0848 = write_val; 
            
            read_val = *(uint16*)(0x0848);
            
            if( write_val == read_val ) 
            {
                error = 0; 
            }
             
        }   
    }
    
    return error;    
}

/*===========================================================================================================================
 *函数名:      static uint8 CurrH_DisChg( uint8 dischar_cur_over,uint8 year,uint8 month,uint8 day,uint8 hour,uint8 minute ) 
 *功能:       
 *参数:        无       
 *返回：       无
 *说明：       7放电过流故障
============================================================================================================================*/
static 
uint8 CurrH_DisChg( uint8 dischar_cur_over,uint8 year,uint8 month,uint8 day,uint8 hour,uint8 minute ) 
{
    uint8 error = 0,temp;
    static uint8 first_fault;
    uint16 write_val,read_val;
       
    if( dischar_cur_over != 0 )                 
    {
        
        EPAGE = 0xFC;
        
        temp = *(uint8*)(0x0850);
        
        temp = temp & 0x03;
        
        if( temp != dischar_cur_over || first_fault == 0 ) 
        {
            error = 1;
             
            write_val = dischar_cur_over; 
            write_val = 0x2010 | write_val;
            first_fault = 1;
            
            *(uint8*)0x084a = year;
            *(uint8*)0x084b = month;
            *(uint8*)0x084c = day;
            *(uint8*)0x084d = hour;
            *(uint8*)0x084e = minute;
            *(uint16*)0x084f = write_val; 
            
            read_val = *(uint16*)(0x084f);
            
            if( write_val == read_val ) 
            {
                error = 0; 
            }
             
        }   
    }
    
    return error;    
}

/*===========================================================================================================================
 *函数名:      static uint8 TempH_DisChg( uint8 dischar_temper_over,uint8 year,uint8 month,uint8 day,uint8 hour,uint8 minute ) 
 *功能:       
 *参数:        无       
 *返回：       无
 *说明：       8放电过温故障
============================================================================================================================*/
static 
uint8 TempH_DisChg( uint8 dischar_temper_over,uint8 year,uint8 month,uint8 day,uint8 hour,uint8 minute ) 
{
    uint8 error = 0,temp;
    static uint8 first_fault;
    uint16 write_val,read_val;
       
    if( dischar_temper_over != 0 )                 
    {
        
        EPAGE = 0xFC;
        
        temp = *(uint8*)(0x0857);
        
        temp = temp & 0x03;
        
        if( temp != dischar_temper_over || first_fault == 0 ) 
        {
            error = 1;
             
            write_val = dischar_temper_over; 
            write_val = 0x1020 | write_val;
            first_fault = 1;
            
            *(uint8*)0x0851 = year;
            *(uint8*)0x0852 = month;
            *(uint8*)0x0853 = day;
            *(uint8*)0x0854 = hour;
            *(uint8*)0x0855 = minute;
            *(uint16*)0x0856 = write_val; 
            
            read_val = *(uint16*)(0x0856);
            
            if( write_val == read_val ) 
            {
                error = 0; 
            }
             
        }   
    }
    
    return error;    
}

/*===========================================================================================================================
 *函数名:      static uint8 TempH_Charge( uint8 charge_temper_over,uint8 year,uint8 month,uint8 day,uint8 hour,uint8 minute ) 
 *功能:       
 *参数:        无       
 *返回：       无
 *说明：       9充电过温故障
============================================================================================================================*/
static 
uint8 TempH_Charge( uint8 charge_temper_over,uint8 year,uint8 month,uint8 day,uint8 hour,uint8 minute ) 
{
    uint8 error = 0,temp;
    static uint8 first_fault;
    uint16 write_val,read_val;
       
    if( charge_temper_over != 0 )                 
    {
        
        EPAGE = 0xFC;
        
        temp = *(uint8*)(0x085e);
        
        temp = temp & 0x03;
        
        if( temp != charge_temper_over || first_fault == 0 ) 
        {
            error = 1;
             
            write_val = charge_temper_over; 
            write_val = 0x1010 | write_val;
            first_fault = 1;
            
            *(uint8*)0x0858 = year;
            *(uint8*)0x0859 = month;
            *(uint8*)0x085a = day;
            *(uint8*)0x085b = hour;
            *(uint8*)0x085c = minute;
            *(uint16*)0x085d = write_val; 
            
            read_val = *(uint16*)(0x085d);
            
            if( write_val == read_val ) 
            {
                error = 0; 
            }
             
        }   
    }
    
    return error;    
}

/*===========================================================================================================================
 *函数名:      static uint8 TempL_DisChg( uint8 dischar_temper_under,uint8 year,uint8 month,uint8 day,uint8 hour,uint8 minute ) 
 *功能:       
 *参数:        无       
 *返回：       无
 *说明：       10放电低温故障
============================================================================================================================*/
static 
uint8 TempL_DisChg( uint8 dischar_temper_under,uint8 year,uint8 month,uint8 day,uint8 hour,uint8 minute ) 
{
    uint8 error = 0,temp;
    static uint8 first_fault;
    uint16 write_val,read_val;
       
    if( dischar_temper_under != 0 )                 
    {
        
        EPAGE = 0xFC;
        
        temp = *(uint8*)(0x0865);
        
        temp = temp & 0x03;
        
        if( temp != dischar_temper_under || first_fault == 0 ) 
        {
            error = 1;
             
            write_val = dischar_temper_under; 
            write_val = 0x1024 | write_val;
            first_fault = 1;
            
            *(uint8*)0x085f = year;
            *(uint8*)0x0860 = month;
            *(uint8*)0x0861 = day;
            *(uint8*)0x0862 = hour;
            *(uint8*)0x0863 = minute;
            *(uint16*)0x0864 = write_val; 
            
            read_val = *(uint16*)(0x0864);
            
            if( write_val == read_val ) 
            {
                error = 0; 
            }
             
        }   
    }
    
    return error;    
}

/*===========================================================================================================================
 *函数名:      static uint8 TempL_Charge( uint8 charge_temper_under,uint8 year,uint8 month,uint8 day,uint8 hour,uint8 minute ) 
 *功能:       
 *参数:        无       
 *返回：       无
 *说明：       11充电过温故障
============================================================================================================================*/
static 
uint8 TempL_Charge( uint8 charge_temper_under,uint8 year,uint8 month,uint8 day,uint8 hour,uint8 minute ) 
{
    uint8 error = 0,temp;
    static uint8 first_fault;
    uint16 write_val,read_val;
       
    if( charge_temper_under != 0 )                 
    {
        
        EPAGE = 0xFC;
        
        temp = *(uint8*)(0x086c);
        
        temp = temp & 0x03;
        
        if( temp != charge_temper_under || first_fault == 0 ) 
        {
            error = 1;
             
            write_val = charge_temper_under; 
            write_val = 0x1014 | write_val;
            first_fault = 1;
            
            *(uint8*)0x0866 = year;
            *(uint8*)0x0867 = month;
            *(uint8*)0x0868 = day;
            *(uint8*)0x0869 = hour;
            *(uint8*)0x086a = minute;
            *(uint16*)0x086b = write_val; 
            
            read_val = *(uint16*)(0x086b);
            
            if( write_val == read_val ) 
            {
                error = 0; 
            }
             
        }   
    }
    
    return error;    
}

/*===========================================================================================================================
 *函数名:      static uint8 TempD_DisChg( uint8 dischar_temper_delta,uint8 year,uint8 month,uint8 day,uint8 hour,uint8 minute ) 
 *功能:       
 *参数:        无       
 *返回：       无
 *说明：       12放电温差故障
============================================================================================================================*/
static 
uint8 TempD_DisChg( uint8 dischar_temper_delta,uint8 year,uint8 month,uint8 day,uint8 hour,uint8 minute ) 
{
    uint8 error = 0,temp;
    static uint8 first_fault;
    uint16 write_val,read_val;
       
    if( dischar_temper_delta != 0 )                 
    {
        
        EPAGE = 0xFC;
        
        temp = *(uint8*)(0x0873);
        
        temp = temp & 0x03;
        
        if( temp != dischar_temper_delta || first_fault == 0 ) 
        {
            error = 1;
             
            write_val = dischar_temper_delta; 
            write_val = 0x1028 | write_val;
            first_fault = 1;
            
            *(uint8*)0x086d = year;
            *(uint8*)0x086e = month;
            *(uint8*)0x086f = day;
            *(uint8*)0x0870 = hour;
            *(uint8*)0x0871 = minute;
            *(uint16*)0x0872 = write_val; 
            
            read_val = *(uint16*)(0x0872);
            
            if( write_val == read_val ) 
            {
                error = 0; 
            }
             
        }   
    }
    
    return error;    
}

/*===========================================================================================================================
 *函数名:      static uint8 TempD_Charge( uint8 charge_temper_under,uint8 year,uint8 month,uint8 day,uint8 hour,uint8 minute ) 
 *功能:       
 *参数:        无       
 *返回：       无
 *说明：       13充电温差故障
============================================================================================================================*/
static 
uint8 TempD_Charge( uint8 charge_temper_under,uint8 year,uint8 month,uint8 day,uint8 hour,uint8 minute ) 
{
    uint8 error = 0,temp;
    static uint8 first_fault;
    uint16 write_val,read_val;
       
    if( charge_temper_under != 0 )                 
    {
        
        EPAGE = 0xFC;
        
        temp = *(uint8*)(0x0880);
        
        temp = temp & 0x03;
        
        if( temp != charge_temper_under || first_fault == 0 ) 
        {
            error = 1;
             
            write_val = charge_temper_under; 
            write_val = 0x1018 | write_val;
            first_fault = 1;
            
            *(uint8*)0x0874 = year;
            *(uint8*)0x0875 = month;
            *(uint8*)0x0876 = day;
            *(uint8*)0x0877 = hour;
            *(uint8*)0x0878 = minute;
            *(uint16*)0x0879 = write_val; 
            
            read_val = *(uint16*)(0x0879);
            
            if( write_val == read_val ) 
            {
                error = 0; 
            }
             
        }   
    }
    
    return error;    
}

/*===========================================================================================================================
 *函数名:      static uint8 InsulationPos_Fault( uint8 insulation_pos_fault,uint8 year,uint8 month,uint8 day,uint8 hour,uint8 minute ) 
 *功能:       
 *参数:        无       
 *返回：       无
 *说明：       14正极绝缘故障
============================================================================================================================*/
static uint8 InsulationPos_Fault( uint8 insulation_pos_fault,uint8 year,uint8 month,uint8 day,uint8 hour,uint8 minute ) 
{
    uint8 error = 0,temp;
    static uint8 first_fault;
    uint16 write_val,read_val;
       
    if( insulation_pos_fault != 0 )                 
    {
        
        EPAGE = 0xFC;
        
        temp = *(uint8*)(0x0881);
        
        temp = temp & 0x03;
        
        if( temp != insulation_pos_fault || first_fault == 0 ) 
        {
            error = 1;
             
            write_val = insulation_pos_fault; 
            write_val = 0x3000 | write_val;
            first_fault = 1;
            
            *(uint8*)0x087b = year;
            *(uint8*)0x087c = month;
            *(uint8*)0x087d = day;
            *(uint8*)0x087e = hour;
            *(uint8*)0x087f = minute;
            *(uint16*)0x0880 = write_val; 
            
            read_val = *(uint16*)(0x0880);
            
            if( write_val == read_val ) 
            {
                error = 0; 
            }
             
        }   
    }
    
    return error;    
}

/*===========================================================================================================================
 *函数名:      static uint8 InsulationNeg_Fault( uint8 insulation_neg_fault,uint8 year,uint8 month,uint8 day,uint8 hour,uint8 minute ) 
 *功能:       
 *参数:        无       
 *返回：       无
 *说明：       15正极绝缘故障
============================================================================================================================*/
static uint8 InsulationNeg_Fault( uint8 insulation_neg_fault,uint8 year,uint8 month,uint8 day,uint8 hour,uint8 minute ) 
{
    uint8 error = 0,temp;
    static uint8 first_fault;
    uint16 write_val,read_val;
       
    if( insulation_neg_fault != 0 )                 
    {
        
        EPAGE = 0xFC;
        
        temp = *(uint8*)(0x0888);
        
        temp = temp & 0x03;
        
        if( temp != insulation_neg_fault || first_fault == 0 ) 
        {
            error = 1;
             
            write_val = insulation_neg_fault; 
            write_val = 0x3004 | write_val;
            first_fault = 1;
            
            *(uint8*)0x0882 = year;
            *(uint8*)0x0883 = month;
            *(uint8*)0x0884 = day;
            *(uint8*)0x0885 = hour;
            *(uint8*)0x0886 = minute;
            *(uint16*)0x0887 = write_val; 
            
            read_val = *(uint16*)(0x0887);
            
            if( write_val == read_val ) 
            {
                error = 0; 
            }
             
        }   
    }
    
    return error;    
}

/*===========================================================================================================================
 *函数名:      static uint8 OpenWire_Fault( uint8 open_wire_fault,uint8 year,uint8 month,uint8 day,uint8 hour,uint8 minute ) 
 *功能:       
 *参数:        无       
 *返回：       无
 *说明：       16导线开路故障
============================================================================================================================*/
static uint8 OpenWire_Fault( uint8 open_wire_fault,uint8 year,uint8 month,uint8 day,uint8 hour,uint8 minute ) 
{
    uint8 error = 0,temp;
    static uint8 first_fault;
    uint16 write_val,read_val;
       
    if( open_wire_fault != 0 )                 
    {
        
        EPAGE = 0xFD;
        
        temp = *(uint8*)(0x0826);
        
        temp = temp & 0x03;
        
        if( temp != open_wire_fault || first_fault == 0 ) 
        {
            error = 1;
             
            write_val = 0xD100;
            first_fault = 1;
            
            *(uint8*)0x0820 = year;
            *(uint8*)0x0821 = month;
            *(uint8*)0x0822 = day;
            *(uint8*)0x0823 = hour;
            *(uint8*)0x0824 = minute;
            *(uint16*)0x0825= write_val; 
            
            read_val = *(uint16*)(0x0825);
            
            if( write_val == read_val ) 
            {
                error = 0; 
            }
             
        }   
    }
    
    return error;    
}


/*=======================================================================
 *函数名:      static uint8 Read_Fault_EEprom(uint8 read_falg,uint8 clear_falg)
 *功能:       
 *参数:        无       
 *返回：       无
 *说明：       读取故障或清除故障标志位,read_flag=0xAA,则读取所有故障；
               clear_flag = 0xAA,则清除EEPROM中存储的故障
========================================================================*/
static uint8 Read_Fault_Eeprom(uint8 read_flag,uint8 clear_flag)
{

    uint8 error = 0;
    uint8 j,temp;
    uint8 *EEPROM_ptr=NULL;
    uint16 i;
    CANFRAME  BMS_to_Upmonitor;
    
    if( read_flag == 0xAA ) 
    {
       error = 1;
       
       EPAGE = 0xFC;
       EEPROM_ptr = (uint8* )0x0820;
       for( i = 0 ; i < 15 ; i++ ) 
       {
           BMS_to_Upmonitor.m_ID = Eeprom_Communication_ID;       
        	 BMS_to_Upmonitor.m_IDE = 1;
         	 BMS_to_Upmonitor.m_RTR = 0;
         	 BMS_to_Upmonitor.m_dataLen = 8;
        	 BMS_to_Upmonitor.m_priority = 6;
        	 BMS_to_Upmonitor.m_data[0] = 0xFC;              // 保护代码
           BMS_to_Upmonitor.m_data[1] = *EEPROM_ptr++;
        	 BMS_to_Upmonitor.m_data[2] = *EEPROM_ptr++;   
        	 BMS_to_Upmonitor.m_data[3] = *EEPROM_ptr++;
        	 BMS_to_Upmonitor.m_data[4] = *EEPROM_ptr++;
        	 BMS_to_Upmonitor.m_data[5] = *EEPROM_ptr++;
        	 BMS_to_Upmonitor.m_data[6] = *EEPROM_ptr++;
        	 BMS_to_Upmonitor.m_data[7] = *EEPROM_ptr++;       
           temp= MSCAN1SendMsg(&BMS_to_Upmonitor); 
       } 
       
       
       
       EPAGE = 0xFD;
       EEPROM_ptr = (uint8* )0x0820;
       BMS_to_Upmonitor.m_ID = Eeprom_Communication_ID;       
    	 BMS_to_Upmonitor.m_IDE = 1;
     	 BMS_to_Upmonitor.m_RTR = 0;
     	 BMS_to_Upmonitor.m_dataLen = 8;
    	 BMS_to_Upmonitor.m_priority = 6;
    	 BMS_to_Upmonitor.m_data[0] = 0xFD;                  // 故障代码
       BMS_to_Upmonitor.m_data[1] = *EEPROM_ptr++;
    	 BMS_to_Upmonitor.m_data[2] = *EEPROM_ptr++;   
    	 BMS_to_Upmonitor.m_data[3] = *EEPROM_ptr++;
    	 BMS_to_Upmonitor.m_data[4] = *EEPROM_ptr++;
    	 BMS_to_Upmonitor.m_data[5] = *EEPROM_ptr++;
    	 BMS_to_Upmonitor.m_data[6] = *EEPROM_ptr++;
    	 BMS_to_Upmonitor.m_data[7] = *EEPROM_ptr++;       
       temp= MSCAN1SendMsg(&BMS_to_Upmonitor);
         
       error = 0;
       
    } 
    else if( clear_flag == 0xAA ) 
    {
      
        DisableInterrupts;

        error = 1;
        
        for( j = 0 ; j < 2 ; j++) 
        {
           
           EPAGE = 0xFC + j ; 
           
           EEPROM_ptr = (uint8*)0x0800;
           
           for( i = 0 ; i < 1024 ; i++ ) 
           {
               
               *(EEPROM_ptr++) = 0x00 ;
           }     
        }
        
        error = 0;
        
        EnableInterrupts;
    }
    
    EEPROM_ptr = NULL;
    
    return error;
    
}
/*=======================================================================
 *函数名:      void Task_StorFault_EEprom(void)
 *功能:       
 *参数:        无       
 *返回：       无
 *说明：       存储故障到内部分页的EEprom中0xFC,0xFD，0x08200~0x0BFF.
               0x0800~0x081F预留。由于EEPROM空间有限，所以只存储最近
               一次的故障。
========================================================================*/
void Task_StorFault_EEprom(void)
{
   
    uint8 readfault[17];
    uint8 read_flag;
    uint8 clear_flag;

    readfault[0] = VoltSH(0x01,EEPROM_FaultSave.IIC_Year,EEPROM_FaultSave.IIC_Month,EEPROM_FaultSave.IIC_Day,EEPROM_FaultSave.IIC_Hour,EEPROM_FaultSave.IIC_Minute); 
    
    readfault[1] = VoltSL(0x01,EEPROM_FaultSave.IIC_Year,EEPROM_FaultSave.IIC_Month,EEPROM_FaultSave.IIC_Day,EEPROM_FaultSave.IIC_Hour,EEPROM_FaultSave.IIC_Minute); 
    
    readfault[2] = VoltCH(0x01,EEPROM_FaultSave.IIC_Year,EEPROM_FaultSave.IIC_Month,EEPROM_FaultSave.IIC_Day,EEPROM_FaultSave.IIC_Hour,EEPROM_FaultSave.IIC_Minute); 
    
    readfault[3] = VoltCL(0x01,EEPROM_FaultSave.IIC_Year,EEPROM_FaultSave.IIC_Month,EEPROM_FaultSave.IIC_Day,EEPROM_FaultSave.IIC_Hour,EEPROM_FaultSave.IIC_Minute); 
    
    readfault[4] = VoltCD(0x01,EEPROM_FaultSave.IIC_Year,EEPROM_FaultSave.IIC_Month,EEPROM_FaultSave.IIC_Day,EEPROM_FaultSave.IIC_Hour,EEPROM_FaultSave.IIC_Minute); 
    
    readfault[5] = CurrH_Charge(0x01,EEPROM_FaultSave.IIC_Year,EEPROM_FaultSave.IIC_Month,EEPROM_FaultSave.IIC_Day,EEPROM_FaultSave.IIC_Hour,EEPROM_FaultSave.IIC_Minute); 
    
    readfault[6] = CurrH_DisChg(0x01,EEPROM_FaultSave.IIC_Year,EEPROM_FaultSave.IIC_Month,EEPROM_FaultSave.IIC_Day,EEPROM_FaultSave.IIC_Hour,EEPROM_FaultSave.IIC_Minute); 
    
    readfault[7] = TempH_DisChg(0x01,EEPROM_FaultSave.IIC_Year,EEPROM_FaultSave.IIC_Month,EEPROM_FaultSave.IIC_Day,EEPROM_FaultSave.IIC_Hour,EEPROM_FaultSave.IIC_Minute); 
    
    readfault[8] = TempH_Charge(0x01,EEPROM_FaultSave.IIC_Year,EEPROM_FaultSave.IIC_Month,EEPROM_FaultSave.IIC_Day,EEPROM_FaultSave.IIC_Hour,EEPROM_FaultSave.IIC_Minute); 
    
    readfault[9] = TempL_DisChg(0x01,EEPROM_FaultSave.IIC_Year,EEPROM_FaultSave.IIC_Month,EEPROM_FaultSave.IIC_Day,EEPROM_FaultSave.IIC_Hour,EEPROM_FaultSave.IIC_Minute); 
    
    readfault[10] = TempL_Charge(0x01,EEPROM_FaultSave.IIC_Year,EEPROM_FaultSave.IIC_Month,EEPROM_FaultSave.IIC_Day,EEPROM_FaultSave.IIC_Hour,EEPROM_FaultSave.IIC_Minute); 
    
    readfault[11] = TempD_DisChg(0x01,EEPROM_FaultSave.IIC_Year,EEPROM_FaultSave.IIC_Month,EEPROM_FaultSave.IIC_Day,EEPROM_FaultSave.IIC_Hour,EEPROM_FaultSave.IIC_Minute); 
    
    readfault[12] = TempD_Charge(0x01,EEPROM_FaultSave.IIC_Year,EEPROM_FaultSave.IIC_Month,EEPROM_FaultSave.IIC_Day,EEPROM_FaultSave.IIC_Hour,EEPROM_FaultSave.IIC_Minute); 
    
    readfault[13] = InsulationNeg_Fault(0x01,EEPROM_FaultSave.IIC_Year,EEPROM_FaultSave.IIC_Month,EEPROM_FaultSave.IIC_Day,EEPROM_FaultSave.IIC_Hour,EEPROM_FaultSave.IIC_Minute); 
    
    readfault[14] = InsulationPos_Fault(0x01,EEPROM_FaultSave.IIC_Year,EEPROM_FaultSave.IIC_Month,EEPROM_FaultSave.IIC_Day,EEPROM_FaultSave.IIC_Hour,EEPROM_FaultSave.IIC_Minute); 
    
    readfault[15] = OpenWire_Fault(0x01,EEPROM_FaultSave.IIC_Year,EEPROM_FaultSave.IIC_Month,EEPROM_FaultSave.IIC_Day,EEPROM_FaultSave.IIC_Hour,EEPROM_FaultSave.IIC_Minute); 
    
    readfault[16] = Read_Fault_Eeprom(read_flag,clear_flag);

}


/*=======================================================================
========================================================================*/
