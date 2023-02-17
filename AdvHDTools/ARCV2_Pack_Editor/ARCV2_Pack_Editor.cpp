#include <iostream>

//#include "..\AdvHDStaticLibrary\ARCV2.h"
#include "..\AdvHDStaticLibrary\ARCV1.h"
//#include "..\AdvHDStaticLibrary\WS2.h"

using namespace AdvHDStaticLibrary;


int main()
{
	ARCV1 arc1;
	arc1.Create("Voice.arc", "Voice");
	//arc1.Extract("Voice.arc", "Voice");
	//WS2::Enc(L"01_02.ws2");
	//WS2::Enc(L"01_01.ws2");
	//ARCV2 arc;
	//arc.Extract(L"Rio1.arc",L"Rio1");
	//arc.Create(L"out.arc", L"Rio1");
	//arc.SetBaseFolder(L"SysGraphic");
	//arc.SetPackName(L"out.arc");
	//arc.Create();
}