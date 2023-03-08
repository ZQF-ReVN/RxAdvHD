#pragma once


namespace AdvHDStaticLibrary
{
	void LZSS_AdvHD_Decode(char* pEnc, char* pDec);
	size_t Fake_AdvHD_Encode(char* pRaw, size_t szRaw, char* pEnc);
}