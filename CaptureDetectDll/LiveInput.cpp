#include "pch.h"

#include "BrandBrigade.h"

extern std::wstring filePath;
extern bool newFile;
extern std::atomic<bool> keepRunning;  // Fast state flag
extern std::condition_variable cv_live;

void LiveInput()
{
	filePath.clear();
	newFile = true;

	keepRunning = true;
	cv_live.notify_one();  // Wake up the thread
}
