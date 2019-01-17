using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;//命名空间也应该就近原则

namespace BMSTool.OtherClass
{
   public class CheckIsSet
    {
        public CheckIsSet(MainForm mform)
        {
            tmrIsError.Elapsed += new System.Timers.ElapsedEventHandler(tmrIsError_Tick); //到达时间的时候执行事件；   
            tmrIsError.AutoReset = false;   //设置是执行一次（false）还是一直执行(true)；   
            tmrIsError.Enabled = false;     //是否执行System.Timers.Timer.Elapsed事件；   
        }
        public string RecCheckWord = string.Empty;
        public System.Timers.Timer tmrIsError = new System.Timers.Timer(1000);
        private void tmrIsError_Tick(object source, System.Timers.ElapsedEventArgs e)
        {
            if (RecCheckWord != string.Empty)
            {
                MessageBox.Show("标定失败!");
            }
        }
    }
}
