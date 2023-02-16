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
		std::wstring m_wsArc;
		std::ifstream m_ifsArc;
		std::wstring m_wsBaseFolder;
		ARCV1_Struct::ARCHeader_V1 m_Header;
		std::vector<ARCV1_Struct::ARCResType_V1> m_vecType;
		std::vector<ARCV1_Struct::ARCResEntry_V1> m_vecEntry;

	private:
		void SetPackName(std::wstring wsArc);
		void SetBaseFolder(std::wstring wsFolder);
		bool OpenPack();

		void  BufferReSize(size_t szRes);
		char* ReadResData(ARCV1_Struct::ARCResEntry_V1& Entry);
		char* ReadResFile(std::wstring wsResName, size_t szRes);
		bool  SaveResFile(ARCV1_Struct::ARCResEntry_V1& Entry, std::wstring& wsResName);

		bool ReadIndex();
		bool CreateIndex();

	public:
		ARCV1();
		ARCV1(std::wstring wsArc);
		~ARCV1();

		bool Extract(std::wstring wsArc, std::wstring wsFolder);

	};
}