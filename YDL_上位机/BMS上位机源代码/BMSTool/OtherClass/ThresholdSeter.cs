using BMSTool.Analysiss;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading;
using System.Windows.Forms;

namespace BMSTool.OtherClass
{
    class ThresholdSeter//通过调用这个类实现一键标定
    {
        private MainForm MainForm;
        private Form SetForm;
        public ThresholdSeter(MainForm mainForm,Form SetForm)
        {
            this.MainForm = mainForm;
            this.SetForm = SetForm;
        }
      
        //绑定阈值标定事件
        public void BandTsdSet(string ID, int WordsNum, double Unit, int OffSet)
        {
            Type type = SetForm.GetType();
            FieldInfo[] fields = type.GetFields(BindingFlags.NonPublic | BindingFlags.Instance);
            for (int i = 0; i < fields.Length; i++)
            {
                Object OB = fields[i].GetValue(SetForm);
                if (OB != null)
                {
                    if (OB.GetType() == typeof(Button))
                    {
                        Button btn = (Button)fields[i].GetValue(SetForm);
                        if (btn.Text == "标定")
                        {
                            btn.Click += (object sender, EventArgs e) =>
                            {
                                string name = btn.Name;
                                string Num = name.Substring(name.Length - 2);
                                string SendTsd = "";
                                string[] Tsds = GetTsdDatas(sender);//得到阈值数组
                                if ((ID == "1911F4C0")&&(Num=="02"))
                                {
                                    string str = Datas2Tsd(Tsds, WordsNum, Unit, 0);//处理温差没有偏移量
                                    SendTsd = Num + str + "FF";
                                    

                                }
                                else
                                {
                                    string str = Datas2Tsd(Tsds, WordsNum, Unit, OffSet);//处理阈值
                                    SendTsd = Num + str + "FF";
                                }
                                
                                bool Success= MainForm.CanSend(SendTsd, "扩展帧", "数据帧", ID);
                                if (Success == true)
                                {
                                    Seter seter = new Seter(MainForm);
                                    bool flag= seter.CheckIsRec(ID);
                                    if (flag)
                                    {
                                        MessageBox.Show("标定成功！");
                                    }
                                    else
                                    {
                                        MessageBox.Show("标定失败！");
                                    }
                                }
                                else
                                {
                                    MessageBox.Show("发送失败！");
                                }
                                sendRequest();

                            };


                        }
                        else { }

                    }
                    else { }
                }
                else { }
                

            }

        }
        private string Datas2Tsd(string[] Datas, int WordsNum, double Unit, int OffSet)//(数据集合，每个数据的长度，单位，偏移量)
        {
            int Count = 0;
            if (WordsNum == 2)//使用字节来划分内容
            {
                Count = 3;
            }
            else
            {
                Count = 6;
            }
            string SendStr = string.Empty;
            for (int i = 0; i < Count; i++)//如果是6个字节就补完数据
            {
                if ((Datas[i] == string.Empty)||(Datas[i]==null))
                {
                    string temStr =  SendHelper.ProvideFF(WordsNum);
                    SendStr += temStr;
                }
                else
                { 
                    string Hexadecimal = Compute.str10to16(Datas[i], WordsNum, Unit,OffSet);
                    SendStr += Hexadecimal;
                }
            }
            return SendStr;
        }
        private string[] GetTsdDatas(object ob)//其实这种使用硬编码的方式并不太好，只实现了表面电的自动化，还不如把这个功能分离到其他类
        {
            Button btn = (Button)ob;
            Control control = btn.Parent;
            Control.ControlCollection controls = control.Controls;
            string[] Data = new string[6];
            for (int j = 0; j < controls.Count; j++)
            {
                Control ctl = controls[j];
                Type type = ctl.GetType();
                if (type == typeof(TextBox))
                {
                    string name = ctl.Name;
                    int start = name.Length - 1;
                    string str = name.Substring(start);
                    int index = Convert.ToInt32(str)-1;

                    string content = ctl.Text;
                    Data[index] = content;
                }
                else { }
            }
            return Data;
        }
        private bool sendRequest()
        {
            string frameData= "AAAA" + "FFFF" + "FFFF" + "FFFF";
            string frameType = "扩展帧";
            string frameCmd = "数据帧";
            string frameID = "19FFF4C0";
            bool flag = false;
            for (int i = 0; i < 3; i++)
            {
                if (MainForm.CanSend(frameData, frameType, frameCmd, frameID) == false)
                {
                    MessageBox.Show("发送失败，请检查连接！");
                    flag = true;
                    //return false;
                }
                else
                {
                    //return true;  
                }
            }
            Thread.Sleep(100);
            return flag;

        }

    }
}
