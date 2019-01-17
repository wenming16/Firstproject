using BMSTool.Analysiss;
using BMSTool.DataPacks;
using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace BMSTool.Forms
{
    struct CtlFormat
    {
        public string Content;
        public Label lab;
        public PictureBox ptb;
        public string Feature;
    }
    struct CtlFormat1
    {
        public string Content;
        public Label lab;
        public PictureBox ptb;
        public bool Feature;
    }
    public partial class SysError : BasicForm
    {
        private BrowserForm browser = new BrowserForm();
        public SysError(MainForm mainForm)
        {
            InitializeComponent();
            this.browser =mainForm.browserForm;
        }
        
       

     
        protected override void  DataToForm()
        {
            //如何建立好一对三的关系
            //使用结构数组再加上遍历搜索


            DataPack dataPack = Analysiser.dataPack;


            CtlData ctlData = new CtlData();//这个原理类似于散列表
            DataPack.VolError vol = Analysiser.dataPack.volError;
            ctlData.Add("单体电压过压", lab_CellVol_H, ptb_CellVol_H, vol.Cell_H);
            ctlData.Add("单体电压欠压", lab_CellVol_L, ptb_CellVol_L, vol.Cell_L);
            ctlData.Add("单体压差", lab_CellVolGap, ptb_CellVolGap, vol.Cell_Gap);
            ctlData.Add("总压过高", lab_AllVol_H, ptb_AllVol_H, vol.All_H);
            //BYTE4
            ctlData.Add("总压过低", lab_AllVol_L, ptb_AllVol_L, vol.All_L);
            ctlData.Add("电池绝缘", lab_Insulation, ptb_Insulation, dataPack.InsualError);

            DataPack.TempError temp = Analysiser.dataPack.tempError;
            ctlData.Add("放电温度过高", lab_DisCrgTemp_H, ptb_DisCrgTemp_H, temp.Discharge_H);
            ctlData.Add("放电温度过低", lab_DisCrgTempL, ptb_DisCrgTemp_L, temp.Discharge_L);
            //byte5
            ctlData.Add("放电温差过大", lab_DisCrgTempGap, ptb_DisCrgTempGap, temp.DischargeGap_H);
            ctlData.Add("充电温度过高", lab_CrgTempH, ptb_CrgTempH, temp.Charge_H);
            ctlData.Add("充电温度过低", lab_CrgTempL, ptb_CrgTempL, temp.Charge_L);
            ctlData.Add("充电温差大", lab_CrgTempGap, ptb_CrgTempGap, temp.ChargeGap_H);
            //byte6
            ctlData.Add("SOC高", lab_SOCH, ptb_SOCH, dataPack.SocError.High);
            ctlData.Add("SOC低", lab_SOCL, ptb_SOCL, dataPack.SocError.Low);
            ctlData.Add("充电电流过大", lab_CrgCur_H, ptb_CrgCurH, dataPack.curError.Charge_H);
            ctlData.Add("放电电流过大", lab_DisCrgCurH, ptb_DisCrgCurH, dataPack.curError.DisCharge_H);
            for (int i = 0; i < ctlData.length; i++)
            {
                CtlFormat ctl = ctlData.Search(i);
                string Content = ctl.Content;
                PictureBox ptb = ctl.ptb;
                Label lab = ctl.lab;
                string Feature = ctl.Feature;
                lab.Text = Content + Feature;
                if (Feature == "正常")
                {
                    ptb.ImageLocation = Application.StartupPath + @"\Photo\报警_绿色.png";
                }
                else
                {
                    ptb.ImageLocation = Application.StartupPath + @"\Photo\报警_红色.png";
                }
            }

            CtlData1 ctlData1 = new CtlData1();//类似于哈希表

            ctlData1.Add("充电枪温度", lab_CrgGunTemp, ptb_CrgGunTemp, temp.ChargeGun_H);
            ctlData1.Add("慢充充电枪温度", lab_SlowGunTemp, ptb_SlowGunTemp, temp.SlowCharge_H);
            ctlData1.Add("总压测量", lab_AllVolTest, ptb_AllVolTest, dataPack.volError.ALL_test);
            ctlData1.Add("CSSU芯片温度", lab_CSSUTemp, ptb_CSSUTemp, temp.CSSU);
            ctlData1.Add("BMU芯片温度", lab_BMUTemp, ptb_BMUTemp, temp.BMU);
            ctlData1.Add("HVU芯片温度", lab_HVUTemp, ptb_HVUTemp, temp.HVU);

            DataPack.SwitchError switchError = Analysiser.dataPack.switchError;
            ctlData1.Add("主正继电器", lab_PosSwitch, ptb_PosSwitch, switchError.Positive);
            ctlData1.Add("主负继电器", lab_NegSwitch, ptb_NegSwitch, switchError.Negative);
            ctlData1.Add("慢充继电器", lab_SlowSwitch, ptb_SlowSwitch, switchError.Slow);
            ctlData1.Add("快充继电器", lab_QuickSwitch, ptb_QuickSwitch, switchError.Quick);

            DataPack.CommError comm = Analysiser.dataPack.commError;
            ctlData1.Add("CSSU通信", lab_CSSUComm, ptb_CSSUComm,comm.CSSU);
            ctlData1.Add("VCU通信", lab_VCUComm, ptb_VCUComm, comm.VCU);
            ctlData1.Add("HVU通信", lab_HVUComm, ptb_HVUComm, comm.HVU);
            ctlData1.Add("充电机通信", lab_TBOXComm,ptb_TBOXComm, comm.Tbox);

            for (int i = 0; i < ctlData1.length; i++)
            {
                CtlFormat1 ctl = ctlData1.Search(i);
                string Content = ctl.Content;
                PictureBox ptb = ctl.ptb;
                Label lab = ctl.lab;
                bool Feature = ctl.Feature;
             
                if (Feature ==true)
                {
                    ptb.ImageLocation = Application.StartupPath + @"\Photo\报警_绿色.png";
                    lab.Text = Content + "正常";
                }
                else
                {
                    ptb.ImageLocation = Application.StartupPath + @"\Photo\报警_红色.png";
                    lab.Text = Content + "故障";
                
                }
            }

            










        }

        private void ptb_CrgComm_Click(object sender, EventArgs e)
        {
          //  CrgCommError commError = new CrgCommError();
            //browser.FormtoTab(commError);
        }

        private void SysError_Load(object sender, EventArgs e)
        {

        }
    }
    class CtlData//结构体故障等级数组类
    {
        
        private List<CtlFormat> ctls = new List<CtlFormat>();
        public int length//使用属性实现只读访问
        {
            set { }
            get
            {
                int count = ctls.Count;
                return count; }
        }
       // public 
        public void Add(string content,Label label,PictureBox picture,string Feature)
        {
            CtlFormat ctl = new CtlFormat();
            ctl.Content = content;
            ctl.lab = label;
            ctl.ptb = picture;
            if (Feature == null)
            {
                ctl.Feature = "正常";
              
            }
            else
            {
                if (Feature == string.Empty)
                {
                    ctl.Feature ="正常";
                }
                else
                {
                    ctl.Feature = Feature;
                }

            }
            ctls.Add(ctl);
        }
        public CtlFormat Search(int i)//输入
        {
            CtlFormat ctl = new CtlFormat();
            ctl = ctls[i];
            return ctl;
        }
    }
    class CtlData1//是否故障类
    {
        private List<CtlFormat1> ctls = new List<CtlFormat1>();
        public int length//使用属性实现只读访问
        {
            set { }
            get
            {
                int count = ctls.Count;
                return count;
            }
        }
        // public 
        public void Add(string content, Label label, PictureBox picture, bool Feature)
        {
            CtlFormat1 ctl = new CtlFormat1();
            ctl.Content = content;
            ctl.lab = label;
            ctl.ptb = picture;
            if (Feature == null)
            {
                ctl.Feature = true;

            }
            else
            {
                ctl.Feature = Feature;
            }
            ctls.Add(ctl);
        }
        public CtlFormat1 Search(int i)//输入
        {
            CtlFormat1 ctl = new CtlFormat1();
            ctl = ctls[i];
            return ctl;
        }
    }
}
