#include "ARCV1.h"
#include "..\TDA\FileX.h"
#include "..\TDA\StringX.h"
#include "..\TDA\EnumFiles.h"

#include <Windows.h>
#include <iostream>


namespace AdvHDStaticLibrary
{
	ARCV1::ARCV1() :
		m_pBuffer(nullptr), m_szAllocMax(0), m_Header({ 0 })
	{

	}

	ARCV1::ARCV1(std::string msArc) :
		m_pBuffer(nullptr), m_szAllocMax(0), m_Header({ 0 })
	{
		SetPackName(msArc);
	}

	ARCV1::~ARCV1()
	{
		if (m_pBuffer)
		{
			delete[] m_pBuffer;
		}
	}

	bool ARCV1::Extract(std::string msArc, std::string msFolder)
	{
		SetPackName(msArc);
		SetBaseFolder(msFolder);
		if (!(OpenPack() && ReadIndex())) return false;

		size_t count = 0;
		std::string mFileName;
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

				//Save
				SaveResFile(*pEntry, mFileName);
			}

			count += type.uiResCount; //Next Type
		}

		return true;
	}

	bool ARCV1::Create(std::string msArc, std::string msFolder)
	{
		if (!OpenPack()) return false;

		SetPackName(msArc);
		SetBaseFolder(msFolder);

		//Get Files Name
		TDA::EnumFilesA enumFile(m_msBaseFolder);
		std::vector<std::string>& vecFilesNameList = enumFile.GetAllFilesName();

		return CreateIndex(vecFilesNameList);
	}

	void ARCV1::SetPackName(std::string msArc)
	{
		m_msArc = msArc;
	}

	//Set Extract File Folder or Create Pack Folder
	void ARCV1::SetBaseFolder(std::string msFolder)
	{
		m_msBaseFolder = msFolder;
		std::string& currentPath = msFolder;
		TDA::FileX::GetCurrentDirectoryPath(currentPath);
		CreateDirectoryA((currentPath + m_msBaseFolder).c_str(), NULL);
	}

	bool ARCV1::OpenPack()
	{
		m_ifsArc.open(m_msArc, std::ios::binary);

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
	char* ARCV1::ReadResFile(std::string msResName, size_t szRes)
	{
		std::ifstream ifsFile(m_msBaseFolder + '/' + msResName, std::ios::binary);
		if (!ifsFile.is_open()) return nullptr;

		BufferReSize(szRes);
		ifsFile.read(m_pBuffer, szRes);
		return m_pBuffer;
	}

	//Save Pack Resource Data To File
	bool ARCV1::SaveResFile(ARCV1_Struct::ARCResEntry_V1& Entry, std::string& msResName)
	{
		if (m_msBaseFolder.size())
		{
			msResName = m_msBaseFolder + '/' + msResName;
		}

		std::ofstream oFile(msResName, std::ios::binary);
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

	bool ARCV1::FindType(std::string& msSuffix)
	{
		for (auto& iteType : m_vecType)
		{
			if (msSuffix.find(iteType.aTypeName) == 0)
			{
				iteType.uiResCount++;
				return true;
			}
		}

		return false;
	}

	bool ARCV1::CreateIndex(std::vector<std::string>& vecFileList)
	{
		//Create Pack
		std::ofstream oArc(m_msArc, std::ios::binary);
		if (!oArc.is_open()) return false;

		//Create Type
		std::string fileSuffix;
		ARCV1_Struct::ARCResType_V1 type = { 0 };
		for (auto& file : vecFileList)
		{
			//Processing File Suffix
			if (file[file.size() - 4] != '.') return false;
			fileSuffix = file.substr(file.size() - 3);
			if (fileSuffix.size() != 3) return false;

			//Processing Type
			if (!FindType(fileSuffix))
			{
				type.aTypeName[0] = fileSuffix[0];
				type.aTypeName[1] = fileSuffix[1];
				type.aTypeName[2] = fileSuffix[2];
				type.uiResCount = 1;
				m_vecType.emplace_back(type);
			}
		}

		//Init Type Index
		size_t indexOff = sizeof(m_Header) + sizeof(ARCV1_Struct::ARCResType_V1) * m_vecType.size();
		for (auto& iteType : m_vecType)
		{
			iteType.uiIndexOff = indexOff;
			indexOff += iteType.uiResCount * sizeof(ARCV1_Struct::ARCResEntry_V1); // Next Type Index
		}

		//Create Index(Entries)
		size_t dataOff = indexOff;
		std::string fileName;
		ARCV1_Struct::ARCResEntry_V1 entry = { 0 };
		for (auto& iteType : m_vecType)
		{
			for (auto& file : vecFileList)
			{
				if (file.find(iteType.aTypeName, file.size() - 4) == std::string::npos) continue;
				fileName = file.substr(0, file.size() - 4);
				if (fileName.size() > 9) return false;

				memcpy(entry.aResName, fileName.data(), fileName.size());
				entry.uiOff = dataOff;
				entry.uiSize = static_cast<size_t>(TDA::FileX::GetFileSize((m_msBaseFolder + '/' + file).c_str()));
				m_vecEntry.emplace_back(entry);

				dataOff += entry.uiSize;
				memset(&entry, 0x0, sizeof(entry));
			}
		}

		//Init Header
		m_Header.uiTypeCount = m_vecType.size();


		//Write Header
		oArc.write(reinterpret_cast<char*>(&m_Header), sizeof(m_Header));

		//Write Types
		for (auto& iteType : m_vecType)
		{
			oArc.write(reinterpret_cast<char*>(&iteType), sizeof(iteType));
		}

		//Write Index
		for (auto& iteEntry : m_vecEntry)
		{
			oArc.write(reinterpret_cast<char*>(&iteEntry), sizeof(iteEntry));
		}

		//Write Res
		std::string resName;
		size_t countRes = 0;
		ARCV1_Struct::ARCResEntry_V1* pEntry = nullptr;
		for (auto& iteType : m_vecType)
		{
			for (size_t countEntry = 0; countEntry < iteType.uiResCount; countEntry++)
			{
				pEntry = &m_vecEntry[countRes + countEntry];

				resName = pEntry->aResName;
				resName.append(".");
				resName.append(iteType.aTypeName);

				oArc.write(ReadResFile(resName, pEntry->uiSize), pEntry->uiSize);
			}

			countRes += iteType.uiResCount;
		}

		oArc.flush();

		return true;
	}
}