/*
	Storm.cpp
*/

#include "storm.h"

// Storm Function Names
funcSFileCloseArchive SFileCloseArchive;
funcSFileCloseFile SFileCloseFile;
funcSFileDestroy SFileDestroy;
funcSFileGetFileArchive SFileGetFileArchive;
funcSFileGetFileSize SFileGetFileSize;
funcSFileOpenArchive SFileOpenArchive;
funcSFileOpenFile SFileOpenFile;
funcSFileOpenFileEx SFileOpenFileEx;
funcSFileReadFile SFileReadFile;
funcSFileSetBasePath SFileSetBasePath;
funcSFileSetFilePointer SFileSetFilePointer;
funcSFileSetLocale SFileSetLocale;
funcSFileGetBasePath SFileGetBasePath;
funcSFileGetArchiveName SFileGetArchiveName;
funcSFileGetFileName SFileGetFileName;

/////////////////////////////////////////////////////////////////////////////
// LoadStorm - You must call this before any storm functions
//     may be called.  Pass the filename of storm (usually just
//     storm.dll) to this function and use its return value in a call to
//     FreeLibrary before closing the program.  If this function returns
//     NULL, you do not need to and should not call FreeLibrary with the
//     return value of this function.

HINSTANCE LoadStorm(char * DllFileName)
{
	HINSTANCE DllHandle;
	BOOL AlreadyLoaded=FALSE;
	DllHandle = GetModuleHandle(DllFileName);
	if (DllHandle == NULL) DllHandle = LoadLibrary(DllFileName);
	else AlreadyLoaded=TRUE;
	if (DllHandle != NULL)
	{
		SFileCloseArchive = (funcSFileCloseArchive)GetProcAddress(DllHandle, (LPCSTR)0xFC);
		SFileCloseFile = (funcSFileCloseFile)GetProcAddress(DllHandle, (LPCSTR)0xFD);
		SFileDestroy = (funcSFileDestroy)GetProcAddress(DllHandle, (LPCSTR)0x106);
		SFileGetFileArchive = (funcSFileGetFileArchive)GetProcAddress(DllHandle, (LPCSTR)0x108);
		SFileGetFileSize = (funcSFileGetFileSize)GetProcAddress(DllHandle, (LPCSTR)0x109);
		SFileOpenArchive = (funcSFileOpenArchive)GetProcAddress(DllHandle, (LPCSTR)0x10A);
		SFileOpenFile = (funcSFileOpenFile)GetProcAddress(DllHandle, (LPCSTR)0x10B);
		SFileOpenFileEx = (funcSFileOpenFileEx)GetProcAddress(DllHandle, (LPCSTR)0x10C);
		SFileReadFile = (funcSFileReadFile)GetProcAddress(DllHandle, (LPCSTR)0x10D);
		SFileSetBasePath = (funcSFileSetBasePath)GetProcAddress(DllHandle, (LPCSTR)0x10E);
		SFileSetFilePointer = (funcSFileSetFilePointer)GetProcAddress(DllHandle, (LPCSTR)0x10F);
		SFileSetLocale = (funcSFileSetLocale)GetProcAddress(DllHandle, (LPCSTR)0x110);
		SFileGetBasePath = (funcSFileGetBasePath)GetProcAddress(DllHandle, (LPCSTR)0x111);
		SFileGetArchiveName = (funcSFileGetArchiveName)GetProcAddress(DllHandle, (LPCSTR)0x113);
		SFileGetFileName = (funcSFileGetFileName)GetProcAddress(DllHandle, (LPCSTR)0x114);
	}
	if (AlreadyLoaded==TRUE) return 0;
	return DllHandle;
}

