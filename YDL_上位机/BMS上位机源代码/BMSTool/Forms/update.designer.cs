namespace BMSTool.Forms
{
    partial class update
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
        {/*
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
          * */
            Hide();
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(update));
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.Rtb_Comm = new System.Windows.Forms.RichTextBox();
            this.Updata = new System.Windows.Forms.Panel();
            this.label5 = new System.Windows.Forms.Label();
            this.cmb_updateObj = new System.Windows.Forms.ComboBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.rbtn_NoApp = new System.Windows.Forms.RadioButton();
            this.rbtn_HaveApp = new System.Windows.Forms.RadioButton();
            this.label4 = new System.Windows.Forms.Label();
            this.txb_NewVersion = new System.Windows.Forms.TextBox();
            this.btn_Start = new System.Windows.Forms.Button();
            this.pgb_Updata = new System.Windows.Forms.ProgressBar();
            this.ptb_Updata = new System.Windows.Forms.PictureBox();
            this.txb_UpdataFile = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.txb_OldVersion = new System.Windows.Forms.TextBox();
            this.btn_ReadVer = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.Updata.SuspendLayout();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.ptb_Updata)).BeginInit();
            this.SuspendLayout();
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            // 
            // Rtb_Comm
            // 
            this.Rtb_Comm.Location = new System.Drawing.Point(555, 23);
            this.Rtb_Comm.Name = "Rtb_Comm";
            this.Rtb_Comm.Size = new System.Drawing.Size(218, 261);
            this.Rtb_Comm.TabIndex = 1;
            this.Rtb_Comm.Text = "";
            // 
            // Updata
            // 
            this.Updata.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.Updata.Controls.Add(this.label5);
            this.Updata.Controls.Add(this.cmb_updateObj);
            this.Updata.Controls.Add(this.groupBox1);
            this.Updata.Controls.Add(this.label4);
            this.Updata.Controls.Add(this.txb_NewVersion);
            this.Updata.Controls.Add(this.btn_Start);
            this.Updata.Controls.Add(this.pgb_Updata);
            this.Updata.Controls.Add(this.ptb_Updata);
            this.Updata.Controls.Add(this.txb_UpdataFile);
            this.Updata.Controls.Add(this.label3);
            this.Updata.Controls.Add(this.label1);
            this.Updata.Controls.Add(this.txb_OldVersion);
            this.Updata.Controls.Add(this.btn_ReadVer);
            this.Updata.Location = new System.Drawing.Point(10, 2);
            this.Updata.Name = "Updata";
            this.Updata.Size = new System.Drawing.Size(539, 282);
            this.Updata.TabIndex = 20;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(274, 54);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(96, 28);
            this.label5.TabIndex = 35;
            this.label5.Text = "升级对象";
            // 
            // cmb_updateObj
            // 
            this.cmb_updateObj.FormattingEnabled = true;
            this.cmb_updateObj.Items.AddRange(new object[] {
            "主板",
            "子板1",
            "子板2",
            "子板3",
            "子板4",
            "子板5",
            "子板6",
            "子板7",
            "子板8",
            "子板9",
            "子板10"});
            this.cmb_updateObj.Location = new System.Drawing.Point(359, 50);
            this.cmb_updateObj.Margin = new System.Windows.Forms.Padding(4, 3, 4, 3);
            this.cmb_updateObj.Name = "cmb_updateObj";
            this.cmb_updateObj.Size = new System.Drawing.Size(116, 36);
            this.cmb_updateObj.TabIndex = 34;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.rbtn_NoApp);
            this.groupBox1.Controls.Add(this.rbtn_HaveApp);
            this.groupBox1.Location = new System.Drawing.Point(53, 34);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(191, 50);
            this.groupBox1.TabIndex = 33;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "有无应用程序";
            // 
            // rbtn_NoApp
            // 
            this.rbtn_NoApp.AutoSize = true;
            this.rbtn_NoApp.Location = new System.Drawing.Point(99, 20);
            this.rbtn_NoApp.Name = "rbtn_NoApp";
            this.rbtn_NoApp.Size = new System.Drawing.Size(58, 32);
            this.rbtn_NoApp.TabIndex = 1;
            this.rbtn_NoApp.Text = "无";
            this.rbtn_NoApp.UseVisualStyleBackColor = true;
            // 
            // rbtn_HaveApp
            // 
            this.rbtn_HaveApp.AutoSize = true;
            this.rbtn_HaveApp.Checked = true;
            this.rbtn_HaveApp.ForeColor = System.Drawing.SystemColors.ControlText;
            this.rbtn_HaveApp.Location = new System.Drawing.Point(29, 20);
            this.rbtn_HaveApp.Name = "rbtn_HaveApp";
            this.rbtn_HaveApp.Size = new System.Drawing.Size(58, 32);
            this.rbtn_HaveApp.TabIndex = 0;
            this.rbtn_HaveApp.TabStop = true;
            this.rbtn_HaveApp.Text = "有";
            this.rbtn_HaveApp.UseVisualStyleBackColor = true;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(49, 248);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(101, 28);
            this.label4.TabIndex = 32;
            this.label4.Text = "新版本号:";
            this.label4.Visible = false;
            // 
            // txb_NewVersion
            // 
            this.txb_NewVersion.BackColor = System.Drawing.Color.White;
            this.txb_NewVersion.Location = new System.Drawing.Point(123, 243);
            this.txb_NewVersion.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.txb_NewVersion.Name = "txb_NewVersion";
            this.txb_NewVersion.ReadOnly = true;
            this.txb_NewVersion.Size = new System.Drawing.Size(230, 35);
            this.txb_NewVersion.TabIndex = 31;
            this.txb_NewVersion.Visible = false;
            // 
            // btn_Start
            // 
            this.btn_Start.BackColor = System.Drawing.Color.DodgerBlue;
            this.btn_Start.Location = new System.Drawing.Point(410, 146);
            this.btn_Start.Name = "btn_Start";
            this.btn_Start.Size = new System.Drawing.Size(100, 30);
            this.btn_Start.TabIndex = 29;
            this.btn_Start.Text = "启动升级";
            this.btn_Start.UseVisualStyleBackColor = false;
            this.btn_Start.Click += new System.EventHandler(this.btn_Start_Click);
            // 
            // pgb_Updata
            // 
            this.pgb_Updata.BackColor = System.Drawing.Color.White;
            this.pgb_Updata.Location = new System.Drawing.Point(25, 153);
            this.pgb_Updata.Name = "pgb_Updata";
            this.pgb_Updata.Size = new System.Drawing.Size(349, 23);
            this.pgb_Updata.TabIndex = 28;
            // 
            // ptb_Updata
            // 
            this.ptb_Updata.Image = ((System.Drawing.Image)(resources.GetObject("ptb_Updata.Image")));
            this.ptb_Updata.Location = new System.Drawing.Point(359, 99);
            this.ptb_Updata.Name = "ptb_Updata";
            this.ptb_Updata.Size = new System.Drawing.Size(45, 36);
            this.ptb_Updata.TabIndex = 27;
            this.ptb_Updata.TabStop = false;
            this.ptb_Updata.Click += new System.EventHandler(this.ptb_Updata_Click);
            // 
            // txb_UpdataFile
            // 
            this.txb_UpdataFile.BackColor = System.Drawing.Color.White;
            this.txb_UpdataFile.Location = new System.Drawing.Point(123, 106);
            this.txb_UpdataFile.Name = "txb_UpdataFile";
            this.txb_UpdataFile.ReadOnly = true;
            this.txb_UpdataFile.Size = new System.Drawing.Size(230, 35);
            this.txb_UpdataFile.TabIndex = 26;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(51, 109);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(117, 28);
            this.label3.TabIndex = 25;
            this.label3.Text = "升级文件：";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(49, 203);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(101, 28);
            this.label1.TabIndex = 22;
            this.label1.Text = "旧版本号:";
            this.label1.Visible = false;
            // 
            // txb_OldVersion
            // 
            this.txb_OldVersion.BackColor = System.Drawing.Color.White;
            this.txb_OldVersion.Location = new System.Drawing.Point(123, 201);
            this.txb_OldVersion.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.txb_OldVersion.Name = "txb_OldVersion";
            this.txb_OldVersion.ReadOnly = true;
            this.txb_OldVersion.Size = new System.Drawing.Size(230, 35);
            this.txb_OldVersion.TabIndex = 21;
            this.txb_OldVersion.Visible = false;
            // 
            // btn_ReadVer
            // 
            this.btn_ReadVer.BackColor = System.Drawing.Color.DodgerBlue;
            this.btn_ReadVer.Location = new System.Drawing.Point(410, 225);
            this.btn_ReadVer.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.btn_ReadVer.Name = "btn_ReadVer";
            this.btn_ReadVer.Size = new System.Drawing.Size(100, 32);
            this.btn_ReadVer.TabIndex = 20;
            this.btn_ReadVer.Text = "读取版本号";
            this.btn_ReadVer.UseVisualStyleBackColor = false;
            this.btn_ReadVer.Visible = false;
            this.btn_ReadVer.Click += new System.EventHandler(this.btn_ReadVer_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(555, 2);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(117, 28);
            this.label2.TabIndex = 21;
            this.label2.Text = "通信报文：";
            // 
            // update
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(13F, 28F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.White;
            this.ClientSize = new System.Drawing.Size(787, 296);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.Updata);
            this.Controls.Add(this.Rtb_Comm);
            this.Font = new System.Drawing.Font("微软雅黑", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.Name = "update";
            this.Text = "固件升级";
            this.Updata.ResumeLayout(false);
            this.Updata.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.ptb_Updata)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.RichTextBox Rtb_Comm;
        private System.Windows.Forms.Panel Updata;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox txb_NewVersion;
        private System.Windows.Forms.Button btn_Start;
        private System.Windows.Forms.ProgressBar pgb_Updata;
        private System.Windows.Forms.PictureBox ptb_Updata;
        private System.Windows.Forms.TextBox txb_UpdataFile;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox txb_OldVersion;
        private System.Windows.Forms.Button btn_ReadVer;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.RadioButton rbtn_NoApp;
        private System.Windows.Forms.RadioButton rbtn_HaveApp;
        private System.Windows.Forms.Label label5;
        public System.Windows.Forms.ComboBox cmb_updateObj;
    }
}