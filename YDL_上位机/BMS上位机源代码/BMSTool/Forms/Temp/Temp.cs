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
using BMSTool.DataPacks;
using BMSTool.Analysiss;
using BMSTool.OtherClass;

namespace BMSTool.Forms
{
    public partial class Temp : BasicForm
    {
        private MainForm mainForm;
        public Temp(MainForm mainForm)
        {
            this.mainForm = mainForm;
            InitializeComponent();
            ThresholdSeter seter = new ThresholdSeter(mainForm, this);
            seter.BandTsdSet("1911F4C0", 1, 1, 40);
            OpenFromControl();
        }
        private void OpenFromControl()
        {
            FromControlTemp[] fromControls = new FromControlTemp[10];
            PictureBox[] Temps = { ptb_Temp0, ptb_Temp1, ptb_Temp2, ptb_Temp3, ptb_Temp4,
                ptb_Temp5, ptb_Temp6, ptb_Temp7, ptb_Temp8, ptb_Temp9, };
            for (int i = 0; i <Temps.Length; i++)
            {
                int j = i;
                PictureBox ptb_Temp = Temps[i];

                ptb_Temp.Click += (object sender, EventArgs e) =>
                {
                    fromControls[j] = new FromControlTemp(j,mainForm);
                    fromControls[j].Text = "从控板" + (j + 1)+"温度";
                   mainForm.browserForm.FormtoTab(fromControls[j]);
                };
            }
        }
        

        protected override void DataToForm()
        {
            DataPack dataPack = Analysiser.dataPack;
         
            TextBox[] Txbs1 = { txb_Crg_TemH1, txb_Crg_TemH2, txb_Crg_TemH3 };
            string[] datas1 = dataPack.tempTsd.ChargeBat_Hs;
            DatatoTxbs(Txbs1, datas1);

            TextBox[] Txbs2 = { txb_Crg_TemL14, txb_Crg_TemL25, txb_Crg_TemL36 };
            string[] datas2 = dataPack.tempTsd.ChargeBat_Ls;
            DatatoTxbs(Txbs2, datas2);

            TextBox[] Txbs3 = { txb_Crg_TemGap1, txb_Crg_TemGap2, txb_Crg_TemGap3 };
            String[] datas3 = dataPack.tempTsd.ChargeBatGap_Hs;
            DatatoTxbs(Txbs3, datas3);

            TextBox[] Txbs4 = { txb_DisCrg_TemH1, txb_DisCrg_TemH2, txb_DisCrg_TemH3 };
            string[] datas4 = dataPack.tempTsd.DisChargeBat_Hs;
            DatatoTxbs(Txbs4, datas4);

            TextBox[] Txbs5 = { txb_DisCrg_TemL14, txb_DisCrg_TemL25, txb_DisCrg_TemL36 };
            string[] datas5 = dataPack.tempTsd.DisChargeBat_Ls;
            DatatoTxbs(Txbs5, datas5);

            TextBox[] Txbs6 = { txb_DisCrg_TemGap14, txb_DisCrg_TemGap25, txb_DisCrg_TemGap36 };
            string[] datas6 = dataPack.tempTsd.DisChargeBatGap_Hs;
            DatatoTxbs(Txbs6, datas6);

            TextBox[] Txbs7 = { txb_CrgGun1, txb_CrgGun2,txb_CrgGun3 };
            string[] datas7 = dataPack.tempTsd.ChargerGun_Hs;
            DatatoTxbs(Txbs7, datas7);

            TextBox[] Txbs8 = { txb_ChipH14, txb_ChipH25, txb_ChipH36 };
            string[] datas8 = dataPack.tempTsd.MainBoard_Hs;
            DatatoTxbs(Txbs8, datas8);

            //温度信息
            txb_MainTemp.Text = dataPack.sysTemp.MainBoard;

            TextBox[] Txbs9 = {txb_SlowGunTemp1, txb_SlowGunTemp2 };
            string [] datas9 = dataPack.sysTemp.SlowGuns;
            DatatoTxbs(Txbs9, datas9);
            //快充
            TextBox[] Txbs10 = { txb_QuickGunTemp1, txb_QuickGunTemp2, txb_QuickGunTemp3, txb_QuickGunTemp4 };
            string[] datas10 = dataPack.sysTemp.QuickGuns;
            DatatoTxbs(Txbs10, datas10);         
        }

        protected override void clearScreen()
        {
            DataPack dataPack = Analysiser.dataPack;
            clearDatas(dataPack.tempTsd.ChargeBat_Hs);
            clearDatas(dataPack.tempTsd.ChargeBat_Ls);
            clearDatas(dataPack.tempTsd.ChargeBatGap_Hs);
            clearDatas(dataPack.tempTsd.DisChargeBat_Hs);
            clearDatas(dataPack.tempTsd.DisChargeBat_Ls);
            clearDatas(dataPack.tempTsd.DisChargeBatGap_Hs);
            clearDatas(dataPack.tempTsd.ChargerGun_Hs);
            clearDatas(dataPack.tempTsd.MainBoard_Hs);
            clearDatas(dataPack.sysTemp.SlowGuns);
            clearDatas(dataPack.sysTemp.QuickGuns);
        }
      
        





}

}
