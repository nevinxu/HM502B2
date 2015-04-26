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
            Dundas.Charting.WinControl.ChartArea chartArea1 = new Dundas.Charting.WinControl.ChartArea();
            Dundas.Charting.WinControl.Legend legend1 = new Dundas.Charting.WinControl.Legend();
            Dundas.Charting.WinControl.Title title1 = new Dundas.Charting.WinControl.Title();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(RF));
            this.Lea = new System.Windows.Forms.Panel();
            this.label12 = new System.Windows.Forms.Label();
            this.Lead = new System.Windows.Forms.RichTextBox();
            this.BatteryValue = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.BloodPressure = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.HeartRate = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabel2 = new System.Windows.Forms.ToolStripStatusLabel();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.panel3 = new System.Windows.Forms.Panel();
            this.trackBar2 = new System.Windows.Forms.TrackBar();
            this.textBox4 = new System.Windows.Forms.TextBox();
            this.textBox3 = new System.Windows.Forms.TextBox();
            this.radioButton4 = new System.Windows.Forms.RadioButton();
            this.radioButton3 = new System.Windows.Forms.RadioButton();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.trackBar1 = new System.Windows.Forms.TrackBar();
            this.radioButton2 = new System.Windows.Forms.RadioButton();
            this.radioButton1 = new System.Windows.Forms.RadioButton();
            this.label4 = new System.Windows.Forms.Label();
            this.panel4 = new System.Windows.Forms.Panel();
            this.comboBoxPortel = new System.Windows.Forms.ComboBox();
            this.comboBoxCom = new System.Windows.Forms.ComboBox();
            this.SerialSetButton = new System.Windows.Forms.Button();
            this.DataStoreButton = new System.Windows.Forms.Button();
            this.label7 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.SerialPort = new System.IO.Ports.SerialPort(this.components);
            this.Timer = new System.Timers.Timer();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.skinEngine1 = new Sunisoft.IrisSkin.SkinEngine(((System.ComponentModel.Component)(this)));
            this.chart1 = new Dundas.Charting.WinControl.Chart();
            this.MonitorText = new System.Windows.Forms.RichTextBox();
            this.ScanButton = new System.Windows.Forms.Button();
            this.MACComboBox = new System.Windows.Forms.ComboBox();
            this.ConnectBLEButton = new System.Windows.Forms.Button();
            this.label11 = new System.Windows.Forms.Label();
            this.BLENUMlabel = new System.Windows.Forms.Label();
            this.PauseButton = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            this.textBoxScan = new System.Windows.Forms.TextBox();
            this.IDValuebutton = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.label10 = new System.Windows.Forms.Label();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.button3 = new System.Windows.Forms.Button();
            this.button4 = new System.Windows.Forms.Button();
            this.Lea.SuspendLayout();
            this.statusStrip1.SuspendLayout();
            this.panel3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).BeginInit();
            this.panel4.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.Timer)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart1)).BeginInit();
            this.SuspendLayout();
            // 
            // Lea
            // 
            this.Lea.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.Lea.Controls.Add(this.label12);
            this.Lea.Controls.Add(this.Lead);
            this.Lea.Controls.Add(this.BatteryValue);
            this.Lea.Controls.Add(this.label9);
            this.Lea.Controls.Add(this.BloodPressure);
            this.Lea.Controls.Add(this.label3);
            this.Lea.Controls.Add(this.label8);
            this.Lea.Controls.Add(this.label5);
            this.Lea.Controls.Add(this.HeartRate);
            this.Lea.Location = new System.Drawing.Point(602, 12);
            this.Lea.Name = "Lea";
            this.Lea.Size = new System.Drawing.Size(179, 131);
            this.Lea.TabIndex = 1;
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(48, 55);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(11, 12);
            this.label12.TabIndex = 19;
            this.label12.Text = "%";
            this.label12.Click += new System.EventHandler(this.label12_Click);
            // 
            // Lead
            // 
            this.Lead.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.Lead.Location = new System.Drawing.Point(59, 87);
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
            this.BatteryValue.Location = new System.Drawing.Point(11, 48);
            this.BatteryValue.Name = "BatteryValue";
            this.BatteryValue.ReadOnly = true;
            this.BatteryValue.Size = new System.Drawing.Size(37, 23);
            this.BatteryValue.TabIndex = 17;
            this.BatteryValue.TabStop = false;
            this.BatteryValue.Text = "无数据";
            this.BatteryValue.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label9.Location = new System.Drawing.Point(123, 20);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(40, 16);
            this.label9.TabIndex = 16;
            this.label9.Text = "血压";
            // 
            // BloodPressure
            // 
            this.BloodPressure.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.BloodPressure.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.BloodPressure.Location = new System.Drawing.Point(117, 48);
            this.BloodPressure.Name = "BloodPressure";
            this.BloodPressure.ReadOnly = true;
            this.BloodPressure.Size = new System.Drawing.Size(57, 23);
            this.BloodPressure.TabIndex = 15;
            this.BloodPressure.TabStop = false;
            this.BloodPressure.Text = "无数据";
            this.BloodPressure.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label3.Location = new System.Drawing.Point(7, 95);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(40, 16);
            this.label3.TabIndex = 11;
            this.label3.Text = "导联";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label8.Location = new System.Drawing.Point(11, 20);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(40, 16);
            this.label8.TabIndex = 10;
            this.label8.Text = "电量";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label5.Location = new System.Drawing.Point(68, 20);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(40, 16);
            this.label5.TabIndex = 8;
            this.label5.Text = "心率";
            // 
            // HeartRate
            // 
            this.HeartRate.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.HeartRate.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.HeartRate.Location = new System.Drawing.Point(68, 48);
            this.HeartRate.Name = "HeartRate";
            this.HeartRate.ReadOnly = true;
            this.HeartRate.Size = new System.Drawing.Size(48, 23);
            this.HeartRate.TabIndex = 7;
            this.HeartRate.TabStop = false;
            this.HeartRate.Text = "无数据";
            this.HeartRate.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label1.Location = new System.Drawing.Point(677, 6);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(35, 14);
            this.label1.TabIndex = 1;
            this.label1.Text = "数据";
            // 
            // statusStrip1
            // 
            this.statusStrip1.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel1,
            this.toolStripStatusLabel2});
            this.statusStrip1.Location = new System.Drawing.Point(0, 593);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(1063, 22);
            this.statusStrip1.SizingGrip = false;
            this.statusStrip1.TabIndex = 3;
            // 
            // toolStripStatusLabel1
            // 
            this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
            this.toolStripStatusLabel1.Size = new System.Drawing.Size(0, 17);
            // 
            // toolStripStatusLabel2
            // 
            this.toolStripStatusLabel2.Name = "toolStripStatusLabel2";
            this.toolStripStatusLabel2.Size = new System.Drawing.Size(128, 17);
            this.toolStripStatusLabel2.Text = "蓝牙设备状态：未连接";
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            // 
            // panel3
            // 
            this.panel3.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel3.Controls.Add(this.trackBar2);
            this.panel3.Controls.Add(this.textBox4);
            this.panel3.Controls.Add(this.textBox3);
            this.panel3.Controls.Add(this.radioButton4);
            this.panel3.Controls.Add(this.radioButton3);
            this.panel3.Controls.Add(this.textBox2);
            this.panel3.Controls.Add(this.textBox1);
            this.panel3.Controls.Add(this.trackBar1);
            this.panel3.Controls.Add(this.radioButton2);
            this.panel3.Controls.Add(this.radioButton1);
            this.panel3.Location = new System.Drawing.Point(600, 350);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(181, 226);
            this.panel3.TabIndex = 5;
            // 
            // trackBar2
            // 
            this.trackBar2.AccessibleRole = System.Windows.Forms.AccessibleRole.Window;
            this.trackBar2.Location = new System.Drawing.Point(-1, 175);
            this.trackBar2.Maximum = 65535;
            this.trackBar2.Name = "trackBar2";
            this.trackBar2.Size = new System.Drawing.Size(171, 45);
            this.trackBar2.TabIndex = 26;
            this.trackBar2.Scroll += new System.EventHandler(this.trackBar2_Scroll);
            // 
            // textBox4
            // 
            this.textBox4.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.textBox4.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.textBox4.Location = new System.Drawing.Point(110, 146);
            this.textBox4.Name = "textBox4";
            this.textBox4.ReadOnly = true;
            this.textBox4.Size = new System.Drawing.Size(54, 23);
            this.textBox4.TabIndex = 25;
            // 
            // textBox3
            // 
            this.textBox3.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.textBox3.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.textBox3.Location = new System.Drawing.Point(110, 113);
            this.textBox3.Name = "textBox3";
            this.textBox3.ReadOnly = true;
            this.textBox3.Size = new System.Drawing.Size(54, 23);
            this.textBox3.TabIndex = 24;
            // 
            // radioButton4
            // 
            this.radioButton4.AutoSize = true;
            this.radioButton4.BackColor = System.Drawing.SystemColors.ButtonShadow;
            this.radioButton4.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.radioButton4.Location = new System.Drawing.Point(3, 148);
            this.radioButton4.Name = "radioButton4";
            this.radioButton4.Size = new System.Drawing.Size(106, 20);
            this.radioButton4.TabIndex = 23;
            this.radioButton4.TabStop = true;
            this.radioButton4.Text = "Y 终止坐标";
            this.radioButton4.UseVisualStyleBackColor = false;
            this.radioButton4.CheckedChanged += new System.EventHandler(this.radioButton4_CheckedChanged);
            // 
            // radioButton3
            // 
            this.radioButton3.AutoSize = true;
            this.radioButton3.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.radioButton3.Location = new System.Drawing.Point(3, 116);
            this.radioButton3.Name = "radioButton3";
            this.radioButton3.Size = new System.Drawing.Size(106, 20);
            this.radioButton3.TabIndex = 22;
            this.radioButton3.TabStop = true;
            this.radioButton3.Text = "Y 起始坐标";
            this.radioButton3.UseVisualStyleBackColor = true;
            this.radioButton3.CheckedChanged += new System.EventHandler(this.radioButton3_CheckedChanged);
            // 
            // textBox2
            // 
            this.textBox2.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.textBox2.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.textBox2.Location = new System.Drawing.Point(110, 49);
            this.textBox2.Name = "textBox2";
            this.textBox2.ReadOnly = true;
            this.textBox2.Size = new System.Drawing.Size(54, 23);
            this.textBox2.TabIndex = 21;
            // 
            // textBox1
            // 
            this.textBox1.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.textBox1.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.textBox1.Location = new System.Drawing.Point(110, 15);
            this.textBox1.Name = "textBox1";
            this.textBox1.ReadOnly = true;
            this.textBox1.Size = new System.Drawing.Size(54, 23);
            this.textBox1.TabIndex = 20;
            // 
            // trackBar1
            // 
            this.trackBar1.AccessibleRole = System.Windows.Forms.AccessibleRole.Window;
            this.trackBar1.Location = new System.Drawing.Point(-1, 78);
            this.trackBar1.Maximum = 512;
            this.trackBar1.Name = "trackBar1";
            this.trackBar1.Size = new System.Drawing.Size(171, 45);
            this.trackBar1.TabIndex = 16;
            this.trackBar1.Scroll += new System.EventHandler(this.trackBar1_Scroll);
            // 
            // radioButton2
            // 
            this.radioButton2.AutoSize = true;
            this.radioButton2.BackColor = System.Drawing.SystemColors.ButtonShadow;
            this.radioButton2.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.radioButton2.Location = new System.Drawing.Point(4, 50);
            this.radioButton2.Name = "radioButton2";
            this.radioButton2.Size = new System.Drawing.Size(106, 20);
            this.radioButton2.TabIndex = 15;
            this.radioButton2.TabStop = true;
            this.radioButton2.Text = "X 终止坐标";
            this.radioButton2.UseVisualStyleBackColor = false;
            this.radioButton2.CheckedChanged += new System.EventHandler(this.radioButton2_CheckedChanged);
            // 
            // radioButton1
            // 
            this.radioButton1.AutoSize = true;
            this.radioButton1.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.radioButton1.Location = new System.Drawing.Point(4, 16);
            this.radioButton1.Name = "radioButton1";
            this.radioButton1.Size = new System.Drawing.Size(106, 20);
            this.radioButton1.TabIndex = 14;
            this.radioButton1.TabStop = true;
            this.radioButton1.Text = "X 起始坐标";
            this.radioButton1.UseVisualStyleBackColor = true;
            this.radioButton1.CheckedChanged += new System.EventHandler(this.radioButton1_CheckedChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label4.Location = new System.Drawing.Point(668, 344);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(35, 14);
            this.label4.TabIndex = 6;
            this.label4.Text = "设置";
            // 
            // panel4
            // 
            this.panel4.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel4.Controls.Add(this.comboBoxPortel);
            this.panel4.Controls.Add(this.comboBoxCom);
            this.panel4.Controls.Add(this.SerialSetButton);
            this.panel4.Controls.Add(this.DataStoreButton);
            this.panel4.Controls.Add(this.label7);
            this.panel4.Controls.Add(this.label6);
            this.panel4.Location = new System.Drawing.Point(600, 162);
            this.panel4.Name = "panel4";
            this.panel4.Size = new System.Drawing.Size(181, 169);
            this.panel4.TabIndex = 7;
            // 
            // comboBoxPortel
            // 
            this.comboBoxPortel.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.comboBoxPortel.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.comboBoxPortel.FormattingEnabled = true;
            this.comboBoxPortel.Items.AddRange(new object[] {
            "9600",
            "115200"});
            this.comboBoxPortel.Location = new System.Drawing.Point(80, 73);
            this.comboBoxPortel.Name = "comboBoxPortel";
            this.comboBoxPortel.Size = new System.Drawing.Size(86, 24);
            this.comboBoxPortel.TabIndex = 17;
            // 
            // comboBoxCom
            // 
            this.comboBoxCom.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.comboBoxCom.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.comboBoxCom.FormattingEnabled = true;
            this.comboBoxCom.Location = new System.Drawing.Point(80, 28);
            this.comboBoxCom.Name = "comboBoxCom";
            this.comboBoxCom.Size = new System.Drawing.Size(86, 24);
            this.comboBoxCom.TabIndex = 16;
            this.comboBoxCom.MouseClick += new System.Windows.Forms.MouseEventHandler(this.comboBoxCom_MouseClick);
            // 
            // SerialSetButton
            // 
            this.SerialSetButton.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.SerialSetButton.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.SerialSetButton.Location = new System.Drawing.Point(13, 111);
            this.SerialSetButton.Name = "SerialSetButton";
            this.SerialSetButton.Size = new System.Drawing.Size(62, 37);
            this.SerialSetButton.TabIndex = 13;
            this.SerialSetButton.Text = "初始化串口";
            this.SerialSetButton.UseVisualStyleBackColor = false;
            this.SerialSetButton.Click += new System.EventHandler(this.SerialSetButton_Click);
            // 
            // DataStoreButton
            // 
            this.DataStoreButton.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.DataStoreButton.Enabled = false;
            this.DataStoreButton.Location = new System.Drawing.Point(100, 111);
            this.DataStoreButton.Name = "DataStoreButton";
            this.DataStoreButton.Size = new System.Drawing.Size(64, 37);
            this.DataStoreButton.TabIndex = 9;
            this.DataStoreButton.Text = "数据保存";
            this.DataStoreButton.UseVisualStyleBackColor = false;
            this.DataStoreButton.Click += new System.EventHandler(this.button1_Click_1);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label7.Location = new System.Drawing.Point(19, 31);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(56, 16);
            this.label7.TabIndex = 14;
            this.label7.Text = "串口号";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label6.Location = new System.Drawing.Point(19, 76);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(56, 16);
            this.label6.TabIndex = 15;
            this.label6.Text = "波特率";
            // 
            // SerialPort
            // 
            this.SerialPort.BaudRate = 115200;
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
            // chart1
            // 
            this.chart1.AlwaysRecreateHotregions = true;
            this.chart1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(64)))));
            this.chart1.BackGradientEndColor = System.Drawing.Color.Lavender;
            this.chart1.BorderLineColor = System.Drawing.Color.LightSlateGray;
            this.chart1.BorderLineStyle = Dundas.Charting.WinControl.ChartDashStyle.Solid;
            this.chart1.BorderSkin.FrameBackColor = System.Drawing.Color.SteelBlue;
            this.chart1.BorderSkin.FrameBackGradientEndColor = System.Drawing.Color.LightBlue;
            this.chart1.BorderSkin.FrameBorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(100)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))));
            this.chart1.BorderSkin.FrameBorderWidth = 2;
            this.chart1.BorderSkin.PageColor = System.Drawing.Color.Transparent;
            this.chart1.BorderSkin.SkinStyle = Dundas.Charting.WinControl.BorderSkinStyle.Raised;
            chartArea1.Area3DStyle.WallWidth = 0;
            chartArea1.AxisX.MajorGrid.LineColor = System.Drawing.Color.FromArgb(((int)(((byte)(65)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))));
            chartArea1.AxisX.MinorGrid.LineColor = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))));
            chartArea1.AxisX.MinorTickMark.Size = 2F;
            chartArea1.AxisY.LabelStyle.Format = "G4";
            chartArea1.AxisY.MajorGrid.LineColor = System.Drawing.Color.FromArgb(((int)(((byte)(65)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))));
            chartArea1.AxisY.MinorGrid.LineColor = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))));
            chartArea1.AxisY.MinorTickMark.Size = 2F;
            chartArea1.BackColor = System.Drawing.Color.White;
            chartArea1.BackGradientEndColor = System.Drawing.Color.White;
            chartArea1.BorderColor = System.Drawing.Color.LightSlateGray;
            chartArea1.BorderStyle = Dundas.Charting.WinControl.ChartDashStyle.Solid;
            chartArea1.Name = "Default";
            this.chart1.ChartAreas.Add(chartArea1);
            legend1.Alignment = System.Drawing.StringAlignment.Center;
            legend1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(150)))), ((int)(((byte)(255)))), ((int)(((byte)(255)))), ((int)(((byte)(255)))));
            legend1.BorderColor = System.Drawing.Color.LightSlateGray;
            legend1.Docking = Dundas.Charting.WinControl.LegendDocking.Bottom;
            legend1.Name = "Default";
            this.chart1.Legends.Add(legend1);
            this.chart1.Location = new System.Drawing.Point(0, 6);
            this.chart1.Name = "chart1";
            this.chart1.Palette = Dundas.Charting.WinControl.ChartColorPalette.Kindergarten;
            this.chart1.Size = new System.Drawing.Size(594, 551);
            this.chart1.TabIndex = 4;
            this.chart1.Text = "chart1";
            title1.Name = "Title1";
            this.chart1.Titles.Add(title1);
            this.chart1.Click += new System.EventHandler(this.chart1_Click);
            this.chart1.MouseClick += new System.Windows.Forms.MouseEventHandler(this.chart1_MouseClick);
            this.chart1.MouseWheel += new System.Windows.Forms.MouseEventHandler(this.chart1_MouseWheel);
            // 
            // MonitorText
            // 
            this.MonitorText.Location = new System.Drawing.Point(787, 12);
            this.MonitorText.Name = "MonitorText";
            this.MonitorText.Size = new System.Drawing.Size(271, 319);
            this.MonitorText.TabIndex = 8;
            this.MonitorText.Text = "";
            this.MonitorText.TextChanged += new System.EventHandler(this.richTextBox3_TextChanged_1);
            // 
            // ScanButton
            // 
            this.ScanButton.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.ScanButton.Enabled = false;
            this.ScanButton.Location = new System.Drawing.Point(965, 366);
            this.ScanButton.Name = "ScanButton";
            this.ScanButton.Size = new System.Drawing.Size(93, 23);
            this.ScanButton.TabIndex = 10;
            this.ScanButton.Text = "搜索设备";
            this.ScanButton.UseVisualStyleBackColor = false;
            this.ScanButton.Click += new System.EventHandler(this.ScanButton_Click);
            // 
            // MACComboBox
            // 
            this.MACComboBox.Enabled = false;
            this.MACComboBox.FormattingEnabled = true;
            this.MACComboBox.Items.AddRange(new object[] {
            "无设备"});
            this.MACComboBox.Location = new System.Drawing.Point(792, 370);
            this.MACComboBox.Name = "MACComboBox";
            this.MACComboBox.Size = new System.Drawing.Size(167, 20);
            this.MACComboBox.TabIndex = 11;
            this.MACComboBox.SelectedIndexChanged += new System.EventHandler(this.MACComboBox_SelectedIndexChanged);
            // 
            // ConnectBLEButton
            // 
            this.ConnectBLEButton.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.ConnectBLEButton.Enabled = false;
            this.ConnectBLEButton.Location = new System.Drawing.Point(965, 400);
            this.ConnectBLEButton.Name = "ConnectBLEButton";
            this.ConnectBLEButton.Size = new System.Drawing.Size(93, 23);
            this.ConnectBLEButton.TabIndex = 18;
            this.ConnectBLEButton.Text = "设备已断开";
            this.ConnectBLEButton.UseVisualStyleBackColor = false;
            this.ConnectBLEButton.Click += new System.EventHandler(this.ConnectBLEButton_Click);
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(792, 344);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(125, 12);
            this.label11.TabIndex = 21;
            this.label11.Text = "搜索到的蓝牙设备MAC:";
            // 
            // BLENUMlabel
            // 
            this.BLENUMlabel.AutoSize = true;
            this.BLENUMlabel.ForeColor = System.Drawing.Color.Blue;
            this.BLENUMlabel.Location = new System.Drawing.Point(932, 344);
            this.BLENUMlabel.Name = "BLENUMlabel";
            this.BLENUMlabel.Size = new System.Drawing.Size(0, 12);
            this.BLENUMlabel.TabIndex = 22;
            // 
            // PauseButton
            // 
            this.PauseButton.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.PauseButton.Enabled = false;
            this.PauseButton.Location = new System.Drawing.Point(965, 433);
            this.PauseButton.Name = "PauseButton";
            this.PauseButton.Size = new System.Drawing.Size(93, 23);
            this.PauseButton.TabIndex = 24;
            this.PauseButton.Text = "暂停";
            this.PauseButton.UseVisualStyleBackColor = false;
            this.PauseButton.Click += new System.EventHandler(this.PauseButton_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label2.Location = new System.Drawing.Point(678, 156);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(35, 14);
            this.label2.TabIndex = 3;
            this.label2.Text = "设置";
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(965, 468);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(93, 23);
            this.button1.TabIndex = 25;
            this.button1.Text = "单设备测试";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click_2);
            // 
            // textBoxScan
            // 
            this.textBoxScan.Location = new System.Drawing.Point(792, 464);
            this.textBoxScan.MaxLength = 15;
            this.textBoxScan.Name = "textBoxScan";
            this.textBoxScan.Size = new System.Drawing.Size(167, 21);
            this.textBoxScan.TabIndex = 26;
            this.textBoxScan.Text = "123456789ABCDEF";
            this.textBoxScan.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.textBoxScan_KeyPress);
            // 
            // IDValuebutton
            // 
            this.IDValuebutton.Location = new System.Drawing.Point(829, 496);
            this.IDValuebutton.Name = "IDValuebutton";
            this.IDValuebutton.Size = new System.Drawing.Size(75, 23);
            this.IDValuebutton.TabIndex = 29;
            this.IDValuebutton.Text = "ID值设置";
            this.IDValuebutton.UseVisualStyleBackColor = true;
            this.IDValuebutton.Click += new System.EventHandler(this.IDValuebutton_Click);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(965, 497);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(93, 23);
            this.button2.TabIndex = 30;
            this.button2.Text = "程序下载";
            this.button2.UseVisualStyleBackColor = true;
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(804, 444);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(137, 12);
            this.label10.TabIndex = 31;
            this.label10.Text = "请输入ID值并回车或扫描";
            // 
            // timer1
            // 
            this.timer1.Enabled = true;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // button3
            // 
            this.button3.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.button3.Location = new System.Drawing.Point(873, 401);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(68, 23);
            this.button3.TabIndex = 32;
            this.button3.Text = "蓝牙断开";
            this.button3.UseVisualStyleBackColor = false;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // button4
            // 
            this.button4.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.button4.Location = new System.Drawing.Point(794, 401);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(70, 23);
            this.button4.TabIndex = 33;
            this.button4.Text = "蓝牙连接";
            this.button4.UseVisualStyleBackColor = false;
            this.button4.Click += new System.EventHandler(this.button4_Click_1);
            // 
            // RF
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ButtonShadow;
            this.ClientSize = new System.Drawing.Size(1063, 615);
            this.Controls.Add(this.button4);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.IDValuebutton);
            this.Controls.Add(this.textBoxScan);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.PauseButton);
            this.Controls.Add(this.BLENUMlabel);
            this.Controls.Add(this.label11);
            this.Controls.Add(this.ConnectBLEButton);
            this.Controls.Add(this.MACComboBox);
            this.Controls.Add(this.ScanButton);
            this.Controls.Add(this.MonitorText);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.panel3);
            this.Controls.Add(this.chart1);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.Lea);
            this.Controls.Add(this.panel4);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "RF";
            this.Text = "HM502B2心电数据显示软件V1.2(20150426)";
            this.Load += new System.EventHandler(this.Motion_Load);
            this.Lea.ResumeLayout(false);
            this.Lea.PerformLayout();
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.panel3.ResumeLayout(false);
            this.panel3.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).EndInit();
            this.panel4.ResumeLayout(false);
            this.panel4.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.Timer)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel Lea;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.Panel panel3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.RadioButton radioButton2;
        private System.Windows.Forms.RadioButton radioButton1;
        private System.Windows.Forms.TrackBar trackBar1;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.Panel panel4;
        private System.Windows.Forms.ComboBox comboBoxPortel;
        private System.Windows.Forms.ComboBox comboBoxCom;
        private System.Windows.Forms.Button SerialSetButton;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label6;
        private System.IO.Ports.SerialPort SerialPort;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
        private System.Timers.Timer Timer;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
        private Sunisoft.IrisSkin.SkinEngine skinEngine1;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox HeartRate;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label3;
        private Dundas.Charting.WinControl.Chart chart1;
        private System.Windows.Forms.TextBox BloodPressure;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.ComboBox MACComboBox;
        private System.Windows.Forms.Button ScanButton;
        private System.Windows.Forms.Button DataStoreButton;
        private System.Windows.Forms.RichTextBox MonitorText;
        private System.Windows.Forms.Button ConnectBLEButton;
        private System.Windows.Forms.TextBox BatteryValue;
        private System.Windows.Forms.RichTextBox Lead;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label BLENUMlabel;
        private System.Windows.Forms.Button PauseButton;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textBox4;
        private System.Windows.Forms.TextBox textBox3;
        private System.Windows.Forms.RadioButton radioButton4;
        private System.Windows.Forms.RadioButton radioButton3;
        private System.Windows.Forms.TrackBar trackBar2;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button IDValuebutton;
        private System.Windows.Forms.TextBox textBoxScan;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel2;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.Button button3;
    }
}

