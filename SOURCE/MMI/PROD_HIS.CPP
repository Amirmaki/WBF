#ifndef __CONFIG_H
#include "config.h"
#endif
#define _OWNER_Prod_DATA1_H
#include "proddata.h"

void Prod_His()
{
	if(*init==INITIAL)
	{
		APPELHS("Utilise,Black.aid");
		APPELHS ("Option,Couleur,S=15");    //240
		APPELHS ("Option,Couleur,A=15");    //240

		Prod_Init();
		VGInitScreen();

		Prod_NewHispDisp=1;
		Prod_Init();
		Prod_Display();
		*init=NORMAL;
		APPELHS ("Sortie,FLD=V,FLG=V,FLB=V,FLH=V,TF1=V,TF2=V,TF4=V,TF8=V,TF9=V,TF0=V");

		setvisualpage(0);
	}
	APPELHS("Ecran,Saisie/nonblocant");

	if (!strcmp(HsTouche,"FLD"))
	{
		clearKbBuffer();
		Prod_UpdateHPGraphCursor(Right,1);
	}

	if (!strcmp(HsTouche,"FLG"))
	{
		clearKbBuffer();
		Prod_UpdateHPGraphCursor(Left,-1);
	}


	if (!strcmp(HsTouche,"TF4")||!strcmp(HsTouche,"TF0"))
	{
		Prod_Erase();
		closegraph();
		APPELHS("Ferme");
		APPELHS("Mode,18");
		Sortie_Ecran(HsTouche);
	}

	if(!strcmp(HsTouche,"TF1"))
	{
		Prod_Erase();
		closegraph();
		*init=INITIAL;
		fstream f;
		f.open("vue.txt", ios::in);
		f >> vue;
		f.close();
		Vue_Precedente=vue;
		vue  =HELP_SCREEN;
		f.open("vue.txt", ios::out);
		f << vue;
		f.close();
		APPELHS("Ferme");
		APPELHS("Mode,18");
	}

	if (!strcmp(HsTouche,"TF2"))
	{
		Save_Date();
		clearKbBuffer();
	}

	if (!strcmp(HsTouche,"TF8"))
	{
		Previous_Prod();
		clearKbBuffer();
	}
	if (!strcmp(HsTouche,"TF9"))
	{
		Next_Prod();
		clearKbBuffer();
	}
}
