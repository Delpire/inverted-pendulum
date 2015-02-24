using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace EvaUI
{
    class DataLogFile
    {
        public string FilePath { get; set; }

        public DataLogFile(string filePath)
        {
            this.FilePath = filePath;
        }

        public void Write(List<float[]> data, string[] columnNames)
        {
            using (StreamWriter stream = new StreamWriter(FilePath))
            {
                WriteColumnHeader(stream, columnNames);

                StringBuilder lineBuilder = new StringBuilder();
                foreach (float[] dataArray in data)
                {
                    for (int i = 0; i < dataArray.Length; ++i)
                    {
                        if (i > 0)
                        {
                            lineBuilder.Append(",");
                        }

                        lineBuilder.Append(dataArray[i]);
                    }

                    stream.WriteLine(lineBuilder.ToString());

                    lineBuilder.Clear();
                }
            }
        }

        private void WriteColumnHeader(StreamWriter stream, string[] columnNames)
        {
            StringBuilder lineBuilder = new StringBuilder();

            for (int i = 0; i < columnNames.Length; ++i)
            {
                if (i > 0)
                {
                    lineBuilder.Append(",");
                }

                lineBuilder.Append(columnNames[i]);
            }

            stream.WriteLine(lineBuilder.ToString());

            lineBuilder.Clear();
         
        }
    }
}
