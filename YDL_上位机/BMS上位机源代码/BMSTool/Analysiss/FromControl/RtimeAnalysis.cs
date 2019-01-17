using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BMSTool.Analysiss.MainControl.RealTime
{
    class RtimeAnalysis
    {
        protected string judgeFFtoBlack(string data, double unit, int offset)
        {
            string str = "";
            if (data.Contains("FF"))
            {
                str = "";
            }
            else
            {
                str = OtherClass.Compute.str16to10With(data, unit, offset);
            }
            return str;
        }
        protected string FFtoBlack(string data)
        {
            if (data.Contains("FF"))
            {
                return "";
            }
            else
            {
                return data;
            }
        }
        protected string FFtoZero(string data)
        {
            if (data.Contains("FF"))
            {
                return "0";
            }
            else
            {
                return data;
            }
        }
        
    }
}
