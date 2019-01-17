using BMSTool.Analysiss;
using BMSTool.DataPacks;
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
    struct CrgFormat
    {
        public string Content;
        public Label lab;
        public PictureBox ptb;
        public bool Feature;
    }
    public partial class CrgCommError : BasicForm
    {
        public CrgCommError()
        {
            InitializeComponent();
        }
       
        
       protected override void DataToForm()
        {
            //BYTE2
            DataPack dataPack = Analysiser.dataPack;
            BMSByte1(dataPack);
            BMSByte26(dataPack);
            CrgerByte1(dataPack);
            CrgerByte26(dataPack);

        }
        private void CrgerByte1(DataPack dataPack)
        {
            CrgerCtl ctls = new CrgerCtl();
            ctls.Add("正常中止", lab_NormalBreak, ptb_NormalBreak);
            ctls.Add("人为中止", lab_ManBreak, ptb_ManBreak);
            ctls.Add("故障中止", lab_ErrorBreak, ptb_ErrorBreak);
            ctls.Add("BMS主动中止", lab_BMSBreak, ptb_BMSBreak);
            bool[,] Errors = dataPack.CrgerError;
            for (int i = 0; i < 4; i++)
            {
                CrgFormat ctl = new CrgFormat();
                ctl = ctls.Search(i);
                bool flag = Errors[1, i];
                ctlset(ctl, flag);
            }
        }

        private void CrgerByte26(DataPack dataPack)
        {
            CrgerCtl ctls = new CrgerCtl();
            //byte2
            ctls.Add("充电机过温", lab_CrgerTempH, ptb_CrgerTempH);
            ctls.Add("充电机连接器", lab_CrgerConnector, ptb_CrgerConnector);
            ctls.Add("充电机内部过温", lab_CrgerInTempH, ptb_CrgerInTempH);
            ctls.Add("能量传输", lab_PowertTran, ptb_PowerTran);
            //byte3
            ctls.Add("充电机急停", lab_CrgerJerk, ptb_CrgerJerk);
            ctls.Add("其他", lab_CrgerOther, ptb_CrgerOther);
            ctls.Add("电流匹配", lab_CurMatch, ptb_CurMatch);
            ctls.Add("电压", lab_CrgerVol, ptb_CrgerVol);

            bool[,] Errors = dataPack.CrgerError;
            for (int i = 0; i < 2; i++)//像这种for循环外面里面是数组偏移

            {
                for (int j = 0; j < 4; j++)
                {
                    CrgFormat ctl = new CrgFormat();
                    int Index = i * 4 + j;
                    ctl = ctls.Search(Index);
                    bool flag = Errors[i + 1, j];
                    ctlset(ctl, flag);
                }
            }

            ctls.Clear();

            //byte4
            ctls.Add("BRM超时", lab_BRM, ptb_BRM);
            ctls.Add("BCP超时", lab_BCP, ptb_BCP);
            ctls.Add("BR0超时", lab_BR0, ptb_BR0);
            ctls.Add("BCS超时", lab_BCS, ptb_BCS);

          
           //byte5

            //byte6
            ctls.Add("BCL超时", lab_BCL, ptb_BCL);
            ctls.Add("BST超时", lab_BST, ptb_BST);
            ctls.Add("BSD超时", lab_BSD, ptb_BSD);
            for (int i = 0; i < 2; i++)//像这种for循环外面里面是数组偏移

            {
                for (int j = 0; j < 4; j++)
                {
                    CrgFormat ctl = new CrgFormat();
                    int Index = i * 4 + j;
                    if (Index != 7)
                    {
                        ctl = ctls.Search(Index);
                        bool flag = Errors[i + 5, j];
                        ctlset(ctl, flag);
                    }
                    else { }
                }
            }
        }
        private void ctlset(CrgFormat ctl,bool flag)
        {

            if (flag)
            {
                ctl.lab.Text = ctl.Content + "故障";
                ctl.ptb.ImageLocation = Application.StartupPath + @"\Photo\报警_红色.png";
            }
            else

            {
                ctl.lab.Text = ctl.Content + "正常";
                ctl.ptb.ImageLocation = Application.StartupPath + @"\Photo\报警_绿色.png";
            }
        }
        private void BMSByte26(DataPack dataPack)
        {
            CrgerCtl ctls = new CrgerCtl();
            //byte2
            ctls.Add("自检通过", lab_SelfCheck, ptb_SelfCheck);
            ctls.Add("电流匹配", lab_CurMatch, ptb_CurMatch);
            ctls.Add("电压匹配", lab_VolMatch, ptb_VolMatch);
            ctls.Add("绝缘", lab_Insulat, ptb_Insulat);
            //BYTE3
            ctls.Add("输出连接器过温", lab_OutConnectorTempH, Ptb_OutConnectorTempH);
            ctls.Add("充电连接器", lab_CrgConnector, ptb_CrgConnector);
            ctls.Add("电池组过压", lab_GroupVol_H, ptb_GroupVolH);
            ctls.Add("高压继电器", lab_HVSwitch, ptb_HVSwitch);

            //byte4
            ctls.Add("充电枪接触会CC2口", lab_Gun, ptb_Gun);
            ctls.Add("其他", lab_Other, ptb_Other);
            ctls.Add("充电电流过大", lab_CrgCurH, ptb_CrgCurH);
            ctls.Add("电压", lab_BMSVol, ptb_BMSVol);
            //    ctls.Add("电压", lab_CrgerVol, ptb_CrgerVol);
            //byte5
            ctls.Add("CRM00超时", lab_CRM00, ptb_CRM00);
            ctls.Add("CRMAA超时", lab_CRMAA, ptb_CRMAA);
            ctls.Add("CTSCML超时", lab_CTSCML, ptb_CTSCML);
            ctls.Add("CR0超时", lab_CR0, ptb_CR0);
            //byte6
            ctls.Add("CCS超时", lab_CCS, ptb_CCS);
            ctls.Add("CST超时", lab_CST, ptb_CST);
            ctls.Add("CSD超时", lab_CSD, ptb_CSD);




            bool[,] CrgerErrors = dataPack.BMSError;
            for (int i = 0; i < 5; i++)//像这种for循环外面里面是数组偏移
            {
                for (int j = 0; j < 4; j++)
                {
                    CrgFormat ctl = new CrgFormat();
                    int Index = i* 4 + j;
                    if (Index != 19)
                    {
                        ctl = ctls.Search(Index);
                        bool flag = CrgerErrors[i + 2, j];
                        ctlset(ctl, flag);
                    }
                    else { }
                }
            }

        }
        
        private void BMSByte1(DataPack dataPack)
        {
            CrgerCtl ctls = new CrgerCtl();
            ctls.Add("达到给定SOC", lab_SOC, ptb_SOC);
            ctls.Add("达到给定总压", lab_AllVol, ptb_AllVol);
            ctls.Add("达到给定单体电压", lab_CellVol, ptb_CellVol);
            ctls.Add("充电机主动中止", lab_CrgerBreak, ptb_CrgerBreak);
            bool[,] BMSErrors = dataPack.BMSError;
            for (int i = 0; i < 4; i++)
            {
                CrgFormat ctl = new CrgFormat();
                ctl = ctls.Search(i);
                bool flag = BMSErrors[1, i];
                if (flag)
                {
                    ctl.lab.Text = ctl.Content;
                    ctl.ptb.ImageLocation = Application.StartupPath + @"\Photo\报警_红色.png";
                }
                else
                {
                    ctl.lab.Text = "未" + ctl.Content;
                    ctl.ptb.ImageLocation = Application.StartupPath + @"\Photo\报警_绿色.png";
                }

            }
        }
    }
    class CrgerCtl//是否故障类
    {
        private List<CrgFormat> ctls = new List<CrgFormat>();
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
        public void Add(string content, Label label, PictureBox picture)
        {
            CrgFormat ctl = new CrgFormat();
            ctl.Content = content;
            ctl.lab = label;
            ctl.ptb = picture;
           
            ctls.Add(ctl);
        }
        public CrgFormat Search(int i)//输入
        {
            CrgFormat ctl = new CrgFormat();
            ctl = ctls[i];
            return ctl;
        }
        public void Clear()
        {
            ctls = new List<CrgFormat>();
        }
    }
}
