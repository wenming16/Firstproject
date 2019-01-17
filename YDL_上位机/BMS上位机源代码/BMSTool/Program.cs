using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using System.IO;
using System.Collections;
using System.Text.RegularExpressions;
using System.Net.NetworkInformation;
using System.Threading;

namespace BMSTool
{
    static class Program
    {
        /// <summary>
        /// 应用程序的主入口点。
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Judges myJudges = new Judges();
            if (myJudges.IsOpenMainfrm())
            {
                BrowserForm bForm = new BrowserForm();
                bForm.UserType = myJudges.Type;
                Application.Run(bForm);
            }
            else
            {
                MessageBox.Show("软件许可证错误!");
            }
        }
    }
    //读取文本校核license

    //输出一个bool类型和三个string类型
    public struct FeatureValue
    {
        public string Type;
        public int Count;
        public string DateTime;
        public ArrayList MacAddress;
    }
    class Reader
    {
        //读文本
        public FeatureValue ReadandMatch(string FilePath, FeatureValue featureValue)
        {
            FileStream fs = new FileStream(FilePath, FileMode.Open);
            StreamReader sr = new StreamReader(fs, System.Text.Encoding.Default);
            string line = sr.ReadLine();

            while (line != null)
            {
                featureValue = match(line, featureValue);
                line = sr.ReadLine();
            }
            sr.Close();
            fs.Close();
            return featureValue;
        }
        private FeatureValue match(string line, FeatureValue myFeatureValue)
        {

            string pattern_type = ".*09HXYA-(?<type>\\w+)@(?<count>\\d+)-@chinapower \\..*";
            string pattern_datetime = ".*09HXYA-(?<datetime>\\d{4}-\\d{2}-\\d{2})-ABC\\..*";
            string pattern_mac = ".*09HXYA-mac:(?<mac>.*)-513\\..*";//这一条最开始没有
            Regex regex_type_count = new Regex(pattern_type);
            Regex regex_datetime = new Regex(pattern_datetime);
            Regex regex_mac = new Regex(pattern_mac);
            Match match_type_count = regex_type_count.Match(line);
            Match match_datetime = regex_datetime.Match(line);
            Match match_mac = regex_mac.Match(line);
            
            if (match_type_count.Success)
            {
                myFeatureValue.Type = match_type_count.Groups[1].Value;
                myFeatureValue.Count = Convert.ToInt32(match_type_count.Groups[2].Value);
            }
            else { }

            if (match_datetime.Success)
            {
                myFeatureValue.DateTime = match_datetime.Groups[1].Value;

            }
            else { }
            if (myFeatureValue.MacAddress.Count == 0)
            {
                string StrLocalMac = ReadLocalMac();
                myFeatureValue.MacAddress.Add(StrLocalMac);
            }
            { }
            if (match_mac.Success)//匹配成功将文本中mac地址存入macaddress；如果一直没有匹配成功就要把本机的加进去
            {
                string macadress = match_mac.Groups[1].Value.Trim();

                for (int i = 0; i < myFeatureValue.MacAddress.Count; i++)
                {
                    string temMacadress = myFeatureValue.MacAddress[i].ToString().Trim();
                    if (macadress.Equals(temMacadress))
                    {
                        return myFeatureValue;
                    }
                    else { }
                }
                myFeatureValue.MacAddress.Add(macadress);
            }
            else { }
            return myFeatureValue;

        }
        //读取mac地址
        private string ReadLocalMac()
        {

            string str = null;
            foreach (NetworkInterface nic in NetworkInterface.GetAllNetworkInterfaces())
            {
                if (nic.OperationalStatus == OperationalStatus.Up)
                {
                    str = nic.GetPhysicalAddress().ToString().Trim();
                    break;
                }
            }
            return str;

        }
    }
    //
    class Writer
    {
        //写文本
        public void WriteLine(string PathStr, FeatureValue myFeatureValue)
        {
            string type = myFeatureValue.Type;
            int count = myFeatureValue.Count;
            string datetime = myFeatureValue.DateTime;
            ArrayList macaddress = myFeatureValue.MacAddress;
            FileStream wfs = new FileStream(PathStr, FileMode.Create);
            StreamWriter sw = new StreamWriter(wfs, System.Text.Encoding.Default);
            ArrayList Lines = new ArrayList();
            string temstr = null;

            temstr = GenerateCheckCode() + "09HXYA-" + type + "@" + count + "-@chinapower ." + GenerateCheckCode();//用户类型+最大使用次数
            Lines.Add(temstr);
            temstr = GenerateCheckCode() + "09HXYA-" + datetime + "-ABC." + GenerateCheckCode();//截至时间
            Lines.Add(temstr);

            foreach (string str in macaddress)
            {
                temstr = GenerateCheckCode() + "09HXYA-mac:" + str + "-513. " + GenerateCheckCode();//mac地址
                Lines.Add(temstr);
            }


            {
                int Seed = 100;
                while (Lines.Count < 8)
                {
                    Seed = (Seed / 2 + 1) * 2;
                    temstr = GenerateCheckCode();
                    temstr += GenerateCheckCode();
                    Lines.Add(temstr);
                }
            }


            Lines = disruptOrder(Lines);

            for (int i = 0; i < Lines.Count; i++)
            {
                sw.WriteLine(Lines[i]);
            }
            sw.Close();
            wfs.Close();

        }
        private ArrayList disruptOrder(ArrayList lines)
        {
            int linesCount = lines.Count;
            for (int i = 0; i < 5; i++)
            {

                int r1 = Generatenumber(0, lines.Count - 1);
                int r2 = Generatenumber(0, lines.Count / 2);
                string temstr = (string)lines[r1];
                lines[r1] = lines[r2];
                lines[r2] = temstr;
            }
            return lines;

        }
        //随机生成字符串
        private string GenerateCheckCode()
        {
            int codeCount = Generatenumber(45, 50);
            string str = string.Empty;

            for (int i = 0; i < codeCount; i++)
            {
                char ch;

                int num = Generatenumber(10, 50);
                if ((num % 2) == 0)
                {
                    ch = (char)(0x30 + ((ushort)(num % 10)));
                }
                else
                {
                    ch = (char)(0x41 + ((ushort)(num % 0x1a)));
                }
                str = str + ch.ToString(); ;
            }

            for (int j = 0; j < codeCount / 2; j++)
            {
                int s = Generatenumber(1, codeCount - 2);
                str = str.Insert(s, "-");
            }
            return str;
        }
        //生成一个固定范围的随机数
        int seed = 200;
        private int Generatenumber(int start, int stop)
        {
            int end = 0;
            seed += 200;
            Random ran = new Random(seed);
            end = ran.Next(start, stop);
            return end;


        }
    }
    class Judges
    {
        public string Type;
        public bool IsOpenMainfrm()
        {
            try
            {
                string FilePath = Path.Combine(Application.StartupPath, @"license.lic");
                FeatureValue myFeatureValue = new FeatureValue();
                myFeatureValue.MacAddress = new ArrayList();

                Reader myReader = new Reader();
                myFeatureValue = myReader.ReadandMatch(FilePath, myFeatureValue);

                bool isStart = IsGrantRight(myFeatureValue);
                if (isStart == true)
                {
                    Writer myWriter = new Writer();
                    myWriter.WriteLine(FilePath, myFeatureValue);
                }
                return isStart;
            }
            catch (Exception ex)
            {
                return false;
            }

        }
        private bool IsGrantRight(FeatureValue featureValue)
        {
            bool IsEnableUse = true;
            ArrayList macaddress = featureValue.MacAddress;
            int count = featureValue.Count;
            string DateTime = featureValue.DateTime;
            if (macaddress.Count > count)
            {
                IsEnableUse = false;
            }
            //66时间加密
            UInt32 U32Tmp = Convert.ToUInt32(DateTime.Substring(0, 4), 10);
            U32Tmp -= 6666;
            string strDecodeTime = U32Tmp.ToString("d4") + "-";
            U32Tmp = Convert.ToUInt32(DateTime.Substring(5, 2), 10);
            U32Tmp -= 66;
            strDecodeTime += U32Tmp.ToString("d2") + "-";
            U32Tmp = Convert.ToUInt32(DateTime.Substring(8, 2), 10);
            U32Tmp -= 66;
            strDecodeTime += U32Tmp.ToString("d2");
            DateTime dt1 = Convert.ToDateTime(strDecodeTime);
            DateTime dt2 = System.DateTime.Now;
            if (dt2 > dt1)
            {
                IsEnableUse = false;
            }
            return IsEnableUse;
        }
    }
}
