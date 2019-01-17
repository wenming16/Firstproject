namespace BMSTool.Forms
{
    partial class SohSet
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
            this.label2 = new System.Windows.Forms.Label();
            this.txb_SysTime = new System.Windows.Forms.MaskedTextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.panel1 = new System.Windows.Forms.Panel();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.txb_AllRunTime = new System.Windows.Forms.TextBox();
            this.txb_OnceTime = new System.Windows.Forms.TextBox();
            this.txb_PowerOff = new System.Windows.Forms.TextBox();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(45, 154);
            this.label2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(179, 18);
            this.label2.TabIndex = 17;
            this.label2.Text = "BMS系统运行总时长：";
            // 
            // txb_SysTime
            // 
            this.txb_SysTime.Location = new System.Drawing.Point(129, 4);
            this.txb_SysTime.Margin = new System.Windows.Forms.Padding(4);
            this.txb_SysTime.Mask = "0000-00-00 90:00:00";
            this.txb_SysTime.Name = "txb_SysTime";
            this.txb_SysTime.Size = new System.Drawing.Size(202, 28);
            this.txb_SysTime.TabIndex = 20;
            this.txb_SysTime.ValidatingType = typeof(System.DateTime);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(2, 14);
            this.label3.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(116, 18);
            this.label3.TabIndex = 21;
            this.label3.Text = "BMS系统时间:";
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.txb_SysTime);
            this.panel1.Controls.Add(this.label3);
            this.panel1.Location = new System.Drawing.Point(66, 464);
            this.panel1.Margin = new System.Windows.Forms.Padding(4);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(350, 45);
            this.panel1.TabIndex = 22;
            this.panel1.Visible = false;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(63, 262);
            this.label4.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(161, 18);
            this.label4.TabIndex = 23;
            this.label4.Text = "BMS上次断电时长：";
            this.label4.Visible = false;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(63, 206);
            this.label5.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(161, 18);
            this.label5.TabIndex = 24;
            this.label5.Text = "BMS单次运行时长：";
            this.label5.Visible = false;
            // 
            // txb_AllRunTime
            // 
            this.txb_AllRunTime.Location = new System.Drawing.Point(242, 150);
            this.txb_AllRunTime.Margin = new System.Windows.Forms.Padding(4);
            this.txb_AllRunTime.Name = "txb_AllRunTime";
            this.txb_AllRunTime.Size = new System.Drawing.Size(148, 28);
            this.txb_AllRunTime.TabIndex = 26;
            // 
            // txb_OnceTime
            // 
            this.txb_OnceTime.Location = new System.Drawing.Point(242, 202);
            this.txb_OnceTime.Margin = new System.Windows.Forms.Padding(4);
            this.txb_OnceTime.Name = "txb_OnceTime";
            this.txb_OnceTime.Size = new System.Drawing.Size(146, 28);
            this.txb_OnceTime.TabIndex = 27;
            this.txb_OnceTime.Visible = false;
            // 
            // txb_PowerOff
            // 
            this.txb_PowerOff.Location = new System.Drawing.Point(242, 258);
            this.txb_PowerOff.Margin = new System.Windows.Forms.Padding(4);
            this.txb_PowerOff.Name = "txb_PowerOff";
            this.txb_PowerOff.Size = new System.Drawing.Size(146, 28);
            this.txb_PowerOff.TabIndex = 28;
            this.txb_PowerOff.Visible = false;
            // 
            // SohSet
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 18F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.White;
            this.ClientSize = new System.Drawing.Size(482, 610);
            this.Controls.Add(this.txb_PowerOff);
            this.Controls.Add(this.txb_OnceTime);
            this.Controls.Add(this.txb_AllRunTime);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.label2);
            this.Margin = new System.Windows.Forms.Padding(6);
            this.Name = "SohSet";
            this.Text = "SOH";
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Panel panel1;
        public System.Windows.Forms.MaskedTextBox txb_SysTime;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox txb_AllRunTime;
        private System.Windows.Forms.TextBox txb_OnceTime;
        private System.Windows.Forms.TextBox txb_PowerOff;
    }
}