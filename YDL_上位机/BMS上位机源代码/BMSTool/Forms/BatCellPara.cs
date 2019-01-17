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

namespace BMSTool.Forms
{
    public partial class BatCellPara : Form
    {
        public MainForm myMainForm;
        public bool FormOpen;
        public bool UpdateFlag;
        public BatCellPara(MainForm mainForm)
        {
            InitializeComponent();
            comboBox1.SelectedIndex = 0;

        }

      
    }
}
