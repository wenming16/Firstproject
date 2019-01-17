using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using BMSTool.Analysiss.MainControl.RealTime;
using BMSTool.DataPacks;
using BMSTool.OtherClass;

namespace BMSTool.Analysiss.MainControl
{
    class ID1811C0F4:RtimeAnalysis
    {
        public List<Analysiser.CsvUnitFormat> Analysis(string FrameData)
        {
            List<Analysiser.CsvUnitFormat> CsvUnits = new List<Analysiser.CsvUnitFormat>();
            Analysiser.CsvUnitFormat CsvUnit = new Analysiser.CsvUnitFormat();

            DataPack.TempTsd tempTsd = Analysiser.dataPack.tempTsd;

            String IDflag = FrameData.Substring(0, 2);
            switch (IDflag)
            {
                case "00":
                    for (int i = 0; i < 3; i++)
                    {
                        string Str = FrameData.Substring(2 * i+2, 2);
                        Str = judgeFFtoBlack(Str,1,40);
                        tempTsd.ChargeBat_Hs[i] = Str;
                    }
                    for (int i = 0; i < 3; i++)
                    {

                        string Str = FrameData.Substring(2 * i+8, 2);
                        Str = judgeFFtoBlack(Str,1,40);
                        tempTsd.ChargeBat_Ls[i] = Str;
                    }


                    break;
                case "01":
                    for (int i = 0; i < 3; i++)
                    {
                        string Str = FrameData.Substring(2 * i+2, 2);
                        Str = judgeFFtoBlack(Str,1,40);
                        tempTsd.DisChargeBat_Hs[i] = Str;
                    }
                    for (int i = 0; i < 3; i++)
                    {

                        string Str = FrameData.Substring(2 * i+8, 2);
                        Str = judgeFFtoBlack(Str,1,40);
                        tempTsd.DisChargeBat_Ls[i] = Str;
                    }
                    break;
                case "02":
                    for (int i = 0; i < 3; i++)
                    {
                        string Str = FrameData.Substring(2 * i+2, 2);
                        Str = judgeFFtoBlack(Str,1,0);
                        tempTsd.ChargeBatGap_Hs[i] = Str;
                    }
                    for (int i = 0; i < 3; i++)
                    {
                        string Str = FrameData.Substring(2 * i+8, 2);
                        Str = judgeFFtoBlack(Str,1,0);
                        tempTsd.DisChargeBatGap_Hs[i] = Str;
                    }
                    break;
                case "03":
                    for (int i = 0; i < 3; i++)
                    {
                        string Str = FrameData.Substring(2 * i+2, 2);
                        Str = judgeFFtoBlack(Str,1,40);
                        tempTsd.ChargerGun_Hs[i] = Str;
                    }
                    for (int i = 0; i < 3; i++)
                    {

                        string Str = FrameData.Substring(2 * i+8, 2);
                        Str = judgeFFtoBlack(Str,1,40);
                        tempTsd.MainBoard_Hs[i] = Str;
                    }
                    break;
            }
            Analysiser.dataPack.tempTsd = tempTsd;
            return CsvUnits;
        }
       
    }
}
