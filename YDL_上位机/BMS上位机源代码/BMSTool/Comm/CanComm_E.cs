using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Threading;
using BMSTool.Comm;
using BMSTool;

namespace BMSTool.Comm
{

    public class CanComm_E : CanComm
    {
        //1.ZLGCAN系列接口卡信息的数据类型。
        struct VCI_BOARD_INFO//结构体包含USB-CAN系列接口卡的设备信息
        {//结构体将在VCI_ReadBoardInfo函数中被填充
            public UInt16 hw_Version;//硬件版本号
            public UInt16 fw_Version;//固件版本号
            public UInt16 dr_Version;//驱动程序版本号
            public UInt16 in_Version;//接口库版本号
            public UInt16 irq_Num;//保留参数
            public byte can_Num;//表示有几路CAN通道
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 20)]
            public byte[] str_Serial_Num;//此板卡的序列号
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 40)]
            public byte[] str_hw_Type;//硬件类型
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
            public byte[] Reserved;//系统保留
        }

        /////////////////////////////////////////////////////
        //2.定义CAN信息帧的数据类型。//使用不安全代码
        unsafe struct VCI_CAN_OBJ//CAN帧结构体，即一个结构表示一个帧的数据结构
        {
            public uint ID;               //帧ID
            public uint TimeStamp;        //时间标识
            public byte TimeFlag;         //是否使用时间标识
            public byte SendType;         //发送标志。保留，未用
            public byte RemoteFlag;       //是否是远程帧
            public byte ExternFlag;       //是否是扩展帧
            public byte DataLen;          //数据长度
            public fixed byte Data[8];    //CAN帧数据
            public fixed byte Reserved[3];//系统保留

        }

        //3.定义CAN控制器状态的数据类型。
        struct VCI_CAN_STATUS
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
        struct VCI_ERR_INFO
        {
            public uint ErrCode;
            public byte Passive_ErrData1;
            public byte Passive_ErrData2;
            public byte Passive_ErrData3;
            public byte ArLost_ErrData;
        }

        //5.定义初始化CAN的数据类型
        struct VCI_INIT_CONFIG
        {
            public UInt32 AccCode;//验收码
            public UInt32 AccMask;//屏蔽码
            public UInt32 Reserved;//保留
            public byte Filter;   //1接收所有帧。2标准帧滤波，3是扩展帧滤波。
            public byte Timing0;    //波特率定时器0
            public byte Timing1;    //波特率定时器
            public byte Mode;     //模式，0表示正常模式，1表示只听模式,2自测模式
        }

        /*------------其他数据结构描述---------------------------------*/
        //6.USB-CAN总线适配器板卡信息的数据类型1，该类型为VCI_FindUsbDevice函数的返回参数。
        struct VCI_BOARD_INFO1
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
        struct VCI_REF_NORMAL
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

        [DllImport("ControlCAN_E.dll")]//打开CAN设备函数。注意一个设备只能打开一次
        static extern UInt32 VCI_OpenDevice(UInt32 DeviceType, UInt32 DeviceInd, UInt32 Reserved);
        [DllImport("ControlCAN_E.dll")]//关闭VCI_Close函数
        static extern UInt32 VCI_CloseDevice(UInt32 DeviceType, UInt32 DeviceInd);
        [DllImport("ControlCAN_E.dll")]//初始化CAN设备
        static extern UInt32 VCI_InitCAN(UInt32 DeviceType, UInt32 DeviceInd, UInt32 CANInd, ref VCI_INIT_CONFIG pInitConfig);

        [DllImport("ControlCAN_E.dll")]//获取设备信息
        static extern UInt32 VCI_ReadBoardInfo(UInt32 DeviceType, UInt32 DeviceInd, ref VCI_BOARD_INFO pInfo);
        //[DllImport("ControlCAN_E.dll")]//读取错误信息
        //static extern UInt32 VCI_ReadErrInfo(UInt32 DeviceType, UInt32 DeviceInd, UInt32 CANInd, ref VCI_ERR_INFO pErrInfo);
        [DllImport("ControlCAN_E.dll")]//读取CAN状态
        static extern UInt32 VCI_ReadCANStatus(UInt32 DeviceType, UInt32 DeviceInd, UInt32 CANInd, ref VCI_CAN_STATUS pCANStatus);

        [DllImport("ControlCAN_E.dll")]//获取适配器内所有参数
        static extern UInt32 VCI_GetReference(UInt32 DeviceType, UInt32 DeviceInd, UInt32 CANInd, UInt32 RefType, ref byte pData);
        //[DllImport("ControlCAN_E.dll")]//设置适配器的相关参数
        //static extern UInt32 VCI_SetReference(UInt32 DeviceType, UInt32 DeviceInd, UInt32 CANInd, UInt32 RefType, ref byte pData);
        [DllImport("ControlCAN_E.dll")]
        //static extern UInt32 VCI_SetReference(UInt32 DeviceType, UInt32 DeviceInd, UInt32 CANInd, UInt32 RefType, ref byte pData);
        unsafe static extern UInt32 VCI_SetReference(UInt32 DeviceType, UInt32 DeviceInd, UInt32 CANInd, UInt32 RefType, byte* pData);

        [DllImport("ControlCAN_E.dll")]//CAN通道的接收缓冲区
        static extern UInt32 VCI_GetReceiveNum(UInt32 DeviceType, UInt32 DeviceInd, UInt32 CANInd);
        [DllImport("ControlCAN_E.dll")]//清空CAN通道的缓冲区
        static extern UInt32 VCI_ClearBuffer(UInt32 DeviceType, UInt32 DeviceInd, UInt32 CANInd);

        [DllImport("ControlCAN_E.dll")]//启动CAN卡的某一个CAN通道
        static extern UInt32 VCI_StartCAN(UInt32 DeviceType, UInt32 DeviceInd, UInt32 CANInd);
        [DllImport("ControlCAN_E.dll")]//复位CAN
        static extern UInt32 VCI_ResetCAN(UInt32 DeviceType, UInt32 DeviceInd, UInt32 CANInd);

        [DllImport("ControlCAN_E.dll")]//发送函数，返回实际发送的帧数
        static extern UInt32 VCI_Transmit(UInt32 DeviceType, UInt32 DeviceInd, UInt32 CANInd, ref VCI_CAN_OBJ pSend, UInt32 Len);

        [DllImport("ControlCAN_E.dll")]//接收函数，从指定的CAN通道接收缓冲区中读取数据
        static extern UInt32 VCI_Receive(UInt32 DeviceType, UInt32 DeviceInd, UInt32 CANInd, ref VCI_CAN_OBJ pReceive, UInt32 Len, Int32 WaitTime);

        // [DllImport("ControlCAN_E.dll", CharSet = CharSet.Ansi)]
        //static extern UInt32 VCI_Receive(UInt32 DeviceType, UInt32 DeviceInd, UInt32 CANInd, IntPtr pReceive, UInt32 Len, Int32 WaitTime);



        private CanDeviceInfo can = new CanDeviceInfo();
        public CanComm_E(CanDeviceInfo canDeviceInfo)
        {
            can.CanType = canDeviceInfo.CanType;
            can.DeviceIndex = canDeviceInfo.DeviceIndex;
            can.CanIndex = canDeviceInfo.CanIndex;
            can.Config = canDeviceInfo.Config;


        }
        private bool openState = false;
        public override bool isOpen()
        {
            return openState;
        }

        public override bool Close()
        {
            if (VCI_CloseDevice(can.CanType, can.DeviceIndex) == 1)
            {
                openState = false;
                return true;
            }
            else
            {
                return false;
            }


        }
        //CAN通信连接函数

        public unsafe override bool Open()
        {
            uint State = VCI_OpenDevice(can.CanType, can.DeviceIndex, 0);
            if (State == 0)//调用类库的函数//返回值为0是失败
            {
                return false;
            }
            else { }
            //设置波特率并初始化
            VCI_INIT_CONFIG myVCI_INIT_CONFIG = new VCI_INIT_CONFIG();
            string BaudRate = can.Config.Substring(0, 7);
            UInt32 baud=0; 
            switch (BaudRate)
            {
                case "125kbps":
                    myVCI_INIT_CONFIG.Timing0 = 0x03;
                    myVCI_INIT_CONFIG.Timing1 = 0x14;
                    baud = 0x001C0011;
                    break;
                case "250kbps":
                    myVCI_INIT_CONFIG.Timing0 = 0x01;
                    myVCI_INIT_CONFIG.Timing1 = 0x14;
                    baud = 0x001C0008;
                    break;
                case "500kbps":
                    myVCI_INIT_CONFIG.Timing0 = 0x00;
                    myVCI_INIT_CONFIG.Timing1 = 0x1C;
                    baud = 0x001C0007;
                    break;
            }
           
            myVCI_INIT_CONFIG.AccCode = Convert.ToUInt32(can.Config.Substring(7, 10), 16);
            myVCI_INIT_CONFIG.AccMask = Convert.ToUInt32(can.Config.Substring(17, 10), 16);
            myVCI_INIT_CONFIG.Filter = 1;

            if (VCI_SetReference(can.CanType, can.CanIndex, can.CanIndex, 0, (byte*)&baud) != 1)
            {
                MessageBox.Show("设置波特率错误，打开设备失败!", "错误", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return false;
            }

            VCI_InitCAN(can.CanType, can.DeviceIndex, can.CanIndex, ref myVCI_INIT_CONFIG);//调用类库初始化函数
            VCI_StartCAN(can.CanType, can.DeviceIndex, can.CanIndex);//开启Can通道
            State = VCI_ClearBuffer(can.CanType, can.DeviceIndex, can.CanIndex);//清空缓存
            this.openState = true;
            return true;
        }
        public override bool ClearBuffer()
        {
            UInt32 state = VCI_ClearBuffer(can.CanType, can.DeviceIndex, can.CanIndex);
            if (state == 1)
            {
                return true;
            }
            else { return false; }

        }
        unsafe public override bool Send(string data, string frameType, string frameCmd, string frameID)
        {
            VCI_CAN_OBJ mySendObj = new VCI_CAN_OBJ();
            switch (frameCmd)
            {
                case "数据帧":
                    mySendObj.RemoteFlag = 0;
                    break;
                case "远程帧":
                    mySendObj.RemoteFlag = 1;
                    break;
            }
            switch (frameType)
            {
                case "标准帧":
                    mySendObj.ExternFlag = 0;
                    break;
                case "扩展帧":
                    mySendObj.ExternFlag = 1;
                    break;
            }

            mySendObj.ID = Convert.ToUInt32("0x" + frameID, 16);
            mySendObj.DataLen = Convert.ToByte(data.Length / 2);
            for (int dLen = 0; dLen < data.Length / 2; dLen++)
            {
                mySendObj.Data[dLen] = Convert.ToByte("0x" + data.Substring(dLen * 2, 2), 16);
            }

            int SendCount = (int)VCI_Transmit(can.CanType, can.DeviceIndex, can.CanIndex, ref mySendObj, 1);
            if (SendCount == -1)
            {
                MessageBox.Show("发送失败");
                return false;
            }
            else
            {
                return true;
            }

        }
        public unsafe override CanDataFrame[] RevData()
        {
            VCI_CAN_OBJ[] myVCI_CAN_OBJ = new VCI_CAN_OBJ[2500];
            uint FrameCount = VCI_Receive(can.CanType, can.DeviceIndex, can.CanIndex, ref myVCI_CAN_OBJ[0], 2500, 0);
            CanDataFrame[] reCanDataFrame = null;
            if ((FrameCount < 3000) && (FrameCount > 0))
            {
                reCanDataFrame = new CanDataFrame[FrameCount];

                for (int i = 0; i < FrameCount; i++)
                {
                    if (myVCI_CAN_OBJ[i].RemoteFlag == 0)
                    {
                        reCanDataFrame[i].frameCmd = "数据帧";
                    }
                    else
                    {
                        reCanDataFrame[i].frameCmd = "远程帧";
                    }
                    if (myVCI_CAN_OBJ[i].ExternFlag == 0)
                    {
                        reCanDataFrame[i].frameType = "标准帧";
                    }
                    else
                    {
                        reCanDataFrame[i].frameType = "扩展帧";
                    }
                    reCanDataFrame[i].frameID = Convert.ToString(myVCI_CAN_OBJ[i].ID, 16).ToUpper();
                    fixed (VCI_CAN_OBJ* tmp = &myVCI_CAN_OBJ[i])
                    {
                        for (byte j = 0; j < myVCI_CAN_OBJ[i].DataLen; j++)
                        {
                            reCanDataFrame[i].frameData += tmp->Data[j].ToString("X2");
                        }
                    }
                    if (reCanDataFrame[i].frameData != null)
                    {
                        reCanDataFrame[i].frameData.ToUpper();
                    }
                    else
                    {
                    }
                }
            }

            return reCanDataFrame;
        }

    }
}


