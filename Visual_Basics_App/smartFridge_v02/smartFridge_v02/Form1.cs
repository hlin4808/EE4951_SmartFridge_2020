using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace smartFridge_v02
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            if (!serialPort1.IsOpen)
            {
                tbMessages.Text = "nothing located in port";
                serialPort1.Open();
                tbMessages.Text = "port opened";
            }
            else
            {
                tbMessages.Text = "port is busy rn";
            }

        }

        private string rxString;
        private void serialPort1_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            rxString = serialPort1.ReadExisting();
            this.Invoke(new EventHandler(displayText));
        }

        private void displayText(object o, EventArgs e)
        {
            tbMessages.Clear();
            tbMessages.AppendText(rxString);
        }

        private void buttonAskForItem_Click(object sender, EventArgs e)
        {
            serialPort1.Write("R");
            serialPort1.Write(tbAskForItem.Text);
            tbAskForItem.Clear();
            serialPort1.Write("@");
        }

        private void buttonPutIn_Click(object sender, EventArgs e)
        {
            serialPort1.Write("E");
            serialPort1.Write(tbPutIn.Text);
            tbPutIn.Clear();
            serialPort1.Write("@");
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            serialPort1.Close();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void viewContents_Click(object sender, EventArgs e)
        {
            serialPort1.Write("V@");
        }

        private void expDates_Click(object sender, EventArgs e)
        {
            serialPort1.Write("D@");
        }


        private void bShowCal_Click(object sender, EventArgs e)
        {
            Form calForm = new Form();
            MonthCalendar mCal = new MonthCalendar();

            calForm.Controls.Add(mCal);

            // Panel panelCal = new Panel();

            // panelCal.Controls.Add(mCal);
            //mCal.Visible = true;
            calForm.ShowDialog();
            tbMessages.Clear();
            tbMessages.AppendText(mCal.SelectionStart.ToString("yyyyMMdd"));    //Shows date in message box
            serialPort1.Write("Z");
            serialPort1.Write(mCal.SelectionStart.ToString("yyyyMMdd"));
            calForm.Close();
            
        }
    }
}
