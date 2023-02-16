#include "ARCV1.h"
#include "..\TDA\FileX.h"
#include "..\TDA\StringX.h"

#include <Windows.h>
#include <iostream>


namespace AdvHDStaticLibrary
{
	ARCV1::ARCV1() :
		m_pBuffer(nullptr), m_szAllocMax(0), m_Header({ 0 })
	{

	}

	ARCV1::ARCV1(std::wstring wsArc) :
		m_pBuffer(nullptr), m_szAllocMax(0), m_Header({ 0 })
	{
		SetPackName(wsArc);
	}

	ARCV1::~ARCV1()
	{
		if (m_pBuffer)
		{
			delete[] m_pBuffer;
		}
	}

	bool ARCV1::Extract(std::wstring wsArc, std::wstring wsFolder)
	{
		SetPackName(wsArc);
		SetBaseFolder(wsFolder);
		if (!(OpenPack() && ReadIndex())) return false;

		size_t count = 0;
		std::string mFileName;
		std::wstring wFileName;
		ARCV1_Struct::ARCResEntry_V1* pEntry = nullptr;
		for (auto& type : m_vecType)
		{
			for (size_t iteRes = 0; iteRes < type.uiResCount; iteRes++)
			{
				pEntry = &m_vecEntry[count + iteRes];

				//Splicing Suffix
				mFileName = pEntry->aResName;
				mFileName.append(".");
				mFileName.append(type.aTypeName);

				//CVT To Wide String
				TDA::StringX::StrToWStr(mFileName, wFileName, 932);

				//Save
				SaveResFile(*pEntry, wFileName);
			}

			count += type.uiResCount; //Next Type
		}

		return true;
	}

	void ARCV1::SetPackName(std::wstring wsArc)
	{
		m_wsArc = wsArc;
	}

	//Set Extract File Folder or Create Pack Folder
	void ARCV1::SetBaseFolder(std::wstring wsFolder)
	{
		m_wsBaseFolder = wsFolder;
		std::wstring& currentPath = wsFolder;
		TDA::FileX::GetCurrentDirectoryPath(currentPath);
		CreateDirectoryW((currentPath + m_wsBaseFolder).c_str(), NULL);
	}

	bool ARCV1::OpenPack()
	{
		m_ifsArc.open(m_wsArc, std::ios::binary);

		return m_ifsArc.is_open();
	}

	void ARCV1::BufferReSize(size_t szRes)
	{
		if (!m_szAllocMax)
		{
			m_pBuffer = new char[szRes];
			m_szAllocMax = szRes;
		}

		if (szRes > m_szAllocMax)
		{
			delete[] m_pBuffer;
			m_pBuffer = new char[szRes];
			m_szAllocMax = szRes;
		}
	}

	//Read Pack Resource Data To Buffer
	char* ARCV1::ReadResData(ARCV1_Struct::ARCResEntry_V1& Entry)
	{
		BufferReSize(Entry.uiSize);
		m_ifsArc.seekg(Entry.uiOff);
		m_ifsArc.read(m_pBuffer, Entry.uiSize);
		return m_pBuffer;
	}

	//Read File Data To Buffer
	char* ARCV1::ReadResFile(std::wstring wsResName, size_t szRes)
	{
		std::ifstream ifsFile(wsResName, std::ios::binary);
		if (!ifsFile.is_open()) return nullptr;

		BufferReSize(szRes);
		ifsFile.read(m_pBuffer, szRes);
		return m_pBuffer;
	}

	//Save Pack Resource Data To File
	bool ARCV1::SaveResFile(ARCV1_Struct::ARCResEntry_V1& Entry, std::wstring& wsResName)
	{
		if (m_wsBaseFolder.size())
		{
			wsResName = m_wsBaseFolder + L'/' + wsResName;
		}

		std::ofstream oFile(wsResName, std::ios::binary);
		if (!oFile.is_open()) return false;

		oFile.write(ReadResData(Entry), Entry.uiSize);
		oFile.flush();

		return true;
	}

	bool ARCV1::ReadIndex()
	{
		//Read Header
		m_ifsArc.read(reinterpret_cast<char*>(&m_Header), sizeof(m_Header));

		//Read Types
		ARCV1_Struct::ARCResType_V1 type = { 0 };
		for (size_t iteTypes = 0; iteTypes < m_Header.uiTypeCount; iteTypes++)
		{
			m_ifsArc.read(reinterpret_cast<char*>(&type), sizeof(type));
			m_vecType.emplace_back(type);
		}

		//Read Index(Entries)
		ARCV1_Struct::ARCResEntry_V1 entry = { 0 };
		for (auto& type : m_vecType)
		{
			for (size_t iteEntry = 0; iteEntry < type.uiResCount; iteEntry++)
			{
				m_ifsArc.read(reinterpret_cast<char*>(&entry), sizeof(entry));
				m_vecEntry.emplace_back(entry);
			}
		}

		return true;
	}

	bool ARCV1::CreateIndex()
	{
		return true;
	}
}