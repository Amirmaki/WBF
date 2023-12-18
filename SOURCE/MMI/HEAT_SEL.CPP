#ifndef __HTCVWIND_H
#include "htcvwind.h"
#endif
#ifndef __WINSTRUC_H
#include "winstruc.h"
#endif
#ifndef _DATABASE_H
#include "database.h"
#endif
#ifndef __CONFIG_H
#include "config.h"
#endif
#ifndef __LANGUAGE_H
#include "language.h"
#endif

void Heat_Select()
{
	if(*init==INITIAL)
	{
		*init=NORMAL;
		Vue_Precedente=REHEAT_CONTROL;
		APPELHS ("Utilise,HEATSEL.AID");
		APPELHS ("Fusionne,logo_cc.img,1,2");
		APPELHS ("Fusionne,heat_scr.img,3,13");
		APPELHS ("Fusionne,targ_ttl.img,1,38");

		APPELHS ("Option,Couleur,S=15");    // couleur saisie (fond*16+caractere)
		APPELHS ("Option,Couleur,A=240");
		APPELHS ("Option,verticale,N");
		APPELHS ("Sortie,ESC=V,RET=V,HOM=V,END=V,FLH=M,FLB=D,FLD=S,FLG=P,TF1=V,TF4=V,TF6=V,TF7=V,TF8=V,TF9=V,TF0=V");
		Heat_Buf=new char[sizeof(LObj)];
		Heat_InitWindow();
		Heat_SetupWindow();

	}
	APPELHS ("Ecran,Saisie/nonblocant");

	if (!strcmp(HsTouche,"FLB"))
		Heat_Down();

	if (!strcmp(HsTouche,"FLH"))
		Heat_Up();

	if(!strcmp(HsTouche,"TF1"))
	{
		*init=INITIAL;
#ifdef DBG_VUE
		fstream f;
		f.open("vue.txt", ios::in);
		f >> vue;
		f.close();
#endif
		Vue_Precedente=vue;
		vue = HELP_SCREEN;
#ifdef DBG_VUE
		f.open("vue.txt", ios::out);
		f << vue;
		f.close();
#endif
	}

	if (!strcmp(HsTouche,"TF6"))
		Heat_Del();

	if (!strcmp(HsTouche,"TF7"))
		Heat_PgUp();

	if (!strcmp(HsTouche,"TF8"))
		Heat_PgDown();

	if (!strcmp(HsTouche,"END"))
		Heat_End();

	if (!strcmp(HsTouche,"HOM"))
		Heat_Home();


	if(!strcmp(HsTouche,"TF4") || !strcmp(HsTouche,"TF0"))
	{
		delete Heat_Buf;
		Sortie_Ecran(HsTouche);
	}
}
