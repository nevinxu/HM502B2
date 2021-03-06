﻿using System;
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
using System.Collections;

namespace MotionSensor
{
    public partial class RF : Form
    {
        const int N = 64;
        const int M = 8; //每次接收到包心电采样点
        string[] Xdata = new string[M * N];
        double[] XdataV = new double[M * N];

        private double[,] EcgData_Com = new double[64, 8];   //原始采集到的心电数据
        private List<double> EcgData_Pulse = new List<double>(512);

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
        private List<byte> SerialReceiveData5 = new List<byte>(40960);//默认分配1页内存，并始终限制不允许超
        private List<double> SerialEcgData = new List<double>(512);//默认分配1页内存，并始终限制不允许超



        private byte EcgCaptureFlag = 0;
        private byte BLEConnectFlag = 0;

        private int ScanBLENum = 0;
        private byte[,] ScanBLEMAC = new byte[100, 6];
        private byte[,] ScanBleName = new byte[100, 11];

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
        byte[] ECGSetPairMAC = new byte[6];
        private int EcgDataTimer = 0;

        private Int16 amplification = 60;
        private Int16 difference_Value = 525;

        private Int16 amplification_Back = 1;
        private Int16 difference_Value_Back = 0;

        private int ScalingFlag = 0;
        private int calibration_Num = 0;
        private List<double> calibration_Value = new List<double>(4096);
        private double[] ScalingEcgMin = new double[8];
        private double[] ScalingEcgMax = new double[8];

        private int ZeroValue = 5000;
        private int DisplayMultipleNum = 10;
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

        Thread ScanEcgUartThread;
        Thread EcgUartProcessThread;

        Thread ScanUpdataThread;
        Thread[] UpDataThread = new Thread[4];
        private int DowmLoadNum = 0;

        private List<string> GetComNameList = new List<String>(100);
        private SerialPort[] UpDataCom = new SerialPort[4];
        private ProgressBar[] UpDataProgressBar = new ProgressBar[4];
        private CheckBox[] UpDataCheckBox = new CheckBox[4];
        private Label[] UpDataLabel = new Label[4];

        private int UPDATACOMNUM = 4;

        private SerialPort EcgCom = new SerialPort();
        private int EcgComReceiveFlag = 0;
        private int ScanDeviceFlag = 0;
        private int ConnectDeviceFlag = 0;
        private int PairDeviceFlag = 0;


        public RF()
        {
            InitializeComponent();
        }


        public double[] SoAndChan(double[] voltages)
        {
            const double THRESHOLD_PARAM = 8;
            const double FILTER_PARAMETER = 16;
            const int SAMPLE_RATE = 510;

            // initial maxi should be the max slope of the first 250 points.
            double initial_maxi = -2 * voltages[0] - voltages[1] + voltages[3] + 2 * voltages[4];
            for (int i = 2; i < SAMPLE_RATE; i++)
            {
                double slope = -2 * voltages[i - 2] - voltages[i - 1] + voltages[i + 1] + 2 * voltages[i + 2];
                if (slope > initial_maxi)
                    initial_maxi = slope;
            }

            // Since we don't know how many R peaks we'll have, we'll use an ArrayList
            List<double> rTime = new List<double>(100);
          //  ArrayList rTime = new ArrayList();

            // set initial maxi
            double maxi = initial_maxi;
            bool first_satisfy = false;
            bool second_satisfy = false;
            int onset_point = 0;
            int R_point = 0;
            bool rFound = false;
            // I want a way to plot all the r dots that are found...
            int[] rExist = new int[voltages.Length];
            // First two voltages should be ignored because we need rom length
            for (int i = 2; i < voltages.Length - 2; i++)
            {

                // Last two voltages should be ignored too
                if (!first_satisfy || !second_satisfy)
                {
                    // Get Slope:
                    double slope = -2 * voltages[i - 2] - voltages[i - 1] + voltages[i + 1] + 2 * voltages[i + 2];

                    // Get slope threshold
                    double slope_threshold = (THRESHOLD_PARAM / 16) * maxi;

                    // We need two consecutive datas that satisfy slope > slope_threshold
                    if (slope > slope_threshold)
                    {
                        if (!first_satisfy)
                        {
                            first_satisfy = true;
                            onset_point = i;
                        }
                        else
                        {
                            if (!second_satisfy)
                            {
                                second_satisfy = true;
                            }
                        }
                    }
                }
                // We found the ONSET already, now we find the R point
                else
                {

                    if (voltages[i] < voltages[i - 1])
                    {
                        rTime.Add(i - 1);
                        R_point = i - 1;

                        // Since we have the R, we should reset
                        first_satisfy = false;
                        second_satisfy = false;

                        // and update maxi
                        double first_maxi = voltages[R_point] - voltages[onset_point];
                        maxi = ((first_maxi - maxi) / FILTER_PARAMETER) + maxi;
                    }
                }
            }

            double[] results = new double[rTime.Count];

            // Now we convert the ArrayList to an array and return it
            for (int i = 0; i < rTime.Count; i++)
            {
                results[i] = rTime[i];
            }

            return results;
        }


        int TestFlagCharge(int Flag,string DisplayString,int delay)
        {
            int num = 0;
            while (EcgComReceiveFlag == Flag)
            {
                Thread.Sleep(1);
                num++;
                if (num >= delay)
                {
                    OutMsg(MonitorText, DisplayString, Color.Red);
                    return -1;
                }
            }
            return 0;
        }

        void HideWave()
        {
            for (int i = 0; i < (N * M); i++)
            {
                XdataV[i] = 0;
            }
            chart1.Series["Series_Ecg"].Points.DataBindXY(Xdata, XdataV);
            chart1.Series["Series_Ecg"].Color = Color.Black;
        }

        int TestCommandGetStatus(int EcgComReceiveFlag1, int EcgComReceiveFlag2, string DisplayString1, string DisplayString2, string DisplayString3,int delay)
        {
            EcgComReceiveFlag = EcgComReceiveFlag1;
            string DisplayString = "BLE主机串口(" + ConnectUartName[4] + ")：" + DisplayString1;
            OutMsg(MonitorText, DisplayString, Color.Red);

            DisplayString = "BLE主机串口(" + ConnectUartName[4] + ")：" + DisplayString2;
            if (TestFlagCharge(EcgComReceiveFlag1, DisplayString, delay) != 0)
            {
                return -1;
            }
            if (EcgComReceiveFlag == EcgComReceiveFlag2)
            {
                DisplayString = "BLE主机串口(" + ConnectUartName[4] + ")：" + DisplayString3;
                OutMsg(MonitorText, DisplayString, Color.Red);
            }
            else
            {
                return -2;
            }
            return 0;
        }


        void DisplayScanDevice()
        {
            byte[] m_blename = new byte[11];
            for (int i = 0; i < ScanBLENum; i++)
            {
                for (int j = 0; j < 11; j++)
                {
                    m_blename[j] = ScanBleName[i, j];
                }
                string aa = Encoding.UTF8.GetString(m_blename);
                string bb = "(" + ScanBLEMAC[i, 5].ToString("X2") + ":" + ScanBLEMAC[i, 4].ToString("X2") + ":" + 
                    ScanBLEMAC[i, 3].ToString("X2") + ":" + ScanBLEMAC[i, 2].ToString("X2") + ":" + 
                    ScanBLEMAC[i, 1].ToString("X2") + ":" + ScanBLEMAC[i, 0].ToString("X2") + ")" + aa;
                MAClistBox.Invoke(new EventHandler(delegate
                {
                    if (ScanBleName[i, 0] == 'B' && (ScanBleName[i, 1] == 'D'))
                    {
                        MAClistBox.Items.Add(bb + "(心电补丁)");
                    }
                    else if (ScanBleName[i, 0] == 'B' && (ScanBleName[i, 1] == 'G'))
                    {
                        MAClistBox.Items.Add(bb + "(血压计)");
                    }
                    else
                    {
                        MAClistBox.Items.Add(bb + "(未知设备)");
                    }
                //    MACComboBox.Enabled = true;
                }));
            }
            if (ScanBLENum == 0)
            {
                //MACComboBox.Invoke(new EventHandler(delegate
                //{
                //MAClistBox.Items.Add ("无设备");
                //}));
            }
        }

        void EcgUartProcessFunction()
        {
            string DisplayString1 = "";
            string DisplayString2 = "";
            string DisplayString3 = "";

            if(ScanEcgUartThread.IsAlive)
            {
                ScanEcgUartThread.Abort();
            }
            while (EcgComConnectFlag == 1)
            {
                if (ScanDeviceFlag == 1)
                {
                        DisplayString1 = "请求搜索设备...\r\n";
                        DisplayString2 = "搜索设备失败！\r\n";
                        DisplayString3 = "搜索设备成功！\r\n";
                        serialcommand.SendDisAdvertiseSerialCommand(EcgCom);
                        if (TestCommandGetStatus(0x31, 0x32, DisplayString1, DisplayString2, DisplayString3, 1000) == 0)
                        {
                            DisplayScanDevice();
                            ScanButton.Invoke(new EventHandler(delegate
                            {
                                ScanButton.Text = "搜索完成";
                                ScanButton.Enabled = false;
                                MAClistBox.Enabled = true;
                            }));
                            Thread.Sleep(1000);
                            ScanButton.Invoke(new EventHandler(delegate
                            {
                                ScanButton.Text = "搜索设备";
                                ScanButton.Enabled = true;
                            }));
                        }
                        else
                        {
                            ScanButton.Invoke(new EventHandler(delegate
                            {
                                ScanButton.Text = "搜索失败，请重新搜索";
                                ScanButton.Enabled = true;
                            }));
                        }
                        ScanDeviceFlag = 0;
                }
                else if (ConnectDeviceFlag == 1)  //连接请求
                {
                    DisplayString1 = "请求连接设备...\r\n";
                    DisplayString2 = "连接设备失败！\r\n";
                    DisplayString3 = "连接设备成功！\r\n";
                    MAClistBox.Invoke(new EventHandler(delegate
                    {
                        serialcommand.ConnectBLESerialCommand(EcgCom, (byte)MAClistBox.SelectedIndex);
                    }));
                    if (TestCommandGetStatus(0x41, 0x42, DisplayString1, DisplayString2, DisplayString3, 300) == 0)
                    {
                        ScanButton.Invoke(new EventHandler(delegate
                        {
                            ScanButton.Text = "设备已连接";
                            ScanButton.Enabled = true;
                            PairButton.Enabled = false;
                            MAClistBox.Invoke(new EventHandler(delegate
                            {
                                MAClistBox.Enabled = false;
                            }));
                            ConnectDeviceFlag = 2;
                            button5.Enabled = true;
                            button1.Enabled = true;
                        }));
                    }
                    else 
                    {
                        if (EcgComReceiveFlag == 0x43)
                        {
                            ScanButton.Invoke(new EventHandler(delegate
                            {
                                ScanButton.Text = "配对MAC出错";
                                ScanButton.Enabled = false;
                                button5.Enabled = false;
                                button1.Enabled = false;
                            }));
                        }
                        else if (EcgComReceiveFlag == 0x41)
                        {
                            ScanButton.Invoke(new EventHandler(delegate
                            {
                                ScanButton.Text = "连接失败";
                                ScanButton.Enabled = false;
                                button5.Enabled = false;
                                button1.Enabled = false;
                            }));
                        }
                        Thread.Sleep(1000);
                        ScanButton.Invoke(new EventHandler(delegate
                        {
                            ScanButton.Text = "搜索设备";
                            ScanButton.Enabled = true;
                            PairButton.Enabled = true;
                            ConnectDeviceFlag = 0;
                        }));
                    }
                }
                else if (ConnectDeviceFlag == 3) //请求断开
                {
                    DisplayString1 = "请求断开心电补丁...\r\n";
                    DisplayString2 = "断开心电补丁失败！\r\n";
                    DisplayString3 = "心电补丁已断开！\r\n";
                    serialcommand.DisconnectBLESerialCommand(EcgCom);
                    
                    if (TestCommandGetStatus(3, 4, DisplayString1, DisplayString2, DisplayString3, 500) == 0)
                    {
                        ScanButton.Invoke(new EventHandler(delegate
                        {
                            ScanButton.Text = "设备已断开";
                            MAClistBox.Enabled = true;
                            ScanButton.Enabled = false;
                            toolStripStatusLabel2.Text = "";
                            toolStripStatusLabel3.Text = "";
                            button5.Enabled = false;
                            button1.Enabled = false;

                            LeadLabel.Text = "";
                            BatteryValueLabel.Text = "";
                            PulseLabel.Text = "";

                            HideWave();

                        }));
                        Thread.Sleep(1000);
                        ScanButton.Invoke(new EventHandler(delegate
                        {
                            ScanButton.Text = "搜索设备";
                            MAClistBox.Enabled = true;
                            ConnectDeviceFlag = 0;
                            ScanButton.Enabled = true;
                            PairButton.Enabled = true;

                        }));
                    }
                }
                else if (ConnectDeviceFlag == 2)   //已连接
                {
                    DisplayString1 = "请求停止接收心电数据...\r\n";
                    DisplayString2 = "请求停止接收心电数据失败！\r\n";
                    DisplayString3 = "请求停止接收心电数据成功！\r\n";
                    serialcommand.StopReceiveECGDataSerialCommand(EcgCom);
                    TestCommandGetStatus(5, 6, DisplayString1, DisplayString2, DisplayString3, 500);

                    DisplayString1 = "请求获取硬件版本...\r\n";
                    DisplayString2 = "获取硬件版本失败！\r\n";
                    DisplayString3 = "获取硬件版本成功！\r\n";
                    serialcommand.GetHardWareVersionSerialCommand(EcgCom);
                    if (TestCommandGetStatus(0x71, 0x72, DisplayString1, DisplayString2, DisplayString3, 500) == 0)
                    {
                        ;
                    }
                    else
                    {
                        ;
                    }

                    DisplayString1 = "请求获取软件版本...\r\n";
                    DisplayString2 = "获取软件版本失败！\r\n";
                    DisplayString3 = "获取软件版本成功！\r\n";
                    serialcommand.GetSoftWareVersionSerialCommand(EcgCom);
                    if (TestCommandGetStatus(0x73, 0x74, DisplayString1, DisplayString2, DisplayString3, 500) == 0)
                    {
                        ;
                    }
                    else
                    {
                        ;
                    }
                    DisplayString1 = "请求获取0mv校准值...\r\n";
                    DisplayString2 = "获取0mv校准值失败！\r\n";
                    DisplayString3 = "获取0mv校准值成功！\r\n";
                    serialcommand.calibration0mvSerialCommand(EcgCom);
                    if (TestCommandGetStatus(0x81, 0x82, DisplayString1, DisplayString2, DisplayString3, 500) == 0)
                    {
                        ;
                    }
                    else
                    {
                        ;
                    }

                    DisplayString1 = "请求获取1mv校准值...\r\n";
                    DisplayString2 = "获取1mv校准值失败！\r\n";
                    DisplayString3 = "获取1mv校准值成功！\r\n";
                    serialcommand.calibration1mvSerialCommand(EcgCom);
                    if (TestCommandGetStatus(0x83, 0x84, DisplayString1, DisplayString2, DisplayString3, 500) == 0)
                    {
                        ;
                    }
                    else
                    {
                        ;
                    }
                    toolStripStatusLabel2.Text = "当前心电补丁 1mv校准值: " + amplification.ToString() + "    0mv校准值： " + difference_Value.ToString();


                    toolStripStatusLabel3.Text = "硬件版本:" + Encoding.UTF8.GetString(HardVersion) + "   软件版本:" + Encoding.UTF8.GetString                       (SoftVersion);

                    DisplayString1 = "请求接收心电数据...\r\n";
                    DisplayString2 = "接收心电数据失败！\r\n";
                    DisplayString3 = "接收心电数据成功！\r\n";
                    serialcommand.ReceiveECGDataSerialCommand(EcgCom);
                    if (TestCommandGetStatus(0x51, 0x52, DisplayString1, DisplayString2, DisplayString3, 500) == 0)
                    {
                        ConnectDeviceFlag = 4;
                    }
                    else
                    {
                        ;
                    }
                }
                else if (ConnectDeviceFlag == 4)
                {
                    if (EcgComReceiveFlag == 4)
                    {
                        ConnectDeviceFlag = 0;
                        ScanButton.Invoke(new EventHandler(delegate
                        {
                            ScanButton.Text = "连接断开";
                            ScanButton.Enabled = false;
                        }));
                        Thread.Sleep(1000);
                        ScanButton.Invoke(new EventHandler(delegate
                        {
                            ScanButton.Text = "搜索设备";
                            ScanButton.Enabled = true;
                        }));
                    }
                    else
                    {

                        for (int i = 0; i < 512; i++)
                        {
                            XdataV[i] = EcgData_Com[i / 8, i % 8];
                        }

                        

                        chart1.Invoke(new EventHandler(delegate
                        {
                            if (LeadOffStatus == 0)
                            {
                                LeadLabel.Text = "导联脱落";
                                LeadLabel.ForeColor = Color.Red;
                            }
                            else
                            {
                                LeadLabel.Text = "导联正常";
                                LeadLabel.ForeColor = Color.Green;

                            }

                            BatteryValueLabel.Text = "当前电池电量:" + Vbat.ToString() + "%";

                          //  
                            if (EcgData_Pulse.Count >= 512)
                            {
                                double[] ff = new double[512];
                                for (int i = 0; i < 512; i++)
                                {
                                    ff[i] = EcgData_Pulse[i];
                                }
                                double[] aa = SoAndChan(ff);

                                int bb = aa.GetLength(0);
                                if (bb >= 2)
                                {
                                    List<double> cc = new List<double>(100);
                                    for (int i = 1; i < bb; i++)
                                    {
                                        cc.Add(aa[i] - aa[i - 1]);
                                    }
                                    double dd = new double();
                                    for (int i = 0; i < cc.Count(); i++)
                                    {
                                        dd += cc[i];
                                    }
                                    dd = dd / cc.Count();

                                    dd = 60 * 128 / dd + 0.5;

                                    dd = (double)(int)(dd);
                                    PulseLabel.Text = "当前心率：" + dd.ToString();

                                }
                            }
                            else
                            {
                                PulseLabel.Text = "正在计算心率...";
                            }

                            chart1.Series["Series_Ecg"].Points.DataBindXY(Xdata, XdataV);
                            chart1.Series["Series_Ecg"].Color = Color.FromArgb(0, 192, 0);
                            if (EcgDataTimer < 64 && (EcgDataTimer >= 0))
                            {
                                chart1.Series["Series_Ecg"].Points.ElementAt(EcgDataTimer * M + 2).Color = Color.Black;
                                chart1.Series["Series_Ecg"].Points.ElementAt(EcgDataTimer * M + 3).Color = Color.Black;
                                chart1.Series["Series_Ecg"].Points.ElementAt(EcgDataTimer * M + 4).Color = Color.Black;
                                chart1.Series["Series_Ecg"].Points.ElementAt(EcgDataTimer * M + 5).Color = Color.Black;
                                chart1.Series["Series_Ecg"].Points.ElementAt(EcgDataTimer * M + 6).Color = Color.Black;
                                chart1.Series["Series_Ecg"].Points.ElementAt(EcgDataTimer * M + 7).Color = Color.Black;
                            }
                            //for (int i = 1; i < 512; i++)
                            //{
                            //    if ((XdataV[i] > 7000) || (XdataV[i] < -7000))
                            //    {
                            //        chart1.Series["Series_Ecg"].Points.ElementAt(i).Color = Color.Black;
                            //        chart1.Series["Series_Ecg"].Points.ElementAt(i - 1).Color = Color.Black;
                            //    }
                            //}


                        }));
                        ScalingFuction();
                        Thread.Sleep(100);
                    }
                }
                else if (ConnectDeviceFlag == 5)
                {
                    ;
                }
                else if (PairDeviceFlag == 1)
                {
                    DisplayString1 = "请求设置配对MAC...\r\n";
                    DisplayString2 = "设置配对MAC失败！\r\n";
                    DisplayString3 = "设置配对MAC成功！\r\n";
                    serialcommand.PairingCommand(EcgCom, ECGSetPairMAC);
                    if (TestCommandGetStatus(0x61, 0x62, DisplayString1, DisplayString2, DisplayString3, 500) == 0)
                    {
                        MAClistBox.Invoke(new EventHandler(delegate
                        {
                            string aa = ECGPairMAC[5].ToString("X2") + ":" + ECGPairMAC[4].ToString("X2") + ":" + ECGPairMAC[3].ToString("X2") + ":" + ECGPairMAC[2].ToString("X2") + ":" + ECGPairMAC[1].ToString("X2") + ":" + ECGPairMAC[0].ToString("X2");
                            PatchMACLabel.Text = "当前配对的MAC：" + aa;
                            toolStripStatusLabel4.Text = "当前配对的MAC：" + aa;
                            PairButton.Text = "配对完成";
                            PairButton.Enabled = false;
                        }));
                        Thread.Sleep(1000);
                        MAClistBox.Invoke(new EventHandler(delegate
                        {
                            PairButton.Text = "配对";
                            PairButton.Enabled = true;
                        }));
                    }
                    else
                    {
                        MAClistBox.Invoke(new EventHandler(delegate
                        {
                            PairButton.Text = "配对失败，请重新配对";
                        }));
                    }
                    PairDeviceFlag = 0;

                }
                else
                {
                    Thread.Sleep(5);
                    DisplayString1 = "请求断开心电补丁...\r\n";
                    DisplayString2 = "断开心电补丁失败！\r\n";
                    DisplayString3 = "心电补丁已断开！\r\n";
                    serialcommand.DisconnectBLESerialCommand(EcgCom);
                    TestCommandGetStatus(3, 4, DisplayString1, DisplayString2, DisplayString3, 500);

                    DisplayString1 = "请求停止接收心电数据...\r\n";
                    DisplayString2 = "请求停止接收心电数据失败！\r\n";
                    DisplayString3 = "请求停止接收心电数据成功！\r\n";
                    serialcommand.StopReceiveECGDataSerialCommand(EcgCom);
                    TestCommandGetStatus(5, 6, DisplayString1, DisplayString2, DisplayString3, 500);

                    DisplayString1 = "请求关闭自动连接配置...\r\n";
                    DisplayString2 = "请求关闭自动连接配置失败！\r\n";
                    DisplayString3 = "关闭自动连接配置成功！\r\n";
                    serialcommand.DisAutoConnectBLESerialCommand(EcgCom);
                    TestCommandGetStatus(7, 8, DisplayString1, DisplayString2, DisplayString3, 500);

                    DisplayString1 = "请求获取自动连接配置状态...\r\n";
                    DisplayString2 = "获取自动连接配置状态失败！\r\n";
                    DisplayString3 = "获取自动连接配置状态成功！\r\n";
                    serialcommand.AutoConnectBLEStatusSerialCommand(EcgCom);
                    TestCommandGetStatus(9, 10, DisplayString1, DisplayString2, DisplayString3, 500);

                    DisplayString1 = "请求获取配对MAC...\r\n";
                    DisplayString2 = "获取配对MAC失败！\r\n";
                    DisplayString3 = "获取配对MAC成功！\r\n";
                    serialcommand.ReceivePairingStatusCCommand(EcgCom);
                    if (TestCommandGetStatus(0x10, 0x11, DisplayString1, DisplayString2, DisplayString3, 500) == 0)
                    {
                        MAClistBox.Invoke(new EventHandler(delegate
                        {
                            string aa = ECGPairMAC[5].ToString("X2") + ":" + ECGPairMAC[4].ToString("X2") + ":" + ECGPairMAC[3].ToString("X2") + ":" + ECGPairMAC[2].ToString("X2") + ":" + ECGPairMAC[1].ToString("X2") + ":" + ECGPairMAC[0].ToString("X2");
                            PatchMACLabel.Text = "当前配对的MAC：" + aa;
                            toolStripStatusLabel4.Text = "当前配对的MAC：" + aa;
                        }));
                    }

                    DisplayString1 = "请求设置测试模式...\r\n";
                    DisplayString2 = "设置测试模式失败！\r\n";
                    DisplayString3 = "设置测试模式成功！\r\n";
                    serialcommand.SetTestModeCommand(EcgCom);
                    TestCommandGetStatus(0x12, 0x13, DisplayString1, DisplayString2, DisplayString3, 500);
                    Thread.Sleep(1000);
                }
            }
            ScanEcgUartThread = new Thread(new ThreadStart(ScanEcgUartFunction));
            ScanEcgUartThread.Start();
        }

        void ScanEcgUartFunction()
        {
            Int16 ScanSequence = 0;
            while (true)
            {
                if (EcgComConnectFlag == 0x00)
                {
                    GetComList(GetComNameList);    //获取当前所有的串口
                    if (ScanSequence >= GetComNameList.Count() - 1)
                    {
                        GetComList(GetComNameList);    //获取当前所有的串口
                        ScanSequence = 0;
                    }
                    else
                    {
                        ScanSequence++;
                    }
                    ConnectUartName[4] = GetComNameList[ScanSequence];
                }
                string DisplayString = "BLE主机串口：请求连接" + ConnectUartName[4] + "...\r\n";
                OutMsg(MonitorText, DisplayString, Color.Red);
                EcgComReceiveFlag = 0x01;
                EcgComUpDataConnectReq(EcgCom, ConnectUartName[4]);
                Thread.Sleep(500);
                if(EcgComReceiveFlag == 0x01)
                {
                        DisplayString = "BLE主机串口： "+ConnectUartName[4] + "连接失败！\r\n";
                        OutMsg(MonitorText, DisplayString, Color.Red);

                        EcgComReceiveFlag = 0;
                        EcgComConnectFlag = 0;

                        tabControl1.Invoke(new EventHandler(delegate
                        {
                            tabControl1.TabPages[0].Text = "心电补丁主机测试工装未连接";
                            toolStripStatusLabel1.Text = "BLE主机串口未连接";
                        }));

                        if (EcgUartProcessThread.IsAlive)
                        {
                            EcgUartProcessThread.Abort();
                        }
                        EcgComReceiveFlag = 0;
                }
                if (EcgComReceiveFlag == 0x02)
                {
                    DisplayString = "BLE主机串口： " + ConnectUartName[4] + "连接成功！\r\n";
                    OutMsg(MonitorText, DisplayString, Color.Red);

                    EcgComConnectFlag = 1;

                    tabControl1.Invoke(new EventHandler(delegate
                    {
                        tabControl1.TabPages[0].Text = "心电补丁主机测试工装已连接";
                        toolStripStatusLabel1.Text = "BLE主机串口： " + ConnectUartName[4] + "连接成功";
                    }));
                    if (!EcgUartProcessThread.IsAlive)
                    {
                        ScanButton.Invoke(new EventHandler(delegate
                        {
                            ScanButton.Enabled = true;
                        }));
                        EcgUartProcessThread = new Thread(new ThreadStart(EcgUartProcessFunction));
                        EcgUartProcessThread.Start();
                    }
                }
            }
        }

        void EcgComUpDataConnectReq(SerialPort serialport, string comName)
        {
            if (ComInit(serialport, comName, 115200))
            {
                serialcommand.SendConnectSerialCommand(serialport);
            }
        }

        void UpDataComUpDataConnectReq(SerialPort serialport, string comName)
        {
            if (ComInit(serialport, comName, 115200))
            {
                serialcommand.UpGradeCommand(serialport);
            }
        }

        void ConnectStatusCheckBoxDisplay(int i)

        {
            UpDataCheckBox[i].Invoke(new EventHandler(delegate
            {
                UpDataCheckBox[i].Text = ConnectUartName[i] + "已连接";
                UpDataCheckBox[i].Checked = true;
            }));
        }

        void DisConnectStatusCheckBoxDisplay(int i)
        {
            UpDataCheckBox[i].Invoke(new EventHandler(delegate
            {
                UpDataCheckBox[i].Text = "未连接";
                UpDataCheckBox[i].Checked = false;
            }));
    }

        void ScanUpdataUartFunction()
        {
            Int16 ScanSequence = 0;
            
            while (true)
            {
                {
                    for (int i = 0; i < UPDATACOMNUM; i++)
                    {
                        if (UpDataComConnectFlag[i] == 0)   //判断是否连接
                        {
                            GetComList(GetComNameList);    //获取当前所有的串口
                            if (ScanSequence >= (GetComNameList.Count()-1))
                            {
                                GetComList(GetComNameList);    //获取当前所有的串口
                                ScanSequence = 0;
                            }
                            else
                            {
                                ScanSequence++;
                            }
                            ConnectUartName[i] = GetComNameList[ScanSequence];
                        }

                        System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                        string DisplayString = "升级串口：请求连接" + ConnectUartName[i] + "...\r\n";
                        DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                        OutMsg(MonitorText, DisplayString, Color.Red);
                        DownLoadFlag[i] = 13;
                        if (i == 0)
                        {
                            SerialReceiveData.Clear();
                            UpDataComUpDataConnectReq(UpDataCom[i], ConnectUartName[i]);
                        }
                        else if (i == 1)
                        {
                            SerialReceiveData2.Clear();
                            UpDataComUpDataConnectReq(UpDataCom[i], ConnectUartName[i]);
                        }
                        else if (i == 2)
                        {
                            SerialReceiveData3.Clear();
                            UpDataComUpDataConnectReq(UpDataCom[i], ConnectUartName[i]);
                        }
                        else if (i == 3)
                        {
                            SerialReceiveData4.Clear();
                            UpDataComUpDataConnectReq(UpDataCom[i], ConnectUartName[i]);
                        }
                        Thread.Sleep(500);
                        if (DownLoadFlag[i] == 14)
                        {
                            converter = new System.Text.ASCIIEncoding();
                            DisplayString = "升级串口：" + ConnectUartName[i] + "连接成功！\r\n";
                            DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                            OutMsg(MonitorText, DisplayString, Color.Red);

                            ConnectStatusCheckBoxDisplay(i);
                            UpDataComConnectFlag[i] = 1;   //连接成功标志！
                        }
                        else
                        {
                            converter = new System.Text.ASCIIEncoding();
                            DisplayString = "升级串口：" + ConnectUartName[i] + "连接失败！\r\n";
                            DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                            OutMsg(MonitorText, DisplayString, Color.Red);

                            ConnectUartName[i] = "";
                            UpDataComConnectFlag[i] = 0; //连接失败标志！

                            DisConnectStatusCheckBoxDisplay(i);

                            UpDataCom[i].Close();
                            
                        }
                    }
                }
                if (UpDataCheckBox[0].Checked == true || (UpDataCheckBox[1].Checked == true) || (UpDataCheckBox[2].Checked == true) || (UpDataCheckBox[3].Checked == true))
                {
                    UpDataButton.Invoke(new EventHandler(delegate
                    {
                        UpDataButton.Enabled = true;
                        tabControl1.TabPages[1].Text = "程序下载（可升级）";
                    }));
                }
                else
                {
                    UpDataButton.Invoke(new EventHandler(delegate
                    {
                        UpDataButton.Enabled = true;
                        tabControl1.TabPages[1].Text = "程序下载（无设备连接）";
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
                    msg = DateTime.Now.ToLongTimeString() + ": " + msg;
                    rtb.Text = rtb.Text.Insert(0, msg);
                    rtb.Select(0, msg.Length - 1);
                    rtb.SelectionColor = color;//设置文本颜色
                    //  rtb.AppendText(msg);//输出文本，换行
                    rtb.ScrollToCaret();//滚动条滚到到最新插入行
                }));
        }


        #region 获取串口号
        public void GetComList(List<string> ComNameList)
        {
            RegistryKey keyCom = Registry.LocalMachine.OpenSubKey("Hardware\\DeviceMap\\SerialComm");
            if (keyCom != null)
            {
                string[] sSubKeys = keyCom.GetValueNames();
                ComNameList.Clear();
                foreach (string sName in sSubKeys)
                {
                    string sValue = (string)keyCom.GetValue(sName);
                    ComNameList.Add(sValue);
                }
            }
        }
        #endregion

        void UpDataComReceiveAnalyze(int i, List<byte> ReceiveDataList)
        { 
            int length;
            int SerialReceiveLength;
            System.Threading.Thread.Sleep(1);
            if (!UpDataCom[i].IsOpen)   //检测串口是否关闭
            {
                return;
            }
            /********************************************串口接收**************************************************/
            length = UpDataCom[i].BytesToRead;
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
                UpDataCom[i].DiscardInBuffer();
                return;
            }

            byte[] buf = new byte[SerialReceiveLength];//声明一个临时数组存储当前来的串口数据
            if (!UpDataCom[i].IsOpen)   //检测串口是否关闭
            {
                return;
            }
            UpDataCom[i].Read(buf, 0, SerialReceiveLength);

            ReceiveDataList.AddRange(buf);

            UpdataReceiveDataProcess(i, ReceiveDataList);
        }

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
                    while (ReceiveDataList.Count>0 && (ReceiveDataList[0] != 0x5a))
                    {
                        ReceiveDataList.RemoveAt(0);
                    }
                }
            }
        }

        #region 串口中断ISR
        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            UpDataComReceiveAnalyze(0, SerialReceiveData);
        }
        #endregion
        private void serialPort2_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            UpDataComReceiveAnalyze(1, SerialReceiveData2);
        }
        private void serialPort3_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            UpDataComReceiveAnalyze(2, SerialReceiveData3);
        }
        private void serialPort4_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            UpDataComReceiveAnalyze(3, SerialReceiveData4);
        }

        private void serialPort5_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            int length;
            int SerialReceiveLength;
            System.Threading.Thread.Sleep(1);
            if (!EcgCom.IsOpen)   //检测串口是否关闭
            {
                return;
            }
            /********************************************串口接收**************************************************/
            length = EcgCom.BytesToRead;
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
                EcgCom.DiscardInBuffer();
                return;
            }

            byte[] buf = new byte[SerialReceiveLength];//声明一个临时数组存储当前来的串口数据 
            if (!EcgCom.IsOpen)   //检测串口是否关闭
            {
                return;
            }
            EcgCom.Read(buf, 0, SerialReceiveLength);
            SerialReceiveData5.AddRange(buf);
            EcgComReceiveProcess(SerialReceiveData5);
        }


        void EcgComReceiveProcess(List<byte> ReceiveDataList)
        {
            while (ReceiveDataList.Count >= 4 && ((ReceiveDataList[3] + 4) <= ReceiveDataList.Count))
            {
                if (ReceiveDataList[0] == 0x77)   //type (command)
                {
                    if (ReceiveDataList[2] == 0x00)  //状态正常
                    {
                        if (ReceiveDataList[1] == 0x02)  //
                        {
                            if (ScanDeviceFlag == 0 && (ConnectDeviceFlag == 0) && (PairDeviceFlag == 0))
                            {
                                EcgComReceiveFlag = 2;
                            }
                        }
                        else if (ReceiveDataList[1] == 0x0B) //心电补丁已断开
                        {
                            if (ScanDeviceFlag == 0 && (ConnectDeviceFlag == 0) && (PairDeviceFlag == 0) || (ConnectDeviceFlag == 3) || (ConnectDeviceFlag == 4))
                            {
                                EcgComReceiveFlag = 4;
                            }
                        }
                        else if (ReceiveDataList[1] == 0x17) //请求停止接收心电数据成功
                        {
                            if (ScanDeviceFlag == 0 && (ConnectDeviceFlag == 0) && (PairDeviceFlag == 0) || (ConnectDeviceFlag == 2) || (ConnectDeviceFlag == 3) || (ConnectDeviceFlag == 5))
                            {
                                EcgComReceiveFlag = 6;
                            }
                        }
                        else if (ReceiveDataList[1] == 0x12)
                        {
                            if (ScanDeviceFlag == 0 && (ConnectDeviceFlag == 0) && (PairDeviceFlag == 0))
                            {
                                if (ReceiveDataList[4] == 0x01)    //自动连接配置完成！
                                {

                                }
                                else if (ReceiveDataList[4] == 0x00)   //停止自动连接！
                                {
                                    EcgComReceiveFlag = 8;
                                }
                            }
                        }
                        else if (ReceiveDataList[1] == 0x1B)  //获取自动搜索连接状态成功
                        {
                            if (ScanDeviceFlag == 0 && (ConnectDeviceFlag == 0) && (PairDeviceFlag == 0))
                            {
                                EcgComReceiveFlag = 10;
                                checkBox1.Invoke(new EventHandler(delegate
                                {
                                    if (ReceiveDataList[4] == 1)
                                    {
                                        checkBox1.Checked = true;
                                    }
                                    else if (ReceiveDataList[4] == 0)
                                    {
                                        checkBox1.Checked = false;
                                    }
                                }));
                            }
                        }
                        else if (ReceiveDataList[1] == 0x0E)  //正在搜索设备
                        {
                            ScanBLENum = 0;
                            MAClistBox.Invoke(new EventHandler(delegate
                            {
                                //MACComboBox.Items.Clear();
                                MAClistBox.Items.Clear();
                            }));
                        }
                        else if (ReceiveDataList[1] == 0x04)//搜索到一个心电设备
                        {
                            for (int j = 0; j < 6; j++)
                            {
                                ScanBLEMAC[ScanBLENum, j] = ReceiveDataList[4 + j];
                            }
                            if ((ReceiveDataList.Count - 0x10) >= 11)
                            {
                                for (int j = 0; j < 11; j++)
                                {
                                    ScanBleName[ScanBLENum, j] = ReceiveDataList[0x10 + j];
                                }
                            }
                            ScanBLENum++;
                        }
                        else if (ReceiveDataList[1] == 0x2E)  //搜索结束
                        {
                            if (ScanDeviceFlag == 1)
                            {
                                EcgComReceiveFlag = 0x32;
                                // button6.Enabled = true;   //配对按钮
                                ScanButton.Invoke(new EventHandler(delegate
                                {
                                    ScanButton.Enabled = true;
                                }));
                            }
                        }
                        else if (ReceiveDataList[1] == 0x06)
                        {
                                if (ReceiveDataList[4] == 1) //心电补丁连接成功
                                {
                                    EcgComReceiveFlag = 0x42;
                                }
                                else    //选择的设备与绑定的mac不符，无法连接
                                {

                                    EcgComReceiveFlag = 0x43;
                                } 
                        }
                        else if (ReceiveDataList[1] == 0x23) //获取配对状态成功
                        {
                            if (ScanDeviceFlag == 0 && (ConnectDeviceFlag == 0) && (PairDeviceFlag == 0) || (PairDeviceFlag == 1))
                            {
                                EcgComReceiveFlag = 0x11;
                                for (int i = 0; i < 6; i++)
                                {
                                    ECGPairMAC[i] = ReceiveDataList[4 + i];
                                }
                            }
                        }
                        else if (ReceiveDataList[1] == 0x25) //设置测试模式成功
                        {
                            if (ScanDeviceFlag == 0 && (ConnectDeviceFlag == 0) && (PairDeviceFlag == 0))
                            {
                                EcgComReceiveFlag = 0x13;
                            }
                        }
                        else if (ReceiveDataList[1] == 0x15)
                        {
                            if (ReceiveDataList[4] == 0)   //设备未连接,无法设置成功
                            {
                                EcgComReceiveFlag = 0x53;
                            }
                            else if (ReceiveDataList[4] == 1) //设置接收心电数据成功
                            {
                                EcgComReceiveFlag = 0x52;
                            }


                        }
                        else if (ReceiveDataList[1] == 0x21)   //设置配对MAC成功
                        {
                            if (PairDeviceFlag == 1)
                            {
                                EcgComReceiveFlag = 0x62;
                                for (int i = 0; i < 6; i++)
                                {
                                    ECGPairMAC[i] = ReceiveDataList[4 + i];
                                }
                            }
                        }
                        else if (ReceiveDataList[1] == 0x31)  //获取硬件版本成功
                        {
                            EcgComReceiveFlag = 0x72;
                            for (int i = 0; i < 5; i++)
                            {
                                HardVersion[i] = ReceiveDataList[4 + i];
                            }
                        }
                        else if (ReceiveDataList[1] == 0x33)  //获取软件版本成功
                        {
                            EcgComReceiveFlag = 0x74;
                            for (int i = 0; i < 12; i++)
                            {
                                SoftVersion[i] = ReceiveDataList[4 + i];
                            }
                        }
                        else if (ReceiveDataList[1] == 0x1D)   //获取主设备蓝牙MAC成功
                        {
                            for (int i = 0; i < 6; i++)
                            {
                                BLECentralMAC[i] = ReceiveDataList[4 + i];
                            }
                        }
                        else if (ReceiveDataList[1] == 0x1F)
                        {
                            //蓝牙心电补丁未连接，无法获取MAC
                            if (ReceiveDataList[4] == 0 && (ReceiveDataList[5] == 0) && (ReceiveDataList[6] == 0) && (ReceiveDataList[7] == 0))
                            {
                                    ;
                            }
                            else//蓝牙心电补丁MAC获取成功
                            {
                                    ;
                            }

                        }
                        else if (ReceiveDataList[1] == 0x19)   //心电补丁ID获取成功
                        {
                            for (int i = 0; i < ReceiveDataList[3]; i++)
                            {
                                ECGPatchID[i] = ReceiveDataList[4 + i];
                            }
                        }
                        else if (ReceiveDataList[1] == 0x2B)  //获取0mv校准值成功
                        {
                            difference_Value = (Int16)(ReceiveDataList[4] + (ReceiveDataList[5] << 8));
                            if (difference_Value <= 0 || (difference_Value > 1000))
                            {
                                difference_Value = 500;
                                
                            }
                            EcgComReceiveFlag = 0x82;
                        }
                        else if (ReceiveDataList[1] == 0x27)//获取1mv定标值成功
                        {
                            amplification = (Int16)(ReceiveDataList[4] + (ReceiveDataList[5] << 8));
                            if (amplification <= 0 || (amplification > 200))
                            {
                                amplification = 60;
                                
                            }
                            EcgComReceiveFlag = 0x84;
                        }
                        else if (ReceiveDataList[1] == 0x2D) //设置0mv校准值成功
                        {
                            if (ReceiveDataList[4] == 1)
                            {
                                EcgComReceiveFlag = 0x7A;
                            }
                        }
                        else if (ReceiveDataList[1] == 0x29)//设置1mv定标值成功
                        {
                            if (ReceiveDataList[4] == 1)
                            {
                                EcgComReceiveFlag = 0x7C;
                            }
                        }
                        else if (ReceiveDataList[1] == 0x35)  //设置心电补丁ID成功
                        {
                            for (int i = 0; i < 10; i++)
                            {
                                 ECGPatchID[i] = ReceiveDataList[4 + i];
                            }
                        }
                        else if (ReceiveDataList[1] == 0x52)  //开始1mv校准成功
                        {
                            if (ReceiveDataList[4] == 1)
                            {
                                EcgComReceiveFlag = 0x76;
                            }
                        }
                        else if (ReceiveDataList[1] == 0x53)  //开始0mv校准成功
                        {
                            if (ReceiveDataList[4] == 1)
                            {
                                EcgComReceiveFlag = 0x78;
                            }
                        }
                        else if (ReceiveDataList[1] == 0x09)
                        {
                            if (ConnectDeviceFlag == 4)
                            {
                                EcgComReceiveFlag = 0x99;
                                EcgDataTimer = ReceiveDataList[4]%64;

                                LeadOffStatus = (ReceiveDataList[5]>>7);
                                Vbat = ReceiveDataList[5] & 0x7F;

                                EcgData_Com[ReceiveDataList[4] % 64, 0] = (Convert.ToDouble(ReceiveDataList[6]) + Convert.ToDouble((ReceiveDataList[10] & 0xc0) << 2)) * DisplayMultipleNum - ZeroValue;
                                EcgData_Com[ReceiveDataList[4] % 64, 1] = (Convert.ToDouble(ReceiveDataList[7]) + Convert.ToDouble((ReceiveDataList[10] & 0x30) << 4)) * DisplayMultipleNum - ZeroValue;
                                EcgData_Com[ReceiveDataList[4] % 64, 2] = (Convert.ToDouble(ReceiveDataList[8]) + Convert.ToDouble((ReceiveDataList[10] & 0x0c) << 6)) * DisplayMultipleNum - ZeroValue;
                                EcgData_Com[ReceiveDataList[4] % 64, 3] = (Convert.ToDouble(ReceiveDataList[9]) + Convert.ToDouble((ReceiveDataList[10] & 0x03) << 8)) * DisplayMultipleNum - ZeroValue;
                                EcgData_Com[ReceiveDataList[4] % 64, 4] = (Convert.ToDouble(ReceiveDataList[11]) + Convert.ToDouble((ReceiveDataList[15] & 0xc0) << 2)) * DisplayMultipleNum - ZeroValue;
                                EcgData_Com[ReceiveDataList[4] % 64, 5] = (Convert.ToDouble(ReceiveDataList[12]) + Convert.ToDouble((ReceiveDataList[15] & 0x30) << 4)) * DisplayMultipleNum - ZeroValue;
                                EcgData_Com[ReceiveDataList[4] % 64, 6] = (Convert.ToDouble(ReceiveDataList[13]) + Convert.ToDouble((ReceiveDataList[15] & 0x0c) << 6)) * DisplayMultipleNum - ZeroValue;
                                EcgData_Com[ReceiveDataList[4] % 64, 7] = (Convert.ToDouble(ReceiveDataList[14]) + Convert.ToDouble((ReceiveDataList[15] & 0x03) << 8)) * DisplayMultipleNum - ZeroValue;

                                if (EcgData_Pulse.Count > (512 - 8))
                                {
                                    EcgData_Pulse.RemoveRange(0, 8);
                                }
                                EcgData_Pulse.Add(EcgData_Com[ReceiveDataList[4] % 64, 0]);
                                EcgData_Pulse.Add(EcgData_Com[ReceiveDataList[4] % 64, 1]);
                                EcgData_Pulse.Add(EcgData_Com[ReceiveDataList[4] % 64, 2]);
                                EcgData_Pulse.Add(EcgData_Com[ReceiveDataList[4] % 64, 3]);
                                EcgData_Pulse.Add(EcgData_Com[ReceiveDataList[4] % 64, 4]);
                                EcgData_Pulse.Add(EcgData_Com[ReceiveDataList[4] % 64, 5]);
                                EcgData_Pulse.Add(EcgData_Com[ReceiveDataList[4] % 64, 6]);
                                EcgData_Pulse.Add(EcgData_Com[ReceiveDataList[4] % 64, 7]);

                                if ((ScalingFlag == 1) || (ScalingFlag == 11))   //校准或定标
                                {
                                    calibration_Value.Add(EcgData_Com[ReceiveDataList[4] % 64, 0]);
                                    calibration_Value.Add(EcgData_Com[ReceiveDataList[4] % 64, 1]);
                                    calibration_Value.Add(EcgData_Com[ReceiveDataList[4] % 64, 2]);
                                    calibration_Value.Add(EcgData_Com[ReceiveDataList[4] % 64, 3]);
                                    calibration_Value.Add(EcgData_Com[ReceiveDataList[4] % 64, 4]);
                                    calibration_Value.Add(EcgData_Com[ReceiveDataList[4] % 64, 5]);
                                    calibration_Value.Add(EcgData_Com[ReceiveDataList[4] % 64, 6]);
                                    calibration_Value.Add(EcgData_Com[ReceiveDataList[4] % 64, 7]);
                                }

                            }
                        }
                        ReceiveDataList.RemoveRange(0, ReceiveDataList[3] + 4);
                    }
                }
                if (ReceiveDataList.Count > 0 && (ReceiveDataList[0] != 0x77))
                {
                    ReceiveDataList.RemoveAt(0);
                }
            }
            while (ReceiveDataList.Count>0 && (ReceiveDataList[0] != 0x77))
            {
                ReceiveDataList.RemoveAt(0);
            }
        }

        int ScalingFuction()
        {
            if (calibration_Value.Count >= 1024)
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
                        for (int i = 200; i < 1024; i++)  //前面不要了
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

                    calibration_Value.Clear();

                    ScalingFlag = 2;   //完成1mv定标
                    ZeroValue = 5000;
                    DisplayMultipleNum = 10;

                    ConnectDeviceFlag = 5;

                    Thread.Sleep(50);
                    string DisplayString1 = "请求停止接收心电数据...\r\n";
                    string DisplayString2 = "请求停止接收心电数据失败！\r\n";
                    string DisplayString3 = "请求停止接收心电数据成功！\r\n";
                    serialcommand.StopReceiveECGDataSerialCommand(EcgCom);
                    TestCommandGetStatus(5, 6, DisplayString1, DisplayString2, DisplayString3, 500);

                    DisplayString1 = "请求保存1mv校准值...\r\n";
                    DisplayString2 = "保存1mv校准值失败！\r\n";
                    DisplayString3 = "保存1mv校准值成功！\r\n";
                    serialcommand.Setcalibration1mvSerialCommand(EcgCom, amplification);
                    if (TestCommandGetStatus(0x7b, 0x7c, DisplayString1, DisplayString2, DisplayString3, 500) == 0)
                    {
                        button1.Invoke(new EventHandler(delegate
                        {
                            button5.Text = "1mv校准成功";
                            button5.Enabled = false;
                        }));
                        DisplayString1 = "请求获取1mv校准值...\r\n";
                        DisplayString2 = "获取1mv校准值失败！\r\n";
                        DisplayString3 = "获取1mv校准值成功！\r\n";
                        serialcommand.calibration1mvSerialCommand(EcgCom);
                        if (TestCommandGetStatus(0x83, 0x84, DisplayString1, DisplayString2, DisplayString3, 500) == 0)
                        {
                            ;
                        }
                        else
                        {
                            ;
                        }
                        DisplayString1 = "请求获取0mv校准值...\r\n";
                        DisplayString2 = "获取0mv校准值失败！\r\n";
                        DisplayString3 = "获取0mv校准值成功！\r\n";
                        serialcommand.calibration0mvSerialCommand(EcgCom);
                        if (TestCommandGetStatus(0x81, 0x82, DisplayString1, DisplayString2, DisplayString3, 500) == 0)
                        {
                            ;
                        }
                        else
                        {
                            ;
                        }
                        DisplayString1 = "请求接收心电数据...\r\n";
                        DisplayString2 = "接收心电数据失败！\r\n";
                        DisplayString3 = "接收心电数据成功！\r\n";
                        serialcommand.ReceiveECGDataSerialCommand(EcgCom);
                        TestCommandGetStatus(0x51, 0x52, DisplayString1, DisplayString2, DisplayString3, 500);

                        toolStripStatusLabel2.Text = "当前心电补丁 1mv校准值: " + amplification.ToString() + "    0mv校准值： " + difference_Value.ToString();
                    }
                    else
                    {
                        button1.Invoke(new EventHandler(delegate
                        {
                            button5.Text = "1mv校准失败";
                            button5.Enabled = false;
                        }));
                    }
                    ConnectDeviceFlag = 4;
                    Thread.Sleep(1000);
                    button1.Invoke(new EventHandler(delegate
                    {
                        button5.ForeColor = Color.Black;
                        button5.Text = "1mv定标";
                        button5.Enabled = true;
                        button1.Enabled = true;
                    }));
                    return 1;
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
                    calibration_Value.Clear();
                    ZeroValue = 5000;
                    DisplayMultipleNum = 10;
                    ScalingFlag = 12;   //完成0mv校准

                    ConnectDeviceFlag = 5;

                    Thread.Sleep(50);
                    string DisplayString1 = "请求停止接收心电数据...\r\n";
                    string DisplayString2 = "请求停止接收心电数据失败！\r\n";
                    string DisplayString3 = "请求停止接收心电数据成功！\r\n";
                    serialcommand.StopReceiveECGDataSerialCommand(EcgCom);
                    TestCommandGetStatus(5, 6, DisplayString1, DisplayString2, DisplayString3, 500);

                    DisplayString1 = "请求保存0mv校准值...\r\n";
                    DisplayString2 = "保存0mv校准值失败！\r\n";
                    DisplayString3 = "保存0mv校准值成功！\r\n";
                    serialcommand.Setcalibration0mvSerialCommand(EcgCom, difference_Value);
                    if (TestCommandGetStatus(0x79, 0x7a, DisplayString1, DisplayString2, DisplayString3, 500) == 0)
                    {
                        button1.Invoke(new EventHandler(delegate
                        {
                            button1.Text = "0mv校准成功";
                            button1.Enabled = false;
                        }));
                        DisplayString1 = "请求获取1mv校准值...\r\n";
                        DisplayString2 = "获取1mv校准值失败！\r\n";
                        DisplayString3 = "获取1mv校准值成功！\r\n";
                        serialcommand.calibration1mvSerialCommand(EcgCom);
                        if (TestCommandGetStatus(0x83, 0x84, DisplayString1, DisplayString2, DisplayString3, 500) == 0)
                        {
                            ;
                        }
                        else
                        {
                            ;
                        }
                        DisplayString1 = "请求获取0mv校准值...\r\n";
                        DisplayString2 = "获取0mv校准值失败！\r\n";
                        DisplayString3 = "获取0mv校准值成功！\r\n";
                        serialcommand.calibration0mvSerialCommand(EcgCom);
                        if (TestCommandGetStatus(0x81, 0x82, DisplayString1, DisplayString2, DisplayString3, 500) == 0)
                        {
                            ;
                        }
                        else
                        {
                            ;
                        }

                        DisplayString1 = "请求接收心电数据...\r\n";
                        DisplayString2 = "接收心电数据失败！\r\n";
                        DisplayString3 = "接收心电数据成功！\r\n";
                        serialcommand.ReceiveECGDataSerialCommand(EcgCom);
                        TestCommandGetStatus(0x51, 0x52, DisplayString1, DisplayString2, DisplayString3, 500);

                        toolStripStatusLabel2.Text = "当前心电补丁 1mv校准值: " + amplification.ToString() + "    0mv校准值： " + difference_Value.ToString();
                    }
                    else
                    {
                        button1.Invoke(new EventHandler(delegate
                        {
                            button1.Text = "0mv校准失败";
                            button1.Enabled = false;
                        }));
                    }
                    ConnectDeviceFlag = 4;
                    
                    Thread.Sleep(1000);
                    button1.Invoke(new EventHandler(delegate
                    {
                        button1.ForeColor = Color.Black;
                        button1.Text = "0mv校准";
                        button1.Enabled = true;
                        button5.Enabled = true;
                    }));
                    return 2;
                }
            }
            return -1;
        }

        #region 串口初始化
        bool ComInit(SerialPort serialport, string comName, int comBaudRate)
        {
            if (serialport.IsOpen)
            {
                serialport.Close();
            }
            try
            {
                serialport.PortName = comName; //port name COM1
                serialport.BaudRate = comBaudRate;  //BaudRate 115200bps
                serialport.Parity = Parity.None; //Parity none
                serialport.StopBits = StopBits.One; //StopBits 1
                serialport.DataBits = comDataBits;        // DataBits 8bit
                serialport.ReadTimeout = 1000;  // ReadTimeout 2 second
                serialport.ReceivedBytesThreshold = 2;
                serialport.Open();//打开串口   

                if (serialport == UpDataCom[0])
                {
                    serialport.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(serialPort1_DataReceived);
                }
                else if (serialport == UpDataCom[1])
                {
                    serialport.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(serialPort2_DataReceived);
                }
                else if (serialport == UpDataCom[2])
                {
                    serialport.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(serialPort3_DataReceived);
                }
                else if (serialport == UpDataCom[3])
                {
                    serialport.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(serialPort4_DataReceived);
                }
                else if (serialport == EcgCom)
                {
                    serialport.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(serialPort5_DataReceived);
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

        #region 波形的初始化
        private void Motion_Load(object sender, EventArgs e)
        {

            LeadLabel.Text = "";
            BatteryValueLabel.Text = "";
            PulseLabel.Text = "";

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

            //textBox1.Text = (chartlenMin * 1000 / 128).ToString();
            //textBox2.Text = (chartlenMax * 1000 / 128).ToString();

            //numericUpDown1.Value = Ecgamplification;

            ScanUpDataComFlag = 1;


            for (int i = 0; i < 4; i++)
            {
                UpDataProgressBar[i] = new ProgressBar();
                UpDataProgressBar[i].Location = new System.Drawing.Point(300, 170 + 20 * i);
                UpDataProgressBar[i].Size = new System.Drawing.Size(130, 20);
                tabControl1.TabPages[1].Controls.Add(UpDataProgressBar[i]);

                UpDataCheckBox[i] = new CheckBox();
                UpDataCheckBox[i].Location = new System.Drawing.Point(216, 170 + 20 * i);
                UpDataCheckBox[i].Text = "未连接";
                tabControl1.TabPages[1].Controls.Add(UpDataCheckBox[i]);

                UpDataLabel[i] = new Label();
                UpDataLabel[i].Location = new System.Drawing.Point(477, 175 + 20 * i);
                UpDataLabel[i].Text = "等待连接";
                UpDataLabel[i].Size = new System.Drawing.Size(150, 12);
                tabControl1.TabPages[1].Controls.Add(UpDataLabel[i]);


                UpDataCom[i] = new SerialPort();



            }

            ScanEcgUartThread = new Thread(new ThreadStart(ScanEcgUartFunction));
            ScanEcgUartThread.Start();

            ScanUpdataThread = new Thread(new ThreadStart(ScanUpdataUartFunction));
            ScanUpdataThread.Start();

            for (int i = 0; i < UPDATACOMNUM; i++)
            {
                UpDataThread[i] = new Thread(new ParameterizedThreadStart(Upgrade2));
            }

            EcgUartProcessThread = new Thread(new ThreadStart(EcgUartProcessFunction));
        }
        #endregion

        private void trackBar1_Scroll(object sender, EventArgs e)
        {
            //if (radioButton2.Checked)
            //{
            //    trackBar1.Minimum = chartlenMin + 128;
            //    chartlenMax = trackBar1.Value;
            //}
            //else if (radioButton1.Checked)
            //{
            //    trackBar1.Maximum = chartlenMax - 128;
            //    chartlenMin = trackBar1.Value;
            //}
            //textBox1.Text = (chartlenMin * 1000 / 128).ToString();
            //textBox2.Text = (chartlenMax * 1000 / 128).ToString();
            //chart1.ChartAreas["ChartArea_Ecg"].AxisX.Maximum = chartlenMax;
            //chart1.ChartAreas["ChartArea_Ecg"].AxisX.Minimum = chartlenMin;
            //chart1.ChartAreas["ChartArea_Ecg"].AxisX.Interval = (chartlenMax - chartlenMin) / 4;
        }


        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {
            //if (radioButton1.Checked)
            //{
            //    trackBar1.Minimum = 0;
            //    trackBar1.Value = chartlenMin;
            //}
            //trackBar1.Focus();
        }

        private void radioButton2_CheckedChanged(object sender, EventArgs e)
        {
            //if (radioButton2.Checked)
            //{
            //    trackBar1.Maximum = 512;
            //    trackBar1.Value = chartlenMax;
            //}
            //trackBar1.Focus();
        }

        #region 定时器处理函数
        unsafe private void timer1_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
        {
            int EcgMaxValue, EcgMinValue;


            while (SerialReceiveData.Count >= 4 && ((SerialReceiveData[3] + 4) <= SerialReceiveData.Count))
            {
               
                if (DebugMode == 2)
                {
                    if (SerialReceiveData[0] == 0x77)   //type (command)
                    {
                            if (SerialReceiveData[1] == 0x12)
                            {
                                if (SerialReceiveData[1] == 0x36)
                            {
                                string DisplayString = "正在接收主设备接收心电包数...\r\n";
                                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                                OutMsg(MonitorText, DisplayString, Color.Red);

                                double CentralReceivePackage = SerialReceiveData[4] + (SerialReceiveData[5] << 8) + (SerialReceiveData[6] << 16) + (SerialReceiveData[7] << 24);

                                //this.toolStripStatusLabel8.Text = "主设备接收包：" + CentralReceivePackage.ToString();

                                //double CentralLostPackage = SerialReceiveData[8] + (SerialReceiveData[9] << 8) + (SerialReceiveData[10] << 16) + (SerialReceiveData[11] << 24);

                                //this.toolStripStatusLabel7.Text = "主设备丢包：" + CentralLostPackage.ToString();
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
                                            //AmplificationValue.Text = "正在1mv定标...";
                                            //differenceValue.Text = "正在1mv定标...";
                                        }
                                        else if (ScalingFlag == 11)
                                        {
                                            System.Text.ASCIIEncoding converter2 = new System.Text.ASCIIEncoding();
                                            string DisplayString3 = "正在0mv校准...\r\n";
                                            DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString3;
                                            OutMsg(MonitorText, DisplayString, Color.Red);
                                            //AmplificationValue.Text = "正在0mv校准...";
                                            //differenceValue.Text = "正在0mv校准...";
                                        }
                                        else
                                        {
                                            //AmplificationValue.Text = "获取的1mv定标值为：" + Convert.ToString(amplification);
                                            //differenceValue.Text = "获取的0mv校准值为：" + Convert.ToString(difference_Value);
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
                                    //this.toolStripStatusLabel5.Text = "上位机丢包数：" + LostPackageNum.ToString();

                                    //ReceivePackageNum++;
                                    //this.toolStripStatusLabel6.Text = "上位机接收包：" + ReceivePackageNum.ToString();

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
                    PairButton.Enabled = false;   //定标按键
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
                        //this.toolStripStatusLabel2.Text = "主蓝牙设备MAC：" + BLECentralMAC[5].ToString("X2") + ":" + BLECentralMAC[4].ToString("X2")
                        //            + ":" + BLECentralMAC[3].ToString("X2") + ":" + BLECentralMAC[2].ToString("X2") + ":" + BLECentralMAC[1].ToString("X2") + ":" + BLECentralMAC[0].ToString("X2")
                        //            + "  连接状态：心电补丁已连接 MAC:" +
                        //    ECGPatchMAC[5].ToString("X2") + ":" + ECGPatchMAC[4].ToString("X2") +
                        //    ":" + ECGPatchMAC[3].ToString("X2") + ":" + ECGPatchMAC[2].ToString("X2")
                        //    + ":" + ECGPatchMAC[1].ToString("X2") + ":" + ECGPatchMAC[0].ToString("X2")
                        //    + "  " + "RSSI:" + Convert.ToInt16(RSSIValue) + "  ID: " + str + "  " + str2 + str3;

                        //this.EcgPatchVersionLabel.Text = "心电补丁已连接 硬件版本:" + Encoding.UTF8.GetString(HardVersion) + "   软件版本:" + Encoding.UTF8.GetString(SoftVersion);
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
            //if (ConnectBLEButton.Text == "设备已连接")
            {

                //  BloodPressure.Text = "";
                if ((LeadOffStatus & 0x01) == 0x01)
                {

                }
                else
                {
                    try
                    {

                    }
                    finally
                    {
                        ;
                    }
                }
            }
            //else
            {

            }

            Timer.Enabled = false;

        }

        #region 选择串口设备号
        private void comboBoxCom_MouseClick(object sender, MouseEventArgs e)
        {
            //GetComList();
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
            if (ScanButton.Text == "搜索设备" || (ScanButton.Text == "搜索失败，请重新搜索"))
            {
                if (EcgUartProcessThread.IsAlive)
                {
                    EcgUartProcessThread.Abort();
                }

                ScanDeviceFlag = 1;   //发送搜索命令标志

                ScanButton.Text = "正在搜索设备...";
                ScanButton.Enabled = false;

                EcgUartProcessThread = new Thread(new ThreadStart(EcgUartProcessFunction));
                EcgUartProcessThread.Start();
            }
            else if (ScanButton.Text == "设备已连接")
            {
                if (EcgUartProcessThread.IsAlive)
                {
                    EcgUartProcessThread.Abort();
                }

                ConnectDeviceFlag = 3;

                ScanButton.Text = "正在断开设备...";

                EcgUartProcessThread = new Thread(new ThreadStart(EcgUartProcessFunction));
                EcgUartProcessThread.Start();
            }
            //if (EcgUartProcessThread.IsAlive)
            //{
            //    EcgUartProcessThread.Abort();
            //}


            //    MACComboBox.Items.Clear();

            //    ScanButton.Enabled = false;
            //    ConnectBLEButton.Enabled = false;
            //    MACComboBox.Enabled = false;
            //    // BLENUMlabel.Text = "";
            //    ScanBLENum = 0;
            //    button6.Enabled = false;
            //}
            //PauseButton.Enabled = false;

            //EcgUartProcessThread = new Thread(new ThreadStart(EcgUartProcessFunction));
            //EcgUartProcessThread.Start();
        }
        #endregion

        #region 选择连接设备的MAC
        private void MACComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            //ChoiceBLE = (byte)MACComboBox.SelectedIndex;
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
            //if (ConnectBLEButton.Text == "设备已连接")
            {
                checkBox1.Enabled = true;
                serialcommand.DisconnectBLESerialCommand(serialPort1);
                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                string DisplayString = "请求断开设备...\r\n";
                DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                OutMsg(MonitorText, DisplayString, Color.Red);
                DisConnectBLEEnableFlag = 1;
                //this.toolStripStatusLabel2.Text = "蓝牙设备状态：未连接";
            }
            //else
            {
                checkBox1.Enabled = false;

                //if (MACComboBox.SelectedIndex < 100)
                //{

                //    if (ScanBleName[MACComboBox.SelectedIndex, 0] == 'B' && (ScanBleName[MACComboBox.SelectedIndex, 1] == 'D'))
                //    {
                //        serialcommand.ConnectBLESerialCommand(serialPort1, (byte)MACComboBox.SelectedIndex);
                //        System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                //        string DisplayString = "请求连接设备...\r\n";
                //        DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                //        OutMsg(MonitorText, DisplayString, Color.Red);
                //        DisConnectBLEEnableFlag = 0;
                //    }
                //    else
                //    {
                //        System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                //        string DisplayString = "所选设备不是心电补丁，请重新选择连接！！！\r\n";
                //        DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                //        OutMsg(MonitorText, DisplayString, Color.Red);
                //        checkBox1.Enabled = true;
                //    }
                //}
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
            //if (ConnectBLEButton.Text == "设备已连接")
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
            //else
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
            
            string DisplayString1 = "请求停止接收心电数据...\r\n";
            string DisplayString2 = "请求停止接收心电数据失败！\r\n";
            string DisplayString3 = "请求停止接收心电数据成功！\r\n";
            ConnectDeviceFlag = 5;
            Thread.Sleep(50);
                
            button5.Text = "请求1mv定标";
            button5.ForeColor = Color.Red;
            button5.Refresh();
            
           // button5.Enabled = false;
            button1.Enabled = false;

            serialcommand.StopReceiveECGDataSerialCommand(EcgCom);
            if (TestCommandGetStatus(5, 6, DisplayString1, DisplayString2, DisplayString3, 500) == 0)
            {
                DisplayString1 = "请求获取配对MAC...\r\n";
                DisplayString2 = "获取配对MAC失败！\r\n";
                DisplayString3 = "获取配对MAC成功！\r\n";
                serialcommand.StartSetcalibration1mvSerialCommand(EcgCom);
                if (TestCommandGetStatus(0x75, 0x76, DisplayString1, DisplayString2, DisplayString3, 1000) == 0)
                {
                    if (EcgUartProcessThread.IsAlive)
                    {
                        EcgUartProcessThread.Abort();
                    }
                    ScalingFlag = 1;    //开始1mv校准

                    ZeroValue = 0;
                    DisplayMultipleNum = 1;

                    DisplayString1 = "请求接收心电数据...\r\n";
                    DisplayString2 = "接收心电数据失败！\r\n";
                    DisplayString3 = "接收心电数据成功！\r\n";
                    serialcommand.ReceiveECGDataSerialCommand(EcgCom);
                    if (TestCommandGetStatus(0x51, 0x52, DisplayString1, DisplayString2, DisplayString3, 500) == 0)
                    {
                        ConnectDeviceFlag = 4;
                        button5.Text = "正在1mv定标...";
                       // button5.Enabled = false;
                    }
                    else
                    {
                        DisplayString1 = "请求接收心电数据...\r\n";
                        DisplayString2 = "接收心电数据失败！\r\n";
                        DisplayString3 = "接收心电数据成功！\r\n";
                        serialcommand.ReceiveECGDataSerialCommand(EcgCom);
                        TestCommandGetStatus(0x51, 0x52, DisplayString1, DisplayString2, DisplayString3, 500);
                        {
                            button5.Text = "1mv校准失败";
                            button5.Enabled = false;
                            Thread.Sleep(1000);
                            button5.ForeColor = Color.Black;
                            button5.Text = "1mv校准";
                            button5.Enabled = true;
                            button1.Enabled = true;
                        }
                    }

                    EcgUartProcessThread = new Thread(new ThreadStart(EcgUartProcessFunction));
                    EcgUartProcessThread.Start();
                }
                else
                {
                    DisplayString1 = "请求接收心电数据...\r\n";
                    DisplayString2 = "接收心电数据失败！\r\n";
                    DisplayString3 = "接收心电数据成功！\r\n";
                    serialcommand.ReceiveECGDataSerialCommand(EcgCom);
                    TestCommandGetStatus(0x51, 0x52, DisplayString1, DisplayString2, DisplayString3, 500);
                    {
                        button5.Text = "1mv定标失败";
                        button5.Enabled = false;
                        Thread.Sleep(1000);
                        button5.ForeColor = Color.Black;
                        button5.Text = "1mv定标";
                        button5.Enabled = true;
                        button1.Enabled = true;
                        ConnectDeviceFlag = 4;
                    }
                }
            }
            else
            {
                button5.Text = "1mv定标失败";
                button5.Enabled = false;
                Thread.Sleep(1000);
                button5.ForeColor = Color.Black;
                button5.Text = "0mv定标";
                button5.Enabled = true;
                button1.Enabled = true;
                ConnectDeviceFlag = 4;
            }
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
        void GetSetPairMAC()
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
            for (int i = 0; i < 6; i++)
            {
                ECGSetPairMAC[i] = (byte)((cc[(5 - i) * 3] << 4) + (cc[(5 - i) * 3 + 1]));
            }
        }
        private void button6_Click(object sender, EventArgs e)
        {
            if (EcgUartProcessThread.IsAlive)
            {
                EcgUartProcessThread.Abort();
            }
            GetSetPairMAC();
            ConnectDeviceFlag = 0;
            PairDeviceFlag = 1;   //

            PairButton.Text = "正在设置配对MAC";

            EcgUartProcessThread = new Thread(new ThreadStart(EcgUartProcessFunction));
            EcgUartProcessThread.Start();
        }
        private void button1_Click(object sender, EventArgs e)
        {
            string DisplayString1 = "请求停止接收心电数据...\r\n";
            string DisplayString2 = "请求停止接收心电数据失败！\r\n";
            string DisplayString3 = "请求停止接收心电数据成功！\r\n";
            ConnectDeviceFlag = 5;
            Thread.Sleep(50);

            button1.Text = "请求0mv校准";
            button1.ForeColor = Color.Red;
            button1.Refresh();

            serialcommand.StopReceiveECGDataSerialCommand(EcgCom);
            if (TestCommandGetStatus(5, 6, DisplayString1, DisplayString2, DisplayString3, 500) == 0)
            {
                DisplayString1 = "请求获取配对MAC...\r\n";
                DisplayString2 = "获取配对MAC失败！\r\n";
                DisplayString3 = "获取配对MAC成功！\r\n";
                serialcommand.StartSetcalibration0mvSerialCommand(EcgCom);
                if (TestCommandGetStatus(0x77, 0x78, DisplayString1, DisplayString2, DisplayString3, 1000) == 0)
                {
                    if (EcgUartProcessThread.IsAlive)
                    {
                        EcgUartProcessThread.Abort();
                    }
                    ScalingFlag = 11;    //开始0mv校准

                    ZeroValue = 0;
                    DisplayMultipleNum = 1;

                    DisplayString1 = "请求接收心电数据...\r\n";
                    DisplayString2 = "接收心电数据失败！\r\n";
                    DisplayString3 = "接收心电数据成功！\r\n";
                    serialcommand.ReceiveECGDataSerialCommand(EcgCom);
                    if (TestCommandGetStatus(0x51, 0x52, DisplayString1, DisplayString2, DisplayString3, 500) == 0)
                    {
                        ConnectDeviceFlag = 4;
                        button1.Text = "正在0mv校准...";
                       // button1.Enabled = false;
                    }
                    else
                    {
                        DisplayString1 = "请求接收心电数据...\r\n";
                        DisplayString2 = "接收心电数据失败！\r\n";
                        DisplayString3 = "接收心电数据成功！\r\n";
                        serialcommand.ReceiveECGDataSerialCommand(EcgCom);
                        TestCommandGetStatus(0x51, 0x52, DisplayString1, DisplayString2, DisplayString3, 500);
                        {
                            button1.Text = "0mv校准失败";
                            button1.Enabled = false;
                            Thread.Sleep(100);
                            button1.Text = "0mv校准";
                            button1.ForeColor = Color.Black;
                            button1.Enabled = true;
                            button5.Enabled = true;
                        }
                    }

                    EcgUartProcessThread = new Thread(new ThreadStart(EcgUartProcessFunction));
                    EcgUartProcessThread.Start();
                }
                else
                {
                    DisplayString1 = "请求接收心电数据...\r\n";
                    DisplayString2 = "接收心电数据失败！\r\n";
                    DisplayString3 = "接收心电数据成功！\r\n";
                    serialcommand.ReceiveECGDataSerialCommand(EcgCom);
                    TestCommandGetStatus(0x51, 0x52, DisplayString1, DisplayString2, DisplayString3, 500);
                    {
                        button1.Text = "0mv校准失败";
                        button1.Enabled = false;
                        Thread.Sleep(100);
                        button1.Text = "0mv校准";
                        button1.ForeColor = Color.Black;
                        button1.Enabled = true;
                        button5.Enabled = true;
                        ConnectDeviceFlag = 4;
                    }
                }
            }
            else
            {
                button1.Text = "0mv校准失败";
                button1.Enabled = false;
                Thread.Sleep(100);
                button1.Text = "0mv校准";
                button1.Enabled = true;
                button1.ForeColor = Color.Black;
                button5.Enabled = true;
                ConnectDeviceFlag = 4;
            }
            
        }


        private void numericUpDown1_ValueChanged(object sender, EventArgs e)
        {
            //chart1.ChartAreas["ChartArea_Ecg"].AxisY.Maximum = (int)(6000 / (double)(numericUpDown1.Value));
            //chart1.ChartAreas["ChartArea_Ecg"].AxisY.Minimum = (int)(-6000 / (double)(numericUpDown1.Value));
            //chart1.ChartAreas["ChartArea_Ecg"].AxisY.Interval = (int)chart1.ChartAreas["ChartArea_Ecg"].AxisY.Maximum / 6;
            //chart1.ChartAreas["ChartArea_Ecg"].AxisY.Maximum = chart1.ChartAreas["ChartArea_Ecg"].AxisY.Interval * 6;
            //chart1.ChartAreas["ChartArea_Ecg"].AxisY.Minimum = -chart1.ChartAreas["ChartArea_Ecg"].AxisY.Interval * 6;
            //chart1.ChartAreas["ChartArea_Ecg"].AxisY.MinorGrid.Interval = chart1.ChartAreas["ChartArea_Ecg"].AxisY.Interval / 5;
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

            for (int i = 0; i < UPDATACOMNUM; i++)
            {
                if (UpDataCheckBox[i].Checked == true)
                {
                    DownLoadFlag[i] = 1;
                    //SerialReceiveData.Clear();
                    serialcommand.FlashEraseAllUnsecure_Command(UpDataCom[i]);
                    num = 0;
                    while (DownLoadFlag[i] == 1)
                    {
                        Thread.Sleep(10);
                        num++;
                        if (num > 100)
                        {
                            num = 0;
                            string DisplayString = ConnectUartName[i] + ":擦除flash无响应！\r\n请重新升级!\r\n";
                            DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                            OutMsg(MonitorText, DisplayString, Color.Red);
                            UpDataCheckBox[i].Checked = false;
                            //SerialReceiveData.Clear();
                            break;
                        }
                    }
                    if (DownLoadFlag[i] == 2)
                    {
                        string DisplayString = ConnectUartName[i] + ":擦除flash成功！\r\n";
                        DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                        OutMsg(MonitorText, DisplayString, Color.Red);

                        serialcommand.Upgrade_ACKCommand(UpDataCom[i]);
                    }
                    else if (DownLoadFlag[i] == 3)
                    {
                        string DisplayString = ConnectUartName[i] + ":擦除flash失败！\r\n请重新升级!\r\n";
                        DisplayString = DateTime.Now.ToLongTimeString() + ": " + DisplayString;
                        OutMsg(MonitorText, DisplayString, Color.Red);
                        //checkBox3.Checked = false;
                    }

                    Thread.Sleep(5);
                }
            }
            Upgrade();

        }

        private void Upgrade()
        {
            string[] text = File.ReadAllLines(textBox3.Text);

            for (int i = 0; i < UPDATACOMNUM; i++)
            {
                UpDataProgressBar[i].Maximum = text.Length / 2;
                UpDataProgressBar[i].Value = 0;

                if (UpDataCheckBox[i].Checked == true)
                {
                    if (UpDataThread[i].IsAlive)
                    {
                        UpDataThread[i].Abort();
                    }
                    UpDataThread[i] = new Thread(new ParameterizedThreadStart(Upgrade2));
                    UpDataThread[i].Start(UpDataCom[i]);
                    DowmLoadNum++;
                }
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
                UpDataProgressBar[i].Invoke(new EventHandler(delegate
                {
                        UpDataProgressBar[i].Value++;
                }));
                UpDataLabel[i].Invoke(new EventHandler(delegate
                {
                        int a = UpDataProgressBar[i].Value * 100 / UpDataProgressBar[i].Maximum;
                        UpDataLabel[i].Text = "正在下载中..." + a.ToString() + "%";
                }));

            }
            if (StoreDataList.Count > 0)
            {
                List2Byte(StoreDataList, buffer, StoreDataList.Count);
                if (WriteLastFlash(i, serialport, buffer, (UInt16)StoreDataList.Count) != 0)
                {
                    return -4;
                }
                StoreDataList.RemoveRange(0, StoreDataList.Count);
                UpDataProgressBar[i].Invoke(new EventHandler(delegate
                {
                    UpDataProgressBar[i].Value++;
                }));
                UpDataLabel[i].Invoke(new EventHandler(delegate
                {
                    int a = UpDataProgressBar[i].Value * 100 / UpDataProgressBar[i].Maximum;
                    UpDataLabel[i].Text = "正在下载中..." + a.ToString() + "%";
                }));
            }
            return 0;
        }

        void Upgrade2(object serialport)
        {
            int i = 0;
            List<byte> DataBuffer = new List<byte>(999999);
            List<int> LineAddr = new List<int>(9999);

            SerialPort parameter = serialport as SerialPort;//类型转换 

            if (parameter == UpDataCom[0])
            {
                i = 0;
            }
            else if (parameter == UpDataCom[1])
            {
                i = 1;
            }
            else if (parameter == UpDataCom[2])
            {
                i = 2;
            }
            else if (parameter == UpDataCom[3])
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
                                UpDataLabel[i].Invoke(new EventHandler(delegate
                                {
                                    UpDataLabel[i].Text = "升级失败，请重新升级!";
                                }));
                                DowmLoadNum--;

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
                                UpDataLabel[i].Invoke(new EventHandler(delegate
                                {
                                    UpDataLabel[i].Text = "升级失败，请重新升级!";
                                }));
                                DowmLoadNum--;
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

                        UpDataLabel[i].Invoke(new EventHandler(delegate
                        {
                            UpDataLabel[i].Text = "下载完成!";
                        }));
                        DowmLoadNum--;
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
            if (ScanEcgUartThread.IsAlive)
            {
                ScanEcgUartThread.Abort();
            }

            if (ScanUpdataThread.IsAlive)
            {
                ScanUpdataThread.Abort();
            }

            for (int i = 0; i < UPDATACOMNUM; i++)
            {
                if (UpDataThread[i].IsAlive)
                {
                    UpDataThread[i].Abort();
                }
            }
            if (EcgUartProcessThread.IsAlive)
            {
                EcgUartProcessThread.Abort();
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

        private void MAClistBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            
        }

        private void MAClistBox_DoubleClick(object sender, EventArgs e)
        {
            if (EcgUartProcessThread.IsAlive)
            {
                EcgUartProcessThread.Abort();
            }
            if (ScanBleName[MAClistBox.SelectedIndex, 0] != 'B' || (ScanBleName[MAClistBox.SelectedIndex, 1] != 'D'))
            {
                MessageBox.Show("连接设备不是心电补丁", "蓝牙连接错误", MessageBoxButtons.OK, MessageBoxIcon.Question);
            }
            else
            {
                ConnectDeviceFlag = 1;
                ScanButton.Text = "正在连接心电补丁";
            }

            EcgUartProcessThread = new Thread(new ThreadStart(EcgUartProcessFunction));
            EcgUartProcessThread.Start();

        }

        private void MAClistBox_Click(object sender, EventArgs e)
        {
            if (MAClistBox.SelectedIndex < 1000 && (MAClistBox.SelectedIndex>=0))
            {
                textBox5.Text = ScanBLEMAC[MAClistBox.SelectedIndex, 5].ToString("X2") + ":" + ScanBLEMAC[MAClistBox.SelectedIndex, 4].ToString("X2") + ":" + ScanBLEMAC[MAClistBox.SelectedIndex, 3].ToString("X2") + ":" + ScanBLEMAC[MAClistBox.SelectedIndex, 2].ToString("X2") + ":" + ScanBLEMAC[MAClistBox.SelectedIndex, 1].ToString("X2") + ":" + ScanBLEMAC[MAClistBox.SelectedIndex, 0].ToString("X2");
                PairButton.Enabled = true;
            }
        }
    }

}
