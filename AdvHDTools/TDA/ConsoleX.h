#pragma once
#include <Windows.h>
#include <iostream>


namespace TDA
{
	class ConsoleX
	{
	public:
		static FILE* SetConsole(LPCWSTR lpTitle);
	};
}

