//*******************************************************************
//Author: Meir Basson
// This dll project allows capturing video from either a webcam or a file, detecting
//objects in the video’s frame, and showing the frame and the detected objects using rectangles
// 
// 
//This dll project demonstrates the integration of the following technologies:
//1. MediaLook MFormats SDK - for capturing the frames from the webcam and video files
//2. OpenCV's DNN module - for detecting objects in the frames
//3. OpenCV - for drawing on the frames
//4. CLR windows forms - for displaying the frames on the panel window of the C# GUI
//5. data type marshalling, for calling C++ functions in a C# environment,
//
//The dll also uses multithreading to enable switching between webcam, video file, and 
// stopping options during runtime.
//********************************************************************
#pragma once
#ifndef LIVE_INPUT_H
#define LIVE_INPUT_H
#include <thread>


#include "stdafx.h"
#include "MIDL64/MFormats.h"
#include "Include/ArgParse/get_arguments.h"
#include "Include/ArgParse/platform_os.h"

#include "OpenCV_dnn.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


#ifdef __cplusplus
extern "C" {
#endif
	//converts input windows handle to unmanaged CLR form and starts a new thread with function 'FileInputPanel'
	void __declspec(dllexport) FileInput(HWND panelHandle, wchar_t * filepath);

	//converts input windows handle to unmanaged CLR form and starts a new thread with function 'LiveInputPanel'
	void __declspec(dllexport) LiveInput(HWND panelHandle);

	//stops the working thread, if it exists
	void __declspec(dllexport) StopPreview();
#ifdef __cplusplus
}
#endif

//converts the input MFrame to an OpenCV Mat
cv::Mat GetCVMatFromMFFrame(const CComPtr<IMFFrame>& cpFrame);

//streams frames of input video/image on the given panel, with rectangles and labels over detected objects
void FileInputPanel(gcroot<Panel^> preview, wchar_t* filepath);

//streams computer webcam frames on the given panel, with rectangles and labels over detected objects
void LiveStreamToPreview(gcroot<Panel^> preview);

#endif
