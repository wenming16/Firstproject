using BMSTool.DataPacks;
using BMSTool.OtherClass;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BMSTool.Analysiss.MainControl.RealTime
{
    class ID1820C0F4
    {
        public List<Analysiser.CsvUnitFormat> Analysis(string FrameData)
        {
            List<Analysiser.CsvUnitFormat> CsvUnits = new List<Analysiser.CsvUnitFormat>();
            Analysiser.CsvUnitFormat CsvUnit = new Analysiser.CsvUnitFormat();
            DataPack.TimeInfo timeInfo=  Analysiser.dataPack.timeInfo;//这是个值类型//所以只传递值


            string NumberID = FrameData.Substring(0, 2);
            switch (NumberID)
            {
                case "00":
                    timeInfo = Case00(FrameData, timeInfo);
                    break;
                case "01":
                     
                    timeInfo =Case01(FrameData,timeInfo);
                    break;
                case "02":
                    //String SelfCheck = Analysiser.dataPack.CheckSelf;//自检
                    DataPack.SelfCheck selfCheck= Analysiser.dataPack.selfChecK;
                    selfCheck = Case02(FrameData, selfCheck);
                    Analysiser.dataPack.selfChecK = selfCheck;
                    break;
            }
            Analysiser.dataPack.timeInfo =timeInfo;
            return CsvUnits;
        }
        private DataPack.TimeInfo Case00(string FrameData, DataPack.TimeInfo timeInfo)
            {
            string Data = FrameData.Substring(2, 12);
            string[] Features= Process(Data,5);
            string  Year =Features[0];
            string Month = Features[1];
            string Day = Features[2];
            string Hour =Features[3];
            string Minute = Features[4];

            timeInfo.SysReal = Year + Month + Day + Hour + Minute;
            return timeInfo;
    
        }
        private string[]Process(string Data,int Num)
        {
            string[] Features = new string[Num];
            for (int i = 0; i < Num; i++)
            {
               string str = Data.Substring(i * 2, 2);
                Features[i] = Compute.str16to10(str);
            }
            return Features;
        }


        
        private DataPack.TimeInfo Case01(string FrameData, DataPack.TimeInfo timeInfo)
        {
            string str1 = FrameData.Substring(2, 2);
            String temStr = string.Empty ;
            switch (str1)
            {
                case "00":
                    temStr = "放电";
                    break;
                case "01":
                    temStr = "快充";
                    break;
                case "02":
                    temStr = "慢充";
                    break;

            }
            timeInfo.SysState = temStr;

            string str2 = FrameData.Substring(4, 4);
            str2 = Compute.str16to10(str2);
            timeInfo.AllRun = str2;

            string str3 = FrameData.Substring(8, 4);
            str3 = Compute.str16to10(str3);
            timeInfo.OnceRun = str3;
            
            String str4 = FrameData.Substring(12, 4);
            str4 = Compute.str16to10(str4);
            timeInfo.PowerOff = str4;

            return timeInfo;
           
        }
        private DataPack.SelfCheck Case02(string FrameData, DataPack.SelfCheck selfCheck)
        {
            string str1 = FrameData.Substring(2, 2);
            string temStr=string.Empty;
            switch (str1)
            {
                case "00":
                    temStr = "自检中";
                    break;
                case "01":
                    temStr = "成功";
                    break;
                case "02":
                    temStr = "失败";
                    break;
            }
            selfCheck.state= temStr;
            int num = selfCheck.errorCode.Length;
            for (int i = 0; i < num; i++)
            {
                selfCheck.errorCode[i] = FrameData.Substring(4+2*i, 2);
            }
            return selfCheck;

        }
        
    }
}
