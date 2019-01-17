using BMSTool.DataPacks;
using BMSTool.OtherClass;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using static BMSTool.DataPacks.DataPack;

namespace BMSTool.Analysiss.MainControl.RealTime
{
    class ID1840C0F4
    {
        public List<Analysiser.CsvUnitFormat> Analysis(string FrameData)
        {
            List<Analysiser.CsvUnitFormat> CsvUnits = new List<Analysiser.CsvUnitFormat>();
            Analysiser.CsvUnitFormat CsvUnit = new Analysiser.CsvUnitFormat();
            DataPack.SysTemp sysTemp = Analysiser.dataPack.sysTemp;
            string IDNumber = FrameData.Substring(0, 2);
            switch (IDNumber)
            {
                case "00":
                    string str1 = FrameData.Substring(2, 2);
                    str1 = Compute.str16to10With(str1,1,40);
                    sysTemp.Cell_H = str1;
                    

                    string str2 = FrameData.Substring(4, 2);
                    str2 = Compute.str16to10With(str2, 1, 40);
                    sysTemp.Cell_L = str2;

                    string str3 = FrameData.Substring(6,2);
                    str3 = Compute.str16to10With(str3, 1, 40);
                    sysTemp.CellGap = str3;
                    break;
                case "01":
                    str1 = FrameData.Substring(2, 2);
                    str1 = Compute.str16to10With(str1, 1, 40);
                    sysTemp.MainBoard = str1;
                    for (int i = 0; i < 2; i++)
                    {
                        str2 = FrameData.Substring(4+i*2, 2);
                        if (str2 != "FF")
                        {
                            str2 = Compute.str16to10With(str2, 1, 40);
                            sysTemp.SlowGuns[i] = str2;
                        }
                        else { }
                       
                    }
                    for (int i = 0; i < 4; i++)
                    {
                        str3 = FrameData.Substring(8+i*2, 2);
                        if (str3 != "FF")
                        {
                            str3 = Compute.str16to10With(str3, 1, 40);
                            sysTemp.QuickGuns[i] = str3;
                        }
                        else { }  
                      
                    }

                    
                    break;
            }
            Analysiser.dataPack.sysTemp = sysTemp;
            return CsvUnits;
        }
    }
}
