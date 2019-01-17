using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using BMSTool.OtherClass;

namespace BMSTool.ReadInfos
{
   public  class ReadEpprom
    {
        public ReadEpprom()
        {
            fileName = AcquirefileName("EEPROM信息");
        }
        private string fileName;
        private StreamWriter sw;
        public void AnalysisData(string frameData, string frameID)//解析主函数
        {
            sw = EstablishConnection(fileName);//建立连接，并获取io流
            string Content = string.Empty;//只要是重新声明的变量，跳出块后都会被释放，再次使用一定重新初试化
            Content = ReadEEPROM(frameData);
            sw.WriteLine(Content);
            sw.Close();


        }
        private string ReadEEPROM(string FrameData)
        {
            string Content = "";
            string str1 = FrameData.Substring(0, 2);
            if (str1 == "FC")
            {
                Content += "保护";
            }
            else
            {
                Content += "故障";
            }
            string str2 = FrameData.Substring(2, 10);
            Content += "," + str2;
            string str3 = FrameData.Substring(12, 4);
            Content += "," + str3;
            return Content;
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
