#include <Windows.h>
#include "../../ThirdParty/TDA/ConsoleX.h"
#include "../../ThirdParty/TDA/MemX.h"
#include <fstream>

//HOOKCODE Macro Definition
#define HOOKCODE_START __asm pushad __asm pushfd
#define HOOKCODE_END __asm popfd __asm popad __asm ret

//Global Variables Definition
BOOL g_isOutInfo = TRUE;
LPWSTR g_lpPackName = NULL;
LPWSTR g_lpFileName = NULL;
HANDLE g_hSemaphore = NULL;
LPCWSTR g_lpBaseFolder = L"FileHook/";
WCHAR g_lpReplacePath[MAX_PATH] = { 0 };

//Games Already Tested
/*
* 恋するあまいろホームステイ -留学生はわんこ系幼馴染
* ReadFileUnity 0x004067E0
*/
#define ReadFileUnity 0x004067E0
#define HookSize 0x6

VOID PrintfInfo(BOOL isExist)
{
	if (!g_isOutInfo) return;

	if (g_lpPackName)
	{
		std::wcout << L"Pack:" << g_lpPackName << L'\n';
	}
	else
	{
		std::wcout << L"Pack:" << L"NULL" << L'\n';
	}

	if (g_lpFileName)
	{
		std::wcout << L"File:" << g_lpFileName << L'\n';
	}
	else
	{
		std::wcout << L"File:" << L"NULL" << L'\n';
	}

	if (isExist)
	{
		std::wcout << L"Hook:" << g_lpReplacePath << L'\n';
	}

	std::wcout << std::endl;
}

BOOL FileExist()
{
	//Clear Buffer
	memset(g_lpReplacePath, 0, MAX_PATH);

	//Cut Relative Path
	lstrcatW(g_lpReplacePath, g_lpBaseFolder);
	lstrcatW(g_lpReplacePath, g_lpPackName);
	lstrcatW(g_lpReplacePath, L"/");
	lstrcatW(g_lpReplacePath, g_lpFileName);

	//Check Is Exist
	DWORD fileAttributes = GetFileAttributesW(g_lpReplacePath);
	if ((fileAttributes != INVALID_FILE_ATTRIBUTES) && (fileAttributes != FILE_ATTRIBUTE_DIRECTORY))
	{
		PrintfInfo(TRUE);
		return TRUE;
	}
	else
	{
		PrintfInfo(FALSE);
		return FALSE;
	}
}

VOID __declspec(naked) ReadFileUnity_Hook()
{
	HOOKCODE_START;
	WaitForSingleObject(g_hSemaphore, INFINITE);

	//Clear String Pointer
	g_lpPackName = NULL;
	g_lpFileName = NULL;

	//Init String Pointer
	__asm
	{
		mov eax, dword ptr[esp + 0x38] //lpPackName
		mov g_lpPackName, eax
		mov eax, dword ptr[esp + 0x3C] //lpFileName
		mov g_lpFileName, eax
	}

	//Check Is Need Replace
	if (FileExist() && g_lpPackName && g_lpFileName)
	{
		__asm
		{
			mov dword ptr[esp + 0x38], 0
			mov eax, offset g_lpReplacePath
			mov dword ptr[esp + 0x3C], eax
		}
	}

	ReleaseSemaphore(g_hSemaphore, 1, NULL);
	HOOKCODE_END;
}

VOID StartHook()
{
	if (g_isOutInfo) TDA::ConsoleX::SetConsole(L"AdvHD File Hook");

	g_hSemaphore = CreateSemaphoreW(NULL, 1, 1, NULL);

	TDA::MemX::SetHook((DWORD)ReadFileUnity, (DWORD)ReadFileUnity_Hook, HookSize);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		StartHook();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		CloseHandle(g_hSemaphore);
		break;
	}
	return TRUE;
}

VOID __declspec(dllexport) DirA() {}