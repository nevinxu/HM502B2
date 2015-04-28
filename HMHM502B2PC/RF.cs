using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Dundas.Charting.WinControl;
using System.IO;
using System.IO.Ports;
using Microsoft.Win32;

namespace MotionSensor
{
    public partial class RF : Form
    {

        const int N = 64;
        const int M = 8; //每次接收到包心电采样点
        string[] Xdata = new string[M * N];
        double[] XdataV = new double[M * N];

        byte[] ECGData = new byte[M*2];

        int chartlenMin = 0;    //chart 显示最大点数
        int chartlenMax = N * M;

        int chartYMin = 0;    //chart 显示最大点数
        int chartYMax = 1024;

        int SerialReceiveLength;

        private string comName; //port name
        private int comBaudRate; //port baudrate
        private int comDataBits = 8; //port databits

        private int rate = 0;
        private int Vbat = 0;
        private int LeadOffStatus = 0;
        private int bp_fig = 0;

        private byte ChoiceBLE = 0;   //选择的蓝牙序列号
        private int DataTransmissionFlag = 0;  //数据正在传输标志
        private byte PauseFlag = 1;  //初始化时暂停的

        private List<byte> SerialReceiveData = new List<byte>(40960);//默认分配1页内存，并始终限制不允许超
        private List<byte> SerialEcgData = new List<byte>(40960);//默认分配1页内存，并始终限制不允许超

        private byte EcgCaptureFlag = 0;
        private byte BLEConnectFlag = 0;

        private int ScanBLENum = 0;
        private byte[,] ScanBLEMAC = new byte[10,6];

        private int BLEConnectFlagTimerOut = 0;

        string StartStoreDataTime = "";

        private int connhandle = 0;





        public RF()
        {
            InitializeComponent();
            skinEngine1.SkinFile = Application.StartupPath + @"\gu.hw";
        }

        private void OutMsg(RichTextBox rtb, string msg, Color color)
        {
            rtb.Invoke(new EventHandler(delegate
            {
                rtb.SelectAll();
                rtb.SelectionColor = Color.Black;
                //  richTextBox.Text = rtb.Text.Insert(0, msg+ Environment.NewLine);
                rtb.SelectionStart = rtb.Text.Length;//设置插入符位置为文本框末
                rtb.SelectionColor = color;//设置文本颜色
                //  rtb.Text = richTextBox.Text;
                rtb.AppendText(msg);//输出文本，换行
                rtb.ScrollToCaret();//滚动条滚到到最新插入行
            }));
        }


        #region 获取串口号
        public void GetComList()
        {
            RegistryKey keyCom = Registry.LocalMachine.OpenSubKey("Hardware\\DeviceMap\\SerialComm");
            if (keyCom != null)
            {
                string[] sSubKeys = keyCom.GetValueNames();
                this.comboBoxCom.Items.Clear();
                foreach (string sName in sSubKeys)
                {
                    string sValue = (string)keyCom.GetValue(sName);
                    this.comboBoxCom.Items.Add(sValue);
                }
            }
        }
        #endregion

        #region 串口中断ISR
        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            int length;
          //  System.Threading.Thread.Sleep(5);
            if (!SerialPort.IsOpen)   //检测串口是否关闭
            {
                return;
            }
/********************************************串口接收**************************************************/
            length = SerialPort.BytesToRead;
            
            if ((length > 0) && (length < 5000))
            {
                SerialReceiveLength = length;
            }
            else
            {
                SerialPort.DiscardInBuffer();
                return;
            }

            byte[] buf = new byte[SerialReceiveLength];//声明一个临时数组存储当前来的串口数据 
            SerialPort.Read(buf, 0, SerialReceiveLength);
            SerialReceiveData.AddRange(buf);   
            WriteTheWin();
    
        }
        #endregion


        #region 串口初始化
        bool ComInit()
        {
            try
            {
                if (SerialPort.IsOpen)
                {
                    SerialPort.Close();
                }
                comName = comboBoxCom.Text;
                comBaudRate = int.Parse(comboBoxPortel.Text);
                SerialPort.PortName = comName; //port name COM1
                SerialPort.BaudRate = comBaudRate;  //BaudRate 115200bps
                SerialPort.Parity = Parity.None; //Parity none
                SerialPort.StopBits = StopBits.One; //StopBits 1
                SerialPort.DataBits = comDataBits;        // DataBits 8bit
                SerialPort.ReadTimeout = 1000;  // ReadTimeout 2 second
                SerialPort.Open();//打开串口
                this.toolStripStatusLabel1.Text = "已经打开串口：" + comName;
                    //+ ", 串口波特率：" + comBaudRate.ToString() +
                    //"  数据位：8位，  停止位:1位， 检验位：None,  流控制：None";
                this.SerialPort.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.serialPort1_DataReceived);
                SerialPort.ReceivedBytesThreshold = 4;
                return true;
            }
            catch (System.Exception)
            {
                MessageBox.Show("不能打开当前串口", "系统初始化失败", MessageBoxButtons.OK, MessageBoxIcon.Question);
                return false;
            }
        }
        #endregion

        #region 定时器时能
        public void WriteTheWin()
        {
            Timer.Enabled = true;
        }
        #endregion

        #region 波形的初始化
        private void Motion_Load(object sender, EventArgs e)
        {
            GetComList();    //获取当前所有的串口

            comboBoxCom.SelectedIndex = 0;
            comboBoxPortel.SelectedIndex = 1;
            MACComboBox.SelectedIndex = 0;
            radioButton1.Select();

            chart1.Titles[0].Text = "原始心电输出信号";
            chart1.Titles[0].Alignment = ContentAlignment.TopCenter;
            chart1.Titles[0].Font = new Font("黑体", 10, FontStyle.Bold);
            chart1.Titles[0].Color = Color.FromArgb(128, 72, 72);
            chart1.ChartAreas.Clear();
            chart1.ChartAreas.Add("DX");

            chart1.ChartAreas["DX"].AxisX.Arrows = ArrowsType.Lines;
            chart1.ChartAreas["DX"].AxisY.Title = "心电电压值";

            chart1.ChartAreas["DX"].AxisY.Arrows = ArrowsType.Lines;

            chart1.ChartAreas["DX"].AxisY.Maximum = chartYMax;
            chart1.ChartAreas["DX"].AxisY.Minimum = chartYMin;
            chart1.ChartAreas["DX"].AxisY.Interval = 50;

            chart1.ChartAreas["DX"].AxisX.Title = "时间";

            chart1.ChartAreas["DX"].AxisX.Maximum = chartlenMax;  //4s
            chart1.ChartAreas["DX"].AxisX.Minimum = chartlenMin;
           // chart1.ChartAreas["DX"].AxisX.Interval = 50;
            chart1.ChartAreas["DX"].AxisX.Interval = (chart1.ChartAreas["DX"].AxisX.Maximum - chart1.ChartAreas["DX"].AxisX.Minimum) / 20;

            Color c1 = Color.FromArgb(50, 50, 50);
            chart1.ChartAreas["DX"].AxisX.MajorGrid.LineColor = c1;
            chart1.ChartAreas["DX"].AxisY.MajorGrid.LineColor = c1;
            chart1.ChartAreas["DX"].ShadowColor = Color.White;
            chart1.ChartAreas["DX"].BackColor = Color.Black;

            chart1.Series.Clear();
            chart1.Series.Add("数据个数");
            chart1.Legends[0].Enabled = false;
            chart1.Series["数据个数"].Color = Color.Green;
            chart1.Series["数据个数"].Type = SeriesChartType.Line;
            chart1.Series["数据个数"].ChartArea = "DX";


            for (int i = 0; i < N * M; i++)
            {
                Xdata[i] = i.ToString();
            }
            chart1.Series["数据个数"].Points.DataBindXY(Xdata, XdataV);

            textBox1.Text = chartlenMin.ToString();
            textBox2.Text = chartlenMax.ToString();

            textBox3.Text = chartYMin.ToString();
            textBox4.Text = chartYMax.ToString();

        }
        #endregion

        private void chart1_MouseWheel(object sender, MouseEventArgs e)
        {
            if (e.Delta == 120)
            {
                if ((radioButton2.Checked) && (chartlenMax > chartlenMin))
                {
                    chartlenMax -= 5;
                }
                if ((radioButton1.Checked) && (chartlenMin > 0))
                {

                    chartlenMin -= 5;
                }
            }
            if (e.Delta == -120)
            {

                if ((radioButton2.Checked) && (chartlenMax < N * 128))
                {

                    chartlenMax += 5;
                }
                if ((radioButton1.Checked) && (chartlenMin + 5 < chartlenMax))
                {
                    chartlenMin += 5;
                }
            }
            textBox1.Text = chartlenMin.ToString();
            textBox2.Text = chartlenMax.ToString();
            chart1.ChartAreas["DX"].AxisX.Maximum = chartlenMax;
            chart1.ChartAreas["DX"].AxisX.Minimum = chartlenMin;
        }

        private void chart1_MouseClick(object sender, MouseEventArgs e)
        {
            this.chart1.Focus();
        }

        private void trackBar1_Scroll(object sender, EventArgs e)
        {
            chart1.ChartAreas["DX"].AxisX.Interval = (chartlenMax - chartlenMin)/ 16;
            if (radioButton2.Checked)
            {
                if (chartlenMax >= chartlenMin)
                {
                    chartlenMax = trackBar1.Value;
                }
                else
                {
                    chartlenMax = chartlenMin;
                }
            }
            else if (radioButton1.Checked)
            {
                if (chartlenMin <= chartlenMax)
                {
                    chartlenMin = trackBar1.Value;
                }
                else
                {
                    chartlenMin = chartlenMax;
                }
            }
            textBox1.Text = chartlenMin.ToString();
            textBox2.Text = chartlenMax.ToString();
            chart1.ChartAreas["DX"].AxisX.Maximum = chartlenMax;
            chart1.ChartAreas["DX"].AxisX.Minimum = chartlenMin;
        }

        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton1.Checked)
            {
                trackBar1.Value = chartlenMin;
            }
            trackBar1.Focus();
        }

        private void radioButton2_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton2.Checked)
            {
                trackBar1.Value = chartlenMax;
            }
            trackBar1.Focus();
        }

        private void SerialSetButton_Click(object sender, EventArgs e)
        {
            if (SerialSetButton.Text == "初始化串口")
            {
                if (ComInit())
                {
                    System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                    string DisplayString = "串口连接成功！\r\n";
                    DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                    OutMsg(MonitorText, DisplayString, Color.Red);
                    comboBoxCom.Enabled = false;
                    comboBoxPortel.Enabled = false;
                    SerialSetButton.Text = "关闭串口";
                    ScanButton.Enabled = true;

                    PauseFlag = 0;
                    PauseButton.Text = "运行中";

                    BLEConnectFlag = 1;  //假设设备正在通讯

                    SendConnectSerialCommand();  //发生连接central端命令  

                }
            }
            else
            {
                //SendDisConnectSerialCommand();
                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                string DisplayString = "串口已断开！\r\n";
                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                OutMsg(MonitorText, DisplayString, Color.Red);
                SerialSetButton.Text = "初始化串口";
                rate = 0;
                Vbat = 0;

                comboBoxCom.Enabled = true;
                comboBoxPortel.Enabled = true;
                this.toolStripStatusLabel1.Text = comName + "已经关闭！";
                if (SerialPort.IsOpen)
                {
                    SerialPort.Close();
                }
                DataStoreButton.Enabled = false;
                ScanButton.Enabled = false;
                ConnectBLEButton.Enabled = false;
            }
            for (int i = 0; i < N * M; i++)
            {
                XdataV[i] = 0;
            }
            chart1.Series["数据个数"].Points.DataBindXY(Xdata, XdataV);
        }

        #region 定时器处理函数
        private void timer1_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
        {
            int EcgMaxValue, EcgMinValue;

            while (SerialReceiveData.Count >= 5)
            {
                BLEConnectFlagTimerOut = 0;
                if (SerialReceiveData[0] == 0x04)   //type (command)
                {
                    if (SerialReceiveData[5] == 0x00)  //状态正常
                    {
                        if ((SerialReceiveData[3] == 0x00) && (SerialReceiveData[4] == 0x06))
                        { 
                                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                string DisplayString = "GAP_DeviceInitDone！\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);
                                TGAP_GEN_DISC_SCANCommand(1000);

                                if (EcgCaptureFlag == 1)
                                {
                                    // System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                    DisplayString = "设备已连接，心电采集中...\r\n";
                                    DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                    OutMsg(MonitorText, DisplayString, Color.Red);
                                    DataStoreButton.Enabled = true;
                                    StartStoreDataTime = DateTime.Now.ToString("yyyy-MM-dd") + "-" + DateTime.Now.Hour.ToString() + "-" + DateTime.Now.Minute.ToString();
                                    string filePath2 = System.IO.Directory.GetCurrentDirectory() + "//ecg_data.bin";
                                    //判断文件是不是存在
                                    if (File.Exists(filePath2))
                                    {
                                        //如果存在则删除
                                        File.Delete(filePath2);
                                    }

                                }
                        }
                        else if ((SerialReceiveData[3] == 0x7f) && (SerialReceiveData[4] == 0x06)) //GAP_HCI_ExtentionCommandStatus
                        {
                            if ((SerialReceiveData[6] == 0x00) && (SerialReceiveData[7] == 0xFE))
                            {
                                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                string DisplayString = "GAP_DeviceInit！\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);
                            }
                            else if ((SerialReceiveData[6] == 0x04) && (SerialReceiveData[7] == 0xFE))
                            {
                                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                string DisplayString = "GAP_DeviceDiscoveryRequest！\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);
                            }
                            else if ((SerialReceiveData[6] == 0x09) && (SerialReceiveData[7] == 0xFE))
                            {
                                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                string DisplayString = "GAP_EstablishLinkRequest！\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);
                                ConnectBLEButton.Text = "设备已连接";
                                PauseButton.Text = "运行中";

                                DisplayString = "设备已连接！\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);

                                BLEConnectFlag = 1;

                                DataStoreButton.Enabled = true;
                                StartStoreDataTime = DateTime.Now.ToString("yyyy-MM-dd") + "-" + DateTime.Now.Hour.ToString() + "-" + DateTime.Now.Minute.ToString();
                                string filePath3 = System.IO.Directory.GetCurrentDirectory() + "//ecg_data.bin";
                                //判断文件是不是存在
                                if (File.Exists(filePath3))
                                {
                                    //如果存在则删除
                                    File.Delete(filePath3);
                                }
                            }
                            else if ((SerialReceiveData[6] == 0x0A) && (SerialReceiveData[7] == 0xFE))
                            {
                                //System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                //string DisplayString = "GAP_TerminateLinkRequest！\r\n";
                                //DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                //OutMsg(MonitorText, DisplayString, Color.Red);
                                //ConnectBLEButton.Text = "设备已断开";
                                //ConnectBLEButton.Enabled = true;
                                //this.toolStripStatusLabel2.Text = "蓝牙设备状态：未连接";

                                //DataStoreButton.Enabled = false;

                                //ScanButton.Enabled = true;

                                //DisplayString = "设备已断开！\r\n";
                                //DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                //OutMsg(MonitorText, DisplayString, Color.Red);

                                //for (int i = 0; i < N * M; i++)
                                //{
                                //    XdataV[i] = 0;
                                //}
                                //chart1.Series["数据个数"].Points.DataBindXY(Xdata, XdataV);

                                //BLEConnectFlag = 0;
                            }
                            else if ((SerialReceiveData[6] == 0x30) && (SerialReceiveData[7] == 0xFE))
                            {
                                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                string DisplayString = "GAP_SetParam Success！\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);
                            }
                        }
                        else if ((SerialReceiveData[3] == 0x05) && (SerialReceiveData[4] == 0x06))
                        { 
                            connhandle = SerialReceiveData[13] + (SerialReceiveData[14]<<8);
                        }
                        else if ((SerialReceiveData[3] == 0x06) && (SerialReceiveData[4] == 0x06))
                        {
                            System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                            string DisplayString = "GAP_TerminateLinkRequest！\r\n";
                            DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                            OutMsg(MonitorText, DisplayString, Color.Red);
                            ConnectBLEButton.Text = "设备已断开";
                            ConnectBLEButton.Enabled = true;
                            this.toolStripStatusLabel2.Text = "蓝牙设备状态：未连接";

                            DataStoreButton.Enabled = false;

                            ScanButton.Enabled = true;

                            DisplayString = "设备已断开！\r\n";
                            DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                            OutMsg(MonitorText, DisplayString, Color.Red);

                            for (int i = 0; i < N * M; i++)
                            {
                                XdataV[i] = 0;
                            }
                            chart1.Series["数据个数"].Points.DataBindXY(Xdata, XdataV);

                            BLEConnectFlag = 0;
                        }
                        else if ((SerialReceiveData[3] == 0x0D) && (SerialReceiveData[4] == 0x06))
                        {
                            if ((SerialReceiveData[6] == 0x00) && (SerialReceiveData[7] == 0x02))
                            {
                                for (int i = 0; i < 6; i++)
                                {
                                    ScanBLEMAC[ScanBLENum, i] = SerialReceiveData[8 + i];
                                }
                                ScanBLENum++;

                                MACComboBox.Items.Add(Convert.ToString(SerialReceiveData[13], 16) + ":" + Convert.ToString(SerialReceiveData[12], 16) + ":" + Convert.ToString(SerialReceiveData[11], 16) + ":" + Convert.ToString(SerialReceiveData[10], 16) + ":" + Convert.ToString(SerialReceiveData[9], 16) + ":" + Convert.ToString(SerialReceiveData[8], 16));
                                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                string DisplayString = "搜索到一个设备！\r\n正在继续搜索设备...\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);

                            }
                            if ((SerialReceiveData[6] == 0x04) && (SerialReceiveData[7] == 0x02))
                            {
                                //MACComboBox.Items.Add(SerialReceiveData[8] + ":" + SerialReceiveData[9] + ":" + SerialReceiveData[10] + ":" + SerialReceiveData[11] + ":" + SerialReceiveData[9] + ":" + SerialReceiveData[13]);
                                //System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                //string DisplayString = "搜索到一个设备！\r\n正在继续搜索设备...\r\n";
                                //DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                //OutMsg(MonitorText, DisplayString, Color.Red);
                            }
                        }
                        else if ((SerialReceiveData[3] == 0x01) && (SerialReceiveData[4] == 0x06))
                        {
                            System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                            string DisplayString = "GAP_DeviceDiscoveryDone！\r\n";
                            DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                            OutMsg(MonitorText, DisplayString, Color.Red);
                            ScanButton.Enabled = true;
                            MACComboBox.Enabled = true;
                            if (ScanBLENum > 0)
                            {
                                MACComboBox.SelectedIndex = 0;
                            }
                            ConnectBLEButton.Enabled = true;
                            ConnectBLEButton.Text = "设备已断开";
                        }
                        else if ((SerialReceiveData[3] == 0x1b) && (SerialReceiveData[4] == 0x05))
                        {
                            EcgCaptureFlag = 1;
                            if (SerialReceiveData[8] == 18)
                            {
                                ;
                                for (int j = (M - 8); j < M; j++)   //
                                {
                                    for (int k = 0; k < N - 1; k++)
                                    {
                                        XdataV[k * M + j] = XdataV[(k + 1) * M + j];
                                    }
                                }
                                XdataV[(N - 1) * M + 0] = Convert.ToDouble(SerialReceiveData[15] & 0x7f) + Convert.ToDouble((SerialReceiveData[16] & 0x07) << 7);
                                XdataV[(N - 1) * M + 1] = Convert.ToDouble((SerialReceiveData[16] & 0x78) >> 3) + Convert.ToDouble((SerialReceiveData[17] & 0x3f) << 4);
                                XdataV[(N - 1) * M + 2] = Convert.ToDouble((SerialReceiveData[17] & 0x40) >> 6) + Convert.ToDouble((SerialReceiveData[18] & 0x7f) << 1) + Convert.ToDouble((SerialReceiveData[19] & 0x03) << 8);
                                XdataV[(N - 1) * M + 3] = Convert.ToDouble((SerialReceiveData[19] & 0x7c) >> 2) + Convert.ToDouble((SerialReceiveData[20] & 0x1f) << 5);
                                XdataV[(N - 1) * M + 4] = Convert.ToDouble((SerialReceiveData[20] & 0x60) >> 5) + Convert.ToDouble((SerialReceiveData[21] & 0x7f) << 2) + Convert.ToDouble((SerialReceiveData[22] & 0x01) << 9);
                                XdataV[(N - 1) * M + 5] = Convert.ToDouble((SerialReceiveData[22] & 0x7e) >> 1) + Convert.ToDouble((SerialReceiveData[23] & 0x0f) << 6);
                                XdataV[(N - 1) * M + 6] = Convert.ToDouble((SerialReceiveData[23] & 0x70) >> 4) + Convert.ToDouble((SerialReceiveData[24] & 0x7f) << 3);

                                XdataV[(N - 1) * M + 7] = Convert.ToDouble(SerialReceiveData[25] & 0x7f) + Convert.ToDouble((SerialReceiveData[26] & 0x07) << 7);
                                DataTransmissionFlag = 1;
                                BLEConnectFlagTimerOut = 0;
                                LeadOffStatus = Convert.ToInt16(SerialReceiveData[13]);
                                Vbat = Convert.ToInt16(SerialReceiveData[14]);
                            }
                            else if (SerialReceiveData[8] == 16)
                            {
                                for (int j = (M - 8); j < M; j++)   //
                                {
                                    for (int k = 0; k < N - 1; k++)
                                    {
                                        XdataV[k * M + j] = XdataV[(k + 1) * M + j];
                                    }
                                }
                                XdataV[(N - 1) * M + 0] = Convert.ToDouble(SerialReceiveData[15]) + Convert.ToDouble((SerialReceiveData[19] & 0xc0) << 2);
                                XdataV[(N - 1) * M + 1] = Convert.ToDouble(SerialReceiveData[16]) + Convert.ToDouble((SerialReceiveData[19] & 0x30) << 4);
                                XdataV[(N - 1) * M + 2] = Convert.ToDouble(SerialReceiveData[17]) + Convert.ToDouble((SerialReceiveData[19] & 0x0c) << 6);
                                XdataV[(N - 1) * M + 3] = Convert.ToDouble(SerialReceiveData[18]) + Convert.ToDouble((SerialReceiveData[19] & 0x03) << 8);
                                XdataV[(N - 1) * M + 4] = Convert.ToDouble(SerialReceiveData[20]) + Convert.ToDouble((SerialReceiveData[24] & 0xc0) << 2);
                                XdataV[(N - 1) * M + 5] = Convert.ToDouble(SerialReceiveData[21]) + Convert.ToDouble((SerialReceiveData[24] & 0x30) << 4);
                                XdataV[(N - 1) * M + 6] = Convert.ToDouble(SerialReceiveData[22]) + Convert.ToDouble((SerialReceiveData[24] & 0x0c) << 6);
                                XdataV[(N - 1) * M + 7] = Convert.ToDouble(SerialReceiveData[23]) + Convert.ToDouble((SerialReceiveData[24] & 0x03) << 8);


                                DataTransmissionFlag = 1;
                                BLEConnectFlagTimerOut = 0;
                                LeadOffStatus = Convert.ToInt16(SerialReceiveData[12]);
                                Vbat = Convert.ToInt16(SerialReceiveData[14]<<8)+ Convert.ToInt16(SerialReceiveData[13]);                          
                            }

                            else if (SerialReceiveData[8] == 22)
                            {
                                ;
                                for (int j = 0; j < M; j++)   //
                                {
                                    for (int k = 0; k < N - 1; k++)
                                    {
                                        XdataV[k * M + j] = XdataV[(k + 1) * M + j];
                                    }
                                    XdataV[(N - 1) * M + j] = (Convert.ToDouble(SerialReceiveData[(j << 1) + 16]) * 256) + Convert.ToDouble(SerialReceiveData[(j << 1) + 15]);
                                }
                                DataTransmissionFlag = 1;
                                BLEConnectFlagTimerOut = 0;
                                LeadOffStatus = Convert.ToInt16(SerialReceiveData[12]);
                                Vbat = Convert.ToInt16(SerialReceiveData[14]) * 256 + Convert.ToInt16(SerialReceiveData[13]);
                            }
                            //byte[] ECGData = new byte[M * 2 + 3];
                            //for (int i = 0; i < M * 2; i++)
                            //{
                            //   // ECGData[i] = SerialReceiveData[11 + i];
                            //}

                            FileStream fs = null;
                            string filePath = System.IO.Directory.GetCurrentDirectory() + "//ecg_data.bin";
                            try
                            {
                                fs = File.OpenWrite(filePath);
                                //设定书写的开始位置为文件的末尾   
                                fs.Position = fs.Length;
                                //将待写入内容追加到文件末尾   
                                fs.Write(ECGData, 0, M * 2);
                                fs.Position = fs.Length;

                            }
                            catch (Exception ex)
                            {
                                Console.WriteLine("文件打开失败{0}", ex.ToString());
                            }
                            finally
                            {
                                fs.Close();
                            }
                        }
                        
                        SerialReceiveData.RemoveRange(0, SerialReceiveData[2] + 3);//从接收列表中删除包
                    }
                    else
                    {
                        //这里是很重要的，如果数据开始不是头，则删除数据  
                        SerialReceiveData.RemoveAt(0);
                    }
                }
                else
                {
                    //这里是很重要的，如果数据开始不是头，则删除数据  
                    SerialReceiveData.RemoveAt(0);
                }
                
            }

            if (BLEConnectFlag == 1)
            {
                if (DataTransmissionFlag == 1)
                {
                    DataTransmissionFlag = 0;
                    ConnectBLEButton.Text = "设备已连接";
                    ConnectBLEButton.Enabled = true;
                    PauseButton.Enabled = true;
                    ScanButton.Enabled = false;

                    EcgMaxValue = 0;
                    EcgMinValue = 65535;
                    for (int i = 0; i < (M * N); i++)
                    {
                        if ((int)XdataV[i] > EcgMaxValue)
                        {
                            EcgMaxValue = (int)XdataV[i];
                        }
                        if ((int)XdataV[i] < EcgMinValue)
                        {
                            EcgMinValue = (int)XdataV[i];
                        }
                    }

                    chart1.ChartAreas["DX"].AxisY.Maximum = (EcgMaxValue + EcgMinValue) / 2 + 550;
                    chart1.ChartAreas["DX"].AxisY.Minimum = (EcgMaxValue + EcgMinValue) / 2 - 550;
                    chart1.ChartAreas["DX"].AxisY.Interval = 110;

                    //chart1.ChartAreas["DX"].AxisY.Maximum = 128;
                    //chart1.ChartAreas["DX"].AxisY.Minimum = -127;
                    //chart1.ChartAreas["DX"].AxisY.Interval = 50;

                    if (PauseButton.Text == "运行中")
                    {
                        this.toolStripStatusLabel2.Text = "蓝牙设备状态：已连接";
                        chart1.Series["数据个数"].Points.DataBindXY(Xdata, XdataV);
                    }


                }
            }
            if (ConnectBLEButton.Text == "设备已连接")
            {
                //HeartRate.Text = rate.ToString();
                BatteryValue.SelectAll();
                //double batteryValue = 3300 *4 * Vbat / 65535;
                double batteryValue = Vbat;
                BatteryValue.Text = batteryValue.ToString();
              //  BloodPressure.Text = "";

                if ((LeadOffStatus & 0x01) == 0x01)
                {
                    Lead.Text = "连接正常";
                    Lead.SelectAll();
                    Lead.SelectionColor = Color.Green;
                    Lead.Text = "连接正常";
                }
                else
                {
                    Lead.Text = "导联脱落";
                    Lead.SelectAll();
                    Lead.SelectionColor = Color.Red;
                    Lead.Text = "导联脱落";

                }
            }
            else
            {
                HeartRate.Text = "";
                BatteryValue.Text = "";
                Lead.Text = "";
                BloodPressure.Text = "";


            }

            Timer.Enabled = false;

        }
        #endregion

        #region 选择串口设备号
        private void comboBoxCom_MouseClick(object sender, MouseEventArgs e)
        {
            GetComList();
        }
        #endregion

        #region 保存文件按键处理函数
        private void button1_Click_1(object sender, EventArgs e)
        {
            System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
            string DisplayString = "开始保存心电数据。。。\r\n";
            DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
            OutMsg(MonitorText, DisplayString, Color.Red);


            // SaveFileDialog saveFileDialog = new SaveFileDialog();
            saveFileDialog1.Filter = "bin文件(*.bin)|*.bin|文本文件(*.txt)|*.txt";
            saveFileDialog1.Title = "保存心电数据";
            saveFileDialog1.FilterIndex = 1;
            saveFileDialog1.RestoreDirectory = true;
            string FileName = "ecg_data"+"  " + StartStoreDataTime +"--"+ DateTime.Now.ToString("yyyy-MM-dd") + "-" + DateTime.Now.Hour.ToString() + "-" + DateTime.Now.Minute.ToString() + ".bin"; // 
            saveFileDialog1.FileName = FileName;
            saveFileDialog1.AddExtension = true;
            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                //string sss = saveFileDialog1.FileName;
                //FileStream fst = new FileStream(sss, FileMode.Append);
                //StreamWriter swt = new StreamWriter(fst, System.Text.Encoding.GetEncoding("utf-8"));
                //for (int i = 0; i < 256; i++)
                //{
                //    string adrbuf = XdataV[i].
                //swt.Close();ToString();
                //    swt.WriteLine(adrbuf);
                //}
                //fst.Close();
                string filePath = System.IO.Directory.GetCurrentDirectory() + "//ecg_data.bin";
                string localFilePath = saveFileDialog1.FileName.ToString(); //获得文件路径 

                try
                {
                    File.Copy(filePath, localFilePath);
                }
                catch (Exception ex)
                {
                    Console.WriteLine("文件删除失败{0}", ex.ToString());
                }
                finally
                {
                }
                MessageBox.Show("保存数据成功！");
            }
        }
        #endregion

        #region 第二个按键处理函数
        private void ScanButton_Click(object sender, EventArgs e)
        {
            if (ScanButton.Text == "接收数据")
            {
                ;
            }
            if (ScanButton.Text == "搜索设备")
            {
                SendDisAdvertiseSerialCommand();
                ScanButton.Enabled = false;
                ConnectBLEButton.Enabled = false;
                MACComboBox.Enabled = false;
                BLENUMlabel.Text = "";
                ScanBLENum = 0;
            }
            PauseButton.Enabled = false;
        }
        #endregion


        #region 选择连接设备的MAC
        private void MACComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            ChoiceBLE = (byte)MACComboBox.SelectedIndex;
        }
        #endregion

        #region 富文本内容改变处理函数
        private void richTextBox3_TextChanged_1(object sender, EventArgs e)
        {
            MonitorText.ScrollToCaret();
        }
        #endregion

        #region 第一个按键处理函数
        private void ConnectBLEButton_Click(object sender, EventArgs e)
        {
            if (ConnectBLEButton.Text == "设备已连接")
            {
                DisconnectBLESerialCommand();
            }
            else
            {
                ConnectBLESerialCommand();
            }
            
        }
        #endregion

        private void button4_Click(object sender, EventArgs e)
        {

        }


        private void SendConnectSerialCommand()
        {
            byte[] ssss ={01, 00, 0xFE, 0x26, 08, 05, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 
            00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 
            00, 00, 00, 00, 00, 00, 01, 00, 00, 00 };
            SerialPort.Write(ssss, 0, 42);

            System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
            string DisplayString = "Send <GAP_DeviceInit> Command!!!\r\n";
            DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
            OutMsg(MonitorText, DisplayString, Color.Red);
        }
        private void SendDisConnectSerialCommand()
        {
            byte[] ssss = new byte[4];
            ssss[0] = 0x77;
            ssss[1] = 0x0C;
            ssss[2] = 0x00;
            ssss[3] = 0x00;
            SerialPort.Write(ssss, 0, 4);
        }
        private void SendDisAdvertiseSerialCommand()
        {
            byte[] ssss = {01, 04, 0xFE, 03, 03, 01, 00};
            SerialPort.Write(ssss, 0, 7);

            System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
            string DisplayString = "Send <GAP_DeviceDiscoveryRequest> Command!!!\r\n";
            DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
            OutMsg(MonitorText, DisplayString, Color.Red);

            MACComboBox.Items.Clear();
        }
        private void ConnectBLESerialCommand()
        {
            byte[] ssss ={ 01, 09, 0xFE, 0x09, 00, 00, 02, 0xE6, 0x39, 00, 0x0B, 0x0E, 00 };

            for (int i = 0; i < 6; i++)
            {
                ssss[7 + i] = ScanBLEMAC[ChoiceBLE, i];
            }

                SerialPort.Write(ssss, 0, 13);

            System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
            string DisplayString = "Send <GAP_EstablishLinkRequest> Command!!!\r\n";
            DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
            OutMsg(MonitorText, DisplayString, Color.Red);
        }
        private void DisconnectBLESerialCommand()
        {
            byte[] ssss ={ 01, 0x0A, 0xFE, 03, 00, 00, 0x13 };
            ssss[4] = (byte)connhandle;
            ssss[5] = (byte)(connhandle >> 8);

            try
            {
                SerialPort.Write(ssss, 0, 7);
            }
            catch (Exception ex)
            {
                Console.WriteLine("串口连接失败{0}", ex.ToString());
                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                string DisplayString = "主设备usb连接断开,请重新插拔设备!!\r\n";
                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                OutMsg(MonitorText, DisplayString, Color.Red);
                ConnectBLEButton.Text = "设备已断开";
                ConnectBLEButton.Enabled = true;
                this.toolStripStatusLabel2.Text = "蓝牙设备状态：未连接";

                DataStoreButton.Enabled = false;

                ScanButton.Enabled = true;

                DisplayString = "设备已断开！\r\n";
                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                OutMsg(MonitorText, DisplayString, Color.Red);

                for (int i = 0; i < N * M; i++)
                {
                    XdataV[i] = 0;
                }
                chart1.Series["数据个数"].Points.DataBindXY(Xdata, XdataV);

                BLEConnectFlag = 0;
                return;
            }
            finally
            {

            }
            System.Text.ASCIIEncoding converter2 = new System.Text.ASCIIEncoding();
            string DisplayString2 = "Send <GAP_EstablishLinkRequest> Command!!!\r\n";
            DisplayString2 = DateTime.Now.ToLongTimeString() + ": " + DisplayString2;
            OutMsg(MonitorText, DisplayString2, Color.Red);

            string DisplayString3 = "设备正在断开，请稍等。。。\r\n";
            DisplayString3 = DateTime.Now.ToLongTimeString() + ": " + DisplayString3;
            OutMsg(MonitorText, DisplayString3, Color.Red);
        }
        private void TGAP_GEN_DISC_SCANCommand(short msec)
        {
            byte[] ssss ={ 01, 0x30, 0xFE, 03, 02, 00, 04} ;
            ssss[5] = (byte)(msec);
            ssss[6] = (byte)(msec>>8);
            SerialPort.Write(ssss, 0, 7);
            System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
            string DisplayString = "Send <TGAP_GEN_DISC_SCAN> Command  !!!\r\n";
            DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
            OutMsg(MonitorText, DisplayString, Color.Red);
        }
       // private void DataReceiveSuccessCommand()


        private void PauseButton_Click(object sender, EventArgs e)
        {
            if (PauseFlag == 1)
            {
                PauseFlag = 0;
                PauseButton.Text = "运行中";

                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                string DisplayString = "心电采集中...\r\n";
                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                OutMsg(MonitorText, DisplayString, Color.Red);

                for (int i = 0; i < N * M; i++)
                {
                    XdataV[i] = 0;
                }
                chart1.Series["数据个数"].Points.DataBindXY(Xdata, XdataV);
            }
            else
            {
                PauseFlag = 1;
                PauseButton.Text = "暂停";

                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                string DisplayString = "心电采集暂停!!!\r\n";
                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                OutMsg(MonitorText, DisplayString, Color.Red);
            }
        }


        private void radioButton3_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton3.Checked)
            {
                trackBar2.Value = chartYMin;
            }
            trackBar2.Focus();
        }

        private void radioButton4_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton4.Checked)
            {
                trackBar2.Value = chartYMax;
            }
            trackBar2.Focus();
        }

        private void trackBar2_Scroll(object sender, EventArgs e)
        {
            chart1.ChartAreas["DX"].AxisY.Interval = (chartYMax - chartYMin) / 16;
            if (radioButton4.Checked)
            {
                if (chartYMax > chartYMin)
                {
                    chartYMax = trackBar2.Value;
                }
                else
                {
                    chartYMax = chartYMin+10;
                    trackBar2.Value = chartYMax;
                }
            }
            else if(radioButton3.Checked)
            {
                if (chartYMin < chartYMax)
                {
                    chartYMin = trackBar2.Value;
                }
                else
                {
                    chartYMin = chartYMax-10;
                    trackBar2.Value = chartYMin;
                }
            }
            textBox3.Text = chartYMin.ToString();
            textBox4.Text = chartYMax.ToString();

            chart1.ChartAreas["DX"].AxisY.Maximum = chartYMax;
            chart1.ChartAreas["DX"].AxisY.Minimum = chartYMin;
        }

        private void button1_Click_2(object sender, EventArgs e)
        {

        }

        private void textBoxScan_KeyPress(object sender, KeyPressEventArgs e)
        {
             if (e.KeyChar.Equals('\r'))
            {
                try
                {
                    if ((textBoxScan.Text.Length == 15)/* && (textBoxScan.Text.Length >=9)*/)
                    {
                      //  textBoxID.Text = textBoxScan.Text.Substring(textBoxScan.Text.Length - 9, 9);
                        textBoxScan.SelectAll();
                        textBoxScan.Focus();
                        string DisplayString = DateTime.Now.ToLongTimeString() + ": "+"条码扫描：" + textBoxScan.Text.ToString() + "\r\n";
                        OutMsg(MonitorText, DisplayString, Color.Red);
                    }
                    else
                    {
                        MessageBox.Show("条码长度大于15位或者小于9位，可能导致数据截取错误！", "条码扫描异常", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                }
                catch (ArgumentOutOfRangeException)
                {
                    MessageBox.Show("条码长度大于15位或者小于9位，可能导致数据截取错误！", "条码扫描异常", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }

            }
        }

        private void IDValuebutton_Click(object sender, EventArgs e)
        {
            if (ConnectBLEButton.Text == "设备已连接")
            {
                byte[] ssss = new byte[13];
                ssss[0] = 0x77;   //
                ssss[1] = 0x10;   //ID发送命令
                ssss[2] = 0x00;
                ssss[3] = 0x09;
               // char[] charIDValue = textBoxID.Text.ToCharArray();
                try
                {
               //     if (textBoxID.Text.Length == 9)
                    {
                        for (int i = 0; i < 9; i++)
                        {
                           // ssss[4 + i] = (byte)charIDValue[i];
                        }
                        if (SerialPort.IsOpen)
                        {
                            SerialPort.Write(ssss, 0, 13);
                        }
                        else
                        {
                            string DisplayString = "串口未打开！\r\n";
                            DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                            OutMsg(MonitorText, DisplayString, Color.Red);
                        }
                    }
                   // else
                    {
                        string DisplayString = "ID数据错误！\r\n";
                        DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                        OutMsg(MonitorText, DisplayString, Color.Red);
                    }
                }
                catch (ArgumentOutOfRangeException)
                {
                    MessageBox.Show("数据出错！");
                }
            }
            else
            {
                string DisplayString = "未连接心电补丁！\r\n";
                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                OutMsg(MonitorText, DisplayString, Color.Red);
            }



        }

        private void chart1_Click(object sender, EventArgs e)
        {

        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            BLEConnectFlagTimerOut++;
            if (BLEConnectFlagTimerOut >= 30)
            {
                BLEConnectFlagTimerOut = 0;
                if (ConnectBLEButton.Text == "设备已连接")
                {
                    DisconnectBLESerialCommand();
                    ConnectBLEButton.Enabled = false;
                    ConnectBLEButton.Text = "设备正在断开";
                }
                
            }
        }

        private void button4_Click_1(object sender, EventArgs e)
        {
            ConnectBLESerialCommand();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            DisconnectBLESerialCommand();
        }

        private void label12_Click(object sender, EventArgs e)
        {

        }
    }
}
