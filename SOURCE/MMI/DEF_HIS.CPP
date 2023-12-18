#ifndef __CONFIG_H
#include "config.h"
#endif
#include "furndata.h"
#include "V10data1.h"
#include <fstream.h>

void Def_His()
{
	if(*init == INITIAL)
	{
		APPELHS("Mode,16");
		APPELHS("Utilise,Black.aid");
		V10Init();
		Furn_NewHistDisp = 1;
//		VGDispViewNb   = 9;
		V10DispZorP    = 0;
		for (int i = 0; i < 7 ;i++)
			V10DispPCurveNb[i]=-1;


		V10DispPCurveNb[0]=0;
		V10DispPCurveNb[1]=1;
		V10DispPCurveNb[2]=2;
		V10DispPCurveNb[3]=3;
		V10DispPCurveNb[4]=4;
		V10DispPCurveNb[5]=5;

		i=0;
		for (i = 0; i < 7;i++)
			V10DispPCi[i]=1;

		VGInitScreen();
		Furn_Init();
		Furn_Display();

		Furn_NewHistDisp = 0;
		*init=NORMAL;
		APPELHS ("Sortie,FLD=V,FLG=V,FLB=V,FLH=V,TF1=V,TF2=V,TF4=V,TF7=V,TF8=V,TF9=V,TF0=V");
	}


	APPELHS("Ecran,Saisie/nonblocant");

	if (!strcmp(HsTouche,"FLD"))
	{
		clearKbBuffer();
		Furn_UpdateGraphCursor(Right,1);
		clearKbBuffer();
	}

	if (!strcmp(HsTouche,"FLG"))
	{
		clearKbBuffer();
		Furn_UpdateGraphCursor(Left,-1);
	}

	if (!strcmp(HsTouche,"TF4")||!strcmp(HsTouche,"TF0"))
	{
		Furn_Erase();
		closegraph();
		*init = INITIAL;   //Supprim‚ FH 26/11/98 pour ‚viter plantage
		vue   = MAIN_MENU;                            //FH
		fstream f;
		f.open("vue.txt", ios::out);
		f << vue;
		f.close();
		APPELHS("Ferme");                            //FH
		APPELHS("Mode,18");
      //Sortie_Ecran(HsTouche);//Rajout‚ FH 26/11/98
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


	if (!strcmp(HsTouche,"TF2"))
	{
		Furn_UpdateGraphCursor(Right,1);
		Save_Dates();
		clearKbBuffer();
	}
}

