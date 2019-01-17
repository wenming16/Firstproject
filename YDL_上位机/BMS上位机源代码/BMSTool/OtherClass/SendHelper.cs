using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BMSTool.OtherClass
{
    class SendHelper
    {
        public static string ProvideFF(int WordsNum)
        {
            string str = string.Empty;
            for (int i = 0; i < WordsNum; i++)
            {
                str += "FF";
            }
            return str;
        }
    }
}
