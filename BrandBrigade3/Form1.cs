//**************************************************************
//Author: Meir Basson
//C# application for capturing video from computer webcam or input video/image file, and displaying it with
//object detection 
//the application uses a dll written in C++. 
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.VisualStyles;

namespace BrandBrigade3
{
    public partial class BrandBrigadeTask : Form
    {
        //starts the capture from given vidoe/image file and displays it in the preview panel with object detection  
        [DllImport("CaptureDetectDll.dll", CharSet = CharSet.Unicode)]
        public static extern void FileInput(IntPtr panelHandle, string filename);

        //starts the capture from the webcam asnd displays it in the preview panel with object detection  
        [DllImport("CaptureDetectDll.dll")]
        public static extern void LiveInput(IntPtr panelHandle);

        //stops the capturing process, if one is running
        [DllImport("CaptureDetectDll.dll")]
        public static extern void StopPreview();

        public BrandBrigadeTask()
        {
            InitializeComponent();
        }

        private void FromFileBtn_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            if (openFileDialog.ShowDialog() == DialogResult.OK && openFileDialog.FileName != string.Empty)
            {
                FileInput(PreviewPanel.Handle, openFileDialog.FileName);
            }

        }

        private void FromWebCamBtn_Click(Object sender, EventArgs e)
        {
            LiveInput(PreviewPanel.Handle);
        }
        
        private void StopBtn_Click(Object sender, EventArgs e)
        {
            StopPreview();
        }
        private void BrandBrigadeTask_FormClosed(object sender, EventArgs e)
        {
            StopPreview();
        }
    }
}
