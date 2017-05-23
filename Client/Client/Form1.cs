using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Timers;
using System.IO.Ports;

namespace Client
{
    public partial class Form1 : Form
    {
        Sensor sensor;
        public Form1()
        {
            InitializeComponent();
            sensor = new Sensor(SP);
            sensor.Connect();
            button1.Enabled = sensor.Connected();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
        }


        private void OnTimedEvent(object sender, ElapsedEventArgs e)
        {
        }

        private void updateTextBox(string txt)
        {
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
        }

        private void button1_Click(object sender, EventArgs e)
        {
            SP.WriteLine("T" + (char)(DateTime.Now.Year - 2000)+ (char)DateTime.Now.Month+ (char)DateTime.Now.Day + (char)DateTime.Now.Hour + (char)DateTime.Now.Minute + (char)DateTime.Now.Second);
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (!sensor.Connected())
            {
                sensor.Connect();
                button1.Enabled = sensor.Connected();
            }
        }
    }
}
