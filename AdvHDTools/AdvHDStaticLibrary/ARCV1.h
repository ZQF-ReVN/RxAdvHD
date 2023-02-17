#pragma once
#include <string>
#include <vector>
#include <fstream>

#include "ARC_Struct.h"


namespace AdvHDStaticLibrary
{
	class ARCV1
	{
	private:
		char* m_pBuffer;
		size_t m_szAllocMax;
		std::string m_msArc;
		std::ifstream m_ifsArc;
		std::string m_msBaseFolder;
		ARCV1_Struct::ARCHeader_V1 m_Header;
		std::vector<ARCV1_Struct::ARCResType_V1> m_vecType;
		std::vector<ARCV1_Struct::ARCResEntry_V1> m_vecEntry;

	private:
		void SetPackName(std::string msArc);
		void SetBaseFolder(std::string msFolder);
		bool OpenPack();

		void  BufferReSize(size_t szRes);
		char* ReadResData(ARCV1_Struct::ARCResEntry_V1& Entry);
		char* ReadResFile(std::string msResName, size_t szRes);
		bool  SaveResFile(ARCV1_Struct::ARCResEntry_V1& Entry, std::string& msResName);

		bool  FindType(std::string& msSuffix);

		bool   ReadIndex();
		size_t CreateType(std::vector<std::string>& vecFileList);
		bool   CreateIndex(std::vector<std::string>& vecFileList);

	public:
		ARCV1();
		ARCV1(std::string msArc);
		~ARCV1();

		bool Extract(std::string msArc, std::string msFolder);
		bool Create(std::string msArc, std::string msFolder);

	};
}