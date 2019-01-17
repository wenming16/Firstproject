using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BMSTool.DataPacks
{
    public class DataPack
    {
        public DataPack()
        {
            FromCtr();
        }


        private void FromCtr()//从控板初始化结构体内数组
        {
            int Num_6804 = 4;
            int Num_Temp = 14;
            int Num_Vol_6804 = 12;
            for (int i = 0; i < 10; i++)
            {
                fromVols[i].Values = new string[Num_6804, Num_Vol_6804];
                fromTemps[i].Values = new string[Num_Temp];
            }

        }
        //CSSU
        //18FF970---18FF971
        public struct FromVol
        { 
            public string[,] Values;
            public string MaxValue;
            public string MaxNum;
            public string MinValue;
            public string MinNum;
            public string TotalValue;

        }
        public FromVol[] fromVols = new FromVol[10];

        public int[,] batDistributeNums = new int[10,4];

        //18FF980-18FF981
        public struct FromTemp
        {
            public string[] Values;
            public string MaxValue;
            public string MaxNum;
            public string MinValue;
            public string MinNum;

        }
        public FromTemp[] fromTemps = new FromTemp[10];
        //18FF990
        public struct ModuleError
        {
            public string ChipTemp_h;
            //public string Vol_h;
            //public string Temp_h;
            public string Open_Wire;

        }
        public ModuleError[] ModuleErrors = new ModuleError[10];
        //19FF990
        public bool[,,] openWires = new bool[10, 4, 16];
        //BMU
        //ID1800C0F4
        //0
        public struct BatInfo
        {
            public string Type;
            public string Resist;
            public string Capacity;
            public string CycleLife;
            public int stringNum;
            public int paraNum;
        }
        public BatInfo batInfo = new BatInfo();


        //阈值结构体
        //ID1810C0F4-ID1812C0F4
        public struct VolTsd
        {
            public string[] CellVol_Hs;
            public string[] CellVol_Ls;
            public string[] CellVolGaps;

            public string[] SysVol_Hs;
            public string[] SysVol_Ls;
            public string[] SysVolGaps;

            public VolTsd(int i)
            {
                CellVol_Hs = new string[i];
                CellVol_Ls = new string[i];
                CellVolGaps = new string[i];

                SysVol_Hs = new string[i];
                SysVol_Ls = new string[i];
                SysVolGaps = new string[i];
            }
        }
        public VolTsd volTsd = new VolTsd(3);
        public struct CurTsd
        {
            public string[] SysDisCrgCur_Hs;
            public string[] SysCrgCur_Hs;
            public CurTsd(int i)
            {
                SysDisCrgCur_Hs = new string[i];
                SysCrgCur_Hs = new string[i];
            }
        }
        public CurTsd curTsd = new CurTsd(3);

        public struct SocTsd
        {
            public string High;
            public string Low;
        }
        public SocTsd[] socTsd = new SocTsd[3];

        public struct TempTsd
        {
            public string[] ChargeBat_Hs;
            public string[] ChargeBat_Ls;

            public string[] DisChargeBat_Hs;
            public string[] DisChargeBat_Ls;

            public string[] ChargeBatGap_Hs;
            public string[] DisChargeBatGap_Hs;

            public string[] ChargerGun_Hs;

            public string[] MainBoard_Hs;
            public TempTsd(int i)
            {
                ChargeBat_Hs = new string[i];
                ChargeBat_Ls = new string[i];

                DisChargeBat_Hs = new string[i];
                DisChargeBat_Ls = new string[i];

                ChargeBatGap_Hs = new string[i];
                DisChargeBatGap_Hs = new string[i];

                ChargerGun_Hs = new string[i];

                MainBoard_Hs = new string[i];
            }
        }
        public TempTsd tempTsd = new TempTsd(3);
        public string[] InsulaTsds = new string[3];

        //实时信息//1820C0F4
        public struct TimeInfo
        {
            public string SysReal;
            public string SysState;
            public string AllRun;
            public string OnceRun;
            public string PowerOff;
        }
        public TimeInfo timeInfo = new TimeInfo();

        public struct SelfCheck
        {
            public string state;
            //自检码
            public string[] errorCode ;
            public SelfCheck(int i )
            {
                state = string.Empty;
                errorCode = new string[i];
            }
        }
        public SelfCheck selfChecK = new SelfCheck(4);
       
        //1830C0F4
        public struct SysVol
        {
            public string CellAcc;
            public string MainControl;
            public string TotalGap;
            public string Cell_H;
            public string Cell_L;
            public string CellGap;
        }
        public SysVol sysVol = new SysVol();
        //1840C0F4
        public struct SysTemp
        {
            public string Cell_H;
            public string Cell_L;
            public string CellGap;

            public string MainBoard;
            public string[] SlowGuns;
            public string[] QuickGuns;
            public SysTemp(int i, int j)
            {
                Cell_H = string.Empty;
                Cell_L = string.Empty;
                CellGap = string.Empty;

                MainBoard = string.Empty;
                SlowGuns = new string[i];

                QuickGuns = new string[j];
            }
        }
        public SysTemp sysTemp = new SysTemp(2, 4);

        //1850COF4
        public struct SysCur
        {
            public string Test;
            public string ChargeLim;
            public string DisChargeLim;
            public string MaxFeedLim;

        }
        public SysCur sysCur;
        //1860COF4
        public struct SysInsual
        {
            public string PosVol;
            public string NegVol;
            public string PosResis;
            public string NegResis;
        }
        public SysInsual sysInsual = new SysInsual();
        //1870COF4
        public struct Soc
        {
            public string Read;
            public string Vol;

        }
        public Soc soc = new Soc();
        public string Soh;
        public struct KWH
        {
            public string OneCharge;
            public string OneDisCharge;
            public string AccCharge;
            public string AccDisCharge;
        }
        public KWH kWH = new KWH();

        //1880C0F4
        public struct SwitchState
        {
            public bool mainPos;
            public bool mainNeg;
            public bool PreCharge;
            public bool QuickCharge;
        }
        public SwitchState Switch = new SwitchState();
        public struct VolError
        {
            public string Cell_H;
            public string Cell_L;
            public string Cell_Gap;
            public string All_H;
            public string All_L;
            public bool ALL_test;
            public VolError(bool b)
            {
                Cell_H = string.Empty;
                Cell_L = string.Empty;
                Cell_Gap = string.Empty;
                All_H = string.Empty;
                All_L = string.Empty;
                ALL_test = b;
            }
        }
        public VolError volError = new VolError(true);
        public string InsualError = string.Empty;
        public struct TempError
        {
            public string Discharge_H;
            public string Discharge_L;
            public string DischargeGap_H;
            public string Charge_H;
            public string Charge_L;
            public string ChargeGap_H;
            public bool ChargeGun_H;
            public bool SlowCharge_H;
            public bool CSSU;
            public bool BMU;
            public bool HVU;
            public TempError(bool b)
            {
                Discharge_H = string.Empty;
                Discharge_L = string.Empty;
                DischargeGap_H = string.Empty;
                Charge_H = string.Empty;
                Charge_L = string.Empty;
                ChargeGap_H = String.Empty;
                ChargeGun_H = b;
                SlowCharge_H = b;
                CSSU = b;
                BMU = b;
                HVU = b;
            }
        }

        public TempError tempError = new TempError(true);
        public struct SOCError
        {
            public string High;
            public string Low;
        }
        public SOCError SocError = new SOCError();
        public struct CurError
        {
            public string Charge_H;
            public string DisCharge_H;
        }
        public CurError curError = new CurError();
        public struct SwitchError
        {
            public bool Positive;
            public bool Negative;
            public bool Slow;
            public bool Quick;
            public SwitchError(bool b)
            {
                Positive = b;
                Negative = b;
                Slow = b;
                Quick = b;
            }
        }
        public SwitchError switchError = new SwitchError(true);
        public struct CommError
        {
            public bool CSSU;
            public bool VCU;
            public bool HVU;
            public bool Tbox;
            public CommError(bool b)
            {
                CSSU = b;
                VCU = b;
                HVU = b;
                Tbox = b;
            }

        }
        public CommError commError = new CommError(true);


        //均衡
        public bool [,] balance = new bool [10,36];
        //BMS
        public struct CrgBreakBMS
        {
            public bool ReachSoc;
            public bool ReachAllVol;
            public bool ReachCellVol;
            public bool CrgBreak;
        }
        CrgBreakBMS bMSCrgBreak = new CrgBreakBMS();

        public struct CrgErrorBMS
        {
            public bool SelfCheck;
            public bool CurMatch;
            public bool VolMatch;
            public bool Insula;
            //BYTE3
            public bool ConnectorTemp_H;
            public bool CrgConnector;
            public bool BatVol_h;
            public bool HighVolSwitch;
            //byte4
            public bool GunAndcc2;
            public bool Other;
            public bool CrgCur;
            public bool Vol;
        }
        public struct CrgTimeOutBms
        {
            public bool CRMOO;
            public bool CRMA;
            public bool CTSCML;
            public bool CRO;
            public bool CCS;
            public bool CST;
            public bool CSD;
        }

        //充电机
        public struct CrgBreakGun
        {
            public bool NormalStop;
            public bool Man_madeStop;
            public bool ErrorStop;
            public bool BmsStop;
        }
        CrgBreakGun crgBreakGun = new CrgBreakGun();

        public struct CrgGunError
        {
            //BYTE2
            public bool CrgOverTemp;
            public bool CrgConnect;
            public bool CrgSelfOverTemp;
            public bool PowerTransfer;
            //BYTE3
            public bool Crgscram;
            public bool Other;
            public bool CurrentMatch;
            public bool VoltageState;
            //byte4 0xFF
        }
        public struct CrgTimeOutGun
        {
            public bool BRM;
            public bool BCP;
            public bool BRO;
            public bool BCS;
            public bool BCL;
            public bool BST;
            public bool BSD;
        }
        public bool [,] BMSError = new bool[8,4];
        public bool[,] CrgerError = new bool[8, 4];






    }
}
