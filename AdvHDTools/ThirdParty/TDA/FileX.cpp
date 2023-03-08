#include "FileX.h"

#include <Windows.h>


namespace TDA
{
	size_t FileX::GetCurrentDirectoryPath(char* pPath)
	{
		size_t sz = GetCurrentDirectoryA(MAX_PATH, pPath);
		strcat_s(pPath, MAX_PATH, "\\");
		return sz;
	}

	size_t FileX::GetCurrentDirectoryPath(wchar_t* pPath)
	{
		size_t sz = GetCurrentDirectoryW(MAX_PATH, pPath);
		wcscat_s(pPath, MAX_PATH, L"\\");
		return sz;
	}

	size_t FileX::GetCurrentDirectoryPath(std::string& msPath)
	{
		msPath.resize(MAX_PATH);
		msPath.resize(GetCurrentDirectoryA(MAX_PATH, const_cast<LPSTR>(msPath.data())));
		msPath.append("\\");
		return msPath.size();
	}

	size_t FileX::GetCurrentDirectoryPath(std::wstring& wsPath)
	{
		wsPath.resize(MAX_PATH);
		wsPath.resize(GetCurrentDirectoryW(MAX_PATH, const_cast<LPWSTR>(wsPath.data())));
		wsPath.append(L"\\");
		return wsPath.size();
	}

	void FileX::BackSlash(const char* pPath, std::string& msPath)
	{
		msPath = pPath;
		BackSlash(const_cast<char*>(msPath.data()));
	}

	void FileX::BackSlash(const wchar_t* pPath, std::wstring& wsPath)
	{
		wsPath = pPath;
		BackSlash(const_cast<wchar_t*>(wsPath.data()));
	}

	void FileX::BackSlash(char* pPath)
	{
		for (size_t ite = 0; pPath[ite]; ite++)
		{
			switch (pPath[ite])
			{
			case '/':
				pPath[ite] = '\\';
				break;
			case '\\':
				pPath[ite] = '/';
				break;
			}
		}
	}

	void FileX::BackSlash(wchar_t* pPath)
	{
		for (size_t ite = 0; pPath[ite]; ite++)
		{
			switch (pPath[ite])
			{
			case L'/':
				pPath[ite] = L'\\';
				break;
			case L'\\':
				pPath[ite] = L'/';
				break;
			}
		}
	}

	bool FileX::PathRemoveFileName(std::wstring& wsPath, std::wstring& wsRemoved)
	{
		size_t pos = wsPath.rfind(L"\\");
		if (pos != std::wstring::npos)
		{
			wsRemoved = wsPath.substr(0, pos + 1);

			return true;
		}
		else
		{
			return false;
		}
	}

	std::streamsize FileX::GetFileSize(const wchar_t* wsFile)
	{
		std::ifstream ifsFile(wsFile, std::ios::binary);
		if (!ifsFile.is_open()) return false;

		ifsFile.seekg(0, std::ios::end);
		std::streamsize szFile = ifsFile.tellg();

		return szFile;
	}

	std::streamsize FileX::GetFileSize(const char* msFile)
	{
		std::ifstream ifsFile(msFile, std::ios::binary);
		if (!ifsFile.is_open()) return false;

		ifsFile.seekg(0, std::ios::end);
		std::streamsize szFile = ifsFile.tellg();

		return szFile;
	}
}

