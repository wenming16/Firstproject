using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Threading;
using BMSTool.Monitor;
using BMSTool;
namespace BMSTool.Comm
{

    public  class CanComm
    {
        //1.ZLGCAN系列接口卡信息的数据类型。
        public struct VCI_BOARD_INFO
        {
            public UInt16 hw_Version;
            public UInt16 fw_Version;
            public UInt16 dr_Version;
            public UInt16 in_Version;
            public UInt16 irq_Num;
            public byte can_Num;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 20)]
            public byte[] str_Serial_Num;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 40)]
            public byte[] str_hw_Type;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
            public byte[] Reserved;
        }

        /////////////////////////////////////////////////////
        //2.定义CAN信息帧的数据类型。//使用不安全代码
        unsafe public struct VCI_CAN_OBJ
        {
            public uint ID;
            public uint TimeStamp;        //时间标识
            public byte TimeFlag;         //是否使用时间标识
            public byte SendType;         //发送标志。保留，未用
            public byte RemoteFlag;       //是否是远程帧
            public byte ExternFlag;       //是否是扩展帧
            public byte DataLen;
            public fixed byte Data[8];
            public fixed byte Reserved[3];

        }

        //3.定义CAN控制器状态的数据类型。
        public struct VCI_CAN_STATUS
        {
            public byte ErrInterrupt;
            public byte regMode;
            public byte regStatus;
            public byte regALCapture;
            public byte regECCapture;
            public byte regEWLimit;
            public byte regRECounter;
            public byte regTECounter;
            public uint Reserved;
        }

        //4.定义错误信息的数据类型。
        public struct VCI_ERR_INFO
        {
            public uint ErrCode;
            public byte Passive_ErrData1;
            public byte Passive_ErrData2;
            public byte Passive_ErrData3;
            public byte ArLost_ErrData;
        }

        //5.定义初始化CAN的数据类型
        public struct VCI_INIT_CONFIG
        {
            public UInt32 AccCode;
            public UInt32 AccMask;
            public UInt32 Reserved;
            public byte Filter;   //1接收所有帧。2标准帧滤波，3是扩展帧滤波。
            public byte Timing0;
            public byte Timing1;
            public byte Mode;     //模式，0表示正常模式，1表示只听模式,2自测模式
        }

        /*------------其他数据结构描述---------------------------------*/
        //6.USB-CAN总线适配器板卡信息的数据类型1，该类型为VCI_FindUsbDevice函数的返回参数。
        public struct VCI_BOARD_INFO1
        {
            public UInt16 hw_Version;
            public UInt16 fw_Version;
            public UInt16 dr_Version;
            public UInt16 in_Version;
            public UInt16 irq_Num;
            public byte can_Num;
            public byte Reserved;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 8)]
            public byte[] str_Serial_Num;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
            public byte[] str_hw_Type;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
            public byte[][] str_Usb_Serial;
        }

        //7.定义常规参数类型
        public struct VCI_REF_NORMAL
        {
            public byte Mode;     //模式，0表示正常模式，1表示只听模式,2自测模式
            public byte Filter;   //1接收所有帧。2标准帧滤波，3是扩展帧滤波。
            public UInt32 AccCode;//接收滤波验收码
            public UInt32 AccMask;//接收滤波屏蔽码
            public byte kBaudRate;//波特率索引号，0-SelfDefine,1-5Kbps(未用),2-18依次为：10kbps,20kbps,40kbps,50kbps,80kbps,100kbps,125kbps,200kbps,250kbps,400kbps,500kbps,666kbps,800kbps,1000kbps,33.33kbps,66.66kbps,83.33kbps
            public byte Timing0;
            public byte Timing1;
            public byte CANRX_EN;//保留，未用
            public byte UARTBAUD;//保留，未用
        }

        //8.定义波特率设置参数类型
        public struct VCI_BAUD_TYPE
        {
            public UInt32 Baud;				//存储波特率实际值
            public byte SJW;				//同步跳转宽度，取值1-4
            public byte BRP;				//预分频值，取值1-64
            public byte SAM;				//采样点，取值0=采样一次，1=采样三次
            public byte PHSEG2_SEL;		    //相位缓冲段2选择位，取值0=由相位缓冲段1时间决定,1=可编程
            public byte PRSEG;				//传播时间段，取值1-8
            public byte PHSEG1;			    //相位缓冲段1，取值1-8
            public byte PHSEG2;			    //相位缓冲段2，取值1-8

        }

        //9.定义Reference参数类型
        public struct VCI_REF_STRUCT
        {
            public VCI_REF_NORMAL RefNormal;
            public byte Reserved;
            public VCI_BAUD_TYPE BaudType;
        }

        public struct DataSendStruct
        {
            //public 
        }

        public struct CanDataFrame
        {
            public string frameType;
            public string frameCmd;
            public string frameID;
            public string frameData;
        }
        public struct Can_Device_Pram
        {
            public UInt32 DeviceType;
            public UInt32 DeviceID;
            public UInt32 CanIdex;
            public bool OpenFlag;
            public VCI_CAN_OBJ[] RevData;//= new VCI_CAN_OBJ[3000];
            public CanDataFrame[] strRevData;//= new CanDataFrame[3000];
            public VCI_CAN_OBJ SendData;//=new VCI_CAN_OBJ();
            public VCI_INIT_CONFIG myCanConfig;// = new VCI_INIT_CONFIG();
            public UInt32 RevDataCount;
            public bool DataUserFlag;

        }
        public static Can_Device_Pram BmsCan;
        public static Can_Device_Pram CdjCan;
        public static Can_Device_Pram VcuCan;

        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_OpenDevice(UInt32 DeviceType, UInt32 DeviceInd, UInt32 Reserved);
        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_CloseDevice(UInt32 DeviceType, UInt32 DeviceInd);
        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_InitCAN(UInt32 DeviceType, UInt32 DeviceInd, UInt32 CANInd, ref VCI_INIT_CONFIG pInitConfig);

        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_ReadBoardInfo(UInt32 DeviceType, UInt32 DeviceInd, ref VCI_BOARD_INFO pInfo);
        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_ReadErrInfo(UInt32 DeviceType, UInt32 DeviceInd, UInt32 CANInd, ref VCI_ERR_INFO pErrInfo);
        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_ReadCANStatus(UInt32 DeviceType, UInt32 DeviceInd, UInt32 CANInd, ref VCI_CAN_STATUS pCANStatus);

        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_GetReference(UInt32 DeviceType, UInt32 DeviceInd, UInt32 CANInd, UInt32 RefType, ref byte pData);
        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_SetReference(UInt32 DeviceType, UInt32 DeviceInd, UInt32 CANInd, UInt32 RefType, ref byte pData);

        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_GetReceiveNum(UInt32 DeviceType, UInt32 DeviceInd, UInt32 CANInd);
        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_ClearBuffer(UInt32 DeviceType, UInt32 DeviceInd, UInt32 CANInd);

        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_StartCAN(UInt32 DeviceType, UInt32 DeviceInd, UInt32 CANInd);
        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_ResetCAN(UInt32 DeviceType, UInt32 DeviceInd, UInt32 CANInd);

        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_Transmit(UInt32 DeviceType, UInt32 DeviceInd, UInt32 CANInd, ref VCI_CAN_OBJ pSend, UInt32 Len);

        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_Receive(UInt32 DeviceType, UInt32 DeviceInd, UInt32 CANInd, ref VCI_CAN_OBJ pReceive, UInt32 Len, Int32 WaitTime);

        // [DllImport("controlcan.dll", CharSet = CharSet.Ansi)]
        //static extern UInt32 VCI_Receive(UInt32 DeviceType, UInt32 DeviceInd, UInt32 CANInd, IntPtr pReceive, UInt32 Len, Int32 WaitTime);

        /*------------其他函数描述---------------------------------*/
        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_GetReference2(UInt32 DevType, UInt32 DevIndex, UInt32 CANIndex, UInt32 Reserved, ref VCI_REF_STRUCT pRefStruct);
        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_SetReference2(UInt32 DevType, UInt32 DevIndex, UInt32 CANIndex, UInt32 RefType, ref byte pData);
        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_ResumeConfig(UInt32 DevType, UInt32 DevIndex, UInt32 CANIndex);

        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_ConnectDevice(UInt32 DevType, UInt32 DevIndex);
        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_UsbDeviceReset(UInt32 DevType, UInt32 DevIndex, UInt32 Reserved);
        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_FindUsbDevice(ref VCI_BOARD_INFO1 pInfo);
        /*------------函数描述结束---------------------------------*/
        public static Can_Device_Pram CanCommInit(Can_Device_Pram Can)
        {
            Can = new Can_Device_Pram();
            Can.RevData = new VCI_CAN_OBJ[3000];
            Can.strRevData = new CanDataFrame[3000];
            Can.SendData = new VCI_CAN_OBJ();
            Can.myCanConfig = new VCI_INIT_CONFIG();
            return Can;

        }
        public static bool CanOpen(string CanName,UInt32 DeviceType, UInt32 DeviceID, UInt32 CanIndex, string config)
        {
#if true
            
            UInt32 Reserve = 0;
            UInt32 State = 0;
            int error = 1;
            try
            {

                switch (CanName)
                {
                    case "BMS":
                        switch (config.Substring(0, 7))
                        {
                            case "125kbps":
                                CanComm.BmsCan.myCanConfig.Timing0 = 0x03;
                                CanComm.BmsCan.myCanConfig.Timing1 = 0x1C;
                                break;
                            case "250kbps":
                                CanComm.BmsCan.myCanConfig.Timing0 = 0x01;
                                CanComm.BmsCan.myCanConfig.Timing1 = 0x1C;
                                break;
                            case "500kbps":
                                CanComm.BmsCan.myCanConfig.Timing0 = 0x00;
                                CanComm.BmsCan.myCanConfig.Timing1 = 0x1C;
                                break;

                        }
                        CanComm.BmsCan.myCanConfig.AccCode = Convert.ToUInt32(config.Substring(7, 10), 16);
                        CanComm.BmsCan.myCanConfig.AccMask = Convert.ToUInt32(config.Substring(17, 10), 16);
                        CanComm.BmsCan.myCanConfig.Filter = 1;
                        CanComm.BmsCan.DeviceType = DeviceType;
                        CanComm.BmsCan.DeviceID = DeviceID;  //设备索引
                        CanComm.BmsCan.CanIdex = CanIndex;   //can通道

                        if (CanComm.BmsCan.OpenFlag)
                        {//如果Can是打开的，则关闭 
                            CanClose("BMS",CanComm.BmsCan.DeviceType, CanComm.BmsCan.DeviceID, Reserve);
                        }
                        if (VCI_OpenDevice(CanComm.BmsCan.DeviceType, CanComm.BmsCan.DeviceID, Reserve) == 0)
                        {
                            return false;
                        }
                        State = VCI_InitCAN(CanComm.BmsCan.DeviceType, CanComm.BmsCan.DeviceID, CanComm.BmsCan.CanIdex, ref CanComm.BmsCan.myCanConfig);


                        State = VCI_StartCAN(CanComm.BmsCan.DeviceType, CanComm.BmsCan.DeviceID, CanComm.BmsCan.CanIdex);

                        State = VCI_ClearBuffer(CanComm.BmsCan.DeviceType, CanComm.BmsCan.DeviceID, CanComm.BmsCan.CanIdex);
                        CanComm.BmsCan.DataUserFlag = true;
                        CanComm.BmsCan.OpenFlag = true;
                        break;
                    case "充电机":
                        switch (config.Substring(0, 7))
                        {
                            case "125kbps":
                                CanComm.CdjCan.myCanConfig.Timing0 = 0x03;
                                CanComm.CdjCan.myCanConfig.Timing1 = 0x1C;
                                break;
                            case "250kbps":
                                CanComm.CdjCan.myCanConfig.Timing0 = 0x01;
                                CanComm.CdjCan.myCanConfig.Timing1 = 0x1C;
                                break;
                            case "500kbps":
                                CanComm.CdjCan.myCanConfig.Timing0 = 0x00;
                                CanComm.CdjCan.myCanConfig.Timing1 = 0x1C;
                                break;

                        }
                        CanComm.CdjCan.myCanConfig.AccCode = Convert.ToUInt32(config.Substring(7, 10), 16);
                        CanComm.CdjCan.myCanConfig.AccMask = Convert.ToUInt32(config.Substring(17, 10), 16);
                        CanComm.CdjCan.myCanConfig.Filter = 1;
                        CanComm.CdjCan.DeviceType = DeviceType;
                        CanComm.CdjCan.DeviceID = DeviceID;
                        CanComm.CdjCan.CanIdex = CanIndex;


                        if (CanComm.CdjCan.OpenFlag)
                        {//如果Can是打开的，则关闭 
                            CanClose("充电机",CanComm.CdjCan.DeviceType, CanComm.CdjCan.DeviceID, Reserve);
                        }
                        if (VCI_OpenDevice(CanComm.CdjCan.DeviceType, CanComm.CdjCan.DeviceID, Reserve) == 0)
                        {
                            return false;
                        }
                        State = VCI_InitCAN(CanComm.CdjCan.DeviceType, CanComm.CdjCan.DeviceID, CanComm.CdjCan.CanIdex, ref CanComm.CdjCan.myCanConfig);


                        State = VCI_StartCAN(CanComm.CdjCan.DeviceType, CanComm.CdjCan.DeviceID, CanComm.CdjCan.CanIdex);

                        State = VCI_ClearBuffer(CanComm.CdjCan.DeviceType, CanComm.CdjCan.DeviceID, CanComm.CdjCan.CanIdex);
                        CanComm.CdjCan.DataUserFlag = true;
                        CanComm.CdjCan.OpenFlag = true;
                        break;
                    case "VCU":
                        switch (config.Substring(0, 7))
                        {
                            case "125kbps":
                                CanComm.VcuCan.myCanConfig.Timing0 = 0x03;
                                CanComm.VcuCan.myCanConfig.Timing1 = 0x1C;
                                break;
                            case "250kbps":
                                CanComm.VcuCan.myCanConfig.Timing0 = 0x01;
                                CanComm.VcuCan.myCanConfig.Timing1 = 0x1C;
                                break;
                            case "500kbps":
                                CanComm.VcuCan.myCanConfig.Timing0 = 0x00;
                                CanComm.VcuCan.myCanConfig.Timing1 = 0x1C;

                                break;

                        }
                        CanComm.VcuCan.myCanConfig.AccCode = Convert.ToUInt32(config.Substring(7, 10), 16);
                        CanComm.VcuCan.myCanConfig.AccMask = Convert.ToUInt32(config.Substring(17, 10), 16);
                        CanComm.VcuCan.myCanConfig.Filter = 1;
                        CanComm.VcuCan.DeviceType = DeviceType;
                        CanComm.VcuCan.DeviceID = DeviceID;
                        CanComm.VcuCan.CanIdex = CanIndex;


                        if (CanComm.VcuCan.OpenFlag)
                        {//如果Can是打开的，则关闭 
                            CanClose("VCU",CanComm.VcuCan.DeviceType, CanComm.VcuCan.DeviceID, Reserve);
                        }
                        if (VCI_OpenDevice(CanComm.VcuCan.DeviceType, CanComm.VcuCan.DeviceID, Reserve) == 0)
                        {
                            return false;
                        }
                        State = VCI_InitCAN(CanComm.VcuCan.DeviceType, CanComm.VcuCan.DeviceID, CanComm.VcuCan.CanIdex, ref CanComm.VcuCan.myCanConfig);


                        State = VCI_StartCAN(CanComm.VcuCan.DeviceType, CanComm.VcuCan.DeviceID, CanComm.VcuCan.CanIdex);

                        State = VCI_ClearBuffer(CanComm.VcuCan.DeviceType, CanComm.VcuCan.DeviceID, CanComm.VcuCan.CanIdex);
                        CanComm.VcuCan.DataUserFlag = true;
                        CanComm.VcuCan.OpenFlag = true;
                        break;
                }
                return true;
            }
            catch (Exception ex)
            {
                return false;
            }
            
            
#else
           
#endif 
            
        }
        public static bool CanInitBuffer(string CanName)
        {
            switch (CanName)
            { 
                case "BMS":
                    VCI_ClearBuffer(BmsCan.DeviceType, BmsCan.DeviceID, BmsCan.CanIdex);
                    BmsCan.RevDataCount = 0;
                    BmsCan.DataUserFlag = true;
                    break;
                case "充电机":
                    VCI_ClearBuffer(CdjCan.DeviceType, CdjCan.DeviceID, CdjCan.CanIdex);
                    CdjCan.RevDataCount = 0;
                    CdjCan.DataUserFlag = true;
                    break;
                case "VCU":
                    VCI_ClearBuffer(VcuCan.DeviceType, VcuCan.DeviceID, VcuCan.CanIdex);
                    VcuCan.RevDataCount = 0;
                    VcuCan.DataUserFlag = true;
                    break;
            }
            
           
            return true;

        }
        public static bool CanClose(string CanName, UInt32 DeviceType, UInt32 DeviceID, UInt32 CanIndex)
        {

            try
            {
                if (VCI_CloseDevice(DeviceType, DeviceID) == 1)
                {
                    switch (CanName)
                    { 
                        case"BMS":
                            CanComm.BmsCan.OpenFlag = false;
                            break;
                        case "充电机":
                            CanComm.CdjCan.OpenFlag = false;
                            break;
                        case "VCU":
                            CanComm.VcuCan.OpenFlag = false;
                            break;
                    }
                    return true;
                }
                return false;

                
            }
            catch (Exception error)
            { 
                
            }
            return false;
        }
        unsafe public static bool CanSend(string CanName,string data,string frameType,string frameCmd,string frameID)
        {
            VCI_CAN_OBJ mySendObj = new VCI_CAN_OBJ();
            int dLen = 0;
            byte bframeCmd = 0;
            byte bframeType = 0;
           // if (BmsCan.OpenFlag)
          
                switch (frameCmd)
                { 
                    case "数据帧":
                        bframeCmd = 0;
                        break;
                    case "远程帧":
                        bframeCmd = 1;
                        break;
                }
                switch (frameType)
                { 
                    case "标准帧":
                        bframeType = 0;
                        break;
                    case "扩展帧":
                        bframeType = 1;
                        break;
                }
                mySendObj.RemoteFlag = bframeCmd;//Convert.ToByte(frameCmd);
                mySendObj.ExternFlag = bframeType;// Convert.ToByte(frameType);
                mySendObj.ID = Convert.ToUInt32("0x"+frameID,16);
                mySendObj.DataLen = Convert.ToByte(data.Length/2);
                for (dLen = 0; dLen < data.Length / 2; dLen++)
                {
                    mySendObj.Data[dLen] = Convert.ToByte("0x"+data.Substring(dLen*2, 2),16);
                    //dLen += 2;
                }
                switch (CanName)
                {
                    case "BMS":
                        if (BmsCan.OpenFlag)
                        {
                            VCI_Transmit(BmsCan.DeviceType, BmsCan.DeviceID, BmsCan.CanIdex, ref mySendObj, 1);
                            if ((MainForm.myCanComm.FormOpen == true))
                            { 
                                MainForm.myCanComm.Display(data, frameType, frameCmd, frameID, false);
                            }
                        }
                        else
                        {
                            return false;
                        }
                        break;
                    case "充电机":
                        if (CdjCan.OpenFlag)
                        {
                            VCI_Transmit(CdjCan.DeviceType, CdjCan.DeviceID, CdjCan.CanIdex, ref mySendObj, 1);
                            if ((MainForm.myCanComm.FormOpen == true))
                            {
                                MainForm.myCanComm.Display(data, frameType, frameCmd, frameID, false);
                            }
                        }
                        else
                        {
                            return false;
                        }
                        break;
                    case "VCU":
                        if (VcuCan.OpenFlag)
                        {
                            VCI_Transmit(VcuCan.DeviceType, VcuCan.DeviceID, VcuCan.CanIdex, ref mySendObj, 1);
                        }
                        else
                        {
                            return false;
                        }
                        break;
                } 
                return true;
         
        }
        unsafe public static void CanRevData(string CanName)
        {
            UInt32 frameCount = 0;
            UInt32 frameIndex = 0;
            UInt32 lastFrameIndex = 0;
            CanCommunication.DataSendPram RevData = new CanCommunication.DataSendPram();
            try
            {
                switch (CanName)
                { 
                    case "BMS":
                        if (BmsCan.OpenFlag)
                        {
                            if (BmsCan.DataUserFlag == true)
                            {
                                BmsCan.DataUserFlag = false;
                                lastFrameIndex = 0;
                                BmsCan.RevDataCount = lastFrameIndex;

                            }
                            else
                            {//接上次存储
                                lastFrameIndex = BmsCan.RevDataCount;

                            }//数据已经使用过重新存，否则接上次存
                            frameCount = VCI_Receive(BmsCan.DeviceType, BmsCan.DeviceID, BmsCan.CanIdex, ref BmsCan.RevData[lastFrameIndex], 3000 - lastFrameIndex, 100);

                            if (frameCount > 0 && (frameCount < 3000))
                            {
                                BmsCan.RevDataCount += frameCount;
                                for (UInt32 i = 0; i < frameCount; i++)
                                {
                                    if (BmsCan.RevData[i + lastFrameIndex].RemoteFlag == 0)
                                    {
                                        RevData.frameCmd = "数据帧";
                                    }
                                    else
                                    {
                                        RevData.frameCmd = "远程帧";
                                    }
                                    if (BmsCan.RevData[i + lastFrameIndex].ExternFlag == 0)
                                    {
                                        RevData.frameType = "标准帧";
                                    }
                                    else
                                    {
                                        RevData.frameType = "扩展帧";
                                    }
                                    RevData.frameID = Convert.ToString(BmsCan.RevData[i + lastFrameIndex].ID, 16).ToUpper();
                                    RevData.frameData = "";
                                    fixed (VCI_CAN_OBJ* tmp = &BmsCan.RevData[i + lastFrameIndex])
                                    {
                                        for (byte j = 0; j < BmsCan.RevData[i + lastFrameIndex].DataLen; j++)
                                        {
                                            RevData.frameData += tmp->Data[j].ToString("X2");//Convert.ToString(tmp->Data[j]);
                                        }
                                    }
                                    RevData.frameData.ToUpper();
                                    BmsCan.strRevData[i + lastFrameIndex].frameData = RevData.frameData;
                                    BmsCan.strRevData[i + lastFrameIndex].frameType = RevData.frameType;
                                    BmsCan.strRevData[i + lastFrameIndex].frameCmd = RevData.frameCmd;
                                    BmsCan.strRevData[i + lastFrameIndex].frameID = RevData.frameID;
                                    if ((MainForm.myCanComm.FormOpen == true))
                                    {
                                        MainForm.myCanComm.Display(RevData.frameData, RevData.frameType, RevData.frameCmd, RevData.frameID, true);

                                    }
                                }

                            }
                            else if (frameCount > 3000)
                            { //
                                //大于3000不处理
                                VCI_ClearBuffer(BmsCan.DeviceType, BmsCan.DeviceID, BmsCan.CanIdex);
                                lastFrameIndex = 0;
                                BmsCan.RevDataCount = lastFrameIndex;
                                BmsCan.DataUserFlag = true;
                            }
                        }
                        else
                        {
                        }
                        break;
                    case"充电机":
                        if (CdjCan.OpenFlag)
                        {
                            if (CdjCan.DataUserFlag == true)
                            {
                                CdjCan.DataUserFlag = false;
                                lastFrameIndex = 0;
                                CdjCan.RevDataCount = lastFrameIndex;

                            }
                            else
                            {//接上次存储
                                lastFrameIndex = CdjCan.RevDataCount;

                            }//数据已经使用过重新存，否则接上次存
                            frameCount = VCI_Receive(CdjCan.DeviceType, CdjCan.DeviceID, CdjCan.CanIdex, ref CdjCan.RevData[lastFrameIndex], 3000 - lastFrameIndex, 100);

                            if (frameCount > 0 && (frameCount < 3000))
                            {
                                CdjCan.RevDataCount += frameCount;
                                for (UInt32 i = 0; i < frameCount; i++)
                                {
                                    if (CdjCan.RevData[i + lastFrameIndex].RemoteFlag == 0)
                                    {
                                        RevData.frameCmd = "数据帧";
                                    }
                                    else
                                    {
                                        RevData.frameCmd = "远程帧";
                                    }
                                    if (CdjCan.RevData[i + lastFrameIndex].ExternFlag == 0)
                                    {
                                        RevData.frameType = "标准帧";
                                    }
                                    else
                                    {
                                        RevData.frameType = "扩展帧";
                                    }
                                    RevData.frameID = Convert.ToString(CdjCan.RevData[i + lastFrameIndex].ID, 16).ToUpper();
                                    RevData.frameData = "";
                                    fixed (VCI_CAN_OBJ* tmp = &CdjCan.RevData[i + lastFrameIndex])
                                    {
                                        for (byte j = 0; j < CdjCan.RevData[i + lastFrameIndex].DataLen; j++)
                                        {
                                            RevData.frameData += tmp->Data[j].ToString("X2");//Convert.ToString(tmp->Data[j]);
                                        }
                                    }
                                    RevData.frameData.ToUpper();
                                    CdjCan.strRevData[i + lastFrameIndex].frameData = RevData.frameData;
                                    CdjCan.strRevData[i + lastFrameIndex].frameType = RevData.frameType;
                                    CdjCan.strRevData[i + lastFrameIndex].frameCmd = RevData.frameCmd;
                                    CdjCan.strRevData[i + lastFrameIndex].frameID = RevData.frameID;
                                    //if (MainForm.mySystemInfo.DeviceType == "主机")
                                    //{
                                    //    string strDisplay = string.Empty;
                                    //    strDisplay = "接收<-" + "帧ID：" + RevData.frameID + "帧数据" + RevData.frameData;
                                    //    MainForm.myBusChongdian.DisplayCdjFrame(strDisplay);
                                    //}
                                    if ((MainForm.myCanComm.FormOpen == true))
                                    {
                                        MainForm.myCanComm.Display(RevData.frameData, RevData.frameType, RevData.frameCmd, RevData.frameID, true);

                                    }
                                }

                            }
                            else if (frameCount > 3000)
                            { //
                                //大于3000不处理
                                VCI_ClearBuffer(CdjCan.DeviceType, CdjCan.DeviceID, CdjCan.CanIdex);
                                lastFrameIndex = 0;
                                CdjCan.RevDataCount = lastFrameIndex;
                                CdjCan.DataUserFlag = true;
                            }
                        }
                        else
                        {
                        }
                        break;
                    case"VCU":
                        if (VcuCan.OpenFlag)
                        {
                            if (VcuCan.DataUserFlag == true)
                            {
                                VcuCan.DataUserFlag = false;
                                lastFrameIndex = 0;
                                VcuCan.RevDataCount = lastFrameIndex;

                            }
                            else
                            {//接上次存储
                                lastFrameIndex = VcuCan.RevDataCount;

                            }//数据已经使用过重新存，否则接上次存
                            frameCount = VCI_Receive(VcuCan.DeviceType, VcuCan.DeviceID, VcuCan.CanIdex, ref VcuCan.RevData[lastFrameIndex], 3000 - lastFrameIndex, 100);

                            if (frameCount > 0 && (frameCount < 3000))
                            {
                                VcuCan.RevDataCount += frameCount;
                                for (UInt32 i = 0; i < frameCount; i++)
                                {
                                    if (VcuCan.RevData[i + lastFrameIndex].RemoteFlag == 0)
                                    {
                                        RevData.frameCmd = "数据帧";
                                    }
                                    else
                                    {
                                        RevData.frameCmd = "远程帧";
                                    }
                                    if (VcuCan.RevData[i + lastFrameIndex].ExternFlag == 0)
                                    {
                                        RevData.frameType = "标准帧";
                                    }
                                    else
                                    {
                                        RevData.frameType = "扩展帧";
                                    }
                                    RevData.frameID = Convert.ToString(VcuCan.RevData[i + lastFrameIndex].ID, 16).ToUpper();
                                    RevData.frameData = "";
                                    fixed (VCI_CAN_OBJ* tmp = &CdjCan.RevData[i + lastFrameIndex])
                                    {
                                        for (byte j = 0; j < CdjCan.RevData[i + lastFrameIndex].DataLen; j++)
                                        {
                                            RevData.frameData += tmp->Data[j].ToString("X2");//Convert.ToString(tmp->Data[j]);
                                        }
                                    }
                                    RevData.frameData.ToUpper();
                                    VcuCan.strRevData[i + lastFrameIndex].frameData = RevData.frameData;
                                    VcuCan.strRevData[i + lastFrameIndex].frameType = RevData.frameType;
                                    VcuCan.strRevData[i + lastFrameIndex].frameCmd = RevData.frameCmd;
                                    VcuCan.strRevData[i + lastFrameIndex].frameID = RevData.frameID;
                                }

                            }
                            else if (frameCount > 3000)
                            { //
                                //大于3000不处理
                                VCI_ClearBuffer(VcuCan.DeviceType, VcuCan.DeviceID, VcuCan.CanIdex);
                                lastFrameIndex = 0;
                                VcuCan.RevDataCount = lastFrameIndex;
                                VcuCan.DataUserFlag = true;
                            }
                        }
                        else
                        {
                        }
                        break;
                }
                
            }
            catch (Exception error)
            { 
                
            }
        }
        
        
    }
}
