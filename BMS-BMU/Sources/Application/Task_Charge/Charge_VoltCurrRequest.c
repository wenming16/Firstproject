#include  "includes.h"
static float Charge_CurrInit(float currlimt, float chargeInit);
static float ChargeEnd_CurrentOut(uint16 cellvoltmax, float curr); 
static uint8 ChargeEndJudge(float soc,float curr_end,uint16 cellvoltmax,uint8 temph,uint8 templ, uint8 BMSFlt, uint8 chargeFlt);
/*=======================================================================
 *������:      BmsToChargePileInfo(void) 
 *����:        �Ѽ���������������Ϣ
 *����:        ��       
 *���أ�       ��
 
 *˵����       ������Ҫ���͸���������Ϣ,��߳������ֲ��
========================================================================*/ 
void Charge_VoltCurrRequest(void)
{
  //static uint8 cnt;
  float curr;
  //����ѹ����
  g_BMSCharge.Volt_Max_ChargePile = (CELL_VOLT_MAX * SYS_SERIES_YiDongLi + 5);         //���������˵�ѹ �ֱ���:0.1V
  if(g_BMSCharge.Control_ChargePile == 1)//���ֹ���
  {
    g_BMSCharge.Volt_Max_ChargePile = 0;
  }
  //����������
  curr = Charge_CurrInit(CurrLimit.Curr_Charge_Cons, g_BMSCharge.Curr_Max_ChargePile);
  g_BMSCharge.Curr_Max_ChargePile = ChargeEnd_CurrentOut(g_VoltInfo.CellVolt_Max, curr);//�Ƚ��д��� �ֱ���0.1A
  
  g_BMSCharge.Control_ChargePile  = ChargeEndJudge(g_SOCInfo.SOC_ValueRead, g_BMSCharge.Curr_Max_ChargePile,\
                                                   g_VoltInfo.CellVolt_Max,\
                                                   g_TempInfo.CellTemp_Max, g_TempInfo.CellTemp_Min,\
                                                   g_Flt_Charge.Level_Charge_SwitchOff_flag, g_Charge_State.FltState);
  
  g_BMSCharge.VoltC_Max           = g_VoltInfo.CellVolt_Max;  //������ߵ�ѹ �ֱ���:0.001V
  g_BMSCharge.VoltC_Min           = g_VoltInfo.CellVolt_Min;  //������͵�ѹ �ֱ���:0.001V
  g_BMSCharge.SOC                 = g_SOCInfo.SOC_ValueRead;  //�Ƚ��д��� �ֱ���:0.4%
  g_BMSCharge.Temp_Max            = g_TempInfo.CellTemp_Max;
  g_BMSCharge.VoltS               = g_VoltInfo.SysVolt_Total; //��g_VoltInfo.SysVolt_Total��λ:0.1mV->0.1V
  
  //������
  if(g_Flt_Charge.Level_Temp_High == 2)
  {
    BMSCharge_State.TempH_Cell = 1;
  }
  else
  {
    BMSCharge_State.TempH_Cell = 0;
  }
  
  //������
   if(g_Flt_Charge.Level_Temp_Low == 2)
  {
    BMSCharge_State.TempL_Cell = 1;
  }
  else
  {
    BMSCharge_State.TempL_Cell = 0;
  }
  
  //������
   if(g_Flt_Charge.Level_Current_Charge_High == 2)
  {
    BMSCharge_State.CurrH_Cell = 1;
  }
  else
  {
    BMSCharge_State.CurrH_Cell = 0;
  }
  
  //��Ե����
   if(g_Flt_Charge.Level_Insul == 2)
  {
    BMSCharge_State.Insul = 1;
  }
  else
  {
    BMSCharge_State.Insul = 0;
  }
  
  //BMS���ձ��ĳ�ʱ
  if(State_Offline.CSSU1 == 1)
  {
    BMSCharge_State.BMSGetMsg = 1;
  } 
  else
  {
    BMSCharge_State.BMSGetMsg = 0;
  }
  
  //���ϱ�־ 1:�κι���
  if(g_Charge_State.FltState||g_Flt_Charge.Level_Charge_SwitchOff_flag)
  {
    BMSCharge_State.FaultFlag = 1;
  }
  else
  {
    BMSCharge_State.FaultFlag = 0;
  }   
}

/*=======================================================================
 *������:      ChargeEndJudge(void) 
 *����:        �����ֹ�ж�
 *����:        ��       
 *���أ�       ��
 
 *˵����       
========================================================================*/ 
static
uint8 ChargeEndJudge(float soc,float curr_end,uint16 cellvoltmax,uint8 temph,uint8 templ, uint8 BMSFlt, uint8 chargeFlt)
{
  static uint8 cnt[5];
  //SOC����100%
  if(soc+0.002>=1)        
  {
    if(++cnt[0]>3)
    {
      cnt[0] = 3;
      return 1;
    }
  }
  else
  {
    cnt[0] = 0;
  }
  //���ĩ�ڵ���С��0.03C
  if(abs(curr_end) < 0.03*SYS_CAPACITY) //0.03*542=16.26A
  {
    if(++cnt[1]>3)
    {
      cnt[1] = 3;
      return (1);  
    }
  }
  else
  {
     cnt[1] = 0;
  }
  //���ﵥ���ֹ��ѹ
  if(cellvoltmax/10000.0 > CELL_VOLT_MAX)
  {
    if(++cnt[2]>3)
    {
      cnt[2] = 3;
      return (1); 
    }
  }
  else
  {
     cnt[2] = 0;
  }
  //��������¶�
  if(temph > CELL_TEMP_MAX_CHARGE + 40)
  {
    if(++cnt[3]>3)
    {
      cnt[3] = 3;
      return (1);  
    }
  }
  else
  {
     cnt[3] = 0;
  }
  //����ֹͣ
  if(templ < CELL_TEMP_MIN_CHARGE + 40)
  {
    if(++cnt[4]>3)
    {
      cnt[4] = 3;
      return (1);
    }
  }
  else
  {
     cnt[4] = 0;
  }
  //BMS���Ϻͳ����������
  if((BMSFlt == 1)||(chargeFlt == 1))
  {
    return 1;
  }

  return (0);
}

/*=======================================================================
 *������:      Charge_Curr(uint16, float curr) 
 *����:        �����ĩ�˵ĵ�������
 *����:        chargeInit����ʼ��ʱ�ĵ���ֵ
               curr:����
 *���أ�       float:���ǹ����ֵ
 *˵����       �жϵ���Ƿ񵽴�3.63V,��������ԭ�����������0.7��������
               ������С����3.63Vʱ���Ͱ�ԭ���ĵ�����С����
========================================================================*/
static 
float Charge_CurrInit(float currlimt, float chargeInit)
{
   float currIn;
   if (chargeInit <= 0)//��ʼ��chargeInitֵ��ȷ��
   {
      chargeInit = currlimt;
   }
   if(chargeInit > currlimt)//��������ĩ�ں�������ֵ���ڵ�������ֵʱ������ֵ
   {  
      currIn = currlimt;
   }
   else//���򰴱������ֵ
   {
      currIn = chargeInit; 
   }
   return currIn;
}

/*=======================================================================
 *������:      ChargeEnd_CurrentOut(uint16, float curr) 
 *����:        ���ĩ�˵ı��������
 *����:        cellvoltmax��������ѹ
               currIn:����
 *���أ�       float:���ǹ����ֵ
 *˵����       �жϵ���Ƿ񵽴�3.63V,��������ԭ�����������0.7��������
               ������С����3.63Vʱ���Ͱ�ԭ���ĵ�����С����
========================================================================*/
static 
float ChargeEnd_CurrentOut(uint16 cellvoltmax, float currIn)
{
  float current;
  static uint8 currflag;
  static uint8 cnt;
  if((cellvoltmax >= 36300)&&(currflag == 0)) //currflag������Ϊ�����ظ��Ľ�����
  {
    currflag = 1;
    cnt = 0;
    if(currIn >= 0.01*SYS_CAPACITY)
    {
      current = 0.7*currIn;  
    }
    else
    {
      current = currIn;
    }
  }
  else
  {
    if(++cnt>3)//3s֮�����жϵ�ѹ����پ����Ƿ���н�����
    {
      cnt = 0;
      currflag = 0;
    }
    current = currIn; 
  }
  return current;
}




