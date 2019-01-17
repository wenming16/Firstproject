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
    public partial class Insula : BasicForm
    {
        public Insula(MainForm mainForm)
        {
            InitializeComponent();
            ThresholdSeter seter = new ThresholdSeter(mainForm, this);
            seter.BandTsdSet("1913F4C0", 2, 0.1, 0);
        }
       
        protected override void DataToForm()
        {
            TextBox[] Txbs = { txb_Tsd1, txb_Tsd2, txb_Tsd3 };
            string[] Datas = Analysiser.dataPack.InsulaTsds;
            DatatoTxbs(Txbs, Datas);
            DataPack.SysInsual sysInsual = Analysiser.dataPack.sysInsual;
            txb_PosVol.Text = sysInsual.PosVol;
            txb_NegVol.Text = sysInsual.NegVol;
            txb_PosResis.Text = sysInsual.PosResis;
            txb_NegResis.Text = sysInsual.NegResis;
        }
        protected override void clearScreen()
        {
            DataPack dataPack = Analysiser.dataPack;
            clearDatas(dataPack.InsulaTsds);
            dataPack.sysInsual.PosVol = string.Empty;
            dataPack.sysInsual.NegVol = string.Empty;
            dataPack.sysInsual.PosResis = string.Empty;
            dataPack.sysInsual.NegResis = string.Empty;
        }


    }
}
