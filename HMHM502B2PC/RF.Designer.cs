﻿namespace MotionSensor
{
    partial class RF
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Series series1 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.DataPoint dataPoint1 = new System.Windows.Forms.DataVisualization.Charting.DataPoint(0D, "0,0");
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(RF));
            this.Lea = new System.Windows.Forms.Panel();
            this.label12 = new System.Windows.Forms.Label();
            this.Lead = new System.Windows.Forms.RichTextBox();
            this.BatteryValue = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.DataStoreButton = new System.Windows.Forms.Button();
            this.Timer = new System.Timers.Timer();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.skinEngine1 = new Sunisoft.IrisSkin.SkinEngine(((System.ComponentModel.Component)(this)));
            this.MonitorText = new System.Windows.Forms.RichTextBox();
            this.ScanButton = new System.Windows.Forms.Button();
            this.label11 = new System.Windows.Forms.Label();
            this.PauseButton = new System.Windows.Forms.Button();
            this.textBoxScan = new System.Windows.Forms.TextBox();
            this.IDValuebutton = new System.Windows.Forms.Button();
            this.label10 = new System.Windows.Forms.Label();
            this.checkBox1 = new System.Windows.Forms.CheckBox();
            this.button5 = new System.Windows.Forms.Button();
            this.PairButton = new System.Windows.Forms.Button();
            this.textBox5 = new System.Windows.Forms.TextBox();
            this.chart1 = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.button1 = new System.Windows.Forms.Button();
            this.AmplificationValue = new System.Windows.Forms.Label();
            this.differenceValue = new System.Windows.Forms.Label();
            this.label15 = new System.Windows.Forms.Label();
            this.label16 = new System.Windows.Forms.Label();
            this.label17 = new System.Windows.Forms.Label();
            this.label18 = new System.Windows.Forms.Label();
            this.label19 = new System.Windows.Forms.Label();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabel2 = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabel3 = new System.Windows.Forms.ToolStripStatusLabel();
            this.PatchMACLabel = new System.Windows.Forms.Label();
            this.MAClistBox = new System.Windows.Forms.ListBox();
            this.ConnectBLEButton = new System.Windows.Forms.Button();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.MACComboBox = new System.Windows.Forms.ComboBox();
            this.button2 = new System.Windows.Forms.Button();
            this.textBox3 = new System.Windows.Forms.TextBox();
            this.UpDataButton = new System.Windows.Forms.Button();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.menuStrip2 = new System.Windows.Forms.MenuStrip();
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.statusStrip2 = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel4 = new System.Windows.Forms.ToolStripStatusLabel();
            this.Lea.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.Timer)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart1)).BeginInit();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.statusStrip1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.statusStrip2.SuspendLayout();
            this.SuspendLayout();
            // 
            // Lea
            // 
            this.Lea.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.Lea.Controls.Add(this.label12);
            this.Lea.Controls.Add(this.Lead);
            this.Lea.Controls.Add(this.BatteryValue);
            this.Lea.Controls.Add(this.label3);
            this.Lea.Controls.Add(this.label8);
            this.Lea.Controls.Add(this.label1);
            this.Lea.Location = new System.Drawing.Point(547, 12);
            this.Lea.Name = "Lea";
            this.Lea.Size = new System.Drawing.Size(169, 102);
            this.Lea.TabIndex = 1;
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(135, 23);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(11, 12);
            this.label12.TabIndex = 19;
            this.label12.Text = "%";
            // 
            // Lead
            // 
            this.Lead.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.Lead.Location = new System.Drawing.Point(62, 56);
            this.Lead.Name = "Lead";
            this.Lead.Size = new System.Drawing.Size(100, 26);
            this.Lead.TabIndex = 18;
            this.Lead.TabStop = false;
            this.Lead.Text = "";
            // 
            // BatteryValue
            // 
            this.BatteryValue.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.BatteryValue.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.BatteryValue.Location = new System.Drawing.Point(73, 18);
            this.BatteryValue.Name = "BatteryValue";
            this.BatteryValue.ReadOnly = true;
            this.BatteryValue.Size = new System.Drawing.Size(54, 23);
            this.BatteryValue.TabIndex = 17;
            this.BatteryValue.TabStop = false;
            this.BatteryValue.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label3.Location = new System.Drawing.Point(8, 66);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(40, 16);
            this.label3.TabIndex = 11;
            this.label3.Text = "导联";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label8.Location = new System.Drawing.Point(17, 18);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(40, 16);
            this.label8.TabIndex = 10;
            this.label8.Text = "电量";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label1.Location = new System.Drawing.Point(59, -1);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(35, 14);
            this.label1.TabIndex = 1;
            this.label1.Text = "数据";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label4.Location = new System.Drawing.Point(126, 182);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(35, 14);
            this.label4.TabIndex = 6;
            this.label4.Text = "设置";
            // 
            // DataStoreButton
            // 
            this.DataStoreButton.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.DataStoreButton.Enabled = false;
            this.DataStoreButton.Location = new System.Drawing.Point(741, 79);
            this.DataStoreButton.Name = "DataStoreButton";
            this.DataStoreButton.Size = new System.Drawing.Size(64, 37);
            this.DataStoreButton.TabIndex = 9;
            this.DataStoreButton.Text = "数据保存";
            this.DataStoreButton.UseVisualStyleBackColor = false;
            this.DataStoreButton.Click += new System.EventHandler(this.button1_Click_1);
            // 
            // Timer
            // 
            this.Timer.Interval = 0.1D;
            this.Timer.SynchronizingObject = this;
            this.Timer.Elapsed += new System.Timers.ElapsedEventHandler(this.timer1_Elapsed);
            // 
            // skinEngine1
            // 
            this.skinEngine1.SerialNumber = "";
            this.skinEngine1.SkinFile = null;
            // 
            // MonitorText
            // 
            this.MonitorText.Location = new System.Drawing.Point(837, 23);
            this.MonitorText.Name = "MonitorText";
            this.MonitorText.Size = new System.Drawing.Size(271, 629);
            this.MonitorText.TabIndex = 8;
            this.MonitorText.Text = "";
            this.MonitorText.TextChanged += new System.EventHandler(this.richTextBox3_TextChanged_1);
            // 
            // ScanButton
            // 
            this.ScanButton.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.ScanButton.Enabled = false;
            this.ScanButton.Location = new System.Drawing.Point(579, 262);
            this.ScanButton.Name = "ScanButton";
            this.ScanButton.Size = new System.Drawing.Size(200, 23);
            this.ScanButton.TabIndex = 10;
            this.ScanButton.Text = "搜索设备";
            this.ScanButton.UseVisualStyleBackColor = false;
            this.ScanButton.Click += new System.EventHandler(this.ScanButton_Click);
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(550, 148);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(125, 12);
            this.label11.TabIndex = 21;
            this.label11.Text = "搜索到的蓝牙设备MAC:";
            // 
            // PauseButton
            // 
            this.PauseButton.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.PauseButton.Enabled = false;
            this.PauseButton.Location = new System.Drawing.Point(722, 339);
            this.PauseButton.Name = "PauseButton";
            this.PauseButton.Size = new System.Drawing.Size(93, 23);
            this.PauseButton.TabIndex = 24;
            this.PauseButton.Text = "暂停";
            this.PauseButton.UseVisualStyleBackColor = false;
            this.PauseButton.Click += new System.EventHandler(this.PauseButton_Click);
            // 
            // textBoxScan
            // 
            this.textBoxScan.Location = new System.Drawing.Point(563, 379);
            this.textBoxScan.MaxLength = 10;
            this.textBoxScan.Name = "textBoxScan";
            this.textBoxScan.Size = new System.Drawing.Size(157, 21);
            this.textBoxScan.TabIndex = 26;
            this.textBoxScan.Text = "D123456789";
            this.textBoxScan.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.textBoxScan_KeyPress);
            // 
            // IDValuebutton
            // 
            this.IDValuebutton.Enabled = false;
            this.IDValuebutton.Location = new System.Drawing.Point(722, 379);
            this.IDValuebutton.Name = "IDValuebutton";
            this.IDValuebutton.Size = new System.Drawing.Size(93, 23);
            this.IDValuebutton.TabIndex = 29;
            this.IDValuebutton.Text = "ID值设置";
            this.IDValuebutton.UseVisualStyleBackColor = true;
            this.IDValuebutton.Click += new System.EventHandler(this.IDValuebutton_Click);
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(566, 344);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(137, 12);
            this.label10.TabIndex = 31;
            this.label10.Text = "请输入ID值并回车或扫描";
            // 
            // checkBox1
            // 
            this.checkBox1.AutoSize = true;
            this.checkBox1.Enabled = false;
            this.checkBox1.Location = new System.Drawing.Point(719, 147);
            this.checkBox1.Name = "checkBox1";
            this.checkBox1.Size = new System.Drawing.Size(96, 16);
            this.checkBox1.TabIndex = 33;
            this.checkBox1.Text = "自动搜索连接";
            this.checkBox1.UseVisualStyleBackColor = true;
            this.checkBox1.CheckedChanged += new System.EventHandler(this.checkBox1_CheckedChanged);
            // 
            // button5
            // 
            this.button5.Enabled = false;
            this.button5.Location = new System.Drawing.Point(579, 406);
            this.button5.Name = "button5";
            this.button5.Size = new System.Drawing.Size(93, 23);
            this.button5.TabIndex = 36;
            this.button5.Text = "1mv定标";
            this.button5.UseVisualStyleBackColor = true;
            this.button5.Click += new System.EventHandler(this.button5_Click);
            // 
            // PairButton
            // 
            this.PairButton.Enabled = false;
            this.PairButton.Location = new System.Drawing.Point(607, 509);
            this.PairButton.Name = "PairButton";
            this.PairButton.Size = new System.Drawing.Size(143, 23);
            this.PairButton.TabIndex = 37;
            this.PairButton.Text = "配对";
            this.PairButton.UseVisualStyleBackColor = true;
            this.PairButton.Click += new System.EventHandler(this.button6_Click);
            // 
            // textBox5
            // 
            this.textBox5.Location = new System.Drawing.Point(595, 482);
            this.textBox5.Name = "textBox5";
            this.textBox5.Size = new System.Drawing.Size(165, 21);
            this.textBox5.TabIndex = 38;
            this.textBox5.TextChanged += new System.EventHandler(this.textBox5_TextChanged);
            // 
            // chart1
            // 
            chartArea1.AxisX.Interval = 128D;
            chartArea1.AxisX.LineDashStyle = System.Windows.Forms.DataVisualization.Charting.ChartDashStyle.Dot;
            chartArea1.AxisX.MajorGrid.LineColor = System.Drawing.Color.Gray;
            chartArea1.AxisX.Maximum = 512D;
            chartArea1.AxisX.Minimum = 0D;
            chartArea1.AxisX.MinorGrid.Enabled = true;
            chartArea1.AxisX.MinorGrid.Interval = 16D;
            chartArea1.AxisX.MinorGrid.LineColor = System.Drawing.Color.Gray;
            chartArea1.AxisX.MinorGrid.LineDashStyle = System.Windows.Forms.DataVisualization.Charting.ChartDashStyle.Dot;
            chartArea1.AxisX.Title = "时间/ms";
            chartArea1.AxisY.Interval = 1000D;
            chartArea1.AxisY.MajorGrid.LineColor = System.Drawing.Color.Gray;
            chartArea1.AxisY.Maximum = 6000D;
            chartArea1.AxisY.Minimum = -6000D;
            chartArea1.AxisY.MinorGrid.Enabled = true;
            chartArea1.AxisY.MinorGrid.Interval = 200D;
            chartArea1.AxisY.MinorGrid.LineColor = System.Drawing.Color.Gray;
            chartArea1.AxisY.MinorGrid.LineDashStyle = System.Windows.Forms.DataVisualization.Charting.ChartDashStyle.DashDotDot;
            chartArea1.AxisY.MinorTickMark.LineColor = System.Drawing.Color.White;
            chartArea1.AxisY.Title = "心电数据/1uv";
            chartArea1.BackColor = System.Drawing.Color.Black;
            chartArea1.Name = "ChartArea_Ecg";
            this.chart1.ChartAreas.Add(chartArea1);
            this.chart1.Location = new System.Drawing.Point(6, 0);
            this.chart1.Name = "chart1";
            series1.ChartArea = "ChartArea_Ecg";
            series1.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Spline;
            series1.Color = System.Drawing.Color.Lime;
            series1.Name = "Series_Ecg";
            dataPoint1.ToolTip = "#VAL";
            series1.Points.Add(dataPoint1);
            series1.ShadowColor = System.Drawing.Color.Red;
            series1.YValuesPerPoint = 2;
            this.chart1.Series.Add(series1);
            this.chart1.Size = new System.Drawing.Size(541, 503);
            this.chart1.TabIndex = 41;
            this.chart1.Text = "chart1";
            this.chart1.GetToolTipText += new System.EventHandler<System.Windows.Forms.DataVisualization.Charting.ToolTipEventArgs>(this.chart1_GetToolTipText);
            this.chart1.MouseDown += new System.Windows.Forms.MouseEventHandler(this.chart1_MouseDown);
            this.chart1.MouseEnter += new System.EventHandler(this.chart1_MouseEnter);
            this.chart1.MouseLeave += new System.EventHandler(this.chart1_MouseLeave);
            this.chart1.MouseUp += new System.Windows.Forms.MouseEventHandler(this.chart1_MouseUp);
            // 
            // button1
            // 
            this.button1.Enabled = false;
            this.button1.Location = new System.Drawing.Point(579, 441);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(93, 23);
            this.button1.TabIndex = 42;
            this.button1.Text = "0Mv校准";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // AmplificationValue
            // 
            this.AmplificationValue.AutoSize = true;
            this.AmplificationValue.Location = new System.Drawing.Point(678, 411);
            this.AmplificationValue.Name = "AmplificationValue";
            this.AmplificationValue.Size = new System.Drawing.Size(137, 12);
            this.AmplificationValue.TabIndex = 43;
            this.AmplificationValue.Text = "未定标或无法获取定标值";
            this.AmplificationValue.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // differenceValue
            // 
            this.differenceValue.AutoSize = true;
            this.differenceValue.Location = new System.Drawing.Point(678, 447);
            this.differenceValue.Name = "differenceValue";
            this.differenceValue.Size = new System.Drawing.Size(137, 12);
            this.differenceValue.TabIndex = 44;
            this.differenceValue.Text = "未定标或无法获取定标值";
            this.differenceValue.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(8, 514);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(47, 12);
            this.label15.TabIndex = 45;
            this.label15.Text = "label15";
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Location = new System.Drawing.Point(8, 526);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(47, 12);
            this.label16.TabIndex = 46;
            this.label16.Text = "label16";
            this.label16.Visible = false;
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Location = new System.Drawing.Point(214, 520);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(137, 12);
            this.label17.TabIndex = 47;
            this.label17.Text = "获取的第一个点的值:0mV";
            // 
            // label18
            // 
            this.label18.AutoSize = true;
            this.label18.Location = new System.Drawing.Point(214, 541);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(137, 12);
            this.label18.TabIndex = 48;
            this.label18.Text = "获取的第二个点的值:0mV";
            // 
            // label19
            // 
            this.label19.AutoSize = true;
            this.label19.Location = new System.Drawing.Point(406, 515);
            this.label19.Name = "label19";
            this.label19.Size = new System.Drawing.Size(65, 12);
            this.label19.TabIndex = 49;
            this.label19.Text = "高度差:0mV";
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Controls.Add(this.tabPage3);
            this.tabControl1.Location = new System.Drawing.Point(0, 23);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(831, 629);
            this.tabControl1.TabIndex = 59;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.statusStrip2);
            this.tabPage1.Controls.Add(this.statusStrip1);
            this.tabPage1.Controls.Add(this.PatchMACLabel);
            this.tabPage1.Controls.Add(this.MAClistBox);
            this.tabPage1.Controls.Add(this.DataStoreButton);
            this.tabPage1.Controls.Add(this.chart1);
            this.tabPage1.Controls.Add(this.label18);
            this.tabPage1.Controls.Add(this.label19);
            this.tabPage1.Controls.Add(this.label17);
            this.tabPage1.Controls.Add(this.label16);
            this.tabPage1.Controls.Add(this.Lea);
            this.tabPage1.Controls.Add(this.label15);
            this.tabPage1.Controls.Add(this.label11);
            this.tabPage1.Controls.Add(this.label4);
            this.tabPage1.Controls.Add(this.checkBox1);
            this.tabPage1.Controls.Add(this.ScanButton);
            this.tabPage1.Controls.Add(this.PauseButton);
            this.tabPage1.Controls.Add(this.ConnectBLEButton);
            this.tabPage1.Controls.Add(this.textBoxScan);
            this.tabPage1.Controls.Add(this.IDValuebutton);
            this.tabPage1.Controls.Add(this.label10);
            this.tabPage1.Controls.Add(this.differenceValue);
            this.tabPage1.Controls.Add(this.button5);
            this.tabPage1.Controls.Add(this.AmplificationValue);
            this.tabPage1.Controls.Add(this.PairButton);
            this.tabPage1.Controls.Add(this.button1);
            this.tabPage1.Controls.Add(this.textBox5);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(823, 603);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "心电补丁测试";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel1,
            this.toolStripStatusLabel2,
            this.toolStripStatusLabel3});
            this.statusStrip1.Location = new System.Drawing.Point(3, 578);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(817, 22);
            this.statusStrip1.TabIndex = 52;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // toolStripStatusLabel1
            // 
            this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
            this.toolStripStatusLabel1.Size = new System.Drawing.Size(131, 17);
            this.toolStripStatusLabel1.Text = "toolStripStatusLabel1";
            // 
            // toolStripStatusLabel2
            // 
            this.toolStripStatusLabel2.Name = "toolStripStatusLabel2";
            this.toolStripStatusLabel2.Size = new System.Drawing.Size(131, 17);
            this.toolStripStatusLabel2.Text = "toolStripStatusLabel2";
            // 
            // toolStripStatusLabel3
            // 
            this.toolStripStatusLabel3.Name = "toolStripStatusLabel3";
            this.toolStripStatusLabel3.Size = new System.Drawing.Size(131, 17);
            this.toolStripStatusLabel3.Text = "toolStripStatusLabel3";
            // 
            // PatchMACLabel
            // 
            this.PatchMACLabel.AutoSize = true;
            this.PatchMACLabel.Location = new System.Drawing.Point(556, 541);
            this.PatchMACLabel.Name = "PatchMACLabel";
            this.PatchMACLabel.Size = new System.Drawing.Size(83, 12);
            this.PatchMACLabel.TabIndex = 51;
            this.PatchMACLabel.Text = "当前配对MAC：";
            // 
            // MAClistBox
            // 
            this.MAClistBox.FormattingEnabled = true;
            this.MAClistBox.ItemHeight = 12;
            this.MAClistBox.Location = new System.Drawing.Point(547, 168);
            this.MAClistBox.Name = "MAClistBox";
            this.MAClistBox.Size = new System.Drawing.Size(268, 88);
            this.MAClistBox.TabIndex = 50;
            this.MAClistBox.Click += new System.EventHandler(this.MAClistBox_Click);
            this.MAClistBox.SelectedIndexChanged += new System.EventHandler(this.MAClistBox_SelectedIndexChanged);
            this.MAClistBox.DoubleClick += new System.EventHandler(this.MAClistBox_DoubleClick);
            // 
            // ConnectBLEButton
            // 
            this.ConnectBLEButton.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.ConnectBLEButton.Enabled = false;
            this.ConnectBLEButton.Location = new System.Drawing.Point(723, 310);
            this.ConnectBLEButton.Name = "ConnectBLEButton";
            this.ConnectBLEButton.Size = new System.Drawing.Size(93, 23);
            this.ConnectBLEButton.TabIndex = 18;
            this.ConnectBLEButton.Text = "设备已断开";
            this.ConnectBLEButton.UseVisualStyleBackColor = false;
            this.ConnectBLEButton.Click += new System.EventHandler(this.ConnectBLEButton_Click);
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.MACComboBox);
            this.tabPage2.Controls.Add(this.button2);
            this.tabPage2.Controls.Add(this.textBox3);
            this.tabPage2.Controls.Add(this.UpDataButton);
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(823, 603);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "程序下载";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // MACComboBox
            // 
            this.MACComboBox.Enabled = false;
            this.MACComboBox.FormattingEnabled = true;
            this.MACComboBox.Items.AddRange(new object[] {
            "无设备"});
            this.MACComboBox.Location = new System.Drawing.Point(283, 279);
            this.MACComboBox.Name = "MACComboBox";
            this.MACComboBox.Size = new System.Drawing.Size(257, 20);
            this.MACComboBox.TabIndex = 61;
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(182, 101);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(109, 23);
            this.button2.TabIndex = 60;
            this.button2.Text = "选择下载文件";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click_1);
            // 
            // textBox3
            // 
            this.textBox3.Location = new System.Drawing.Point(310, 101);
            this.textBox3.Name = "textBox3";
            this.textBox3.Size = new System.Drawing.Size(273, 21);
            this.textBox3.TabIndex = 59;
            this.textBox3.Text = "D:\\HM502B2.hex";
            // 
            // UpDataButton
            // 
            this.UpDataButton.Enabled = false;
            this.UpDataButton.Location = new System.Drawing.Point(100, 181);
            this.UpDataButton.Name = "UpDataButton";
            this.UpDataButton.Size = new System.Drawing.Size(93, 23);
            this.UpDataButton.TabIndex = 30;
            this.UpDataButton.Text = "程序下载";
            this.UpDataButton.UseVisualStyleBackColor = true;
            this.UpDataButton.Click += new System.EventHandler(this.button2_Click);
            // 
            // tabPage3
            // 
            this.tabPage3.Location = new System.Drawing.Point(4, 22);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Size = new System.Drawing.Size(823, 603);
            this.tabPage3.TabIndex = 2;
            this.tabPage3.Text = "数据保存";
            this.tabPage3.UseVisualStyleBackColor = true;
            // 
            // menuStrip2
            // 
            this.menuStrip2.Location = new System.Drawing.Point(0, 0);
            this.menuStrip2.Name = "menuStrip2";
            this.menuStrip2.Size = new System.Drawing.Size(1108, 24);
            this.menuStrip2.TabIndex = 60;
            this.menuStrip2.Text = "menuStrip2";
            // 
            // statusStrip2
            // 
            this.statusStrip2.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel4});
            this.statusStrip2.Location = new System.Drawing.Point(3, 556);
            this.statusStrip2.Name = "statusStrip2";
            this.statusStrip2.Size = new System.Drawing.Size(817, 22);
            this.statusStrip2.TabIndex = 53;
            this.statusStrip2.Text = "statusStrip2";
            // 
            // toolStripStatusLabel4
            // 
            this.toolStripStatusLabel4.Name = "toolStripStatusLabel4";
            this.toolStripStatusLabel4.Size = new System.Drawing.Size(131, 17);
            this.toolStripStatusLabel4.Text = "toolStripStatusLabel4";
            // 
            // RF
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ButtonShadow;
            this.ClientSize = new System.Drawing.Size(1108, 652);
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this.MonitorText);
            this.Controls.Add(this.menuStrip2);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "RF";
            this.Text = "HM502B2心电数据显示软件V2.00(201507016)";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.RF_FormClosed);
            this.Load += new System.EventHandler(this.Motion_Load);
            this.Lea.ResumeLayout(false);
            this.Lea.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.Timer)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart1)).EndInit();
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.tabPage2.ResumeLayout(false);
            this.tabPage2.PerformLayout();
            this.statusStrip2.ResumeLayout(false);
            this.statusStrip2.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel Lea;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label4;
        private System.Timers.Timer Timer;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
        private Sunisoft.IrisSkin.SkinEngine skinEngine1;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button ScanButton;
        private System.Windows.Forms.Button DataStoreButton;
        private System.Windows.Forms.RichTextBox MonitorText;
        private System.Windows.Forms.TextBox BatteryValue;
        private System.Windows.Forms.RichTextBox Lead;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Button PauseButton;
        private System.Windows.Forms.Button IDValuebutton;
        private System.Windows.Forms.TextBox textBoxScan;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.CheckBox checkBox1;
        private System.Windows.Forms.Button button5;
        private System.Windows.Forms.Button PairButton;
        private System.Windows.Forms.TextBox textBox5;
        private System.Windows.Forms.DataVisualization.Charting.Chart chart1;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Label differenceValue;
        private System.Windows.Forms.Label AmplificationValue;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.Label label19;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.MenuStrip menuStrip2;
        private System.Windows.Forms.TabPage tabPage3;
        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.ListBox MAClistBox;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.ComboBox MACComboBox;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.TextBox textBox3;
        private System.Windows.Forms.Button UpDataButton;
        private System.Windows.Forms.Label PatchMACLabel;
        private System.Windows.Forms.Button ConnectBLEButton;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel2;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel3;
        private System.Windows.Forms.StatusStrip statusStrip2;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel4;
    }
}

