#include "pch.h"

#include "BrandBrigade.h"

extern std::thread working_thread;
extern std::atomic_bool stop_thread_1;

// Namespaces.
using namespace cv;
using namespace std;
using namespace cv::dnn;


void FileInputPanel(gcroot<Panel^> preview, wchar_t* filepath)
{
	preview->BackgroundImageLayout = ImageLayout::Stretch;

	CComBSTR cbsFileName = CComBSTR(filepath);
	bool _bLoop = true;

	CComPtr<IMFReader> cpReader;    //IMFReader pointer
	// Create MFReader instance
	HRESULT hr = cpReader.CoCreateInstance(__uuidof(MFReader));

	//Open File
	hr = cpReader->ReaderOpen(cbsFileName, CComBSTR(L""));

	CComPtr<IMFFrame> cpFrame;
	M_AV_PROPS avProps = {};        //Video props structure
	M_TIME mTime = {};
	bool bRewind{ false };

	// Load class list.
	vector<string> class_list;
	get_object_names(class_list);

	cv::dnn::Net dnnNet = get_brandbrigade_net();
	std::vector<cv::String> outNames = dnnNet.getUnconnectedOutLayersNames();

	while (stop_thread_1 == false)
	{
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


void FileInput(HWND panelHandle, wchar_t* filepath)
{
	if (working_thread.joinable())
	{
		stop_thread_1 = true;
		working_thread.join();
	}
	gcroot<Panel^> rgr = (Panel^)Windows::Forms::Control::FromHandle((IntPtr)panelHandle);

	stop_thread_1 = false;

	working_thread = std::thread(FileInputPanel, rgr, filepath);

}
