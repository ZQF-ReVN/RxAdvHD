#pragma once
#include <string>
#include <vector>
#include <fstream>

#include "ARC_Struct.h"


namespace AdvHDStaticLibrary
{
	class ARCV2
	{
	private:
		char* m_pBuffer;
		size_t m_szAllocMax;
		std::wstring m_wsArc;
		std::ifstream m_ifsArc;
		std::wstring m_wsBaseFolder;
		ARC_Struct::ARCHeader_V2 m_Header;
		std::vector<ARC_Struct::ARCResEntry_V2> m_vecEntry;

	private:
		void BufferReSize(size_t szRes);
		bool SaveResFile(ARC_Struct::ARCResEntry_V2& Entry);
		char* ReadResData(ARC_Struct::ARCResEntry_V2& Entry);
		char* ReadResFile(std::wstring wsResName, size_t szRes);

		bool ReadIndex();
		size_t CreateIndex(std::vector<std::wstring>& vecFileList);

	public:
		ARCV2();
		ARCV2(std::wstring wsArc);
		~ARCV2();

		std::vector<ARC_Struct::ARCResEntry_V2>& GetIndex() { return m_vecEntry; }

		bool OpenPack();
		void SetPackName(std::wstring wsPack);
		void SetBaseFolder(std::wstring wsFolder);

		void PrintIndex_Console();
		bool PrintIndex_File(std::wstring wsFile);

		bool Create(std::wstring wsPack, std::wstring wsFolder);
		bool Extract(std::wstring wsPack, std::wstring wsFolder);
	};
}