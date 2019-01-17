namespace BMSTool.Forms
{
    partial class ReadInfo
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
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.btnEEpromClear = new System.Windows.Forms.Button();
            this.btnEEPromRead = new System.Windows.Forms.Button();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.btnReadInfoReTme = new System.Windows.Forms.Button();
            this.groupBox3.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.btnEEpromClear);
            this.groupBox3.Controls.Add(this.btnEEPromRead);
            this.groupBox3.Location = new System.Drawing.Point(36, 26);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(239, 130);
            this.groupBox3.TabIndex = 0;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "EEPROM";
            // 
            // btnEEpromClear
            // 
            this.btnEEpromClear.Location = new System.Drawing.Point(158, 97);
            this.btnEEpromClear.Name = "btnEEpromClear";
            this.btnEEpromClear.Size = new System.Drawing.Size(75, 23);
            this.btnEEpromClear.TabIndex = 1;
            this.btnEEpromClear.Text = "清除信息";
            this.btnEEpromClear.UseVisualStyleBackColor = true;
            this.btnEEpromClear.Click += new System.EventHandler(this.btnEEpromClear_Click);
            // 
            // btnEEPromRead
            // 
            this.btnEEPromRead.Location = new System.Drawing.Point(158, 40);
            this.btnEEPromRead.Name = "btnEEPromRead";
            this.btnEEPromRead.Size = new System.Drawing.Size(75, 23);
            this.btnEEPromRead.TabIndex = 0;
            this.btnEEPromRead.Text = "读取信息";
            this.btnEEPromRead.UseVisualStyleBackColor = true;
            this.btnEEPromRead.Click += new System.EventHandler(this.btnEEPromRead_Click);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.btnReadInfoReTme);
            this.groupBox2.Location = new System.Drawing.Point(306, 29);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(252, 127);
            this.groupBox2.TabIndex = 1;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "实时信息";
            // 
            // btnReadInfoReTme
            // 
            this.btnReadInfoReTme.Location = new System.Drawing.Point(158, 37);
            this.btnReadInfoReTme.Name = "btnReadInfoReTme";
            this.btnReadInfoReTme.Size = new System.Drawing.Size(75, 23);
            this.btnReadInfoReTme.TabIndex = 0;
            this.btnReadInfoReTme.Text = "读取信息";
            this.btnReadInfoReTme.UseVisualStyleBackColor = true;
            this.btnReadInfoReTme.Click += new System.EventHandler(this.btnReadInfoReTme_Click);
            // 
            // ReadInfo
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(630, 196);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Name = "ReadInfo";
            this.Text = "信息读取";
            this.groupBox3.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Button btnEEpromClear;
        private System.Windows.Forms.Button btnEEPromRead;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button btnReadInfoReTme;
    }
}