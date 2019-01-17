/*=======================================================================
 *Subsystem:   ���
 *File:        Task_InsulDetect.c
 *Author:      WenMing
 *Description: �ӿ�
               ��ѹ��⿪�أ��P               PB0                 
               ��ԵV+���أ�                   PB1
               ��ԵV-���أ�                   PB2
               ��Ե��ѹ�����⿪������       PB3
               ��Ե��ѹ�����⿪������       PB4
               ��ѹ���                       PAD00
               V+��ѹ���:                    PAD01
               V-��ѹ���:                    PAD08
               ����Ƶ��:                      2MHz
 =============================================================================
 * History:    �޸���ʷ��¼�б�
 * 1. Date:    
      Author:  
      Modification:
===============================================================================*/
#include  "includes.h"
IsoResist g_IsoDetect;        //�����Ե���ṹ��
/*=======================================================================
 *������:      Insulation_Init
 *����:        ��ذ���ѹ����ʼ������ѹ��⣬��Ե��⣩
 *����:        ��         
                      
 *���أ�       ��
 *˵����       ADC ʱ��Ƶ�ʣ�2MHz
========================================================================*/
uint8  Insulation_Init(void) 
{
  //��Ե�������������
  memset(&g_IsoDetect, 0, sizeof(IsoResist)); //��Ե�������  
    
  HighVoltS1_Dir = 1;
  HighVoltS1 = INS_SwitchOFF;      //��Ե���V+���عر�

  HighVoltS2_Dir = 1; 
  HighVoltS2 = INS_SwitchOFF;      //��Ե���V-���عر�
  return 0;
}

/*=======================================================================
 *������:      ADC_Insul_HVPOSITIVE(void)
 *����:        ���Եص�ѹ���
 *����:        ��         
 *���أ�       uint16:����ADC������
 *˵����       ADC ʱ��Ƶ�ʣ�2MHz
========================================================================*/
static
uint16 ADC_Insul_HVPositive(void) 
{
  uint16 insulp;
  insulp = ADC_Value(HVPositiveChannel); 
  return insulp;  
}

/*=======================================================================
 *������:      ADC_Insul_HVPOSITIVE(void)
 *����:        ���Եص�ѹ���
 *����:        ��         
 *���أ�       uint16:����ADC������
 *˵����       ADC ʱ��Ƶ�ʣ�2MHz
========================================================================*/
static
uint16 ADC_Insul_HVNegative(void) 
{
  uint16 insuln;
  insuln = ADC_Value(HVNegtiveChannel); 
  return insuln;  
}

/*=======================================================================
 *������:      InsulationDetect
 *����:        ��ذ���Ե���
 *����:        ��         
                      
 *���أ�       ��
 *˵����       ADC ʱ��Ƶ�ʣ�2MHz
========================================================================*/
void Task_InsulationDetect(void) 
{
  uint8 count,i;
  static uint8 Time_Flag,Time_Cnt;
  static uint32  SumVpositive,SumVnegtive;
  float   Vpositive_1,Vnegtive_1;
  uint16  VposBuff[12],VnegBuff[12],VposBuff1[12]/*,VnegBuff1[12],VposBuff2[12]*/,VnegBuff2[12];
  uint16  Max_Volt,Max_Volt1,Min_Volt,Min_Volt1;
  static  float Iso_Vposi,Iso_Vnegt;

  if(Time_Flag == 0)//��1��ѭ��(�������Եغ͸��Եؿ���ȫ���رյ�) 
  { 
    Time_Cnt = 0;           
    for(count = 0; count < 12; count++)//����12����ȥ�����ֵ�����ֵ 
    {
      VposBuff[count] = ADC_Insul_HVPositive();   //���Ե��̣�PAD1
      
      VnegBuff[count] = ADC_Insul_HVNegative();   //���Ե��̣�PAD8
    }    
    
    Max_Volt = 0;                 
    Max_Volt1 = 0; 
    Min_Volt = 0xFFFF;
    Min_Volt1 = 0xFFFF;
    SumVpositive =0;
    SumVnegtive =0;
    
    for(i =0; i<12; i++)//����ÿһ�������е������Сֵ(12��)
    {
       if(VposBuff[i] >=Max_Volt)
       {
          Max_Volt = VposBuff[i];
       } 
       if(VposBuff[i] <= Min_Volt)
       {
          Min_Volt = VposBuff[i];
       }
       
       if(VnegBuff[i] >Max_Volt1)
       {
          Max_Volt1 = VnegBuff[i];
       } 
       if(VnegBuff[i] < Min_Volt1)
       {
          Min_Volt1 = VnegBuff[i];
       }
    }
    
    for(count = 0; count < 12; count++)      
    {
       SumVpositive += VposBuff[count];
       SumVnegtive  += VnegBuff[count];
    }
            
    SumVpositive = SumVpositive - Max_Volt - Min_Volt;
    SumVnegtive  = SumVnegtive - Max_Volt1 - Min_Volt1; 
            
    Iso_Vposi = SumVpositive/10.0;
    Iso_Vnegt = SumVnegtive/10.0;// ȥ�������Сֵ֮����ƽ��ֵ
            
    g_IsoDetect.insulation_Vposit =  Iso_Vposi* STAND_VOLT/4096.0/100.0*5100.0;
    g_IsoDetect.insulation_Vnegt  =  Iso_Vnegt* STAND_VOLT/4096.0/100.0*5100.0;
    
    SumVpositive =0;
    SumVnegtive =0;
  
    g_IsoDetect.insulation_TotalVolt = ((Iso_Vposi + Iso_Vnegt) * STAND_VOLT)/4096.0/100.0*5100.0;
  }
   
  if(g_IsoDetect.insulation_TotalVolt <= LowVoltageDec) // �ж���ѹֵ�Ƿ���Ҫ���о�Ե���
  {
      //g_IsoDetect.insulation_grade = 0x00;
      //g_IsoDetect.insulation_curr = 0xaa;
      g_IsoDetect.insulation_resist = 0x1388; //��ͨ��Э��5M
  } 
  else// ����ѹ����LowVoltageDec������ֵ 
  {                                                    
     if(Iso_Vposi >= Iso_Vnegt) //����>=����
     {           
       Time_Flag = 1; 
                      
       if(Time_Cnt == 0)//��1��ѭ��(�Ƚ�������ѹ�͸�����ѹ�Ĵ�С)
       {
         HighVoltS1 = INS_SwitchON;  // ��Ե���V+���رպ�,�պϺ�ͣһ��ʱ���ڼ���ѹֵ���ӳ�ʱ����� 
         HighVoltS2 = INS_SwitchOFF; // ��Ե���V-���ضϿ�
       }
      
       if(Time_Cnt >= 1)//��2��ѭ��(V��>V��,˵����������С,�ڵ�1ѭ���п������Եؿ���) 
       {             
         Time_Flag = 0;
         Time_Cnt = 0;
                                  
         for(count = 0; count < DetectCount; count++)    //��ΪֻҪ�����ĵ�ѹ��������ѹ����ȥ�� 
         {
            VposBuff1[count] = ADC_Insul_HVPositive();   //���Ե��̣�PAD1
            //VnegBuff1[count] = ADC_Insul_HVNegative(); //���Ե��̣�PAD8
         }           
          Max_Volt = 0;                 
          //Max_Volt1 = 0; 
          Min_Volt = 0xFFFF;
          //Min_Volt1 = 0xFFFF;
          for(i =0; i< DetectCount;i++)// ����ÿһ�������е������Сֵ
          {
             if(VposBuff1[i] >Max_Volt)
             {
                Max_Volt = VposBuff1[i];
             } 
             if(VposBuff1[i] < Min_Volt)
             {
                Min_Volt = VposBuff1[i];
             }
             /*
             if(VnegBuff1[i] >Max_Volt1)
             {
                Max_Volt1 = VnegBuff1[i];
             } 
             if(VnegBuff1[i] < Min_Volt1)
             {
                Min_Volt1 = VnegBuff1[i];
             }*/
          }
          
          for(count = 0; count < 12; count++)//����12��ɼ�����ֵ
          {
             SumVpositive += VposBuff1[count];
             //SumVnegtive += VnegBuff1[count];
             
          }                                             
          // ��ȥ�����Сֵ����ʮ������ƽ��ֵ
          SumVpositive = SumVpositive - Max_Volt - Min_Volt;
          Vpositive_1 = SumVpositive/10.0;
          //SumVnegtive = SumVnegtive - Max_Volt1 - Min_Volt1;
          //Vnegtive_1 = SumVnegtive/10.0;//ȥ�������Сֵ֮����ƽ��ֵ 
          
          
          SumVpositive = 0;
          //SumVnegtive=0;    
          
          HighVoltS1 = INS_SwitchOFF;
          HighVoltS2 = INS_SwitchOFF; 
          
          if(Iso_Vposi<10)//��ֹ������о�Ե�������ʱ��ĸΪ0
          {
            g_IsoDetect.insulation_resist   =   0; //��������
            g_IsoDetect.insulation_resist_N =   0; //��������
            g_IsoDetect.insulation_resist_P =   0; //�������� 
          }
          else if(Iso_Vnegt<10)//��ֹ������о�Ե�������ʱ��ĸΪ0
          {
            g_IsoDetect.insulation_resist_N = (uint16)((abs(Iso_Vposi-Vpositive_1)*1.0/Vpositive_1*(1+Iso_Vnegt/Iso_Vposi) * Bias_Resitance)+0.5); //��������
            g_IsoDetect.insulation_resist_P = (uint16)0x1388; //�������� 
            g_IsoDetect.insulation_resist = g_IsoDetect.insulation_resist_N;
          }
          else
          {
            g_IsoDetect.insulation_resist_N = (uint16)((abs(Iso_Vposi-Vpositive_1)*1.0/Vpositive_1*(1+Iso_Vnegt/Iso_Vposi) * Bias_Resitance)+0.5); //��������
            g_IsoDetect.insulation_resist_P = (uint16)((abs(Iso_Vposi-Vpositive_1)*1.0/Vpositive_1*(1+Iso_Vposi/Iso_Vnegt) * Bias_Resitance)+0.5);//�������� 
            g_IsoDetect.insulation_resist   = g_IsoDetect.insulation_resist_N; //��������
          }
         }
       }  
        
       else//V����<V����
       {  
         Time_Flag = 1; 
         if(Time_Cnt == 0)//��1��ѭ��(�Ƚ�������ѹ�͸�����ѹ�Ĵ�С)
         {                    
           HighVoltS1 = INS_SwitchOFF; //V+���ضϿ�
           HighVoltS2 = INS_SwitchON;  //V-���رպ�
         }
         if(Time_Cnt>0)//��2��ѭ��(V��<V��,˵����������С,�ڵ�1ѭ���п������Եؿ���) 
         {         
           Time_Flag = 0;
           Time_Cnt = 0;
         
           for(count = 0; count < DetectCount; count++)  // ֻҪ�˸����ĵ�ѹ�������ǿ���ȥ�� 
           {
             //VposBuff2[count] = ADC_Insul_HVPositive();//���Ե��̣�PAD1
             VnegBuff2[count] = ADC_Insul_HVNegative();  //���Ե��̣�PAD8            
           }  
                      
            //Max_Volt = 0;                 
            Max_Volt1 = 0; 
            //Min_Volt = 0xFFFF;
            Min_Volt1 = 0xFFFF;
            for(i =0; i< DetectCount;i++)// ����ÿһ�������е������Сֵ 
            {
               /*if(VposBuff2[i] >Max_Volt)
               {
                  Max_Volt = VposBuff2[i];
               } 
               if(VposBuff2[i] < Min_Volt)
               {
                  Min_Volt = VposBuff2[i];
               }*/
               
               if(VnegBuff2[i] >Max_Volt1)
               {
                  Max_Volt1 = VnegBuff2[i];
               } 
               if(VnegBuff2[i] < Min_Volt1)
               {
                  Min_Volt1 = VnegBuff2[i];
               }
            }
            
            for(count = 0; count < 12; count++)      
            {
               //SumVpositive += VposBuff2[count];
               SumVnegtive  += VnegBuff2[count];
           
            }
            //��ȥ�����Сֵ����ʮ������ƽ��ֵ                              
            //SumVpositive = SumVpositive - Max_Volt - Min_Volt;
            //Vpositive_1 = SumVpositive/10.0;
            SumVnegtive = SumVnegtive - Max_Volt1 - Min_Volt1; 
            Vnegtive_1 = SumVnegtive/10.0;   // ȥ�������Сֵ֮����ƽ��ֵ 
            
            HighVoltS1 = INS_SwitchOFF;
            HighVoltS2 = INS_SwitchOFF; 
            
            //SumVpositive=0;
            SumVnegtive=0;

            if(Iso_Vposi<10)//��ֹ������о�Ե�������ʱ��ĸΪ0
            {
              g_IsoDetect.insulation_resist_P = (uint16)((abs(Iso_Vnegt-Vnegtive_1)*1.0/Vnegtive_1*(1+Iso_Vposi/Iso_Vnegt) * Bias_Resitance)+0.5);//��������
              g_IsoDetect.insulation_resist_N = (uint16)0x1388;
              g_IsoDetect.insulation_resist   = g_IsoDetect.insulation_resist_P;
            }
            else if(Iso_Vnegt<10)//��ֹ������о�Ե�������ʱ��ĸΪ0
            {
              g_IsoDetect.insulation_resist = 0;      
              g_IsoDetect.insulation_resist_P = 0;//��������
              g_IsoDetect.insulation_resist_N = 0;
            }
            else
            {                
              g_IsoDetect.insulation_resist_P = (uint16)((abs(Iso_Vnegt-Vnegtive_1)*1.0/Vnegtive_1*(1+Iso_Vposi/Iso_Vnegt) * Bias_Resitance)+0.5);//��������
              g_IsoDetect.insulation_resist_N = (uint16)((abs(Iso_Vnegt-Vnegtive_1)*1.0/Vnegtive_1*(1+Iso_Vnegt/Iso_Vposi) * Bias_Resitance)+0.5); //��������
              g_IsoDetect.insulation_resist   = g_IsoDetect.insulation_resist_P;
            }
         }         
      }          
    //�жϾ�Ե����
    
    if(g_IsoDetect.insulation_resist > 0x1388)//5M
    {
      //g_IsoDetect.insulation_grade  = 0x00;
      //g_IsoDetect.insulation_curr   = 0xaa;
      g_IsoDetect.insulation_resist = 0x1388; //��ͨ��Э��5M
    }    
   /* else 
    {
        if(g_IsoDetect.insulation_resist > Resistance_Alarm1)//�޹���
        {
          g_IsoDetect.insulation_grade = 0x00;
          g_IsoDetect.insulation_curr = 0xaa;      
        } 
        if(g_IsoDetect.insulation_resist < Resistance_Alarm2)//��߾�Ե����
        {
          g_IsoDetect.insulation_grade = 0x02;
          g_IsoDetect.insulation_curr = 0x55;
        }
        else //�μ�����
        {
          g_IsoDetect.insulation_grade = 0x02;
          g_IsoDetect.insulation_curr = 0x55;
        }   
    }*/  
  }   
  g_Roll_Tick.Roll_Insul++;
  Time_Cnt++;      
}