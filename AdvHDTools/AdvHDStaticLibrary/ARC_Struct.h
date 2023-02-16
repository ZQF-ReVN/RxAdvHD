#pragma once


namespace AdvHDStaticLibrary
{
	namespace ARCV1_Struct
	{
		/*
        * ARC Pack File V1
        *	Header
        *	Type[ARCHeader.uiTypeCount]
        *	Entry[??]
        *	Data
        */
		struct ARCHeader_V1
		{
			unsigned int uiTypeCount;
		};

		struct ARCResType_V1
		{
			char aTypeName[4];
			unsigned int uiResCount;
			unsigned int uiIndexOff;    // (Pack File Offset)
		};

        #pragma pack(1)
		struct ARCResEntry_V1
		{
			char aResName[0x9];         // (Null Character)
			unsigned int uiSize;
			unsigned int uiOff;         // (Pack File Offset)
		};
        #pragma pack()
	}

	namespace ARCV2_Struct
	{
		/*
		* ARC Pack File V2
	    *   Header
	    *   Entry[Header.uiResCount]
	    *   Data
		*/
		struct ARCHeader_V2
		{
			unsigned int uiResCount;
			unsigned int uiIndexSize;
		};

        #pragma pack(1)
		struct ARCResEntry_V2
		{
			unsigned int uiSize;
			unsigned int uiOff;         // (Data Sec Offset)
			wchar_t aResName[0xFF];     // (Null Character)
		};
        #pragma pack()
	}
}