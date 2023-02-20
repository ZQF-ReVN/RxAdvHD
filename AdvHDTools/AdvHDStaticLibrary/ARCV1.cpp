#include "ARCV1.h"
#include "..\TDA\FileX.h"
#include "..\TDA\StringX.h"
#include "..\TDA\EnumFiles.h"

#include <Windows.h>
#include <iostream>


namespace AdvHDStaticLibrary
{
	ARCV1::ARCV1() : m_Header({ 0 })
	{

	}

	ARCV1::ARCV1(std::string msArc) : m_Header({ 0 })
	{
		SetPackName(msArc);
	}

	ARCV1::~ARCV1()
	{

	}

	bool ARCV1::Extract(std::string msArc, std::string msFolder)
	{
		SetPackName(msArc);
		SetBaseFolder(msFolder);
		if (!(OpenPack() && ReadIndex())) return false;

		std::string mResName;
		size_t resCounted = 0;
		ARCV1_Struct::ARCResEntry_V1* pEntry = nullptr;
		for (auto& iteType : m_vecType)
		{
			for (size_t resCount = 0; resCount < iteType.uiResCount; resCount++)
			{
				pEntry = &m_vecEntry[resCounted + resCount];

				//Splicing Extension
				mResName = pEntry->aResName;
				mResName.append(".");
				mResName.append(iteType.aTypeName);

				//Save
				SaveResFile(*pEntry, mResName);
			}

			resCounted += iteType.uiResCount; //Next Type
		}

		return true;
	}

	bool ARCV1::Create(std::string msArc, std::string msFolder)
	{
		//Set Pack Name
		SetPackName(msArc);

		//Create Pack File
		std::ofstream oArc(m_msArc, std::ios::binary);
		if (!oArc.is_open()) return false;

		//Set Work Folder
		SetBaseFolder(msFolder);

		//Get Files Name
		TDA::EnumFilesA enumRes(m_msBaseFolder);

		//Create Index
		if (!CreateIndex(enumRes.GetAllFilesName())) return false;

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
		size_t resCounted = 0;
		ARCV1_Struct::ARCResEntry_V1* pEntry = nullptr;
		for (auto& iteType : m_vecType)
		{
			for (size_t resCount = 0; resCount < iteType.uiResCount; resCount++)
			{
				pEntry = &m_vecEntry[resCounted + resCount];

				resName = pEntry->aResName;
				resName.append(".");
				resName.append(iteType.aTypeName);

				oArc.write(ReadResFile(resName, pEntry->uiSize), pEntry->uiSize);
			}

			resCounted += iteType.uiResCount; // Next Type
		}

		oArc.flush();

		return true;
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

	bool ARCV1::FindType(std::string& msExtension)
	{
		for (auto& iteType : m_vecType)
		{
			if (msExtension.find(iteType.aTypeName) == 0)
			{
				iteType.uiResCount++;
				return true;
			}
		}

		return false;
	}

	size_t ARCV1::CreateType(std::vector<std::string>& vecResList)
	{
		//Create Type
		std::string resExtension;
		ARCV1_Struct::ARCResType_V1 type = { 0 };
		for (auto& res : vecResList)
		{
			if (res[res.size() - 4] != '.') return false;
			resExtension = res.substr(res.size() - 3);
			if (resExtension.size() != 3) return false;

			if (!FindType(resExtension))
			{
				type.aTypeName[0] = resExtension[0];
				type.aTypeName[1] = resExtension[1];
				type.aTypeName[2] = resExtension[2];
				type.uiResCount = 1;
				m_vecType.emplace_back(type);
			}

			memset(&type, 0x0, sizeof(type));
		}

		//Init Type Index
		size_t indexOff = sizeof(m_Header) + sizeof(ARCV1_Struct::ARCResType_V1) * m_vecType.size();
		for (auto& iteType : m_vecType)
		{
			iteType.uiIndexOff = indexOff;
			indexOff += iteType.uiResCount * sizeof(ARCV1_Struct::ARCResEntry_V1); // Next Type Index
		}

		return indexOff;
	}

	bool ARCV1::CreateIndex(std::vector<std::string>& vecResList)
	{
		size_t dataOff = CreateType(vecResList);

		std::string resName;
		ARCV1_Struct::ARCResEntry_V1 entry = { 0 };
		for (auto& iteType : m_vecType)
		{
			for (auto& res : vecResList)
			{
				if (res.find(iteType.aTypeName, res.size() - 4) == std::string::npos) continue;
				resName = res.substr(0, res.size() - 4);
				if (resName.size() > 9) return false;

				memcpy(entry.aResName, resName.data(), resName.size());
				entry.uiOff = dataOff;
				entry.uiSize = static_cast<size_t>(TDA::FileX::GetFileSize((m_msBaseFolder + '/' + res).c_str()));
				m_vecEntry.emplace_back(entry);

				dataOff += entry.uiSize;
				memset(&entry, 0x0, sizeof(entry));
			}
		}

		//Init Header
		m_Header.uiTypeCount = m_vecType.size();

		return true;
	}
}