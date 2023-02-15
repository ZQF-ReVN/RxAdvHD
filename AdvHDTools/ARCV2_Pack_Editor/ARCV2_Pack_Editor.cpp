#include <iostream>

#include "..\AdvHDStaticLibrary\ARCV2.h"

using namespace AdvHDStaticLibrary;


int main()
{
	ARCV2 arc;
	arc.Extract(L"Rio1.arc",L"Rio1");
	//arc.Create(L"out.arc", L"Rio1");
	//arc.SetBaseFolder(L"SysGraphic");
	//arc.SetPackName(L"out.arc");
	//arc.Create();
}