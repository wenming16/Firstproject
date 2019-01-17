using BMSTool.OtherClass;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace BMSTool.Analysiss.Other
{
    class AnalysisHelper
    {
        public static string[] GetInfos(string Data, int Words_Num, double Unit, int Offset)//解析时可以帮助批量处理数据
        {

            string[] Infos = new string[Words_Num];
            int Length = Data.Length / (2 * Words_Num);
            for (int i = 0; i < Words_Num; i++)
            {
                string TemStr = Data.Substring(i * Length, Length);
                Infos[i] = Compute.str16to10With(TemStr,Unit,Offset);
            }
            return Infos;
        }

        

       


    }
}
