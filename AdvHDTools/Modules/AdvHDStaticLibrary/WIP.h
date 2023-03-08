#pragma once
#include <string>
#include <fstream>
#include <vector>

#include "WIP_Struct.h"
#include "../../ThirdParty/TDA/AutoBuffer.h"


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
		static wchar_t* MakeName(size_t nSequence);

	public:
		WIP();
		~WIP();

		bool SplitFrame(std::wstring wsWIP);
		bool MergeFrame(std::wstring wsFolder);
		bool Decode(std::wstring wsWIP);
		bool Encode(std::wstring wsWIP, std::wstring wsWebP);
	};

	class WIPDecoder : public  TDA::AutoBuffer
	{
	private:
		char* m_pEncData;
		size_t m_nWidth;
		size_t m_nHeigh;
		size_t m_nChannel;

	public:
		WIPDecoder(char* pEncData, size_t nWidth, size_t nHeigh, size_t nChannel);
		~WIPDecoder();

		char* GetPixelArray();
		size_t GetDecodeSize();
		void MergeChannel(char* pSplit, char* pMerged);

	};

	class WIPEncoder : public TDA::AutoBuffer
	{
	private:
		char* m_pPixelArray;
		size_t m_nWidth;
		size_t m_nHeigh;
		size_t m_nChannel;

	public:
		WIPEncoder(char* pPixelArray, size_t nWidth, size_t nHeigh, size_t nChannel);
		~WIPEncoder();

		char* GetEncodeData(size_t* pEncSize);
		void SplitChannel(char* pMerged, char* pSplit);
	};
}