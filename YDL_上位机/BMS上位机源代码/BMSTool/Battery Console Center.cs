using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
//using BMSTool;

namespace BMSTool
{
    public partial class BrowserForm : Form
    {
        public BrowserForm()
        {
            InitializeComponent();
        }
        public string UserType;
        //字段声明
        private List<FormSize> OldForms = new List<FormSize>();

        //记录窗体初始的大小的结构体
        private struct FormSize
        {
            public string FormName;
            public int width;
            public int height;

        }
        //BrowserForm窗体加载事件//在里面注册双击移出窗体事件，以及将从窗体移入tabcontrol事件
        private void BrowserForm_Load(object sender, EventArgs e)
        {

            if (UserType == "CLIENT")
            {
                this.Text = "Battery Control Center V2.5.8.2 (用户)";
            }
            else
            {
                this.Text = "Battery Control Center V2.5.8.2 (管理员)";
            }
            tabControl1.Dock = DockStyle.Fill;
            tabControl1.AllowDrop = true;//允许拖拽   
            MainForm m_frm = new MainForm(this);
            //主窗体属性设置
            m_frm.IsMdiContainer = false;
            m_frm.TopLevel = false;
            m_frm.IsMdiContainer = false;
            m_frm.Dock = DockStyle.Fill;
            m_frm.ControlBox = false;
           

            tabPage1.Text = m_frm.Text;
            tabPage1.Controls.Add(m_frm);
            m_frm.Show();
        }


        public TabPage GetTabPage(Point point) //鼠标双击处获得相应的tabpage
        {
            //遍历所有选项卡找到鼠标点击处的选项卡所对应的tabpage
            for (int i = 0; i < this.tabControl1.TabPages.Count; i++)
            {
                if (tabControl1.GetTabRect(i).Contains(point))
                    return tabControl1.TabPages[i];
            }
            return null;
        }
        //将从窗体放入tabcontrol
        public  void FormtoTab(Form frm)//将form窗体放入tabpage中
        {

            SaveFormSize(OldForms, frm);
            TabPage page = new TabPage();
            //page属性设定
            //
            page.Text = frm.Text;
            for (int i = 0; i < tabControl1.TabPages.Count; i++)
            {
                if (tabControl1.TabPages[i].Text == page.Text)//如果窗体已经打开，就跳转到指定位置
                {
                    tabControl1.SelectedIndex = i;
                    frm.Dispose();
                    return;
                }

            }

            frm.TopLevel = false;

            page.Controls.Add(frm);

            frm.Dock = DockStyle.Fill;
            frm.FormBorderStyle = FormBorderStyle.None;

            tabControl1.AllowDrop = false;

            //那就把界面转向相应的选项卡，不再新增，
            tabControl1.Controls.Add(page);
            frm.Location = tabControl1.Location;
            frm.StartPosition = FormStartPosition.WindowsDefaultBounds;
            tabControl1.SelectedIndex = tabControl1.TabPages.Count - 1;//获取当前选项卡页
            frm.Location = tabControl1.Location;
            frm.Show();
        }

        //双击弹出
        private void tabControl1_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            TabPage page = GetTabPage(new Point(e.X, e.Y));
            //限制主窗体
            if (page.Text == "主窗体")
            {
                return;
            }
            var frm1 = page.Controls[0] as Form;//page中的索引号为0的控件为form窗体
                                                //frm1的属性设置

            frm1.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Sizable;//边框大小可变
            frm1.Parent = null;//frm1没有包含其的父窗体
            frm1.TopLevel = true;//frm1为顶级级别
            frm1.Owner = this;//parent和owner的区别在于：前者是容器（构件在其范围内显示或移动），物理层面的可视化
                              //owner是构件的内存管理，负责对象的创建与销毁
            SetFormSize(frm1);
            if (frm1.WindowState == FormWindowState.Minimized)
            { frm1.WindowState = FormWindowState.Normal; }

            frm1.Show();
            tabControl1.TabPages.Remove(page);//移出page

            //单击从窗体最小化选项进入浏览器

            frm1.Resize += (ob, ea) =>
            {
                if (frm1.WindowState == FormWindowState.Minimized)
                {
                    FormtoTab(frm1);

                }
            };

        }

        private void tabControl1_DrawItem(object sender, DrawItemEventArgs e)
        {
            //画一个等同于选项卡大小的矩形
            Rectangle myTabRect = tabControl1.GetTabRect(e.Index);
            //绘制选项卡标签

            e.Graphics.FillRectangle(Brushes.White, e.Bounds.X, e.Bounds.Y, e.Bounds.Width, e.Bounds.Height);
            e.Graphics.DrawString(tabControl1.TabPages[e.Index].Text, Font, SystemBrushes.ControlText, myTabRect.X + 4, myTabRect.Y + 4);//绘制文字部分

            //绘制x
            using (Pen pen = new Pen((Color.LightSlateGray), 1))
            {
                Point p1 = new Point(myTabRect.X + myTabRect.Width - 6, myTabRect.Y + 6);
                Point p2 = new Point(myTabRect.X + myTabRect.Width - myTabRect.Height + 6, myTabRect.Y + myTabRect.Height - 6);
                e.Graphics.DrawLine(pen, p1, p2);
                Point p3 = new Point(myTabRect.X + myTabRect.Width - myTabRect.Height + 6, myTabRect.Y + 6);
                Point p4 = new Point(myTabRect.X + myTabRect.Width - 6, myTabRect.Y + myTabRect.Height - 6);
                e.Graphics.DrawLine(pen, p3, p4);
            }
            e.Graphics.Dispose();
            //注册鼠标单击tabcontrol1事件,在合适的区域删除tabpage//注册事件就是每次画完图注册一次
            tabControl1.MouseClick += (ob, ea) =>//指令+数据（每次更新了）
            {
                if (tabControl1.SelectedIndex == 0)
                {}
                else
                {
                    int x = ea.X;
                    int y = ea.Y;
                    bool isClose = (x > myTabRect.X + myTabRect.Width - myTabRect.Height) && (x < myTabRect.X + myTabRect.Width) && (y > myTabRect.Y) && (y < myTabRect.Y + myTabRect.Height);
                    if (isClose)
                    {
                        TabPage tabPage = tabControl1.SelectedTab;
                      //  Control.ControlCollection collection = tabPage.Controls;
                        this.tabControl1.TabPages.Remove(tabPage);
                    }
                    else { }
                }
               
            };

        }
        private void SaveFormSize(List<FormSize> FormSizes, Form form)
        {
            FormSize FormSize;
            FormSize.height = form.Height;
            FormSize.width = form.Width;
            FormSize.FormName = form.Name;
            if (FormSizes.Count == 0)
            {
               FormSizes.Add(FormSize);
            }
            bool flag = false;
            for (int i = 0; i <FormSizes.Count; i++)
            {
                if (form.Name == FormSizes[i].FormName)
                {
                    flag = true;
                }
                else { }
            }
            if (flag == true)
            {
               
            }
            else { FormSizes.Add(FormSize); }
        }
        private void SetFormSize(Form form)
        {
            foreach (FormSize temFormSize in OldForms)
            {
                if (form.Name == temFormSize.FormName)
                {
                    form.Width = temFormSize.width;
                    form.Height = temFormSize.height;
                    form.WindowState = FormWindowState.Normal;
                }


            }
        }

       
    }
}

