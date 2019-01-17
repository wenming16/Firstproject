using BMSTool.Analysiss;
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
    public partial class balance : BasicForm
    {
        public balance()
        {
            InitializeComponent();
        }
        protected override void DataToForm()
        {
            Label  []labels = { lab_balance0, lab_balance1, lab_balance2, lab_balance3, lab_balance4,
                lab_balance5, lab_balance5, lab_balance6, lab_balance7, lab_balance8, lab_balance9 };
            bool[,] balances = Analysiser.dataPack.balance;
            List<int> list = new List<int>();
            for (int i = 0; i < 10; i++)
            {
                bool flag = false;
                for (int j=0; j < 32; j++)
                {
                    if (balances[i, j] == true)
                    {
                        flag = true;
                        int num = j+1;
                        list.Add(i);
                        labels[i].Text = num + "号电池正在均衡";
                    }
                }
                if (flag == false)
                {
                    labels[i].Text = "无均衡";
                }
            }

        }
        protected override void clearScreen()
        {
            bool[,] balances = Analysiser.dataPack.balance;
            for (int i = 0; i < 10; i++)
            {
                for (int j = 0; j < 32; j++)
                {
                    balances[i, j] = false;
                }
            }
        }

        private void lab_balance1_Click(object sender, EventArgs e)
        {

        }
    }
}
