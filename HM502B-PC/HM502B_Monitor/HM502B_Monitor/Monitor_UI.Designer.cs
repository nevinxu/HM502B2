namespace HM502B_Monitor
{
    partial class Monitor_UI
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
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend1 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series1 = new System.Windows.Forms.DataVisualization.Charting.Series();
            this.EcgDataChart = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.timer = new System.Timers.Timer();
            ((System.ComponentModel.ISupportInitialize)(this.EcgDataChart)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.timer)).BeginInit();
            this.SuspendLayout();
            // 
            // EcgDataChart
            // 
            chartArea1.Name = "ChartECGData";
            this.EcgDataChart.ChartAreas.Add(chartArea1);
            legend1.Name = "Legend1";
            this.EcgDataChart.Legends.Add(legend1);
            this.EcgDataChart.Location = new System.Drawing.Point(24, 3);
            this.EcgDataChart.Name = "EcgDataChart";
            series1.ChartArea = "ChartECGData";
            series1.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Spline;
            series1.Legend = "Legend1";
            series1.Name = "EcgData";
            series1.XValueType = System.Windows.Forms.DataVisualization.Charting.ChartValueType.Int32;
            series1.YValueType = System.Windows.Forms.DataVisualization.Charting.ChartValueType.Int32;
            this.EcgDataChart.Series.Add(series1);
            this.EcgDataChart.Size = new System.Drawing.Size(712, 332);
            this.EcgDataChart.TabIndex = 0;
            this.EcgDataChart.Text = "EcgDataChart";
            // 
            // timer
            // 
            this.timer.Enabled = true;
            this.timer.SynchronizingObject = this;
            this.timer.Elapsed += new System.Timers.ElapsedEventHandler(this.timer_Elapsed);
            // 
            // Monitor_UI
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(890, 359);
            this.Controls.Add(this.EcgDataChart);
            this.Name = "Monitor_UI";
            this.Text = "Form1";
            ((System.ComponentModel.ISupportInitialize)(this.EcgDataChart)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.timer)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.DataVisualization.Charting.Chart EcgDataChart;
        private System.Timers.Timer timer;

    }
}

