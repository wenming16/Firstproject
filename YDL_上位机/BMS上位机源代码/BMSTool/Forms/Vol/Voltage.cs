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
using System.Drawing;
using BMSTool.DataPacks;
using BMSTool.Analysiss;
using BMSTool.OtherClass;

namespace BMSTool.Forms
{
    public partial class Vol : BasicForm
    {
        private MainForm myMainForm;
        public Vol(MainForm mainForm)
        {
            this.myMainForm = mainForm;
            InitializeComponent();
            ThresholdSeter seter = new ThresholdSeter(mainForm, this);
            seter.BandTsdSet("1910F4C0", 2, 0.1, 0);
            BandFromControl();
        }
        private void BandFromControl()
        {
            FromControlVol[] fromControls = new FromControlVol[10];
            ProgressBar[] Vols = { pgb_Vol0, pgb_Vol1, pgb_Vol2, pgb_Vol3, pgb_Vol4, pgb_Vol5, pgb_Vol6, pgb_Vol7, pgb_Vol8, pgb_Vol9};
            for (int i = 0; i < Vols.Length; i++)
            {
                int j = i;//事件注册的时使用的内存应当是不同的//事件只能先注册才能使用
                ProgressBar bar_Vol = Vols[i];
   
                bar_Vol.Click += (object sender, EventArgs e) =>
                {
                    fromControls[j] = new FromControlVol(j,myMainForm);
                    fromControls[j].Text = "从控板" +(j+1);
                    myMainForm.browserForm.FormtoTab(fromControls[j]);
                };
            }  
        }
       protected override void DataToForm()
        {
            DataPack dataPack = Analysiser.dataPack;
            txb_SysVol_L1.Text = dataPack.volTsd.SysVol_Ls[0];
            txb_SysVol_L2.Text = dataPack.volTsd.SysVol_Ls[1];
            txb_SysVol_L3.Text = dataPack.volTsd.SysVol_Ls[2];

            txb_SysVol_H1.Text = dataPack.volTsd.SysVol_Hs[0];
            txb_SysVol_H2.Text = dataPack.volTsd.SysVol_Hs[1];
            txb_SysVol_h3.Text = dataPack.volTsd.SysVol_Hs[2];

            txb_SysVolGap1.Text = dataPack.volTsd.SysVolGaps[0];
            txb_SysVolGap2.Text = dataPack.volTsd.SysVolGaps[1];
            txb_SysVolGap3.Text = dataPack.volTsd.SysVolGaps[2];
            ProgressBar[] Vols = { pgb_Vol0, pgb_Vol1, pgb_Vol2, pgb_Vol3, pgb_Vol4, pgb_Vol5, pgb_Vol6, pgb_Vol7, pgb_Vol8, pgb_Vol9 };
            Label[] labs = { lab_vol0, lab_vol1, lab_vol2, lab_vol3, lab_vol4, lab_vol5, lab_vol6, lab_vol7, lab_vol8, lab_vol9 };
            for (int i = 0; i < Vols.Length; i++)
            {
                string str = dataPack.fromVols[i].TotalValue;
                if (str != "")
                {
                    Vols[i].Maximum = 100 * 10;
                    double value = Convert.ToDouble(str);
                    Vols[i].Value = (int)value * 10;
                    if (value == 0)
                    {
                        labs[i].Text = "00V";
                    }
                    else
                    {
                        labs[i].Text = Math.Round(value, 2).ToString()+"V";
                    }
                  
                  labs[i].TextAlign = ContentAlignment.MiddleLeft;
                }
                else { }
              
            }

        }
       protected override void clearScreen()
        {
            DataPack dataPack = Analysiser.dataPack;
            clearDatas(dataPack.volTsd.SysVol_Ls);
            clearDatas(dataPack.volTsd.SysVol_Hs);
            clearDatas(dataPack.volTsd.SysVolGaps);
            ProgressBar[] Vols = { pgb_Vol0, pgb_Vol1, pgb_Vol2, pgb_Vol3, pgb_Vol4, pgb_Vol5, pgb_Vol6, pgb_Vol7, pgb_Vol8, pgb_Vol9 };
            Label[] labs = { lab_vol0, lab_vol1, lab_vol2, lab_vol3, lab_vol4, lab_vol5, lab_vol6, lab_vol7, lab_vol8, lab_vol9 };
            for (int i = 0; i < Vols.Length; i++)
            {
                string str = dataPack.fromVols[i].TotalValue;
                if (str != "")
                {
                    Vols[i].Maximum = 100 * 10000;
                    Vols[i].Value = 0;
                    labs[i].Text = "00";
                    
                }
                else { }
            }
        }
    }
 }

