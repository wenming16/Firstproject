namespace BMSTool.Comm
{
    partial class CanSet
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.cmb_DeviceIndex = new System.Windows.Forms.ComboBox();
            this.label3 = new System.Windows.Forms.Label();
            this.cmb_CommCycle = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.cmb_CanChannel = new System.Windows.Forms.ComboBox();
            this.label6 = new System.Windows.Forms.Label();
            this.cmb_CanType = new System.Windows.Forms.ComboBox();
            this.label5 = new System.Windows.Forms.Label();
            this.btn_CanClose = new System.Windows.Forms.Button();
            this.btn_CanOpen = new System.Windows.Forms.Button();
            this.cmb_BaudRate = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.groupBox1.SuspendLayout();
            this.statusStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.cmb_DeviceIndex);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.cmb_CommCycle);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.cmb_CanChannel);
            this.groupBox1.Controls.Add(this.label6);
            this.groupBox1.Controls.Add(this.cmb_CanType);
            this.groupBox1.Controls.Add(this.label5);
            this.groupBox1.Controls.Add(this.btn_CanClose);
            this.groupBox1.Controls.Add(this.btn_CanOpen);
            this.groupBox1.Controls.Add(this.cmb_BaudRate);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Font = new System.Drawing.Font("微软雅黑", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(263, 271);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "设备参数";
            // 
            // cmb_DeviceIndex
            // 
            this.cmb_DeviceIndex.Font = new System.Drawing.Font("微软雅黑", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.cmb_DeviceIndex.FormattingEnabled = true;
            this.cmb_DeviceIndex.Items.AddRange(new object[] {
            "0",
            "1"});
            this.cmb_DeviceIndex.Location = new System.Drawing.Point(115, 65);
            this.cmb_DeviceIndex.Name = "cmb_DeviceIndex";
            this.cmb_DeviceIndex.Size = new System.Drawing.Size(121, 28);
            this.cmb_DeviceIndex.TabIndex = 17;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("微软雅黑", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label3.Location = new System.Drawing.Point(19, 68);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(79, 20);
            this.label3.TabIndex = 16;
            this.label3.Text = "设备索引：";
            // 
            // cmb_CommCycle
            // 
            this.cmb_CommCycle.Font = new System.Drawing.Font("微软雅黑", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.cmb_CommCycle.FormattingEnabled = true;
            this.cmb_CommCycle.Items.AddRange(new object[] {
            "500ms",
            "1000ms",
            "1500ms"});
            this.cmb_CommCycle.Location = new System.Drawing.Point(115, 182);
            this.cmb_CommCycle.Name = "cmb_CommCycle";
            this.cmb_CommCycle.Size = new System.Drawing.Size(121, 28);
            this.cmb_CommCycle.TabIndex = 15;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(19, 185);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(79, 20);
            this.label2.TabIndex = 14;
            this.label2.Text = "通信周期：";
            // 
            // cmb_CanChannel
            // 
            this.cmb_CanChannel.Font = new System.Drawing.Font("微软雅黑", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.cmb_CanChannel.FormattingEnabled = true;
            this.cmb_CanChannel.Items.AddRange(new object[] {
            "CAN1",
            "CAN2"});
            this.cmb_CanChannel.Location = new System.Drawing.Point(115, 104);
            this.cmb_CanChannel.Name = "cmb_CanChannel";
            this.cmb_CanChannel.Size = new System.Drawing.Size(121, 28);
            this.cmb_CanChannel.TabIndex = 13;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("微软雅黑", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label6.Location = new System.Drawing.Point(19, 107);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(81, 20);
            this.label6.TabIndex = 12;
            this.label6.Text = "CAN通道：";
            // 
            // cmb_CanType
            // 
            this.cmb_CanType.Font = new System.Drawing.Font("微软雅黑", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.cmb_CanType.FormattingEnabled = true;
            this.cmb_CanType.Items.AddRange(new object[] {
            "USBCAN-II",
            "USBCAN-E-mini"});
            this.cmb_CanType.Location = new System.Drawing.Point(115, 26);
            this.cmb_CanType.Name = "cmb_CanType";
            this.cmb_CanType.Size = new System.Drawing.Size(121, 28);
            this.cmb_CanType.TabIndex = 11;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("微软雅黑", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label5.Location = new System.Drawing.Point(19, 29);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(79, 20);
            this.label5.TabIndex = 10;
            this.label5.Text = "设备类型：";
            // 
            // btn_CanClose
            // 
            this.btn_CanClose.Location = new System.Drawing.Point(135, 228);
            this.btn_CanClose.Name = "btn_CanClose";
            this.btn_CanClose.Size = new System.Drawing.Size(75, 31);
            this.btn_CanClose.TabIndex = 7;
            this.btn_CanClose.Text = "关闭";
            this.btn_CanClose.UseVisualStyleBackColor = true;
            this.btn_CanClose.Click += new System.EventHandler(this.btn_CanClose_Click);
            // 
            // btn_CanOpen
            // 
            this.btn_CanOpen.Location = new System.Drawing.Point(32, 228);
            this.btn_CanOpen.Name = "btn_CanOpen";
            this.btn_CanOpen.Size = new System.Drawing.Size(75, 31);
            this.btn_CanOpen.TabIndex = 6;
            this.btn_CanOpen.Text = "打开";
            this.btn_CanOpen.UseVisualStyleBackColor = true;
            this.btn_CanOpen.Click += new System.EventHandler(this.btn_OpenCan_Click);
            // 
            // cmb_BaudRate
            // 
            this.cmb_BaudRate.Font = new System.Drawing.Font("微软雅黑", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.cmb_BaudRate.FormattingEnabled = true;
            this.cmb_BaudRate.Items.AddRange(new object[] {
            "125kbps",
            "250kbps",
            "500kbps"});
            this.cmb_BaudRate.Location = new System.Drawing.Point(115, 144);
            this.cmb_BaudRate.Name = "cmb_BaudRate";
            this.cmb_BaudRate.Size = new System.Drawing.Size(121, 28);
            this.cmb_BaudRate.TabIndex = 1;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(19, 147);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(65, 20);
            this.label1.TabIndex = 0;
            this.label1.Text = "波特率：";
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel1});
            this.statusStrip1.Location = new System.Drawing.Point(0, 326);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(287, 22);
            this.statusStrip1.TabIndex = 1;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // toolStripStatusLabel1
            // 
            this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
            this.toolStripStatusLabel1.Size = new System.Drawing.Size(34, 17);
            this.toolStripStatusLabel1.Text = "CAN";
            // 
            // timer1
            // 
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // CanSet
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(287, 348);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.groupBox1);
            this.MaximizeBox = false;
            this.Name = "CanSet";
            this.Text = "CAN参数设置";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.ComboBox cmb_BaudRate;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btn_CanClose;
        private System.Windows.Forms.Button btn_CanOpen;
        private System.Windows.Forms.ComboBox cmb_CanType;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.ComboBox cmb_CanChannel;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label2;
        public System.Windows.Forms.ComboBox cmb_CommCycle;
        private System.Windows.Forms.ComboBox cmb_DeviceIndex;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
        private System.Windows.Forms.Timer timer1;
    }
}