using BMSTool.DataPacks;
using BMSTool.OtherClass;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BMSTool.Analysiss.MainControl.RealTime
{
    class ID1880C0F4
    {
        private static bool IsError0 = true;
        private static bool IsError1 = true;
        public static bool IsError = true;
        public List<Analysiser.CsvUnitFormat> Analysis(string FrameData)
        {
            IsError = IsError0 && IsError1;
            List<Analysiser.CsvUnitFormat> CsvUnits = new List<Analysiser.CsvUnitFormat>();
            Analysiser.CsvUnitFormat CsvUnit = new Analysiser.CsvUnitFormat();
            
            DataPack dataPack = Analysiser.dataPack;
            string IDNumber = FrameData.Substring(0, 2);
            switch (IDNumber)
            {
                case "00":
                    dataPack= Case00(dataPack, FrameData);
                    string str0 = FrameData.Substring(6, 8)+FrameData.Substring(15,1);//下位机发错数据跳过一位
                    IsError0 = getAllState(str0);
                    break;
                case "01":
                    dataPack = Case01(dataPack, FrameData);
                    string str1 = FrameData.Substring(2, 14);
                    IsError1 = getAllState(str1);
                    break;
            }
            Analysiser.dataPack = dataPack;
            return CsvUnits;
        }
        private bool getAllState( string data)
        {
            bool state = true;
            for (int i = 0; i < data.Length; i++)
            {
                string str = data.Substring(i, 1);
                if ((str != "0")&&(str!="F"))
                {
                    state=false;
                }
            }
            return state;
        }

        private DataPack Case00(DataPack dataPack,String  FrameData)
        {
            string str1 = FrameData.Substring(2, 2);
            SWitchstate(str1, ref dataPack.Switch);

            Byte36Get(FrameData, ref dataPack);
            Byte7Get(FrameData, ref dataPack);
           

            return dataPack;
        }
        private void Byte7Get(string FrameData, ref DataPack dataPack)
        {
            string str = FrameData.Substring(14, 2);
            string Byte7 = Compute.str16to2(str);
            for (int i = 0; i < 3; i++)
            {
                string BitStr = Byte7.Substring(6 - i * 2, 2);
                bool Error = true;
                switch (BitStr)
                {
                    case "00":
                        Error = true;
                        break;
                    case "01":
                        Error = false;
                        break;
                }
                switch (i)
                {
                    case 0:
                        dataPack.tempError.ChargeGun_H = Error;
                        break;
                    case 1:
                        dataPack.tempError.SlowCharge_H = Error;
                        break;
                    case 2:
                        dataPack.volError.ALL_test = Error;
                        break;

                }
            }
        }
        private void Byte36Get(string FrameData, ref DataPack dataPack)
        {
            
            DataPack.VolError volError = dataPack.volError;
            DataPack.TempError tempError = dataPack.tempError;
            


           
            string data1 = FrameData.Substring(6,2);
            string [] Errors1= Byte3to6(data1);
            volError.Cell_H = Errors1[0];
            volError.Cell_L = Errors1[1];
            volError.Cell_Gap = Errors1[2];
            volError.All_H = Errors1[3];


            string Data2 = FrameData.Substring(8, 2);
            string[] Errors2 = Byte3to6(Data2);
            volError.All_L = Errors2[0];
            dataPack.InsualError = Errors2[1];
            tempError.Discharge_H = Errors2[2];
            tempError.Discharge_L = Errors2[3];

            string Data3 = FrameData.Substring(10, 2);
            string[] Errors3 = Byte3to6(Data3);
            tempError.DischargeGap_H = Errors3[0];
            tempError.Charge_H = Errors3[1];
            tempError.Charge_L = Errors3[2];
            tempError.ChargeGap_H = Errors3[3];

            string Data4 = FrameData.Substring(12, 2);
            string[] Errors4 = Byte3to6(Data4);
            dataPack.SocError.High = Errors4[0];
            dataPack.SocError.Low = Errors4[1];
            dataPack.curError.Charge_H = Errors4[2];
            dataPack.curError.DisCharge_H = Errors4[3];


            dataPack.volError = volError;
            dataPack.tempError = tempError;

            Analysiser.dataPack = dataPack;

          
           

        }




        private string[] Byte3to6(string data)
        {
            string [] Error = new string[4];
            string Byte = Compute.str16to2(data);
            for (int i = 0; i < 4; i++)
            {
                string BitStr = Byte.Substring(6 - i * 2, 2);
                Error[i] = ErrorGrade(BitStr);
            }
            return Error;
        }


        private void SWitchstate(string Data,ref DataPack.SwitchState state)
        {
            string  str1 = Compute.str16to2(Data);
            for (int i = 0; i < 4; i++)
            {

                string BitStr = str1.Substring(6 - i * 2, 2);
                bool IsError;
                if (BitStr == "00")
                {
                    IsError = true;
                }
                else
                {
                    IsError = false;
                }
                switch (i)
                {
                    case 0:
                        state.mainPos = IsError;
                        break;
                    case 1:
                        state.mainNeg = IsError;
                        break;
                    case 2:
                        state.PreCharge = IsError;
                        break;
                    case 3:
                        state.QuickCharge = IsError;
                        break;
                }
            }
        }

        private string ErrorGrade (string BitStr)
        {
            string Error = string.Empty;
            switch (BitStr)
            {
                case "00":
                    Error = "正常";
                    break;
                case "01":
                    Error = "1级故障";
                    
                    break;
                case "10":
                    Error = "2级故障";
                    
                    break;
                case "11":
                    Error = "3级故障";
                    
                    break;
            }
            return Error;
        }
        private DataPack Case01(DataPack dataPack, String FrameData)
        {
            int Words = 1;
            string str1 = FrameData.Substring(2, 2*Words);
            str1 = Compute.str16to2(str1);
           
            for (int i = 0; i < 3; i++)
            {
                string BitStr = str1.Substring((8*Words-1)-i, 1);
                bool Error = GetError(BitStr);
                switch (i)
                {
                    case 0:
                        dataPack.tempError.CSSU = Error;
                        break;
                    case 1:
                        dataPack.tempError.BMU = Error;
                        break;
                    case 2:
                        dataPack.tempError.HVU = Error;
                        break;
                }
            }
            Words = 1;
            String Str2 = FrameData.Substring(4, 2*Words);
            Str2 = Compute.str16to2(Str2);
            for(int i = 0; i < 4; i++)
            {
                string BitStr = Str2.Substring((8 * Words - 1) - i, 1);
                bool Error = GetError(BitStr);
                switch (i)
                {
                    case 0:
                        dataPack.switchError.Positive = Error;
                        break;
                    case 1:
                        dataPack.switchError.Negative= Error;
                        break;
                    case 2:
                        dataPack.switchError.Slow = Error;
                        break;
                    case 3:
                        dataPack.switchError.Quick= Error;
                        break;
                }
            }

            string str3 = FrameData.Substring(6, 2);
            str3 = Compute.str16to2(str3);
            for (int i = 0; i < 4; i++)
            {
                string BitStr = str3.Substring(7 - i, 1);
                bool Error = GetError(BitStr);
                switch (i)
                {
                    case 0:
                        dataPack.commError.CSSU = Error;
                        break;
                    case 1:
                        dataPack.commError.VCU= Error;
                        break;
                    case 2:
                        dataPack.commError.HVU = Error;
                        break;
                    case 3:
                        dataPack.commError.Tbox= Error;
                        break;
                }

            }
            string str4 = FrameData.Substring(8, 2);
            //if (str4 == "01")
            //{
            //    dataPack.balance.state = true;
            //}
            //else
            //{
            //    dataPack.balance.state = false;
            //}
            //String str5 = FrameData.Substring(10, 2);
            //int num=Convert.ToInt32(str5,16);
            //dataPack.balance.num = num;
            return dataPack;
        }
        private bool GetError (string BitStr)

        {
            bool Error;
            
            if (BitStr == "0")
            {
                Error =true;
               
            }
            else
            {
                Error = false ;

            }
            return Error;

        }
    }
}
