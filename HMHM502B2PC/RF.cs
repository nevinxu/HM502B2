using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.IO.Ports;
using Microsoft.Win32;
using System.Windows.Forms.DataVisualization.Charting;
using System.Threading;
using RFtest;

namespace MotionSensor
{
    public partial class RF : Form
    {
        const int N = 64;
        const int M = 8; //每次接收到包心电采样点
        string[] Xdata = new string[M * N];
        double[] XdataV = new double[M * N];

        byte[] ECGData = new byte[M * 2];

        int chartlenMin = 0;    //chart 显示最大点数
        int chartlenMax = N * M;

        int chartYMin = 0;    //chart 显示最大点数
        int chartYMax = 1024;

        int SerialReceiveLength;

        private string comName; //port name
        private int comBaudRate; //port baudrate
        private int comDataBits = 8; //port databits

        private int Vbat = 0;
        private int LeadOffStatus = 0;

        private byte ChoiceBLE = 0;   //选择的蓝牙序列号
        private int DataTransmissionFlag = 0;  //数据正在传输标志
        private byte PauseFlag = 1;  //初始化时暂停的

        private List<byte> SerialReceiveData = new List<byte>(40960);//默认分配1页内存，并始终限制不允许超
        private List<byte> SerialReceiveData2 = new List<byte>(40960);//默认分配1页内存，并始终限制不允许超
        private List<byte> SerialReceiveData3 = new List<byte>(40960);//默认分配1页内存，并始终限制不允许超
        private List<byte> SerialReceiveData4 = new List<byte>(40960);//默认分配1页内存，并始终限制不允许超
        private List<double> SerialEcgData = new List<double>(512);//默认分配1页内存，并始终限制不允许超

        private byte EcgCaptureFlag = 0;
        private byte BLEConnectFlag = 0;

        private int ScanBLENum = 0;
        private byte[,] ScanBLEMAC = new byte[100, 6];

        string StartStoreDataTime = "";

        private int connhandle = 0;

        private int DebugMode = 2;
        private int EcgReceiveFlagTimer = 0;

        private int DisConnectBLEEnableFlag = 0;

        private int RSSIValue = 0;

        byte[] ECGPatchID = new byte[15];
        byte[] ECGPatchMAC = new byte[6];
        byte[] BLECentralMAC = new byte[6];

        private int PairingFlag = 1;
        byte[] ECGPairMAC = new byte[6];
        private int EcgDataTimer = 0;
        private Int16 amplification = 60;
        private Int16 difference_Value = 525;

        private Int16 amplification_Back = 1;
        private Int16 difference_Value_Back = 0;

        private int ScalingFlag = 0;
        private int calibration_Num = 0;
        private List<double> calibration_Value = new List<double>(1024);
        private double[] ScalingEcgMin = new double[8];
        private double[] ScalingEcgMax = new double[8];

        private int ZeroValue = 9999;
        private int BaseLine = 0;

        byte[] HardVersion = new byte[5];
        byte[] SoftVersion = new byte[12];

        private double LostPackageNum = 0;
        private double ReceivePackageNum = 0;

        private int LastPackageNum = -1;

        private int DeviceConnectMode = 0;    //1 蓝牙  2 直接连接

        private int ComConnectFlag = 0;   //串口连接标志
        private int EcgComConnectFlag = 0;
        private int[] UpDataComConnectFlag = new int[4];
        private int UpDtatComConnectedNum = 0;
        private int[] ComConnectTimeOut = new int[20];
        private int AutoComConnectFlag = 1;
        private int Ecgamplification = 1;
        private int StartEcgCaptureFlag = 0;

        byte[,] ScanBleName = new byte[100, 11];

        int X_Start;
        int Y_Start;
        DataPoint StoreDP;
        DataPoint StartStoreDP;

        private int UpgradeFlag = 1;  //升级标志

        private int UpgradeNum = 0;

        SerialCommand serialcommand = new SerialCommand();
        private int ConnectUartNum = 0;
        private string[] ConnectUartName = new string[20];
        private int[] DownLoadFlag = new int[4];
        private int ScanUpDataComFlag = 0;

        Thread ScanUpdataThread;
        Thread UpDataThread1;
        Thread UpDataThread2;
        Thread UpDataThread3;
        Thread UpDataThread4;
        private int DowmLoadNum = 0;


        public RF()
        {
            InitializeComponent();
        }

        void ScanEcgUartFunction()
        { 
        
        }

        void UpDataComUpDataConnectReq(SerialPort serialport)
        {
            if (ComInit(serialport))
            {
                serialcommand.UpGradeCommand(serialport);
            }
        }
        void ScanUpdataUartFunction()
        {
            GetComList();    //获取当前所有的串口
            comboBoxCom.Invoke(new EventHandler(delegate
            {
                comboBoxCom.SelectedIndex = 0;
            }));
           // UpDtatComConnectedNum = 0;//当前连接的串口数
            while (true)
            {
                //if (ScanUpDataComFlag == 1)
                {
                    for (int i = 0; i < 4; i++)
                    {
                        if (UpDataComConnectFlag[i] == 1)   //判断是都断开
                        {
                            comboBoxCom.Invoke(new EventHandler(delegate
                            {
                                comboBoxCom.Text = ConnectUartName[i];
                            }));
                        }
                        else                                //搜索为连接的设备
                        {
                            comboBoxCom.Invoke(new EventHandler(delegate
                             {
                                 if (comboBoxCom.SelectedIndex < (comboBoxCom.Items.Count - 1))
                                 {
                                     comboBoxCom.SelectedIndex++;
                                 }
                                 else if (comboBoxCom.SelectedIndex >= (comboBoxCom.Items.Count - 1))
                                 {
                                     GetComList();    //获取当前所有的串口
                                     comboBoxCom.SelectedIndex = 0;
                                 }
                                 for (int j = 0; j < 4; j++)
                                 {
                                     if (comboBoxCom.Text == ConnectUartName[j])
                                     {
                                         if (comboBoxCom.SelectedIndex < (comboBoxCom.Items.Count - 1))
                                         {
                                             comboBoxCom.SelectedIndex++;
                                         }
                                         else if (comboBoxCom.SelectedIndex >= (comboBoxCom.Items.Count - 1))
                                         {
                                             GetComList();    //获取当前所有的串口
                                             comboBoxCom.SelectedIndex = 0;
                                         }
                                     }
                                 }
                                 ConnectUartName[i] = comboBoxCom.Text;

                             }));
                        }

                        System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                        string DisplayString = "升级串口：请求连接" + ConnectUartName[i] + "...\r\n";
                        DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                        OutMsg(MonitorText, DisplayString, Color.Red);
                        DownLoadFlag[i] = 13;
                        if (i == 0)
                        {
                            UpDataComUpDataConnectReq(serialPort1);
                        }
                        else if (i == 1)
                        {
                            UpDataComUpDataConnectReq(serialPort2);
                        }
                        else if (i == 2)
                        {
                            UpDataComUpDataConnectReq(serialPort3);
                        }
                        else if (i == 3)
                        {
                            UpDataComUpDataConnectReq(serialPort4);
                        }
                        Thread.Sleep(500);
                        if (DownLoadFlag[i] == 14)
                        {
                            converter = new System.Text.ASCIIEncoding();
                            DisplayString = "升级串口：" + ConnectUartName[i] + "连接成功！\r\n";
                            DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                            OutMsg(MonitorText, DisplayString, Color.Red);

                            tabControl1.Invoke(new EventHandler(delegate
                            {
                                tabControl1.SelectedIndex = 1;
                            }));
                            if (i == 0)
                            {
                                checkBox3.Invoke(new EventHandler(delegate
                                {
                                    checkBox3.Text = ConnectUartName[i] + "已连接";
                                    checkBox3.Checked = true;
                                }));
                            }
                            else if (i == 1)
                            {
                                checkBox4.Invoke(new EventHandler(delegate
                                {
                                    checkBox4.Text = ConnectUartName[i] + "已连接";
                                    checkBox4.Checked = true;
                                }));
                            }
                            else if (i == 2)
                            {
                                checkBox5.Invoke(new EventHandler(delegate
                                {
                                    checkBox5.Text = ConnectUartName[i] + "已连接";
                                    checkBox5.Checked = true;
                                }));
                            }
                            else if (i == 3)
                            {
                                checkBox6.Invoke(new EventHandler(delegate
                                {
                                    checkBox6.Text = ConnectUartName[i] + "已连接";
                                    checkBox6.Checked = true;
                                }));
                            }
                            UpDataComConnectFlag[i] = 1;
                        }
                        else
                        {
                            converter = new System.Text.ASCIIEncoding();
                            DisplayString = "升级串口：" + ConnectUartName[i] + "连接失败！\r\n";
                            DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                            OutMsg(MonitorText, DisplayString, Color.Red);

                            ConnectUartName[i] = "";

                            if (i == 0)
                            {
                                checkBox3.Invoke(new EventHandler(delegate
                                {
                                    checkBox3.Text = "未连接";
                                    checkBox3.Checked = false;
                                }));
                            }
                            else if (i == 1)
                            {
                                checkBox4.Invoke(new EventHandler(delegate
                                {
                                    checkBox4.Text = "未连接";
                                    checkBox4.Checked = false;
                                }));
                            }
                            else if (i == 2)
                            {
                                checkBox5.Invoke(new EventHandler(delegate
                                {
                                    checkBox5.Text = "未连接";
                                    checkBox5.Checked = false;
                                }));
                            }
                            else if (i == 3)
                            {
                                checkBox6.Invoke(new EventHandler(delegate
                                {
                                    checkBox6.Text = "未连接";
                                    checkBox6.Checked = false;
                                }));
                            }

                            if (i == 0)
                            {
                                serialPort1.Close();
                            }
                            else if (i == 1)
                            {
                                serialPort2.Close();
                            }
                            else if (i == 2)
                            {
                                serialPort3.Close();
                            }
                            else if (i == 3)
                            {
                                serialPort4.Close();
                            }


                            UpDataComConnectFlag[i] = 0;
                        }
                    }
                }
                //else
                //{
                //    Thread.Sleep(1000);
                //}
                if (checkBox3.Checked == true || (checkBox4.Checked == true) || (checkBox5.Checked == true) || (checkBox6.Checked == true))
                {
                    UpDataButton.Invoke(new EventHandler(delegate
                    {
                        UpDataButton.Enabled = true;
                        tabControl1.TabPages[1].Text = "程序下载（可升级）";
                    }));
                }
            }
        }

        private void OutMsg(RichTextBox rtb, string msg, Color color)
        {
            if (rtb.IsDisposed || !rtb.IsHandleCreated) return;
                rtb.Invoke(new EventHandler(delegate
                {
                    rtb.SelectAll();
                    rtb.SelectionColor = Color.Black;
                    rtb.Text = rtb.Text.Insert(0, msg);
                    rtb.Select(0, msg.Length - 1);
                    rtb.SelectionColor = color;//设置文本颜色
                    //  rtb.AppendText(msg);//输出文本，换行
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
                comboBoxCom.Invoke(new EventHandler(delegate
                {
                    this.comboBoxCom.Items.Clear();
                }));
                foreach (string sName in sSubKeys)
                {
                    string sValue = (string)keyCom.GetValue(sName);
                    comboBoxCom.Invoke(new EventHandler(delegate
                    {
                        this.comboBoxCom.Items.Add(sValue);
                    }));
                }
            }
        }
        #endregion


        void UpdataReceiveDataProcess(int i,List<byte> ReceiveDataList)
        {
            if (ReceiveDataList.Count >= 2)
            {
                if (ReceiveDataList[0] == 0x5a && (ReceiveDataList[1] == 0xa7))
                {
                    if (DownLoadFlag[i] == 13)
                    {
                        DownLoadFlag[i] = 14;
                    }
                    else
                    {
                        DownLoadFlag[i] = 15;
                    }
                    ReceiveDataList.Clear();
                }
                else if (ReceiveDataList[0] == 0x5a && (ReceiveDataList[1] == 0xa1))
                {
                    if (DownLoadFlag[i] == 1)
                    {
                        if (ReceiveDataList.Count >= 20)
                        {
                            if (ReceiveDataList[2] == 0x5a && (ReceiveDataList[3] == 0xa4))
                            {
                                if (ReceiveDataList[12] == 0 && (ReceiveDataList[16] == 0x0D))
                                {
                                    DownLoadFlag[i] = 2;
                                }
                                else if (ReceiveDataList[12] == 0 && (ReceiveDataList[16] == 0x01))
                                {
                                    DownLoadFlag[i] = 2;
                                }
                                else
                                {
                                    DownLoadFlag[i] = 3;
                                }
                            }
                            else
                            {
                                DownLoadFlag[i] = 3;
                            }
                            ReceiveDataList.Clear();
                        }
                    }
                    else if (DownLoadFlag[i] == 4)
                    {
                        if (ReceiveDataList.Count >= 20)
                        {
                            if (ReceiveDataList[2] == 0x5a && (ReceiveDataList[3] == 0xa4))
                            {
                                if (ReceiveDataList[12] == 0 && (ReceiveDataList[16] == 0x04))
                                {
                                    DownLoadFlag[i] = 5;
                                }
                                else
                                {
                                    DownLoadFlag[i] = 6;
                                }
                            }
                            else
                            {
                                DownLoadFlag[i] = 6;
                            }
                            ReceiveDataList.Clear();
                        }
                    }
                    else if (DownLoadFlag[i] == 7)
                    {
                        if (ReceiveDataList.Count >= 2)
                        {
                            if (ReceiveDataList[0] == 0x5a && (ReceiveDataList[1] == 0xa1))
                            {
                                DownLoadFlag[i] = 8;
                            }
                            else
                            {
                                DownLoadFlag[i] = 9;
                            }
                            ReceiveDataList.Clear();
                        }
                    }
                    else if (DownLoadFlag[i] == 10)
                    {
                        if (ReceiveDataList.Count >= 20)
                        {
                            if (ReceiveDataList[0] == 0x5a && (ReceiveDataList[1] == 0xa1))
                            {
                                if (ReceiveDataList[2] == 0x5a && (ReceiveDataList[3] == 0xa4))
                                {
                                    if (ReceiveDataList[12] == 0 && (ReceiveDataList[16] == 0x04))
                                    {
                                        DownLoadFlag[i] = 11;
                                    }
                                    else
                                    {
                                        DownLoadFlag[i] = 12;
                                    }
                                }
                                else
                                {
                                    DownLoadFlag[i] = 12;
                                }
                            }
                            else
                            {
                                DownLoadFlag[i] = 12;
                            }
                            ReceiveDataList.Clear();
                        }
                    }
                }
                else
                {
                    ReceiveDataList.RemoveAt(0);
                }
            }
        }

        #region 串口中断ISR
        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            int length;
            int SerialReceiveLength;
            System.Threading.Thread.Sleep(2);
            if (!serialPort1.IsOpen)   //检测串口是否关闭
            {
                return;
            }
            /********************************************串口接收**************************************************/
            length = serialPort1.BytesToRead;
            if (length == 0)
            {
                return;
            }

            if ((length > 0) && (length < 5000))
            {
                SerialReceiveLength = length;
            }
            else
            {
                serialPort1.DiscardInBuffer();
                return;
            }

            byte[] buf = new byte[SerialReceiveLength];//声明一个临时数组存储当前来的串口数据 
            serialPort1.Read(buf, 0, SerialReceiveLength);
            SerialReceiveData.AddRange(buf);

            UpdataReceiveDataProcess(0,SerialReceiveData);

        }
        #endregion

        private void serialPort2_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            int length;
            int SerialReceiveLength;
            System.Threading.Thread.Sleep(2);
            if (!serialPort2.IsOpen)   //检测串口是否关闭
            {
                return;
            }
            /********************************************串口接收**************************************************/
            length = serialPort2.BytesToRead;
            if (length == 0)
            {
                return;
            }

            if ((length > 0) && (length < 5000))
            {
                SerialReceiveLength = length;
            }
            else
            {
                serialPort2.DiscardInBuffer();
                return;
            }

            byte[] buf = new byte[SerialReceiveLength];//声明一个临时数组存储当前来的串口数据 
            serialPort2.Read(buf, 0, SerialReceiveLength);

            SerialReceiveData2.AddRange(buf);

            UpdataReceiveDataProcess(1,SerialReceiveData2);
        }
        private void serialPort3_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            int length;
            int SerialReceiveLength;
            System.Threading.Thread.Sleep(2);
            if (!serialPort3.IsOpen)   //检测串口是否关闭
            {
                return;
            }
            /********************************************串口接收**************************************************/
            length = serialPort3.BytesToRead;
            if (length == 0)
            {
                return;
            }

            if ((length > 0) && (length < 5000))
            {
                SerialReceiveLength = length;
            }
            else
            {
                serialPort3.DiscardInBuffer();
                return;
            }

            byte[] buf = new byte[SerialReceiveLength];//声明一个临时数组存储当前来的串口数据 
            serialPort3.Read(buf, 0, SerialReceiveLength);

            SerialReceiveData3.AddRange(buf);

            UpdataReceiveDataProcess(2,SerialReceiveData3);
        }
        private void serialPort4_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            int length;
            int SerialReceiveLength;
            System.Threading.Thread.Sleep(2);
            if (!serialPort4.IsOpen)   //检测串口是否关闭
            {
                return;
            }
            /********************************************串口接收**************************************************/
            length = serialPort4.BytesToRead;
            if (length == 0)
            {
                return;
            }

            if ((length > 0) && (length < 5000))
            {
                SerialReceiveLength = length;
            }
            else
            {
                serialPort4.DiscardInBuffer();
                return;
            }

            byte[] buf = new byte[SerialReceiveLength];//声明一个临时数组存储当前来的串口数据 
            serialPort4.Read(buf, 0, SerialReceiveLength);

            SerialReceiveData4.AddRange(buf);

            UpdataReceiveDataProcess(3,SerialReceiveData4);
        }


        #region 串口初始化
        bool ComInit(SerialPort serialport)
        {
            if (serialport.IsOpen)
            {
                serialport.Close();
            }
            try
            {
                comboBoxCom.Invoke(new EventHandler(delegate
                {
                    comName = comboBoxCom.Text;
                }));

                comboBoxPortel.Invoke(new EventHandler(delegate
                {
                    comBaudRate = int.Parse(comboBoxPortel.Text);
                }));

                serialport.PortName = comName; //port name COM1
                serialport.BaudRate = comBaudRate;  //BaudRate 115200bps
                serialport.Parity = Parity.None; //Parity none
                serialport.StopBits = StopBits.One; //StopBits 1
                serialport.DataBits = comDataBits;        // DataBits 8bit
                serialport.ReadTimeout = 1000;  // ReadTimeout 2 second
                serialport.Open();//打开串口
               // this.toolStripStatusLabel1.Text = "已经打开串口：" + comName;
                serialport.ReceivedBytesThreshold = 2;

                if (serialport == serialPort1)
                {
                    serialport.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(serialPort1_DataReceived);
                }
                else if (serialport == serialPort2)
                {
                    serialport.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(serialPort2_DataReceived);
                }
                else if (serialport == serialPort3)
                {
                    serialport.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(serialPort3_DataReceived);
                }
                else if (serialport == serialPort4)
                {
                    serialport.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(serialPort4_DataReceived);
                }

                
                return true;
            }
            catch (System.Exception)
            {
                // MessageBox.Show("不能打开当前串口", "系统初始化失败", MessageBoxButtons.OK, MessageBoxIcon.Question);
                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                string DisplayString = "不能打开"+comName+"！\r\n";
                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                OutMsg(MonitorText, DisplayString, Color.Red);
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

            if (AutoComConnectFlag == 1)
            {
                SerialSetButton.Enabled = false;
            }
            else
            {
                SerialSetButton.Enabled = true;
            }

            for (int i = 0; i < (N * M); i++)
            {
                Xdata[i] = ((i + 1) * 1000 / 128).ToString();
            }
            chart1.Series["Series_Ecg"].Points.DataBindXY(Xdata, XdataV);
            chart1.Series["Series_Ecg"].Color = Color.Black;

            for (int i = 0; i < chart1.Series["Series_Ecg"].Points.Count; i++)
            {
                chart1.Series["Series_Ecg"].Points[i].ToolTip = "#VALX,#VALY";
            }

            textBox1.Text = (chartlenMin * 1000 / 128).ToString();
            textBox2.Text = (chartlenMax * 1000 / 128).ToString();

            numericUpDown1.Value = Ecgamplification;

            ScanUpDataComFlag = 1;

            Thread ScanEcgUartThread = new Thread(new ThreadStart(ScanEcgUartFunction));
            ScanEcgUartThread.Start();
            ScanUpdataThread = new Thread(new ThreadStart(ScanUpdataUartFunction));
            ScanUpdataThread.Start();

            UpDataThread1 = new Thread(new ParameterizedThreadStart(Upgrade2));
            UpDataThread1.Name = "Thread A:";
            UpDataThread2 = new Thread(new ParameterizedThreadStart(Upgrade2));
            UpDataThread2.Name = "Thread B:";
            UpDataThread3 = new Thread(new ParameterizedThreadStart(Upgrade2));
            UpDataThread3.Name = "Thread C:";
            UpDataThread4 = new Thread(new ParameterizedThreadStart(Upgrade2));
            UpDataThread4.Name = "Thread D:";

        }
        #endregion

        private void trackBar1_Scroll(object sender, EventArgs e)
        {
            if (radioButton2.Checked)
            {
                trackBar1.Minimum = chartlenMin + 128;
                chartlenMax = trackBar1.Value;
            }
            else if (radioButton1.Checked)
            {
                trackBar1.Maximum = chartlenMax - 128;
                chartlenMin = trackBar1.Value;
            }
            textBox1.Text = (chartlenMin * 1000 / 128).ToString();
            textBox2.Text = (chartlenMax * 1000 / 128).ToString();
            chart1.ChartAreas["ChartArea_Ecg"].AxisX.Maximum = chartlenMax;
            chart1.ChartAreas["ChartArea_Ecg"].AxisX.Minimum = chartlenMin;
            chart1.ChartAreas["ChartArea_Ecg"].AxisX.Interval = (chartlenMax - chartlenMin) / 4;
        }


        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton1.Checked)
            {
                trackBar1.Minimum = 0;
                trackBar1.Value = chartlenMin;
            }
            trackBar1.Focus();
        }

        private void radioButton2_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton2.Checked)
            {
                trackBar1.Maximum = 512;
                trackBar1.Value = chartlenMax;
            }
            trackBar1.Focus();
        }

        private void SerialSetButton_Click(object sender, EventArgs e)
        {
            if (SerialSetButton.Text == "初始化串口")
            {
                if (ComInit(serialPort1))
                {
                    System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                    string DisplayString = "正在连接" + comboBoxCom.Text + "串口...\r\n";
                    DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                    OutMsg(MonitorText, DisplayString, Color.Red);

                    comboBoxCom.Enabled = false;
                    comboBoxPortel.Enabled = false;
                    SerialSetButton.Text = "关闭串口";

                    PauseFlag = 0;
                    PauseButton.Text = "运行中";

                    BLEConnectFlag = 1;  //假设设备正在通讯

                    serialcommand.SendConnectSerialCommand(serialPort1);  //发生连接central端命令  

                }
            }
            else
            {
                checkBox1.Checked = false;

                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                string DisplayString = "串口已断开！\r\n";
                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                OutMsg(MonitorText, DisplayString, Color.Red);
                SerialSetButton.Text = "初始化串口";

                Vbat = 0;

                comboBoxCom.Enabled = true;
                comboBoxPortel.Enabled = true;
                this.toolStripStatusLabel1.Text = comName + "已经关闭！";
                if (serialPort1.IsOpen)
                {
                    serialPort1.Close();
                    System.Threading.Thread.Sleep(500);
                }
                ComConnectFlag = 0;
                DataStoreButton.Enabled = false;
                ScanButton.Enabled = false;
                ConnectBLEButton.Enabled = false;
                button6.Enabled = false;
                IDValuebutton.Enabled = false;
                button5.Enabled = false;
                button1.Enabled = false;
                checkBox1.Enabled = false;


            }
            for (int i = 0; i < N * M; i++)
            {
                XdataV[i] = ZeroValue;
            }
            chart1.Series["Series_Ecg"].Points.DataBindXY(Xdata, XdataV);
            chart1.Series["Series_Ecg"].Color = Color.Black;

            this.EcgPatchVersionLabel.Text = "";
            this.toolStripStatusLabel2.Text = "";
            EcgPatchVersionLabel.Text = "";
            toolStripStatusLabel5.Text = "";
            toolStripStatusLabel6.Text = "";
            toolStripStatusLabel7.Text = "";
            toolStripStatusLabel8.Text = "";
            BatteryValue.Text = "";
            Lead.Text = "";
        }

        #region 定时器处理函数
        unsafe private void timer1_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
        {
            int EcgMaxValue, EcgMinValue;

            if (!serialPort1.IsOpen)
            {
                return;
            }

            if (SerialReceiveData.Count == 10)
            {
                if (SerialReceiveData[0] == 0x5A && (SerialReceiveData[1] == 0xA7))
                {
                    System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                    string DisplayString = ConnectUartName[0] + "连接成功，准备升级！\r\n";
                    DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                    OutMsg(MonitorText, DisplayString, Color.Red);

                    if (checkBox3.Text == "未连接")
                    {
                        checkBox3.Text = comboBoxCom.Text + "已连接";
                        checkBox3.Checked = true;
                    }

                    ComConnectFlag = 1;
                    UpDataButton.Enabled = true;
                    ComConnectTimeOut[0] = 0;
                    if (ConnectUartNum < 1)
                    {
                        ConnectUartNum = 1;
                    }
                }
            }

            else if (UpgradeNum == 2)
            {
                if (SerialReceiveData.Count > 0)
                    SerialReceiveData[0] = 0;
                return;
            }


            if (DebugMode == 2)
            {
                while (SerialReceiveData.Count >= 1 && (SerialReceiveData[0] != 0x77))
                {
                    SerialReceiveData.RemoveAt(0);
                }
            }

            while (SerialReceiveData.Count >= 4 && ((SerialReceiveData[3] + 4) <= SerialReceiveData.Count))
            {
                #region
                if (DebugMode == 1)
                {
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
                                // TGAP_GEN_DISC_SCANCommand(1000);

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
                                connhandle = SerialReceiveData[13] + (SerialReceiveData[14] << 8);
                            }
                            else if ((SerialReceiveData[3] == 0x06) && (SerialReceiveData[4] == 0x06))
                            {
                                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                string DisplayString = "GAP_TerminateLinkRequest！\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);
                                ConnectBLEButton.Text = "设备已断开";
                                ConnectBLEButton.Enabled = true;

                                button6.Enabled = false;   //定标按键

                                this.toolStripStatusLabel2.Text = "蓝牙设备状态：未连接";

                                DataStoreButton.Enabled = false;

                                ScanButton.Enabled = true;

                                DisplayString = "设备已断开！\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);

                                for (int i = 0; i < N * M; i++)
                                {
                                    XdataV[i] = ZeroValue;
                                }
                                chart1.Series["Series_Ecg"].Points.DataBindXY(Xdata, XdataV);
                                chart1.Series["Series_Ecg"].Color = Color.Black;

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
                                    LeadOffStatus = Convert.ToInt16(SerialReceiveData[12]);
                                    Vbat = Convert.ToInt16(SerialReceiveData[14] << 8) + Convert.ToInt16(SerialReceiveData[13]);
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
                #endregion
                else if (DebugMode == 2)
                {
                    if (SerialReceiveData[0] == 0x77)   //type (command)
                    {
                        if (SerialReceiveData[2] == 0x00)  //状态正常
                        {
                            #region 串口连接成功
                            if (SerialReceiveData[1] == 0x02)
                            {
                                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                string DisplayString = "串口连接成功！\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);

                                ComConnectFlag = 1;

                                serialcommand.DisconnectBLESerialCommand(serialPort1);
                                System.Threading.Thread.Sleep(200);

                                serialcommand.StopReceiveECGDataSerialCommand(serialPort1);

                                converter = new System.Text.ASCIIEncoding();
                                DisplayString = "请求停止接收心电数据...\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                System.Threading.Thread.Sleep(200);

                                serialcommand.DisAutoConnectBLESerialCommand(serialPort1);

                                converter = new System.Text.ASCIIEncoding();
                                DisplayString = "请求关闭自动连接配置...\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);
                                System.Threading.Thread.Sleep(200);

                                serialcommand.AutoConnectBLEStatusSerialCommand(serialPort1);

                                converter = new System.Text.ASCIIEncoding();
                                DisplayString = "获取自动连接配置状态...\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);
                            }
                            #endregion

                            #region 正在搜索设备！
                            else if (SerialReceiveData[1] == 0x0E)
                            {
                                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                string DisplayString = "正在搜索设备！\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);
                                ScanBLENum = 0;
                                MACComboBox.Items.Clear();
                            }
                            #endregion
                            #region
                            else if (SerialReceiveData[1] == 0x04)
                            {
                                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                string DisplayString = "搜索到一个心电设备！\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);
                                int[] macbuffer = new int[6];
                                for (int j = 0; j < 6; j++)
                                {
                                    macbuffer[j] = SerialReceiveData[4 + j];
                                    ScanBLEMAC[ScanBLENum, j] = SerialReceiveData[4 + j];
                                }

                                if ((SerialReceiveData.Count - 0x10) >= 11)
                                {
                                    for (int j = 0; j < 11; j++)
                                    {
                                        ScanBleName[ScanBLENum, j] = SerialReceiveData[0x10 + j];
                                    }
                                }
                                for (int j = 0; j < 11; j++)
                                {
                                    if (ScanBleName[ScanBLENum, j] == 0)
                                    {
                                        ScanBleName[ScanBLENum, j] = 0x20;
                                    }
                                }
                                byte[] m_blename = new byte[11];
                                for (int j = 0; j < 11; j++)
                                {
                                    m_blename[j] = ScanBleName[ScanBLENum, j];
                                }
                                string aa = Encoding.UTF8.GetString(m_blename);
                                if (ScanBleName[ScanBLENum, 0] == 'B' && (ScanBleName[ScanBLENum, 1] == 'D'))
                                {
                                    MACComboBox.Items.Add("(" + macbuffer[5].ToString("X2") + ":" + macbuffer[4].ToString("X2") + ":" + macbuffer[3].ToString("X2") + ":" + macbuffer[2].ToString("X2") + ":" + macbuffer[1].ToString("X2") + ":" + macbuffer[0].ToString("X2") + ")" + aa + "(心电补丁)");

                                }
                                else if (ScanBleName[ScanBLENum, 0] == 'B' && (ScanBleName[ScanBLENum, 1] == 'G'))
                                {
                                    MACComboBox.Items.Add("(" + macbuffer[5].ToString("X2") + ":" + macbuffer[4].ToString("X2") + ":" + macbuffer[3].ToString("X2") + ":" + macbuffer[2].ToString("X2") + ":" + macbuffer[1].ToString("X2") + ":" + macbuffer[0].ToString("X2") + ")" + aa + "(血压计)");
                                }
                                else
                                {
                                    MACComboBox.Items.Add("(" + macbuffer[5].ToString("X2") + ":" + macbuffer[4].ToString("X2") + ":" + macbuffer[3].ToString("X2") + ":" + macbuffer[2].ToString("X2") + ":" + macbuffer[1].ToString("X2") + ":" + macbuffer[0].ToString("X2") + ")" + aa + "(未知设备)");

                                }
                                ScanBLENum++;
                                ConnectBLEButton.Enabled = true;
                                button6.Enabled = true;
                                ConnectBLEButton.Text = "设备已断开";
                                MACComboBox.SelectedIndex = 0;
                                ScanButton.Enabled = true;
                                MACComboBox.Enabled = true;
                            }
                            #endregion
                            #region
                            else if (SerialReceiveData[1] == 0x2E)
                            {
                                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                string DisplayString = "搜索结束！\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);

                                button6.Enabled = true;   //配对按钮
                                ScanButton.Enabled = true;
                            }
                            #endregion
                            #region
                            else if (SerialReceiveData[1] == 0x06)
                            {
                                if (SerialReceiveData[4] == 1)
                                {
                                    System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                    string DisplayString = "心电补丁连接成功！\r\n";
                                    DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                    OutMsg(MonitorText, DisplayString, Color.Red);

                                    BLEConnectFlag = 1;

                                    checkBox1.Enabled = false;

                                    DataStoreButton.Enabled = true;

                                    button6.Enabled = false;
                                    IDValuebutton.Enabled = true;
                                    button5.Enabled = true;
                                    button1.Enabled = true;
                                    PauseButton.Enabled = true;
                                    StartEcgCaptureFlag = 0;
                                    string filePath = System.IO.Directory.GetCurrentDirectory() + "//ecg_data.hex";
                                    File.Delete(filePath);
                                }
                                else
                                {
                                    System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                    string DisplayString = "选择的设备与绑定的mac不符，无法连接！\r\n";
                                    DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                    OutMsg(MonitorText, DisplayString, Color.Red);
                                }
                            }
                            #endregion
                            #region
                            else if (SerialReceiveData[1] == 0x0B)
                            {
                                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                string DisplayString = "设备已断开！\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);

                                ConnectBLEButton.Text = "设备已断开";
                                ConnectBLEButton.Enabled = true;
                                ScanButton.Enabled = true;
                                DataStoreButton.Enabled = false;

                                checkBox1.Enabled = true;

                                if (DisConnectBLEEnableFlag == 0)
                                {
                                    MACComboBox.Items.Clear();
                                    MACComboBox.Items.Add("无设备");
                                    MACComboBox.SelectedIndex = 0;
                                    ConnectBLEButton.Enabled = false;
                                }

                                for (int i = 0; i < N * M; i++)
                                {
                                    XdataV[i] = ZeroValue;
                                }
                                chart1.Series["Series_Ecg"].Points.DataBindXY(Xdata, XdataV);
                                chart1.Series["Series_Ecg"].Color = Color.Black;

                                BLEConnectFlag = 0;

                                this.toolStripStatusLabel2.Text = "主蓝牙设备MAC：" + BLECentralMAC[5].ToString("X2") + ":" + BLECentralMAC[4].ToString("X2")
                                    + ":" + BLECentralMAC[3].ToString("X2") + ":" + BLECentralMAC[2].ToString("X2") + ":" + BLECentralMAC[1].ToString("X2") + ":" + BLECentralMAC[0].ToString("X2")

                                    + "  连接状态：心电补丁未连接" + "   配对心电补丁MAC：" + ECGPairMAC[5].ToString("X2") + ":" + ECGPairMAC[4].ToString("X2") +
                                    ":" + ECGPairMAC[3].ToString("X2") + ":" + ECGPairMAC[2].ToString("X2") +
                                    ":" + ECGPairMAC[1].ToString("X2") + ":" + ECGPairMAC[0].ToString("X2");

                                this.EcgPatchVersionLabel.Text = "";
                                AmplificationValue.Text = "未定标或无法获取定标值";
                                differenceValue.Text = "未定标或无法获取定标值";

                                this.EcgPatchVersionLabel.Text = "";
                                EcgPatchVersionLabel.Text = "";
                                toolStripStatusLabel5.Text = "";
                                toolStripStatusLabel6.Text = "";
                                toolStripStatusLabel7.Text = "";
                                toolStripStatusLabel8.Text = "";
                                BatteryValue.Text = "";
                                Lead.Text = "";

                                IDValuebutton.Enabled = false;
                                button5.Enabled = false;
                                button1.Enabled = false;
                                button6.Enabled = true;
                                PauseButton.Enabled = false;

                                StartEcgCaptureFlag = 0;

                            }
                            #endregion
                            #region
                            else if (SerialReceiveData[1] == 0x12)
                            {
                                if (SerialReceiveData[4] == 0x01)
                                {
                                    System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                    string DisplayString = "自动连接配置完成！\r\n";
                                    DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                    OutMsg(MonitorText, DisplayString, Color.Red);
                                    checkBox1.Checked = true;
                                }
                                else if (SerialReceiveData[4] == 0x00)
                                {
                                    System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                    string DisplayString = "停止自动连接！\r\n";
                                    DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                    OutMsg(MonitorText, DisplayString, Color.Red);
                                    checkBox1.Checked = false;
                                }

                            }
                            #endregion
                            #region
                            else if (SerialReceiveData[1] == 0x13)
                            {
                                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                string DisplayString = "接收一次RSSI值！\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);
                                RSSIValue = SerialReceiveData[4] - 256;
                                ConnectBLEButton.Text = "设备已连接";
                                ConnectBLEButton.Enabled = true;
                                button6.Enabled = true;   //定标按键

                            }
                            #endregion
                            #region
                            else if (SerialReceiveData[1] == 0x17)
                            {
                                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                string DisplayString = "设置停止接收心电数据成功！\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);
                                for (int i = 0; i < N * M; i++)
                                {
                                    XdataV[i] = ZeroValue;
                                }
                                chart1.Series["Series_Ecg"].Points.DataBindXY(Xdata, XdataV);
                                chart1.Series["Series_Ecg"].Color = Color.Black;

                            }
                            #endregion
                            #region
                            else if (SerialReceiveData[1] == 0x1B)
                            {
                                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                string DisplayString = "获取自动搜索连接状态成功！\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);
                                if (SerialReceiveData[4] == 1)
                                {
                                    checkBox1.Checked = true;
                                }
                                else if (SerialReceiveData[4] == 0)
                                {
                                    checkBox1.Checked = false;
                                }
                                serialcommand.SetTestModeCommand(serialPort1);
                                converter = new System.Text.ASCIIEncoding();
                                DisplayString = "设置为测试模式\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);

                            }
                            #endregion
                            #region
                            else if (SerialReceiveData[1] == 0x25)
                            {
                                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                string DisplayString = "设置测试模式成功！\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);
                                serialcommand.ReceiveCentralMACCommand(serialPort1);
                                converter = new System.Text.ASCIIEncoding();
                                DisplayString = "请求获取主设备蓝牙MAC\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);
                            }
                            #endregion
                            #region
                            else if (SerialReceiveData[1] == 0x1D)
                            {
                                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                string DisplayString = "获取主设备蓝牙MAC成功！\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);
                                for (int i = 0; i < 6; i++)
                                {
                                    BLECentralMAC[i] = SerialReceiveData[4 + i];
                                }
                                serialcommand.ReceivePairingStatusCCommand(serialPort1);
                                converter = new System.Text.ASCIIEncoding();
                                DisplayString = "请求获取配对状态\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);

                            }
                            #endregion
                            #region
                            else if (SerialReceiveData[1] == 0x23)
                            {
                                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                string DisplayString = "获取配对状态成功！\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);

                                textBox5.Text = SerialReceiveData[9].ToString("X2") + ":" + SerialReceiveData[8].ToString("X2") + ":" + SerialReceiveData[7].ToString("X2")
                                       + ":" + SerialReceiveData[6].ToString("X2") + ":" + SerialReceiveData[5].ToString("X2") + ":" + SerialReceiveData[4].ToString("X2");
                                for (int i = 0; i < 6; i++)
                                {
                                    ECGPairMAC[i] = SerialReceiveData[4 + i];
                                }

                                this.toolStripStatusLabel2.Text = "主蓝牙设备MAC：" + BLECentralMAC[5].ToString("X2") + ":" + BLECentralMAC[4].ToString("X2")
                                    + ":" + BLECentralMAC[3].ToString("X2") + ":" + BLECentralMAC[2].ToString("X2") + ":" + BLECentralMAC[1].ToString("X2") + ":" + BLECentralMAC[0].ToString("X2")

                                    + "  连接状态：心电补丁未连接" + "   配对心电补丁MAC：" + ECGPairMAC[5].ToString("X2") + ":" + ECGPairMAC[4].ToString("X2") +
                                    ":" + ECGPairMAC[3].ToString("X2") + ":" + ECGPairMAC[2].ToString("X2") +
                                    ":" + ECGPairMAC[1].ToString("X2") + ":" + ECGPairMAC[0].ToString("X2");
                                serialcommand.ReceiveECGPatchMACCommand(serialPort1);
                                converter = new System.Text.ASCIIEncoding();
                                DisplayString = "请求获取心电补丁蓝牙MAC\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);
                            }
                            #endregion
                            #region
                            else if (SerialReceiveData[1] == 0x1F)
                            {
                                if (SerialReceiveData[4] == 0 && (SerialReceiveData[5] == 0) && (SerialReceiveData[6] == 0) && (SerialReceiveData[7] == 0))
                                {
                                    System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                    string DisplayString = "蓝牙心电补丁未连接，无法获取MAC\r\n";
                                    DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                    OutMsg(MonitorText, DisplayString, Color.Red);
                                    ScanButton.Enabled = true;
                                }
                                else
                                {
                                    System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                    string DisplayString = "获取心电补丁MAC成功！\r\n";
                                    DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                    OutMsg(MonitorText, DisplayString, Color.Red);
                                    for (int i = 0; i < 6; i++)
                                    {
                                        ECGPatchMAC[i] = SerialReceiveData[4 + i];
                                    }
                                    serialcommand.ReceiveECGPatchIDSerialCommand(serialPort1);
                                    converter = new System.Text.ASCIIEncoding();
                                    DisplayString = "请求接收心电补丁ID\r\n";
                                    DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                    OutMsg(MonitorText, DisplayString, Color.Red);
                                }

                            }
                            #endregion

                            #region
                            else if (SerialReceiveData[1] == 0x19)
                            {
                                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                string DisplayString = "心电补丁ID获取成功！\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);
                                for (int i = 0; i < SerialReceiveData[3]; i++)
                                {
                                    ECGPatchID[i] = SerialReceiveData[4 + i];
                                }

                                this.toolStripStatusLabel2.Text = "主蓝牙设备MAC：" + BLECentralMAC[5].ToString("X2") + ":" + BLECentralMAC[4].ToString("X2")
                                    + ":" + BLECentralMAC[3].ToString("X2") + ":" + BLECentralMAC[2].ToString("X2") + ":" + BLECentralMAC[1].ToString("X2") + ":" + BLECentralMAC[0].ToString("X2")

                                    + "  连接状态：心电补丁未连接" + "   配对心电补丁MAC：" + ECGPairMAC[5].ToString("X2") + ":" + ECGPairMAC[4].ToString("X2") +
                                    ":" + ECGPairMAC[3].ToString("X2") + ":" + ECGPairMAC[2].ToString("X2") +
                                    ":" + ECGPairMAC[1].ToString("X2") + ":" + ECGPairMAC[0].ToString("X2");
                                serialcommand.calibration0mvSerialCommand(serialPort1);
                                converter = new System.Text.ASCIIEncoding();
                                DisplayString = "请求获取0mv校准值...\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);
                            }
                            #endregion
                            #region
                            else if (SerialReceiveData[1] == 0x15)
                            {
                                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                if (SerialReceiveData[4] == 0)
                                {
                                    string DisplayString = "设备未连接,无法设置成功！\r\n";
                                    DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                    OutMsg(MonitorText, DisplayString, Color.Red);
                                }
                                else if (SerialReceiveData[4] == 1)
                                {
                                    string DisplayString = "设置接收心电数据成功！\r\n";
                                    DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                    OutMsg(MonitorText, DisplayString, Color.Red);
                                    EcgDataTimer = 0;

                                    LostPackageNum = 0;
                                    ReceivePackageNum = 0;

                                    LastPackageNum = -1;
                                    this.toolStripStatusLabel5.Text = "上位机丢包数：" + LostPackageNum.ToString();
                                    this.toolStripStatusLabel6.Text = "上位机接收包：" + ReceivePackageNum.ToString();

                                    StartEcgCaptureFlag = 1;
                                }


                            }
                            #endregion
                            #region
                            else if (SerialReceiveData[1] == 0x2B)
                            {
                                string DisplayString = "获取0mv校准值成功！\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);

                                difference_Value = (Int16)(SerialReceiveData[4] + (SerialReceiveData[5] << 8));
                                if (difference_Value <= 0 || (difference_Value > 1000))
                                {
                                    difference_Value = 500;
                                }
                                serialcommand.calibration1mvSerialCommand(serialPort1);
                                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                DisplayString = "请求获取1mv定标值...\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);
                            }
                            #endregion
                            #region
                            else if (SerialReceiveData[1] == 0x27)
                            {
                                string DisplayString = "获取1mv定标值成功！\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);

                                amplification = (Int16)(SerialReceiveData[4] + (SerialReceiveData[5] << 8));
                                if (amplification <= 0 || (amplification > 100))
                                {
                                    amplification = 60;
                                }
                                serialcommand.GetHardWareVersionSerialCommand(serialPort1);
                                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                DisplayString = "请求获取硬件版本...\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);
                            }
                            #endregion
                            #region
                            else if (SerialReceiveData[1] == 0x31)
                            {
                                string DisplayString = "获取硬件版本成功！\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);

                                for (int i = 0; i < 5; i++)
                                {
                                    HardVersion[i] = SerialReceiveData[4 + i];
                                }
                                serialcommand.GetSoftWareVersionSerialCommand(serialPort1);
                                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                DisplayString = "请求获取软件版本...\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);
                            }
                            #endregion
                            #region
                            else if (SerialReceiveData[1] == 0x33)
                            {
                                string DisplayString = "获取软件版本成功！\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);

                                for (int i = 0; i < 12; i++)
                                {
                                    SoftVersion[i] = SerialReceiveData[4 + i];
                                }
                                serialcommand.ReceiveECGDataSerialCommand(serialPort1);
                                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                DisplayString = "请求接收心电数据...\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);
                            }
                            #endregion
                            #region
                            else if (SerialReceiveData[1] == 0x2D)
                            {
                                if (SerialReceiveData[4] == 1)
                                {
                                    string DisplayString = "设置0mv校准值成功！\r\n";
                                    DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                    OutMsg(MonitorText, DisplayString, Color.Red);
                                    serialcommand.calibration1mvSerialCommand(serialPort1);
                                    System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                    DisplayString = "请求获取1mv定标值...\r\n";
                                    DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                    OutMsg(MonitorText, DisplayString, Color.Red);
                                }
                            }
                            #endregion
                            #region
                            else if (SerialReceiveData[1] == 0x29)
                            {
                                if (SerialReceiveData[4] == 1)
                                {
                                    string DisplayString = "设置1mv定标值成功！\r\n";
                                    DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                    OutMsg(MonitorText, DisplayString, Color.Red);
                                }
                            }
                            #endregion

                            #region
                            else if (SerialReceiveData[1] == 0x35)
                            {
                                // if (SerialReceiveData[4] == 1)
                                {
                                    for (int i = 0; i < 10; i++)
                                    {
                                        ECGPatchID[i] = SerialReceiveData[4 + i];
                                    }
                                    string DisplayString = "设置心电补丁ID值成功！\r\n";
                                    DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                    OutMsg(MonitorText, DisplayString, Color.Red);
                                }
                            }
                            #endregion
                            #region
                            else if (SerialReceiveData[1] == 0x21)
                            {
                                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                string DisplayString = "设置配对MAC地址成功！\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);

                                textBox5.Text = SerialReceiveData[9].ToString("X2") + ":" + SerialReceiveData[8].ToString("X2") + ":" + SerialReceiveData[7].ToString("X2")
                                      + ":" + SerialReceiveData[6].ToString("X2") + ":" + SerialReceiveData[5].ToString("X2") + ":" + SerialReceiveData[4].ToString("X2");
                                for (int i = 0; i < 6; i++)
                                {
                                    ECGPairMAC[i] = SerialReceiveData[4 + i];
                                }

                                this.toolStripStatusLabel2.Text = "主蓝牙设备MAC：" + BLECentralMAC[5].ToString("X2") + ":" + BLECentralMAC[4].ToString("X2")
                                    + ":" + BLECentralMAC[3].ToString("X2") + ":" + BLECentralMAC[2].ToString("X2") + ":" + BLECentralMAC[1].ToString("X2") + ":" + BLECentralMAC[0].ToString("X2")

                                    + "  连接状态：心电补丁未连接" + "   配对心电补丁MAC：" + ECGPairMAC[5].ToString("X2") + ":" + ECGPairMAC[4].ToString("X2") +
                                    ":" + ECGPairMAC[3].ToString("X2") + ":" + ECGPairMAC[2].ToString("X2") +
                                    ":" + ECGPairMAC[1].ToString("X2") + ":" + ECGPairMAC[0].ToString("X2");

                            }
                            #endregion

                            #region
                            else if (SerialReceiveData[1] == 0x36)
                            {
                                string DisplayString = "正在接收主设备接收心电包数...\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);

                                double CentralReceivePackage = SerialReceiveData[4] + (SerialReceiveData[5] << 8) + (SerialReceiveData[6] << 16) + (SerialReceiveData[7] << 24);

                                this.toolStripStatusLabel8.Text = "主设备接收包：" + CentralReceivePackage.ToString();

                                double CentralLostPackage = SerialReceiveData[8] + (SerialReceiveData[9] << 8) + (SerialReceiveData[10] << 16) + (SerialReceiveData[11] << 24);

                                this.toolStripStatusLabel7.Text = "主设备丢包：" + CentralLostPackage.ToString();
                            }
                            #endregion

                            #region
                            else if (SerialReceiveData[1] == 0x09)
                            {
                                if (StartEcgCaptureFlag == 1)
                                {
                                    EcgReceiveFlagTimer++;
                                    if (EcgReceiveFlagTimer >= 20)
                                    {
                                        EcgReceiveFlagTimer = 0;
                                        System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                        string DisplayString = "正在接收心电数据！\r\n";
                                        DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                        OutMsg(MonitorText, DisplayString, Color.Red);
                                        if (ScalingFlag == 1)
                                        {
                                            System.Text.ASCIIEncoding converter2 = new System.Text.ASCIIEncoding();
                                            string DisplayString2 = "正在1mv定标...\r\n";
                                            DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString2;
                                            OutMsg(MonitorText, DisplayString, Color.Red);
                                            AmplificationValue.Text = "正在1mv定标...";
                                            differenceValue.Text = "正在1mv定标...";
                                        }
                                        else if (ScalingFlag == 11)
                                        {
                                            System.Text.ASCIIEncoding converter2 = new System.Text.ASCIIEncoding();
                                            string DisplayString3 = "正在0mv校准...\r\n";
                                            DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString3;
                                            OutMsg(MonitorText, DisplayString, Color.Red);
                                            AmplificationValue.Text = "正在0mv校准...";
                                            differenceValue.Text = "正在0mv校准...";
                                        }
                                        else
                                        {
                                            AmplificationValue.Text = "获取的1mv定标值为：" + Convert.ToString(amplification);
                                            differenceValue.Text = "获取的0mv校准值为：" + Convert.ToString(difference_Value);
                                        }
                                    }
                                    if (LastPackageNum != -1)  //不是第一次  
                                    {
                                        int buffer = LastPackageNum + 1;
                                        if (buffer == 256)
                                        {
                                            buffer = 0;
                                        }
                                        if (SerialReceiveData[4] != buffer)
                                        {
                                            LostPackageNum++;
                                        }
                                    }
                                    LastPackageNum = SerialReceiveData[4];
                                    this.toolStripStatusLabel5.Text = "上位机丢包数：" + LostPackageNum.ToString();

                                    ReceivePackageNum++;
                                    this.toolStripStatusLabel6.Text = "上位机接收包：" + ReceivePackageNum.ToString();

                                    if (difference_Value < 0 || (difference_Value > 1000))
                                    {
                                        difference_Value = 500;
                                    }
                                    if (amplification < 0 || (amplification > 1000))
                                    {
                                        amplification = 60;
                                    }

                                    XdataV[EcgDataTimer * M + 0] = (Convert.ToDouble(SerialReceiveData[6]) + Convert.ToDouble((SerialReceiveData[10] & 0xc0) << 2) - difference_Value) * 1000 / amplification + BaseLine;
                                    XdataV[EcgDataTimer * M + 1] = (Convert.ToDouble(SerialReceiveData[7]) + Convert.ToDouble((SerialReceiveData[10] & 0x30) << 4) - difference_Value) * 1000 / amplification + BaseLine;
                                    XdataV[EcgDataTimer * M + 2] = (Convert.ToDouble(SerialReceiveData[8]) + Convert.ToDouble((SerialReceiveData[10] & 0x0c) << 6) - difference_Value) * 1000 / amplification + BaseLine;
                                    XdataV[EcgDataTimer * M + 3] = (Convert.ToDouble(SerialReceiveData[9]) + Convert.ToDouble((SerialReceiveData[10] & 0x03) << 8) - difference_Value) * 1000 / amplification + BaseLine;
                                    XdataV[EcgDataTimer * M + 4] = (Convert.ToDouble(SerialReceiveData[11]) + Convert.ToDouble((SerialReceiveData[15] & 0xc0) << 2) - difference_Value) * 1000 / amplification + BaseLine;
                                    XdataV[EcgDataTimer * M + 5] = (Convert.ToDouble(SerialReceiveData[12]) + Convert.ToDouble((SerialReceiveData[15] & 0x30) << 4) - difference_Value) * 1000 / amplification + BaseLine;
                                    XdataV[EcgDataTimer * M + 6] = (Convert.ToDouble(SerialReceiveData[13]) + Convert.ToDouble((SerialReceiveData[15] & 0x0c) << 6) - difference_Value) * 1000 / amplification + BaseLine;
                                    XdataV[EcgDataTimer * M + 7] = (Convert.ToDouble(SerialReceiveData[14]) + Convert.ToDouble((SerialReceiveData[15] & 0x03) << 8) - difference_Value) * 1000 / amplification + BaseLine;

                                    FileStream fs = null;
                                    string filePath = System.IO.Directory.GetCurrentDirectory() + "//ecg_data.hex";
                                    try
                                    {
                                        fs = File.OpenWrite(filePath);
                                        fs.Position = fs.Length;
                                        for (int i = 0; i < 8; i++)
                                        {
                                            int j = 0;
                                            byte[] bytes = new byte[10];
                                            short m_ecgdata = (short)XdataV[EcgDataTimer * M + i];
                                            bytes[0] = (byte)m_ecgdata;
                                            bytes[1] = (byte)(m_ecgdata / 256);
                                            fs.Write(bytes, 0, 2);
                                        }

                                    }
                                    catch (Exception ex)
                                    {
                                        Console.WriteLine("鏂囦欢鎵撳紑澶辫触{0}", ex.ToString());
                                    }
                                    finally
                                    {
                                        fs.Close();
                                    }
                                    DataStoreButton.Enabled = true;

                                    if ((ScalingFlag == 1) || (ScalingFlag == 11))   //校准或定标
                                    {
                                        calibration_Value.Add(XdataV[EcgDataTimer * M + 0]);
                                        calibration_Value.Add(XdataV[EcgDataTimer * M + 1]);
                                        calibration_Value.Add(XdataV[EcgDataTimer * M + 2]);
                                        calibration_Value.Add(XdataV[EcgDataTimer * M + 3]);
                                        calibration_Value.Add(XdataV[EcgDataTimer * M + 4]);
                                        calibration_Value.Add(XdataV[EcgDataTimer * M + 5]);
                                        calibration_Value.Add(XdataV[EcgDataTimer * M + 6]);
                                        calibration_Value.Add(XdataV[EcgDataTimer * M + 7]);
                                        calibration_Num += 8;
                                        if (calibration_Num >= 1024)
                                        {
                                            if (ScalingFlag == 1)
                                            {
                                                for (int i = 0; i < 8; i++)
                                                {
                                                    ScalingEcgMin[i] = 50000;
                                                }
                                                for (int i = 0; i < 8; i++)
                                                {
                                                    ScalingEcgMax[i] = -50000;
                                                }
                                                for (int j = 0; j < 8; j++)
                                                {
                                                    for (int i = 200; i < 1024; i++)
                                                    {
                                                        if (ScalingEcgMin[j] > calibration_Value[i])
                                                        {
                                                            if (j > 0)
                                                            {
                                                                if (ScalingEcgMin[j - 1] < calibration_Value[i])
                                                                {
                                                                    ScalingEcgMin[j] = calibration_Value[i];
                                                                }
                                                            }
                                                            else
                                                            {
                                                                ScalingEcgMin[j] = calibration_Value[i];
                                                            }
                                                        }
                                                        if (ScalingEcgMax[j] < calibration_Value[i])
                                                        {
                                                            if (j > 0)
                                                            {
                                                                if (ScalingEcgMax[j - 1] > calibration_Value[i])
                                                                {
                                                                    ScalingEcgMax[j] = calibration_Value[i];
                                                                }
                                                            }
                                                            else
                                                            {
                                                                ScalingEcgMax[j] = calibration_Value[i];
                                                            }

                                                        }
                                                    }
                                                }
                                                for (int i = 1; i < 4; i++)
                                                {
                                                    ScalingEcgMax[0] += ScalingEcgMax[i];
                                                    ScalingEcgMin[0] += ScalingEcgMin[i];
                                                }
                                                amplification = (Int16)((ScalingEcgMax[0] - ScalingEcgMin[0]) / 4);
                                                difference_Value = difference_Value_Back;

                                                calibration_Value.RemoveRange(0, 1024);
                                                serialcommand.Setcalibration1mvSerialCommand(serialPort1, amplification);
                                                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                                string DisplayString = "保存1mv定标值...\r\n";
                                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                                OutMsg(MonitorText, DisplayString, Color.Red);
                                                ScalingFlag = 2;   //完成1mv定标
                                                calibration_Num = 0;
                                            }
                                            else if (ScalingFlag == 11)
                                            {
                                                double m_value = 0;
                                                for (int i = 0; i < 1024; i++)
                                                {
                                                    m_value += calibration_Value[i];
                                                }
                                                difference_Value = (Int16)(m_value / 1024);
                                                amplification = amplification_Back;
                                                calibration_Value.RemoveRange(0, 1024);
                                                serialcommand.Setcalibration0mvSerialCommand(serialPort1, difference_Value);
                                                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                                string DisplayString = "保存0mv校准值...\r\n";
                                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                                OutMsg(MonitorText, DisplayString, Color.Red);
                                                ScalingFlag = 12;   //完成0mv校准
                                                calibration_Num = 0;
                                            }
                                            // BaseLine = 500;
                                        }
                                    }

                                    EcgDataTimer++;
                                    if (EcgDataTimer >= (chartlenMax / M))
                                    {
                                        EcgDataTimer = (chartlenMin / 8);
                                    }

                                    DataTransmissionFlag = 1;

                                    LeadOffStatus = Convert.ToInt16(SerialReceiveData[5]) >> 7;
                                    Vbat = Convert.ToInt16(SerialReceiveData[5] & 0x7f);
                                }
                            }
                            #endregion

                            SerialReceiveData.RemoveRange(0, SerialReceiveData[3] + 4);//从接收列表中删除包
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
            }


            if (BLEConnectFlag == 1)
            {
                if (DataTransmissionFlag == 1)
                {
                    DataTransmissionFlag = 0;
                    ConnectBLEButton.Text = "设备已连接";
                    ConnectBLEButton.Enabled = true;
                    button6.Enabled = false;   //定标按键
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

                    if (PauseButton.Text == "运行中")
                    {
                        string str = System.Text.Encoding.Default.GetString(ECGPatchID);
                        string str2;
                        if (ScalingFlag == 0)
                        {
                            if ((difference_Value != -1) && (calibration_Num != -1))
                            {
                                str2 = "已定标    ";
                            }
                            else
                            {
                                str2 = "未定标    ";
                            }
                        }
                        else if (ScalingFlag == 1)
                        {
                            str2 = "正在1mv定标...";
                        }
                        else if (ScalingFlag == 2)
                        {
                            str2 = "1mv已定标！";
                            ScalingFlag = 0;
                        }
                        else if (ScalingFlag == 11)
                        {
                            str2 = "正在0mv校准...";
                        }
                        else if (ScalingFlag == 12)
                        {
                            str2 = "0mv已校准！";
                            ScalingFlag = 0;
                        }
                        else
                        {
                            str2 = "定标状态未知";
                        }
                        string str3;
                        if (PairingFlag == 0)
                        {
                            str3 = "未配对";

                        }
                        else
                        {
                            str3 = "配对MAC：" + ECGPairMAC[5].ToString("X2") + ":" + ECGPairMAC[4].ToString("X2") +
                                    ":" + ECGPairMAC[3].ToString("X2") + ":" + ECGPairMAC[2].ToString("X2") +
                                    ":" + ECGPairMAC[1].ToString("X2") + ":" + ECGPairMAC[0].ToString("X2");
                        }
                        this.toolStripStatusLabel2.Text = "主蓝牙设备MAC：" + BLECentralMAC[5].ToString("X2") + ":" + BLECentralMAC[4].ToString("X2")
                                    + ":" + BLECentralMAC[3].ToString("X2") + ":" + BLECentralMAC[2].ToString("X2") + ":" + BLECentralMAC[1].ToString("X2") + ":" + BLECentralMAC[0].ToString("X2")
                                    + "  连接状态：心电补丁已连接 MAC:" +
                            ECGPatchMAC[5].ToString("X2") + ":" + ECGPatchMAC[4].ToString("X2") +
                            ":" + ECGPatchMAC[3].ToString("X2") + ":" + ECGPatchMAC[2].ToString("X2")
                            + ":" + ECGPatchMAC[1].ToString("X2") + ":" + ECGPatchMAC[0].ToString("X2")
                            + "  " + "RSSI:" + Convert.ToInt16(RSSIValue) + "  ID: " + str + "  " + str2 + str3;

                        this.EcgPatchVersionLabel.Text = "心电补丁已连接 硬件版本:" + Encoding.UTF8.GetString(HardVersion) + "   软件版本:" + Encoding.UTF8.GetString(SoftVersion);
                        try
                        {
                            chart1.Series["Series_Ecg"].Color = Color.FromArgb(0, 192, 0);
                        }
                        catch (System.Exception)
                        {
                            System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                            string DisplayString = "错误!!!\r\n";
                            DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                            OutMsg(MonitorText, DisplayString, Color.Red);
                        }

                        chart1.Series["Series_Ecg"].Points.DataBindXY(Xdata, XdataV);

                        if (EcgDataTimer < 63 && (EcgDataTimer >= 0))
                        {
                            chart1.Series["Series_Ecg"].Points.ElementAt(EcgDataTimer * M + 2).Color = Color.Black;
                            chart1.Series["Series_Ecg"].Points.ElementAt(EcgDataTimer * M + 3).Color = Color.Black;
                            chart1.Series["Series_Ecg"].Points.ElementAt(EcgDataTimer * M + 4).Color = Color.Black;
                            chart1.Series["Series_Ecg"].Points.ElementAt(EcgDataTimer * M + 5).Color = Color.Black;
                            chart1.Series["Series_Ecg"].Points.ElementAt(EcgDataTimer * M + 6).Color = Color.Black;
                            chart1.Series["Series_Ecg"].Points.ElementAt(EcgDataTimer * M + 7).Color = Color.Black;
                        }
                        for (int i = 1; i < 512; i++)
                        {
                            if ((XdataV[i] > 7000) || (XdataV[i] < -7000))
                            {
                                chart1.Series["Series_Ecg"].Points.ElementAt(i).Color = Color.Black;
                                chart1.Series["Series_Ecg"].Points.ElementAt(i - 1).Color = Color.Black;
                            }
                        }
                    }
                }
            }
            if (ConnectBLEButton.Text == "设备已连接")
            {
                //HeartRate.Text = rate.ToString();
                BatteryValue.SelectAll();
                //  double batteryValue = 3300 *4 * Vbat /1024 ;
                double batteryValue = Vbat;
                BatteryValue.Text = batteryValue.ToString();
                //  BloodPressure.Text = "";
                if ((LeadOffStatus & 0x01) == 0x01)
                {
                    Lead.Text = "连接正常";
                    Lead.SelectAll();
                    Lead.SelectionColor = Color.Green;
                }
                else
                {
                    try
                    {
                        Lead.Text = "导联脱落";
                        Lead.SelectAll();
                        Lead.SelectionColor = Color.Red;
                    }
                    finally
                    {
                        ;
                    }
                }
            }
            else
            {
                BatteryValue.Text = "";
                Lead.Text = "";
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
            saveFileDialog1.Filter = "bin文件(*.hex)|*.hex|文本文件(*.txt)|*.txt";
            saveFileDialog1.Title = "保存心电数据";
            saveFileDialog1.FilterIndex = 1;
            saveFileDialog1.RestoreDirectory = true;
            string FileName = "ecg_data" + "  " + StartStoreDataTime + "--" + DateTime.Now.ToString("yyyy-MM-dd") + "-" + DateTime.Now.Hour.ToString() + "-" + DateTime.Now.Minute.ToString() + ".hex"; // 
            saveFileDialog1.FileName = FileName;
            saveFileDialog1.AddExtension = true;
            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                string filePath = System.IO.Directory.GetCurrentDirectory() + "\\ecg_data.hex";
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
                serialcommand.SendDisAdvertiseSerialCommand(serialPort1);
                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                string DisplayString = "请求搜索设备...\r\n";
                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                OutMsg(MonitorText, DisplayString, Color.Red);
                MACComboBox.Items.Clear();

                ScanButton.Enabled = false;
                ConnectBLEButton.Enabled = false;
                MACComboBox.Enabled = false;
                // BLENUMlabel.Text = "";
                ScanBLENum = 0;
                button6.Enabled = false;
            }
            PauseButton.Enabled = false;
        }
        #endregion

        #region 选择连接设备的MAC
        private void MACComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            ChoiceBLE = (byte)MACComboBox.SelectedIndex;
            textBox5.Text = ScanBLEMAC[ChoiceBLE, 5].ToString("X2") + ":" + ScanBLEMAC[ChoiceBLE, 4].ToString("X2") + ":" + ScanBLEMAC[ChoiceBLE, 3].ToString("X2")
        + ":" + ScanBLEMAC[ChoiceBLE, 2].ToString("X2") + ":" + ScanBLEMAC[ChoiceBLE, 1].ToString("X2") + ":" + ScanBLEMAC[ChoiceBLE, 0].ToString("X2");
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
                checkBox1.Enabled = true;
                serialcommand.DisconnectBLESerialCommand(serialPort1);
                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                string DisplayString = "请求断开设备...\r\n";
                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                OutMsg(MonitorText, DisplayString, Color.Red);
                DisConnectBLEEnableFlag = 1;
                this.toolStripStatusLabel2.Text = "蓝牙设备状态：未连接";
            }
            else
            {
                checkBox1.Enabled = false;

                if (MACComboBox.SelectedIndex < 100)
                {

                    if (ScanBleName[MACComboBox.SelectedIndex, 0] == 'B' && (ScanBleName[MACComboBox.SelectedIndex, 1] == 'D'))
                    {
                        serialcommand.ConnectBLESerialCommand(serialPort1, (byte)MACComboBox.SelectedIndex);
                        System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                        string DisplayString = "请求连接设备...\r\n";
                        DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                        OutMsg(MonitorText, DisplayString, Color.Red);
                        DisConnectBLEEnableFlag = 0;
                    }
                    else
                    {
                        System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                        string DisplayString = "所选设备不是心电补丁，请重新选择连接！！！\r\n";
                        DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                        OutMsg(MonitorText, DisplayString, Color.Red);
                        checkBox1.Enabled = true;
                    }
                }
            }

        }
        #endregion

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
                    XdataV[i] = ZeroValue;
                }
                chart1.Series["Series_Ecg"].Points.DataBindXY(Xdata, XdataV);
                chart1.Series["Series_Ecg"].Color = Color.Black;
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
                        string DisplayString = DateTime.Now.ToLongTimeString() + ": " + "条码扫描：" + textBoxScan.Text.ToString() + "\r\n";
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
                byte[] ssss = new byte[14];
                ssss[0] = 0x77;   //
                ssss[1] = 0x34;   //ID发送命令
                ssss[2] = 0x00;
                ssss[3] = 10;
                char[] charIDValue = textBoxScan.Text.ToCharArray();
                try
                {
                    if (textBoxScan.Text.Length == 10)
                    {
                        for (int i = 0; i < 10; i++)
                        {
                            ssss[4 + i] = (byte)charIDValue[i];
                        }
                        if (serialPort1.IsOpen)
                        {
                            string DisplayString = "请求设置ID！\r\n";
                            DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                            OutMsg(MonitorText, DisplayString, Color.Red);

                            serialPort1.Write(ssss, 0, 14);
                        }
                        else
                        {
                            string DisplayString = "串口未打开！\r\n";
                            DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                            OutMsg(MonitorText, DisplayString, Color.Red);
                        }
                    }
                    else
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
        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox1.Checked == true)
            {
                serialcommand.AutoConnectBLESerialCommand(serialPort1);
                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                string DisplayString = "请求自动连接配置...\r\n";
                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                OutMsg(MonitorText, DisplayString, Color.Red);
                ScanButton.Enabled = false;
            }
            else
            {
                serialcommand.DisAutoConnectBLESerialCommand(serialPort1);
                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                string DisplayString = "请求关闭自动连接配置...\r\n";
                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                OutMsg(MonitorText, DisplayString, Color.Red);
                ScanButton.Enabled = true;
            }
        }

        private void button5_Click(object sender, EventArgs e)
        {
            ScalingFlag = 1;    //开始1mv定标
            amplification_Back = amplification;
            difference_Value_Back = difference_Value;
            difference_Value = 0;
            amplification = 1000;
            BaseLine = 0;
            System.Threading.Thread.Sleep(100);
        }

        private void textBox5_TextChanged(object sender, EventArgs e)
        {
            string InputChar;
            try
            {
                InputChar = textBox5.Text.Substring(textBox5.Text.Length - 1, 1);
                char[] cc = InputChar.ToCharArray();
                if ((textBox5.Text.Length <= 17))
                {
                    if ((cc[0] >= '0') && (cc[0] <= '9') || (cc[0] >= 'a') && (cc[0] <= 'f') || (cc[0] >= 'A') && (cc[0] <= 'F') || (cc[0] <= ':'))
                    {
                        string s = textBox5.Text.ToUpper();
                        this.textBox5.Text = s;
                        textBox5.Text = s;
                        //   this.textBox5.SelectionStart = this.textBox5.Text.Length;
                        //   this.textBox5.Focus();
                        if (((this.textBox5.Text.Length + 1) % 3) == 0 && (this.textBox5.Text.Length < 17))
                        {
                            string ss = textBox5.Text;
                            int idx = textBox5.SelectionStart;
                            ss = ss.Insert(idx, ":");
                            textBox5.Text = ss;
                            textBox5.SelectionStart = idx + 1;
                            textBox5.Focus();
                        }
                    }
                    else
                    {
                        string strNew = this.textBox5.Text.Substring(0, this.textBox5.Text.Length - 1);
                        this.textBox5.Text = strNew;
                        // this.textBox5.SelectionStart = this.textBox5.Text.Length;
                        // this.textBox5.Focus();
                        MessageBox.Show("超出输入范围", "MAC地址输入异常", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                }
                else
                {
                    string strNew = this.textBox5.Text.Substring(0, this.textBox5.Text.Length - 1);
                    this.textBox5.Text = strNew;
                    this.textBox5.SelectionStart = this.textBox5.Text.Length;
                    this.textBox5.Focus();
                }
            }
            catch (ArgumentOutOfRangeException)
            {
                // MessageBox.Show("条码长度大于15位或者小于9位，可能导致数据截取错误！", "条码扫描异常", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

        }
        private void button6_Click(object sender, EventArgs e)
        {
            string InputChar = textBox5.Text;
            char[] cc = InputChar.ToCharArray();
            for (int i = 0; i < 0x11; i++)
            {
                if (cc[i] >= '0' && (cc[i] <= '9'))
                {
                    cc[i] = (char)(cc[i] - 0x30);
                }
                else if (cc[i] >= 'A' && (cc[i] <= 'F'))
                {
                    cc[i] = (char)(cc[i] - 'A' + 10);
                }
            }
            serialcommand.PairingCommand(serialPort1, cc);
            System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
            string DisplayString = "请求设置配对心电补丁的MAC\r\n";
            DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
            OutMsg(MonitorText, DisplayString, Color.Red);
        }
        private void button1_Click(object sender, EventArgs e)
        {
            ScalingFlag = 11;    //开始0mv校准

            amplification_Back = amplification;
            difference_Value_Back = difference_Value;
            difference_Value = 0;
            amplification = 1000;
            BaseLine = 0;
            System.Threading.Thread.Sleep(100);
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            if (ComConnectFlag == 1)
            {
                //ComConnectTimeOut = 0;
                if (!serialPort1.IsOpen)
                {
                    checkBox1.Checked = false;

                    System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                    string DisplayString = "串口已断开！\r\n";
                    DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                    OutMsg(MonitorText, DisplayString, Color.Red);
                    SerialSetButton.Text = "初始化串口";

                    Vbat = 0;

                    comboBoxCom.Enabled = true;
                    comboBoxPortel.Enabled = true;
                    this.toolStripStatusLabel1.Text = comName + "已经关闭！";
                    if (serialPort1.IsOpen)
                    {
                        serialPort1.Close();
                        System.Threading.Thread.Sleep(500);
                    }
                    ComConnectFlag = 0;
                    DataStoreButton.Enabled = false;
                    ScanButton.Enabled = false;
                    ConnectBLEButton.Enabled = false;

                    for (int i = 0; i < N * M; i++)
                    {
                        XdataV[i] = ZeroValue;
                    }
                    chart1.Series["Series_Ecg"].Points.DataBindXY(Xdata, XdataV);
                    chart1.Series["Series_Ecg"].Color = Color.Black;

                    this.EcgPatchVersionLabel.Text = "";
                    this.toolStripStatusLabel2.Text = "";
                }
                else
                {
                    if (UpgradeFlag == 0)
                    {
                        if ((BLECentralMAC[0] == 0) && (BLECentralMAC[1] == 0) && (BLECentralMAC[2] == 0) && (BLECentralMAC[3] == 0) && (BLECentralMAC[4] == 0) && (BLECentralMAC[5] == 0))
                        {
                            serialcommand.SetTestModeCommand(serialPort1);
                            System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                            string DisplayString = "设置为测试模式\r\n";
                            DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                            OutMsg(MonitorText, DisplayString, Color.Red);
                        }
                    }
                    if (UpgradeFlag == 1)
                    {
                        ComConnectTimeOut[0]++;
                        if (ComConnectTimeOut[0] >= 5)
                        {
                            serialcommand.UpGradeCommand(serialPort1);
                            if (ConnectUartNum >= 1)
                            {
                                if (ConnectUartNum == 1)
                                {
                                    if (comboBoxCom.SelectedIndex < (comboBoxCom.Items.Count - 1))
                                    {
                                        comboBoxCom.SelectedIndex++;
                                    }
                                    else if (comboBoxCom.SelectedIndex >= (comboBoxCom.Items.Count - 1))
                                    {
                                        comboBoxCom.SelectedIndex = 0;
                                    }
                                    if (comboBoxCom.Text == ConnectUartName[0])
                                    {
                                        return;
                                    }
                                    ConnectUartName[1] = comboBoxCom.Text;
                                    System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                    string DisplayString = "正在连接" + ConnectUartName[1] + "...\r\n";
                                    DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                    OutMsg(MonitorText, DisplayString, Color.Red);
                                    if (ComInit(serialPort2))
                                    {
                                        ComConnectTimeOut[1] = 0;
                                    }
                                }
                                ComConnectTimeOut[1]++;
                                serialcommand.UpGradeCommand(serialPort2);
                                if (ComConnectTimeOut[1] >= 2)
                                {
                                    System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                    string DisplayString = ConnectUartName[1] + "已断开！\r\n";
                                    DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                    OutMsg(MonitorText, DisplayString, Color.Red);

                                    checkBox4.Text = "未连接";
                                    checkBox4.Checked = false;
                                    ConnectUartNum = 1;
                                }
                            }

                        }
                        if (ComConnectTimeOut[0] > 10)
                        {
                            System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                            string DisplayString = ConnectUartName[0] + "已断开！\r\n";
                            DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                            OutMsg(MonitorText, DisplayString, Color.Red);

                            checkBox3.Text = "未连接";
                            checkBox3.Checked = false;

                            ConnectUartNum = 0;
                            ComConnectFlag = 0;
                        }
                    }
                }
            }
            else if (ComConnectFlag == 0)
            {
                if (AutoComConnectFlag == 1)
                {
                    ComConnectTimeOut[0]++;
                    if (ComConnectTimeOut[0] >= 5)
                    {
                        ComConnectTimeOut[0] = 0;
                        if (comboBoxCom.SelectedIndex < (comboBoxCom.Items.Count - 1))
                        {
                            if (UpgradeFlag == 1)
                            {
                                UpgradeFlag = 0;
                            }
                            else
                            {
                                UpgradeFlag = 1;
                                comboBoxCom.SelectedIndex++;
                            }
                        }
                        else if (comboBoxCom.SelectedIndex >= (comboBoxCom.Items.Count - 1))
                        {
                            if (UpgradeFlag == 1)
                            {
                                UpgradeFlag = 0;
                            }
                            else
                            {
                                UpgradeFlag = 1;
                                GetComList();    //获取当前所有的串口
                                comboBoxCom.SelectedIndex = 0;
                            }
                        }
                        ConnectUartName[0] = comboBoxCom.Text;
                        if (serialPort2.IsOpen)
                        {
                            ConnectUartNum = 0;
                            serialPort2.Close();
                            checkBox4.Text = "未连接";
                            checkBox4.Checked = false;
                        }
                        if (serialPort3.IsOpen)
                        {
                            ConnectUartNum = 0;
                            serialPort3.Close();
                            checkBox5.Text = "未连接";
                            checkBox5.Checked = false;
                        }
                        if (serialPort4.IsOpen)
                        {
                            ConnectUartNum = 0;
                            serialPort4.Close();
                            checkBox6.Text = "未连接";
                            checkBox6.Checked = false;
                        }
                        if (ComInit(serialPort1))
                        {
                            System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                            string DisplayString = "正在连接" + ConnectUartName[0] + "...\r\n";
                            DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                            OutMsg(MonitorText, DisplayString, Color.Red);
                            comboBoxCom.Enabled = false;
                            comboBoxPortel.Enabled = false;
                            SerialSetButton.Text = "关闭串口";

                            if (UpgradeFlag == 0)
                            {
                                ScanButton.Enabled = true;

                                PauseFlag = 0;
                                PauseButton.Text = "运行中";

                                BLEConnectFlag = 1;  //假设设备正在通讯

                                serialcommand.SendConnectSerialCommand(serialPort1);  //发生连接central端命令  
                            }
                            else
                            {
                                serialcommand.UpGradeCommand(serialPort1);
                            }

                        }
                    }
                }

            }
        }

        private void checkBox2_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox2.Checked)
            {
                AutoComConnectFlag = 1;
                SerialSetButton.Enabled = false;
            }
            else
            {
                AutoComConnectFlag = 0;
                SerialSetButton.Enabled = true;
            }
        }

        private void numericUpDown1_ValueChanged(object sender, EventArgs e)
        {
            chart1.ChartAreas["ChartArea_Ecg"].AxisY.Maximum = (int)(6000 / (double)(numericUpDown1.Value));
            chart1.ChartAreas["ChartArea_Ecg"].AxisY.Minimum = (int)(-6000 / (double)(numericUpDown1.Value));
            chart1.ChartAreas["ChartArea_Ecg"].AxisY.Interval = (int)chart1.ChartAreas["ChartArea_Ecg"].AxisY.Maximum / 6;
            chart1.ChartAreas["ChartArea_Ecg"].AxisY.Maximum = chart1.ChartAreas["ChartArea_Ecg"].AxisY.Interval * 6;
            chart1.ChartAreas["ChartArea_Ecg"].AxisY.Minimum = -chart1.ChartAreas["ChartArea_Ecg"].AxisY.Interval * 6;
            chart1.ChartAreas["ChartArea_Ecg"].AxisY.MinorGrid.Interval = chart1.ChartAreas["ChartArea_Ecg"].AxisY.Interval / 5;
        }

        private void chart1_MouseDown(object sender, MouseEventArgs e)
        {
            label15.Text = "移动到需要测试的终点，放开鼠标按键";
            this.chart1.Focus();
            X_Start = e.X;
            Y_Start = e.Y;
            if (e.Button == System.Windows.Forms.MouseButtons.Left)
            {
                // label17.Text = StoreDP.YValues[0].ToString();
                label17.Text = string.Format("获取的第一个点的值:{0:F0}mV", StoreDP.YValues[0]);
                StartStoreDP = StoreDP;
            }
            else if (e.Button == System.Windows.Forms.MouseButtons.Right)
            {
                //label18.Text = StoreDP.YValues[0].ToString();
                label18.Text = string.Format("获取的第二个点的值:{0:F0}mV", StoreDP.YValues[0]);
                label19.Text = string.Format("高度差::{0:F0}mV", Math.Abs(StoreDP.YValues[0] - StartStoreDP.YValues[0]));
            }
        }

        private void chart1_MouseUp(object sender, MouseEventArgs e)
        {
            label15.Text = "所测试的宽度为:" + Math.Abs(e.X - X_Start).ToString() + "   所测试的高度为:" + Math.Abs(e.Y - Y_Start).ToString();
        }

        private void chart1_GetToolTipText(object sender, ToolTipEventArgs e)
        {
            if (e.HitTestResult.ChartElementType == ChartElementType.DataPoint)
            {
                this.Cursor = Cursors.Hand;
                int i = e.HitTestResult.PointIndex;
                StoreDP = e.HitTestResult.Series.Points[i];
                label16.Visible = true;
                label16.Text = string.Format("当前点的心电电压值:{0:F0}mV", StoreDP.YValues[0]);
            }
            else
            {
                label16.Visible = false;
                this.Cursor = Cursors.Default;
            }
        }
        private void chart1_MouseLeave(object sender, EventArgs e)
        {
            label16.Visible = false;
        }

        private void chart1_MouseEnter(object sender, EventArgs e)
        {
            label16.Visible = false;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            int num = 0;
            ScanUpdataThread.Abort();

            System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
            string DisplayString3 = "请求删除flash...\r\n";
            DisplayString3 = DateTime.Now.ToLongTimeString() + ": " + DisplayString3;
            OutMsg(MonitorText, DisplayString3, Color.Red);

            if (checkBox3.Checked == true)
            {
                DownLoadFlag[0] = 1;
                serialcommand.FlashEraseAllUnsecure_Command(serialPort1);
                num = 0;
                while (DownLoadFlag[0] == 1)
                {
                    Thread.Sleep(10);
                    num++;
                    if (num>100)
                    {
                        num = 0;
                        string DisplayString = ConnectUartName[0] + ":擦除flash无响应！\r\n请重新升级!\r\n";
                        DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                        OutMsg(MonitorText, DisplayString, Color.Red);
                        checkBox3.Checked = false;
                        break;
                    }
                }
                if (DownLoadFlag[0] == 2)
                {
                    string DisplayString = ConnectUartName[0] + ":擦除flash成功！\r\n";
                    DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                    OutMsg(MonitorText, DisplayString, Color.Red);

                    serialcommand.Upgrade_ACKCommand(serialPort1);
                }
                else if (DownLoadFlag[0] == 3)
                {
                    string DisplayString = ConnectUartName[0] + ":擦除flash失败！\r\n请重新升级!\r\n";
                    DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                    OutMsg(MonitorText, DisplayString, Color.Red);
                    checkBox3.Checked = false;
                }

                Thread.Sleep(5);


            }
            if (checkBox4.Checked == true)
            {
                DownLoadFlag[1] = 1;
                serialcommand.FlashEraseAllUnsecure_Command(serialPort2);
                num = 0;
                while (DownLoadFlag[1] == 1)
                {
                    Thread.Sleep(10);
                    num++;
                    if (num > 100)
                    {
                        num = 0;
                        string DisplayString = ConnectUartName[1] + ":擦除flash无响应！\r\n请重新升级!\r\n";
                        DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                        OutMsg(MonitorText, DisplayString, Color.Red);
                        checkBox4.Checked = false;
                        break;
                    }
                }
                if (DownLoadFlag[1] == 2)
                {
                    string DisplayString = ConnectUartName[1] + ":擦除flash成功！\r\n";
                    DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                    OutMsg(MonitorText, DisplayString, Color.Red);

                    serialcommand.Upgrade_ACKCommand(serialPort2);
                }
                else if (DownLoadFlag[1] == 3)
                {
                    string DisplayString = ConnectUartName[1] + ":擦除flash失败！\r\n请重新升级!\r\n";
                    DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                    OutMsg(MonitorText, DisplayString, Color.Red);
                    checkBox4.Checked = false;
                }

                Thread.Sleep(5);
            }
            if (checkBox5.Checked == true)
            {
                DownLoadFlag[2] = 1;
                serialcommand.FlashEraseAllUnsecure_Command(serialPort3);
                num = 0;
                while (DownLoadFlag[2] == 1)
                {
                    Thread.Sleep(10);
                    num++;
                    if (num > 100)
                    {
                        num = 0;
                        string DisplayString = ConnectUartName[2] + ":擦除flash无响应！\r\n请重新升级!\r\n";
                        DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                        OutMsg(MonitorText, DisplayString, Color.Red);
                        checkBox5.Checked = false;
                        break;
                    }
                }
                if (DownLoadFlag[2] == 2)
                {
                    string DisplayString = ConnectUartName[2] + ":擦除flash成功！\r\n";
                    DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                    OutMsg(MonitorText, DisplayString, Color.Red);

                    serialcommand.Upgrade_ACKCommand(serialPort3);
                }
                else if (DownLoadFlag[2] == 3)
                {
                    string DisplayString = ConnectUartName[2] + ":擦除flash失败！\r\n请重新升级!\r\n";
                    DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                    OutMsg(MonitorText, DisplayString, Color.Red);
                    checkBox5.Checked = false;
                }

                Thread.Sleep(5);
            }
            if (checkBox6.Checked == true)
            {
                DownLoadFlag[3] = 1;
                serialcommand.FlashEraseAllUnsecure_Command(serialPort4);
                num = 0;
                while (DownLoadFlag[3] == 1)
                {
                    Thread.Sleep(10);
                    num++;
                    if (num > 100)
                    {
                        num = 0;
                        string DisplayString = ConnectUartName[3] + ":擦除flash无响应！\r\n请重新升级!\r\n";
                        DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                        OutMsg(MonitorText, DisplayString, Color.Red);
                        checkBox6.Checked = false;
                        break;
                    }
                }
                if (DownLoadFlag[3] == 2)
                {
                    string DisplayString = ConnectUartName[3] + ":擦除flash成功！\r\n";
                    DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                    OutMsg(MonitorText, DisplayString, Color.Red);

                    serialcommand.Upgrade_ACKCommand(serialPort4);
                }
                else if (DownLoadFlag[3] == 3)
                {
                    string DisplayString = ConnectUartName[3] + ":擦除flash失败！\r\n请重新升级!\r\n";
                    DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                    OutMsg(MonitorText, DisplayString, Color.Red);
                    checkBox6.Checked = false;
                }

                Thread.Sleep(5);
            }

            Upgrade();

        }

        private void Upgrade()
        {
            string[] text = File.ReadAllLines(textBox3.Text);
                progressBar1.Maximum = text.Length / 2;
                progressBar1.Value = 0;
                progressBar2.Maximum = text.Length / 2;
                progressBar2.Value = 0;
                progressBar3.Maximum = text.Length / 2;
                progressBar3.Value = 0;
                progressBar4.Maximum = text.Length / 2;
                progressBar4.Value = 0;

                if (checkBox3.Checked == true)
                {
                    if (UpDataThread1.IsAlive)
                    {
                        UpDataThread1.Abort();
                    }
                    UpDataThread1 = new Thread(new ParameterizedThreadStart(Upgrade2));
                    UpDataThread1.Name = "Thread A:";
                    UpDataThread1.Start(serialPort1);
                    DowmLoadNum++;
                }
                if (checkBox4.Checked == true)
                {
                    if (UpDataThread2.IsAlive)
                    {
                        UpDataThread2.Abort();
                    }
                    UpDataThread2 = new Thread(new ParameterizedThreadStart(Upgrade2));
                    UpDataThread2.Name = "Thread A:";
                    UpDataThread2.Start(serialPort2);
                    DowmLoadNum++;
                }
                if (checkBox5.Checked == true)
                {
                    if (UpDataThread3.IsAlive)
                    {
                        UpDataThread3.Abort();
                    }
                    UpDataThread3 = new Thread(new ParameterizedThreadStart(Upgrade2));
                    UpDataThread3.Name = "Thread A:";
                    UpDataThread3.Start(serialPort3);
                    DowmLoadNum++;
                }
                if (checkBox6.Checked == true)
                {
                    if (UpDataThread4.IsAlive)
                    {
                        UpDataThread4.Abort();
                    }
                    UpDataThread4 = new Thread(new ParameterizedThreadStart(Upgrade2));
                    UpDataThread4.Name = "Thread A:";
                    UpDataThread4.Start(serialPort4);
                    DowmLoadNum++;
                }          
        }

        int WriteFlashACK(int i,SerialPort serialport, Int32 StartAddress, Int32 ByteCount)
        {
            int num = 0;
            System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
            string DisplayString3 = serialport.PortName + ":一块程序开始下载。。。\r\n" + "起始地址: 0x" + StartAddress.ToString("X4") +
                "  数据大小：0x" + ByteCount.ToString("X4") + "\r\n";
            DisplayString3 = DateTime.Now.ToLongTimeString() + ": " + DisplayString3;
            OutMsg(MonitorText, DisplayString3, Color.Red);

            DownLoadFlag[i] = 4;
            serialcommand.WriteMemory_Command(serialport, StartAddress, ByteCount);
            num = 0;
            while(DownLoadFlag[i] == 4)
            {
                Thread.Sleep(1);
                num++;
                if(num>100)
                {
                    num = 0;
                    string DisplayString = serialport.PortName + ":写数据请求无响应！\r\n请重新升级!\r\n";
                    DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                    OutMsg(MonitorText, DisplayString, Color.Red);
                    return -1;
                }
            }
            if (DownLoadFlag[i] == 5)
            {
                string DisplayString = serialport.PortName + ":写数据请求成功！\r\n" + "数据正在下载中...\r\n";
                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                OutMsg(MonitorText, DisplayString, Color.Red);
            }
            else if (DownLoadFlag[i] == 6)
            {
                string DisplayString = serialport.PortName + ":写数据请求失败！\r\n";
                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                OutMsg(MonitorText, DisplayString, Color.Red);
                return -2;
            }
            serialcommand.Upgrade_ACKCommand(serialport);
            return 0;
        }

        int WriteGeneralFlash(int i,SerialPort serialport, byte[] Byte)
        {
            int num = 0;
            DownLoadFlag[i] = 7;
            serialcommand.WriteMemoryData_Command(serialport, Byte, 32);

            num = 0;
            while (DownLoadFlag[i] == 7)
            {
                Thread.Sleep(1);
                num++;
                if (num > 100)
                {
                    num = 0;
                    string DisplayString = serialport.PortName + ":写数据请求无响应！\r\n请重新升级!\r\n";
                    DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                    OutMsg(MonitorText, DisplayString, Color.Red);
                    return -1;
                }
            }

            //if (DownLoadFlag == 8)
            //{
            //    converter = new System.Text.ASCIIEncoding();
            //    DisplayString3 = ConnectUartName[0] + ":正确写入一段数据！\r\n";
            //    DisplayString3 = DateTime.Now.ToLongTimeString() + ": " + DisplayString3;
            //    OutMsg(MonitorText, DisplayString3, Color.Red);
            //}
            if (DownLoadFlag[i] == 9)
            {
                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                string DisplayString3 = serialport.PortName + ":数据写错误！\r\n";
                DisplayString3 = DateTime.Now.ToLongTimeString() + ": " + DisplayString3;
                OutMsg(MonitorText, DisplayString3, Color.Red);
                return -2;
            }
            return 0;
        }

        int WriteLastFlash(int i,SerialPort serialport, byte[] Byte, UInt16 Length)
        {
            int num = 0;
            DownLoadFlag[i] = 10;
            serialcommand.WriteMemoryData_Command(serialport, Byte, Length);
            
            while (DownLoadFlag[i] == 10)
            {
                Thread.Sleep(1);
                num++;
                if (num > 100)
                {
                    num = 0;
                    System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                    string DisplayString3 = serialport.PortName + ":一块程序下载完成无响应！\r\n请重新升级!\r\n";
                    DisplayString3 = DateTime.Now.ToLongTimeString() + ": " + DisplayString3;
                    OutMsg(MonitorText, DisplayString3, Color.Red);
                    return -1;
                }
            }
            if (DownLoadFlag[i] == 11)
            {
                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                string DisplayString3 = serialport.PortName + ":一块程序下载完成！\r\n";
                DisplayString3 = DateTime.Now.ToLongTimeString() + ": " + DisplayString3;
                OutMsg(MonitorText, DisplayString3, Color.Red);
            }
            else if (DownLoadFlag[i] == 12)
            {
                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                string DisplayString3 = serialport.PortName + ":一块程序下载失败！\r\n";
                DisplayString3 = DateTime.Now.ToLongTimeString() + ": " + DisplayString3;
                OutMsg(MonitorText, DisplayString3, Color.Red);
                return -2;
            }
            serialcommand.Upgrade_ACKCommand(serialport);
            Thread.Sleep(10);
            return 0;
        }


        void GetHexData(byte[] DataBuffer, List<byte> StoreDataList, List<int> StoreLineAddrList)
        {
            int getlinelength = 0;
            int c, d, h, f, g;
            if (((int)DataBuffer[2]) > 0x39)
            {
                c = 0x40 - 0x09;
            }
            else
            {
                c = 0x30;
            }
            if (((int)DataBuffer[3]) > 0x39)
            {
                d = 0x40 - 0x09;
            }
            else
            {
                d = 0x30;
            }
            if (((int)DataBuffer[4]) > 0x39)
            {
                h = 0x40 - 0x09;
            }
            else
            {
                h = 0x30;
            }
            if (((int)DataBuffer[5]) > 0x39)
            {
                f = 0x40 - 0x09;
            }
            else
            {
                f = 0x30;
            }
            if (((int)DataBuffer[6]) > 0x39)
            {
                g = 0x40 - 0x09;
            }
            else
            {
                g = 0x30;
            }
            getlinelength = ((DataBuffer[1] - 0x30) << 4) + (DataBuffer[2] - c);
            StoreLineAddrList.Add(((DataBuffer[3] - d) << 12) + ((DataBuffer[4] - h) << 8) + ((DataBuffer[5] - f) << 4) + (DataBuffer[6] - g));
            for (int i = 0; i < getlinelength; i++)
            {
                int a, b;
                if (((int)DataBuffer[(i << 1) + 9]) > 0x39)
                {
                    a = 0x40 - 0x09;
                }
                else
                {
                    a = 0x30;
                }
                if (((int)DataBuffer[(i << 1) + 10]) > 0x39)
                {
                    b = 0x40 - 0x09;
                }
                else
                {
                    b = 0x30;
                }
                StoreDataList.Add((byte)(int)(((DataBuffer[(i << 1) + 9] - a) << 4) + (DataBuffer[(i << 1) + 10] - b)));
            }
        }
        void List2Byte(List<byte> BufferList, byte[] buffer, int length)
        {
            for (int i = 0; i < length; i++)
            {
                buffer[i] = BufferList[i];
            }
        }

        int WritePageData2Flash(int i,SerialPort serialport, List<byte> StoreDataList, List<int> StoreLineAddrList)
        {
            byte[] buffer = new byte[32];
            if (WriteFlashACK(i, serialport, StoreLineAddrList[0], StoreDataList.Count) != 0)
            {
                return -1;
            }
            while (StoreDataList.Count >= 32)
            {
                List2Byte(StoreDataList, buffer, 32);
                if (WriteGeneralFlash(i, serialport, buffer) != 0)
                {
                    return -2;
                }
                StoreDataList.RemoveRange(0, 32);
                if (StoreDataList.Count == 32)
                {
                    List2Byte(StoreDataList, buffer, 32);

                    if (WriteLastFlash(i, serialport, buffer, 32) != 0)
                    {
                        return -3;
                    }
                    StoreDataList.RemoveRange(0, 32);
                }
                if (serialport == serialPort1)
                {
                    progressBar1.Invoke(new EventHandler(delegate
                    {
                        progressBar1.Value++;
                    }));
                    label9.Invoke(new EventHandler(delegate
                    {
                        int a = progressBar1.Value * 100 / progressBar1.Maximum;
                        label9.Text = "正在下载中..." + a.ToString()+"%";
                    }));
                }
                else if (serialport == serialPort2)
                {
                    progressBar2.Invoke(new EventHandler(delegate
                    {
                        progressBar2.Value++;
                    }));
                    label20.Invoke(new EventHandler(delegate
                    {
                        int a = progressBar2.Value * 100 / progressBar2.Maximum;
                        label20.Text = "正在下载中..." + a.ToString() + "%";
                    }));
                }
                else if (serialport == serialPort3)
                {
                    progressBar3.Invoke(new EventHandler(delegate
                    {
                        progressBar3.Value++;
                    }));
                    label21.Invoke(new EventHandler(delegate
                    {
                        int a = progressBar3.Value * 100 / progressBar3.Maximum;
                        label21.Text = "正在下载中..." + a.ToString() + "%";
                    }));
                }
                else if (serialport == serialPort4)
                {
                    progressBar4.Invoke(new EventHandler(delegate
                    {
                        progressBar4.Value++;
                    }));
                    label22.Invoke(new EventHandler(delegate
                    {
                        int a = progressBar4.Value * 100 / progressBar4.Maximum;
                        label22.Text = "正在下载中..." + a.ToString() + "%";
                    }));
                }

            }
            if (StoreDataList.Count > 0)
            {
                List2Byte(StoreDataList, buffer, StoreDataList.Count);
                if (WriteLastFlash(i, serialport, buffer, (UInt16)StoreDataList.Count) != 0)
                {
                    return -4;
                }
                StoreDataList.RemoveRange(0, StoreDataList.Count);
                if (serialport == serialPort1)
                {
                    progressBar1.Invoke(new EventHandler(delegate
                    {
                        progressBar1.Value++;
                    }));
                    label9.Invoke(new EventHandler(delegate
                    {
                        int a = progressBar1.Value * 100 / progressBar1.Maximum;
                        label9.Text = "正在下载中..." + a.ToString() + "%";
                    }));
                }
                else if (serialport == serialPort2)
                {
                    progressBar2.Invoke(new EventHandler(delegate
                    {
                        progressBar2.Value++;
                    }));
                    label20.Invoke(new EventHandler(delegate
                    {
                        int a = progressBar2.Value * 100 / progressBar2.Maximum;
                        label20.Text = "正在下载中..." + a.ToString() + "%";
                    }));
                }
                else if (serialport == serialPort3)
                {
                    progressBar3.Invoke(new EventHandler(delegate
                    {
                        progressBar3.Value++;
                    }));
                    label21.Invoke(new EventHandler(delegate
                    {
                        int a = progressBar3.Value * 100 / progressBar3.Maximum;
                        label21.Text = "正在下载中..." + a.ToString() + "%";
                    }));
                }
                else if (serialport == serialPort4)
                {
                    progressBar4.Invoke(new EventHandler(delegate
                    {
                        progressBar4.Value++;
                    }));
                    label22.Invoke(new EventHandler(delegate
                    {
                        int a = progressBar4.Value * 100 / progressBar4.Maximum;
                        label22.Text = "正在下载中..." + a.ToString() + "%";
                    }));
                }
            }
            return 0;
        }

        void Upgrade2(object serialport)
        {
            int i = 0;
            List<byte> DataBuffer = new List<byte>(999999);
            List<int> LineAddr = new List<int>(9999);

            SerialPort parameter = serialport as SerialPort;//类型转换 

            if (parameter == serialPort1)
            {
                i = 0;
            }
            else if (parameter == serialPort2)
            {
                i = 1;
            }
            else if (parameter == serialPort3)
            {
                i = 2;
            }
            else if (parameter == serialPort4)
            {
                i = 3;
            }

            StreamReader sr = new StreamReader(textBox3.Text);
            string s;
            while ((s = sr.ReadLine()) != null)
            {
                byte[] bytes = System.Text.Encoding.ASCII.GetBytes(s);
                if (bytes[0] == 0x3A)    //:
                {
                    if ((bytes[7] == 0x30) && (bytes[8] == 0x34))
                    {
                        if (DataBuffer.Count > 0)
                        {
                            if (WritePageData2Flash(i, parameter, DataBuffer, LineAddr) != 0)
                            {
                                if (serialport == serialPort1)
                                {
                                    label9.Invoke(new EventHandler(delegate
                                    {
                                        label9.Text = "升级失败，请重新升级!";
                                    }));
                                    DowmLoadNum--;
                                }
                                else if (serialport == serialPort2)
                                {
                                    label20.Invoke(new EventHandler(delegate
                                    {
                                        label20.Text = "升级失败，请重新升级!";
                                    }));
                                    DowmLoadNum--;
                                }
                                else if (serialport == serialPort3)
                                {
                                    label21.Invoke(new EventHandler(delegate
                                    {
                                        label21.Text = "升级失败，请重新升级!";
                                    }));
                                    DowmLoadNum--;
                                }
                                else if (serialport == serialPort4)
                                {
                                    label22.Invoke(new EventHandler(delegate
                                    {
                                        label22.Text = "升级失败，请重新升级!";
                                    }));
                                    DowmLoadNum--;
                                }
                                if(DowmLoadNum == 0)
                                {
                                    ScanUpdataThread = new Thread(new ThreadStart(ScanUpdataUartFunction));
                                    ScanUpdataThread.Start();
                                }
                                return;
                            }
                        }
                        LineAddr.Clear();
                    }
                    else if ((bytes[7] == 0x30) && (bytes[8] == 0x31))
                    {
                        if (DataBuffer.Count > 0)
                        {
                            if(WritePageData2Flash(i, parameter, DataBuffer, LineAddr) != 0)
                            {
                                if (serialport == serialPort1)
                                {
                                    label9.Invoke(new EventHandler(delegate
                                    {
                                        label9.Text = "升级失败，请重新升级!";
                                    }));
                                    DowmLoadNum--;
                                }
                                else if (serialport == serialPort2)
                                {
                                    label20.Invoke(new EventHandler(delegate
                                    {
                                        label20.Text = "升级失败，请重新升级!";
                                    }));
                                    DowmLoadNum--;
                                }
                                else if (serialport == serialPort3)
                                {
                                    label21.Invoke(new EventHandler(delegate
                                    {
                                        label21.Text = "升级失败，请重新升级!";
                                    }));
                                    DowmLoadNum--;
                                }
                                else if (serialport == serialPort4)
                                {
                                    label22.Invoke(new EventHandler(delegate
                                    {
                                        label22.Text = "升级失败，请重新升级!";
                                    }));
                                    DowmLoadNum--;
                                }
                                if(DowmLoadNum == 0)
                                {
                                    ScanUpdataThread = new Thread(new ThreadStart(ScanUpdataUartFunction));
                                    ScanUpdataThread.Start();
                                }
                                return;
                            }
                        }
                        System.Text.ASCIIEncoding converter2 = new System.Text.ASCIIEncoding();
                        string DisplayString2 = parameter.PortName + ":升级完成！\r\n";
                        DisplayString2 = DateTime.Now.ToLongTimeString() + ": " + DisplayString2;
                        OutMsg(MonitorText, DisplayString2, Color.Red);

                        if (serialport == serialPort1)
                        {
                            label9.Invoke(new EventHandler(delegate
                            {
                                label9.Text = "下载完成!";
                            }));
                            DowmLoadNum--;
                        }
                        else if (serialport == serialPort2)
                        {
                            label20.Invoke(new EventHandler(delegate
                            {
                                label20.Text = "下载完成";
                            }));
                            DowmLoadNum--;
                        }
                        else if (serialport == serialPort3)
                        {
                            label21.Invoke(new EventHandler(delegate
                            {
                                label21.Text = "下载完成";
                            }));
                            DowmLoadNum--;
                        }
                        else if (serialport == serialPort4)
                        {
                            label22.Invoke(new EventHandler(delegate
                            {
                                label22.Text = "下载完成";
                            }));
                            DowmLoadNum--;
                        }
                        if(DowmLoadNum == 0)
                        {
                            ScanUpdataThread = new Thread(new ThreadStart(ScanUpdataUartFunction));
                            ScanUpdataThread.Start();
                        }

                    }
                    else if (bytes[7] == 0x30 && (bytes[8] == 0x30))  //数据
                    {
                        GetHexData(bytes, DataBuffer, LineAddr);
                    }
                }
            }
        }

        private void RF_FormClosed(object sender, FormClosedEventArgs e)
        {
            ScanUpdataThread.Abort();
            if (UpDataThread1.IsAlive)
            {
                UpDataThread1.Abort();
            }
            if (UpDataThread2.IsAlive)
            {
                UpDataThread2.Abort();
            }
            if (UpDataThread3.IsAlive)
            {
                UpDataThread3.Abort();
            }
            if (UpDataThread4.IsAlive)
            {
                UpDataThread4.Abort();
            }
        }

        private void button2_Click_1(object sender, EventArgs e)
        {
            string DisplayString = "正在打开升级文件。。。\r\n";
            DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
            OutMsg(MonitorText, DisplayString, Color.Red);


            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = "bin文件(*.hex)|*.hex";
            openFileDialog.Title = "打开升级文件";
            openFileDialog.FilterIndex = 1;
            openFileDialog.RestoreDirectory = true;
            DialogResult dialogresult = openFileDialog.ShowDialog();
            if (dialogresult == DialogResult.OK)
            {
                textBox3.Text = openFileDialog.FileName.ToString();
            }
        }
    }

}
