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
using BMSTool.OtherClass;
using BMSTool.Analysiss.Other;
using System.Reflection;
using System.Text.RegularExpressions;

namespace BMSTool.Forms
{
    public partial class BasicForm : Form
    {
       
        public BasicForm()
        {
            InitializeComponent();
        }
        private void 停止刷新ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (contextMenuStrip1.Items[0].Text == "停止刷新")
            {
                tmr_DatatoForm.Enabled = false;
                contextMenuStrip1.Items[0].Text = "开始刷新";
            }
            else
            {
                tmr_DatatoForm.Enabled = true;
                contextMenuStrip1.Items[0].Text = "停止刷新";
            }
        }
        private void tmr_DatatoForm_Tick(object sender, EventArgs e)
        {
            DataToForm();
        }
        protected virtual void DataToForm()//
        {
        }
       protected void DatatoTxbs(TextBox[] Txbs, string[] Datas)
        {
            int Num = Datas.Length;
            for (int i = 0; i < Num; i++)
            {
                Txbs[i].Text = Datas[i];
            }
        }

        private void tsp_ClearScreen_Click(object sender, EventArgs e)
        {
            clearScreen();
        }
        protected virtual void clearScreen()
        {

        }
        protected string judgeFFtoBlack(string data)
        {
            string str = data;
            if (data.Contains("FF"))
            {
                str = "";
            }
            return str;
        }
        protected string judgeFFto0(string data)
        {
            string str = data;
            if (data.Contains("FF"))
            {
                str = "0";
            }
            return str;
        }
        protected void clearDatas(string[] datas)
        {
            for (int i = 0; i < datas.Length; i++)
            {
                datas[i] = null;
            }
        }
    }
    
}

