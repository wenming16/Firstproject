using BMSTool.DataPacks;
using BMSTool.OtherClass;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BMSTool.Analysiss.MainControl.RealTime
{
    class ID1870C0F4
    {
        public List<Analysiser.CsvUnitFormat> Analysis(string FrameData)
        {
            List<Analysiser.CsvUnitFormat> CsvUnits = new List<Analysiser.CsvUnitFormat>();
            Analysiser.CsvUnitFormat CsvUnit = new Analysiser.CsvUnitFormat();

            String IDNumber = FrameData.Substring(0, 2);
            switch (IDNumber)
            {
                case "00":
                    DataPack.Soc soc = Analysiser.dataPack.soc;
                    string str1 = FrameData.Substring(2, 2);
                    str1 = Compute.str16to10(str1);
                    soc.Read = str1;
                    string str2 = FrameData.Substring(4, 2);
                    str2 = Compute.str16to10(str2);
                    soc.Vol = str2;
                    Analysiser.dataPack.soc = soc;

                    string str3 = FrameData.Substring(6, 2);
                    str3 = Compute.str16to10(str3);
                    Analysiser.dataPack.Soh = str3 ;

                    DataPack.KWH kWH = Analysiser.dataPack.kWH;
                    string str4 = FrameData.Substring(8, 4);
                    str4 = Compute.str16to10With(str4,0.1,0);
                    kWH.OneCharge = str4;

                    string str5 = FrameData.Substring(12, 4);
                    str5 = Compute.str16to10With(str5, 0.1, 0);
                    kWH.OneDisCharge = str5;
                    Analysiser.dataPack.kWH = kWH;
                    break;
                case "01":
                    kWH = Analysiser.dataPack.kWH;
                    str5 = FrameData.Substring(2, 6);
                    str5 = Compute.str16to10With(str5, 0.1, 0);
                    kWH.AccCharge = str5;

                    string str6 = FrameData.Substring(8, 6);
                    str6 = Compute.str16to10With(str6, 0.1, 0);
                    kWH.AccDisCharge = str6;

                    Analysiser.dataPack.kWH = kWH;
                    break;
                    
            }
            return CsvUnits;
        }
    }
}
