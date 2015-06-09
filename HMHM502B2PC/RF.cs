using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
//using Dundas.Charting.WinControl;
using System.IO;
using System.IO.Ports;
using Microsoft.Win32;
using System.Windows.Forms.DataVisualization.Charting;

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
        private List<double> SerialEcgData = new List<double>(512);//默认分配1页内存，并始终限制不允许超

        private byte EcgCaptureFlag = 0;
        private byte BLEConnectFlag = 0;

        private int ScanBLENum = 0;
        private byte[,] ScanBLEMAC = new byte[10,6];

        private int BLEConnectFlagTimerOut = 0;

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
        private Int16 amplification = 1;
        private Int16 difference_Value = 0;

        private Int16 amplification_Back = 1;
        private Int16 difference_Value_Back = 0;

        private int ScalingFlag = 0;
        private int calibration_Num = 0;
        private List<double> calibration_Value = new List<double>(1024);
        private double[] ScalingEcgMin = new double[8];
        private double[] ScalingEcgMax = new double[8];

        private int ZeroValue = -6001;
        private int BaseLine = 500;

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
            System.Threading.Thread.Sleep(20);
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

            //chart1.Titles[0].Text = "原始心电输出信号";
            //chart1.Titles[0].Alignment = ContentAlignment.TopCenter;
            //chart1.Titles[0].Font = new Font("黑体", 10, FontStyle.Bold);
            //chart1.Titles[0].ForeColor = Color.FromArgb(128, 72, 72);
          //  chart1.ChartAreas.Clear();
          //  chart1.ChartAreas.Add("DX");

          //  chart1.ChartAreas["DX"].AxisX.ArrowStyle = AxisArrowStyle.Lines;
          //  chart1.ChartAreas["DX"].AxisY.Title = "心电电压值/mV";

          //  chart1.ChartAreas["DX"].AxisY.ArrowStyle = AxisArrowStyle.Lines;

          //  chart1.ChartAreas["DX"].AxisY.Maximum = 1024;
          //  chart1.ChartAreas["DX"].AxisY.Minimum = 0;
          ////  chart1.ChartAreas["DX"].AxisY.Interval = 1000;

          //  chart1.ChartAreas["DX"].AxisX.Title = "时间/mS";

          //  chart1.ChartAreas["DX"].AxisX.Maximum = 4000;  //4s
          //  chart1.ChartAreas["DX"].AxisX.Minimum = 0;
          //  chart1.ChartAreas["DX"].AxisX.Interval = 1000;
          //  // chart1.ChartAreas["DX"].AxisX.Interval = (chart1.ChartAreas["DX"].AxisX.Maximum - chart1.ChartAreas["DX"].AxisX.Minimum) / 20;

            //Color c1 = Color.FromArgb(50, 50, 50);
            //chart1.ChartAreas["DX"].AxisX.MajorGrid.LineColor = c1;
            //chart1.ChartAreas["DX"].AxisY.MajorGrid.LineColor = c1;
          //  chart1.ChartAreas["DX"].ShadowColor = Color.White;
          //  chart1.ChartAreas["DX"].BackColor = Color.Black;

          //  chart1.Series.Clear();
          //  chart1.Series.Add("数据个数");
          //  chart1.Legends[0].Enabled = false;
          //  chart1.Series["数据个数"].Color = Color.Green;
          //  chart1.Series["数据个数"].ChartType = SeriesChartType.Line;
          //  chart1.Series["数据个数"].ChartArea = "DX";


            for (int i = 0; i < (N * M); i++)
            {
                Xdata[i] = (i*8).ToString();
                //  XdataV[i] = i;
            }
           // chart1.Series["数据个数"].Points.DataBindXY(Xdata, XdataV);
            //chart1.Series["Series_Ecg"].Points.DataBindXY(Xdata, XdataV);


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
            //chart1.ChartAreas["ChartArea_Ecg"].AxisX.Interval = (chartlenMax - chartlenMin) / 16;
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
            chart1.ChartAreas["ChartArea_Ecg"].AxisX.Maximum = chartlenMax;
            chart1.ChartAreas["ChartArea_Ecg"].AxisX.Minimum = chartlenMin;
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
                    string DisplayString = "正在连接串口...\r\n";
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
                checkBox1.Checked = false;

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
                    System.Threading.Thread.Sleep(500);
                }
                DataStoreButton.Enabled = false;
                ScanButton.Enabled = false;
                ConnectBLEButton.Enabled = false;
            }
            for (int i = 0; i < N * M; i++)
            {
                XdataV[i] = ZeroValue;
            }
            chart1.Series["Series_Ecg"].Points.DataBindXY(Xdata, XdataV);
            this.toolStripStatusLabel2.Text = "";

            
        }


    unsafe int rhythmcount(double* fifo)
{
    int max_data = 512;
    int i;
    uint *diff = stackalloc uint[max_data];            //求导数数据区
    uint dif_max=0;                  //导数最大值
    uint thr_max;                    //阈值最大值
    int j=0;
    int *k = stackalloc int[8];                       //最大值数组
	int rhythm = 0;
    float DR_R;                             //R-R间距
	
    *diff=0;                                //数据区首两个数据导数置0
    *(diff+1)=0;
    for(i=2;i<max_data-2;i++)               //按DIFF(i)=f(i+1)-f(i-1)+2*f(i+2)-2*f(i-2)公式计算导数
    {
        *(diff+i)=(uint)(*(fifo+i+1))+2*(uint)(*(fifo+i+2))-(uint)(*(fifo+i-1))-2*(uint)(*(fifo+i-2));
    }
    *(diff+max_data-1)=0;                   //数据区末两个数据导数置0
    *(diff+max_data)=0;
    max_data = 512;
    for(i=0;i<max_data;i++)                 //求导数最大值
    {
        if((dif_max)>*(diff+i))
        dif_max=*(diff+i);
    } 
    thr_max=(dif_max>>1)-(dif_max>>3);       //设定阈值，为导数最大值的0.375倍
    for(i=1;i<max_data-1;i++)                //将满足f(i)>thr_max和f(i)*f(i+1)<0这两个条件的值找到,即为R波最高点
    {
        if ((*(diff + i) > thr_max))
            if ((((*(diff + i)) ^ (*(diff + i + 1))) >> 7) >0)
        k[j++]=i;
    }
    DR_R=(k[j-1]-k[0])/(j-1);                //计算R-R间距
    rhythm=(int)(12000/DR_R);      //通过公式200*60/DR-R求得心率值
	return rhythm;
}

    #region 定时器处理函数
    unsafe private void timer1_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
        {
            int EcgMaxValue, EcgMinValue;
            while (SerialReceiveData.Count >= 4 && ((SerialReceiveData[3]+4) <= SerialReceiveData.Count))
            {
                BLEConnectFlagTimerOut = 0;
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
                #endregion
                else if (DebugMode == 2)
                {
                    
                    if (SerialReceiveData[0] == 0x77)   //type (command)
                    {
                        if (SerialReceiveData[2] == 0x00)  //状态正常
                        {
                            if (SerialReceiveData[1] == 0x02)
                            {
                                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                string DisplayString = "串口连接成功！\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);

                                StopReceiveECGDataSerialCommand();
                                System.Threading.Thread.Sleep(100);
                                DisAutoConnectBLESerialCommand();
                                System.Threading.Thread.Sleep(100);
                                AutoConnectBLEStatusSerialCommand();
                            }
                            else if (SerialReceiveData[1] == 0x0E)
                            {
                                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                string DisplayString = "正在搜索设备！\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);
                            }
                            else if (SerialReceiveData[1] == 0x04)
                            {
                                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                string DisplayString = "搜索设备完成！\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);

                                MACComboBox.Items.Clear();

                                if (SerialReceiveData[3] > 0)
                                {
                                    int[] macbuffer = new int[6];
                                    ScanBLENum = 0;
                                    for (int i = 0; i < (SerialReceiveData[3] / 6); i++)
                                    {
                                        for(int j = 0;j<6;j++)
                                        {
                                            macbuffer[j] = SerialReceiveData[4+j + 6 * i];
                                            ScanBLEMAC[ScanBLENum, j] = SerialReceiveData[4 + j + 6 * i];
                                        }
                                        ScanBLENum++;
                                        MACComboBox.Items.Add(macbuffer[5].ToString("X2") + ":" + macbuffer[4].ToString("X2") + ":" + macbuffer[3].ToString("X2") + ":" + macbuffer[2].ToString("X2") + ":" + macbuffer[1].ToString("X2") + ":" + macbuffer[0].ToString("X2"));
                                    }



                                    ConnectBLEButton.Enabled = true;
                                    ConnectBLEButton.Text = "设备已断开";


                                   // button6.Enabled = false;   //定标按键
                                    //for(int j = 0;j<6;j++)
                                    //{
                                    //    ECGPatchMAC[j] = ScanBLEMAC[0, j];
                                    //}

                                    textBox5.Text = ScanBLEMAC[0, 5].ToString("X2") + ":" + ScanBLEMAC[0, 4].ToString("X2") + ":" + ScanBLEMAC[0, 3].ToString("X2")
                                       + ":" + ScanBLEMAC[0, 2].ToString("X2") + ":" + ScanBLEMAC[0, 1].ToString("X2") + ":" + ScanBLEMAC[0, 0].ToString("X2"); 
                                }
                                else
                                {
                                    MACComboBox.Items.Add("无设备");
                                    textBox5.Text = "00:00:00:00:00:00";
                                }
                                MACComboBox.SelectedIndex = 0;

                                ScanButton.Enabled = true;
                                MACComboBox.Enabled = true;
                            }
                            else if (SerialReceiveData[1] == 0x06)
                            {
                                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                string DisplayString = "心电补丁连接成功！\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);

                                BLEConnectFlag = 1;

                                DataStoreButton.Enabled = true;

                                string filePath = System.IO.Directory.GetCurrentDirectory() + "//ecg_data.hex";
                                File.Delete(filePath);

                               // System.Threading.Thread.Sleep(500);
                              //  ReceiveECGPatchIDSerialCommand();

                            }
                            else if (SerialReceiveData[1] == 0x0B)
                            {
                                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                string DisplayString = "设备已断开！\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);

                                ConnectBLEButton.Text = "设备已断开";
                                ConnectBLEButton.Enabled = true;
                             //   button6.Enabled = false;   //定标按键
                                ScanButton.Enabled = true;
                                DataStoreButton.Enabled = false;

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

                                BLEConnectFlag = 0;

                                this.toolStripStatusLabel2.Text = "主蓝牙设备MAC：" + BLECentralMAC[5].ToString("X2") + ":" + BLECentralMAC[4].ToString("X2")
                                    + ":" + BLECentralMAC[3].ToString("X2") + ":" + BLECentralMAC[2].ToString("X2") + ":" + BLECentralMAC[1].ToString("X2") + ":" + BLECentralMAC[0].ToString("X2")

                                    + "  连接状态：心电补丁未连接";
                                amplification = -1;
                                difference_Value = -1;
                                AmplificationValue.Text = Convert.ToString(amplification);
                                differenceValue.Text = Convert.ToString(difference_Value); 

                            }
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
                            else if (SerialReceiveData[1] == 0x13)
                            {
                                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                string DisplayString = "接收一次RSSI值！\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);
                                RSSIValue = SerialReceiveData[4]-256;
                                ConnectBLEButton.Text = "设备已连接";
                                ConnectBLEButton.Enabled = true;
                                button6.Enabled = true;   //定标按键

                            }
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
                            }
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
                               // System.Threading.Thread.Sleep(100);
                                SetTestModeCommand();
                                
                            }
                            else if (SerialReceiveData[1] == 0x25)
                            {
                                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                string DisplayString = "设置测试模式成功！\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);

                               // System.Threading.Thread.Sleep(100);
                                ReceiveCentralMACCommand();
                            }
                            else if (SerialReceiveData[1] == 0x1D)
                            {
                                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                string DisplayString = "获取主设备蓝牙MAC成功！\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);
                                for(int i = 0;i<6;i++)
                                {
                                    BLECentralMAC[i] = SerialReceiveData[4 + i];
                                }

                              //  System.Threading.Thread.Sleep(100);
                                ReceiveECGPatchMACCommand();
                            }
                            else if (SerialReceiveData[1] == 0x1F)
                            {
                                if (SerialReceiveData[4] == 0 && (SerialReceiveData[5] == 0) && (SerialReceiveData[6] == 0) && (SerialReceiveData[7] == 0))
                                {
                                    System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                    string DisplayString = "蓝牙心电补丁未连接，无法获取MAC\r\n";
                                    DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                    OutMsg(MonitorText, DisplayString, Color.Red);
                                }
                                else
                                {
                                    System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                    string DisplayString = "获取心电补丁MAC成功！\r\n";
                                    DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                    OutMsg(MonitorText, DisplayString, Color.Red);
                                }
                                for (int i = 0; i < 6; i++)
                                {
                                    ECGPatchMAC[i] = SerialReceiveData[4 + i];
                                }
                              //  System.Threading.Thread.Sleep(100);
                                ReceivePairingStatusCCommand();
                            }
                            else if (SerialReceiveData[1] == 0x23)
                            {
                                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                string DisplayString = "获取配对状态成功！\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);

                                textBox5.Text = SerialReceiveData[9].ToString("X2") + ":" + SerialReceiveData[8].ToString("X2") + ":" + SerialReceiveData[7].ToString("X2")
                                       + ":" + SerialReceiveData[6].ToString("X2") + ":" + SerialReceiveData[5].ToString("X2") + ":" + SerialReceiveData[4].ToString("X2"); 
                                for(int i = 0;i<6;i++)
                                {
                                    ECGPairMAC[i] = SerialReceiveData[4 + i];
                                }

                               // System.Threading.Thread.Sleep(100);
                                ReceiveECGPatchIDSerialCommand();
                            }
                            else if (SerialReceiveData[1] == 0x19)
                            {

                                //if (SerialReceiveData[4] == 0)
                                //{
                                //    System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                //    string DisplayString = "蓝牙心电补丁未连接，无法获取ID\r\n";
                                //    DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                //    OutMsg(MonitorText, DisplayString, Color.Red);
                                //}
                                //else
                                {
                                    System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                    string DisplayString = "心电补丁ID获取成功！\r\n";
                                    DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                    OutMsg(MonitorText, DisplayString, Color.Red);
                                    for (int i = 0; i < 15; i++)
                                    {
                                        ECGPatchID[i] = SerialReceiveData[4 + i];
                                    }
                                }

                                this.toolStripStatusLabel2.Text = "主蓝牙设备MAC：" + BLECentralMAC[5].ToString("X2") + ":" + BLECentralMAC[4].ToString("X2")
                                    + ":" + BLECentralMAC[3].ToString("X2") + ":" + BLECentralMAC[2].ToString("X2") + ":" + BLECentralMAC[1].ToString("X2") + ":" + BLECentralMAC[0].ToString("X2")

                                    + "  连接状态：心电补丁未连接" + "   配对心电补丁MAC：" + ECGPairMAC[5].ToString("X2") + ":" + ECGPairMAC[4].ToString("X2") +
                                    ":" + ECGPairMAC[3].ToString("X2") + ":" + ECGPairMAC[2].ToString("X2") +
                                    ":" + ECGPairMAC[1].ToString("X2") + ":" + ECGPairMAC[0].ToString("X2");

                               // System.Threading.Thread.Sleep(100);
                                calibration0mvSerialCommand();
                            }
                            else if (SerialReceiveData[1] == 0x15)
                            {
                                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                                if (SerialReceiveData[4] == 0)
                                {
                                    string DisplayString = "设备未连接,无法设置成功！\r\n";
                                    DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                    OutMsg(MonitorText, DisplayString, Color.Red);
                                    System.Threading.Thread.Sleep(100);
                                    AutoConnectBLESerialCommand();
                                }
                                else if (SerialReceiveData[4] == 1)
                                {
                                    string DisplayString = "设置接收心电数据成功！\r\n";
                                    DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                    OutMsg(MonitorText, DisplayString, Color.Red);
                                    
                                }


                            }
                            else if (SerialReceiveData[1] == 0x2B)
                            {
                                string DisplayString = "获取0mv校准值成功！\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);
                                
                                difference_Value =(Int16)( SerialReceiveData[4] + (SerialReceiveData[5]<<8));
                                calibration1mvSerialCommand();
                            }
                            else if (SerialReceiveData[1] == 0x27)
                            {
                                string DisplayString = "获取1mv定标值成功！\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);

                                amplification = (Int16)(SerialReceiveData[4] + (SerialReceiveData[5] << 8)); 
                                GetHardWareVersionSerialCommand();
                            }
                            else if (SerialReceiveData[1] == 0x31)
                            {
                                string DisplayString = "获取硬件版本成功！\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);
                                GetSoftWareVersionSerialCommand();
                            }
                            else if (SerialReceiveData[1] == 0x33)
                            {
                                string DisplayString = "获取软件版本成功！\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);
                                ReceiveECGDataSerialCommand();
                            }
                            else if (SerialReceiveData[1] == 0x2D)
                            {
                                if (SerialReceiveData[4] == 1)
                                {
                                    string DisplayString = "设置0mv校准值成功！\r\n";
                                    DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                    OutMsg(MonitorText, DisplayString, Color.Red);
                                    calibration1mvSerialCommand();
                                }
                            }
                            else if (SerialReceiveData[1] == 0x29)
                            {
                                if (SerialReceiveData[4] == 1)
                                {
                                    string DisplayString = "设置1mv定标值成功！\r\n";
                                    DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                    OutMsg(MonitorText, DisplayString, Color.Red);
                                }
                            }

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
                            else if (SerialReceiveData[1] == 0x09)
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
                                    }
                                    else if (ScalingFlag == 11)
                                    {
                                        System.Text.ASCIIEncoding converter2 = new System.Text.ASCIIEncoding();
                                        string DisplayString3 = "正在0mv校准...\r\n";
                                        DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString3;
                                        OutMsg(MonitorText, DisplayString, Color.Red);
                                    }
                                    AmplificationValue.Text = Convert.ToString(amplification);
                                    differenceValue.Text = Convert.ToString(difference_Value);
                                }

                                Rectangle rect = new Rectangle();
                                rect = Screen.GetWorkingArea(this);

                                XdataV[EcgDataTimer * M + 0] = (Convert.ToDouble(SerialReceiveData[8]) + Convert.ToDouble((SerialReceiveData[12] & 0xc0) << 2) - difference_Value) * 100 / amplification + BaseLine;
                                XdataV[EcgDataTimer * M + 1] = (Convert.ToDouble(SerialReceiveData[9]) + Convert.ToDouble((SerialReceiveData[12] & 0x30) << 4) - difference_Value) * 100 / amplification + BaseLine;
                                XdataV[EcgDataTimer * M + 2] = (Convert.ToDouble(SerialReceiveData[10]) + Convert.ToDouble((SerialReceiveData[12] & 0x0c) << 6) - difference_Value) * 100 / amplification + BaseLine;
                                XdataV[EcgDataTimer * M + 3] = (Convert.ToDouble(SerialReceiveData[11]) + Convert.ToDouble((SerialReceiveData[12] & 0x03) << 8) - difference_Value) * 100 / amplification + BaseLine;
                                XdataV[EcgDataTimer * M + 4] = (Convert.ToDouble(SerialReceiveData[13]) + Convert.ToDouble((SerialReceiveData[17] & 0xc0) << 2) - difference_Value) * 100 / amplification + BaseLine;
                                XdataV[EcgDataTimer * M + 5] = (Convert.ToDouble(SerialReceiveData[14]) + Convert.ToDouble((SerialReceiveData[17] & 0x30) << 4) - difference_Value) * 100 / amplification + BaseLine;
                                XdataV[EcgDataTimer * M + 6] = (Convert.ToDouble(SerialReceiveData[15]) + Convert.ToDouble((SerialReceiveData[17] & 0x0c) << 6) - difference_Value) * 100 / amplification + BaseLine;
                                XdataV[EcgDataTimer * M + 7] = (Convert.ToDouble(SerialReceiveData[16]) + Convert.ToDouble((SerialReceiveData[17] & 0x03) << 8) - difference_Value) * 100 / amplification + BaseLine;

                                //XdataV[EcgDataTimer * M + 8] = 0;
                                //XdataV[EcgDataTimer * M + 9] = 0;
                                //XdataV[EcgDataTimer * M + 10] = 0;
                                //XdataV[EcgDataTimer * M + 11] = 0;
                                //XdataV[EcgDataTimer * M + 12] = 0;
                                //XdataV[EcgDataTimer * M + 13] = 0;
                                //XdataV[EcgDataTimer * M + 14] = 0;
                                //XdataV[EcgDataTimer * M + 15] = 0;
                                //XdataV[EcgDataTimer * M +16] = 0;


                                SerialEcgData.Add(XdataV[0]);
                                SerialEcgData.Add(XdataV[1]);
                                SerialEcgData.Add(XdataV[2]);
                                SerialEcgData.Add(XdataV[3]);
                                SerialEcgData.Add(XdataV[4]);
                                SerialEcgData.Add(XdataV[5]);
                                SerialEcgData.Add(XdataV[6]);
                                SerialEcgData.Add(XdataV[7]);


                                FileStream fs = null;
                               // string filePath = System.IO.Directory.GetCurrentDirectory() + "//ecg_data.txt";
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
                                        bytes[1] = (byte)(m_ecgdata/256);
                                        fs.Write(bytes, 0, 2);
                                    //    if (m_ecgdata < 0)
                                    //    {
                                    //        bytes[j++] = 0x2D;
                                    //        m_ecgdata = Math.Abs(m_ecgdata);
                                    //    }
                                    //    if (m_ecgdata >= 1000)
                                    //    {
                                    //        byte m1 = (byte)(m_ecgdata / 1000);
                                    //        bytes[j++] = (byte)(0x30 + m1);
                                    //    }
                                    //    if (m_ecgdata >= 100)
                                    //    {
                                    //        byte m2 = (byte)(m_ecgdata / 100);
                                    //        m2 = (byte)(m2 % 10);
                                    //        bytes[j++] = (byte)(0x30 + m2);
                                    //    }
                                    //    if (m_ecgdata >= 10)
                                    //    {
                                    //        byte m3 = (byte)(m_ecgdata / 10);
                                    //        m3 = (byte)(m3 % 10);
                                    //        bytes[j++] = (byte)(0x30 + m3);
                                    //    }
                                    //    if (m_ecgdata >= 0)
                                    //    {
                                    //        byte m4 = (byte)(m_ecgdata % 10);
                                    //        bytes[j++] = (byte)(0x30 + m4);
                                    //    }
                                    //    if (m_ecgdata == 0)
                                    //    {
                                    //        m_ecgdata = 0;
                                    //    }
                                    //    bytes[j++] = 0x0d;
                                    //    bytes[j] = 0x0a;
                                    //    fs.Write(bytes, 0, j + 1);

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
                                                ScalingEcgMin[i] = 5000;
                                            }
                                            for (int i = 0; i < 8; i++)
                                            {
                                                ScalingEcgMax[i] = -5000;
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
                                            Setcalibration1mvSerialCommand();
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
                                            difference_Value =(Int16)( m_value / 1024);
                                            amplification = amplification_Back;
                                            calibration_Value.RemoveRange(0, 1024);
                                            Setcalibration0mvSerialCommand();
                                            ScalingFlag = 12;   //完成0mv校准
                                            calibration_Num = 0;
                                        }
                                        BaseLine = 500;
                                    }
                                }

                                EcgDataTimer++;
                                if (EcgDataTimer >= (chartlenMax / M))
                                {
                                    EcgDataTimer = (chartlenMin / 8);
                                }
                                //if (SerialEcgData.Count > 512)
                                //{
                                //    SerialEcgData.RemoveRange(0, SerialEcgData.Count - 512);//从接收列表中删除包
                                //    double* ecgbuffer = stackalloc double[512];            //求导数数据区
                                //    for (int i = 0; i < 512; i++)
                                //    {
                                //        ecgbuffer[i] = SerialEcgData[i];
                                //    }
                                //    int t = rhythmcount(ecgbuffer);
                                //}

                                DataTransmissionFlag = 1;
                                BLEConnectFlagTimerOut = 0;
                                LeadOffStatus = Convert.ToInt16(SerialReceiveData[5]);
                                Vbat = Convert.ToInt16(SerialReceiveData[7] << 8) + Convert.ToInt16(SerialReceiveData[6]);
                            }
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
                    button6.Enabled = true;   //定标按键
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

                    //chart1.ChartAreas["DX"].AxisY.Maximum = chartYMax;
                    //chart1.ChartAreas["DX"].AxisY.Minimum = chartYMin;
                    //chart1.ChartAreas["DX"].AxisY.Interval = 50;

                    //chart1.ChartAreas["DX"].AxisY.Maximum = (EcgMaxValue + EcgMinValue) / 2 + 210;
                    //chart1.ChartAreas["DX"].AxisY.Minimum = (EcgMaxValue + EcgMinValue) / 2 - 210;
                    //chart1.ChartAreas["DX"].AxisY.Interval = 70;

                    //chart1.ChartAreas["DX"].AxisY.Maximum = 128;
                    //chart1.ChartAreas["DX"].AxisY.Minimum = -127;
                    //chart1.ChartAreas["DX"].AxisY.Interval = 50;

                    if (PauseButton.Text == "运行中")
                    {
                        string str = System.Text.Encoding.Default.GetString(ECGPatchID);
                        string str2;
                        if (ScalingFlag == 0)
                        { 
                            str2 = "1mv未定标    " ;
                        }
                        else if (ScalingFlag == 1)
                        {
                            str2 = "正在1mv定标...";
                        }
                        else if (ScalingFlag == 2)
                        {
                            str2 = "1mv已定标！";
                        }
                        else if (ScalingFlag == 11)
                        {
                            str2 = "正在0mv校准...";
                        }
                        else if (ScalingFlag == 12)
                        {
                            str2 = "0mv已校准！";
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
                            str3 = "配对MAC：" + ECGPairMAC[0].ToString("X2") + ":" + ECGPairMAC[1].ToString("X2") +
                                    ":" + ECGPairMAC[2].ToString("X2") + ":" + ECGPairMAC[3].ToString("X2") +
                                    ":" + ECGPairMAC[4].ToString("X2") + ":" + ECGPairMAC[5].ToString("X2");
                        }
                        this.toolStripStatusLabel2.Text = "主蓝牙设备MAC：" + BLECentralMAC[5].ToString("X2") + ":" + BLECentralMAC[4].ToString("X2")
                                    + ":" + BLECentralMAC[3].ToString("X2") + ":" + BLECentralMAC[2].ToString("X2") + ":" + BLECentralMAC[1].ToString("X2") + ":" + BLECentralMAC[0].ToString("X2")
                                    + "  连接状态：心电补丁已连接 MAC:" +
                            ECGPatchMAC[5].ToString("X2") + ":" + ECGPatchMAC[4].ToString("X2") +
                            ":" + ECGPatchMAC[3].ToString("X2") + ":" + ECGPatchMAC[2].ToString("X2")
                            + ":" + ECGPatchMAC[1].ToString("X2") + ":" + ECGPatchMAC[0].ToString("X2")
                            + "  " + "RSSI:" + Convert.ToInt16(RSSIValue) + "  ID: " + str + "  " + str2 + str3;

                       // chart1.Series["数据个数"].Points.DataBindXY(Xdata, XdataV);
                        chart1.Series["Series_Ecg"].Points.DataBindXY(Xdata, XdataV);
                    }
                }
            }
            if (ConnectBLEButton.Text == "设备已连接")
            {
                //HeartRate.Text = rate.ToString();
                BatteryValue.SelectAll();
                double batteryValue = 3300 *4 * Vbat /1024 ;
               // double batteryValue = Vbat;
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
                    Lead.Text = "导联脱落";
                    Lead.SelectAll();
                    Lead.SelectionColor = Color.Red;
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
            saveFileDialog1.Filter = "bin文件(*.hex)|*.hex|文本文件(*.txt)|*.txt";
            saveFileDialog1.Title = "保存心电数据";
            saveFileDialog1.FilterIndex = 1;
            saveFileDialog1.RestoreDirectory = true;
            string FileName = "ecg_data"+"  " + StartStoreDataTime +"--"+ DateTime.Now.ToString("yyyy-MM-dd") + "-" + DateTime.Now.Hour.ToString() + "-" + DateTime.Now.Minute.ToString() + ".hex"; // 
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
            if (DebugMode == 1)
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

            else if (DebugMode == 2)
            {
                byte[] ssss = { 0x77, 0x01, 0x00, 0x00 };
                SerialPort.Write(ssss, 0, 4);
            }
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
            if (DebugMode == 1)
            {
                byte[] ssss = { 01, 04, 0xFE, 03, 03, 01, 00 };
                SerialPort.Write(ssss, 0, 7);

                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                string DisplayString = "Send <GAP_DeviceDiscoveryRequest> Command!!!\r\n";
                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                OutMsg(MonitorText, DisplayString, Color.Red);
            }
            else if (DebugMode == 2)
            {
                byte[] ssss = { 0x77, 0x03, 0x00, 0x00 };
                SerialPort.Write(ssss, 0, 4);
                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                string DisplayString = "请求搜索设备...\r\n";
                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                OutMsg(MonitorText, DisplayString, Color.Red);
            }

            MACComboBox.Items.Clear();
        }
        private void ConnectBLESerialCommand()
        {
            if (DebugMode == 1)
            {
                byte[] ssss = { 01, 09, 0xFE, 0x09, 00, 00, 02, 0xE6, 0x39, 00, 0x0B, 0x0E, 00 };

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
            else if (DebugMode == 2)
            {
                byte[] ssss = { 0x77, 0x05, 0x00, 0x01,0x00 };
                ssss[4] = (byte)MACComboBox.SelectedIndex;
                SerialPort.Write(ssss, 0, 5);
                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                string DisplayString = "请求连接设备...\r\n";
                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                OutMsg(MonitorText, DisplayString, Color.Red);
                DisConnectBLEEnableFlag = 0;
            }
        }
        private void DisconnectBLESerialCommand()
        {
            if (DebugMode == 1)
            {
                byte[] ssss = { 01, 0x0A, 0xFE, 03, 00, 00, 0x13 };
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
                        XdataV[i] = -50001;
                    }
                    chart1.Series["Series_Ecg"].Points.DataBindXY(Xdata, XdataV);

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
            else if (DebugMode == 2)
            {
                if (!SerialPort.IsOpen)   //检测串口是否关闭
                {
                    return;
                }
                byte[] ssss = { 0x77, 0x0A, 0x00, 0x00};
                SerialPort.Write(ssss, 0, 4);
                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                string DisplayString = "请求断开设备...\r\n";
                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                OutMsg(MonitorText, DisplayString, Color.Red);
                DisConnectBLEEnableFlag = 1;
                this.toolStripStatusLabel2.Text = "蓝牙设备状态：未连接";
            }
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
        private void ReceiveECGDataSerialCommand()
        {
            if (DebugMode == 1)
            {
                ;
            }
            else if (DebugMode == 2)
            {
                byte[] ssss = { 0x77, 0x14, 0x00, 0x00 };
                SerialPort.Write(ssss, 0, 4);
                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                string DisplayString = "请求接收心电数据...\r\n";
                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                OutMsg(MonitorText, DisplayString, Color.Red);
            }

        }
        private void StopReceiveECGDataSerialCommand()
        {
            if (DebugMode == 1)
            {
                ;
            }
            else if (DebugMode == 2)
            {
                byte[] ssss = { 0x77, 0x16, 0x00, 0x00 };
                SerialPort.Write(ssss, 0, 4);
                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                string DisplayString = "请求停止接收心电数据...\r\n";
                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                OutMsg(MonitorText, DisplayString, Color.Red);
            }

        }
        private void ReceiveECGPatchIDSerialCommand()
        {
            if (DebugMode == 1)
            {
                ;
            }
            else if (DebugMode == 2)
            {
                byte[] ssss = { 0x77, 0x18, 0x00, 0x00 };
                SerialPort.Write(ssss, 0, 4);
                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                string DisplayString = "请求接收心电补丁ID\r\n";
                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                OutMsg(MonitorText, DisplayString, Color.Red);
            }

        }
        private void ReceiveCentralMACCommand()
        {
            if (DebugMode == 1)
            {
                ;
            }
            else if (DebugMode == 2)
            {
                byte[] ssss = { 0x77, 0x1C, 0x00, 0x00 };
                SerialPort.Write(ssss, 0, 4);
                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                string DisplayString = "请求获取主设备蓝牙MAC\r\n";
                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                OutMsg(MonitorText, DisplayString, Color.Red);
            }

        }
        private void ReceiveECGPatchMACCommand()
        {
            if (DebugMode == 1)
            {
                ;
            }
            else if (DebugMode == 2)
            {
                byte[] ssss = { 0x77, 0x1E, 0x00, 0x00 };
                SerialPort.Write(ssss, 0, 4);
                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                string DisplayString = "请求获取心电补丁蓝牙MAC\r\n";
                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                OutMsg(MonitorText, DisplayString, Color.Red);
            }

        }
        private void ReceivePairingStatusCCommand()
        {
            if (DebugMode == 1)
            {
                ;
            }
            else if (DebugMode == 2)
            {
                byte[] ssss = { 0x77, 0x22, 0x00, 0x00 };
                //   ECGPairMAC
                SerialPort.Write(ssss, 0, 4);
                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                string DisplayString = "请求获取配对状态\r\n";
                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                OutMsg(MonitorText, DisplayString, Color.Red);
            }

        }
        private void SetTestModeCommand()
        {
            if (DebugMode == 1)
            {
                ;
            }
            else if (DebugMode == 2)
            {
                byte[] ssss = { 0x77, 0x24, 0x00, 0x01, 0x01 };
                SerialPort.Write(ssss, 0, 5);
                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                string DisplayString = "设置为测试模式\r\n";
                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                OutMsg(MonitorText, DisplayString, Color.Red);
            }

        }
        private void AutoConnectBLESerialCommand()
        {
            if (DebugMode == 1)
            {
                ;
            }
            else if (DebugMode == 2)
            {
                byte[] ssss = { 0x77, 0x11, 0x00, 0x01, 0x01 };
                SerialPort.Write(ssss, 0, 5);
                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                string DisplayString = "请求自动连接配置...\r\n";
                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                OutMsg(MonitorText, DisplayString, Color.Red);
            }

        }
        private void DisAutoConnectBLESerialCommand()
        {
            if (DebugMode == 1)
            {
                ;
            }
            else if (DebugMode == 2)
            {
                byte[] ssss = { 0x77, 0x11, 0x00, 0x01, 0x00 };
                SerialPort.Write(ssss, 0, 5);
                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                string DisplayString = "请求关闭自动连接配置...\r\n";
                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                OutMsg(MonitorText, DisplayString, Color.Red);
            }

        }
        private void AutoConnectBLEStatusSerialCommand()
        {
            if (DebugMode == 1)
            {
                ;
            }
            else if (DebugMode == 2)
            {
                byte[] ssss = { 0x77, 0x1A, 0x00, 0x00 };
                SerialPort.Write(ssss, 0, 4);
                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                string DisplayString = "获取自动连接配置状态...\r\n";
                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                OutMsg(MonitorText, DisplayString, Color.Red);
            }
        }
        private void PairingCommand()
        {
            if (DebugMode == 1)
            {
                ;
            }
            else if (DebugMode == 2)
            {
                byte[] ssss = { 0x77, 0x20, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

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
                for (int i = 0; i < 6; i++)
                {
                    ssss[4 + i] = (byte)((cc[(5 - i) * 3] << 4) + (cc[(5 - i) * 3 + 1]));
                }


                //   ECGPairMAC
                SerialPort.Write(ssss, 0, 10);
                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                string DisplayString = "请求设置配对心电补丁的MAC\r\n";
                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                OutMsg(MonitorText, DisplayString, Color.Red);
            }

        }
        private void calibration0mvSerialCommand()
        {
            if (DebugMode == 1)
            {
                ;
            }
            else if (DebugMode == 2)
            {
                byte[] ssss = { 0x77, 0x2A, 0x00, 0x00 };
                SerialPort.Write(ssss, 0, 4);
                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                string DisplayString = "请求获取0mv校准值...\r\n";
                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                OutMsg(MonitorText, DisplayString, Color.Red);
            }
        }
        private void calibration1mvSerialCommand()
        {
            if (DebugMode == 1)
            {
                ;
            }
            else if (DebugMode == 2)
            {
                byte[] ssss = { 0x77, 0x26, 0x00, 0x00 };
                SerialPort.Write(ssss, 0, 4);
                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                string DisplayString = "请求获取1mv定标值...\r\n";
                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                OutMsg(MonitorText, DisplayString, Color.Red);
            }
        }

        private void Setcalibration0mvSerialCommand()
        {
            if (DebugMode == 1)
            {
                ;
            }
            else if (DebugMode == 2)
            {
                byte[] ssss = { 0x77, 0x2C, 0x00, 0x02,0x00,0x00 };
                int m_buffer = difference_Value%256;
                ssss[4] = (byte)m_buffer;
                m_buffer = difference_Value / 256;
                ssss[5] = (byte)m_buffer;
                SerialPort.Write(ssss, 0, 6);

                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                string DisplayString = "保存0mv校准值...\r\n";
                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                OutMsg(MonitorText, DisplayString, Color.Red);
            }
        }
        private void Setcalibration1mvSerialCommand()
        {
            if (DebugMode == 1)
            {
                ;
            }
            else if (DebugMode == 2)
            {
                byte[] ssss = { 0x77, 0x28, 0x00, 0x02,0x00,0x00 };
                int m_buffer = amplification % 256;
                ssss[4] = (byte)m_buffer;
                m_buffer = amplification / 256;
                ssss[5] = (byte)m_buffer;
                SerialPort.Write(ssss, 0, 6);

                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                string DisplayString = "保存1mv定标值...\r\n";
                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                OutMsg(MonitorText, DisplayString, Color.Red);
            }
        }
        private void GetHardWareVersionSerialCommand()
        {
            if (DebugMode == 1)
            {
                ;
            }
            else if (DebugMode == 2)
            {
                byte[] ssss = { 0x77, 0x30, 0x00, 0x00 };
                SerialPort.Write(ssss, 0, 4);
                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                string DisplayString = "请求获取硬件版本...\r\n";
                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                OutMsg(MonitorText, DisplayString, Color.Red);
            }
        }
        private void GetSoftWareVersionSerialCommand()
        {
            if (DebugMode == 1)
            {
                ;
            }
            else if (DebugMode == 2)
            {
                byte[] ssss = { 0x77, 0x32, 0x00, 0x00 };
                SerialPort.Write(ssss, 0, 4);
                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                string DisplayString = "请求获取软件版本...\r\n";
                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                OutMsg(MonitorText, DisplayString, Color.Red);
            }
        }
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
            //BLEConnectFlagTimerOut++;
            //if (BLEConnectFlagTimerOut >= 50)
            //{
            //    BLEConnectFlagTimerOut = 0;
            //    if (ConnectBLEButton.Text == "设备已连接")
            //    {
            //        DisconnectBLESerialCommand();
            //        ConnectBLEButton.Enabled = false;
            //        ConnectBLEButton.Text = "设备正在断开";
            //    }
            //    if (!SerialPort.IsOpen)   //检测串口是否关闭
            //    {
            //        return;
            //    }
                //if (BLEConnectFlag == 0)
                //{
                //    if (checkBox1.Checked == true)
                //    {
                //        AutoConnectBLESerialCommand();
                //    }
                //    else
                //    {
                //        DisAutoConnectBLESerialCommand();
                //    }
                //}
            //}
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
 
        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox1.Checked == true)
            {
                AutoConnectBLESerialCommand();
            }
            else
            {
                DisAutoConnectBLESerialCommand();
            }
        }

        private void button3_Click_1(object sender, EventArgs e)
        {
            ReceiveECGDataSerialCommand();
        }

        private void button4_Click_2(object sender, EventArgs e)
        {
            StopReceiveECGDataSerialCommand();
        }

        private void button5_Click(object sender, EventArgs e)
        {
            ScalingFlag = 1;    //开始1mv定标
            amplification_Back = amplification;
            difference_Value_Back = difference_Value;
            difference_Value = 0;
            amplification = 100;
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
            PairingCommand();
        }

        private void axMSComm1_OnComm(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            ScalingFlag = 11;    //开始0mv校准

            amplification_Back = amplification;
            difference_Value_Back = difference_Value;
            difference_Value = 0;
            amplification = 100;
            BaseLine = 0;
            System.Threading.Thread.Sleep(100);
        }
    }
}
