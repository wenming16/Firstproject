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
using System.Windows.Forms.DataVisualization.Charting;
using BMSTool.Analysiss;
using BMSTool.DataPacks;
using BMSTool.Analysiss;
using BMSTool.OtherClass;

namespace BMSTool.Forms
{
    public partial class SocSet : BasicForm
    {
        private MainForm mainForm;
       
        public SocSet(MainForm _mainForm)
        {
            InitializeComponent();
            mainForm = _mainForm;
            ThresholdSeter seter = new ThresholdSeter(mainForm, this);
            seter.BandTsdSet("1910F4C0", 2, 0.0001, 0);
        }
      
        protected override void  DataToForm()
        {
            DataPack dataPack = Analysiser.dataPack;
            txb_SOC_Read.Text = dataPack.soc.Read;
            txb_SOC_Volt.Text = dataPack.soc.Vol;
            txb_SOC_L1.Text = dataPack.socTsd[0].Low;
            txb_SOCL2.Text = dataPack.socTsd[1].Low;
            txb_SOCL3.Text = dataPack.socTsd[2].Low;

            txb_SOCTsdH1.Text = dataPack.socTsd[0].High;
            txb_SocTsdH2.Text = dataPack.socTsd[1].High;
            txb_SOCTsdH3.Text = dataPack.socTsd[2].High;
        }

        protected override void clearScreen()
        {
            DataPack dataPack = Analysiser.dataPack;
            dataPack.soc.Read = string.Empty;
            dataPack.soc.Vol = string.Empty;
            dataPack.socTsd[0].Low = string.Empty;
            dataPack.socTsd[1].Low = string.Empty;
            dataPack.socTsd[2].Low = string.Empty;
            dataPack.socTsd[0].High = string.Empty;
            dataPack.socTsd[1].High = string.Empty;
            dataPack.socTsd[2].High = string.Empty;
        }



        private void btn_SOC_Volt_Click(object sender, EventArgs e)
        {
            SOCSet("SOC_Volt");
        }
        private void SOCSet(string SOC)
        {

            string frameType = string.Empty;
            string frameCmd = string.Empty;
            string frameID = string.Empty;
            string frameData = string.Empty;

            frameType = "扩展帧";
            frameCmd = "数据帧";
            frameID = "1970F4C0";
            switch (SOC)
            {
                case "SOC_Read":
                    if (txb_SOC_Read.Text == "")
                    {
                        MessageBox.Show("请输入参数值！");
                        return;
                    }
                    else { }
                    string V1 = "FF";
                    string V2 = txb_SOC_Read.Text;
                    V2 = Compute.str10to16(V2, 1, 1, 0);
                    string V3 = "FFFFFF";
                    string V4 = "FFFFFF";
                  
                    frameData = V1 + V2 + V3 + V4;
                    break;
                case "SOC_Volt":
                    if (txb_SOC_Volt.Text == "")
                    {
                        MessageBox.Show("请输入参数值！");
                        return;
                    }
                    else { }

                    V1 = "FFFF";
                    V2 = txb_SOC_Volt.Text;
                    V2=Compute.str10to16(V2, 1, 1, 0);
                    V3 = "FFFFFF";
                    V4 = "FFFF";

                    frameData = V1 + V2 + V3 + V4;
                    break;
            }

                
            
           

        }

        private void btn_SOC_Read_Click(object sender, EventArgs e)
        {
            SOCSet("SOC_Read");
        }

        private void btn_SOC_TsdL_Click(object sender, EventArgs e)
        {
            string str = GetSOCTsd(sender) + SendHelper.ProvideFF(3) ;
            mainForm.CanSend(str, "扩展帧", "数据帧", "1812C0F4");
        }
        private void btn_SOC_Tsd_H_Click(object sender, EventArgs e)
        {
            string str = SendHelper.ProvideFF(3)+ GetSOCTsd(sender);
            mainForm.CanSend(str, "扩展帧", "数据帧", "1812C0F4");
        }
        private string  GetSOCTsd(object sender)
        {
            Button btn = (Button)sender;
            Control control = btn.Parent;
            Control.ControlCollection controls = control.Controls;
            string Tsd = string.Empty;
            for (int j = 0; j < controls.Count; j++)
            {
                Control ctl = controls[j];
                Type type = ctl.GetType();
                if (type == typeof(TextBox))
                {
                    string name = ctl.Name;
                    int start = name.Length - 1;
                    string str = name.Substring(start);
                    int index = Convert.ToInt32(str) - 1;
                    string content = ctl.Text;
                    string  temStr= Compute.str10to16(content, 1, 1, 0);
                    Tsd+= temStr;
                }
                else { }
            }

            return Tsd;
        }
       
    }
}
