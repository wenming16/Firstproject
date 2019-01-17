using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using BMSTool.Analysiss;
 namespace BMSTool.OtherClass
{
    class AddItem
    {
        public static List<Analysiser.CsvUnitFormat> CsvUnit(List<Analysiser.CsvUnitFormat> csvUnits, Analysiser.CsvUnitFormat csvUnit)
        {
            if (csvUnits.Count == 0)
            {
                csvUnits.Add(csvUnit);
            }
            else
            {
                bool flag = true;
                for (int i = 0; i < csvUnits.Count; i++)
                {
                    if (csvUnits[i].Name == csvUnit.Name)
                    {
                    Analysiser.CsvUnitFormat temCsvUnit = csvUnit;
                        csvUnits[i] = temCsvUnit;
                        flag = false;

                    }
                    else
                    {
                        // flag= true;
                    }
                }
                if (flag)
                {
                    csvUnits.Add(csvUnit);
                }
            }
            return csvUnits;
        }
    }
}
