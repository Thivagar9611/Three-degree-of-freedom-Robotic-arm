using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using System.IO;
namespace Robotics
{
    public partial class Form1 : Form
    {
        private SerialPort myport;
        private String data;
        private float j;
        private string in_data;
        private int a=0;
        private int b=0;
        private int c = 0;
        public Form1()
        {
            InitializeComponent();
            string[] ports = SerialPort.GetPortNames();

            foreach(string port in ports)
            {
                myport = new SerialPort();
                myport.BaudRate = 115200;
                myport.PortName = port;
                myport.Open();
                myport.DataReceived += receive;
                label7.Text = port;
            }
        }

        private void Label4_Click(object sender, EventArgs e)
        {

        }

        private void Label3_Click(object sender, EventArgs e)
        {

        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void Button1_Click(object sender, EventArgs e)
        {
            double d1 = 3, d2 = 20, d3 = 20;
            double x, y, z;
            double c3, s3;
            int theta1, theta2, theta3;
            if (myport.IsOpen)
            {
            }
            try
            {
                label8.Text = "X Y Z = 0 0 0 ";
                x = Convert.ToDouble(textBox1.Text);
                y = Convert.ToDouble(textBox2.Text);
                z = Convert.ToDouble(textBox3.Text);
                c3 = ((Math.Pow(x, 2) + Math.Pow(y, 2) + Math.Pow((z - d1), 2) - Math.Pow(d2, 2) - Math.Pow(d3, 2)) / (2 * d2 * d3));
                s3 = Math.Abs(Math.Sqrt(1 - Math.Pow(c3, 2)));

                theta1 = Convert.ToInt32((Math.Atan2(y, x)) * 57.2958);
                theta2 = Convert.ToInt32(((Math.Atan2((z - d1), Math.Sqrt(Math.Pow(x, 2) + Math.Pow(y, 2)))) + (Math.Atan2((d3 * s3), (d2 + d3 * c3)))) * 57.2958);
                theta3 = Convert.ToInt32((Math.Atan2(s3, c3)) * 57.2958);
                if (theta1 > 180)
                {
                    theta1 = 180;
                }
                if (theta1 < -180)
                {
                    theta1 = -180;
                }

                if (theta2 > 180)
                {
                    theta2 = 180;
                }
                if (theta2 < -180)
                {
                    theta2 = -180;
                }

                if (theta3 > 180)
                {
                    theta3 = 180;
                }
                if (theta3 < -180)
                {
                    theta3 = -180;
                }
            
               label8.Text = ("Angle: " + theta1 + "," + theta2 + "," + theta3);
            myport.WriteLine(theta1 + "," + theta2 + "," + theta3);
        }
            
            catch(Exception)
            {
                label8.Text = ("Point is out of limit");
            }
        }
        void receive(object sender, SerialDataReceivedEventArgs e)
        {
            data = myport.ReadLine();
            this.Invoke(new EventHandler(display));
            j++;
        }
        private void display(object sender, EventArgs e)
        {
            richTextBox1.AppendText(data + "\n");
        }
        private void connectToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                string[] ports = SerialPort.GetPortNames();

                foreach (string port in ports)
                {
                    myport = new SerialPort();
                    myport.BaudRate = 115200;
                    myport.PortName = port;
                    myport.Open();
                    label7.Text = port;
                }

            }
            catch
            {
                MessageBox.Show("Arduino connected " + myport.PortName + "!", "Arduino is connected.");
            }
        }

        private void disconnectToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                timer1.Enabled = false;
                button2.Enabled = false;
                myport.Close();
                label7.Text = "Disconnected";
                MessageBox.Show("Arduino disconnected.", "Disconnected");
            }
            catch (Exception)
            {
                MessageBox.Show("Error!", "Connection failed.");
            }
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            myport.Parity = Parity.None;
            myport.DataBits = 8;
            myport.StopBits = StopBits.One;
            in_data = myport.ReadLine();
        }

        private void richTextBox1_TextChanged(object sender, EventArgs e)
        {
            richTextBox1.SelectionStart = richTextBox1.Text.Length;
            richTextBox1.ScrollToCaret();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            myport.WriteLine("r");
            label8.Text = ("Angle: " + 0 + "," + 0 + "," + 0);
        }

        private void Button5_Click(object sender, EventArgs e)
        {
            if (myport.IsOpen)
            {
            }
            try
            {
                a++;
                textBox4.Text = a.ToString();
                myport.WriteLine("textBox4.Text");
            }
            catch(Exception)
            {

            }
        }

        private void Button6_Click(object sender, EventArgs e)
        {
            a--;
            textBox4.Text = a.ToString();
        }

        private void Button7_Click(object sender, EventArgs e)
        {
            b++;
            textBox5.Text = b.ToString();
        }

        private void Button8_Click(object sender, EventArgs e)
        {
            b--;
            textBox5.Text = b.ToString();
        }

        private void Button9_Click(object sender, EventArgs e)
        {
            c++;
            textBox6.Text = c.ToString();
        }

        private void Button10_Click(object sender, EventArgs e)
        {
            c--;
            textBox6.Text = c.ToString();
        }
    }
}
