using BMSTool.DataPacks;
using BMSTool.OtherClass;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BMSTool.Analysiss
{
    class ID19FF990
    {
        public List<Analysiser.CsvUnitFormat> Analysis(int ModNum, string FrameData)
        {
            List<Analysiser.CsvUnitFormat> CsvUnits = new List<Analysiser.CsvUnitFormat>();
            Analysiser.CsvUnitFormat CsvUnit = new Analysiser.CsvUnitFormat();

            for (int i = 0; i < 4; i++)
            {
                string str6804 = FrameData.Substring(i * 2, 4);
                str6804 = Compute.str16to2(str6804);
                for (int j = 0; j < 16; j++)
                {
                    string temStr = string.Empty;
                    string str = str6804.Substring(15-j, 1);
                    bool flag = true;
                    switch (str)
                    {
                        case "0":
                            flag = false;
                            temStr = "导线正常";
                            break;
                        case "1":
                            flag = true;
                            temStr = "导线开路";
                            break;
                    }
                    Analysiser.dataPack.openWires[ModNum, i, j] = flag;
                    CsvUnit.Name ="第"+ModNum+"个模组"+ "第" + i + "个6804";
                    CsvUnit.Value = temStr;
                    AddItem.CsvUnit(CsvUnits, CsvUnit);
                }
            }
            return CsvUnits;
        }
    }
}
