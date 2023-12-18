 #include <stdlib.h>
 #define __OWNER_CONFIG_H
 #ifndef __MODWIND_H
 #include "modwind.h"
 #endif
 #ifndef __WINSTRUC_H
 #include "winstruc.h"
 #endif
 #ifndef __FURNWIND_H
 #include "furnwind.h"
 #endif
 #ifndef __TPDTWIND_H
 #include "tpdtwind.h"
 #endif
 #ifndef __HTCVWIND_H
 #include "htcvwind.h"
 #endif
 #ifndef __DELAYWND_H
 #include "delaywnd.h"
 #endif
 #ifndef __DATABASE_H
 #include "database.h"
 #endif
 #ifndef __GLOBDAT_H
 #include "globdat.h"
 #endif
 #ifndef __CONFIG_H
 #include "config.h"
 #endif
 #ifndef __LANGUAGE_H
 #include "language.h"
 #endif
 #ifndef __GLOBVIEW_H
 #include "globview.h"
 #endif
//-------------------------------------------------------------------------
//clear text screen and init graphic driver
void VGInitScreen()
{
	clrscr();
	//init the graphic driver
	int gdriver = EGA, gmode = EGAHI;       //EGA EGAHI
	initgraph(&gdriver,&gmode,"\\BGI");
	int erreur = graphresult();

	if (erreur)
	{
		printf("Graphics error: %s\n", grapherrormsg(erreur));
		printf("Press any key to halt:");
		getch();
	}
	else
		//graphic mode
		VGDispMode = 0;
} //end VGInitScreen

//----------------------------------------------------------------------------
void clearKbBuffer(void)
{
//	CLI();
	*(int far *)MK_FP(0x40,0x1a)=*(int far *)MK_FP(0x40,0x1c);
//	STI();
}

//----------------------------------------------------------------------------
void Abandon(char *Touche)
{
	APPELHS("Ferme");
	int newvue = MAIN_MENU;
#ifdef DBG_VUE
	fstream f;
/*	f.open("vue.txt", ios::in);
	f >> vue;
	f.close();
*/	f.open("vue.txt", ios::out||ios::app);
	f << vue << "Ab" << endl;
	f.close();
#endif
	switch(vue)
		{
		case FURNACE_CONFIRM: 	newvue=FURNACE_CONFIG;	break;
		case MODEL_CONFIRM:		newvue=MODEL_CONFIG;   	break;
		case PRODUCT_CONFIRM:	newvue=PRODUCTS_CONFIG;	break;
		case HEAT_CONFIRM:		newvue=HEAT_CONFIG;		break;
		}
	vue = newvue;
#ifdef DBG_VUE
	f.open("vue.txt", ios::out|ios::app);
	f << vue << "Ab" << endl;
	f.close();
#endif
	if (!strcmp (Touche,"TF0") || !strcmp (Touche,"TF4"))
		Sortie_Ecran(Touche);
}

//----------------------------------------------------------------------------
void Sauvegarde(char *Touche)
{
	int Choix = 0, i;

	APPELHS ("Ecran,Recupere,CHOIX");
	Choix = HsEntier;

	APPELHS("Ferme");
	int newvue = MAIN_MENU;
#ifdef DBG_VUE
	fstream f;
/*	f.open("vue.txt", ios::in);
	f >> vue;
	f.close();
*/
	f.open("vue.txt", ios::out|ios::app);
	f << vue << "ConfSauv1" << endl;
#endif
	switch(vue)
	{
//----------------------- Traitement Furnace Config --------------------------
		case FURNACE_CONFIRM:
			Furn_StoreData();

			if (Choix==2)
				Furn_SaveInFile();
//			f << "SaveOK" << endl;
			memcpy(Furn_Buf,&Four,sizeof(Four));
//			f << "MemCpyOK" << endl;
			newvue = FURNACE_CONFIG;
			break;

//----------------------- Traitement Model Config ----------------------------
		case MODEL_CONFIRM:
			Model_StoreData();

		for (i=0;i<NBZONE;i++)
			sprintf(ModelCopy.KEpsiSup[i],"%d",10*atoi(ModelCopy.KEpsiSup[i]));

			if (Choix==2)
				Model_SaveInFile();

			memcpy(Model_Buf,&ModelCopy,sizeof(ModelCopy));
			newvue = MODEL_CONFIG;
			break;

//----------------------- Traitement Product Config --------------------------
		case PRODUCT_CONFIRM:
			if (Choix==1)
					Prod_StoreData(Prod_IndexList);
			else
					Prod_SaveInFile();

			if (error < 103)
				{
				memcpy(Prod_Buf,&TProd,sizeof(TProd));
				}
			newvue = PRODUCTS_CONFIG;
			break;

//----------------------- Traitement Heat Curves Config ----------------------
		case HEAT_CONFIRM:

			if (Choix==1)
				Heat_StoreData(Heat_IndexList);
			else
				Heat_SaveInFile();

			if (error < 103)
				{
				memcpy(Heat_Buf,&LObj,sizeof(LObj));
				}
			newvue = HEAT_CONFIG;
			break;
	}
	vue = newvue;
#ifdef DBG_VUE
	f << vue << "ConfSauv2" << endl;
	f.close();
#endif
//--------------------- Sortie Ecran -----------------------------------------
	if (!strcmp (Touche,"TF0") || !strcmp (Touche,"TF4"))
		Sortie_Ecran(Touche);
}

//----------------------------------------------------------------------------
void Confirm()
{
	char *uneTouche;

	if ( *init ==INITIAL)
		{
		*init = NORMAL;

		APPELHS("Ecran,Touche");
		strcpy(uneTouche,HsTouche);

		APPELHS ("Ouvre,message3.AID");
		APPELHS ("Option,Couleur,A=E");
#ifdef DBG_VUE
		fstream f;
/*		f.open("vue.txt", ios::in);
		f >> vue;
		f.close();
*/		f.open("vue.txt", ios::out|ios::app);
		f << vue << "Conf" << endl;
		f.close();
#endif
		switch(vue)
			{
			case FURNACE_CONFIRM:	APPELHS ("Affiche,TITRE=%s",Title1);	break;
			case MODEL_CONFIRM:		APPELHS ("Affiche,TITRE=%s",Title2);
											APPELHS ("Option,verticale");				break;
			case PRODUCT_CONFIRM:	APPELHS	("Affiche,TITRE=%s",Title3);	break;
			case HEAT_CONFIRM:		APPELHS	("Affiche,TITRE=%s",Title4);	break;
			}
		APPELHS ("Affiche,MSG,1=%s",EnregTemp);
		APPELHS ("Affiche,MSG,2=%s",EnregDef);
		APPELHS("Affiche,CHOIX=1");
		}
	APPELHS ("Ecran,Saisie/nonblocant");

	if (!strcmp (HsTouche,"RET"))
		Sauvegarde(uneTouche);

	if (!strcmp (HsTouche,"ESC"))
		Abandon(uneTouche);
}

//----------------------------------------------------------------------------
void Teste_Buffer(char *Touche)
{
	char *uneTouche;

//	APPELHS("Ecran,Touche");
	strcpy(uneTouche,Touche);
//	int newvue=QUIT;
#ifdef DBG_VUE
	fstream f;
/*	f.open("vue.txt", ios::in);
	f >> vue;
	f.close();
*/	f.open("vue.txt", ios::out|ios::app);
	f << vue << "TstB1" << endl;
	f.close();
#endif
	switch(vue)
		{
//------------------ Teste Le Buffer de la vue FURNACE_CONFIG ----------------

		case FURNACE_CONFIG:  	Furn_FillBuffers();
			if (memcmp(Furn_Buf,&Four,sizeof(Four)))
				{
				*init = INITIAL;
				vue = FURNACE_CONFIRM;
				}
			else if ( !strcmp(uneTouche,"TF0") || !strcmp(uneTouche,"TF4"))
				Sortie_Ecran(uneTouche);
			else vue = FURNACE_CONFIG;
			break;

//------------------ Teste Le Buffer de la vue MODEL_CONFIG ------------------

		case MODEL_CONFIG:	Model_FillBuffers();
			if (memcmp(Model_Buf,&ModelCopy,sizeof(ModelCopy)))
				{
				*init=INITIAL;
				vue = MODEL_CONFIRM;
				}
			else if ( !strcmp(uneTouche,"TF0") || !strcmp(uneTouche,"TF4"))
				Sortie_Ecran(uneTouche);
			else vue = MODEL_CONFIG;
			break;

//------------------ Teste Le Buffer de la vue PRODUCTS_CONFIG ---------------

		case PRODUCTS_CONFIG:	//Prod_FillBuffers();
			if (memcmp(Prod_Buf,&TProd,sizeof(TProd)))
				{
				*init=INITIAL;
				vue = PRODUCT_CONFIRM;
				}
			else if ( !strcmp(uneTouche,"TF0") || !strcmp(uneTouche,"TF4"))
				Sortie_Ecran(uneTouche);
			else vue = PRODUCTS_CONFIG;
			break;

//------------------ Teste Le Buffer de la vue HEAT_CONFIG -------------------

		case HEAT_CONFIG:	Heat_FillBuffers();
			if (memcmp(Heat_Buf,&LObj,sizeof(LObj)))
				{
				*init = INITIAL;
				vue = HEAT_CONFIRM;
				}
			else if ( !strcmp(uneTouche,"TF0") || !strcmp(uneTouche,"TF4"))
				Sortie_Ecran(uneTouche);
			else vue = HEAT_CONFIG;
			break;
		}
#ifdef DBG_VUE
	f.open("vue.txt", ios::out|ios::app);
	f << vue << "TstB2" << endl;
	f.close();
#endif
}

//--------------------------------------------------------------------------
void Sortie_Ecran(char *Touche)
{
	char *uneTouche;

	*init=INITIAL;
	strcpy(uneTouche,Touche);
#ifdef DBG_VUE
	fstream f;
/*	f.open("vue.txt", ios::in);
	f >> vue;
	f.close();
*/ f.open("vue.txt", ios::out|ios::app);
	f << vue << "SrtE1" << endl;
	f.close();
#endif
	switch(vue)
		{
		case FURNACE_CONFIG: 	delete Furn_Buf; 	break;
		case MODEL_CONFIG:		delete Model_Buf;	break;
		case PRODUCTS_CONFIG:	//TriBulle();
										delete Prod_Buf;	break;
		case HEAT_CONFIG:			//Heat_TriBulle();
										delete Heat_Buf;	break;
		}

	if (!strcmp(uneTouche,"TF0"))
		{
		switch(vue)
			{
			case DELAY:
			case PACING:
			case HEAT_SELECT: vue = REHEAT_CONTROL; break;
			case PROD_HIS:
			case FURNACE_HIS:
			case DEF_HIS:
			case REHEAT_CONTROL: vue = MAIN_MENU;
			default:	vue = CONFIG_MENU;
			}
		}
	else
		vue = MAIN_MENU;
#ifdef DBG_VUE
	f.open("vue.txt", ios::out|ios::app);
	f << vue << "SrtE2" << endl;
	f.close();
#endif
}

//--------------------------------------------------------------------------
void File_Error(int error,char *FileName)
{
	struct date d;
	struct time t;
	char msg_err[80];

	getdate(&d);
	gettime(&t);

	switch(error)
	{
		case READING_ERROR:	strcpy(msg_err, ErrRead);		break;
		case OPENING_ERROR:  strcpy(msg_err, ErrOpen);		break;
		case UPDATING_ERROR:	strcpy(msg_err, ErrUpDate);	break;
		case SAVE_ERROR: 	  	strcpy(msg_err, ErrSave); 		break;
	}

	FILE *fp;
	fpos_t FilePos;

	if((fp=fopen("err_msg.lst","r+"))== NULL)
	{
		fp = fopen("err_msg.lst","w+");
		fprintf(fp,"0000\n");
	}
	else
	{
		fscanf(fp,"%ld",&FilePos);

		if(FilePos < 238)  //5806
			fsetpos(fp, &FilePos);
		else
		{
			rewind(fp);
			fprintf(fp,"0000\n");
		}
	}

	fprintf(fp,"%02d-%02d-%d\t%02d:%02d:%02d\t%s\t%s\n", d.da_day, d.da_mon, d.da_year,
					t.ti_hour, t.ti_min, t.ti_sec, msg_err, FileName);

	fgetpos(fp,&FilePos);
	rewind(fp);

	fprintf(fp,"%0004ld\n",FilePos);
	fclose(fp);
}

//--------------------------------------------------------------------------
void Window_Error()
{
	char Error_Msg[80]="";

	if ( *init==INITIAL)
	{
		*init = NORMAL;
		APPELHS ("Ouvre,message2.AID");
		APPELHS ("Option,Couleur,A=E");

		switch(error)
		{
			case DELETE_ERROR:		APPELHS ("Affiche,ERROR=%s",ErrUpDate);
											APPELHS ("Affiche,MESSAGE,1=%s",strncpy(Error_Msg,ErrDelete,22));
											APPELHS ("Affiche,MESSAGE,2=%s",strcpy (Error_Msg,ErrDelete+22));  	break;

			case SAME_ERROR:	 		APPELHS ("Affiche,ERROR=%s",ErrSame);
											APPELHS ("Affiche,MESSAGE,1=%s",strncpy(Error_Msg,DelOne,17));
											APPELHS ("Affiche,MESSAGE,2=%s",strcpy (Error_Msg,DelOne+17)); 		break;

			case UNVALID_ERROR:		APPELHS ("Affiche,ERROR=%s",ErrDelay);
											APPELHS ("Affiche,MESSAGE,1=%s",strncpy(Error_Msg,UnvalidDelay,13));
											APPELHS ("Affiche,MESSAGE,2=%s",strcpy (Error_Msg,UnvalidDelay+13)); break;

			case RECORDING_ERROR:	APPELHS ("Affiche,ERROR=%s",ErrRecord);
											APPELHS ("Affiche,MESSAGE,2=%s",strcpy (Error_Msg,TooDelays)); break;

			case UNDEFINED_ERROR:	APPELHS ("Affiche,ERROR=%s",ErrDelaysDel);
											APPELHS ("Affiche,MESSAGE,1=%s",strncpy(Error_Msg,DelayNotDefined,18));
											APPELHS ("Affiche,MESSAGE,2=%s",strcpy (Error_Msg,DelayNotDefined+18)); break;

			case LOADING_ERROR:		APPELHS ("Affiche,ERROR=%s",ErrLoadingDelay);
											APPELHS ("Affiche,MESSAGE,1=%s",strncpy(Error_Msg,NoDelayProgr,9));
											APPELHS ("Affiche,MESSAGE,2=%s",strcpy (Error_Msg,NoDelayProgr+9)); break;

			case NBCURVE_ERROR:		APPELHS ("Affiche,ERROR=%s",ErrNbCurve);
											APPELHS ("Affiche,MESSAGE,1=%s",strncpy(Error_Msg,TooCurves,19));
											APPELHS ("Affiche,MESSAGE,2=%s",strcpy (Error_Msg,TooCurves+19)); break;
		}
	}
	APPELHS("Ecran,Saisie/nonblocant");

	if (!strcmp(HsTouche,"RET"))
	{
		APPELHS("Ferme");
		error = 0;

		if (Temp ==0)	vue = HEAT_CONFIG;
		if (Temp ==1)	vue = PRODUCTS_CONFIG;
		if (Temp ==2)	vue = DELAY;
		if (Temp ==3)	vue = ZHIS_CONFIG;
#ifdef DBG_VUE
		fstream f;
		f.open("vue.txt", ios::out|ios::app);
		f << vue << "WinEr" << endl;
		f.close();
#endif
	}
}

//--------------------------------------------------------------------------
void Confirm_OverWrite()
{
	fstream f;
	if ( *init == INITIAL)
	{
		*init=NORMAL;
		APPELHS ("Ouvre,MESSAGE.AID");
		APPELHS ("Option,Couleur,A=E");

		APPELHS ("Affiche,TITRE=%s",WarningTitle);
		APPELHS ("Affiche,FINMESSAGE=%s",OverWriteQst);
	}
	APPELHS("Ecran,Saisie/nonblocant");

	if(!strcmp(HsTouche,"RET"))
	{
			APPELHS("Ferme");

			Delay_EnregSous=0;//RAZ de l'indicateur de chargement (fnct load)
			(void)DBdelete(DB_Delays_Item,List_Delay,"");
			Delay_LoadData();
			Delay_Continue();
			vue = DELAY;
#ifdef DBG_VUE
			f.open("vue.txt", ios::out|ios::app);
			f << vue << "ConfOvr1" << endl;
			f.close();
#endif
	}

	if(!strcmp(HsTouche,"ESC"))
	{
		Delay_EnregSous=0;//RAZ de l'indicateur de chargement (fnct load)
		APPELHS("Ferme");
		vue = DELAY;
#ifdef DBG_VUE
		f.open("vue.txt", ios::out|ios::app);
		f << vue << "ConfOvr2" << endl;
		f.close();
#endif
	}
}
