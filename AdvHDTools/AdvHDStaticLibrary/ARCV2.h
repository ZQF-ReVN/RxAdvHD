#pragma once
#include <string>
#include <vector>
#include <fstream>

#include "ARC_Struct.h"
#include "..\TDA\AutoBuffer.h"


namespace AdvHDStaticLibrary
{
	class ARCV2 : public TDA::AutoBuffer
	{
	private:
		std::wstring m_wsArc;
		std::ifstream m_ifsArc;
		std::wstring m_wsBaseFolder;
		ARCV2_Struct::ARCHeader_V2 m_Header;
		std::vector<ARCV2_Struct::ARCResEntry_V2> m_vecEntry;

	private:
		bool SaveResFile(ARCV2_Struct::ARCResEntry_V2& Entry);
		char* ReadResData(ARCV2_Struct::ARCResEntry_V2& Entry);
		char* ReadResFile(std::wstring wsResName, size_t szRes);

		bool ReadIndex();
		size_t CreateIndex(std::vector<std::wstring>& vecFileList);

	public:
		ARCV2();
		ARCV2(std::wstring wsArc);
		~ARCV2();

		std::vector<ARCV2_Struct::ARCResEntry_V2>& GetIndex() { return m_vecEntry; }

		bool OpenPack();
		void SetPackName(std::wstring wsArc);
		void SetBaseFolder(std::wstring wsFolder);

		void PrintIndex_Console();
		bool PrintIndex_File(std::wstring wsFile);

		bool Create(std::wstring wsPack, std::wstring wsFolder);
		bool Extract(std::wstring wsPack, std::wstring wsFolder);
	};
}