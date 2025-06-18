namespace BrandBrigade3
{
    partial class BrandBrigadeTask
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
            this.PreviewPanel = new System.Windows.Forms.Panel();
            this.FromFileBtn = new System.Windows.Forms.Button();
            this.FromWebCamBtn = new System.Windows.Forms.Button();
            this.StopBtn = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // PreviewPanel
            // 
            this.PreviewPanel.Location = new System.Drawing.Point(12, 12);
            this.PreviewPanel.Name = "PreviewPanel";
            this.PreviewPanel.Size = new System.Drawing.Size(640, 360);
            this.PreviewPanel.TabIndex = 0;
            // 
            // FromFileBtn
            // 
            this.FromFileBtn.Location = new System.Drawing.Point(61, 387);
            this.FromFileBtn.Name = "FromFileBtn";
            this.FromFileBtn.Size = new System.Drawing.Size(135, 47);
            this.FromFileBtn.TabIndex = 1;
            this.FromFileBtn.Text = "Start Capture From File";
            this.FromFileBtn.UseVisualStyleBackColor = true;
            this.FromFileBtn.Click += new System.EventHandler(this.FromFileBtn_Click);
            // 
            // FromWebCamBtn
            // 
            this.FromWebCamBtn.Location = new System.Drawing.Point(259, 387);
            this.FromWebCamBtn.Name = "FromWebCamBtn";
            this.FromWebCamBtn.Size = new System.Drawing.Size(135, 47);
            this.FromWebCamBtn.TabIndex = 2;
            this.FromWebCamBtn.Text = "Start Capture From Webcam";
            this.FromWebCamBtn.UseVisualStyleBackColor = true;
            this.FromWebCamBtn.Click += new System.EventHandler(this.FromWebCamBtn_Click);
            // 
            // StopBtn
            // 
            this.StopBtn.Location = new System.Drawing.Point(461, 387);
            this.StopBtn.Name = "StopBtn";
            this.StopBtn.Size = new System.Drawing.Size(135, 47);
            this.StopBtn.TabIndex = 3;
            this.StopBtn.Text = "Stop";
            this.StopBtn.UseVisualStyleBackColor = true;
            this.StopBtn.Click += new System.EventHandler(this.StopBtn_Click);
            // 
            // BrandBrigadeTask
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(671, 446);
            this.Controls.Add(this.StopBtn);
            this.Controls.Add(this.FromWebCamBtn);
            this.Controls.Add(this.FromFileBtn);
            this.Controls.Add(this.PreviewPanel);
            this.Name = "BrandBrigadeTask";
            this.Text = "Brand Brigade Task";
            this.Load += new System.EventHandler(this.BrandBrigadeTask_Load);
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.BrandBrigadeTask_FormClosed);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel PreviewPanel;
        private System.Windows.Forms.Button FromFileBtn;
        private System.Windows.Forms.Button FromWebCamBtn;
        private System.Windows.Forms.Button StopBtn;
    }
}

