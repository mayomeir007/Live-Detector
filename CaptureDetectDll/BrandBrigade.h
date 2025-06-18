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
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <chrono>

#include "stdafx.h"
#include "MIDL64/MFormats.h"

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

	//converts input windows handle to unmanaged CLR form, and starts a new thread with function 'FileInputPanel' and a new thread with function 'LiveInputPanel'
	void __declspec(dllexport) StartThreads(HWND paneHandle, bool useWebCam);

	//stops live feed if there is one, and replaces existing input file if there is one with new input file
	void __declspec(dllexport) FileInput(wchar_t * filepath);

	//stops existing input file if there is one ,and starts a live feed 
	void __declspec(dllexport) LiveInput();

	//stops the the live feed id there is one, and current input file if there is one
	void __declspec(dllexport) StopPreview();

	//stops the thread with function 'FileInputPanel' and the thread with function 'LiveInputPanel'
	void __declspec(dllexport) StopThreads(bool useWebCam);
#ifdef __cplusplus
}
#endif

//converts the input MFrame to an OpenCV Mat
cv::Mat GetCVMatFromMFFrame(const CComPtr<IMFFrame>& cpFrame);

//streams video/image frames on the given panel, with rectangles and labels over detected objects
void FileInputPanel(gcroot<Panel^> preview);

//streams computer webcam frames on the given panel, with rectangles and labels over detected objects
void LiveStreamToPreview(gcroot<Panel^> preview);

#endif
