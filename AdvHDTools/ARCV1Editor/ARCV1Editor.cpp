#include <iostream>

#include "..\AdvHDStaticLibrary\ARCV1.h"

using namespace AdvHDStaticLibrary;

void ARCV1Editor()
{
	char op = 0;
	std::string pack;
	std::string folder;

	std::wcout
		<< L"Input E Extract Pack\n"
		<< L"Input P Create Pack\n" << std::endl;

	while (true)
	{
		std::wcout << L"Input:";
		std::cin >> op;

		switch (op)
		{
		case L'E':
		{
			std::wcout << L"Input Pack Name:";
			std::cin >> pack;

			std::wcout << L"Input Folder Name:";
			std::cin >> folder;

			ARCV1 arc;
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
			std::cin >> pack;

			std::wcout << L"Input Folder Name:";
			std::cin >> folder;

			ARCV1 arc;
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
	ARCV1Editor();
}