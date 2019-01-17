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
using BMSTool.DataPacks;
using BMSTool.Analysiss;

namespace BMSTool.Forms
{
    public partial class BatCellPar : BasicForm
    {
        public MainForm myMainForm;
        public bool FormOpen;
        public bool UpdateFlag;
        public BatCellPar(MainForm mainForm)
        {
            InitializeComponent();
            cmb_Type.SelectedIndex = 0;
        
        }
      
        private void btn_Set_Click(object sender, EventArgs e)
        {
            
        }

       

        
    protected override void DataToForm()
        {
            DataPack dataPack = Analysiser.dataPack;
            cmb_Type.Text = dataPack.batInfo.Type;
            txb_Capacity.Text = dataPack.batInfo.Capacity;
            txb_CycleLife.Text = dataPack.batInfo.CycleLife;
            txb_Resist.Text = dataPack.batInfo.Resist;
        }
        protected override void clearScreen()
        {
            DataPack dataPack = Analysiser.dataPack;
            dataPack.batInfo.Type = string.Empty;
            dataPack.batInfo.Capacity = string.Empty;
            dataPack.batInfo.CycleLife = string.Empty;
            dataPack.batInfo.Resist = string.Empty;
            Analysiser.dataPack = dataPack;
        }

    }
}
