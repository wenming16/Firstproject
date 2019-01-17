using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Threading;


namespace BMSTool.Comm
{

    public abstract class CanComm
    {

        public abstract bool isOpen();
       
        public abstract unsafe bool Open();
        public abstract bool Close();
        public abstract bool ClearBuffer();//CAN通信数据流函数
        public unsafe abstract bool Send(string data, string frameType, string frameCmd, string frameID);
        public unsafe abstract CanDataFrame[] RevData();
    }
}

