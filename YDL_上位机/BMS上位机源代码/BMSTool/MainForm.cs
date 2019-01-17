using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Xml;
using System.Text;
using System.Windows.Forms;
using BMSTool.Comm;


using System.Threading;
using BMSTool.Forms;
using System.Runtime.InteropServices;
using System.IO;
using System.Windows.Forms.DataVisualization.Charting;


using NPOI.SS.UserModel;
using NPOI.HSSF.UserModel;
using NPOI.XSSF.UserModel;
using System.Reflection;
using BMSTool.ReadInfos;
using BMSTool.OtherClass;
using BMSTool.Analysiss;
using BMSTool.Analysiss;
using BMSTool.Monitor;
using BMSTool.DataPacks;
using BMSTool.Analysiss.MainControl.RealTime;

namespace BMSTool
{

    public struct CanDeviceInfo
    {
        public UInt32 CanType;
        public UInt32 DeviceIndex;
        public UInt32 CanIndex;
        public string Config;
    }
    public struct CanDataFrame
    {
        public string frameData;
        public string frameID;
        public string frameCmd;
        public string frameType;
    }

    public partial class MainForm : BasicForm  //
    {
        [DllImport("user32.dll")]//得到按键函数
        private static extern int GetAsyncKeyState(int vKey);
        public CanComm myCanComm;
        private CsvWriter csvWriter;
        public BrowserForm browserForm;
        public string UserType;
        private StreamWriter historySW; 

        private CanCommunication canCommunication;
        public MainForm(BrowserForm bfrm)//初始化并且对类型、箱体、模组、单体进行最大数构建 
        {
            InitializeComponent();
            UserType = bfrm.UserType;
            browserForm = bfrm;
            BandModuleError();
            BandSwitch();
        }

        private void BandSwitch()
        {
            PictureBox[] pictureBoxs = { ptb_MainPosRelay, ptb_MainNegRelay, ptb_QuickCrgRelay, ptb_PrechargeRelay };
            for (int i = 0; i < pictureBoxs.Length; i++)
            {
               pictureBoxs[i].Click += SwitchSet;
            }
        }
        private void BandModuleError()
        {
            Button []buttons =
                {btn_From0,btn_From1,btn_From2,btn_From3,btn_From4,
                btn_From5,btn_From6,btn_From7,btn_From8,btn_From9};
            for (int i = 0; i < buttons.Length; i++)
            {
                Button btn = buttons[i];
                btn.Click += (object o, EventArgs e) =>
                {
                    string str = btn.Text;
                    int BoxNun = Convert.ToInt32(str);
                    ModuleError moduleError = new ModuleError(BoxNun);
                    browserForm.FormtoTab(moduleError);
                };
            }
          
        }      


       
        //1.声明自适应类实例  
        AutoSizeFormClass asc = new AutoSizeFormClass();
        //2. 为窗体添加Load事件，并在其方法Form1_Load中，调用类的初始化方法，记录窗体和其控件的初始位置和大小  

        private void MainForm_Load(object sender, EventArgs e)
        {
            asc.controllInitializeSize(this);

        }
        //3.为窗体添加SizeChanged事件，并在其方法Form1_SizeChanged中，调用类的自适应方法，完成自适应 
        private void MainForm_Resize(object sender, EventArgs e)
        {
            asc.controlAutoSize(this);
        }
   
        public bool CanSend(string data, string frameType, string frameCmd, string frameID)
        {
            if (myCanComm != null)
            {
                bool Success = myCanComm.Send(data, frameType, frameCmd, frameID);
                if (Success)
                {
                    if (canCommunication != null)
                    {
                        canCommunication.Display("发送", frameID, frameCmd, frameType, data);
                    }
                    else{ }
                }
                else { }

                return Success;
            }
            else
            {
                MessageBox.Show("请打开CAN通信设备！");
                return false;
            }
          
        }
        private void tmr_AnalysisData_Tick(object sender, EventArgs e)
        {
            CanDataFrame[] CanDataFrames = myCanComm.RevData();
            if (CanDataFrames != null)
            {
                for (int i = 0; i < CanDataFrames.Length; i++)
                {
                    CanDataFrame canDataFrame = CanDataFrames[i];
                    List<Analysiser.CsvUnitFormat> csvUnits =
                        Analysiser.Main(canDataFrame.frameID, canDataFrame.frameData);
                    
                    if (canCommunication !=null)
                    {
                        canCommunication.Display("接收", canDataFrame.frameID, "标准帧", "数据帧", canDataFrame.frameData);
                    }
                    else { }
                    if (historySW != null)
                    {
                      
                        string current = DateTime.Now.ToString();
                        string data = current + "," + canDataFrame.frameID + "," + canDataFrame.frameData;
                        historySW.WriteLine(data);
                    }
                    else
                    {
                    }
                }
            }

            else { return; }
            
        }
        private void btn_ReadSysPara_Click(object sender, EventArgs e)
        {
            if (btn_ReadSysPara.Text == "读取系统参数")
            {
                if (myCanComm != null)
                {
                    tmr_AnalysisData.Enabled = true;

                    btn_ReadSysPara.Text = "停止读取参数";
                    string frameData = "";
                    for (int i = 0; i < 3; i++)
                    {
                        frameData = "AAAA" + "FFFF" + "FFFF" + "FFFF";
                        sendStartStop(frameData);
                    }
                    Thread.Sleep(100);
                    tmr_Requester.Enabled = true;

                }
                else
                {
                    MessageBox.Show("请先打开CAN设备！");

                }

            }
            else
            {
                tmr_AnalysisData.Enabled = false;
                tmr_Requester.Enabled = false;
                btn_ReadSysPara.Text = "读取系统参数";
                string frameData = "00FF" + "FFFF" + "FFFF" + "FFFF";
                sendStartStop(frameData);
            }


        }
        private bool sendStartStop(string frameData)
        {
           string frameType = "扩展帧";
           string frameCmd = "数据帧";
           string frameID = "19FFF4C0";    
            if (CanSend(frameData, frameType, frameCmd, frameID) == false)
            {
                MessageBox.Show("发送失败，请检查连接！");
                return false;
            }
            else
            {
                return true;
            }
        }
       


        private void ptb_CanSet_Click(object sender, EventArgs e)
        {
            CanSet canSet = new CanSet(this);
            browserForm.FormtoTab(canSet);
        }

        private void lab_Soc_Click(object sender, EventArgs e)
        {
            //SocSet socSet = new SocSet(this);
            //browserForm.FormtoTab(socSet);
        }

        private void lab_Soh_Click(object sender, EventArgs e)
        {
            SohSet sohSet = new SohSet(this);
            browserForm.FormtoTab(sohSet);
        }

        private void ptb_BatPara_Click(object sender, EventArgs e)
        {
           //BatCellPar batCellPar = new BatCellPar(this);
           //browserForm.FormtoTab(batCellPar);
        }

        private void tmr_TestF6_Tick(object sender, EventArgs e)
        {
            if (GetAsyncKeyState(117) != 0)
            {//F6数据显示
                if (canCommunication == null)
                {
                    canCommunication = new CanCommunication(this);
                }
                else { }
                 browserForm.FormtoTab(canCommunication);
            }
        }

        protected override void DataToForm()
        {
            DataPack dataPack = new DataPack();
            dataPack = Analysiser.dataPack;
            lab_Soc.Text = JudgeValue(dataPack.soc.Read )+ "%";
            lab_Soh.Text = JudgeValue(dataPack.Soh) +"%";
            
            //电池及其系统参数类信息//1800
         
            lab_BatType.Text =dataPack.batInfo.Type;
            lab_CapacityPerBat.Text = JudgeValue( dataPack.batInfo.Capacity);
            //  lab_InResistInfo.Text = JudgeValue( dataPack.batInfo.Resist);
            int stringNum = dataPack.batInfo.stringNum;
            int paraNum = dataPack.batInfo.paraNum;
            if (stringNum != 0)
            {
                lab_StringNum.Text =stringNum .ToString();
            }
            else
            {
                lab_StringNum.Text = "";
            }
            if (paraNum != 0)
            {
                lab_paraNum.Text =paraNum.ToString();
            }
            else
            {
                lab_paraNum.Text = "";
            }
            lab_CycleLife.Text = JudgeValue( dataPack.batInfo.CycleLife);
            //电池自检信息//这个信息要单列出来
            lab_SelfCheckState.Text = JudgeValue(dataPack.selfChecK.state);
            lab_Error0.Text = JudgeValue(dataPack.selfChecK.errorCode[0]);
            lab_Error1.Text = JudgeValue(dataPack.selfChecK.errorCode[1]);
            lab_Error2.Text = JudgeValue(dataPack.selfChecK.errorCode[2]);
            lab_Error3.Text = JudgeValue(dataPack.selfChecK.errorCode[3]);

            lab_WorkCondition.Text = JudgeValue(dataPack.timeInfo.SysState);
            //电流信息
            lab_TotalCur.Text = JudgeValue(dataPack.sysCur.Test);
            lab_CrgCurLim.Text = JudgeValue(dataPack.sysCur.ChargeLim);
            lab_disCrfgLim.Text = JudgeValue(dataPack.sysCur.DisChargeLim);
            lab_MaxFeed.Text = JudgeValue(dataPack.sysCur.MaxFeedLim);
            //电压信息
            lab_TotalVol.Text = JudgeValue(dataPack.sysVol.CellAcc);
            lab_CellMaxVol.Text = JudgeValue( dataPack.sysVol.Cell_H);
            lab_CellMinVol.Text = JudgeValue(dataPack.sysVol.Cell_L);
            lab_CellVolGap.Text = JudgeValue(dataPack.sysVol.CellGap);
            //温度信息
            lab_CellTemp_H.Text = JudgeValue( dataPack.sysTemp.Cell_H);
            lab_CellTemp_L.Text = JudgeValue(dataPack.sysTemp.Cell_L);
            lab_CellTempGap.Text = JudgeValue( dataPack.sysTemp.CellGap);
            lab_MainBoardTemp.Text = JudgeValue( dataPack.sysTemp.MainBoard);

            //绝缘状态
            DataPack.SysInsual insual = dataPack.sysInsual;
            lab_PosVol.Text = JudgeValue( insual.PosVol);
            lab_NegVol.Text = JudgeValue(insual.NegVol);
            lab_PosResis.Text = JudgeValue( insual.PosResis);
            lab_NegResis.Text = JudgeValue( insual.NegResis);
            //均衡

            bool state = false;
            bool[,] balances = Analysiser.dataPack.balance;
            for (int i = 0; i < 10; i++)
            {
                for (int j = 0; j < 32; j++)
                {
                    if (balances[i, j])
                    {
                        state = true;
                    }
                }
            }
            if (state)
            {
                ptb_BalanceSwitch.ImageLocation = Application.StartupPath + @"\Photo\开关_开.png";
            }
            else
            {
                ptb_BalanceSwitch.ImageLocation = Application.StartupPath + @"\Photo\开关_关.png";

            }
            

            //开关状态
            PictureBox[] pictureBoxs = { ptb_MainPosRelay, ptb_MainNegRelay, ptb_QuickCrgRelay, ptb_PrechargeRelay };
            DataPack.SwitchState State = Analysiser.dataPack.Switch;
            bool[] flags = { State.mainPos, State.mainNeg, State.QuickCharge, State.PreCharge };
            SwitchDisplay(pictureBoxs, flags);
            

            //系统故障
            if (ID1880C0F4.IsError)
            {
                ptb_SysIsError.ImageLocation = Application.StartupPath
                    + @"\Photo\报警_绿色.png";
                lab_SysError.Text = "电池系统正常";
            }
            else
            {
                ptb_SysIsError.ImageLocation = Application.StartupPath
                 + @"\Photo\报警_红色.png";
                lab_SysError.Text = "电池系统故障";
            }

            lab_TotalCrgAH.Text = JudgeValue( dataPack.kWH.AccCharge);
            lab_TotalDisCrgAH.Text = JudgeValue( dataPack.kWH.AccDisCharge);
            lab_OnceCrgAH.Text = JudgeValue(dataPack.kWH.OneCharge);
            lab_OnceDisCrgAH.Text = JudgeValue(dataPack.kWH.OneDisCharge);
        }
        protected override void clearScreen()
        {
            DataPack dataPack = new DataPack();
            dataPack = Analysiser.dataPack;
            dataPack.soc.Read = "";
            dataPack.Soh="";

            //电池及其系统参数类信息//1800

           dataPack.batInfo.Type="";
           dataPack.batInfo.Capacity="";
            dataPack.batInfo.Resist="";
            dataPack.batInfo.CycleLife="";
            dataPack.batInfo.paraNum = 0;
            dataPack.batInfo.stringNum = 0;
            //电池自检信息//这个信息要单列出来
           dataPack.selfChecK.state="";
           dataPack.selfChecK.errorCode[0]="";
            dataPack.selfChecK.errorCode[1]="";
            dataPack.selfChecK.errorCode[2]="";
           dataPack.selfChecK.errorCode[3]="";

           dataPack.timeInfo.SysState="";
            //电流信息
           dataPack.sysCur.Test="";
            dataPack.sysCur.ChargeLim="";
            dataPack.sysCur.DisChargeLim="";
           dataPack.sysCur.MaxFeedLim="";
            //电压信息
           dataPack.sysVol.CellAcc="";
           dataPack.sysVol.Cell_H="";
            dataPack.sysVol.Cell_L="";
          dataPack.sysVol.CellGap="";
            //温度信息
           dataPack.sysTemp.Cell_H="";
            dataPack.sysTemp.Cell_L="";
            dataPack.sysTemp.CellGap="";
           dataPack.sysTemp.MainBoard="";

            //绝缘状态
            DataPack.SysInsual insual = dataPack.sysInsual;
           insual.PosVol="";
            insual.NegVol="";
            insual.PosResis="";
           insual.NegResis="";
            dataPack.sysInsual = insual;

          

            //开关状态
           
            DataPack.SwitchState State = Analysiser.dataPack.Switch;
            bool[] flags = { State.mainPos, State.mainNeg, State.QuickCharge, State.PreCharge };
            for (int i = 0; i < flags.Length; i++)
            {
                flags[i] = false;
            }

            Analysiser.dataPack = dataPack;
            //系统故障
            ID1880C0F4.IsError = true;
           dataPack.kWH.AccCharge="";
           dataPack.kWH.AccDisCharge="";
            dataPack.kWH.OneCharge="";
           dataPack.kWH.OneDisCharge="";
        }
        private string JudgeValue(string str)
        {
            if (str == null)
            {
                str = "0";
            }
            else
            {
                if (str == "")
                {
                    str = "0";
                }
            }
            return str;
        }
        private void SwitchDisplay(PictureBox[] ptbs,bool[] flags)
        {
            for (int i = 0; i < ptbs.Length; i++)
            {
                if (flags[i])
                {
                    ptbs[i].ImageLocation= Application.StartupPath + @"\Photo\开关_开.png";
                }
                else
                {
                    ptbs[i].ImageLocation = Application.StartupPath + @"\Photo\开关_关.png";
                }
            }
        }
        private void SwitchSet(Object sender, EventArgs e)
        {
            //判断继电器状态
            PictureBox ptb_Switch = (PictureBox)sender;
            string FileName = ptb_Switch.ImageLocation;
            string  switchstate = "00";
            if (FileName.Contains("开关_开"))
            {
                switchstate = "00";
                ptb_Switch.ImageLocation= Application.StartupPath + @"\Photo\开关_关.png";
            }
            else
            {
                switchstate = "01";
                ptb_Switch.ImageLocation = Application.StartupPath + @"\Photo\开关_开.png";

            }
            //判断哪个继电器，发送不同类型报文
            string Name = ptb_Switch.Name;
            string SendStr = string.Empty;
            switch (Name)
            {
                case "ptb_MainPosRelay":
                    SendStr = SendHelper.ProvideFF(2) + switchstate + SendHelper.ProvideFF(4);
                    break;
                case "ptb_MainNegRelay":
                    SendStr = SendHelper.ProvideFF(3) + switchstate + SendHelper.ProvideFF(3);
                    break;
                case "ptb_PrechargeRelay":
                    SendStr = SendHelper.ProvideFF(4) + switchstate + SendHelper.ProvideFF(2);
                    break;
                case "ptb_QuickCrgRelay":
                    SendStr = SendHelper.ProvideFF(5) + switchstate + SendHelper.ProvideFF(1);
                    break;
            }
            SendStr = "02" + SendStr;
            Boolean success= CanSend(SendStr, "扩展帧", "数据帧", "1915F4C0");
            if (success)
            {
                Seter seter = new Seter(this);
                bool flag = seter.CheckIsRec("1915F4C0");
                seter.DisplayMessage(flag);
            }{ }
           

        }

        private void ptb_HistoryRecord_Click(object sender, EventArgs e)//可以把事件移到前面去避免使用标志位来处理
        {
           if(lab_SaveState.Text == "停止保存")
            {
                string fileFolderPath = System.AppDomain.CurrentDomain.SetupInformation.ApplicationBase;
                fileFolderPath += "历史数据";
                if (!Directory.Exists(fileFolderPath))
                {
                    Directory.CreateDirectory(fileFolderPath);
                }
                string fileName = DateTime.Now.ToLongTimeString().ToString().Replace(":", "_");
                fileName = fileFolderPath + "//" + fileName;
                fileName += ".csv";
                FileStream historyRecorde= new FileStream(fileName, FileMode.OpenOrCreate, FileAccess.Write, FileShare.ReadWrite);
                txt_FilePath.Text = fileName;
                historySW = new StreamWriter(historyRecorde, System.Text.Encoding.UTF8);
                lab_SaveState.Text = "开始保存";
            }
            else
            {
                //csvWriter = null;
              //  historySW.Flush();
              // historySW.Close();
              //  historySW.Dispose();
               historySW = null;
                lab_SaveState.Text = "停止保存";
            }
        }

      

        private void ptb_VolPerBat_Click(object sender, EventArgs e)
        {
            Vol vol = new Vol(this);
            browserForm.FormtoTab(vol);
        }

       
        private void pic_SysTemp_Click(object sender, EventArgs e)
        {
            Temp temp = new Temp(this);
            browserForm.FormtoTab(temp);
        }

        private void ptb_VolCurPerBat_Click(object sender, EventArgs e)
        {
            Current current = new Current(this);
            browserForm.FormtoTab(current);
        }

        private void ptb_InsulationState_Click(object sender, EventArgs e)
        {
            Insula insula = new Insula(this);
            browserForm.FormtoTab(insula);
        }

        private void ptb_SysError_Click(object sender, EventArgs e)
        {
            SysError sysError = new SysError(this);
            browserForm.FormtoTab(sysError);
        }

        private void ptb_PowerMeter_Click(object sender, EventArgs e)
        {
            
        }

        private void pic_Update_Click(object sender, EventArgs e)
        {
            update update = new update(this);
            browserForm.FormtoTab(update);
        }

       
        private void lab_InitialSet_Click(object sender, EventArgs e)
        {
           
        }

        private void panel9_Paint(object sender, PaintEventArgs e)
        {

        }

        private void btnReadInfo_Click(object sender, EventArgs e)
        {
           
            ReadInfo  myReadInfo = new ReadInfo(this);
            browserForm.FormtoTab(myReadInfo);
        }

        private void tmr_Requester_Tick(object sender, EventArgs e)
        {
            string frameData = "AAFF" + "FFFF" + "FFFF" + "FFFF";
            if (myCanComm.isOpen())
            {
                sendStartStop(frameData);
            }
        }

       

        private void btn_iniSet_Click(object sender, EventArgs e)
        {
            InitialSet initialSet = new InitialSet(this);
            browserForm.FormtoTab(initialSet);
        }

        private void ptb_company_Click(object sender, EventArgs e)
        {
            companyInfo companyInfo = new companyInfo();
            browserForm.FormtoTab(companyInfo);
        }

        private void ptb_Balance_Click(object sender, EventArgs e)
        {
            balance balance = new balance();
            browserForm.FormtoTab(balance);
        }
    }




}

