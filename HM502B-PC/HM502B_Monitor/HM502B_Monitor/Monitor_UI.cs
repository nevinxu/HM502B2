using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace HM502B_Monitor
{
    public partial class Monitor_UI : Form
    {
        public Monitor_UI()
        {
            InitializeComponent();
        }
        #region 定时器时能
        public void WriteTheWin()
        {
            timer.Enabled = true;
        }
        #endregion
        #region 波形的初始化
        private void Motion_Load(object sender, EventArgs e)
        {
            //GetComList();    //获取当前所有的串口

            //comboBoxCom.SelectedIndex = 0;
            //comboBoxPortel.SelectedIndex = 1;
            //MACComboBox.SelectedIndex = 0;
            //radioButton1.Select();

            //chart1.Titles[0].Text = "原始心电输出信号";
            //chart1.Titles[0].Alignment = ContentAlignment.TopCenter;
            //chart1.Titles[0].Font = new Font("黑体", 10, FontStyle.Bold);
            //chart1.Titles[0].Color = Color.FromArgb(128, 72, 72);
            //chart1.ChartAreas.Clear();
            //chart1.ChartAreas.Add("DX");

            //chart1.ChartAreas["DX"].AxisX.Arrows = ArrowsType.Lines;
            //chart1.ChartAreas["DX"].AxisY.Title = "心电电压值";

            //chart1.ChartAreas["DX"].AxisY.Arrows = ArrowsType.Lines;

            //chart1.ChartAreas["DX"].AxisY.Maximum = chartYMax;
            //chart1.ChartAreas["DX"].AxisY.Minimum = chartYMin;
            //chart1.ChartAreas["DX"].AxisY.Interval = 50;

            //chart1.ChartAreas["DX"].AxisX.Title = "时间";

            //chart1.ChartAreas["DX"].AxisX.Maximum = chartlenMax;  //4s
            //chart1.ChartAreas["DX"].AxisX.Minimum = chartlenMin;
            //// chart1.ChartAreas["DX"].AxisX.Interval = 50;
            //chart1.ChartAreas["DX"].AxisX.Interval = (chart1.ChartAreas["DX"].AxisX.Maximum - chart1.ChartAreas["DX"].AxisX.Minimum) / 20;

            //Color c1 = Color.FromArgb(50, 50, 50);
            //chart1.ChartAreas["DX"].AxisX.MajorGrid.LineColor = c1;
            //chart1.ChartAreas["DX"].AxisY.MajorGrid.LineColor = c1;
            //chart1.ChartAreas["DX"].ShadowColor = Color.White;
            //chart1.ChartAreas["DX"].BackColor = Color.Black;

            //chart1.Series.Clear();
            //chart1.Series.Add("数据个数");
            //chart1.Legends[0].Enabled = false;
            //chart1.Series["数据个数"].Color = Color.Green;
            //chart1.Series["数据个数"].Type = SeriesChartType.Line;
            //chart1.Series["数据个数"].ChartArea = "DX";


            //for (int i = 0; i < (N * M); i++)
            //{
            //    Xdata[i] = i.ToString();
            //}
            //chart1.Series["数据个数"].Points.DataBindXY(Xdata, XdataV);

            //textBox1.Text = chartlenMin.ToString();
            //textBox2.Text = chartlenMax.ToString();

            //textBox3.Text = chartYMin.ToString();
            //textBox4.Text = chartYMax.ToString();
            EcgDataChart.Series["EcgData"].Points.AddXY("11",12);
            EcgDataChart.Series["EcgData"].Points.AddXY("12",13);
            //EcgDataChart.Series["EcgData"].Points.AddY(13);
            //EcgDataChart.Series["EcgData"].Points.AddY(14);
            //EcgDataChart.Series["EcgData"].Points.AddY(15);
            //EcgDataChart.Series["EcgData"].Points.AddY(1000);

        }
        #endregion
        private void timer_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
        {

        }
    }
}
