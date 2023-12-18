 #define __OWNER_CONF_MNU_H
 #ifndef __CONFIG_H
 #include "config.h"
 #endif
 #ifndef __LANGUAGE_H
 #include "language.h"
 #endif
 #ifndef __PASSWORD_H
 #include "password.h"
 #endif
#include <fstream.h>

void Config_Password()
{
	char Password[21];
	int i=0;

	if ( *init == INITIAL)
	{
		*init=NORMAL;
		Vue_Precedente=MAIN_MENU;
		APPELHS ("Utilise,CFGMENU.AID");
		APPELHS ("Option,Couleur,S=E");    // couleur saisie (fond*16+caractere)
		APPELHS ("Option,Couleur,A=E");

		APPELHS ("Fusionne,logo_cc.img,1,2");
		APPELHS ("Fusionne,cfg_menu.img,1,28");
		APPELHS ("Fusionne,pass.img,9,27");

		APPELHS ("Sortie,TF1=V,TF4=V,TF5=V,TF6=V,TF7=V,TF8=V,TF9=V,TF0=V");
	}
	APPELHS("Saisie,PASSWORD/nonblocant");

	if(!strcmp(HsTouche,"RET"))
	{
		strcpy(Password,HsChaine);

		while(Password[i]!=' ' && i < 20)
			i++;

		if (i <9)
		{
			Password[i]=0;
			Valid = getPassWord(Password);
		}
	}

	if (Valid==1)
		{
		vue = CONFIG_MENU;
#ifdef DBG_VUE
		fstream f;
		f.open("vue.txt", ios::out|ios::app);
		f << vue << endl;
		f.close();
#endif
		}

	if(!strcmp(HsTouche,"TF1"))
	{
		*init=INITIAL;
#ifdef DBG_VUE
		fstream f;
/*		f.open("vue.txt", ios::in);
		f >> vue;
		f.close();
*/		f.open("vue.txt", ios::out|ios::app);
		f << vue << endl;
		f.close();
#endif
		Vue_Precedente=vue;
		vue = HELP_SCREEN;
#ifdef DBG_VUE
		f.open("vue.txt", ios::out|ios::app);
		f << vue << endl;
		f.close();
#endif
	}

	if (!strcmp(HsTouche,"TF0") || !strcmp(HsTouche,"TF4"))
			Sortie_Ecran(HsTouche);
}

void Config_Menu()
{
	if ( *init == INITIAL)
	{
		*init=NORMAL;
		Vue_Precedente=MAIN_MENU;
		APPELHS ("Utilise,CFGMENU.AID");
		APPELHS ("Option,Couleur,S=E");    // couleur saisie (fond*16+caractere)
		APPELHS ("Option,Couleur,A=E");

		APPELHS ("Fusionne,logo_cc.img,1,2");
		APPELHS ("Fusionne,cfg_menu.img,1,28");
		APPELHS ("Fusionne,pass.img,9,27");

		APPELHS ("Sortie,TF1=V,TF4=V,TF5=V,TF6=V,TF7=V,TF8=V,TF9=V,TF0=V");
	}

	APPELHS("Ecran,Saisie/nonblocant");

	if(!strcmp(HsTouche,"TF1"))
	{
		*init=INITIAL;
#ifdef DBG_VUE
		fstream f;
/*		f.open("vue.txt", ios::in);
		f >> vue;
		f.close();
*/		f.open("vue.txt", ios::out|ios::app);
		f << vue << endl;
		f.close();
#endif
		Vue_Precedente=vue;
		vue = HELP_SCREEN;
#ifdef DBG_VUE
		f.open("vue.txt", ios::out|ios::app);
		f << vue << endl;
		f.close();
#endif
	}

	if(!strcmp(HsTouche,"TF6"))
	{
		*init=INITIAL;
#ifdef DBG_VUE
		fstream f;
/*		f.open("vue.txt", ios::in);
		f >> vue;
		f.close();
*/		f.open("vue.txt", ios::out|ios::app);
		f << vue << endl;
		f.close();
#endif
		Vue_Precedente=vue;
		vue = FURNACE_CONFIG;
#ifdef DBG_VUE
		f.open("vue.txt", ios::out|ios::app);
		f << vue << endl;
		f.close();
#endif
	}

	if (!strcmp(HsTouche,"TF7"))
	{
		*init=INITIAL;
#ifdef DBG_VUE
		fstream f;
/*		f.open("vue.txt", ios::in);
		f >> vue;
		f.close();
*/
		f.open("vue.txt", ios::out|ios::app);
		f << vue << endl;
		f.close();
#endif
		Vue_Precedente=vue;
		vue = MODEL_CONFIG;
#ifdef DBG_VUE
		f.open("vue.txt", ios::out|ios::app);
		f << vue << endl;
		f.close();
#endif
	}

	if (!strcmp(HsTouche,"TF8"))
	{
		*init=INITIAL;
#ifdef DBG_VUE
		fstream f;
/*		f.open("vue.txt", ios::in);
		f >> vue;
		f.close();
*/
		f.open("vue.txt", ios::out|ios::app);
		f << vue << endl;
		f.close();
#endif
		Vue_Precedente=vue;
		vue = PRODUCTS_CONFIG;
#ifdef DBG_VUE
		f.open("vue.txt", ios::out|ios::app);
		f << vue << endl;
		f.close();
#endif
	}
	if (!strcmp(HsTouche,"TF9"))
	{
		*init=INITIAL;
#ifdef DBG_VUE
		fstream f;
/*		f.open("vue.txt", ios::in);
		f >> vue;
		f.close();
*/
		f.open("vue.txt", ios::out|ios::app);
		f << vue << endl;
		f.close();
#endif
		Vue_Precedente=vue;
		vue = HEAT_CONFIG;
#ifdef DBG_VUE
		f.open("vue.txt", ios::out|ios::app);
		f << vue << endl;
		f.close();
#endif
	}

	if (!strcmp(HsTouche,"TF0") || !strcmp(HsTouche,"TF4"))
		Sortie_Ecran(HsTouche);
}