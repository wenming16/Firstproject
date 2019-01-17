using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms.DataVisualization.Charting;

namespace BMSTool.OtherClass
{
    class ChartTool
    {
        public static void Initial(Chart chart ,int Length, string DisplayType)
        {

            chart.Series[0].Points.Clear();
            chart.Legends.Clear();
            if (DisplayType == "柱状图")
            {
                chart.Series[0].ChartType = SeriesChartType.Column;
            }
            else
            {
                chart.Series[0].ChartType = SeriesChartType.Line;
            }
            for (int i = 0; i < Length; i++)
            {
                chart.Series[0].Points.Add(0);
            }
            chart.Series[0].ShadowOffset = 1;
            
            chart.ChartAreas[0].Axes[0].MajorGrid.Enabled = false;
            chart.Series[0]["PointWidth"] = "0.4";
            chart.Series[0].IsValueShownAsLabel = true;
          

        }

        public static void DisplayDataToChart(Chart chart, string[] data1, string displyType)//(chart对象，显示的数据，图片显示类型，数据个数，最大值，最小值)
        {
            
            
            string[] data = new string[data1.Length];//只给数据，不给引用，避免影响形参
            for (int i = 0; i < data1.Length; i++)//函数内操作的数组和形参传进来的数组，地址不是一个
            {
                data[i] = data1[i];
            }
            //因为数组也是引用变量，我们只改变了元素的值，如果两个引用变量存着相同的引用，那样就会一起变化
           
            //如果传入数据长度为0，那么使其数据长度默认为12以方便显示
            int dataLen = data.Length;
            if (dataLen > 0)
            {
                chart.Series[0].Points.Clear();
                chart.Legends.Clear();
            }
            else { }
            string StrMax = Compute.MaxStr(data);
            string StrMin = Compute.MinStr(data);
            for (int i = 0; i < dataLen; i++)
            {
                if (data[i] == null)
                {
                    data[i] = "00";
                }
                else{ }
                chart.Series[0].Points.Add(Convert.ToDouble(data[i]));
                if (data[i] == StrMax)
                {
                    chart.Series[0].Points[i].Color = Color.Red;
                }
                else { }
                if (data[i] == StrMin)
                {
                    chart.Series[0].Points[i].Color = Color.Green;
                }
                else { }

            }

            chart.Legends.Clear();
           
        }
    }
}
