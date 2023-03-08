#include <iostream>
#include "../../Modules/AdvHDStaticLibrary/WIP.h"

using namespace AdvHDStaticLibrary;


int wmain(int argc, wchar_t* argv[])
{
	if (argc > 1)
	{
		WIP wip;
		switch (*argv[1])
		{
		case L'S':
		{
			wip.SplitFrame(argv[2]);
		}
		break;

		case L'M':
		{
			wip.MergeFrame(argv[2]);
		}
		break;

		case L'D':
		{
			wip.Decode(argv[2]);
		}
		break;

		case L'E':
		{
			wip.Encode(argv[2], argv[3]);
		}
		break;

		}
	}
	else
	{
		std::wcout
			<< L"Command:\n"
			<< L"S - SplitFrame\n"
			<< L"M - MergeFrame\n"
			<< L"D - Decode WIP Only Sigle Frame\n"
			<< L"E - Encode WIP Only Sigle Frame\n\n";

		std::wcout 
			<< argv[0] << L" S cfgbg.wip\n"
			<< argv[0] << L" M cfgbg\n"
			<< argv[0] << L" D cfgbg#0000.wip\n"
			<< argv[0] << L" E cfgbg#0000.wip cfgbg#0000.wip.webp\n\n";
	}


}