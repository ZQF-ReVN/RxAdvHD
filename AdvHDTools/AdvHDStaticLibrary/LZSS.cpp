#include "LZSS.h"


namespace AdvHDStaticLibrary
{
	void LZSS_Decode(char* pEnc, char* pDec)
	{
		char aSlide[0x1000] = { 0 };
		for (short iteSlide = 1; true;)
		{
			for (char iteToken = 0, nToken = pEnc++[0]; iteToken < 8; iteToken++, nToken >>= 1)
			{
				if ((nToken & 0b00000001) != 0)
				{
					pDec[0] = pEnc++[0];
					aSlide[iteSlide] = pDec++[0];

					iteSlide = ++iteSlide & 0x0FFF;
					continue;
				}

				short offRepate = ((pEnc[0] << 4) & 0x0FFF) + ((pEnc++[1] >> 4) & 0x000F);
				char szRepate = (pEnc++[0] & 0x0F) + 2;
				if (!offRepate) return;

				for (char iteSize = 0; iteSize < szRepate; iteSize++)
				{
					pDec[0] = *(aSlide + offRepate);
					aSlide[iteSlide] = *(pDec++);

					offRepate = ++offRepate & 0x0FFF;
					iteSlide = ++iteSlide & 0x0FFF;
				}
			}
		}
	}

}