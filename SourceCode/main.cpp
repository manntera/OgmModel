#include<iostream>
#include"OgmData\OgmData.h"

#define _CRTDBG_MAP_ALLOC
int main(void)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	OGL::PmxData pmx;
	pmx.LoadFile("puronama/ÉvÉçê∂ÇøÇ·ÇÒ.pmx");
	OGL::OgmData ogm;
	OGL::OgmData ogmm;
	ogm.ImportFromModelData(&pmx);
	ogm.ExportFile("puronama.ogm");
	ogmm.ImportFromModelFile("puronama.ogm");
	return 0;
}