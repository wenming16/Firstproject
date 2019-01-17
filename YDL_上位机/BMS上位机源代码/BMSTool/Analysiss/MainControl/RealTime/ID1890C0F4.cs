using BMSTool.DataPacks;
using BMSTool.OtherClass;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BMSTool.Analysiss.MainControl.RealTime
{
    class ID1890C0F4
    {
        public List<Analysiser.CsvUnitFormat> Analysis(string FrameData)
        {
            List<Analysiser.CsvUnitFormat> CsvUnits = new List<Analysiser.CsvUnitFormat>();
            Analysiser.CsvUnitFormat CsvUnit = new Analysiser.CsvUnitFormat();

            DataPack dataPack = Analysiser.dataPack;

            string IDNumber = FrameData.Substring(0, 2);
            bool[,] Errors = new bool[8, 4];
            switch (IDNumber)
            {
                case "00":
                   
                    Errors = GetErrors(FrameData);
                    dataPack.BMSError = Errors;
                    break;
                case "01":
                  
                    Errors = GetErrors(FrameData);
                    dataPack.CrgerError = Errors;
                    break;
            }
            Analysiser.dataPack = dataPack;
            return CsvUnits;


           
        }
        
        private bool[,] GetErrors( string FrameData)
        {
            bool[,] Errors = new bool[8,4];
            for (int i = 0; i < 8; i++)
            {
                string Data = FrameData.Substring(  i * 2, 2);
                string BinaryData = Compute.str16to2(Data);
               
                    for (int j = 0; j < 4; j++)
                    {
                        string Bit = BinaryData.Substring(6 - 2 * j, 2);
                        if (Bit == "00")//按字节为第一维度按位数为第二维度
                        {
                            Errors[i, j] = false;
                        }
                        else
                        {
                            Errors[i, j] = true;
                        }
                    }
              
               
            }

            return Errors ;
        }

       



    }
}
