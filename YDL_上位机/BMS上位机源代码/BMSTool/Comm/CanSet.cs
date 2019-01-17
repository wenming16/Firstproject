using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using BMSTool;
namespace BMSTool.Comm
{
    
    public partial class CanSet : Form
    {
     
        private MainForm mainForm;
       

        public CanSet(MainForm _mainForm)
        {
            mainForm = _mainForm;//关联着同一个引用
            InitializeComponent();
            cmb_BaudRate.SelectedIndex = 2;
            cmb_CommCycle.SelectedIndex = 0;
            cmb_CanType.SelectedIndex = 0;
            cmb_CanChannel.SelectedIndex = 0;
            cmb_DeviceIndex.SelectedIndex = 0;

        }

        private void btn_OpenCan_Click(object sender, EventArgs e)
        {
           
            string  DeviceType = cmb_CanType.Items[cmb_CanType.SelectedIndex].ToString();
            string CanChannel = cmb_CanChannel.Items[cmb_CanChannel.SelectedIndex].ToString();
            string DeviceIndex = cmb_DeviceIndex.Items[cmb_DeviceIndex.SelectedIndex].ToString();
            string LvboID = "0x00000000";//textBox1.Text.Trim();
            string PinBiMa = "0xFFFFFFFF";//textBox2.Text.Trim();
            string Config = cmb_BaudRate.Items[cmb_BaudRate.SelectedIndex].ToString() + LvboID + PinBiMa;
            CanDeviceInfo canDeviceInfo = new CanDeviceInfo();
            switch (CanChannel)
            {
                case "CAN1":
                    canDeviceInfo.CanIndex = 0;
                    break;
                case "CAN2":
                    canDeviceInfo.CanIndex = 1;
                    break;
            }
            canDeviceInfo.DeviceIndex = Convert.ToUInt32(DeviceIndex) ;
            canDeviceInfo.Config = Config;
            switch (DeviceType)
            {
                case "USBCAN-II":
                    canDeviceInfo.CanType = 4;
                    mainForm.myCanComm = new CanComm_II(canDeviceInfo);
                    break;
                case "USBCAN-E-mini":
                    canDeviceInfo.CanType = 20;
                   mainForm.myCanComm = new CanComm_E(canDeviceInfo);
                    break;
            }
            bool success= mainForm.myCanComm.Open();
            if (success)
            {
                string CanDeviceState = "CAN信息：" + DeviceIndex + "-" + CanChannel + "波特率：" + cmb_BaudRate.Items[cmb_BaudRate.SelectedIndex].ToString() + "打开！";
                toolStripStatusLabel1.Text = CanDeviceState;
                mainForm.lab_BaudRate.Text = cmb_BaudRate.Items[cmb_BaudRate.SelectedIndex].ToString();
                mainForm.lab_CommState.Text = "打开";
            }
            else
            {
                MessageBox.Show("打开失败");
            }


        }

        private void btn_CanClose_Click(object sender, EventArgs e)
        {
            if (mainForm.myCanComm == null)
            {
                MessageBox.Show("请先打开设备！");
                return;
            }
            else
            {
                string DeviceType = cmb_CanType.Items[cmb_CanType.SelectedIndex].ToString();
                string CanChannel = cmb_CanChannel.Items[cmb_CanChannel.SelectedIndex].ToString();
                string DeviceIndex = cmb_DeviceIndex.Items[cmb_DeviceIndex.SelectedIndex].ToString();
           
                bool Success= mainForm.myCanComm.Close();
                if (Success)
                {
                    string CanDeviceState = CanDeviceState = "CAN信息：" + DeviceIndex + "-" + CanChannel + "波特率：" + cmb_BaudRate.Items[cmb_BaudRate.SelectedIndex].ToString() + "关闭！";
                    toolStripStatusLabel1.Text = CanDeviceState;
                    mainForm.lab_BaudRate.Text = cmb_BaudRate.Items[cmb_BaudRate.SelectedIndex].ToString();
                    mainForm.lab_CommState.Text = "关闭";
                }

                else
                {
                    MessageBox.Show("关闭设备失败！");
                }
              
            }
        }

        private void timer1_Tick(object sender, EventArgs e)
        {

        }
    }
}
