using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using BMSTool;
using BMSTool.Comm;
using BMSTool.OtherClass;
using BMSTool.DataPacks;
using BMSTool.Analysiss;

namespace BMSTool.Forms
{
    public partial class SohSet : BasicForm
    {


        private MainForm mainForm;


        public SohSet(MainForm mainForm)
        {
            InitializeComponent();
            this.mainForm = mainForm;
        }
      

        private void btn_SysTime_Click(object sender, EventArgs e)
        {
            DateTime SetTime;
            string frameType = "扩展帧";
            string frameCmd = "数据帧";
            string frameID = "1937F4C0";
            SetTime = DateTime.Now;
            string frameData = string.Format("{0:yyMMddHHmmss}", SetTime);


            try
            {
                bool Success = mainForm.myCanComm.Send(frameData, frameType, frameCmd, frameID);
                if (Success == false)
                {
                    MessageBox.Show("发送失败，请检查连接！");
                    return;
                }
                else { }


            }
            catch (Exception error)
            {

            }
        }

        protected override void DataToForm()
        {
            DataPack.TimeInfo timeInfo = Analysiser.dataPack.timeInfo;
         //   txb_SysState.Text = timeInfo.SysState;
            txb_AllRunTime.Text = timeInfo.AllRun;
            txb_OnceTime.Text = timeInfo.OnceRun;
            txb_PowerOff.Text = timeInfo.PowerOff;
            txb_SysTime.Text = timeInfo.SysReal;
        }

        protected override void clearScreen()
        {
            //这个地方的清屏应该有问题，因为结构体是值变量
            DataPack.TimeInfo timeInfo = Analysiser.dataPack.timeInfo;
            timeInfo.SysState = string.Empty;
            timeInfo.AllRun = string.Empty;
            timeInfo.OnceRun = string.Empty;
            timeInfo.PowerOff = string.Empty;
            timeInfo.SysReal = string.Empty;
             Analysiser.dataPack.timeInfo=timeInfo;
        }
    }
}

