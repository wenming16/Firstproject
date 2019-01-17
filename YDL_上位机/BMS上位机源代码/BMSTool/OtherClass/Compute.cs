using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BMSTool.OtherClass
{
    class Compute
    {
        public static string StrInvert(string src)
        {
            string dst = string.Empty;
            int index = 0;
            if (src.Length % 2 != 0)
            { return  src; }
            else { }
            for (index = (src.Length / 2) - 1; index >= 0; index--)
            {
                dst += src.Substring(index * 2, 2);
            }
            return dst;

        }
        public static string str16to10(string str)

        {
            str = StrInvert(str);
            int temInt = Convert.ToInt32(str, 16);
            string temStr = temInt.ToString();
            return temStr;
        }
        public static string str10to16(string str,int Words,double unit,int offset)
        {
            double temDou = Convert.ToDouble(str);
            temDou = temDou + offset;
            temDou = temDou / unit;
            int temInt = Convert.ToInt32(temDou);
            string temStr = string.Empty;
            switch (Words)
            {
                case 1:
                    temStr = temInt.ToString("X2");
                    break;
                case 2:
                    temStr = temInt.ToString("x4");
                    break;
                case 3:
                    temStr = temInt.ToString("x6");
                    break;
                case 4:
                    temStr = temDou.ToString("x8");
                    break;
            }
            string ReStr = StrInvert(temStr);
            return ReStr;
        }
        //输入低字节在前，高子节在后的16进制字符串，输出正常二进制数
        public static string str16to2(string HexadecimalStr)
        {
            HexadecimalStr = StrInvert( HexadecimalStr);
            int NumberofWords = HexadecimalStr.Length;
            int temInt = Convert.ToInt32(HexadecimalStr, 16);
            string BinaryStr = Convert.ToString(temInt, 2);
            int count = NumberofWords * 4 - BinaryStr.Length;
            if (count > 0)
            {
                string temstr = string.Empty;
                for (int i = 0; i < count; i++)
                {
                    temstr += "0";
                }
                BinaryStr = temstr + BinaryStr;
            }
            return BinaryStr;
        }
        public static string Str2to16(string Binary)

        {
            int temInt = Convert.ToInt32(Binary, 2);
            string Hexadecimal = temInt.ToString("x4");

            return Hexadecimal;
        }
        public static string str16to10With (string str, double unit,int OffSet)//将16进制转化为10进制，并使用分辩率及偏移量
        {
            str = StrInvert(str);
            double temDou = Convert.ToInt32(str, 16)*unit;
            temDou = temDou-OffSet;
            temDou = Math.Round(temDou,2);
            string temStr = temDou.ToString();
            return temStr;
        }
        public static string MinStr(string[] StrArr)
        {
            string temStr = StrArr[0];
            for (int i = 0; i < StrArr.Length; i++)
            {
                if (StrArr[i] == null)
                {

                }
                else
                {
                    bool flag = JudgeSize(temStr, StrArr[i]);
                    if (flag)
                    {
                        temStr = StrArr[i];
                    }
                    else { }
                }

            }
            return temStr;
        }
        public static string MaxStr(string[] StrArr)
        {
            string temStr = StrArr[0];
            for (int i = 0; i < StrArr.Length; i++)
            {
               // if(strArr)
                bool flag = JudgeSize(temStr, StrArr[i]);
                if (flag)
                {

                }
                else { temStr = StrArr[i]; }
            }
            return temStr;
        }
        public static bool JudgeSize(string str1, string str2)
        {

            double temDou1 = Convert.ToDouble(str1);
            double temDou2 = Convert.ToDouble(str2);
            if (temDou1 > temDou2)
            {
                return true;
            }
            else { return false; }
        }
    }
}
