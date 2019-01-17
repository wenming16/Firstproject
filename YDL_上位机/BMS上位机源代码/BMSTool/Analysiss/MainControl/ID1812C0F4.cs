using BMSTool.Analysiss.MainControl.RealTime;
using BMSTool.DataPacks;
using BMSTool.OtherClass;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BMSTool.Analysiss.MainControl
{
    class ID1812C0F4:RtimeAnalysis
    {
        public List<Analysiser.CsvUnitFormat> Analysis(string FrameData)
        {
            List<Analysiser.CsvUnitFormat> CsvUnits = new List<Analysiser.CsvUnitFormat>();
            Analysiser.CsvUnitFormat CsvUnit = new Analysiser.CsvUnitFormat();


            DataPack.CurTsd curTsd = Analysiser.dataPack.curTsd;
            string str1 = FrameData.Substring(0, 2);
            switch (str1)
            {
                case "00":
                    for (int i = 0; i < 3; i ++)
                    {
                        String str = FrameData.Substring(2+4 * i, 4);
                        str = judgeFFtoBlack(str,0.1,750);
                        curTsd.SysCrgCur_Hs[i] = str;
                        
                    }
                    break;
                case "01":
                    for (int i = 0; i < 3; i ++)
                    {
                        String str = FrameData.Substring(2+4*i, 4);
                        str = judgeFFtoBlack(str, 0.1, 750);
                        curTsd.SysDisCrgCur_Hs[i] = str;
                        
                    }
                    break;
                case "02":
                    DataPack.SocTsd[] socTsd = Analysiser.dataPack.socTsd;
                    for (int i = 0; i < 3; i++)
                    {
                        String str = FrameData.Substring(2 + 2 * i, 2);
                        str = judgeFFtoBlack(str, 1, 0);
                        socTsd[i].Low = str ;   
                    }
                    for (int i = 0; i < 3; i++)
                    {
                        String str = FrameData.Substring(8 + 2 * i, 2);
                        str = judgeFFtoBlack(str, 1, 0);
                        socTsd[i].High = str ;
                    }
                    break;
            }

           

            Analysiser.dataPack.curTsd = curTsd;
            return CsvUnits;
        }
       
    }
}
