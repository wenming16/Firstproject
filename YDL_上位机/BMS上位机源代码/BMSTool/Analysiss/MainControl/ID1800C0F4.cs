using BMSTool.DataPacks;
using BMSTool.OtherClass;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BMSTool.Analysiss.MainControl
{
    class ID1800C0F4
    {
        public List<Analysiser.CsvUnitFormat> Analysis(string FrameData)
        {
            List<Analysiser.CsvUnitFormat> CsvUnits = new List<Analysiser.CsvUnitFormat>();
            Analysiser.CsvUnitFormat CsvUnit = new Analysiser.CsvUnitFormat();
            DataPack.BatInfo batInfo = Analysiser.dataPack.batInfo;
            string str1 = FrameData.Substring(0, 2);
            switch (str1)
            {
                case "00":
                    
                    string str2 = FrameData.Substring(2, 2);
                    batInfo.Type = JudgeType(str2);
                    CsvUnit.Name = "电池材料类型";
                    CsvUnit.Value = batInfo.Type;
                    AddItem.CsvUnit(CsvUnits, CsvUnit);

                    string str3 = FrameData.Substring(4, 4);
                    str3 = Compute.str16to10With(str3, 0.1, 0);
                    batInfo.Resist = str3;
                    CsvUnit.Name = "电池内阻";
                    CsvUnit.Value = batInfo.Resist;
                    AddItem.CsvUnit(CsvUnits, CsvUnit);

                    string str4 = FrameData.Substring(8, 4);
                    str4 = Compute.str16to10With(str4, 0.01, 0);
                    batInfo.Capacity = str4;
                    CsvUnit.Name = "电池容量";
                    CsvUnit.Value = batInfo.Capacity;
                    AddItem.CsvUnit(CsvUnits, CsvUnit);

                    string str5 = FrameData.Substring(12, 4);
                    str5 = Compute.str16to10(str5);
                    batInfo.CycleLife = str5;
                    CsvUnit.Name = "循环寿命";
                    CsvUnit.Value = batInfo.CycleLife;
                    AddItem.CsvUnit(CsvUnits, CsvUnit);
                    break;
                case "02":
                    str2 = FrameData.Substring(8, 2);
                    int stringInt = Convert.ToInt32(str2, 16);
                    batInfo.stringNum = stringInt;

                    str3 = FrameData.Substring(14, 2);
                    int paraInt = Convert.ToInt32(str3, 16);
                    batInfo.paraNum = paraInt;
                    break;

                   

            }
            Analysiser.dataPack.batInfo = batInfo;
            return CsvUnits;
        }
        private string JudgeType(string str)
        {
            string temStr = string.Empty;
            switch (str)
            {
                case "01":
                    temStr = "铅酸";
                    break;
                case "02":
                    temStr = "镍氢";
                    break;
                case "03":
                    temStr = "磷酸铁锂";
                    break;
                case "04":
                    temStr = "锰酸锂";
                    break;
                case "05":
                    temStr = "钴酸锂";
                    break;
                case "06":
                    temStr = "三元锂";
                    break;
                case "07":
                    temStr = "聚合物锂离子";
                    break;
                case "08":
                    temStr = "钛酸锂";
                    break;
                    }
            return temStr;
        }
    }
}
