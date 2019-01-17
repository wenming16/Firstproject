using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using BMSTool;
using BMSTool.Comm;
using System.Threading;
using System.IO;


namespace BMSTool.Forms
{
    public partial class update : Form
    {
        private MainForm mainForm;
        struct Frame
        {
            public string ID;
            public String Data;
        }
        public update(MainForm mainForm)
        {
            InitializeComponent();
            this.mainForm =mainForm;
            cmb_updateObj.SelectedIndex = 0;
        }
      
        //读取版本号
        private void btn_ReadVer_Click(object sender, EventArgs e)
        {

        }

        private void ptb_Updata_Click(object sender, EventArgs e)
        {
            try
            {
                OpenFileDialog ofd = new OpenFileDialog();
                ofd.ShowDialog();
                ofd.InitialDirectory = "C:\\";//初始加载路径为C盘；
                txb_UpdataFile.Text = Path.GetFullPath(ofd.FileName);//显示文件的名字
            }
            catch {  }

        }

        private void btn_Start_Click(object sender, EventArgs e)
        {
            if ((mainForm.myCanComm == null) || (!mainForm.myCanComm.isOpen()))
            {
                MessageBox.Show("请打开CAN通信！");
                return;

            }
            DialogResult dialog = MessageBox.Show
                ("是否确定升级？", "警告", MessageBoxButtons.OKCancel, MessageBoxIcon.Question);
            if (dialog == DialogResult.OK)
            {
                mainForm.tmr_AnalysisData.Enabled = false;
                mainForm.tmr_Requester.Enabled = false;
                if (rbtn_HaveApp.Checked == true) //建立连接
                {
                   
                    bool Success = PrepareForUpdate();
                    if (Success)
                    {
                        Rtb_Comm.Text = "";
                        Rtb_Comm.Text += "准备升级，请等待..." + "\r\n";
                        //开始升级
                        Thread.Sleep(500);
                        Thread UpdataThread = new Thread(UpdateProcess);
                        Control.CheckForIllegalCrossThreadCalls = false;
                        UpdataThread.Start();
                      //  UpdateProcess();
                    }
                    else
                    {
                        MessageBox.Show("升级会话失败！");
                    }
                }
                else
                {
                    Rtb_Comm.Text = "";
                    Rtb_Comm.Text += "准备升级，请等待..." + "\r\n";
                    //开始升级
                    Thread UpdataThread = new Thread(UpdateProcess);
                    Control.CheckForIllegalCrossThreadCalls = false;
                    UpdataThread.Start();
                  //  UpdateProcess();
                    
                }
                mainForm.tmr_AnalysisData.Enabled = true;
            } 


        }
        private void UpdateProcess()
        {
            //建立S19文件连接
            
            string FileName = txb_UpdataFile.Text;
            if (FileName =="")
            {
                MessageBox.Show("路径的形式不合法！");
            }
            else { }
            pgb_Updata.Minimum = 0;
            int Num= File.ReadAllLines(FileName).Length;
            pgb_Updata.Maximum =Num;
            pgb_Updata.Value = 0;
            StreamReader sr = new StreamReader(FileName, Encoding.Default);
            Rtb_Comm.Text += "正在发送升级文件，请等待..." + "\r\n";
            //循环发送
            string LineData = string.Empty;
            int Count = 0;
            while ((LineData = sr.ReadLine()) != null)
            {
                //将足够8个字节的发送
                SendUpdata(LineData);
                CanDataFrame[] canDatas= mainForm.myCanComm.RevData();
                Count++;
                pgb_Updata.Value += 1;
            }
            sr.Close();
          
            pgb_Updata.Value = pgb_Updata.Maximum;
           
            Rtb_Comm.Text = "升级完成" + "\r\n";
            Rtb_Comm.Text += "当前版本：" + txb_OldVersion.Text + "\r\n";
     //       mainForm.tmr_Requester.Enabled = true;
           
        }
        private void SendUpdata(string LineData)//发送一行数据
        {
            string ID = "";
            ID = getID();
            int SendCount = ComputeCount(LineData);//计算多少帧
            for (int i = 0; i < SendCount; i++)
            {
                string SendStr = string.Empty;
                for (int j = 0; j < 8; j++)
                {
                    int Index = i * 8 + j;
                    int Length = LineData.Length;
                    if (Index < Length)
                    {
                        int TemInt = Convert.ToInt32(LineData[Index]);
                        string str = (string.Format("{0:X}", TemInt));
                        SendStr += str;
                    }
                    else { }
                }
                mainForm.CanSend(SendStr, "标准帧", "数据帧", ID);
                Thread.Sleep(2);
              
            }
        }
        String getID()
        {
            int index = cmb_updateObj.SelectedIndex;
            String obj = cmb_updateObj.Items[index].ToString();
            String ID = "";
            switch (obj)
            {
              
                case"主板":
                    ID = "001";
                    break;
                case "子板1":
                    ID = "002";
                    break;
                case "子板2":
                    ID = "003";

                    break;
                case "子板3":
                    ID = "004";
                    break;
                case "子板4":
                    ID = "005";

                    break;
                case "子板5":
                    ID = "006";
                    break;
                case "子板6":
                    ID = "007";
                    break;
                case "子板7":
                    ID = "008";

                    break;
                case "子板8":
                    ID = "009";
                    break;
                case "子板9":
                    ID = "010";
                    break;
            }
            return ID;
        }
        private int ComputeCount(string SendStr)
        {
            int Count = SendStr.Length / 8;
            int Remainder = SendStr.Length % 8;
            int AllCount = 0;
            if (Remainder != 0)
            {
                AllCount = Count+1;
                
            }
            else
            {
                 AllCount = Count ;
            }
            return AllCount;
        }

        private bool PrepareForUpdate()
        {
            bool Success = false;
            Frame frame = GetFrame();
            String Data = frame.Data;
            String ID = frame.ID;
            


            String Type = "扩展帧";
            String Cmd = "数据帧";

            mainForm.myCanComm.ClearBuffer();
            mainForm.tmr_AnalysisData.Enabled = false;
            mainForm.CanSend( Data, Type, Cmd, ID);
            Thread.Sleep(8000);

            CanDataFrame [] canDatas= mainForm.myCanComm.RevData();
            for (int i = 0; i < canDatas.Length; i++)
            {
                bool b1 = (canDatas[i].frameID == "1FF");
                bool b2 = (canDatas[i].frameData == "C3");
                if (b1 && b2)
                {
                    Success = true;
                }
                else { }
                if (canDatas[i].frameID == "1FF")
                {
                    int k =0;
                }
              
            }
            
            mainForm.tmr_AnalysisData.Enabled = false;
            return Success;
            
        }
        private Frame GetFrame()
        {
            int index = cmb_updateObj.SelectedIndex;
            string updata = cmb_updateObj.Items[index].ToString();
            Frame frame = new Frame();
            frame.ID = "0000F300";
            switch (updata)
            {
                case "主板":
                    frame.Data = "F0AA";
                    break;
                case "子板1":
                    frame.Data = "01AA";
                    break;
                case "子板2":
                    frame.Data = "02AA";
                    break;
                case "子板3":
                    frame.Data = "03AA";
                    break;
                case "子板4":
                    frame.Data = "04AA";
                    break;
                case "子板5":
                    frame.Data = "05AA";
                    break;
                case "子板6":
                    frame.Data = "06AA";
                    break;
                case "子板7":
                    frame.Data = "07AA";
                    break;
                case "子板8":
                    frame.Data = "08AA";
                    break;
                case "子板9":
                    frame.Data = "09AA";
                    break;
                case "子板10":
                    frame.Data = "0AAA";
                    break;
            }
            return frame;
        }

    }

}
