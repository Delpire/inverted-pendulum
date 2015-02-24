using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace EvaUI
{
    class LogFile
    {
        protected StreamWriter stream;

        protected string basePath;
        protected string fileName;
        protected string filePath;

        public LogFile(string basePath, string fileName)
        {
            this.fileName = fileName;
            UpdatePath(basePath);
        }

        public void Write(string line)
        {
            if (stream == null)
            {
                stream = new StreamWriter(filePath);
            }

            stream.WriteLine(line);
            stream.Flush();
        }

        public void UpdatePath(string basePath)
        {
            this.basePath = basePath;
            this.filePath = Path.Combine(basePath, fileName);

            // reset stream so it gets recreated next write
            stream = null;
        }
    }
}
