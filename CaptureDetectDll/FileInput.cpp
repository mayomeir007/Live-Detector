#include "pch.h"

#include "BrandBrigade.h"


extern std::atomic<bool> keepRunning;
extern std::wstring filePath;
extern std::condition_variable cv_file;
extern bool newFile;

void FileInput(wchar_t* filepath)
{

	keepRunning = false;

	filePath = filepath;
	newFile = true;
	cv_file.notify_one();  // Wake up the thread
}
