 #define __OWNER_MAIN_MNU_H
 #include "config.h"
 #include "language.h"
 #include <dir.h>
 #include "prhislst.h"
 #include "globdat.h"

void Main_Menu()
{
	if ( *init ==INITIAL)
	{
		*init = NORMAL;

		APPELHS ("Utilise,MAINMNU.AID");

		APPELHS ("Fusionne,logo_cc.img,1,2 ");
		APPELHS ("Fusionne,main_m.img ,1,34");
		APPELHS ("Fusionne,opti.img   ,8,23");

		APPELHS ("Sortie,ESC=V,RET=V,TF1=V,TF2=V,TF5=V,TF6=V,TF7=V,TF8=V,TF9=V");
	}
	APPELHS ("Ecran,Saisie/nonblocant");

	if(!strcmp(HsTouche,"TF1"))
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
		vue = HELP_SCREEN;
#ifdef DBG_VUE
		f.open("vue.txt", ios::out|ios::app);
		f << vue << endl;
		f.close();
#endif
	}

	if(!strcmp(HsTouche,"TF2"))
	{
		*init=INITIAL;

		if (Valid==0)
			vue = CONFIG_PASSWORD;
		else
			vue = CONFIG_MENU;
#ifdef DBG_VUE
		fstream f;
		f.open("vue.txt", ios::out|ios::app);
		f << vue << "MainF2" << endl;
		f.close();
#endif
	}

	if(!strcmp(HsTouche,"TF5"))
	{
		*init=INITIAL;
#ifdef DBG_VUE
		fstream f;
/*		f.open("vue.txt", ios::in);
		f >> vue;
		f.close();
*/
		f.open("vue.txt", ios::out|ios::app);
		f << vue << "MainF5" << endl;
		f.close();
#endif
		Vue_Precedente=vue;
		vue = PROD_SEL;
#ifdef DBG_VUE
		f.open("vue.txt", ios::out|ios::app);
		f << vue << "MainF5" << endl;
		f.close();
#endif
	}

	if(!strcmp(HsTouche,"TF6"))
	{
		*init=INITIAL;
		vue = REHEAT_CONTROL;
#ifdef DBG_VUE
		fstream f;
		f.open("vue.txt", ios::out|ios::app);
		f << vue << "MainF6" << endl;
		f.close();
#endif
	}

	if(!strcmp(HsTouche,"TF7"))
	{
		int filename ,fileid;

		if(getNewestHisFile(&filename,&fileid)>0)
		{
			*init=INITIAL;
			vue = PROD_HIS;
#ifdef DBG_VUE
			fstream f;
			f.open("vue.txt", ios::out|ios::app);
			f << vue << "MainF7" << endl;
			f.close();
#endif
		}
	}

	if(!strcmp(HsTouche,"TF8"))
	{
		FILE *fp;
		char filename[MAXPATH];
		sprintf(filename,"%s%s",DB_Path_hisfour,ZONEHIST);

		if((fp=fopen(filename,"r")) != NULL)
		{
			fclose(fp);
			*init=INITIAL;
			vue = FURNACE_HIS;
#ifdef DBG_VUE
			fstream f;
			f.open("vue.txt", ios::out|ios::app);
			f << vue << "MainF8" << endl;
			f.close();
#endif
		}
	}

	if(!strcmp(HsTouche,"TF9"))
	{
		FILE *fp;
		char filename[MAXPATH];
		sprintf(filename,"%s%s",DB_Path_hisfour,DEFHIST);

		if((fp=fopen(filename,"r")) != NULL)
		{
			fclose(fp);
			*init=INITIAL;
			vue = DEF_HIS;
#ifdef DBG_VUE
			fstream f;
			f.open("vue.txt", ios::out|ios::app);
			f << vue << "MainF9" << endl;
			f.close();
#endif
		}
	}

	if(!strcmp(HsTouche,"ESC"))
		{
		vue=QUIT;
#ifdef DBG_VUE
		fstream f;
		f.open("vue.txt", ios::out|ios::app);
		f << vue << "MainEsc" << endl;
		f.close();
#endif
		}
}

