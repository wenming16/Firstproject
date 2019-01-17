using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BMSTool.OtherClass
{
    class ArrConvert
    {
        public static int[] twoToOne(int rowIndex,int[,]arr)
        {
            int length =  arr.GetLength(1);
            int[] nums = new int[length];
            for (int i = 0; i < nums.Length; i++)
            {
                nums[i] = arr[rowIndex, i];
            }
            return nums;
        }
    }
}
