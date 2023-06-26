#include "pch.h"

#include "BrandBrigade.h"

std::thread working_thread;
std::atomic_bool stop_thread_1 = false;

// Namespaces.
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

	while (stop_thread_1 == false)
	{
		cpFrame = NULL;
		//Get frames one by one 
		//cout << time(nullptr) << endl;
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

void LiveInput(HWND panelHandle)
{
	if (working_thread.joinable())
	{
		stop_thread_1 = true;
		working_thread.join();
	}

	gcroot<Panel^> rgr = (Panel ^)Windows::Forms::Control::FromHandle((IntPtr)panelHandle);

	stop_thread_1 = false;

	working_thread = std::thread(LiveStreamToPreview,rgr);

}
