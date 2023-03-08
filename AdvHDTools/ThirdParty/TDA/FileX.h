#pragma once
#include <string>
#include <fstream>


namespace TDA
{
	class FileX
	{
	public:
		static size_t      GetCurrentDirectoryPath(char* pPath);
		static size_t      GetCurrentDirectoryPath(wchar_t* pPath);
		static size_t      GetCurrentDirectoryPath(std::string& msPath);
		static size_t      GetCurrentDirectoryPath(std::wstring& wsPath);
		static void        BackSlash(char* pPath);
		static void        BackSlash(wchar_t* pPath);
		static void        BackSlash(const char* pPath, std::string& msPath);
		static void        BackSlash(const wchar_t* pPath, std::wstring& wsPath);
		static bool        PathRemoveFileName(std::wstring& wsPath, std::wstring& wsRemoved);

		static std::streamsize GetFileSize(const wchar_t* wsFile);
		static std::streamsize GetFileSize(const char* msFile);

		template <typename FSTREAM>
		static std::streamsize GetFileSize(FSTREAM& fsFile)
		{
			std::streamsize szFile = 0;
			std::streampos oldPos = fsFile.tellg();

			fsFile.seekg(0, std::ios::end);
			szFile = fsFile.tellg();
			fsFile.seekg(oldPos, std::ios::beg);

			return szFile;
		}
	};
}

