using BMSTool.Analysiss.MainControl.RealTime;
using BMSTool.DataPacks;
using BMSTool.OtherClass;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BMSTool.Analysiss.MainControl
{
    class ID1813C0F4:RtimeAnalysis
    {
        public List<Analysiser.CsvUnitFormat> Analysis(string FrameData)
        {
            List<Analysiser.CsvUnitFormat> CsvUnits = new List<Analysiser.CsvUnitFormat>();
            Analysiser.CsvUnitFormat CsvUnit = new Analysiser.CsvUnitFormat();


            string []InsualTsds= Analysiser.dataPack.InsulaTsds;
            string str1 = FrameData.Substring(0, 2);
            switch (str1)
            {
                case "00":
                    for (int i = 0; i < 3; i ++)
                    {
                        String str = FrameData.Substring(2+4 * i, 4);
                        str = judgeFFtoBlack(str,0.1, 0); 
                        InsualTsds[i] = str;
                    
                    }
                    break;
             
            }
            Analysiser.dataPack.InsulaTsds = InsualTsds;




            return CsvUnits;
        }
    }
}
