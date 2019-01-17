using BMSTool.DataPacks;
using BMSTool.OtherClass;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BMSTool.Analysiss
{
    class ID18FF971
    {
        public List<Analysiser.CsvUnitFormat> Analysis(int ModNum, string FrameData)
        { 
            DataPack.FromVol volInfo = Analysiser.dataPack.fromVols[ModNum];
            string str1 = FrameData.Substring(0, 4);
            str1 = Compute.str16to10With(str1,0.0001,0);
            volInfo.MaxValue = str1;
            string str2 = FrameData.Substring(4, 2);
            str2 = Compute.str16to10(str2);
            volInfo.MaxNum = str2;
            string str3 = FrameData.Substring(6, 4);
            str3 = Compute.str16to10With(str3, 0.0001, 0);
            volInfo.MinValue = str3;
            string str4 = FrameData.Substring(10, 2);
            str4 = Compute.str16to10(str4);
            volInfo.MinNum = str4;
            Analysiser.dataPack.fromVols[ModNum] = volInfo;
            //均衡
            bool[,] balance = Analysiser.dataPack.balance;
            string str5 = FrameData.Substring(14, 2);
            int  intTem= Convert.ToInt32(str5, 16);
            if (intTem == 0)
            {
                for (int i = 0; i < 32; i++)
                {
                    balance[ModNum, i] = false;
                }
            }
            else
            {
                for (int i = 0; i < 32; i++)
                {
                    balance[ModNum, i] = false;
                }
                balance[ModNum, intTem - 1] = true;
            }
            Analysiser.dataPack.balance = balance;

            List<Analysiser.CsvUnitFormat> CsvUnits = new List<Analysiser.CsvUnitFormat>();
            Analysiser.CsvUnitFormat CsvUnit = new Analysiser.CsvUnitFormat();
            CsvUnit.Name = ModNum + "号从控板最高电压值";
            CsvUnit.Value = volInfo.MaxValue;
            AddItem.CsvUnit(CsvUnits, CsvUnit);
            CsvUnit.Name = ModNum + "号从控板最高最高电压电池编号";
            CsvUnit.Value = volInfo.MaxNum;
            AddItem.CsvUnit(CsvUnits, CsvUnit);
            CsvUnit.Name = ModNum + "号从控板最低电压值";
            CsvUnit.Value = volInfo.MinValue;
            AddItem.CsvUnit(CsvUnits, CsvUnit);
            CsvUnit.Name = ModNum + "号从控板最低电压电池编号";
            CsvUnit.Value = volInfo.MinNum;
            AddItem.CsvUnit(CsvUnits, CsvUnit);
            return CsvUnits;
        }
    }
}
