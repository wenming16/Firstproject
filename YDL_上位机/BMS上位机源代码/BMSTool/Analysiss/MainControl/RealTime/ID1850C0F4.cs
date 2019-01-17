using BMSTool.DataPacks;
using BMSTool.OtherClass;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BMSTool.Analysiss.MainControl.RealTime
{
    class ID1850C0F4
    {
        public List<Analysiser.CsvUnitFormat> Analysis(string FrameData)
        {
            List<Analysiser.CsvUnitFormat> CsvUnits = new List<Analysiser.CsvUnitFormat>();
            Analysiser.CsvUnitFormat CsvUnit = new Analysiser.CsvUnitFormat();

            DataPack.SysCur sysCur = Analysiser.dataPack.sysCur;
            string str1 = FrameData.Substring(0, 4);
            str1 = Compute.str16to10With(str1, 0.1, 750);
            sysCur.Test = str1;

            string str2 = FrameData.Substring(4, 4);
            str2 = Compute.str16to10With(str2,0.1,750);
            sysCur.ChargeLim = str2;

            string str3 = FrameData.Substring(8, 4);
            str3 = Compute.str16to10With(str3, 0.1, 750);
            sysCur.DisChargeLim = str3;

            string str4 = FrameData.Substring(12, 4);
            str4 = Compute.str16to10With(str4, 0.1, 750);
            sysCur.MaxFeedLim = str4;

            Analysiser.dataPack.sysCur = sysCur;

            return CsvUnits;
        }
    }
}
