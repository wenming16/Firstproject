using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using BMSTool.OtherClass;

namespace BMSTool.ReadInfos
{
    class ReadFlash
    {
        /*
        * 因为建立连接一次后有需要断开连接
        * 重新建立时有需要按原有路径建立
        * 所以将获取文件路径名和建立连接分开为两个函数
        */
        public ReadFlash()
        {
            fileName = AcquirefileName("实时信息");
        }
        private string fileName;
        private StreamWriter sw;
        string signalID03;//本地变量不用显式赋初值？
        string signalID05;
        string contentIDO3;
        string contentID05;
        public void AnalysisData(string frameData, string frameID)
        {
            sw = EstablishConnection(fileName);//建立连接，并获取io流
            string Content = string.Empty;//只要是重新声明的变量，跳出块后都会被释放，再次使用一定重新初试化
            Content = AnalysisFlash(frameData, frameID);
            sw.WriteLine(Content);
            sw.Close();
        }
        private string AnalysisFlash(string frameData, string frameID)
        {
            string content = string.Empty;
            if (frameID == "1603F4C0")
            {
                signalID03 = frameData.Substring(0, 4);
                contentIDO3 = AnalysisID03(frameData);
            }
            if (frameID == "1605F4C0")
            {
                signalID05 = frameData.Substring(0, 4);
                contentID05 = AnalysisID05(frameData);
            }
            if (signalID03 == signalID05)
            {
                content = contentIDO3 + contentID05;
                signalID03 = signalID05 = string.Empty;
                contentIDO3 = contentID05 = string.Empty;
            }
            return content;

        }
        private string AnalysisID03(string framedata)
        {
            string content = string.Empty;
            content = CalcuDate(framedata);
            return content;
        }

        private string AnalysisID05(string frameData)//解析二级函数
        {
            string content = string.Empty;
            string TemStr = frameData.Substring(4, 4);
            string BinaryStr = Compute.str16to2(TemStr);
            string StrIdentify = BinaryStr.Substring(14, 2);
            switch (StrIdentify)
            {
                case "00":
                    content = CASE0XFC(frameData);
                    break;
                case "11":
                    content = CASE0XFD(frameData);
                    break;

            }
            return content;
        }
        private string CASE0XFC(string frameData)
        {
            string strContent = string.Empty;
            strContent = "保护" + ",";//写辅助函数优点包括：复用和使程序易读
            string TemStr = frameData.Substring(4, 4);
            string BinaryStr = Compute.str16to2(TemStr);
            string BinaryID = BinaryStr.Substring(0, 14) + "00";
            string HexadecimalID = Compute.Str2to16(BinaryID);
            int IntGrade = Convert.ToInt32(BinaryID.Substring(14, 2), 2);
            string Grade = IntGrade.ToString();
            switch (HexadecimalID)
            {
                case "0100":
                    strContent += "单体电池过压" + "," + Grade + "级" + ",";
                    string HexadecimalBatNum = frameData.Substring(8, 2);
                    string BatNum = Compute.str16to10(HexadecimalBatNum);
                    strContent += "电池编号：" + BatNum + ",";
                    string HexadecimalChipNum = frameData.Substring(10, 2);
                    string ChipNum = Compute.str16to10(HexadecimalBatNum);
                    strContent += "芯片编号：" + ChipNum + ",";
                    string HexadecimalMaxVol = frameData.Substring(12, 4);
                    HexadecimalMaxVol = Compute.StrInvert(HexadecimalMaxVol);
                    double MaxVol = Convert.ToInt32(HexadecimalMaxVol, 16) * 0.0001;
                    strContent += "单体最大电压" + MaxVol + "V" + ";";
                    break;

                case "0108":
                    strContent += "单体电池欠压" + "," + Grade + "级" + ",";
                    HexadecimalBatNum = frameData.Substring(8, 2);
                    BatNum = Compute.str16to10(HexadecimalBatNum);
                    strContent += "电池编号：" + BatNum + ",";
                    HexadecimalChipNum = frameData.Substring(10, 2);
                    ChipNum = Compute.str16to10(HexadecimalBatNum);
                    strContent += "芯片编号：" + ChipNum + ",";
                    string HexadecimalMinVol = frameData.Substring(12, 4);
                    HexadecimalMinVol = Compute.StrInvert(HexadecimalMinVol);
                    double MinVol = Convert.ToInt32(HexadecimalMinVol, 16) * 0.0001;
                    strContent += "单体最小电压" + MinVol + "V" + ",";
                    break;
                case "0110":
                    strContent += "单体压差" + "，" + Grade + "级" + ",";
                    string BatMinVol = frameData.Substring(8, 4);
                    BatMinVol = Compute.StrInvert(BatMinVol);
                    BatMinVol = (Convert.ToInt32(BatMinVol, 16) * 0.0001).ToString();
                    strContent += "最小单体电压" + BatMinVol + "V" + ",";
                    String BatMaxVol = frameData.Substring(12, 4);
                    BatMaxVol = Compute.StrInvert(BatMaxVol);
                    BatMaxVol = (Convert.ToUInt32(BatMaxVol, 16) * 0.0001).ToString();
                    strContent += "最大单体电压" + BatMaxVol + "V" + ",";

                    break;
                case "0300":
                    strContent += "总压过高" + "," + Grade + "级" + ",";
                    strContent += CASE0300or0308(frameData);
                    break;
                case "0308":
                    strContent += "总压过低" + "," + Grade + "级" + ",";
                    strContent += CASE0300or0308(frameData);
                    break;
                case "0310":
                    strContent += "系统压差过大" + "," + Grade + "级" + ",";
                    break;
                case "1000":
                    strContent += "电芯在静置状态温度过高" + "," + Grade + "级" + ",";
                    break;
                case "1008":
                    strContent += "电芯在静置状态温度过低" + "," + Grade + "级" + ",";
                    break;
                case "1010":
                    strContent += "电芯在静置状态温差过大" + "," + Grade + "级" + ",";
                    break;
                case "1020":
                    strContent += "电芯在充电状态温度过高" + "," + Grade + "级" + ",";
                    break;
                case "1028":
                    strContent += "电芯在充电状态温度过低" + "," + Grade + "级" + ",";
                    break;
                case "1030":
                    strContent += "电芯在充电状态温差过大" + "," + Grade + "级" + ",";
                    break;
                case "1040":
                    strContent += "电芯在放电状态温度过高" + "," + Grade + "级" + ",";
                    break;
                case "1048":
                    strContent += "电芯在放电状态温度过低" + "," + Grade + "级" + ",";
                    break;
                case "1050":
                    strContent += "电芯在放电状态温差过大" + "," + Grade + "级" + ",";
                    break;
                case "1100":
                    strContent += "极柱在静置状态温差过大" + "," + Grade + "级" + ",";
                    strContent += CASEHighTemp(frameData);
                    break;
                case "1108":
                    strContent += "极柱在静置状态温差过低" + "," + Grade + "级" + ",";
                    strContent += CASELowTemp(frameData);
                    break;
                case "1110":
                    strContent += "极柱在静置状态温差过大" + "," + Grade + "级" + ",";
                    strContent += CASEHighTemp(frameData);
                    break;
                case "1120":
                    strContent += "极柱在充电状态温度过高" + "," + Grade + "级" + ",";
                    strContent += CASEHighTemp(frameData);
                    break;
                case "1128":
                    strContent += "极柱在充电状态温度过低" + "," + Grade + "级" + ",";
                    strContent += CASELowTemp(frameData);
                    break;
                case "1130":
                    strContent += "极柱在充电状态温差过大" + "," + Grade + "级" + ",";
                    strContent += CASEHighTemp(frameData);
                    break;
                case "1140":
                    strContent += "极柱在放电温度过高" + "," + Grade + "级" + ",";
                    strContent += CASEHighTemp(frameData);
                    break;
                case "1148":
                    strContent += "极柱在放电温度过低" + "," + Grade + "级" + ",";
                    strContent += CASELowTemp(frameData);
                    break;
                case "1150":
                    strContent += "极柱在放电温差过大" + "," + Grade + "级" + ",";
                    strContent += CASEHighTemp(frameData);
                    break;
                case "1220":
                    strContent += "高压导线充电状态过温" + "," + Grade + "级" + ",";
                    break;
                case "1240":
                    strContent += "高压导线放电状态过温" + "," + Grade + "级" + ",";
                    break;
                case "1320":
                    strContent += "充电枪充电状态过温" + "," + Grade + "级" + ",";
                    break;
                case "1400":
                    strContent += "电池包静置状态过温" + "," + Grade + "级" + ",";
                    break;
                case "1408":
                    strContent += "电池包静置状态低温" + "," + Grade + "级" + ",";
                    break;
                case "1410":
                    strContent += "电池包静置状态温差过大" + "," + Grade + "级" + ",";
                    break;
                case "1420":
                    strContent += "电池包充电状态过温" + "," + Grade + "级" + ",";
                    break;
                case "1428":
                    strContent += "电池包充电状态低温" + "," + Grade + "级" + ",";
                    break;
                case "1430":
                    strContent += "电池包充电状态温差过大" + "," + Grade + "级" + ",";
                    break;
                case "1440":
                    strContent += "电池包在放电温度过高" + "," + Grade + "级" + ",";
                    break;
                case "1448":
                    strContent += "电池包在放电温度过低" + "," + Grade + "级" + ",";
                    break;
                case "1450":
                    strContent += "电池包在放电温差过大" + "," + Grade + "级" + ",";
                    break;
                case "2000":
                    strContent += "电流在充电状态过流" + "," + Grade + "级" + ",";

                    break;
                case "2020":
                    strContent += "电流在放电状态过流" + "," + Grade + "级" + ",";
                    // strContent += CASE0200(frameData);
                    break;
                case "3000":
                    strContent += "绝缘正极对地故障" + "," + Grade + "级" + ",";
                    break;
                case "3008":
                    strContent += "绝缘负极对地故障" + "," + Grade + "级" + ",";
                    break;
                case "3010":
                    strContent += "绝缘故障短路" + ",";
                    switch (Grade)
                    {
                        case "1":
                            strContent += "正极短路" + ",";
                            break;
                        case "2":
                            strContent += "负极短路" + ",";
                            break;
                        case "3":
                            strContent += "正负极短路" + ",";
                            break;
                    }
                    break;
                case "3100":
                    strContent += "SOC过高" + "," + Grade + "级" + ",";
                    break;
                case "3108":
                    strContent += "SOC过低" + "," + Grade + "级" + ",";
                    break;
                case "3200":
                    strContent += "SOH性能低" + "," + Grade + "级" + ",";
                    break;
                case "3220":
                    strContent += "SOH容量低" + "," + Grade + "级" + ",";
                    break;
                case "3300":
                    strContent += "SOP保护" + "," + Grade + "级" + ",";
                    break;
                case "3400":
                    strContent += "电池内阻" + "," + Grade + "级" + ",";
                    break;
                case "3500":
                    strContent += "气体气压过高" + "," + Grade + "级" + ",";
                    break;
                case "3520":
                    strContent += "气体成分异常" + "," + Grade + "级" + ",";
                    break;
                case "3540":
                    strContent += "箱体湿度过大" + "," + Grade + "级" + ",";
                    break;
                case "3560":
                    strContent += "箱体电解液泄露" + "," + Grade + "级" + ",";
                    break;

            }

            return strContent;

        }

        private string CASE0300or0308(string frameData)
        {
            string content = string.Empty;
            string TestAllVolt = frameData.Substring(8, 4);
            TestAllVolt = Compute.StrInvert(TestAllVolt);
            TestAllVolt = (Convert.ToInt32(TestAllVolt, 16) * 0.1).ToString();
            content += "检测总压" + TestAllVolt + "V" + ",";
            string AccumuAllVolt = frameData.Substring(8, 4);
            AccumuAllVolt = Compute.StrInvert(AccumuAllVolt);
            AccumuAllVolt = (Convert.ToInt32(AccumuAllVolt, 16) * 0.1).ToString();
            content += "累计总压" + AccumuAllVolt + "V" + ",";
            return content;
        }
        private string CASE2000(string frameData)
        {
            string content = string.Empty;
            string BatMaxVol = frameData.Substring(8, 4);
            BatMaxVol = Compute.StrInvert(BatMaxVol);
            BatMaxVol = (Convert.ToInt32(BatMaxVol, 16) * 0.001).ToString();
            content += "单体最大电压" + BatMaxVol + "V";
            //string Charge
            return content;
        }
        private string CASEHighTemp(string frameData)
        {
            string content = string.Empty;
            string BatTempNum = frameData.Substring(8, 2);
            BatTempNum = Compute.str16to10(BatTempNum);
            content += "高温编号" + BatTempNum + ",";
            string ChipTempNum = frameData.Substring(10, 2);
            content += "高温芯片编号" + ChipTempNum + ",";
            string MinTemp = frameData.Substring(12, 2);
            MinTemp = (Convert.ToInt32(MinTemp, 16) - 40).ToString();
            content += "最低温度" + MinTemp + ",";
            string MaxTemp = frameData.Substring(14, 2);
            MaxTemp = (Convert.ToInt32(MaxTemp, 16) - 40).ToString();
            content += "最高温度" + MaxTemp + ",";
            return content;
        }
        private string CASELowTemp(string frameData)
        {
            string content = string.Empty;
            string BatTempNum = frameData.Substring(8, 2);
            BatTempNum = Compute.str16to10(BatTempNum);
            content += "低温编号" + BatTempNum + ",";
            string ChipTempNum = frameData.Substring(10, 2);
            content += "低温芯片编号" + ChipTempNum + ",";
            string MinTemp = frameData.Substring(12, 2);
            MinTemp = (Convert.ToInt32(MinTemp, 16) - 40).ToString();
            content += "最低温度" + MinTemp + ",";
            string MaxTemp = frameData.Substring(14, 2);
            MaxTemp = (Convert.ToInt32(MaxTemp, 16) - 40).ToString();
            content += "最高温度" + MaxTemp + ",";
            return content;
        }
        private string CASE0XFD(string frameData)
        {
            string strContent = string.Empty;
            strContent = "故障" + ",";
            strContent += CalcuDate(frameData) + ",";//解析日期

            string TemStr = frameData.Substring(12, 4);//具体分析故障类别
            TemStr = Compute.str16to2(TemStr);
            string category = TemStr.Substring(0, 4);

            string IDflag = TemStr.Substring(4, 12) + "0000";
            IDflag = Convert.ToInt32(IDflag, 2).ToString("X4");//最后转为4位16进制
            string[] Categorys = new string[4];
            switch (IDflag)
            {
                case "C000":
                    strContent += "程序卡死" + ",";
                    break;
                case "C100":
                    strContent += "任务没有执行" + ",";
                    break;
                case "D000":
                    strContent += "电源模块故障" + ",";
                    break;
                case "D010":
                    strContent += "主芯片部分" + ",";
                    break;
                case "D020":
                    strContent += "时钟模块" + ",";
                    break;
                case "D030":
                    strContent += "存储模块" + ",";

                    for (int i = 0; i < 4; i++)
                    {
                        Categorys[i] = category.Substring(4 - i, 1);
                        switch (i)
                        {
                            case 0:
                                if (Categorys[0] == "1")
                                {
                                    strContent += "EEPROM故障" + ",";
                                }
                                break;
                            case 1:
                                if (Categorys[1] == "1")
                                {
                                    strContent += "外部Flash故障" + ",";
                                }
                                break;
                            case 2:
                                if (Categorys[2] == "1")
                                {
                                    strContent += "SD卡故障" + ",";
                                }
                                break;
                        }
                    }



                    break;
                case "D040":
                    strContent += "信号检测电路" + ",";
                    break;
                case "D050":
                    strContent += "驱动模块";
                    break;
                case "D10F":
                    strContent += "单体电压导线检测" + ",";

                    for (int i = 0; i < 4; i++)
                    {
                        Categorys[i] = category.Substring(4 - i, 1);
                        switch (i)
                        {
                            case 0:
                                if (Categorys[0] == "1")
                                {
                                    strContent += "单体0号导线开路" + ",";
                                }
                                break;
                            case 1:
                                if (Categorys[1] == "1")
                                {
                                    strContent += "单体1号导线开路" + ",";
                                }
                                break;
                            case 2:
                                if (Categorys[2] == "1")
                                {
                                    strContent += "单体2号导线开路" + ",";
                                }
                                break;
                            case 3:
                                if (Categorys[3] == "1")
                                {
                                    strContent += "单体3号导线开路" + ",";
                                }
                                break;
                        }
                    }
                    break;
                case "D11F":
                    strContent += "外部温度检测" + ",";
                    break;
                case "D12F":
                    strContent += "内部温度检测" + ",";
                    break;
                case "D13F":
                    strContent += "均衡电路检测" + ",";
                    for (int i = 0; i < 4; i++)
                    {
                        Categorys[i] = category.Substring(4 - i, 1);
                        switch (i)
                        {
                            case 0:
                                if (Categorys[0] == "1")
                                {
                                    strContent += "单体均衡电路0号电路故障" + ",";
                                }
                                break;
                            case 1:
                                if (Categorys[1] == "1")
                                {
                                    strContent += "单体均衡电路1号电路故障" + ",";
                                }
                                break;
                            case 2:
                                if (Categorys[2] == "1")
                                {
                                    strContent += "单体均衡电路2号电路故障" + ",";
                                }
                                break;
                            case 3:
                                if (Categorys[3] == "1")
                                {
                                    strContent += "单体均衡电路3号电路故障" + ",";
                                }
                                break;
                        }
                    }
                    break;
                case "D200":
                    strContent += "高压信号总压检测" + ",";
                    break;
                case "D230":
                    strContent += "绝缘电路检测" + ",";
                    break;
                case "E300":
                    strContent += "CAN断路通信" + ",";
                    break;
                case "E310":
                    strContent += "485通信断路" + ",";
                    break;
                case "E320":
                    strContent += "232断路通信" + ",";
                    break;
                case "E330":
                    strContent += "CAN短路" + ",";
                    break;
                case "E340":
                    strContent += "485短路" + ",";
                    break;
                case "E350":
                    strContent += "232断路通信" + ",";
                    break;
                case "D330":
                    strContent += "GPRS通信" + ",";
                    break;
                case "D400":
                    strContent += "线路板温度过高" + ",";
                    break;
                case "D410":
                    strContent += "线路板温度过低" + ",";
                    break;
                case "E200":
                    strContent += "点火线断路/短路/信号异常" + ",";
                    break;
                case "E220":
                    strContent += "充电线断路/短路/信号异常" + ",";
                    for (int i = 0; i < 4; i++)
                    {
                        Categorys[i] = category.Substring(4 - i, 1);
                        switch (i)
                        {
                            case 0:
                                if (Categorys[0] == "1")
                                {
                                    strContent += "充电线接地未与箱体连接" + ",";
                                }
                                break;
                        }
                    }
                    break;
                case "E230":
                    strContent += "预充电线断路/短路/信号异常" + ",";
                    break;
                case "E800":
                    strContent += "12/24V电源高" + ",";
                    break;
                case "E820":
                    strContent += "12/24V电源低" + ",";
                    break;
                case "E900":
                    strContent += "电流传感器故障" + ",";
                    break;
                case "EA00":
                    strContent += "主负继电器故障" + ",";
                    break;
                case "EA10":
                    strContent += "主正继电器故障" + ",";
                    break;
                case "EA20":
                    strContent += "预充继电器故障" + ",";
                    break;
                case "EA30":
                    strContent += "快充继电器故障" + ",";
                    break;
                case "F000":
                    strContent += "累积电压与总压检测差值过大" + ",";
                    break;
                case "F010":
                    strContent += "电池包电压变化超出限制" + ",";
                    break;
                case "F100":
                    strContent += "电池单体最高和最低温度表现强相关性" + ",";
                    break;
                case "F200":
                    strContent += "测量到的电流值与逆变器电流差值过大" + ",";
                    break;




            }

            return strContent;
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
        private string CalcuDate(string frameData)
        {
            string date = string.Empty;
            date += frameData.Substring(4, 2) + "年";
            date += frameData.Substring(6, 2) + "月";
            date += frameData.Substring(8, 2) + "日";
            date += frameData.Substring(10, 2) + "时";
            date += frameData.Substring(12, 2) + "分";
            date += frameData.Substring(14, 2) + "秒";
            return date;
        }
    }
}
