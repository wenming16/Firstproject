namespace BMSTool.Forms
{
    partial class SocSet
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
            /*
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
            this.txb_SOC_Read = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.btn_SOC_Read = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.btn_SOC_Volt = new System.Windows.Forms.Button();
            this.txb_SOC_Volt = new System.Windows.Forms.TextBox();
            this.panel2 = new System.Windows.Forms.Panel();
            this.label16 = new System.Windows.Forms.Label();
            this.label15 = new System.Windows.Forms.Label();
            this.panel3 = new System.Windows.Forms.Panel();
            this.label8 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.btn_SOC_Tsd_H = new System.Windows.Forms.Button();
            this.label7 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.txb_SOCTsdH1 = new System.Windows.Forms.TextBox();
            this.txb_SocTsdH2 = new System.Windows.Forms.TextBox();
            this.txb_SOCTsdH3 = new System.Windows.Forms.TextBox();
            this.txb_SOCL3 = new System.Windows.Forms.TextBox();
            this.txb_SOCL2 = new System.Windows.Forms.TextBox();
            this.label12 = new System.Windows.Forms.Label();
            this.txb_SOC_L1 = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.btn_SOC_TsdL = new System.Windows.Forms.Button();
            this.label4 = new System.Windows.Forms.Label();
            this.label13 = new System.Windows.Forms.Label();
            this.panel1 = new System.Windows.Forms.Panel();
            this.label14 = new System.Windows.Forms.Label();
            this.panel2.SuspendLayout();
            this.panel3.SuspendLayout();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // txb_SOC_Read
            // 
            this.txb_SOC_Read.Location = new System.Drawing.Point(108, 26);
            this.txb_SOC_Read.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.txb_SOC_Read.Name = "txb_SOC_Read";
            this.txb_SOC_Read.Size = new System.Drawing.Size(64, 28);
            this.txb_SOC_Read.TabIndex = 3;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(10, 30);
            this.label2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(89, 18);
            this.label2.TabIndex = 2;
            this.label2.Text = "SOC_Read:";
            // 
            // btn_SOC_Read
            // 
            this.btn_SOC_Read.BackColor = System.Drawing.Color.DodgerBlue;
            this.btn_SOC_Read.ForeColor = System.Drawing.Color.White;
            this.btn_SOC_Read.Location = new System.Drawing.Point(196, 15);
            this.btn_SOC_Read.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.btn_SOC_Read.Name = "btn_SOC_Read";
            this.btn_SOC_Read.Size = new System.Drawing.Size(112, 50);
            this.btn_SOC_Read.TabIndex = 14;
            this.btn_SOC_Read.Text = "标定";
            this.btn_SOC_Read.UseVisualStyleBackColor = false;
            this.btn_SOC_Read.Visible = false;
            this.btn_SOC_Read.Click += new System.EventHandler(this.btn_SOC_Read_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(10, 117);
            this.label1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(89, 18);
            this.label1.TabIndex = 17;
            this.label1.Text = "SOC_Volt:";
            // 
            // btn_SOC_Volt
            // 
            this.btn_SOC_Volt.BackColor = System.Drawing.Color.DodgerBlue;
            this.btn_SOC_Volt.ForeColor = System.Drawing.Color.White;
            this.btn_SOC_Volt.Location = new System.Drawing.Point(196, 98);
            this.btn_SOC_Volt.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.btn_SOC_Volt.Name = "btn_SOC_Volt";
            this.btn_SOC_Volt.Size = new System.Drawing.Size(112, 50);
            this.btn_SOC_Volt.TabIndex = 18;
            this.btn_SOC_Volt.Text = "标定";
            this.btn_SOC_Volt.UseVisualStyleBackColor = false;
            this.btn_SOC_Volt.Visible = false;
            this.btn_SOC_Volt.Click += new System.EventHandler(this.btn_SOC_Volt_Click);
            // 
            // txb_SOC_Volt
            // 
            this.txb_SOC_Volt.Location = new System.Drawing.Point(108, 112);
            this.txb_SOC_Volt.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.txb_SOC_Volt.Name = "txb_SOC_Volt";
            this.txb_SOC_Volt.Size = new System.Drawing.Size(64, 28);
            this.txb_SOC_Volt.TabIndex = 19;
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.label16);
            this.panel2.Controls.Add(this.label15);
            this.panel2.Controls.Add(this.label2);
            this.panel2.Controls.Add(this.btn_SOC_Read);
            this.panel2.Controls.Add(this.txb_SOC_Read);
            this.panel2.Controls.Add(this.txb_SOC_Volt);
            this.panel2.Controls.Add(this.label1);
            this.panel2.Controls.Add(this.btn_SOC_Volt);
            this.panel2.Location = new System.Drawing.Point(182, 39);
            this.panel2.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(314, 170);
            this.panel2.TabIndex = 40;
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Location = new System.Drawing.Point(177, 112);
            this.label16.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(17, 18);
            this.label16.TabIndex = 44;
            this.label16.Text = "%";
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(177, 30);
            this.label15.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(17, 18);
            this.label15.TabIndex = 43;
            this.label15.Text = "%";
            // 
            // panel3
            // 
            this.panel3.Controls.Add(this.label8);
            this.panel3.Controls.Add(this.label10);
            this.panel3.Controls.Add(this.label11);
            this.panel3.Controls.Add(this.label3);
            this.panel3.Controls.Add(this.btn_SOC_Tsd_H);
            this.panel3.Controls.Add(this.label7);
            this.panel3.Controls.Add(this.label9);
            this.panel3.Controls.Add(this.txb_SOCTsdH1);
            this.panel3.Controls.Add(this.txb_SocTsdH2);
            this.panel3.Controls.Add(this.txb_SOCTsdH3);
            this.panel3.Location = new System.Drawing.Point(182, 537);
            this.panel3.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(314, 242);
            this.panel3.TabIndex = 41;
            this.panel3.Visible = false;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(280, 122);
            this.label8.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(17, 18);
            this.label8.TabIndex = 42;
            this.label8.Text = "%";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(284, 74);
            this.label10.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(17, 18);
            this.label10.TabIndex = 41;
            this.label10.Text = "%";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(284, 30);
            this.label11.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(17, 18);
            this.label11.TabIndex = 40;
            this.label11.Text = "%";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(6, 30);
            this.label3.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(161, 18);
            this.label3.TabIndex = 29;
            this.label3.Text = "SOC过高一级阀值：";
            // 
            // btn_SOC_Tsd_H
            // 
            this.btn_SOC_Tsd_H.BackColor = System.Drawing.Color.DodgerBlue;
            this.btn_SOC_Tsd_H.Location = new System.Drawing.Point(86, 174);
            this.btn_SOC_Tsd_H.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.btn_SOC_Tsd_H.Name = "btn_SOC_Tsd_H";
            this.btn_SOC_Tsd_H.Size = new System.Drawing.Size(135, 57);
            this.btn_SOC_Tsd_H.TabIndex = 38;
            this.btn_SOC_Tsd_H.Text = "标定";
            this.btn_SOC_Tsd_H.UseVisualStyleBackColor = false;
            this.btn_SOC_Tsd_H.Click += new System.EventHandler(this.btn_SOC_Tsd_H_Click);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(6, 74);
            this.label7.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(161, 18);
            this.label7.TabIndex = 30;
            this.label7.Text = "SOC过高二级阀值：";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(3, 122);
            this.label9.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(161, 18);
            this.label9.TabIndex = 31;
            this.label9.Text = "SOC过高三级阀值：";
            // 
            // txb_SOCTsdH1
            // 
            this.txb_SOCTsdH1.Location = new System.Drawing.Point(176, 26);
            this.txb_SOCTsdH1.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.txb_SOCTsdH1.Name = "txb_SOCTsdH1";
            this.txb_SOCTsdH1.Size = new System.Drawing.Size(97, 28);
            this.txb_SOCTsdH1.TabIndex = 32;
            // 
            // txb_SocTsdH2
            // 
            this.txb_SocTsdH2.Location = new System.Drawing.Point(176, 69);
            this.txb_SocTsdH2.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.txb_SocTsdH2.Name = "txb_SocTsdH2";
            this.txb_SocTsdH2.Size = new System.Drawing.Size(97, 28);
            this.txb_SocTsdH2.TabIndex = 33;
            // 
            // txb_SOCTsdH3
            // 
            this.txb_SOCTsdH3.Location = new System.Drawing.Point(172, 117);
            this.txb_SOCTsdH3.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.txb_SOCTsdH3.Name = "txb_SOCTsdH3";
            this.txb_SOCTsdH3.Size = new System.Drawing.Size(97, 28);
            this.txb_SOCTsdH3.TabIndex = 34;
            // 
            // txb_SOCL3
            // 
            this.txb_SOCL3.Location = new System.Drawing.Point(172, 117);
            this.txb_SOCL3.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.txb_SOCL3.Name = "txb_SOCL3";
            this.txb_SOCL3.Size = new System.Drawing.Size(97, 28);
            this.txb_SOCL3.TabIndex = 34;
            // 
            // txb_SOCL2
            // 
            this.txb_SOCL2.Location = new System.Drawing.Point(174, 69);
            this.txb_SOCL2.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.txb_SOCL2.Name = "txb_SOCL2";
            this.txb_SOCL2.Size = new System.Drawing.Size(97, 28);
            this.txb_SOCL2.TabIndex = 33;
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(280, 30);
            this.label12.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(17, 18);
            this.label12.TabIndex = 35;
            this.label12.Text = "%";
            // 
            // txb_SOC_L1
            // 
            this.txb_SOC_L1.Location = new System.Drawing.Point(176, 26);
            this.txb_SOC_L1.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.txb_SOC_L1.Name = "txb_SOC_L1";
            this.txb_SOC_L1.Size = new System.Drawing.Size(97, 28);
            this.txb_SOC_L1.TabIndex = 32;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(4, 122);
            this.label6.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(161, 18);
            this.label6.TabIndex = 31;
            this.label6.Text = "SOC过低三级阀值：";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(6, 74);
            this.label5.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(161, 18);
            this.label5.TabIndex = 30;
            this.label5.Text = "SOC过低二级阀值：";
            // 
            // btn_SOC_TsdL
            // 
            this.btn_SOC_TsdL.BackColor = System.Drawing.Color.DodgerBlue;
            this.btn_SOC_TsdL.Location = new System.Drawing.Point(86, 174);
            this.btn_SOC_TsdL.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.btn_SOC_TsdL.Name = "btn_SOC_TsdL";
            this.btn_SOC_TsdL.Size = new System.Drawing.Size(135, 57);
            this.btn_SOC_TsdL.TabIndex = 38;
            this.btn_SOC_TsdL.Text = "标定";
            this.btn_SOC_TsdL.UseVisualStyleBackColor = false;
            this.btn_SOC_TsdL.Click += new System.EventHandler(this.btn_SOC_TsdL_Click);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(6, 30);
            this.label4.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(161, 18);
            this.label4.TabIndex = 29;
            this.label4.Text = "SOC过低一级阀值：";
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(280, 82);
            this.label13.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(17, 18);
            this.label13.TabIndex = 39;
            this.label13.Text = "%";
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.label14);
            this.panel1.Controls.Add(this.label13);
            this.panel1.Controls.Add(this.label4);
            this.panel1.Controls.Add(this.btn_SOC_TsdL);
            this.panel1.Controls.Add(this.label5);
            this.panel1.Controls.Add(this.label6);
            this.panel1.Controls.Add(this.txb_SOC_L1);
            this.panel1.Controls.Add(this.label12);
            this.panel1.Controls.Add(this.txb_SOCL2);
            this.panel1.Controls.Add(this.txb_SOCL3);
            this.panel1.Location = new System.Drawing.Point(182, 236);
            this.panel1.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(314, 242);
            this.panel1.TabIndex = 39;
            this.panel1.Visible = false;
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(280, 122);
            this.label14.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(17, 18);
            this.label14.TabIndex = 40;
            this.label14.Text = "%";
            // 
            // SocSet
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 18F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.White;
            this.ClientSize = new System.Drawing.Size(668, 852);
            this.Controls.Add(this.panel3);
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.panel1);
            this.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.Margin = new System.Windows.Forms.Padding(6, 6, 6, 6);
            this.Name = "SocSet";
            this.Text = "SOC";
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.panel3.ResumeLayout(false);
            this.panel3.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button btn_SOC_Read;
        public System.Windows.Forms.TextBox txb_SOC_Read;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btn_SOC_Volt;
        public System.Windows.Forms.TextBox txb_SOC_Volt;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.Panel panel3;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button btn_SOC_Tsd_H;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label9;
        public System.Windows.Forms.TextBox txb_SOCTsdH1;
        public System.Windows.Forms.TextBox txb_SocTsdH2;
        public System.Windows.Forms.TextBox txb_SOCTsdH3;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label11;
        public System.Windows.Forms.TextBox txb_SOCL3;
        public System.Windows.Forms.TextBox txb_SOCL2;
        private System.Windows.Forms.Label label12;
        public System.Windows.Forms.TextBox txb_SOC_L1;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Button btn_SOC_TsdL;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label label14;
    }
}