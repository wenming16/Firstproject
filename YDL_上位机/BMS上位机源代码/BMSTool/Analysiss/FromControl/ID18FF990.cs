using BMSTool.DataPacks;
using BMSTool.OtherClass;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using BMSTool.Analysiss;
using BMSTool.OtherClass;
using BMSTool.Analysiss.Other;

namespace BMSTool.Analysiss
{
    class ID18FF990
    {
        public  List<Analysiser.CsvUnitFormat> Analysis(int ModNum, string FrameData)
        {
            List<Analysiser.CsvUnitFormat> CsvUnits = new List<Analysiser.CsvUnitFormat>();
            Analysiser.CsvUnitFormat CsvUnit = new Analysiser.CsvUnitFormat();
            DataPack.ModuleError errorInfo = Analysiser.dataPack.ModuleErrors[ModNum];

            string Data = FrameData.Substring(0, 4);
            string[] Infos = AnalysisHelper.GetInfos(Data,2,1,0); 

            errorInfo.ChipTemp_h = IsError(Infos[0]);
            //errorInfo.Vol_h = IsError(Infos[1]);
            //errorInfo.Temp_h = IsError(Infos[2]);         
            errorInfo.Open_Wire = IsError(Infos[1]);
            Analysiser.dataPack.ModuleErrors[ModNum] = errorInfo;

            string str = FrameData.Substring(4, 6);
            str = Compute.str16to10With(str, 0.0001, 0);
           Analysiser.dataPack.fromVols[ModNum].TotalValue = str;

            CsvUnit.Name = "6804芯片温度过高故障";
            CsvUnit.Value = errorInfo.ChipTemp_h;
            AddItem.CsvUnit(CsvUnits, CsvUnit);
           
            AddItem.CsvUnit(CsvUnits, CsvUnit);
           
            AddItem.CsvUnit(CsvUnits, CsvUnit);
            CsvUnit.Name = "导线开路故障";
            CsvUnit.Value = errorInfo.Open_Wire;
            CsvUnit.Name = "模组总压值";
            CsvUnit.Value =Analysiser.dataPack.fromVols[ModNum].TotalValue;

            return CsvUnits;
        }
        private  string IsError(string str)
        {
            string temStr=string.Empty;
            switch (str)
            {
                case "00":
                    temStr = "不可信";
                    break;
                case "01":
                    temStr = "正常";
                    break;
                case "02":
                    temStr = "不正常";
                    break;
            }
            return temStr;
        }
    }
}
