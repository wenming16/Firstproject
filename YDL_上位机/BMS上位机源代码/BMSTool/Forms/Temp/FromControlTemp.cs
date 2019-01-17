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
    public partial class FromControlTemp : BasicForm
    {
        public int BoxNum;
        public FromControlTemp(int BoxNum,MainForm mainForm)
        {
            this.BoxNum = BoxNum;
            InitializeComponent();
            ChartTool.Initial(chart1, 8, "柱状图");
            ThresholdSeter seter = new ThresholdSeter(mainForm, this);
            seter.BandTsdSet("1911F4C0", 1, 1, 40);
        }

   
        protected override void DataToForm()
        {
            DataPack dataPack = Analysiser.dataPack;
            txb_MaxNum.Text = dataPack.fromTemps[BoxNum].MaxNum;
            txb_MaxValue.Text = dataPack.fromTemps[BoxNum].MaxValue;

            txb_MinNum.Text = dataPack.fromTemps[BoxNum].MinNum;
            txb_MinValue.Text = dataPack.fromTemps[BoxNum].MinValue;

            string[] Data = Analysiser.dataPack.fromTemps[BoxNum].Values;
            ChartTool.DisplayDataToChart(chart1, Data, "柱状图");
        }
        protected override void clearScreen()
        {
            DataPack dataPack = Analysiser.dataPack;
            dataPack.fromTemps[BoxNum].MaxNum = string.Empty;
            dataPack.fromTemps[BoxNum].MaxValue = string.Empty;
            dataPack.fromTemps[BoxNum].MinNum = string.Empty;
            dataPack.fromTemps[BoxNum].MinValue = string.Empty;
            clearDatas(dataPack.fromTemps[BoxNum].Values);   
            
        }

    }
}
