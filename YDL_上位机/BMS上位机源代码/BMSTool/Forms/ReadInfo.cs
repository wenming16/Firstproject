using BMSTool.Analysiss;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;

namespace BMSTool.Forms
{

    public partial class ReadInfo : Form//类型声明
    {
        private MainForm MainForm;
        public ReadInfo(MainForm frm)//局部变量命名可以简易
        {
            InitializeComponent();
            MainForm = frm;
        }


        /* 使用从窗体解析优点：
         * 1.保持程序框架稳定，利于后期的可扩展和维护也易读
         * 2.代码解析归类，各部分之间结构清晰
         *   缺点：
         * 1.涉及多对多现象，如同多个报文会控制同一窗体，会导致多次调用，
         *   还有一帧报文控制多个窗体
         *   解决方案多次调用窗体和一次调用多个窗体（为了使数据传到内部，
         *   仍然能按规则解析，必须将解析规则和规则标识统一传入从窗体）
         *   
        */
        /*-------------------------------------关于三类信息解析是否分类---------------------------------*/
        /*优点：
         * 1.结构清晰，代码不会冗杂，混乱
         * 缺点：
         * 1.由于处理数据的通用性不强，所以代码复用率低，难以利用抽象提高效率。
         * 2.实时信息读取一条写一条
        /*
         * 读取信息有两类故障信息（EEprom webflash）
          /*将信息分离，直接使用类解析，反正毫无关系
           * 
           * 
         */
        /*------------------------------------使用函数形参传递变量-------------------------------------------*/
        /*
         * 参数确定，函数独立。逻辑清晰，函数的输出和输入都非常明确
         */
        

      
        

        private void btnReadInfoReTme_Click(object sender, EventArgs e)//读取实时信息
        {           
            string frameType = string.Empty;
            string frameCmd = string.Empty;
            string frameID = string.Empty;
            string frameData = string.Empty;
            frameType = "扩展帧";
            frameCmd = "数据帧";
            frameID = "1950F4C0";
            string V1 = "FFFF";
            string V2 = "AAFF";
            string V3 = "FFFFFFFF";
            frameData = V1 + V2 + V3;
            if (MainForm.CanSend(frameData, frameType, frameCmd, frameID) == false)
            {
                MessageBox.Show("发送失败，请检查连接！");
                return;
            }
            frameID = "19FFF4C0";
            V1 = "55FF";
            V2 = "FFFF";
            V3 = "FFFFFFFF";
            frameData = V1 + V2 + V3;
            if (MainForm.CanSend(frameData, frameType, frameCmd, frameID) == false)
            {
                MessageBox.Show("发送失败，请检查连接！");//使下位机停止发送正常报文
                return;
            }
            Seter seter = new Seter(MainForm);
            bool flag= seter.CheckIsRec(frameID);
            DisplayMessage(flag);
        }
        private void DisplayMessage(bool flag)
        {
            if (flag)
            {
                MessageBox.Show("读取成功！");
            }
            else
            {
                MessageBox.Show("读取失败");
            }
        }
        private void btnEEPromRead_Click(object sender, EventArgs e)//读取EEPROM故障信息
        {
            if ((MainForm.myCanComm == null)||(!MainForm.myCanComm.isOpen()))
            {
                MessageBox.Show("请打开CAN设备！");
                return;
            }
            string InfoType = "EEPROM故障信息";
            string frameType = string.Empty;
            string frameCmd = string.Empty;
            string frameID = string.Empty;
            string frameData = string.Empty;

            frameType = "扩展帧";
            frameCmd = "数据帧";
            frameID = "1950F4C0";
            string V1 = "AAFF";
            string V2 = "FFFF";// SelectIndex[1].ToString("X4
            string V3 = "FFFFFFFF";

            frameData = V1 + V2 + V3;
            
            if (MainForm.CanSend( frameData, frameType, frameCmd, frameID) == false)
            {
                MessageBox.Show("发送失败，请检查连接！");
                return;
            }
            frameID = "19FFF4C0";
            V1 = "55FF";
            V2 = "FFFF";
            V3 = "FFFFFFFF";
            frameData = V1 + V2 + V3;
            if (MainForm.CanSend( frameData, frameType, frameCmd, frameID) == false)
            {
                MessageBox.Show("停止帧发送失败，请检查连接！");
                return;
            }
            //Seter seter = new Seter(MainForm);
           // bool flag = seter.CheckIsRec(frameID);
           // DisplayMessage(flag);
        }

       

       
     
        private void btnEEpromClear_Click(object sender, EventArgs e)//清除EEPROM故障信息
        {

            string frameType = "扩展帧";
            string frameCmd = "数据帧";
            string frameID = "1950F4C0";
            string V1 = "AAFF";
            string V2 = "FFFF";// SelectIndex[1].ToString("X4
            string V3 = "FFFFFFFF";
            string frameData = V1 + V2 + V3;
            if (MainForm.CanSend(frameData, frameType, frameCmd, frameID) == false)
            {
                MessageBox.Show("发送失败，请检查连接！");
                return;
            }
            else
            {
                Seter seter = new Seter(MainForm);
                 bool flag = seter.CheckIsRec(frameID);
                if (flag)
                {
                    MessageBox.Show("清除成功！");
                }
                else
                {
                    MessageBox.Show("清除失败！");
                }
            }

        }
        private void btnClearFlash_Click(object sender, EventArgs e)//清除flash故障信息
        {

        }

    }

}
