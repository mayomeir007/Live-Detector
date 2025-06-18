#include "pch.h"

#include "BrandBrigade.h"

extern std::atomic<bool> keepRunning;
extern std::wstring filePath;
extern bool newFile;

using namespace cv;

Mat GetCVMatFromMFFrame(const CComPtr<IMFFrame>& cpFrame)
{
	IMFFrame* pFrameDraw = nullptr;
	cpFrame.p->MFClone(&pFrameDraw, (eMFrameClone)eMFC_Video_ForceCPU, (eMFCC)eMFCC_RGB24);

	MF_VID_PTR vidptr2;

	pFrameDraw->MFVideoGetBytesEx(&vidptr2);

	Mat opencvmat(vidptr2.szVideoPlanes[0].cy, vidptr2.szVideoPlanes[0].cx, CV_8UC3, (void*)vidptr2.lpVideoPlanes[0], vidptr2.cbVideoRowBytes[0]);

	return opencvmat;
}

void StopPreview()
{
	keepRunning = false;

	filePath.clear();
	newFile = true;
	
}

