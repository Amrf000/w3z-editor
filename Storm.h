#ifndef STORM_H_INCLUDED
#define STORM_H_INCLUDED

#include <windows.h>
#include <winbase.h>

#ifdef __cplusplus
extern "C" {
#endif

extern HINSTANCE LoadStorm(char * DllFileName);

#define SFILE_OPEN_HARD_DISK_FILE 0x0000 //Open archive without regard to the drive type it resides on
#define SFILE_OPEN_CD_ROM_FILE    0x0001 //Open the archive only if it is on a CD-ROM

#define SFILE_SEARCH_CURRENT_ONLY 0x00 //Used with SFileOpenFileEx; only the archive with the handle specified will be searched for the file
#define SFILE_SEARCH_ALL_OPEN     0x01 //SFileOpenFileEx will look through all open archives for the file

#define MPQHANDLE HANDLE

// Storm Function Prototypes
typedef BOOL  (WINAPI* funcSFileOpenArchive)(LPCSTR lpFilename, DWORD dwPriority, DWORD dwFlags, MPQHANDLE *hMPQ);
typedef BOOL  (WINAPI* funcSFileCloseArchive)(MPQHANDLE hMPQ);
typedef BOOL  (WINAPI* funcSFileOpenFile)(LPCSTR lpFileName, MPQHANDLE *hFile);
typedef BOOL  (WINAPI* funcSFileOpenFileEx)(MPQHANDLE hMPQ, LPCSTR lpFileName, DWORD dwSearchScope, MPQHANDLE *hFile);
typedef BOOL  (WINAPI* funcSFileCloseFile)(MPQHANDLE hFile);
typedef DWORD (WINAPI* funcSFileGetFileSize)(MPQHANDLE hFile, DWORD dwFlags);
typedef DWORD (WINAPI* funcSFileSetFilePointer)(MPQHANDLE hFile, long lDistanceToMove, PLONG lplDistanceToMoveHigh, DWORD dwMoveMethod);
typedef BOOL  (WINAPI* funcSFileReadFile)(MPQHANDLE hFile,LPVOID lpBuffer,DWORD nNumberOfBytesToRead,LPDWORD lpNumberOfBytesRead,LPOVERLAPPED lpOverlapped);
typedef LCID  (WINAPI* funcSFileSetLocale)(LCID nNewLocale);
typedef BOOL  (WINAPI* funcSFileDestroy)();
typedef BOOL  (WINAPI* funcSFileGetArchiveName)(MPQHANDLE hMPQ, LPCSTR lpBuffer, DWORD dwBufferLength);
typedef BOOL  (WINAPI* funcSFileGetFileName)(MPQHANDLE hFile, LPCSTR lpBuffer, DWORD dwBufferLength);
typedef BOOL  (WINAPI* funcSFileGetFileArchive)(MPQHANDLE hFile, MPQHANDLE *hMPQ);
typedef BOOL  (WINAPI* funcSFileGetBasePath)(LPCSTR lpBuffer, DWORD dwBufferLength);
typedef BOOL  (WINAPI* funcSFileSetBasePath)(LPCSTR lpNewBasePath);

// Storm Function Names
extern funcSFileCloseArchive SFileCloseArchive;
extern funcSFileCloseFile SFileCloseFile;
extern funcSFileDestroy SFileDestroy;
extern funcSFileGetFileArchive SFileGetFileArchive;
extern funcSFileGetFileSize SFileGetFileSize;
extern funcSFileOpenArchive SFileOpenArchive;
extern funcSFileOpenFile SFileOpenFile;
extern funcSFileOpenFileEx SFileOpenFileEx;
extern funcSFileReadFile SFileReadFile;
extern funcSFileSetBasePath SFileSetBasePath;
extern funcSFileSetFilePointer SFileSetFilePointer;
extern funcSFileSetLocale SFileSetLocale;
extern funcSFileGetBasePath SFileGetBasePath;
extern funcSFileGetArchiveName SFileGetArchiveName;
extern funcSFileGetFileName SFileGetFileName;

#ifdef __cplusplus
};  // extern "C" 
#endif

#endif
