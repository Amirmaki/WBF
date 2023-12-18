#ifndef __ZHIS_H
#define __ZHIS_H
#endif

void DispCurves_FillBuffer();
void DispCurves_SaveData();
void DispCurves_LoadData();
void DispCurves_Init();

struct TypeDispCurves
{
	char Recup[1];
	char PV[(NBZONE-1)];
	char SP[(NBZONE-1)];
	char Qa[(NBZONE-1)];
	char Qg[(NBZONE-1)];
};

#ifdef __OWNER_ZHIS_CFG_H
TypeDispCurves DispCurves;
#else
extern TypeDispCurves DispCurves;
#endif