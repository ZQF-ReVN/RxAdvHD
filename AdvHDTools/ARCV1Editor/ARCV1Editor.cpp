#include <iostream>

#include "..\AdvHDStaticLibrary\ARCV1.h"

using namespace AdvHDStaticLibrary;

int main(int argc, char* argv[])
{
	if (argc > 1)
	{
		switch (*argv[1])
		{
		case L'E':
		{
			ARCV1 arc;
			if (arc.Extract(argv[2], argv[3]))
			{
				std::wcout << L"Pack Extracted\n";
			}
			else
			{
				std::wcout << L"Extract Pack Failed!\n";
			}
		}
		break;

		case L'P':
		{
			ARCV1 arc;
			if (arc.Create(argv[2], argv[3]))
			{
				std::wcout << L"Pack Created\n";
			}
			else
			{
				std::wcout << L"Create Pack Failed!\n";
			}
		}
		break;

		default:
			std::wcout << L"Command Error!\n" << std::endl;
		}
	}
	else
	{
		std::wcout
			<< L"Command:\n"
			<< L"E Extract, P Pack\n"
			<< L"ARCV1Editor.exe [Command] [PackName] [FolderName]\n\n"
			<< L"Example:\n"
			<< L"ARCV1Editor.exe E Chip5.arc Chip5\n"
			<< L"ARCV1Editor.exe P Bgm.arc Bgm\n"
			<< std::endl;
	}
}