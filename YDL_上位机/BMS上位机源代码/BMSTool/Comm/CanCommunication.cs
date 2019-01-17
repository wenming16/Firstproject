using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using BMSTool.Comm;
using System.IO;
using System.Threading;
using BMSTool.Analysiss;
using BMSTool.Forms;

namespace BMSTool.Monitor
{
    public partial class CanCommunication :BasicForm
    {
        public MainForm mainForm;
        public CanCommunication(MainForm mainForm)
        {
            InitializeComponent();
            Control.CheckForIllegalCrossThreadCalls = false;
            this.mainForm = mainForm;

        }
        int Count = 0;
        bool IsUpData=true;
        

        
        public  void Display(string RevOrSend,string frameID, string frameCmd, string frameType, string data)
        {
            try
            {
                if (IsUpData)
                {
                    int RowIndex = 0;
                    RowIndex = dataGridView1.Rows.Add();
                    Count++;
                    dataGridView1.Rows[RowIndex].Cells[0].Value = Count.ToString();
                    if (RevOrSend == "接收")
                    {
                        dataGridView1.Rows[RowIndex].Cells[1].Value = "接收";
                    }
                    else
                    {
                        dataGridView1.Rows[RowIndex].Cells[1].Value = "发送";
                        dataGridView1.Rows[RowIndex].DefaultCellStyle.BackColor = Color.GreenYellow;
                    }
                    dataGridView1.Rows[RowIndex].Cells[2].Value = System.DateTime.Now;
                    dataGridView1.Rows[RowIndex].Cells[3].Value = frameID;
                    dataGridView1.Rows[RowIndex].Cells[4].Value = frameCmd;
                    dataGridView1.Rows[RowIndex].Cells[5].Value = frameType;
                    dataGridView1.Rows[RowIndex].Cells[6].Value = (data.Length / 2).ToString();
                    string displayData = string.Empty;
                    for (int i = 0; i < data.Length / 2; i++)
                    {
                        displayData += (data.Substring(i * 2, 2) + " ");
                    }
                    dataGridView1.Rows[RowIndex].Cells[7].Value = displayData;
                    dataGridView1.FirstDisplayedScrollingRowIndex = dataGridView1.RowCount - 1;
                    if (Count > 5000)
                    {
                        dataGridView1.Rows.RemoveAt(0);
                    }
                }
                else
                { }
            }
            catch
            {
                string str = "出错了！";
            }
           
            
        }

        private void tsp_Updata_Click(object sender, EventArgs e)
        {
            if (IsUpData)
            {
                IsUpData = false;
                tsp_Updata.Text = "开始刷新";
            }
            else {
                IsUpData = true;
                tsp_Updata.Text = "停止刷新";
            }
        }

        private void btn_Start_Click(object sender, EventArgs e)
        {
            //获取数据并判断是否为空
            string Data = txb_Data.Text.Trim();
            string ID = txb_ID.Text.Trim();
            string CMD = cmb_Cmd.Text.Trim();
            string Type = cmb_type.Text.Trim();
            bool b1 = (Data != "");
            bool b2 = (ID != "");
            bool b3 = (CMD != "");
            bool b4 = (Type != "");
            if (b1 && b2 && b3 && b4)
            {
                //int Times = Convert.ToInt32(txb_Times.Text.Trim());
                //int Cycles = Convert.ToInt32(txb_Cycle.Text.Trim());
                mainForm.CanSend(Data, Type, CMD, ID);
            }
            else
            {
                MessageBox.Show("请将数据补全！");
            }
                
            //发送数据
        }

        private void btn_Stop_Click(object sender, EventArgs e)
        {

        }
    }
}
