
using BMSTool.DataPacks;
using BMSTool.OtherClass;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace BMSTool.Analysiss.MainControl.RealTime
{
    class ID1830C0F4
    {
        public List<Analysiser.CsvUnitFormat> Analysis(string FrameData)
        {
            List<Analysiser.CsvUnitFormat> CsvUnits = new List<Analysiser.CsvUnitFormat>();
            Analysiser.CsvUnitFormat CsvUnit = new Analysiser.CsvUnitFormat();
            DataPack.SysVol sysVol = Analysiser.dataPack.sysVol;
            string IDNumber = FrameData.Substring(0, 2);
            switch (IDNumber)
            {
                case "00":
                    string str1 = FrameData.Substring(2, 4);
                    str1 = Compute.str16to10With(str1, 0.1, 0);
                    sysVol.CellAcc = str1;

                    string str2 = FrameData.Substring(6, 4);
                    str2 = Compute.str16to10With(str2, 0.1, 0);
                    sysVol.MainControl = str2;

                    string str3 = FrameData.Substring(10, 4);
                    str3 = Compute.str16to10With(str3, 0.1, 0);
                    sysVol.TotalGap = str3;
                    break;
                case "01":
                     str1 = FrameData.Substring(2, 4);
                    str1 = Compute.str16to10With(str1, 0.0001, 0);
                    sysVol.Cell_H= str1;

                    str2 = FrameData.Substring(6, 4);
                    str2 = Compute.str16to10With(str2, 0.0001, 0);
                    sysVol.Cell_L = str2;

                   str3 = FrameData.Substring(10, 4);
                    str3 = Compute.str16to10With(str3, 0.0001, 0);
                    sysVol.CellGap = str3;
                    break;
            }
            Analysiser.dataPack.sysVol = sysVol;
            return CsvUnits;
        }
        

    }
}