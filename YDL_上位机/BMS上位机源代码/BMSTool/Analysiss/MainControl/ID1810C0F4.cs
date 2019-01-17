using BMSTool.Analysiss.MainControl.RealTime;
using BMSTool.DataPacks;
using BMSTool.OtherClass;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BMSTool.Analysiss.MainControl
{
    class ID1810C0F4:RtimeAnalysis
    {
        public List<Analysiser.CsvUnitFormat> Analysis(string FrameData)
        {
            List<Analysiser.CsvUnitFormat> CsvUnits = new List<Analysiser.CsvUnitFormat>();
            Analysiser.CsvUnitFormat CsvUnit = new Analysiser.CsvUnitFormat();
            string str1 = FrameData.Substring(0, 2);
            DataPack.VolTsd volTsd = Analysiser.dataPack.volTsd;
            switch (str1)
            {
                case "00":
                    GetValue( volTsd.CellVol_Hs, 0.0001, FrameData);
                    break;
                case "01":
                    GetValue(volTsd.CellVol_Ls, 0.0001, FrameData);
                    break;
                case "02":
                    GetValue(volTsd.SysVol_Hs, 0.1, FrameData);
                    break;
                case "03":
                    GetValue(volTsd.SysVol_Ls, 0.1, FrameData);

                    break;
                case "04":
                    GetValue(volTsd.CellVolGaps, 0.0001, FrameData);
                    break;
                case "05":
                    GetValue(volTsd.SysVolGaps, 0.1, FrameData);
                    break;
            }
             Analysiser.dataPack.volTsd=volTsd ;
            return CsvUnits;
        }
        private void GetValue( string[] Recs, double Ratio, string Data)//[] Recs：装载数组
        {
            for (int i = 0; i < 3; i++)
            {
                string temStr = Data.Substring(2 + i * 4, 4);
                if (temStr.Contains("FF"))
                {
                    temStr = "";
                }
                else
                {
                    temStr = Compute.str16to10With(temStr, Ratio, 0);
                }
                 Recs[i] = temStr;
            }
        }
        
}
}
