using BMSTool.Analysiss;
using BMSTool.DataPacks;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Windows.Forms;

namespace BMSTool.Forms
{
    public partial class ModuleError : BasicForm
    {
        private int BoxNum;
        public ModuleError(int BoxNum)
        {
            InitializeComponent();
            this.BoxNum = BoxNum-1;
            this.Text = BoxNum + "号模组故障信息";
        }

       
        protected override void DataToForm()
        {
            Module();
            OpenWire();
        }
        private void Module()
        {
            DataPack.ModuleError error = Analysiser.dataPack.ModuleErrors[BoxNum];
            String str1 = error.ChipTemp_h;
            str1 = Change(str1);
            lab_ChipTemp_H.Text = "采样芯片温度" + str1;
            Set(ptb_ChipTemp_H, str1);
            
            String str4 = error.ChipTemp_h;//这个地方表示了如何在保证代码一致性的基础上，实现多样化显示
            if(str4==""||str4==null||str4=="正常")
            {
                lab_OpenWire.Text = "导线未开路";
            }
            Set(ptb_OpenWire, str4);

        }
        private string Change(string str)
        {
            if ((str ==null))
            {
                str= "正常";
            }
            if (str == "不正常")
            {
                str = "过高";
            }
            return str;
        }
        private void Set(PictureBox ptb,string Feature)
        {
            if ((Feature=="")||(Feature==null)||(Feature=="正常"))
            {
                ptb.ImageLocation = Application.StartupPath + @"\Photo\报警_绿色.png";
            }
            else
            {
                ptb.ImageLocation = Application.StartupPath + @"\Photo\报警_红色.png";

            }
        }
        private void OpenWire()
        {
            Control.ControlCollection controls = panel1.Controls;
            for (int i = 0; i < controls.Count; i++)
            {
                string  name = controls[i].Name;
                if (name.Contains("lab_"))
                {
                    Label label =(Label) controls[i];
                    string Pattern = "lab_(?<Row>\\d+)_(?<Column>\\d+)";//其实这里使用数组比正则表达式可能更好，那样后台和界面的关系更松
                    Regex regex = new Regex(Pattern);
                    Match match = regex.Match(name);
                    if (match.Success)
                    {
                        string str1 = match.Groups[1].Value;
                        int Row = Convert.ToInt32(str1);
                        string str2 = match.Groups[2].Value;
                        int Column = Convert.ToInt32(str2);
                        bool Feature = Analysiser.dataPack.openWires[BoxNum, Row, Column];
                        Set(label, Feature);
                    }
                    else { }
                    
                   

                }
                else { }
            }
        }
        private void Set(Label label, bool Feature)
        {
            if (Feature)
            {
                label.BackColor = Color.Red;
            }
            else
            {
                label.BackColor = Color.Green;
            }
        }
    }
}
