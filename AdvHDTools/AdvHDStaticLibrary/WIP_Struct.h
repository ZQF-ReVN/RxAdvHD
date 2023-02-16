#pragma once


namespace AdvHDStaticLibrary
{
	namespace WIP_Struct
	{
		/*
		* WIP File
		*	WIPHeader
		*	WIPFrame[WIPHeader.usFrames]
		*	Data
		*/
		struct WIPHeader
		{
			char aSignature[4];         // WIPF
			unsigned short usFrames;    // Count WIP File
			unsigned short usBpp;       // Bits Per Pixel
		};

		struct WIPFrame
		{
			unsigned int uiHeigh;
			unsigned int uiWidth;
			unsigned int uiX;
			unsigned int uiY;
			unsigned int uiZ;
			unsigned int uiSize;
		};
	}
}