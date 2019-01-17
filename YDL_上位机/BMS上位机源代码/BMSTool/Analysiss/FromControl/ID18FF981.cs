using BMSTool.DataPacks;
using BMSTool.OtherClass;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BMSTool.Analysiss
{
    class ID18FF981
    {
        public List<Analysiser.CsvUnitFormat> Analysis(int ModNum, string FrameData)
        {
            
            Analysiser.CsvUnitFormat CsvUnit = new Analysiser.CsvUnitFormat();
            DataPack.FromTemp tempInfo = Analysiser.dataPack.fromTemps[ModNum];
            string str1 = FrameData.Substring(0, 2);
            str1 = Compute.str16to10With(str1, 1, 40);
            tempInfo.MaxValue = str1;
            string str2 = FrameData.Substring(2, 2);
            str2 = Compute.str16to10With(str2, 1, 0);
            tempInfo.MaxNum = str2;
            string str3 = FrameData.Substring(4, 2);
            str3 = Compute.str16to10With(str3, 1, 40);
            tempInfo.MinValue = str3;
            string str4 = FrameData.Substring(6, 2);
            str4 = Compute.str16to10With(str4, 1, 0);
            tempInfo.MinNum = str4;
           
            Analysiser.dataPack.fromTemps[ModNum] = tempInfo;
            List<Analysiser.CsvUnitFormat> CsvUnits = new List<Analysiser.CsvUnitFormat>();
            Analysiser.CsvUnitFormat csvUnit = new Analysiser.CsvUnitFormat();
            csvUnit.Name = "模组最高温度值";
            csvUnit.Value = tempInfo.MaxValue;
            AddItem.CsvUnit(CsvUnits, CsvUnit);
            csvUnit.Name = "模组最高温度电池编号";
            csvUnit.Value = tempInfo.MaxNum;
            AddItem.CsvUnit(CsvUnits, CsvUnit);
            csvUnit.Name = "模组最低温度值";
            csvUnit.Value = tempInfo.MaxNum;
            AddItem.CsvUnit(CsvUnits, CsvUnit);
            csvUnit.Name = "模组最低温度电池编号";
            csvUnit.Value = tempInfo.MinNum;
            AddItem.CsvUnit(CsvUnits, CsvUnit);
            return CsvUnits;
        }
    }
}
