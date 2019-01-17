using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using BMSTool.Analysiss;
using BMSTool.Analysiss.FromControl;
using BMSTool.Analysiss.MainControl;
using BMSTool.Analysiss.MainControl.RealTime;
using BMSTool.DataPacks;
using BMSTool.OtherClass;
using BMSTool.ReadInfos;

namespace BMSTool.Analysiss
{
    public class Analysiser
    {

        private static ReadEpprom myReadEpprom;
        public struct CsvUnitFormat
        {
            public string Name;
            public string Value;
        }
        //静态复用不方便//但是这里暂不用考虑复用，静态更适用于低耦合
        public static DataPack dataPack = new DataPack();

        
           

        public static List< CsvUnitFormat >Main(string FrameID, string FrameData)
      {
            List<CsvUnitFormat> CsvUnits = new List<CsvUnitFormat>();

            if (FrameID.Length == 8)
            {
                bool b1 = FrameID.Contains("C0F4");//Contains:是否包含
                if (b1)//主控板
                {
                    switch (FrameID)
                    {
                        //电池基本信息
                        case "1800C0F4":
                            ID1800C0F4 iD1800C0F4 = new ID1800C0F4();
                            iD1800C0F4.Analysis(FrameData);
                            break;
                        //保护等级信息
                        case "1810C0F4":
                            ID1810C0F4 iD1810C0F4 = new ID1810C0F4();
                            iD1810C0F4.Analysis(FrameData);
                            break;
                        case "1811C0F4":
                            ID1811C0F4 iD1811C0F4 = new ID1811C0F4();
                            iD1811C0F4.Analysis(FrameData);
                            break;
                        case "1812C0F4":
                            ID1812C0F4 iD1812C0F4 = new ID1812C0F4();
                            iD1812C0F4.Analysis(FrameData);
                            break;
                        case "1813C0F4":
                            ID1813C0F4 iD1813C0F4 = new ID1813C0F4();
                            iD1813C0F4.Analysis(FrameData);
                            break;
                        //实时信息
                        case "1820C0F4":
                            ID1820C0F4 iD1820C0F4 = new ID1820C0F4();
                            iD1820C0F4.Analysis(FrameData);
                            break;
                        case "1830C0F4":
                            ID1830C0F4 iD1830C0F4 = new ID1830C0F4();
                            iD1830C0F4.Analysis(FrameData);
                            break;
                        case "1840C0F4":
                            ID1840C0F4 iD1840C0F4 = new ID1840C0F4();
                            iD1840C0F4.Analysis(FrameData);
                            break;
                        case "1850C0F4":
                            ID1850C0F4 iD1850C0F4 = new ID1850C0F4();
                            iD1850C0F4.Analysis(FrameData);
                            break;
                        case "1860C0F4":
                            ID1860C0F4 iD1860C0F4 = new ID1860C0F4();
                            iD1860C0F4.Analysis(FrameData);
                            break;
                        case "1870C0F4":
                            ID1870C0F4 iD1870C0F4 = new ID1870C0F4();
                            iD1870C0F4.Analysis(FrameData);
                            break;
                        //电池故障信息
                        case "1880C0F4":
                            ID1880C0F4 iD1880C0F4 = new ID1880C0F4();
                            iD1880C0F4.Analysis(FrameData);
                            break;
                        //充电通信故障帧
                        case "1890C0F4":
                            ID1890C0F4 iD1890C0F4 = new ID1890C0F4();
                            iD1890C0F4.Analysis(FrameData);
                            break;
                        //标定回复帧
                        case "18FFC0F4":
                           
                            break;
                        case "1610C0F4":
                            if (myReadEpprom == null)
                            {
                                MessageBox.Show("读取成功!");
                                myReadEpprom = new ReadEpprom();
                                myReadEpprom.AnalysisData(FrameData, FrameID);
                            } else
                            {
                                myReadEpprom.AnalysisData(FrameData, FrameID);
                            }
                            break;
                      //  case"1"
                    }
                }
                else
                {
                    string str = FrameID.Substring(7, 1);
                    int ModNum = Convert.ToInt32(str, 16);
                    string MatchID = FrameID.Substring(0, 7);
                    switch (MatchID)
                    {
                        case "18FF960":
                            ID18FF960 iD18FF960 = new ID18FF960();//格式帧
                            iD18FF960.Analysis(ModNum,FrameData);

                            break;
                        case "18FF970":
                            ID18FF970 iD18FF970 = new ID18FF970();
                            
                            CsvUnits = iD18FF970.Analysis( ModNum, FrameData);
                            break;
                        case "18FF971":
                            ID18FF971 iD18FF971 = new ID18FF971();
                            CsvUnits = iD18FF971.Analysis(ModNum, FrameData);
                            break;
                        case "18FF980":
                            ID18FF980 iD18FF980 = new ID18FF980();
                            CsvUnits = iD18FF980.Analysis(ModNum, FrameData);
                            break;
                        case "18FF981":
                            ID18FF981 iD18FF981 = new ID18FF981();
                            CsvUnits = iD18FF981.Analysis(ModNum, FrameData);
                            break;
                        case "18FF990":
                            ID18FF990 iD18FF990 = new ID18FF990();
                            CsvUnits = iD18FF990.Analysis(ModNum, FrameData);
                            break;
                        case "19FF990":
                            ID19FF990 iD19FF990 = new ID19FF990();
                            CsvUnits = iD19FF990.Analysis(ModNum, FrameData);
                            break;

                    }
                }
            }
            else
            { }
            
           
        return CsvUnits;
        }

       
        //解析函数
    }
    
}
