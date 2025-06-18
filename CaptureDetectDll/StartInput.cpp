#include "pch.h"

#include "BrandBrigade.h"

std::thread live_stream_thread;
std::thread file_input_thread;

std::atomic_bool stop_threads = false;

std::wstring filePath;
bool newFile = true;

std::atomic<bool> keepRunning(false);  

std::mutex mtx_live;
std::condition_variable cv_live;
std::mutex mtx_file;
std::condition_variable cv_file;

using namespace cv;
using namespace std;
using namespace cv::dnn;

CComPtr<IMFDevice> g_cpLive;      //IMFDevice pointer

void LiveStreamToPreview(gcroot<Panel^> preview)
{
	preview->BackgroundImageLayout = ImageLayout::Stretch;

	// Create MFLive instance
	HRESULT hr = g_cpLive.CoCreateInstance(__uuidof(MFLive));

	//Probe selected live device
	hr = g_cpLive->DeviceSet(eMFDT_Video, 5 - 1, CComBSTR(L""));

	CComQIPtr<IMFFormat> cpFormat(g_cpLive);
	CComBSTR cbsName;
	M_VID_PROPS vidProps = {};

	hr = cpFormat->FormatVideoGetByIndex(eMFT_Input, 0, &vidProps, &cbsName);
	hr = cpFormat->FormatVideoSet(eMFT_Input, &vidProps);

	CComPtr<IMFFrame> cpFrame;
	CComQIPtr<IMFSource> cpSource(g_cpLive);
	M_AV_PROPS avProps = {};

	// Load class list.
	vector<string> class_list;
	get_object_names(class_list);

	cv::dnn::Net dnnNet = get_brandbrigade_net();
	std::vector<cv::String> outNames = dnnNet.getUnconnectedOutLayersNames();

	while (true)
	{
		// Efficiently wait if paused
		std::unique_lock<std::mutex> lock(mtx_live);
		cv_live.wait(lock, [] { return keepRunning.load() || stop_threads; });  // wait until keepRunning == true, TODO add stop_thread_1 is false


		if (stop_threads)
		{
			break;
		}

		cpFrame = NULL;
		//Get frames one by one 
		hr = cpSource->SourceFrameConvertedGet(&avProps, -1, &cpFrame, CComBSTR(L""));
		if (cpFrame)
		{
			Mat opencvmat = GetCVMatFromMFFrame(cpFrame);

			vector<Mat> detections;     // Process the image.

			detections = pre_process(opencvmat, dnnNet, outNames);

			Mat img = post_process(opencvmat, detections, class_list);

			preview->BackgroundImage = gcnew Drawing::Bitmap(img.size().width, img.size().height, img.step, Drawing::Imaging::PixelFormat::Format24bppRgb, (IntPtr)img.data);
		}
	}

	cpFrame = NULL;
	g_cpLive->DeviceClose();
	g_cpLive = NULL;

}

void FileInputPanel(gcroot<Panel^> preview)
{
	preview->BackgroundImageLayout = ImageLayout::Stretch;

	CComPtr<IMFReader> cpReader;    //IMFReader pointer
	// Create MFReader instance
	HRESULT hr = cpReader.CoCreateInstance(__uuidof(MFReader));


	CComPtr<IMFFrame> cpFrame;
	M_AV_PROPS avProps = {};        //Video props structure
	M_TIME mTime = {};
	bool bRewind{ false };

	// Load class list.
	vector<string> class_list;
	get_object_names(class_list);

	cv::dnn::Net dnnNet = get_brandbrigade_net();
	std::vector<cv::String> outNames = dnnNet.getUnconnectedOutLayersNames();


	while (true)
	{
		// Efficiently wait if paused
		std::unique_lock<std::mutex> lock(mtx_file);
		cv_file.wait(lock, [] { return filePath.size() > 0 || stop_threads; });  // Wait until keepRunning is true, TODO add stop_thread_1 is false

		if (stop_threads)
			break;

		if (newFile)
		{
			if (cpReader)
				cpReader->ReaderClose();

			CComBSTR cbsFileName = CComBSTR(filePath.c_str());

			//Open File
			hr = cpReader->ReaderOpen(cbsFileName, CComBSTR(L""));

			newFile = false;
		}

		cpFrame = NULL;
		//Get frame by number. -1 as frame number means next frame
		if (bRewind)
		{
			hr = cpReader->SourceFrameConvertedGetByNumber(&avProps, 0, -1, &cpFrame, CComBSTR(L""));
			bRewind = false;
		}
		else
		{
			hr = cpReader->SourceFrameConvertedGetByTime(&avProps, -1, -1, &cpFrame, CComBSTR(L""));
		}
		if (cpFrame)
		{
			//Check for the last frame to stop the playback
			hr = cpFrame->MFTimeGet(&mTime);
			if ((mTime.eFFlags & eMFF_Last) != 0)
			{
				bRewind = true;
			}

			Mat opencvmat = GetCVMatFromMFFrame(cpFrame);

			vector<Mat> detections;     // Process the image.

			detections = pre_process(opencvmat, dnnNet, outNames);

			Mat img = post_process(opencvmat, detections, class_list);

			preview->BackgroundImage = gcnew Drawing::Bitmap(img.size().width, img.size().height, img.step, Drawing::Imaging::PixelFormat::Format24bppRgb, (IntPtr)img.data);
		}
	}
	if (cpReader)
		cpReader->ReaderClose();

	cpFrame = NULL;
	cpReader = NULL;
}

void StartThreads(HWND panelHandle, bool useWebcam)
{
	gcroot<Panel^> rgr = (Panel^)Windows::Forms::Control::FromHandle((IntPtr)panelHandle);

	file_input_thread = std::thread(FileInputPanel, rgr);

	if (useWebcam)
	{
		live_stream_thread = std::thread(LiveStreamToPreview, rgr);
	}
}

void StopThreads(bool useWebcam)
{
	stop_threads = true;
	cv_live.notify_one();
	cv_file.notify_one();
	if (useWebcam)
	{
		if (live_stream_thread.joinable())
		{
			live_stream_thread.join();
		}
	}
	if (file_input_thread.joinable())
	{
		file_input_thread.join();
	}
}