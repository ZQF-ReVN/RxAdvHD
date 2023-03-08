#include "LZSS.h"

#include <memory>


namespace AdvHDStaticLibrary
{
	#define MakeStruct_LZSS(offset,count) (((unsigned short)offset << 4) | ((unsigned short)count & 0x000F))
	#define MakeToken_LZSS(token,flag) (((unsigned char)token ^ (0x1 << (7 - flag))) & (unsigned char)token)

	void LZSS_AdvHD_Decode(char* pRaw, char* pDec)
	{
		char aSlide[0x1000] = { 0 };
		for (short iteSlide = 1; true;)
		{
			for (char iteToken = 0, nToken = pRaw++[0]; iteToken < 8; iteToken++, nToken >>= 1)
			{
				if (nToken & 0b00000001)
				{
					pDec[0] = pRaw++[0];
					aSlide[iteSlide] = pDec++[0];

					iteSlide = ++iteSlide & 0x0FFF;
				}
				else
				{
					short off = ((pRaw[0] << 4) & 0x0FFF) + ((pRaw++[1] >> 4) & 0x000F);
					short len = (pRaw++[0] & 0x0F) + 2;
					if (!off) return;

					while (len-- > 0)
					{
						pDec[0] = *(aSlide + off);
						aSlide[iteSlide] = pDec++[0];

						iteSlide = ++iteSlide & 0x0FFF;
						off = ++off & 0x0FFF;
					}
				}
			}
		}
	}

	//I'm tired of writing this LZSS compression algorithm
	//So I choose to give up
	size_t Fake_AdvHD_Encode(char* pRaw, size_t szRaw, char* pEnc)
	{
		char* pEnc_Back = pEnc;
		size_t align = szRaw % 8;
		for (size_t iteRaw = 0; iteRaw < ((szRaw - align) / 8); iteRaw++)
		{
			*(unsigned char*)pEnc = 0xFF;
			memcpy(pEnc + 1, pRaw, 8);

			pEnc += 9;
			pRaw += 8;
		}

		if (align)
		{
			*(unsigned char*)pEnc = 0xFF;
			memcpy(pEnc + 1, pRaw, align);
			pEnc += align + 1;
		}

		pEnc[0] = 0x00;
		pEnc[1] = 0x00;
		pEnc[2] = 0x00;

		return (size_t)(pEnc - pEnc_Back + 3);
	}

/*

LZSS_AdvHD
前置知识：
{
   Slide:
   {
      Slide大小为0x1000，带一个指针iteSlide初始值为1。
      当向解压Buffer写入数据时，同时向Slide写入相同的数据。
      当进行解压长度在0x1000内时Slide与解压出来的数据完全同步。
      当大于0x1000则重置Slide的指针为零，即指向开头。
   }

   Token:
   {
      Token为一个字节，总共八位，用于表示其后数据是否需要展开，即是原始数据还是压缩数据。
   }

   Expand:
   {
      Expand为压缩结构，总共四个nibble(半个字节)
      前三个nibble为，在Slide中的偏移，后一个nibble为该偏移对应字节重复的次数-2。
   }

   Block：
   {
      Block由Token定义，如果Token第N个位为1则Block为原始字节，如果为0则为Expand
      Block大小不确定，最大为16即全部为Expand，最小为8即全部为原始字节也就是不压缩。
   }

}


LZSS Data:
79 FF 00 1F 01 2A 00 00 FF DF 02 20

Token:
开头的79写成二进制（小端序）
{1 0 0 1} {1 1 1 0}
   0x9       0x7

Block:
{1    0       0    1} {1 1  1     0}
FF {00 1F} {01 2A} 00 00 FF DF {02 20}
1     0       0    1  1  1  1     0
FF {00 1F} {01 2A} 00 00 FF DF {02 20}

Token对应到Block中，如果是1则为原始数据，如果是0则为压缩数据
Expand:{00 1F} 压缩数据前三个nibble（半个字节）为Slide的偏移
后一个nibble为长度，即要重复的数据位于Slide[001]长度为 F+2

0x79每次向右移动1bit，对00000001 and运算，即 ((Token >>= 1) & 0x1) ? True:False 判断是否需要展开

对该数据解析：
第一个为 1 不展开
Slide
00 FF
Decode
FF

第二个为 0 要展开，偏移为001，展开大小为F+2 = 11
Slide
0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F  
00 FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF
FF FF FF 
Decode
FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF
FF FF 

第三个为 0 要展开，偏移为012，展开大小为A+2 = C
Slide
0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F  
00 FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF
FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF
Decode
FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF
FF FF FF FF FF FF FF FF FF FF FF FF FF FF

第四-七个为 1 不展开
Slide
0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F  
00 FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF
FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF 00
00 FF DF
Decode
FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF
FF FF FF FF FF FF FF FF FF FF FF FF FF FF 00 00
FF DF

第八个为 0 要展开，偏移022，展开大小为0+2 = 2
Slide
0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F  
00 FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF
FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF 00
00 FF DF DF DF
Decode
FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF
FF FF FF FF FF FF FF FF FF FF FF FF FF FF 00 00
FF DF DF DF

*/

}