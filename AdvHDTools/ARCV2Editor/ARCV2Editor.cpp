#include <iostream>

#include "..\AdvHDStaticLibrary\ARCV2.h"

using namespace AdvHDStaticLibrary;

void ARCV2Editor()
{
	wchar_t op = 0;
	std::wstring pack;
	std::wstring folder;

	std::wcout
		<< L"Input E Extract Pack\n"
		<< L"Input P Create Pack\n" << std::endl;

	while (true)
	{
		std::wcout << L"Input:";
		std::wcin >> op;

		switch (op)
		{
		case L'E':
		{
			std::wcout << L"Input Pack Name:";
			std::wcin >> pack;

			std::wcout << L"Input Folder Name:";
			std::wcin >> folder;

			ARCV2 arc;
			if (arc.Extract(pack, folder))
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
			std::wcout << L"Input Pack Name:";
			std::wcin >> pack;

			std::wcout << L"Input Folder Name:";
			std::wcin >> folder;

			ARCV2 arc;
			if (arc.Create(pack, folder))
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

		std::wcout << std::endl;
	}
}

int main()
{
	ARCV2Editor();
}