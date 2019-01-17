namespace BMSTool.Forms
{
    partial class BasicForm
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
            this.components = new System.ComponentModel.Container();
            this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.停止刷新ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.tsp_ClearScreen = new System.Windows.Forms.ToolStripMenuItem();
            this.tmr_DatatoForm = new System.Windows.Forms.Timer(this.components);
            this.contextMenuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // contextMenuStrip1
            // 
            this.contextMenuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.停止刷新ToolStripMenuItem,
            this.tsp_ClearScreen});
            this.contextMenuStrip1.Name = "contextMenuStrip1";
            this.contextMenuStrip1.Size = new System.Drawing.Size(125, 48);
            // 
            // 停止刷新ToolStripMenuItem
            // 
            this.停止刷新ToolStripMenuItem.DoubleClickEnabled = true;
            this.停止刷新ToolStripMenuItem.Name = "停止刷新ToolStripMenuItem";
            this.停止刷新ToolStripMenuItem.Size = new System.Drawing.Size(124, 22);
            this.停止刷新ToolStripMenuItem.Text = "停止刷新";
            this.停止刷新ToolStripMenuItem.Click += new System.EventHandler(this.停止刷新ToolStripMenuItem_Click);
            // 
            // tsp_ClearScreen
            // 
            this.tsp_ClearScreen.Name = "tsp_ClearScreen";
            this.tsp_ClearScreen.Size = new System.Drawing.Size(124, 22);
            this.tsp_ClearScreen.Text = "清屏";
            this.tsp_ClearScreen.Click += new System.EventHandler(this.tsp_ClearScreen_Click);
            // 
            // tmr_DatatoForm
            // 
            this.tmr_DatatoForm.Enabled = true;
            this.tmr_DatatoForm.Tick += new System.EventHandler(this.tmr_DatatoForm_Tick);
            // 
            // BasicForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.LightGray;
            this.ClientSize = new System.Drawing.Size(321, 407);
            this.ContextMenuStrip = this.contextMenuStrip1;
            this.Name = "BasicForm";
            this.Text = "BasicForm";
            this.contextMenuStrip1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion
        protected System.Windows.Forms.ToolStripMenuItem 停止刷新ToolStripMenuItem;
        private System.Windows.Forms.Timer tmr_DatatoForm;
        private System.Windows.Forms.ContextMenuStrip contextMenuStrip1;
        private System.Windows.Forms.ToolStripMenuItem tsp_ClearScreen;
    }
}