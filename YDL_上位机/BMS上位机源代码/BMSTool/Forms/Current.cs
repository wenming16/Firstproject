using BMSTool.Analysiss;
using BMSTool.DataPacks;
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
    public partial class Current : BasicForm
    {
        public Current(MainForm mainForm)
        {
            InitializeComponent();
            ThresholdSeter seter = new ThresholdSeter(mainForm, this);
            seter.BandTsdSet("1912F4C0", 2, 0.1, 750);
        }
        protected override void DataToForm()
        {
            TextBox[] Txbs1 = { txb_DisCrgTsd1, txb_DisCrgTsd2, txb_DisCrgTsd3 };
            string[] Datas1 = Analysiser.dataPack.curTsd.SysCrgCur_Hs;
            DatatoTxbs(Txbs1, Datas1);
            // TextBox[]
            TextBox[] Txbs2 = { txb_CrgTsd1, txb_CrgTsd2, txb_CrgTsd3 };
            string[] Datas2 = Analysiser.dataPack.curTsd.SysDisCrgCur_Hs;
            DatatoTxbs(Txbs2, Datas2);

            //电池系统电流信息
            DataPack.SysCur sysCur = Analysiser.dataPack.sysCur;
            txb_Test.Text = sysCur.Test;
            txb_CrgLim.Text = sysCur.ChargeLim;
            txb_DisCrgLim.Text = sysCur.DisChargeLim;
            txb_MaxFeedLim.Text = sysCur.MaxFeedLim;
        }
        protected override void clearScreen()
        {
            DataPack.SysCur sysCur = Analysiser.dataPack.sysCur;
            DataPack.CurTsd curTsd = Analysiser.dataPack.curTsd;
            clearDatas(curTsd.SysCrgCur_Hs);
            clearDatas(curTsd.SysDisCrgCur_Hs);
            sysCur.Test = string.Empty;
            sysCur.ChargeLim = string.Empty;
            sysCur.DisChargeLim = string.Empty;
            sysCur.MaxFeedLim = string.Empty;
             Analysiser.dataPack.sysCur= sysCur;
            Analysiser.dataPack.curTsd= curTsd;
        }

        }
}
