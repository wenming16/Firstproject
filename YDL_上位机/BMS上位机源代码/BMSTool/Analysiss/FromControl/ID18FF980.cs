using BMSTool.DataPacks;
using BMSTool.OtherClass;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BMSTool.Analysiss
{
    class ID18FF980
    {
        public  List<Analysiser.CsvUnitFormat> Analysis(int ModNum, string FrameData)
        {
            List<Analysiser.CsvUnitFormat> CsvUnits = new List<Analysiser.CsvUnitFormat>();
            Analysiser.CsvUnitFormat CsvUnit = new Analysiser.CsvUnitFormat();
            DataPack.FromTemp tempInfo = Analysiser.dataPack.fromTemps[ModNum];
            string str1 = FrameData.Substring(0, 2);
            int Cycle = Convert.ToInt32(str1, 16);
            for (int i = 0; i < 7; i++)
            {
                string Value = FrameData.Substring(i*2+2, 2);
                if (Value == "FF")
                {
                    return CsvUnits;
                }
                else
                {
                    int Index = Cycle * 7 + i;
                    Value = Compute.str16to10With(Value, 1, 40);
                    tempInfo.Values[Index] = Value;
                    CsvUnit.Name = ModNum + "号从控版" + Index + "号温度";
                    CsvUnit.Value = tempInfo.Values[Index];
                    AddItem.CsvUnit(CsvUnits, CsvUnit);
                }
                
            }
           Analysiser.dataPack.fromTemps[ModNum] = tempInfo;
         return CsvUnits;
        }
    }
}
