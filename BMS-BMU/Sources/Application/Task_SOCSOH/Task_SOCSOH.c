/*=======================================================================
 *Subsystem:   ���
 *File:        Task_SOCSOH.c
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:  
========================================================================*/
#include  "includes.h"

SOCInfo_T     g_SOCInfo;
EnergyInfo_T  g_EnergyInfo;


static float  inition_soc(float v);
static void   SOC_AhIntegral(float current, uint16 Voltagemin, uint16 Voltagemax, uint16 SampleTime);
static float  Energy_TotalCal(float ah1,uint32 Total_V,float Total_E);
static float  ADC_Current(void); 
static void   SOH_Cal(void);

/*=======================================================================
 *������:      Task_SOCSOH(void)
 *����:        SOC�ļ��㺯��
 *����:        ��       
 *���أ�       ��
 *˵����       ��������Ҫ��SOCֵ
========================================================================*/
void Task_SOCSOH(void)                                
{       
  uint8 Value=0;    
  
  //�ɼ�����
  g_DataColletInfo.DataCollet_Current_Filter = FilterFunction_Median(ADC_Current, g_DataColletInfo.DataCollet_Current_Filter);// ȡ����ʵ����ֵ�������˲�
  
  //SOC�Ĺ��� 
  if(g_VoltInfo.CellVolt_Min != 0 || g_VoltInfo.CellVolt_Max != 0xFFFF)     // ������Ч���ж�
  {                                    
    SOC_AhIntegral(g_DataColletInfo.DataCollet_Current_Filter, g_VoltInfo.CellVolt_Min, g_VoltInfo.CellVolt_Max, SOC_PERIOD); //����SOCֵ 
  }
  else
  {   
    g_SOCInfo.SOC_Init = 0;
  }
  SOH_Cal();
  
  g_Roll_Tick.Roll_SOCSOH++;
}

/*=======================================================================
 *������:      ADC_Current(void)
 *����:        ��ذ����������������ļ��
 *����:        ��         
                      
 *���أ�       Current:����������ͨ����ת�����
 *˵����       ADC ʱ��Ƶ�ʣ�2MHz
========================================================================*/
static
float ADC_Current(void) 
{
  uint16 Currtemp;
  float Hall_voltage;
  Currtemp = ADC_Value(HALLCHANNEL); 
  Hall_voltage = (Currtemp/4095.0*STAND_VOLT-2.5)*HALL_RANGE/2.0;  //���ݴ���������ת��
  return Hall_voltage;  
}

/*=======================================================================
 *������:      SOC_AhIntegral(float current, uint16 Voltagemin, uint16 Voltagemax, uint16 SampleTime)
 *����:        SOC��ʱ���ַ�����
 *����:        Volatagemin��������С��ѹ
               Voltagemax����������ѹ
               SampleTime������ִ��ʱ��
 *���أ�       ��
 *˵����       g_SOCInfo.SOC_ValueRead��SOC�ϴ�����ʾ���ϵĶ�ȡֵ��SOC�ϴδ洢ֵ
               soc_Volt���������С��ѹȡȨֵ�õ���SOCֵ
               ˼·��Ϊ�˱����ϴδ洢ֵ�Ͳ��õ���У��ֵ�������ƫ�
                     ���ü�Ȩ�صİ취���洢ֵ�Ͳ��ֵ����ƫ�ƣ���ʼʱƫ
                     ��洢ֵ��������ƫ����ֵ���洢�ڳ��ʱ����SOC>90%
                     ��SOC<20%�ǽ���У����
========================================================================*/  
static
void SOC_AhIntegral(float current, uint16 Voltagemin, uint16 Voltagemax, uint16 SampleTime)  //100ms
{        
  float T=0;
  float AH=0,ah1=0;
  float Vmin = 0, Vmax = 0;
  //static float Cellcap;
  Vmin = Voltagemin/10000.0;
  Vmax = Voltagemax/10000.0;
  T = SampleTime/1000.0;// 100msʱ��ת��Ϊ��
       
  if((g_SOCInfo.SOC_Init <= 0)||(g_SOCInfo.SOC_Init >= 100)) //EFPROM���ֵΪ0ʱ�����ֵ(�״������õ�)
  { 
      //SOC_LEP_DATA.SOC_t = 1;                         //CAN�ⷢ���
      g_SOCInfo.SOC_LowestVoltGet = inition_soc(Vmin);        //����С��ѹ���õ���SOC 
      g_SOCInfo.SOC_HighestVoltGet = inition_soc(Vmax);       //������ѹ���õ���SOC 
      g_SOCInfo.SOC_ValueVoltGet = g_SOCInfo.SOC_LowestVoltGet/(1+g_SOCInfo.SOC_LowestVoltGet-g_SOCInfo.SOC_HighestVoltGet);
      g_SOCInfo.SOC_ValueRead = g_SOCInfo.SOC_ValueVoltGet;  //�״γ�ʼ��SOCֵ���״ε�һ���ԽϺ�  
      g_SOCInfo.SOC_ValueInitDiff = 0; 
      g_SOCInfo.SOC_ValueRealtimeDiff = g_SOCInfo.SOC_ValueInitDiff ;          
      g_SOCInfo.SOC_Init =  (uint16)(g_SOCInfo.SOC_ValueRead*100);
      *(uint16*)(0x0D10) =  (uint16)(g_SOCInfo.SOC_Init); //g_SOCInfo.SOC_Init

  }
  else if(((g_SOCInfo.SOC_ValueRead < 0.2)||(g_SOCInfo.SOC_ValueRead > 0.9)||(Vmax>=3.5)||(Vmin<=3.242)&&(Vmin>=2.68)) && (g_SysTime.SOC_Static_Time >= 4))
  {   //��SOCС��20%�����90%,�ҵ�ؾ���ʱ�����4Сʱ,���ڳ���״̬�µ���С��0.5A����ʱ�����3Сʱ���в��     
      //SOC_LEP_DATA.SOC_t = 2;
      g_SysTime.SOC_Static_Time = 0;
      g_SOCInfo.SOC_LowestVoltGet = inition_soc(Vmin);        //����С��ѹ���õ���SOC 
      g_SOCInfo.SOC_HighestVoltGet = inition_soc(Vmax);       //������ѹ���õ���SOC 
      g_SOCInfo.SOC_ValueVoltGet = g_SOCInfo.SOC_LowestVoltGet/(1+g_SOCInfo.SOC_LowestVoltGet - g_SOCInfo.SOC_HighestVoltGet);
      g_SOCInfo.SOC_ValueInitDiff  = g_SOCInfo.SOC_ValueRead - g_SOCInfo.SOC_ValueVoltGet;
      g_SOCInfo.SOC_ValueRealtimeDiff = g_SOCInfo.SOC_ValueInitDiff ;
  } 
   
  //��ʱ���֣���Ҫ���������С��
  if(current>=0.5 || current<=-0.5)
  {  
    AH = current*T/3600.0/SYS_CAPACITY;//SOCֵ  
    ah1 = current*T/3600.0;//��λ:Ah
  }
  //���״̬     
  if(g_WorkStateJudge.WorkState == MODE_CHARGE)    //��� 
  {      
    if(g_VoltInfo.CellVolt_Max >= (CELL_VOLT_MAX-0.02)*10000)                         
    //if(g_TempInfo.CellVolt_Max >= (MaxVolt_Cal(g_Batt_TempMesg.aveTemp)-0.02)*10000)          //����ѹ�ﵽ(���ֵ-0.02)ʱ�����г��ĩ�˵�У��    
    {     
      g_SOCInfo.SOC_HighestVoltGet = 1.0;                             // �ߵ�ѹ����SOC��Ϊ1��
      if(g_SOCInfo.SOC_ValueRead > g_SOCInfo.SOC_ValueVoltGet)        //���״̬max_V�ﵽ(���ֵ-0.01)Vʱ,R>V,����SOC_deta��SOC_K��ֵ
      {  //soc_vδ��1ǰsoc_r>soc_v,���ڶ��ν���˺���ʱsoc_v=1,��ʱsoc_v>soc_r
        g_SOCInfo.SOC_ValueRealtimeDiff = g_SOCInfo.SOC_ValueRead - 1; 
        g_SOCInfo.SOC_ValueInitDiff  =  g_SOCInfo.SOC_ValueRealtimeDiff;
      }
      else if((1.0-g_SOCInfo.SOC_ValueVoltGet > 0.01) && (g_SOCInfo.SOC_HighestVoltGet>=1.0)) //���״̬max_V�ﵽ(���ֵ-0.02)Vʱ,R<V,����SOC_deta��SOC_K��ֵ 
      {
        g_SOCInfo.SOC_ValueRealtimeDiff = g_SOCInfo.SOC_ValueRead - 1; 
        g_SOCInfo.SOC_ValueInitDiff  =  g_SOCInfo.SOC_ValueRealtimeDiff;
      }      
    }             
   
    g_EnergyInfo.Energy_Total_Charge = Energy_TotalCal(ah1,g_VoltInfo.SysVolt_Total,g_EnergyInfo.Energy_Total_Charge);
  }            
  //�ŵ�״̬
  else     
  {   
    if(g_VoltInfo.CellVolt_Min <= CELL_VOLT_MIN*10000.0)//2.75V     
    {     
      g_SOCInfo.SOC_LowestVoltGet = 0;                                  //�͵�ѹ��SOC��Ϊ0
      if(g_SOCInfo.SOC_ValueRead < g_SOCInfo.SOC_ValueVoltGet)          //��ֹS_V��Ϊ0ʱ��S_R����Ϊ0
      {
        g_SOCInfo.SOC_ValueRealtimeDiff = g_SOCInfo.SOC_ValueRead;
        g_SOCInfo.SOC_ValueInitDiff  = g_SOCInfo.SOC_ValueRealtimeDiff;
      }
      else if((g_SOCInfo.SOC_ValueVoltGet > 0.01) && (g_SOCInfo.SOC_LowestVoltGet <= 0))      //��ֹS_V��Ϊ0ʱ��S_R��������
      {
        g_SOCInfo.SOC_ValueRealtimeDiff = g_SOCInfo.SOC_ValueRead;
        g_SOCInfo.SOC_ValueInitDiff  = g_SOCInfo.SOC_ValueRealtimeDiff;
      }
    }
    g_EnergyInfo.Energy_Total_DisCharge = Energy_TotalCal(ah1,g_VoltInfo.SysVolt_Total,g_EnergyInfo.Energy_Total_DisCharge);          
  } 
  
  g_SOCInfo.SOC_LowestVoltGet  = g_SOCInfo.SOC_LowestVoltGet-AH;        //��������С��ѹ�ۼ�SOC
  g_SOCInfo.SOC_HighestVoltGet = g_SOCInfo.SOC_HighestVoltGet-AH;       //����������ѹ�ۼ�SOC
  
  
  if(g_SOCInfo.SOC_HighestVoltGet >= 1)
     g_SOCInfo.SOC_HighestVoltGet = 1;
  else if (g_SOCInfo.SOC_LowestVoltGet <= 0)
     g_SOCInfo.SOC_LowestVoltGet = 0;  
  
  //���ƫ���ڵ�ѹ�ߵĵ�ص�SOC���ŵ�ƫ���ڵ͵�ѹ��ص�SOC
  g_SOCInfo.SOC_ValueVoltGet = g_SOCInfo.SOC_LowestVoltGet / (1 + g_SOCInfo.SOC_LowestVoltGet - g_SOCInfo.SOC_HighestVoltGet); 
  g_SOCInfo.SOC_ValueRead    = g_SOCInfo.SOC_ValueVoltGet + g_SOCInfo.SOC_ValueRealtimeDiff;         //���ϴζ�ȡ��SOC�ۼ�SOC   

  if(g_SOCInfo.SOC_CalTime % 10==0) //SOC_ReadУ��
  {
    g_SOCInfo.SOC_CalTime = 0;
    if((abs(current)>=0.5) && ((abs(g_SOCInfo.SOC_ValueRealtimeDiff))>=0.001))
    {
       if( (g_WorkStateJudge.WorkState == MODE_CHARGE) && ((g_SOCInfo.SOC_ValueVoltGet >= 0.9)))     //��磬S_V>0.9
       {
         if(g_SOCInfo.SOC_ValueVoltGet < 1)
         {
            g_SOCInfo.SOC_ValueRealtimeDiff = g_SOCInfo.SOC_ValueRealtimeDiff - (abs(current)/(400.0*SYS_CAPACITY))*g_SOCInfo.SOC_ValueInitDiff ;    //S_V>0.9 && S_V<1     
         } 
         else  
         { 
            g_SOCInfo.SOC_ValueRealtimeDiff = g_SOCInfo.SOC_ValueRealtimeDiff - (1.0/180.0)*g_SOCInfo.SOC_ValueInitDiff ;     //S_V>=1
         }
       }
       else if((g_SOCInfo.SOC_ValueVoltGet<0.2) && (g_WorkStateJudge.WorkState == MODE_DISCHARGE))        //�ŵ磬S_V<0.2
       {
         if(g_SOCInfo.SOC_ValueVoltGet > 0)
         { 
            g_SOCInfo.SOC_ValueRealtimeDiff = g_SOCInfo.SOC_ValueRealtimeDiff - (abs(current)/(400.0*SYS_CAPACITY))*g_SOCInfo.SOC_ValueInitDiff ;    //S_V>0 && S_V<0.2 
         } 
         else  
         {  
            g_SOCInfo.SOC_ValueRealtimeDiff = g_SOCInfo.SOC_ValueRealtimeDiff - (abs(current)/(120.0*SYS_CAPACITY))*g_SOCInfo.SOC_ValueInitDiff ;    //S_V<=0
         }      
       }
       else
       {   
          g_SOCInfo.SOC_ValueRealtimeDiff = g_SOCInfo.SOC_ValueRealtimeDiff - (abs(current)/(1200.0*SYS_CAPACITY))*g_SOCInfo.SOC_ValueInitDiff ;     //���S_V 0~0.9 || �ŵ�S_V 0.2~1
       }
    }
  }
  if(g_SOCInfo.SOC_ValueRead >= 1) 
  {
     g_SOCInfo.SOC_ValueRead = 1;
  }
  else if(g_SOCInfo.SOC_ValueRead <= 0) 
  {
     g_SOCInfo.SOC_ValueRead = 0;
  }     
  EEprom_Data.Charge_Times = EEprom_Data.Charge_Times + (uint16)(g_EnergyInfo.Energy_Total_Charge/SYS_ELECTRIC_QUANTITY); //������ 
  g_SOCInfo.SOC_CalTime++; 
} 

/*=======================================================================
 *������:      CellCapacity_Cal
 *����:        ��ͬ�¶��µ�ص�������һ��
 *����:        temp������������ƽ���¶�                
 *���أ�       float���ڴ��¶���ʵ��õ��ĵ������
 *˵����       �ڲ�ͬ���¶��µ�ص�������һ��
========================================================================*/ 
/*static
float CellCapacity_Cal(uint8 tempave)
{
  uint8 i;
  float cap[6] = {26.1196, 32.4033, 36.3806, 41.9106, 44.5642, 47.2677}; //�������
  float temp[6] = {   -20,     -10,       0,      10,      25,      45}; //����¶�
  
  if(tempave<=temp[0])
  {
    return cap[0];
  }
  
  else if(tempave>=temp[5])
  {
    return cap[5];
  }
  else
  {
     for(i=0; i<6; i++)
     {
        if(tempave == temp[i])
        {
          return cap[i];
        }
        else if(tempave < temp[i+1] && (tempave > temp[i]))
        {
           return (cap[i]+(tempave-temp[i])*(cap[i+1]-cap[i])/(temp[i+1]-temp[i]));
        }
     }
  }
}*/

/*=======================================================================
 *������:      MaxVolt_Cal
 *����:        ��ͬ�¶��µ�ص�SOC����ʱ��ߵ�ѹ��ͬ
 *����:        temp������������ƽ���¶�                
 *���أ�       float���ڴ��¶���ʵ��õ��ĵ������
 *˵����       �ڲ�ͬ���¶���SOC�ն�У���ĵ�ѹ��һ��
========================================================================*/ 
/*
float MaxVolt_Cal(uint8 temp)
{
  if(temp>=10+40)
  {
    return 4.3;
  }
  else if(temp>=5+40)
  {
    return 4.2;
  }
  else
  {
    return 3.9;
  }
}
*/
/*=======================================================================
 *������:      float inition_soc(float v)
 *����:        SOC��ʼ�����������ȡSOCֵ
 *����:        ��       
 *���أ�       ��
 *˵����       ��ѹ��SOC��ֵ   soc��Ϊfloat��V��Ϊunsigned short 
========================================================================*/
static
float inition_soc(float v)
{  
  static float soc;
	static uint8 i;              
  #if(CELL_TYPE == 0x03) 
    //������﮵��
   //	float socdata[15]={  0 ,   0.1,    0.2,    0.3,    0.4,    0.5,    0.6,    0.7,    0.8,    0.9,   0.92,   0.94,   0.96,   0.98,     1 };	
   //	float uocdata[15]={2.9, 3.2013, 3.2432, 3.2733, 3.2776, 3.2857, 3.2930, 3.3189, 3.3191, 3.3196, 3.3200, 3.3205, 3.3210, 3.3220, 3.3312};
   //¥��100AH������﮵��
   static float socdata[21]={     0,    0.05,    0.1,     0.15,    0.2,   0.25,    0.3,   0.35,    0.4,   0.45,    0.5,   0.55,    0.6,   0.65,    0.7,   0.75,    0.8,   0.85,    0.9,   0.95,     1};
   static float uocdata[21]={2.6800,  3.1380, 3.2030,   3.2200, 3.2420, 3.2570, 3.2740, 3.2880, 3.2890, 3.2900, 3.2910, 3.2930, 3.2970, 3.3160, 3.3300, 3.3310, 3.3310, 3.3320, 3.3330, 3.3340,3.3710};

  	if(v < uocdata[0])
  	{
  	   soc=0 ;
  	} 
  	else if(v>uocdata[20]) 
  	{
  	   soc=1;    	  
  	} 
    else 
  	{ 	  
    	for(i=0;i<21;i++)
    	{  		    
    	 if (v==uocdata[i])
    	  {
    	  	 soc=socdata[i];
    	     break;
    	  } 	
    	  else if ((v<uocdata[i+1]) && (v>uocdata[i])) 
    	  {
  	    	soc=socdata[i]+(v-uocdata[i] )*(socdata[i+1]-socdata[i])/(uocdata[i+1]-uocdata[i]);
        	break;
    	  }	  	  	  
    	} 
  	}  
  #endif 
	return soc;    	
}

/*=======================================================================
 *������:      Energy_TotalCal(uint8 mode,float ah1,uint32 Total_V,float Total_E,float current)
 *����:        �����ۼ�����
 *����:        mode:����״̬
               ah1:ʵʱ��ʱ
               Total_V:�����ѹ�ܺ�(��λ��Ҫת��ΪV) 
               Total_E:������
               current:ʵʱ���� 
 *���أ�       Total_E:������
 
 *˵����       
========================================================================*/ 
static
float Energy_TotalCal(float ah1,uint32 Total_V,float Total_E)
{
  float Energy=0;

  Energy = ah1*(((float)Total_V)/10000.0)/1000.0; 
  Total_E = Total_E + abs(Energy);//���������KWH

  return (Total_E);
}

/*=======================================================================
 *������:      SOH_Cal
 *����:        SOH�ļ���
 *����:        ��                
 *���أ�       ��
 *˵����       ������ĳ������������ص�ʹ������˥��
========================================================================*/  
static
void SOH_Cal(void) 
{
   //g_BMSMonitor_SOH.SOH = 1 - (g_EnergyInfo.Energy_Total_Charge/SYS_VOLT_NOMINAL/1000.0/CELL_LIFE_CYCLE*0.2);   //��ʼSOH-����ֵ(�궨ֵ��Bms_Life_Count)
   g_BMSMonitor_SOH.SOH = 1; 
}


