#ifndef __CONFIG_H
#include "config.h"
#endif
#ifndef __ZHIS_H
#include "zhis_cfg.h"
#endif
#include "furndata.h"
#define _OWNER_V10DATA1_H
#include "V10data1.h"

#include <fstream.h>

void Furnace_His()
{
	if(*init == INITIAL)
	{
		APPELHS("Utilise,Black.aid");
		V10Init();
		Furn_NewHistDisp = 1;
		V10DispZorP      = 1;

		for (int i = 0; i < 4*maxNbZone+2 ;i++)
			V10DispZCurveNb[i]=-1;

		DispCurves_LoadData();
/*
		for (i = 0; i < 8 ;i++)
			V10DispZCi[i]=-1;
*/
		VGInitScreen();
		Furn_Init();
		Furn_Display();

		Furn_NewHistDisp = 0;
		*init=NORMAL;
		APPELHS ("Sortie,FLD=V,FLG=V,FLB=V,FLH=V,TF1=V,TF2=V,TF3=V,TF4=V,TF7=V,TF8=V,TF9=V,TF0=V");
	}
	APPELHS("Ecran,Saisie/nonblocant");

	if (!strcmp(HsTouche,"FLD"))
	{
		clearKbBuffer();
		Furn_UpdateGraphCursor(Right,1);
	}

	if (!strcmp(HsTouche,"FLG"))
	{
		clearKbBuffer();
		Furn_UpdateGraphCursor(Left,-1);
	}

	if(!strcmp(HsTouche,"TF1"))
	{
		Furn_Erase();
		closegraph();
		*init=INITIAL;
		fstream f;
		f.open("vue.txt", ios::in);
		f >> vue;
		f.close();
		Vue_Precedente=vue;
		vue = HELP_SCREEN;
		f.open("vue.txt", ios::out);
		f << vue;
		f.close();
		APPELHS("Ferme");
		APPELHS("Mode,18");
	}

	if (!strcmp(HsTouche,"TF3"))
	{
		Furn_Erase();
		closegraph();
		*init = INITIAL;
		fstream f;
		f.open("vue.txt", ios::in);
		f >> vue;
		f.close();
		Vue_Precedente=vue;
		vue   = ZHIS_CONFIG;
		f.open("vue.txt", ios::out);
		f << vue;
		f.close();
		APPELHS("Ferme");
		APPELHS("Mode,18");
	}

	if (!strcmp(HsTouche,"TF4")||!strcmp(HsTouche,"TF0"))
	{
		Furn_Erase();
		closegraph();
		APPELHS("Ferme");
//		APPELHS("Mode,18");
		Sortie_Ecran(HsTouche);
	}

	if (!strcmp(HsTouche,"TF2"))
	{
		Furn_UpdateGraphCursor(Right,1);
		Save_Dates();
		clearKbBuffer();
	}
}
