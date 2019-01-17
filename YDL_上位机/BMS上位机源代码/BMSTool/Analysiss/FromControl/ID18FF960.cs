using BMSTool.OtherClass;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using static BMSTool.Analysiss.Analysiser;

namespace BMSTool.Analysiss.FromControl
{
    class ID18FF960
    {
        public void Analysis(int modNum, string FrameData)
        {
            //string str0 = FrameData.Substring(0, 2);
            //batModule.num0 = Convert.ToInt32(str0, 16);
            int [,] arr= Analysiser.dataPack.batDistributeNums;
            int length = arr.GetLength(1);
            for (int i = 0; i < length; i++)
            {
                string str = FrameData.Substring(i * 2, 2);
                if (str == "FF")
                {
                    str = "0";
                }
                arr[modNum,i] = Convert.ToInt32(str, 16);
            }
            //引用变量可以不互传

        }
    }
}
