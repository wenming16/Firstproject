using BMSTool.Comm;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BMSTool
{
    class Director
    {
        
        public Director(CanComm _canComm,MainForm  m)

        {
            canComm = _canComm;
        }
        private CanComm canComm;
        //时钟驱动
            //接收原始数据
            //处理原始数据
        //为保存数据提供接口
    }
}
