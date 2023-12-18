#include "winstruc.h"
#include "database.h"
#ifndef __CONFIG_H
#include "config.h"
#endif
#include "language.h"
#include "ihm.h"
#include "reh_data.h"
#include "globview.h"

time_t last_t;

void Reheat_Control()
{
	if (*init==INITIAL)
		{
		APPELHS("Utilise,Black.aid");
		last_t = time(NULL);
		CreateObjects();
		VGInitScreen();
		Reh_Init();
		//printf("passage dans test 1\n");
		//sleep(1);
		Reh_Display();

		if (TheFurn != NULL)
			DeleteObjects();

		*init = NORMAL;
		APPELHS("Sortie,FLD=V,FLG=V,FLB=V,FLH=V,TF1=V,TF4=V,TF7=V,TF8=V,TF9=V,TF0=V");
		}

	APPELHS("Ecran,Saisie/nonblocant");

	if (time(NULL) >= last_t + 5)
		{
		last_t = time(NULL);
		CreateObjects();
		UpdateView();
		//printf("Passage 2\n");
		if (TheFurn != NULL)
			DeleteObjects();
		}

	if (!strcmp(HsTouche,"FLD"))
		{
		clearKbBuffer();
		Update_Data(Right);
		}

	if (!strcmp(HsTouche,"FLG"))
		{
		clearKbBuffer();
		Update_Data(Left);
		}

	if (!strcmp(HsTouche,"TF4") || !strcmp(HsTouche,"TF0") )
		{
		Reh_Erase();
		closegraph();
		APPELHS("Ferme");
		APPELHS("Mode,18");
		Sortie_Ecran(HsTouche); //Ces instructions peuvent etre indisp ?
		}

	if (!strcmp(HsTouche,"TF1"))
		{
		Reh_Erase();
		closegraph();
		*init = INITIAL;
		fstream f;
		f.open("vue.txt", ios::in);
		f >> vue;
		f.close();
		Vue_Precedente = vue;
		vue = HELP_SCREEN;
		f.open("vue.txt", ios::out);
		f << vue;
		f.close();
		APPELHS("Ferme");
		APPELHS("Mode,18");
		}

	if (!strcmp(HsTouche,"TF7"))
		{
		Reh_Erase();
		closegraph();
		APPELHS("Ferme");
		APPELHS("Mode,18");
		fstream f;
		f.open("vue.txt", ios::in);
		f >> vue;
		f.close();
		Vue_Precedente = vue;
		*init = INITIAL;
		vue = DELAY;
		f.open("vue.txt", ios::out);
		f << vue;
		f.close();
		}

	if (!strcmp(HsTouche,"TF8"))
		{
		Reh_Erase();
		closegraph();
		APPELHS("Ferme");
		APPELHS("Mode,18");
		fstream f;
		f.open("vue.txt", ios::in);
		f >> vue;
		f.close();
		Vue_Precedente = vue;
		*init = INITIAL;
		vue = PACING;
		f.open("vue.txt", ios::out);
		f << vue;
		f.close();
		}

	if (!strcmp(HsTouche,"TF9"))
		{
		Reh_Erase();
		fstream f;
		f.open("vue.txt", ios::in);
		f >> vue;
		f.close();
		Vue_Precedente = vue;
		APPELHS("Ferme");
		APPELHS("Mode,18");
		*init = INITIAL;
		vue = HEAT_SELECT;
		f.open("vue.txt", ios::out);
		f << vue;
		f.close();
		}
}