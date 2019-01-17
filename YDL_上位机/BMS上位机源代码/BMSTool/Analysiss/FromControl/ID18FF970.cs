using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using BMSTool.DataPacks;
using BMSTool.OtherClass;
using static BMSTool.Analysiss.Analysiser;

namespace BMSTool.Analysiss
{
    class ID18FF970
    {
        struct volChip
        {
            public int index;
            public int num;
        }
        public  List<Analysiser.CsvUnitFormat> Analysis( int modNum, string FrameData)
        {
          List<Analysiser.CsvUnitFormat> CsvUnits = new List<Analysiser.CsvUnitFormat>();
            Analysiser.CsvUnitFormat CsvUnit = new Analysiser.CsvUnitFormat();
            int[] arr = ArrConvert.twoToOne(modNum, Analysiser.dataPack.batDistributeNums);
            bool flag = false;
            for (int i = 0; i < arr.Length; i++)
            {
                if (arr[i] != 0)
                {
                    flag = true;
                }
            }
            if (!flag)//如果为空就不解析
            {
                return CsvUnits;
            }
            
            DataPack.FromVol volInfo= Analysiser.dataPack.fromVols[modNum];
            //string str1 = FrameData.Substring(0, 2);
            //int Num_6804 = Convert.ToInt32(str1, 16);

            string str2 = FrameData.Substring(2, 2);
            int CycNum = Convert.ToInt32(str2, 16);
            for (int i = 0; i < 3; i++)
            {
                int start = (i + 1) * 4;
                string Value = FrameData.Substring(start, 4);
                if (Value.Contains("FF"))
                {
                    return CsvUnits;
                }
                Value = Compute.str16to10With(Value,0.0001,0);
                int index = CycNum*3 +i;
                //输入编号和分割数目得到相应的位置
                volChip chip = acquire(index, arr);
                int index_6804 = chip.index;
                int num_6804 = chip.num;


                volInfo.Values[index_6804, num_6804] = Value;



                CsvUnit.Name =modNum+"号从控板"+index_6804 + "号6804" + num_6804 + "号单体电压";
                CsvUnit.Value = Value;
                AddItem.CsvUnit(CsvUnits, CsvUnit);

            }
            Analysiser.dataPack.fromVols[modNum] = volInfo;
            return CsvUnits;
        }
        private volChip acquire(int index,int []arrs)
        {
            volChip chip = new volChip();
                int nodeNum = 0;
                for (int i = 0; i < arrs.Length; i++)
                {
                nodeNum += arrs[i];
                if (index <= nodeNum-1)
                {
                    chip.index = i;
                    chip.num = index-nodeNum+arrs[i];
                    return chip;
                }

                }
            return chip;
          
        }


    }
}
