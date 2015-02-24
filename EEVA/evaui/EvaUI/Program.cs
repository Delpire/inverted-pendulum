using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace EvaUI
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            // These two lines must be ran first.
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            Application.ThreadException += new System.Threading.ThreadExceptionEventHandler(Application_ThreadException);

            // Configure system.
            MainForm mainForm = new MainForm();
            MainController mainController = new MainController(mainForm);
            ((IMainView)mainForm).setController(mainController);

            Application.Run(mainForm);
        }

        // Show custom dialog to user to tell them to email someone who can fix the problem.
        static void Application_ThreadException(object sender, System.Threading.ThreadExceptionEventArgs e)
        { 
            MessageBox.Show(String.Format("An unhandled exception has been thrown :(\n\nIf you would like to help" +
                " get this issue fixed please email this exception message to:\n\n{0}\n\n" + 
                "(To copy text just hit Ctrl+C, if you don't want to overwrite your clipboard you can use " +
                "Windows Snipping Tool)\n\n---------------------------\n\n{1}",
                "kmcgahee@ksu.edu", e.Exception.ToString()),
                "Unhandled Exception", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);

            Application.Exit();
        }
    }
}
