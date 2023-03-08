#include <iostream>

#include "../../Modules/AdvHDStaticLibrary/WS2.h"

using namespace AdvHDStaticLibrary;

int wmain(int argc, wchar_t* argv[])
{
	if (argc > 1)
	{
		if (*argv[1] == L'D')
		{
			if (WS2::Dec(argv[2]))
			{
				std::wcout << L"Decoded\n";
			}
			else
			{
				std::wcout << L"Error!\n";
			}
		}

		if (*argv[1] == L'E')
		{
			if (WS2::Enc(argv[2]))
			{
				std::wcout << L"Encoded\n";
			}
			else
			{
				std::wcout << L"Error!\n";
			}
		}
	}
	else
	{
		std::wcout
			<< L"Command:\n"
			<< L"D Dec, E Enc\n"
			<< L"WS2DecEnc.exe [Command] [FileName]\n\n"
			<< L"Example:\n"
			<< L"WS2DecEnc.exe D 01_01.ws2\n"
			<< L"WS2DecEnc.exe E 01_01.ws2\n"
			<< L"WS2DecEnc.exe D K0708.WSC\n"
			<< L"WS2DecEnc.exe E K0708.WSC\n"
			<< std::endl;
	}
}