using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using BMSTool.OtherClass;

namespace BMSTool.ReadInfos
{
    class ReadRealTimeInfo
    {
        // 读取实时信息 
        public ReadRealTimeInfo()
        {
            fileName = AcquirefileName("实时信息");
            sw = EstablishConnection(fileName);
            ExcelUnits = InitialUnits();
            WriteExcelHead(ExcelUnits, sw); //写表头     
        }

        private string fileName;
        private ExcelFormat[] ExcelUnits;
        private StreamWriter sw;
        //类型声明
        private struct ExcelFormat//可以使用数组索引关联表头和内容，但是不方便索引
        {
            public string name;
            public string content;
        }


        private static ExcelFormat[] InitialUnits()//初试化实时信息单元格数组
        {
            ExcelFormat[] temExcelUnits = new ExcelFormat[74];//能直接赋值数组吗？
            for (int i = 0; i < temExcelUnits.Length; i++)
            {
                temExcelUnits[i].name = string.Empty;
                temExcelUnits[i].content = "0";
            }
            temExcelUnits[0].name = "时间";
            temExcelUnits[1].name = "SOC";
            temExcelUnits[2].name = "预充继电器状态";
            temExcelUnits[3].name = "主负继电器状态";
            temExcelUnits[4].name = "主正继电器状态";
            temExcelUnits[5].name = "快充继电器状态";

            temExcelUnits[6].name = "电流";
            temExcelUnits[7].name = "总压";
            temExcelUnits[8].name = "绝缘电阻";
            temExcelUnits[9].name = "绝缘电压正极对地电压";
            temExcelUnits[10].name = "绝缘电压负极对地电压";
            temExcelUnits[11].name = "最大单体电压";
            temExcelUnits[12].name = "最小单体电压";
            temExcelUnits[13].name = "平均单体电压";
            int count = 14;
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 12; j++)
                {
                    temExcelUnits[count].name = "芯片" + i + "号的" + j + "号单体电池电压";
                    count++;
                }

            }
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 6; j++)
                {
                    temExcelUnits[count].name = "芯片" + i + "号的" + j + "号温度";
                    count++;
                }
            }
            for (int i = 0; i < 3; i++)
            {
                temExcelUnits[count].name = "芯片" + i + "自身的温度";
                count++;
            }
            temExcelUnits[count].name = "Pack平均温度";
            count++;
            temExcelUnits[count].name = "运行时间";
            count++;
            temExcelUnits[count].name = "SOH";

            return temExcelUnits;
        }


        public void AnalysisData(string frameData, string frameId)//解析主函数
        {

            ExcelUnits = RealTimeAnalysis(frameData, ExcelUnits);//解析实时信息
            bool flag = isRecSequence(ExcelUnits);
            string IDflag = frameData.Substring(2, 2);
            //写内容
            if (IDflag == "55" && flag)
            {
                sw = EstablishConnection(fileName);
                WriteExcelContent(ExcelUnits);
            }

        }
        private ExcelFormat[] RealTimeAnalysis(string frameData, ExcelFormat[] ExcelUnits)  //解析实时信息
        {
            //通过第0个字节找到属于哪一系列
            string temStr = frameData.Substring(0, 2);
            string IdFlag = frameData.Substring(2, 2);

            //使用中间变量不能一次性把值放进去，所以还是直接索引

            switch (IdFlag)
            {
                case "01":
                    temStr = frameData.Substring(4, 2) + "年";
                    temStr += frameData.Substring(6, 2) + "月";
                    temStr += frameData.Substring(8, 2) + "日";
                    temStr += frameData.Substring(10, 2) + "时";
                    temStr += frameData.Substring(12, 2) + "分";
                    temStr += frameData.Substring(14, 2) + "秒";
                    ExcelUnits = SetData2ExcelRam("时间", temStr, ExcelUnits);
                    break;
                case "02":
                    temStr = frameData.Substring(4, 2);
                    temStr = Compute.str16to10(temStr) + "%";
                    ExcelUnits = SetData2ExcelRam("SOC", temStr, ExcelUnits);
                    temStr = frameData.Substring(6, 2);
                    int temInt = Convert.ToInt32(temStr, 16);
                    temStr = Convert.ToString(temInt, 2);
                    string temStr0 = temStr.Substring(6, 1);
                    if (temStr0 == "0")
                    {
                        temStr0 = "ON";
                    }
                    if (temStr0 == "1")
                    {
                        temStr0 = "OFF";
                    }
                    ExcelUnits = SetData2ExcelRam("预充继电器状态", temStr0, ExcelUnits);
                    temStr0 = temStr.Substring(5, 1);
                    if (temStr0 == "0")
                    {
                        temStr0 = "ON";
                    }
                    if (temStr0 == "1")
                    {
                        temStr0 = "OFF";
                    }
                    ExcelUnits = SetData2ExcelRam("主负继电器状态", temStr0, ExcelUnits);
                    temStr0 = temStr.Substring(4, 1);
                    if (temStr0 == "0")
                    {
                        temStr0 = "ON";
                    }
                    if (temStr0 == "1")
                    {
                        temStr0 = "OFF";
                    }
                    ExcelUnits = SetData2ExcelRam("主正继电器状态", temStr0, ExcelUnits);
                    temStr0 = temStr.Substring(3, 1);
                    if (temStr0 == "0")
                    {
                        temStr0 = "ON";
                    }
                    if (temStr0 == "1")
                    {
                        temStr0 = "OFF";
                    }
                    ExcelUnits = SetData2ExcelRam("快充继电器状态", temStr0, ExcelUnits);
                    temStr = frameData.Substring(8, 4);
                
                    temStr = Compute.str16to10(temStr);
                    temStr = (Convert.ToInt32(temStr, 10) - 750).ToString();
                    ExcelUnits = SetData2ExcelRam("电流", temStr, ExcelUnits);
                    temStr = frameData.Substring(12, 4);
            
                    temStr = Compute.str16to10(temStr);
                    temStr = (Convert.ToInt32(temStr, 10) * 0.1).ToString();
                    ExcelUnits = SetData2ExcelRam("总压", temStr, ExcelUnits);

                    break;
                case "03":
                    temStr = frameData.Substring(4, 4);
           
                    temStr = Compute.str16to10(temStr);
                    ExcelUnits = SetData2ExcelRam("绝缘电阻", temStr, ExcelUnits);
                    temStr = frameData.Substring(8, 4);
            
                    temStr = Compute.str16to10(temStr);
                    temStr = (Convert.ToInt32(temStr) * 0.1).ToString();
                    ExcelUnits = SetData2ExcelRam("绝缘电压正极对地电压", temStr, ExcelUnits);
                    temStr = frameData.Substring(12, 4);
              
                    temStr = Compute.str16to10(temStr);
                    temStr = (Convert.ToInt32(temStr) * 0.1).ToString();
                    ExcelUnits = SetData2ExcelRam("绝缘电压负极对地电压", temStr, ExcelUnits);
                    break;
                case "04":
                    temStr = frameData.Substring(4, 4);
                    
                    temStr = Compute.str16to10(temStr);
                    temStr = (Convert.ToInt32(temStr) * 0.0001).ToString();
                    ExcelUnits = SetData2ExcelRam("最大单体电压", temStr, ExcelUnits);
                    temStr = frameData.Substring(8, 4);
             
                    temStr = Compute.str16to10(temStr);
                    temStr = (Convert.ToInt32(temStr) * 0.0001).ToString();
                    ExcelUnits = SetData2ExcelRam("最小单体电压", temStr, ExcelUnits);
                    temStr = frameData.Substring(12, 4);
                 
                    temStr = Compute.str16to10(temStr);
                    temStr = (Convert.ToInt32(temStr) * 0.0001).ToString();
                    ExcelUnits = SetData2ExcelRam("平均单体电压", temStr, ExcelUnits);
                    break;
                case "05":
                    ExcelUnits = UnitVoltAnalysis(0, frameData, 0, ExcelUnits);
                    break;
                case "06":
                    ExcelUnits = UnitVoltAnalysis(0, frameData, 3, ExcelUnits);
                    break;
                case "07":
                    ExcelUnits = UnitVoltAnalysis(0, frameData, 6, ExcelUnits);
                    break;
                case "08":
                    ExcelUnits = UnitVoltAnalysis(0, frameData, 9, ExcelUnits);
                    break;
                case "09":
                    ExcelUnits = UnitVoltAnalysis(1, frameData, 0, ExcelUnits);
                    break;
                case "0A":
                    ExcelUnits = UnitVoltAnalysis(1, frameData, 3, ExcelUnits);
                    break;
                case "0B":
                    ExcelUnits = UnitVoltAnalysis(1, frameData, 6, ExcelUnits);
                    break;
                case "0C":
                    ExcelUnits = UnitVoltAnalysis(1, frameData, 9, ExcelUnits);
                    break;
                case "0D":
                    ExcelUnits = UnitVoltAnalysis(2, frameData, 0, ExcelUnits);
                    break;
                case "0E":
                    ExcelUnits = UnitVoltAnalysis(2, frameData, 3, ExcelUnits);
                    break;
                case "0F":
                    ExcelUnits = UnitVoltAnalysis(2, frameData, 6, ExcelUnits);
                    break;
                case "10":
                    ExcelUnits = UnitVoltAnalysis(2, frameData, 9, ExcelUnits);
                    break;
                case "11":
                    ExcelUnits = ChipTempAnalysis(0, frameData, ExcelUnits);
                    break;
                case "12":
                    ExcelUnits = ChipTempAnalysis(1, frameData, ExcelUnits);
                    break;
                case "13":
                    ExcelUnits = ChipTempAnalysis(2, frameData, ExcelUnits);
                    break;
                case "14":
                    temStr = frameData.Substring(4, 2);
                    temStr = (Convert.ToInt32(temStr, 16) - 40).ToString();
                    ExcelUnits = SetData2ExcelRam("Pack平均温度", temStr, ExcelUnits);
                    temStr = frameData.Substring(6, 2);
                    temStr = (Convert.ToInt32(temStr, 16) - 40).ToString();
                    ExcelUnits = SetData2ExcelRam("芯片0自身的温度", temStr, ExcelUnits);
                    temStr = frameData.Substring(8, 2);
                    temStr = (Convert.ToInt32(temStr, 16) - 40).ToString();
                    ExcelUnits = SetData2ExcelRam("芯片1自身的温度", temStr, ExcelUnits);
                    temStr = frameData.Substring(10, 2);
                    temStr = (Convert.ToInt32(temStr, 16) - 40).ToString();
                    ExcelUnits = SetData2ExcelRam("芯片2自身的温度", temStr, ExcelUnits);
                    break;
                case "15":
                    temStr = frameData.Substring(4, 4);
                
                    temStr = Compute.str16to10(temStr);
                    ExcelUnits = SetData2ExcelRam("运行时间", temStr, ExcelUnits);
                    temStr = frameData.Substring(8, 2);
                    temStr = Compute.str16to10(temStr) + "%";
                    ExcelUnits = SetData2ExcelRam("SOH", temStr, ExcelUnits);
                    break;
            }
            return ExcelUnits;
            //通过第1个字节找到属于哪一帧报文
        }
        private bool isRecSequence(ExcelFormat[] ExcelUnits)//是否接受完正数据
        {
            bool flag = true;
            for (int i = 0; i < ExcelUnits.Length; i++)
            {
                if (ExcelUnits[i].content == "0")
                {
                    flag = false;
                }
            }
            return flag;
        }

        ExcelFormat[] UnitVoltAnalysis(int ChipNum, string frameData, int StartNum, ExcelFormat[] ExcelUnits)//单体电池电压解析
        {
            for (int UnitNum = 4; UnitNum < 16; UnitNum += 4)
            {
                string temStr = frameData.Substring(UnitNum, 4);
               
                temStr = Compute.str16to10(temStr);
                temStr = (Convert.ToInt32(temStr) * 0.0001).ToString();
                string strName = "芯片" + ChipNum + "号" + "的" + ((UnitNum - 4) / 4 + StartNum) + "号单体电池电压";
                ExcelUnits = SetData2ExcelRam(strName, temStr, ExcelUnits);
            }
            return ExcelUnits;
        }

        ExcelFormat[] ChipTempAnalysis(int ChipNum, string framedata, ExcelFormat[] ExcelUnits)//芯片温度函数解析
        {
            for (int TempNum = 4; TempNum < 16; TempNum += 2)
            {
                string temStr = framedata.Substring(TempNum, 2);
                temStr = (Convert.ToInt32(temStr, 16) - 40).ToString();
                string strName = "芯片" + ChipNum + "号" + "的" + ((TempNum - 4) / 2) + "号温度";
                SetData2ExcelRam(strName, temStr, ExcelUnits);

            }
            return ExcelUnits;
        }

        ExcelFormat[] SetData2ExcelRam(string name, string data, ExcelFormat[] ExcelUnits)
        {
            for (int i = 0; i < ExcelUnits.Length; i++)
            {
                if (name == ExcelUnits[i].name)
                {
                    ExcelUnits[i].content = data;
                }

            }
            return ExcelUnits;
        }

        private void WriteExcelContent(ExcelFormat[] ExcelUnits)//写该序列文件到Excel
        {
            string strContent = string.Empty;
            for (int i = 0; i < ExcelUnits.Length; i++)
            {
                strContent += ExcelUnits[i].content + ",";
            }
            if (sw != null)
            {
                sw.WriteLine(strContent);
                sw.Close();


            }
        }
        private void WriteExcelHead(ExcelFormat[] ExcelUnits, StreamWriter sw)//给Excel写表头

        {
            string strName = string.Empty;
            for (int i = 0; i < ExcelUnits.Length; i++)
            {
                strName += ExcelUnits[i].name + ",";
            }
            if (sw != null)
            {
                sw.WriteLine(strName);
                sw.Dispose();
            }

            // EstablishConnection();

        }
        private StreamWriter EstablishConnection(string fileName)//建立连接
        {

            StreamWriter sw;
            if (!File.Exists(fileName))//创建文件流
            {
                FileStream fs = new FileStream(fileName, FileMode.OpenOrCreate, FileAccess.Write, FileShare.ReadWrite);//new FileStream(fileName,FileAccess.ReadWrite,false);//System.IO.File.Create(fileName);
                sw = new StreamWriter(fs, System.Text.Encoding.UTF8);
            }
            else
            {
                FileStream fs = new FileStream(fileName, FileMode.Append, FileAccess.Write, FileShare.Read);
                sw = new StreamWriter(fs, System.Text.Encoding.UTF8);
            }

            return sw;
        }
        private string AcquirefileName(string InfoType)//文件夹和文件的名字填全
        {
            string fileFolderPath = System.AppDomain.CurrentDomain.SetupInformation.ApplicationBase;
            fileFolderPath += InfoType + "数据";
            if (!Directory.Exists(fileFolderPath))
            {
                Directory.CreateDirectory(fileFolderPath);
            }
            string fileName = InfoType + DateTime.Now.ToLongTimeString().ToString().Replace(":", "_");
            fileName = fileFolderPath + "//" + fileName;
            //fileName = System.IO.Path.Combine(fileFolderPath, fileName);//这个来将目录合并是一个很好的方法,这里的“/”是一种转义字符，所以需要两个
            fileName += ".csv";
            return fileName;
        }

    }
}
