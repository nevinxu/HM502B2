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
        const int N = 48;
        string[] Xdata = new string[16 * N];
        double[] XdataV = new double[16 * N];

        byte[] comreceivedata = new byte[200];

        int chartlenMin = 0;
        int chartlenMax = N * 16;
        int zb_time;
        int length;

        private string comName; //port name
        private int comBaudRate; //port baudrate
        private int comDataBits = 8;    //port databits
        private int rate = 0;
        private int Vbat = 0;
        private int leadoff = 0;
        private int mtk_fig = 0xFFFF;
        private int bp_fig = 0;


        public RF()
        {
            InitializeComponent();
            skinEngine1.SkinFile = Application.StartupPath + @"\gu.hw";
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
            System.Threading.Thread.Sleep(5);

            length = serialPort1.BytesToRead;
            byte[] data = new byte[length];
            for (int i = 0; i < length; i++)
            {
                data[i] = (byte)serialPort1.ReadByte();
            }
            if ((length > 2) && (data[0] == 'B') && (data[1] == 'P'))
            {
                bp_fig++;
            }
            if (length == 19)
            {
                for (int j = 0; j < 16; j++)
                {
                    int k;
                    for (k = 0; k < N - 1; k++)
                        XdataV[j + k * 16] = XdataV[(k + 1) * 16 + j];
                    XdataV[k * 16 + j] = Convert.ToDouble(data[j + 3]);
                }
              //  rate = Convert.ToInt32(data[135]);
                Vbat = Convert.ToInt32(data[1]);
                leadoff = Convert.ToInt32(data[2]);
                FileStream fs = null;
                string filePath = System.IO.Directory.GetCurrentDirectory() + "//ecg_data.bin";
                try
                {
                    fs = File.OpenWrite(filePath);
                    //设定书写的开始位置为文件的末尾   
                    fs.Position = fs.Length;
                    //将待写入内容追加到文件末尾   
                    //  fs.Write(bytes, 0, bytes.Length);
                    fs.Write(data, 3, 16);
                }
                catch (Exception ex)
                {
                    Console.WriteLine("文件打开失败{0}", ex.ToString());
                }
                finally
                {
                    fs.Close();
                }
                length = 0;
                // FileStream fst = new FileStream(sss, FileMode.Append);
                //  fst.Write(data, 5, 128);
            }
            else {
                for (int i = 0; i < length; i++)
                {
                    comreceivedata[i] = data[i];
                }
            }
            //if (length == 139 && data[0] == '[')
            //{
            //    byte[] ssss = new byte[10];
            //    ssss[0] = (byte)'M';
            //    ssss[1] = (byte)'T';
            //    ssss[2] = (byte)'K';
            //    ssss[3] = data[136];
            //    serialPort1.Write(ssss, 0, 10);
            //    if (mtk_fig != Convert.ToInt32(data[136]))
            //    {
            //        mtk_fig = Convert.ToInt32(data[136]);
            //        for (int j = 0; j < 128; j++)
            //        {
            //            int k;
            //            for (k = 0; k < N - 1; k++)
            //                XdataV[j + k * 128] = XdataV[(k + 1) * 128 + j];
            //            XdataV[k * 128 + j] = Convert.ToDouble(data[j + 5]);
            //        }
            //        rate = Convert.ToInt32(data[135]);
            //        Vbat = Convert.ToInt32(data[134]);
            //        leadoff = Convert.ToInt32(data[133]);
            //        zb_time = Convert.ToInt32(data[136]);
            //        string sss = "D://ecg_data.txt";
            //        FileStream fst = new FileStream(sss, FileMode.Append);
            //        fst.Write(data, 5, 128);
            //        fst.Close();
            //    }
            //    else
            //    {
            //        this.toolStripStatusLabel1.Text = "重传" + mtk_fig.ToString();
            //    }
            //}
            serialPort1.DiscardInBuffer();
            WriteTheWin();
        }
        #endregion


        #region 串口初始化
        bool ComInit()
        {
            try
            {
                comName = comboBoxCom.Text;
                comBaudRate = int.Parse(comboBoxPortel.Text);
                serialPort1.PortName = comName; //port name COM1
                serialPort1.BaudRate = comBaudRate;  //BaudRate 115200bps
                serialPort1.Parity = Parity.None; //Parity none
                serialPort1.StopBits = StopBits.One; //StopBits 1
                serialPort1.DataBits = comDataBits;        // DataBits 8bit
                serialPort1.ReadTimeout = 2000;  // ReadTimeout 2 second
                serialPort1.Open();//打开串口
                this.toolStripStatusLabel1.Text = "已经打开串口：" + comName + ", 串口波特率：" + comBaudRate.ToString() +
                    "  数据位：8位，  停止位:1位， 检验位：None,  流控制：None";
                this.serialPort1.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.serialPort1_DataReceived);
                serialPort1.ReceivedBytesThreshold = 2;
                return true;
            }
            catch (System.Exception)
            {
                MessageBox.Show("不能打开当前串口", "系统初始化失败", MessageBoxButtons.OK, MessageBoxIcon.Question);
                return false;
            }
        }
        #endregion

        #region 设置ZB频率
        private void zb_send_frequency()
        {
            serialPort1.WriteLine("HMC0000000");
            System.Threading.Thread.Sleep(500);
        }
        #endregion

        void button5set()
        {
            if (button5.Text == "初始化串口")
            {
                if (ComInit())
                {
                    comboBoxCom.Enabled = false;
                    comboBoxPortel.Enabled = false;
                    button5.Text = "关闭串口";
                    zb_send_frequency();
                    button1.Enabled = true;
                    button2.Enabled = true;
                    string filePath = System.IO.Directory.GetCurrentDirectory() + "//ecg_data.bin";
                    try
                    {
                        File.Delete(filePath);
                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine("文件删除失败{0}", ex.ToString());
                    }
                    finally
                    {
                    }
                  //  startCommand();
                }
            }
            else
            {
                button5.Text = "初始化串口";
                rate = 0;
                Vbat = 0;
                leadoff = 0;
                comboBoxCom.Enabled = true;
                comboBoxPortel.Enabled = true;
                this.toolStripStatusLabel1.Text = comName + "已经关闭！";
                serialPort1.Close();
                button1.Enabled = false;
                button2.Enabled = false;
                button3.Enabled = false;
            }
            for (int i = 0; i < N * 16; i++)
            {
                XdataV[i] = 0;
            }
        }

        public void WriteTheWin()
        {
            timer1.Enabled = true;
        }

        private void Motion_Load(object sender, EventArgs e)
        {
            GetComList();
            comboBoxCom.SelectedIndex = 0;
            comboBoxPortel.SelectedIndex = 1;
            comboBox1.SelectedIndex = 0;
            radioButton1.Select();
            // chart1.Titles[0].Text = "原始心电输出信号";
            chart1.Titles[0].Alignment = ContentAlignment.TopCenter;
            chart1.Titles[0].Font = new Font("黑体", 10, FontStyle.Bold);
            chart1.Titles[0].Color = Color.FromArgb(128, 72, 72);
            chart1.ChartAreas.Clear();
            chart1.ChartAreas.Add("DX");

            chart1.ChartAreas["DX"].AxisX.Arrows = ArrowsType.Lines;
            //          chart1.ChartAreas["DX"].AxisY.Title = "幅度";

            chart1.ChartAreas["DX"].AxisY.Arrows = ArrowsType.Lines;
            chart1.ChartAreas["DX"].AxisY.Maximum = 256;
            chart1.ChartAreas["DX"].AxisY.Minimum = 100;
            chart1.ChartAreas["DX"].AxisY.Interval = 5;
            chart1.ChartAreas["DX"].AxisX.Interval = 50;
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


            for (int i = 0; i < N * 16; i++)
            {
                Xdata[i] = i.ToString();
            }
            chart1.Series["数据个数"].Points.DataBindXY(Xdata, XdataV);

            textBox1.Text = chartlenMin.ToString();
            textBox2.Text = chartlenMax.ToString();
            chart1.ChartAreas["DX"].AxisX.Maximum = chartlenMax;
            chart1.ChartAreas["DX"].AxisX.Minimum = chartlenMin;
        }

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
            if (radioButton2.Checked)
            {
                if (chartlenMax >= chartlenMin)
                {
                    chartlenMax = trackBar1.Value * N;
                }
                else
                {
                    chartlenMax = chartlenMin;
                }
            }
            else
            {
                if (chartlenMin <= chartlenMax)
                {
                    chartlenMin = trackBar1.Value * N;
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
                trackBar1.Value = chartlenMin / (N * 8);
            }
            trackBar1.Focus();
        }

        private void radioButton2_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton2.Checked)
            {
                trackBar1.Value = chartlenMax / (N * 8);
            }
            trackBar1.Focus();
        }

        private void button5_Click(object sender, EventArgs e)
        {
            button5set();
           // timer1.Enabled = true;
        }

        private void timer1_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
        {
            if (length > 0)
            {
                System.Text.ASCIIEncoding converter = new System.Text.ASCIIEncoding();
                string inputString = converter.GetString(comreceivedata);
             //   string inputString = converter.GetString(comreceivedata);
                if ((comreceivedata[0] == 'H') && (comreceivedata[1] == 'M') && (comreceivedata[2] == '5') && (comreceivedata[3] == '0') && (comreceivedata[4] == '2'))
                {
                    if (comreceivedata[5] == 'S')   //scan
                    {
                        this.comboBox1.Items.Clear();
                        for (int i = 0; i < length; i++)
                        {
                            if (comreceivedata[i] == '\n')
                            {
                                string sValue = inputString.Substring(i - 15, 14);
                                this.comboBox1.Items.Add(sValue);
                            }
                        }                        
                        this.comboBox1.SelectedIndex = 0;
                        inputString = inputString.Substring(6, length - 6);
                        comboBox1.Enabled = true;
                    }
                    else if (comreceivedata[5] == 'N')   //notify  
                    {
                        byte[] ssss = new byte[10];
                        ssss[0] = (byte)'H';
                        ssss[1] = (byte)'M';
                        ssss[2] = (byte)'5';
                        ssss[3] = (byte)'0';
                        ssss[4] = (byte)'2';

                        ssss[5] = (byte)'U';
                        serialPort1.Write(ssss, 0, 10);
                    }
                    else if (comreceivedata[5] == 'B')   //begin
                    {
                        inputString = "\r\nDevice Connected";
                        richTextBox3.AppendText(inputString);
                        StopCommand();
                    }
                }
                length = 0;
                inputString = "\r\n" + DateTime.Now.ToString() + ": " + inputString;
                richTextBox3.AppendText(inputString);
            }

           // textBox4.Text = bp_fig.ToString();
            textBox4.Text = "无数据";
            chart1.Series["数据个数"].Points.DataBindXY(Xdata, XdataV);
            if (button2.Text == "正在接收数据")
            {
                if ((20 < rate) && (rate < 255))
                {
                    textBox3.Text = rate.ToString();
                }
                else
                {
                    textBox3.Text = "无数据";
                }
                if ((80 < Vbat) && (Vbat < 255))
                {
                    richTextBox2.SelectAll();
                    richTextBox2.SelectionColor = Color.Black;
                    richTextBox2.Text = Vbat.ToString();
                    if (Vbat < 105)
                    {
                        richTextBox2.SelectAll();
                        richTextBox2.SelectionColor = Color.Red;
                        richTextBox2.Text = Vbat.ToString();
                        //  richTextBox2.Text = "电量低";
                    }
                }
                else
                {
                    //  richTextBox2.Text = "电量低";
                    richTextBox2.Text = Vbat.ToString();
                }

                if (leadoff == 0)
                {
                    richTextBox1.SelectAll();
                    richTextBox1.SelectionColor = Color.Green;
                    richTextBox1.Text = "连接正常";
                }
                else if (leadoff == 0x20)
                {
                    richTextBox1.SelectAll();
                    richTextBox1.SelectionColor = Color.Red;
                    richTextBox1.Text = "导联脱落";
                }
                else
                {
                    richTextBox1.SelectAll();
                    richTextBox1.SelectionColor = Color.Red;
                    richTextBox1.Text = "导联脱落";

                }
            }
            else
            {
                textBox3.Text = "";
                richTextBox1.Text = "";
                richTextBox2.Text = "";
                textBox4.Text = "";
                
            }
            timer1.Enabled = false;

        }

        #region 产线选择
        private void checkedListBox1_ItemCheck(object sender, ItemCheckEventArgs e)
        {
            if (e.CurrentValue == CheckState.Checked) return;
            for (int i = 0; i < ((CheckedListBox)sender).Items.Count; i++)
            {
                ((CheckedListBox)sender).SetItemChecked(i, false);
            }
            e.NewValue = CheckState.Checked;
        }
        #endregion

        private void openFileDialog1_FileOk(object sender, CancelEventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
           // SaveFileDialog saveFileDialog = new SaveFileDialog();
            saveFileDialog1.Filter = "bin文件(*.bin)|*.bin|文本文件(*.txt)|*.txt";
            saveFileDialog1.Title = "保存心电数据";
            saveFileDialog1.FilterIndex = 1;
            saveFileDialog1.RestoreDirectory = true;
            string FileName ="ecg_data" + DateTime.Now.ToShortDateString().ToString() + "-" + DateTime.Now.Hour.ToString() + "-" + DateTime.Now.Minute.ToString()+ ".bin"; // 
            saveFileDialog1.FileName = FileName;
            saveFileDialog1.AddExtension = true;
            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                //string sss = saveFileDialog1.FileName;
                //FileStream fst = new FileStream(sss, FileMode.Append);
                //StreamWriter swt = new StreamWriter(fst, System.Text.Encoding.GetEncoding("utf-8"));
                //for (int i = 0; i < 256; i++)
                //{
                //    string adrbuf = XdataV[i].ToString();
                //    swt.WriteLine(adrbuf);
                //}
                //swt.Close();
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

        private void button2_Click(object sender, EventArgs e)
        {
            byte[] ssss = new byte[10];
            if (button2.Text == "接收数据")
            {
                ssss[5] = (byte)'A';    //打开接收数据命令
                button2.Text = "正在接收数据";
                button2.Enabled = false;
            }
            if (button2.Text == "搜索设备")
            {
               // button2.Text = "接收数据";
                ssss[5] = (byte)'A';    //搜索设备命令
              //  button2.Enabled = false;
            }

                ssss[0] = (byte)'H';
                ssss[1] = (byte)'M';
                ssss[2] = (byte)'5';
                ssss[3] = (byte)'0';
                ssss[4] = (byte)'2';


                serialPort1.Write(ssss, 0, 10);
                button3.Enabled = true;
        }

        private void richTextBox3_TextChanged(object sender, EventArgs e)
        {
            richTextBox3.ScrollToCaret();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            if (button3.Text == "设备已连接")
            {
                button3.Text = "设备已断开";
                button2.Text = "搜索设备";
                comboBox1.Enabled = true;
                button2.Enabled = true;
            }
            else 
            {
                button3.Text = "设备已连接";
                button2.Text = "接收数据";
                comboBox1.Enabled = false;
              // button2.Enabled = true;
            }
            byte[] ssss = new byte[10];
            ssss[0] = (byte)'H';
            ssss[1] = (byte)'M';
            ssss[2] = (byte)'5';
            ssss[3] = (byte)'0';
            ssss[4] = (byte)'2';

            ssss[5] = (byte)'C';
            serialPort1.Write(ssss, 0, 10);
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            byte[] ssss = new byte[10];
            ssss[0] = (byte)'H';
            ssss[1] = (byte)'M';
            ssss[2] = (byte)'5';
            ssss[3] = (byte)'0';
            ssss[4] = (byte)'2';

            ssss[5] = (byte)'B';
            ssss[6] =  Convert.ToByte(comboBox1.SelectedIndex);

            if (button5.Text == "关闭串口")   //只有在串口打开的情况下才会发送数据
            {
                serialPort1.Write(ssss, 0, 10);
            }
        }

        private void comboBoxCom_SelectedIndexChanged(object sender, EventArgs e)
        {
          //  GetComList();
            //comboBoxCom.SelectedIndex = 0;
            //comboBoxPortel.SelectedIndex = 1;
            //comboBox1.SelectedIndex = 0;
        }

        private void comboBoxCom_DrawItem(object sender, DrawItemEventArgs e)
        {
            //GetComList();
        }

        private void comboBoxCom_MouseClick(object sender, MouseEventArgs e)
        {
            GetComList();
        }
        private void startCommand()
        {
            byte[] ssss = new byte[10];
            ssss[0] = (byte)'H';
            ssss[1] = (byte)'M';
            ssss[2] = (byte)'5';
            ssss[3] = (byte)'0';
            ssss[4] = (byte)'2';

            ssss[5] = (byte)'S';
            if (button5.Text == "关闭串口")
            {
                serialPort1.Write(ssss, 0, 10);
            }
        }
        private void StopCommand()
        {
            byte[] ssss = new byte[10];
            ssss[0] = (byte)'H';
            ssss[1] = (byte)'M';
            ssss[2] = (byte)'5';
            ssss[3] = (byte)'0';
            ssss[4] = (byte)'2';

            ssss[5] = (byte)'A';
            if (button5.Text == "初始化串口")
            {
                serialPort1.Write(ssss, 0, 10);
            }
        }
    }
}
