using BMSTool.Analysiss;
using BMSTool.OtherClass;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace BMSTool.Forms
{
    public partial class InitialSet : BasicForm
    {
        private MainForm mainForm;
        
        public InitialSet(MainForm mainForm)
        {
            InitializeComponent();
            this.mainForm = mainForm;
        }

        private void btn_SOC_Click(object sender, EventArgs e)
        {
                string Data = GetSetData(txb_SOC,2, 1, 0);
                SendSetData(Data, "00", 0, 5);
            
        }
        private string  GetSetData(TextBox textBox ,int WordsNum,double Unit, int OffSet)// //获取数据并处理成要发送的
        { 
             string Data = string.Empty;
            if (textBox.Text != string.Empty)
            {
               string str= textBox.Text;
                Data = Compute.str10to16(str, WordsNum, 1, 0);
            }
            else
            {
                MessageBox.Show("输入参数为空");
            }
            return Data;
          
        }
        private void SendSetData(string Data, string Num, int Front, int Behind)
        {
            //嵌入要发送的8个字节
            if (Data != string.Empty)
            {
                string SendData = Num + SendHelper.ProvideFF(Front) + Data + SendHelper.ProvideFF(Behind);
                mainForm.CanSend(SendData, "扩展帧", "数据帧", "1915F4C0");
                Seter seter = new Seter(mainForm);
                bool flag= seter.CheckIsRec("1915F4C0");
                if (flag)
                {
                    MessageBox.Show("标定成功！");
                }
                else
                {
                    MessageBox.Show("标定失败！");

                }
            }
            else { }
        }

        private void btn_Vol_Click(object sender, EventArgs e)
        {
            string Data = GetSetData(txb_Vol,2, 0.0001, 0);
            SendSetData(Data, "00", 2, 3);
        }

        private void btn_CurTest_Click(object sender, EventArgs e)
        {
            string Data = GetSetData(txb_Cur,2, 1, 0);
            SendSetData(Data, "00", 4, 1);
        }

        private void btn_Resis_Click(object sender, EventArgs e)
        {
            string Data = GetSetData(txb_Resis,2, 0.01, 0);
            SendSetData(Data, "01", 0, 5);
        }

        private void btn_SOH_Click(object sender, EventArgs e)
        {
            string Data = GetSetData(txb_SOH, 4, 0.01, 0);
            SendSetData(Data, "01", 2, 1);
        }

        private void btn_RunTime_Click(object sender, EventArgs e)
        {
            string Data = GetSetData(txb_RunTime, 2, 1, 0);
            SendSetData(Data, "02", 0, 5);
        }
    }
}
