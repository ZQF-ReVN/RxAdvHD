#include "WS2.h"
#include "..\TDA\FileX.h"

#include <fstream>


namespace AdvHDStaticLibrary
{
	bool WS2::Enc(std::wstring wsWS2)
	{
		std::ifstream iWS2(wsWS2, std::ios::binary);
		if (!iWS2.is_open()) return false;

		std::streamsize szWS2 = TDA::FileX::GetFileSize(iWS2);

		char* pWS2 = new char[static_cast<size_t>(szWS2)];
		iWS2.read(pWS2, szWS2);

		for (size_t iteWS2 = 0; iteWS2 < static_cast<size_t>(szWS2); iteWS2++)
		{
			pWS2[iteWS2] = _rotl8(pWS2[iteWS2], 2);
		}

		std::ofstream oWS2(wsWS2 + L".enc", std::ios::binary);
		if (!oWS2.is_open()) return false;

		oWS2.write(pWS2, szWS2);
		oWS2.flush();
		delete[] pWS2;

		return true;
	}

	bool WS2::Dec(std::wstring wsWS2)
	{
		std::ifstream iWS2(wsWS2, std::ios::binary);
		if (!iWS2.is_open()) return false;

		std::streamsize szWS2 = TDA::FileX::GetFileSize(iWS2);

		char* pWS2 = new char[static_cast<size_t>(szWS2)];
		iWS2.read(pWS2, szWS2);

		for (size_t iteWS2 = 0; iteWS2 < static_cast<size_t>(szWS2); iteWS2++)
		{
			pWS2[iteWS2] = _rotr8(pWS2[iteWS2], 2);
		}

		std::ofstream oWS2(wsWS2 + L".dec", std::ios::binary);
		if (!oWS2.is_open()) return false;

		oWS2.write(pWS2, szWS2);
		oWS2.flush();
		delete[] pWS2;

		return true;
	}
}