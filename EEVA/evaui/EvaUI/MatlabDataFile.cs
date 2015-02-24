using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using csmatio;
using csmatio.io;
using csmatio.types;
using csmatio.common;

namespace EvaUI
{
    public class MatlabDataFile
    {
        public string FilePath;

        public MatlabDataFile(string filePath) 
        {
            this.FilePath = filePath;
        }

        public void Write(List<float[]> dataArrays, List<string> dataNames)
        {
            if (dataArrays.Count < 1)
            {
                throw new ArgumentException("Data must contain at least one array");
            }

            if (dataArrays.Count != dataNames.Count)
            {
                throw new ArgumentException("Data arrays and names not equal.");
            }

            List<MLArray> mlList = new List<MLArray>();

            // convert float array to double array since that's what this API supports
            // pre-allocate a single array to be refilled.
            double[] doubleArray = new double[dataArrays[0].Length];

            for (int i = 0; i < dataArrays.Count; ++i)
            {
                for (int e = 0; e < dataArrays[i].Length; ++e)
                {
                    doubleArray[e] = (double)dataArrays[i][e];
                }

                mlList.Add(new MLDouble(dataNames[i], doubleArray, 1));
            }
                        
            MatFileWriter mfw = new MatFileWriter(FilePath, mlList, compress: false);
        }
    }
}
