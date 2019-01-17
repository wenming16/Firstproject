using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;

namespace BMSTool.Analysiss
{
    class Seter//标定回复帧ID18FFC0F4
    {
        private MainForm mainForm;
        public Seter(MainForm mainForm)
        {
            this.mainForm = mainForm;
        }
        public bool CheckIsRec(string SendID)
        {
            mainForm.tmr_AnalysisData.Enabled = false;
           
            bool Success = false;

          //  Thread.Sleep(0);
            CanDataFrame[] Frames = mainForm.myCanComm.RevData();
           
            if (Frames != null)
            {
                for (int j = 0; j < Frames.Length; j++)
                {
                    if (Frames[j].frameID == "1610C0F4")
                    {
                      
                    }
                    if (Frames[j].frameID== "18FFC0F4")
                    {
                        String SendFlag = SendID.Substring(2, 2);
                        String RecFlag = Frames[j].frameData.Substring(0, 2);
                        if (SendFlag == RecFlag)
                        { Success = true; }
                    }
                    else
                    { }
                }
            }
            else
            {
            }
            mainForm.tmr_AnalysisData.Enabled = true ;
            return Success;

        }
        public void DisplayMessage(bool flag)//将他暴露出来而不是放在内部为的是实现多样化输出
        {
            if (flag)
            {
                MessageBox.Show("标定成功！");
            }
            else
            {
                MessageBox.Show("标定失败");
            }
        }

    }
}

