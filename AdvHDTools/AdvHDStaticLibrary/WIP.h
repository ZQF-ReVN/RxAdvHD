#pragma once
#include <string>
#include <fstream>
#include <vector>

#include "WIP_Struct.h"
#include "..\TDA\AutoBuffer.h"


namespace AdvHDStaticLibrary
{
	class WIP : public TDA::AutoBuffer
	{
	private:
		size_t m_szWIP;
		std::wstring m_wsWIP;
		std::ifstream m_ifsWIP;
		WIP_Struct::WIPHeader m_Header;
		std::vector<WIP_Struct::WIPEntry> m_vecEntry;

	private:
		bool OpenWIP(std::wstring wsWIP);
		bool InitIndex();
		bool SaveFrame(std::wstring wsFrame, size_t szPos, size_t szFrame, WIP_Struct::WIPEntry& Frame);
		bool ReadEntry(std::wstring wsFramePath, WIP_Struct::WIPEntry& Frame);
		char* ReadFrame(std::wstring wsFramePath, size_t szFrame);
		wchar_t* MakeName(size_t nSequence);

	public:
		WIP();
		~WIP();

		bool SplitFrame(std::wstring wsWIP);
		bool MergeFrame(std::wstring wsFolder);
		bool Decode(std::wstring wsWIP);
	};

	class WIPDecoder : public  TDA::AutoBuffer
	{
	public:
		WIPDecoder();
		~WIPDecoder();

		size_t DecodePixelArray(char* pEnc, char** ppDec, size_t nWidth, size_t nHeigh, size_t nChannel);
		void MergeChannel(char* pSplit, char* pMerged, size_t nWidth, size_t nHeigh, size_t nChannel);

	};
}