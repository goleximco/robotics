using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using System.Threading;
using System.Net;
using System.Net.Sockets;

namespace ConsoleApplication2udp
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            UdpClient udpClient = new UdpClient();
            udpClient.Connect(textBox_ip.Text, Convert.ToInt16(textBox_port.Text));
            Byte[] senddata = Encoding.ASCII.GetBytes(textBox_message.Text);
            udpClient.Send(senddata, senddata.Length);
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            Thread thdUDPServer = new Thread(new ThreadStart(serverThread));
            thdUDPServer.Start();
        }

        public void serverThread()
        {
            UdpClient udpClient = new UdpClient(8080);
            while (true)
            {
                IPEndPoint RemoteIpEndPoint = new IPEndPoint(IPAddress.Any, 8080);
                Byte[] receiveBytes = udpClient.Receive(ref RemoteIpEndPoint);
                string returnData = Encoding.ASCII.GetString(receiveBytes);
                
                this.Invoke(new MethodInvoker(delegate()
                {
                    
                    listBox_received.Items.Add(RemoteIpEndPoint.Address.ToString() + ":" + returnData.ToString());
                    listBox_received.SelectedIndex = listBox_received.Items.Count - 1;
                    listBox_received.SelectedIndex = -1;

                }));
            }
        }

    }
}
