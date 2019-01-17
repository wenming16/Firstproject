using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using BMSTool;
using BMSTool.Comm;
using System.Windows.Forms.DataVisualization.Charting;
using System.Threading;
using BMSTool.Forms;
using BMSTool.Analysiss;
using BMSTool.DataPacks;
using BMSTool.OtherClass;
using System.Reflection;
using BMSTool.Analysiss.Other;

namespace BMSTool.Forms
{
    public partial class FromControlVol :BasicForm
    {
        int BoxNum;
        public FromControlVol(int BoxNum,MainForm mainForm)
        {
            InitializeComponent();
            this.BoxNum = BoxNum;
            InitialTable(12, "柱状图");
            ThresholdSeter seter = new ThresholdSeter(mainForm,this);
            seter.BandTsdSet("1910F4C0", 2, 0.0001, 0);
        } 
        //初始化表格
        private void InitialTable(int Length, string DisplayType)
        {

            Chart[] charts = { Chart0, chart1, chart2, chart3 };
            for (int i = 0; i < charts.Length; i++)
            {
                ChartTool.Initial(charts[i],Length, "柱状图");
            }   
        }
        protected override void DataToForm()
        {
            DataPack dataPack = Analysiser.dataPack;
            txb_CellVol_L1.Text = dataPack.volTsd.CellVol_Ls[0];
            txb_CellVol_L2.Text = dataPack.volTsd.CellVol_Ls[1];
            txb_CellVol_L3.Text = dataPack.volTsd.CellVol_Ls[2];

            txb_CellVol_H1.Text = dataPack.volTsd.CellVol_Hs[0];
            txb_CellVol_H2.Text = dataPack.volTsd.CellVol_Hs[1];
            txb_CellVol_H3.Text = dataPack.volTsd.CellVol_Hs[2];

            txb_CellGap1.Text = dataPack.volTsd.CellVolGaps[0];
            txb_CellGap2.Text = dataPack.volTsd.CellVolGaps[1];
            txb_CellGap3.Text = dataPack.volTsd.CellVolGaps[2];
            DataToChart();

            int max = Convert.ToInt32(dataPack.fromVols[BoxNum].MaxNum)+1;
            txb_MaxVolNum.Text =max.ToString() ;
            txb_MaxVol.Text = dataPack.fromVols[BoxNum].MaxValue;
            int min = Convert.ToInt32(dataPack.fromVols[BoxNum].MinNum) + 1;
            txb_MinVolNum.Text =min.ToString() ;
            txb_MinVol.Text = dataPack.fromVols[BoxNum].MinValue;
        }
        private void DataToChart()
        {
            Chart[] charts = { Chart0, chart1, chart2, chart3 };//引用变量
            for (int i = 0; i < 4; i++)
            {
               
                string[,] temData = Analysiser.dataPack.fromVols[BoxNum].Values;
                int Column = temData.Length /4;
                string[] Data = new string[Column];
                for (int j = 0; j < Column; j++)
                {
                    Data[j] = temData[i, j];
                }
                Chart chart = charts[i];
                ChartTool.DisplayDataToChart(chart, Data, "柱状图");

            }
        }
        protected override void clearScreen()
        {
            DataPack.VolTsd  volTsd = Analysiser.dataPack.volTsd;
            DataPack dataPack = Analysiser.dataPack;
            DataPack.FromVol[] fromVols = Analysiser.dataPack.fromVols;
          
            clearDatas(volTsd.CellVol_Ls);
            clearDatas(volTsd.CellVol_Hs);
            clearDatas(volTsd.CellVolGaps);
            dataPack.fromVols[BoxNum].MaxNum = string.Empty;
            dataPack.fromVols[BoxNum].MaxValue = string.Empty;
            dataPack.fromVols[BoxNum].MinNum = string.Empty;
            dataPack.fromVols[BoxNum].MinValue = string.Empty;
            for (int i = 0; i < 4; i++)
            {

                string[,] temData = Analysiser.dataPack.fromVols[BoxNum].Values;
                int Column = temData.Length / 4;
                string[] Data = new string[Column];
                for (int j = 0; j < Column; j++)
                {
                   temData[i, j]="0";
                }
               

            }
        }

        private void label38_Click(object sender, EventArgs e)
        {

        }

        private void txb_CellVol_H1_TextChanged(object sender, EventArgs e)
        {

        }
    }
}
