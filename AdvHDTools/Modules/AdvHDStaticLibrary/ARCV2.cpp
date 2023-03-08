#include "ARCV2.h"
#include "../../ThirdParty/TDA/FileX.h"
#include "../../ThirdParty/TDA/StringX.h"
#include "../../ThirdParty/TDA/EnumFiles.h"

#include <iostream>
#include <Windows.h>


namespace AdvHDStaticLibrary
{
	ARCV2::ARCV2() : m_Header({ 0 })
	{

	}

	ARCV2::ARCV2(std::wstring wsArc) : m_Header({ 0 })
	{
		SetPackName(wsArc);
	}

	ARCV2::~ARCV2()
	{

	}

	inline bool ARCV2::OpenPack()
	{
		m_ifsArc.open(m_wsArc, std::ios::binary);

		return m_ifsArc.is_open();
	}

	inline void ARCV2::SetPackName(std::wstring wsArc)
	{
		m_wsArc = wsArc;
	}

	//Set Extract File Folder or Create Pack Folder
	void ARCV2::SetBaseFolder(std::wstring wsFolder)
	{
		m_wsBaseFolder = wsFolder;
		std::wstring& currentPath = wsFolder;
		TDA::FileX::GetCurrentDirectoryPath(currentPath);
		CreateDirectoryW((currentPath + m_wsBaseFolder).c_str(), NULL);
	}

	//Read Pack Resource Data To Buffer
	inline char* ARCV2::ReadResData(ARCV2_Struct::ARCResEntry_V2& Entry)
	{
		BufferReSize(Entry.uiSize);
		m_ifsArc.seekg(Entry.uiOff + sizeof(m_Header) + m_Header.uiIndexSize);
		m_ifsArc.read(m_pBuffer, Entry.uiSize);
		return m_pBuffer;
	}

	//Read File Data To Buffer
	inline char* ARCV2::ReadResFile(std::wstring wsResName, size_t szRes)
	{
		std::ifstream ifsFile(wsResName, std::ios::binary);
		if (!ifsFile.is_open()) return nullptr;

		BufferReSize(szRes);
		ifsFile.read(m_pBuffer, szRes);
		return m_pBuffer;
	}

	//Save Pack Resource Data To File
	inline bool ARCV2::SaveResFile(ARCV2_Struct::ARCResEntry_V2& Entry)
	{
		std::wstring file;
		if (m_wsBaseFolder.size())
		{
			file = m_wsBaseFolder + L'/' + Entry.aResName;
		}
		else
		{
			file = Entry.aResName;
		}
		std::ofstream oFile(file, std::ios::binary);
		if (!oFile.is_open()) return false;

		oFile.write(ReadResData(Entry), Entry.uiSize);

		oFile.flush();

		return true;
	}

	bool ARCV2::ReadIndex()
	{
		if (!m_ifsArc.is_open()) return false;

		//Init Header
		m_ifsArc.read(reinterpret_cast<char*>(&m_Header), sizeof(m_Header));

		//Read Index(Entries) To Buffer
		char* pIndex = new char[m_Header.uiIndexSize];
		m_ifsArc.read(pIndex, m_Header.uiIndexSize);

		//Init Entries
		char* pEntries = pIndex;
		ARCV2_Struct::ARCResEntry_V2 pEntry = { 0 };
		for (size_t iteEntry = 0; iteEntry < m_Header.uiResCount; iteEntry++)
		{
			pEntry.uiSize = *(unsigned int*)(pEntries + 0);
			pEntry.uiOff = *(unsigned int*)(pEntries + 4);
			wcscpy_s(pEntry.aResName, 0xFF, (wchar_t*)(pEntries + 8));

			m_vecEntry.emplace_back(pEntry);

			pEntries += ((wcslen(pEntry.aResName) + 1) * 2) + 8;
		}

		delete[] pIndex;

		return true;
	}

	//Pure File Name. If Need Path Call SetBaseFolder
	size_t ARCV2::CreateIndex(std::vector<std::wstring>& vecFileList)
	{
		size_t szIndex = 0;
		std::streamsize szData = 0;
		ARCV2_Struct::ARCResEntry_V2 entry = { 0 };

		for (auto& file : vecFileList)
		{
			if (m_wsBaseFolder.size())
			{
				entry.uiSize = static_cast<size_t>(TDA::FileX::GetFileSize((m_wsBaseFolder + L'/' + file).c_str()));
			}
			else
			{
				entry.uiSize = static_cast<size_t>(TDA::FileX::GetFileSize(file.c_str()));
			}

			entry.uiOff = static_cast<size_t>(szData);
			wcscpy_s(entry.aResName, 0xFF, file.c_str());

			m_vecEntry.emplace_back(entry);

			szData += entry.uiSize;
			szIndex += ((wcslen(entry.aResName) + 1) * 2) + 8;
		}

		return szIndex;
	}

	void ARCV2::PrintIndex_Console()
	{
		std::wcout << L"Count:" << m_vecEntry.size() << L"\n\n";
		for (auto& entry : m_vecEntry)
		{
			std::wcout
				<< L"FileName:" << entry.aResName << L'\n'
				<< L"FileSize:" << std::hex << L"0x" << entry.uiSize << L'\n'
				<< L"FileOff :" << std::hex << L"0x" << entry.uiOff << L"\n\n";
		}
	}

	bool ARCV2::PrintIndex_File(std::wstring wsFile)
	{
		std::wofstream oInfo(wsFile);
		oInfo.imbue(TDA::StringX::GetCVT_UTF8());
		if (!oInfo.is_open()) return false;

		oInfo << L"Count:" << m_vecEntry.size() << L"\n\n";
		for (auto& entry : m_vecEntry)
		{
			oInfo << L"FileName:" << entry.aResName << L'\n';
			oInfo << L"FileSize:" << std::hex << L"0x" << entry.uiSize << L'\n';
			oInfo << L"FileOff :" << std::hex << L"0x" << entry.uiOff << L"\n\n";
		}

		oInfo.flush();

		return true;
	}

	bool ARCV2::Create(std::wstring wsPack, std::wstring wsFolder)
	{
		SetBaseFolder(wsFolder);
		SetPackName(wsPack);

		//Get Files Name
		TDA::EnumFilesW enumFile(m_wsBaseFolder);
		std::vector<std::wstring>& vecFilesNameList = enumFile.GetAllFilesName();

		//Init Header & Index
		m_Header.uiIndexSize = CreateIndex(vecFilesNameList);
		m_Header.uiResCount = m_vecEntry.size();

		//Create Pack File
		std::ofstream oArc(m_wsArc, std::ios::binary);
		if (!oArc.is_open()) return false;

		//Write Header
		oArc.write(reinterpret_cast<char*>(&m_Header), sizeof(m_Header));

		//Write Index
		for (auto& entry : m_vecEntry)
		{
			oArc.write(reinterpret_cast<char*>(&entry), 8);
			oArc.write(reinterpret_cast<char*>(entry.aResName), static_cast<std::streamsize>((wcslen(entry.aResName) + 1) * 2));
		}

		//Write Data
		for (auto& entry : m_vecEntry)
		{
			oArc.write(ReadResFile(m_wsBaseFolder + L'/' + entry.aResName, entry.uiSize), entry.uiSize);
		}

		oArc.flush();
		return true;
	}

	bool ARCV2::Extract(std::wstring wsPack, std::wstring wsFolder)
	{
		SetBaseFolder(wsFolder);
		SetPackName(wsPack);

		if (!OpenPack()) return false;
		if (!ReadIndex()) return false;

		for (auto& entry : m_vecEntry)
		{
			SaveResFile(entry);
		}

		return true;
	}

}