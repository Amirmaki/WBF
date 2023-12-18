#define __OWNER_FURNWIND_H
#ifndef __WINSTRUC_H
#include "winstruc.h"
#endif
#ifndef __FURNWIND_H
#include "furnwind.h"
#endif
#ifndef __DATABASE_H
#include "database.h"
#endif
#ifndef __DB_H
#include "db.h"
#endif
#ifndef __CONFIG_H
#include "config.h"
#endif
#ifndef __LANGUAGE_H
#include "language.h"
#endif

#ifndef __DIR_H
#include <dir.h>
#endif
#ifndef __TIME_H
#include <time.h>
#endif
#ifndef __FSTREAM_H
#include <fstream.h>
#endif

//#define DBG_VUE
//définition des messages

#if LANGUAGE == 1				// Italian
#define MsgFour1	"Alarm:"
#define MsgFour2	"Errore lettura database"
#define MsgFour3	"Errore apertura file"
#define MsgFour4	"Sostituire il file di database corrente ?"
#define MsgFour5	"Configurazione del forno modificata
#endif

#if LANGUAGE == 2				// English
#define MsgFour1	"Alarm:"
#define MsgFour2	"Database Reading Error"
#define MsgFour3	"File Opening Error"
#define MsgFour4	"Save settings in current database ?"
#define MsgFour5	"Furnace configuration modified"
#endif

void Furnace_Config()
{
	char Touche[4];

	if(*init==INITIAL)
	{
		*init=NORMAL;
		Vue_Precedente=CONFIG_MENU;
		APPELHS ("Utilise,THERMCFG.AID");
		APPELHS ("Fusionne,logo_cc.img,1,2");
		APPELHS ("Fusionne,furn_cfg.img,1,32");
		APPELHS ("Fusionne,furn_scr.img,3,1");

		APPELHS ("Option,Couleur,S=240");    // couleur saisie (fond*16+caractere)
		APPELHS ("Option,Couleur,A=240");

		APPELHS ("Option,verticale,N");
		APPELHS ("Sortie,ESC=V,RET=V,FLH=M,FLB=D,TAB=S,TF1=V,TF2=V,TF4=V,TF5=V,TF0=V");

		Furn_Buf = new char[sizeof(Four)];
		Furn_InitWindow();
		Furn_SetupWindow();
	}
	APPELHS ("Ecran,Saisie/nonblocant");

	if (!strcmp(HsTouche,"TF0") || !strcmp(HsTouche,"TF4"))
		Teste_Buffer(HsTouche);

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
		f << vue << "InitF" << endl;
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

	if (!strcmp(HsTouche,"TF2"))
		Teste_Buffer(HsTouche);
}

void Furn_InitWindow()
{
  //initialisation  des données dans la structure
  //********************************************
	strcpy(Four.Length,"0");
	strcpy(Four.Width,"0");
	strcpy (Four.HTop,"0");
	  //	strcpy (Four.HBot,"0");
	strcpy (Four.Step1,"0");
	strcpy (Four.Step2,"0");
	strcpy (Four.WBeamLim,"0");
	strcpy (Four.ZUBeg,"0");
	strcpy (Four.ZUEnd,"0");
	  // strcpy (Four.Alarm,MsgFour1);
	  // MsgFour1=Alarm:

	for (int i=0; i<NBZONE;i++)
	{
		strcpy (Four.EndAbsSup[i],"0");
		strcpy (Four.TcplAbsSup[i],"0");
		strcpy (Four.TcplDepthSup[i],"0");
		strcpy (Four.QGMaxSup[i],"0");
		strcpy (Four.QAMaxSup[i],"0");
		strcpy (Four.SptMinSup[i],"0");
		strcpy (Four.SptMaxSup[i],"0");
		strcpy (Four.SptVarSup[i],"0");
		strcpy (Four.RespTimeSup[i],"0");
	}
  //chargement des données de la base de données
  //********************************************
	Furn_LoadData();

  //  TransferBuffer = (void far*)&(Four);
}

void Furn_SetupWindow()
{
	APPELHS("Affiche,LENGTH=%s",Four.Length);
	APPELHS("Affiche,WIDTH =%s",Four.Width);
	APPELHS("Affiche,HEIGHT=%s",Four.HTop);
	APPELHS("Affiche,STEP1 =%s",Four.Step1);
	APPELHS("Affiche,STEP2 =%s",Four.Step2);
	APPELHS("Affiche,WBEAMLIM =%s",Four.WBeamLim);
	APPELHS("Affiche,ZUBEG =%s",Four.ZUBeg);
	APPELHS("Affiche,ZUEND =%s",Four.ZUEnd);

	for (int i=0; i < NBZONE; i++)
	{
		APPELHS("Affiche,ENDABSCISS,%d=%s",i+1,Four.EndAbsSup[i]);
		APPELHS("Affiche,TCPLABSCIS,%d=%s",i+1,Four.TcplAbsSup[i]);
		APPELHS("Affiche,TCPLDEPTH ,%d=%s",i+1,Four.TcplDepthSup[i]);
		APPELHS("Affiche,QGMAX     ,%d=%s",i+1,Four.QGMaxSup[i]);
		APPELHS("Affiche,QAMAX     ,%d=%s",i+1,Four.QAMaxSup[i]);
		APPELHS("Affiche,SPTMIN    ,%d=%s",i+1,Four.SptMinSup[i]);
		APPELHS("Affiche,SPTMAX    ,%d=%s",i+1,Four.SptMaxSup[i]);
		APPELHS("Affiche,SLOPE     ,%d=%s",i+1,Four.SptVarSup[i]);
		APPELHS("Affiche,RESPTIME  ,%d=%s",i+1,Four.RespTimeSup[i]);
	}
}

//sauvegarde les données de la vue de configuration dans la structure
//*******************************************************************
void Furn_FillBuffers()
{
	APPELHS ("Ecran,Recupere,LENGTH");
	sscanf(HsChaine,"%s",&Four.Length);

	APPELHS ("Ecran,Recupere,WIDTH");
	sscanf(HsChaine,"%s",&Four.Width);

	APPELHS ("Ecran,Recupere,HEIGHT");
	sscanf(HsChaine,"%s",&Four.HTop);

	APPELHS ("Ecran,Recupere,STEP1");
	sscanf(HsChaine,"%s",&Four.Step1);

	APPELHS ("Ecran,Recupere,STEP2");
	sscanf(HsChaine,"%s",&Four.Step2);

	APPELHS ("Ecran,Recupere,WBEAMLIM");
	sscanf(HsChaine,"%s",&Four.WBeamLim);

	APPELHS ("Ecran,Recupere,ZUBEG");
	sscanf(HsChaine,"%s",&Four.ZUBeg);

	APPELHS ("Ecran,Recupere,ZUEND");
	sscanf(HsChaine,"%s",&Four.ZUEnd);

	for (int i=0;i<NBZONE;i++)
	{
		APPELHS ("Ecran,Recupere,ENDABSCISS,%d",i+1);
		sscanf(HsChaine,"%s",&Four.EndAbsSup[i]);

		APPELHS ("Ecran,Recupere,TCPLABSCIS,%d",i+1);
		sscanf(HsChaine,"%s",&Four.TcplAbsSup[i]);

		APPELHS ("Ecran,Recupere,TCPLDEPTH,%d",i+1);
		sscanf(HsChaine,"%s",&Four.TcplDepthSup[i]);

		APPELHS ("Ecran,Recupere,QGMAX,%d",i+1);
		sscanf(HsChaine,"%s",&Four.QGMaxSup[i]);

		APPELHS ("Ecran,Recupere,QAMAX,%d",i+1);
		sscanf(HsChaine,"%s",&Four.QAMaxSup[i]);

		APPELHS ("Ecran,Recupere,SPTMIN,%d",i+1);
		sscanf(HsChaine,"%s",&Four.SptMinSup[i]);

		APPELHS ("Ecran,Recupere,SPTMAX,%d",i+1);
		sscanf(HsChaine,"%s",&Four.SptMaxSup[i]);

		APPELHS ("Ecran,Recupere,SLOPE,%d",i+1);
		sscanf(HsChaine,"%s",&Four.SptVarSup[i]);

		APPELHS ("Ecran,Recupere,RESPTIME,%d",i+1);
		sscanf(HsChaine,"%s",&Four.RespTimeSup[i]);
	}
}

//Transfert de la base de données vers la structure
//**************************************************
void Furn_LoadData()
{
	int status; //status d'ouverture
	int erreur = 1; // si pb de lecture  erreur = 0

	//chemin de direction où se trouve la database
	char *DB_path = "";   //Path;
	char  touche[3+1];
	//ouverture de la base de données en lecture
	status = DBopen(DB_CfgFour_Item,0,DB_path);
	if (!status)
	{
		// chargement des données modèle de la database
		erreur *= LireDBFloat(DB_CfgFour_Item,0,DB_Four_Length,Four.Length);
		erreur *= LireDBFloat(DB_CfgFour_Item,0,DB_Four_Width,Four.Width);
		erreur *= LireDBFloat(DB_CfgFour_Item,0,DB_Four_HTop,Four.HTop);
	 //	erreur *= LireDBFloat(DB_CfgFour_Item,0,DB_Four_HBot,Four.HBot);
		erreur *= LireDBFloat(DB_CfgFour_Item,0,DB_Four_Step1,Four.Step1);
		erreur *= LireDBFloat(DB_CfgFour_Item,0,DB_Four_Step2,Four.Step2);
		erreur *= LireDBFloat(DB_CfgFour_Item,0,DB_Four_WBeamLim,Four.WBeamLim);
		erreur *= LireDBFloat(DB_CfgFour_Item,0,DB_Four_ZUBeg,Four.ZUBeg);
		erreur *= LireDBFloat(DB_CfgFour_Item,0,DB_Four_ZUEnd,Four.ZUEnd);

		for (int i=0;i<NBZONE;i++)
		{
			erreur *=LireDBFloat(DB_CfgFour_Item,0,(DB_Four_EndAbsSup+i*sizeof(float)),Four.EndAbsSup[i]);
			erreur *=LireDBFloat(DB_CfgFour_Item,0,(DB_Four_TcplAbsSup+i*sizeof(float)),Four.TcplAbsSup[i]);
			erreur *=LireDBFloat(DB_CfgFour_Item,0,(DB_Four_TcplDepthSup+i*sizeof(float)),Four.TcplDepthSup[i]);
			erreur *=LireDBFloat(DB_CfgFour_Item,0,(DB_Four_QGMaxSup+i*sizeof(float)),Four.QGMaxSup[i]);
			erreur *=LireDBFloat(DB_CfgFour_Item,0,(DB_Four_QAMaxSup+i*sizeof(float)),Four.QAMaxSup[i]);
			erreur *=LireDBInt(DB_CfgFour_Item,0,(DB_Four_SptMinSup+i*sizeof(int)),Four.SptMinSup[i]);
			erreur *=LireDBInt(DB_CfgFour_Item,0,(DB_Four_SptMaxSup+i*sizeof(int)),Four.SptMaxSup[i]);
			erreur *=LireDBInt(DB_CfgFour_Item,0,(DB_Four_SptVarSup+i*sizeof(int)),Four.SptVarSup[i]);
			erreur *=LireDBInt(DB_CfgFour_Item,0,(DB_Four_RespTimeSup+i*sizeof(int)),Four.RespTimeSup[i]);
		}

/*    		for (i=0;i<7;i++)
		{
			erreur *=LireDBFloat(DB_CfgFour_Item,0,(DB_Four_EndAbsInf+i*sizeof(float)),Four.EndAbsInf[i]);
			erreur *=LireDBFloat(DB_CfgFour_Item,0,(DB_Four_TcplAbsInf+i*sizeof(float)),Four.TcplAbsInf[i]);
			erreur *=LireDBFloat(DB_CfgFour_Item,0,(DB_Four_TcplDepthInf+i*sizeof(float)),Four.TcplDepthInf[i]);
			erreur *=LireDBFloat(DB_CfgFour_Item,0,(DB_Four_QGMaxInf+i*sizeof(float)),Four.QGMaxInf[i]);
	//      	erreur *=LireDBFloat(DB_CfgFour_Item,0,(DB_Four_QAMaxInf+i*sizeof(float)),Four.QAMaxInf[i]);
			erreur *=LireDBInt(DB_CfgFour_Item,0,(DB_Four_SptMinInf+i*sizeof(int)),Four.SptMinInf[i]);
			erreur *=LireDBInt(DB_CfgFour_Item,0,(DB_Four_SptMaxInf+i*sizeof(int)),Four.SptMaxInf[i]);
			erreur *=LireDBInt(DB_CfgFour_Item,0,(DB_Four_SptVarInf+i*sizeof(int)),Four.SptVarInf[i]);
			erreur *=LireDBInt(DB_CfgFour_Item,0,(DB_Four_RespTimeInf+i*sizeof(int)),Four.RespTimeInf[i]);
		 }   */

		if (!erreur)  //erreur de lecture
		{
			File_Error(100,DB_CfgFour_Name);
			Furn_LoadFile();
		}

		 //fermeture de la database
		 DBclose(DB_CfgFour_Item);
	}
	else //problème d'ouverture du fichier
	{
		File_Error(101,DB_CfgFour_Name);
		Furn_LoadFile();
	}

	//sauvegarde de la base de données dans un buffer
	memcpy(Furn_Buf,&Four,sizeof(Four));
}

//sauvegarde de la structure dans la base de données
//**************************************************
void Furn_StoreData()
{
	int status; //status d'ouverture
	int erreur = 1; //si pb d'écriture erreur = 0;
	char touche[3+1];
	//chemin de direction où se trouve la database
	char *DB_path = "";

//ouverture de la base de données en écriture
	status = DBopen(DB_CfgFour_Item,1,DB_path);
	if (!status)
	{
// chargement des données modèle de la database
		erreur *= DBwriteFloat(DB_CfgFour_Item,0,DB_Four_Length,Four.Length);
		erreur *= DBwriteFloat(DB_CfgFour_Item,0,DB_Four_Width,Four.Width);			erreur *= DBwriteFloat(DB_CfgFour_Item,0,DB_Four_HTop,Four.HTop);
	//	 	erreur *= DBwriteFloat(DB_CfgFour_Item,0,DB_Four_HBot,Four.HBot);
		erreur *= DBwriteFloat(DB_CfgFour_Item,0,DB_Four_Step1,Four.Step1);
		erreur *= DBwriteFloat(DB_CfgFour_Item,0,DB_Four_Step2,Four.Step2);
		erreur *= DBwriteFloat(DB_CfgFour_Item,0,DB_Four_WBeamLim,Four.WBeamLim);
		erreur *= DBwriteFloat(DB_CfgFour_Item,0,DB_Four_ZUBeg,Four.ZUBeg);
		erreur *= DBwriteFloat(DB_CfgFour_Item,0,DB_Four_ZUEnd,Four.ZUEnd);

		for (int i=0;i<NBZONE;i++)
		{
			erreur *=DBwriteFloat(DB_CfgFour_Item,0,(DB_Four_EndAbsSup+i*sizeof(float)),Four.EndAbsSup[i]);
			erreur *=DBwriteFloat(DB_CfgFour_Item,0,(DB_Four_TcplAbsSup+i*sizeof(float)),Four.TcplAbsSup[i]);
			erreur *=DBwriteFloat(DB_CfgFour_Item,0,(DB_Four_TcplDepthSup+i*sizeof(float)),Four.TcplDepthSup[i]);
			erreur *=DBwriteFloat(DB_CfgFour_Item,0,(DB_Four_QGMaxSup+i*sizeof(float)),Four.QGMaxSup[i]);
			erreur *=DBwriteFloat(DB_CfgFour_Item,0,(DB_Four_QAMaxSup+i*sizeof(float)),Four.QAMaxSup[i]);
			erreur *=DBwriteInt(DB_CfgFour_Item,0,(DB_Four_SptMinSup+i*sizeof(int)),Four.SptMinSup[i]);
			erreur *=DBwriteInt(DB_CfgFour_Item,0,(DB_Four_SptMaxSup+i*sizeof(int)),Four.SptMaxSup[i]);
			erreur *=DBwriteInt(DB_CfgFour_Item,0,(DB_Four_SptVarSup+i*sizeof(int)),Four.SptVarSup[i]);
			erreur *=DBwriteInt(DB_CfgFour_Item,0,(DB_Four_RespTimeSup+i*sizeof(int)),Four.RespTimeSup[i]);
		}
	  /*
		for (i=0;i<NBZONEI;i++)
		{
			erreur *=DBwriteFloat(DB_CfgFour_Item,0,(DB_Four_EndAbsInf+i*sizeof(float)),Four.EndAbsInf[i]);
			erreur *=DBwriteFloat(DB_CfgFour_Item,0,(DB_Four_TcplAbsInf+i*sizeof(float)),Four.TcplAbsInf[i]);
			erreur *=DBwriteFloat(DB_CfgFour_Item,0,(DB_Four_TcplDepthInf+i*sizeof(float)),Four.TcplDepthInf[i]);
			erreur *=DBwriteFloat(DB_CfgFour_Item,0,(DB_Four_QGMaxInf+i*sizeof(float)),Four.QGMaxInf[i]);
			erreur *=DBwriteFloat(DB_CfgFour_Item,0,(DB_Four_QAMaxInf+i*sizeof(float)),Four.QAMaxInf[i]);
			erreur *=DBwriteInt(DB_CfgFour_Item,0,(DB_Four_SptMinInf+i*sizeof(int)),Four.SptMinInf[i]);
			erreur *=DBwriteInt(DB_CfgFour_Item,0,(DB_Four_SptMaxInf+i*sizeof(int)),Four.SptMaxInf[i]);
			erreur *=DBwriteInt(DB_CfgFour_Item,0,(DB_Four_SptVarInf+i*sizeof(int)),Four.SptVarInf[i]);
			erreur *=DBwriteInt(DB_CfgFour_Item,0,(DB_Four_RespTimeInf+i*sizeof(int)),Four.RespTimeInf[i]);
			}    */

		//fermeture de la base de données
		DBclose(DB_CfgFour_Item);

		if (!erreur)
			File_Error(100, DB_CfgFour_Name);  	//MsgFour2="Reading Database Error"
	}
	else //problème d'ouverture du fichier
		File_Error(101, DB_CfgFour_Name);
}

//sauvegarde de la structure dans le fichier de configuration
// *.cfg

void Furn_SaveInFile()
{
	char afile[13];
	strcpy(afile,NomApplication);
	strcat(afile,".CFG");
	char *titre = "Zn: ";
	ofstream furnc;
	furnc.open(afile,ios::out|ios::app|ios::trunc);
	if (furnc)
	{
		furnc << "Length(mm): " << Four.Length << endl;
		furnc << "Width(mm): " << Four.Width << endl;
		furnc << "TopHeight(mm): " << Four.HTop << endl;
		furnc << "BotHeight(mm): " << "0" << endl;
		furnc << "NbZone: " << NBZONE << endl;
		furnc << "NbZoneI: " << "0" << endl;
		furnc << "StepLength(mm): "  << Four.Step1 << endl;
		furnc << "NbStep: " << atof(Four.Length)/atof(Four.Step1) << endl;
		furnc << "PCI: " << "0"  << endl;

		furnc << "ZoneSup" << endl;
		for (int iz=0; iz < NBZONE ; iz++)
		{
			titre[1] = (char)(iz+48);
			furnc << titre << Four.EndAbsSup[iz] << " "
  //			<< "2500 "	// zone height
			<< Four.TcplAbsSup[iz] << " "
			<< Four.TcplDepthSup[iz] << " "
			<< Four.QGMaxSup[iz] << " "
			<< Four.QAMaxSup[iz] << " "
	 //			<< "-1 "
			<< Four.SptMinSup[iz] << " "
			<< Four.SptMaxSup[iz] << " "
			<< Four.SptVarSup[iz] << " "
			<< Four.RespTimeSup[iz] << endl;
		}//for each zone

		furnc << "ZoneInf" << endl;
/*	  	for (iz=0; iz < NBZONEI; iz++)
		{
			titre[1] = (char)(iz+48);
			furnc << titre << Four.EndAbsInf[iz] << " "
			<< "2500 "	// zone height
			<< Four.TcplAbsInf[iz] << " "
			<< Four.TcplDepthInf[iz] << " "
			<< "0 "		// Losses
			<< Four.QGMaxInf[iz] << " "
//			<< Four.QAMaxInf[iz] << " "
			<< "-1 "		// QAMaxInf
			<< Four.SptMinInf[iz] << " "
			<< Four.SptMaxInf[iz] << " "
			<< Four.SptVarInf[iz] << " "
			<< Four.RespTimeInf[iz] << " "
			<< iz		// Master zone
			<< " 0"	// Offset
			<< endl;
		}//for each zone
*/
		//  furnc << "NbPanelSup: " << "0" <<endl;
		//  furnc << "NbPanelInf: " << "0" <<endl;
	  furnc << "LongueurProduit(mm): " << "5000" << endl;
	  furnc << "TempEnfourn(øC): " << "20" << endl;
	  if (!strcmp(Four.Step2, "0"))
			furnc << "DoubleStep(0/1): " << "0"  << endl;
	  else
			furnc << "DoubleStep(0/1): " << "1"  << endl;

	  furnc << "Step2Length(mm): " << Four.Step2 << endl;
	  furnc << "ChangeStepAbsc(mm): "<< Four.WBeamLim << endl;
	  furnc << "AbsBegZu(mm): " << Four.ZUBeg << endl;
	  furnc << "AbsEndZu(mm): " << Four.ZUEnd << endl;
	  furnc << "TPSPENTE: " << "4" << endl;
	  furnc << "PenteMax: " << "10" << endl;

	  furnc.close();
	}
	else
		File_Error(101,afile); //File Opening Error
}

void Furn_LoadFile()
{
	char afile[13];
	(void)strcpy (afile,NomApplication);
	(void)strcat(afile,".CFG");
	char Texte[50], Texte2[50];
	FILE *fp;

	fp =fopen(afile,"rt+");

	if (feof(fp)==0)
	{
		fscanf(fp, "%s %s", &Texte, &Four.Length);
		fscanf(fp, "%s %s", &Texte, &Four.Width);
		fscanf(fp, "%s %s", &Texte, &Four.HTop);
		fscanf(fp, "%s %s", &Texte, &Texte2);
		fscanf(fp, "%s %s", &Texte, &Texte2);
		fscanf(fp, "%s %s", &Texte, &Texte2);
		fscanf(fp, "%s %s", &Texte, &Four.Step1);
		fscanf(fp, "%s %s", &Texte, &Texte2);
		fscanf(fp, "%s %s", &Texte, &Texte2);
		fscanf(fp, "%s", &Texte);

		for (int iz=0; iz < NBZONE; iz++)
		{
			fscanf(fp, "%s %s %s %s %s %s %s %s %s %s", &Texte,
					&Four.EndAbsSup[iz],
					&Four.TcplAbsSup[iz],
					&Four.TcplDepthSup[iz],
					&Four.QGMaxSup[iz],
					&Four.QAMaxSup[iz],
					&Four.SptMinSup[iz],
					&Four.SptMaxSup[iz],
					&Four.SptVarSup[iz],
					&Four.RespTimeSup[iz]);
		}//for each zone
/*	  for (iz=0; iz < NBZONEI; iz++)
	  {
		fscanf(fp, "%s %s %s %s %s %s %s %s %s %s", &Texte,
			&Four.EndAbsInf[iz],
			&Four.TcplAbsInf[iz],
			&Four.TcplDepthInf[iz],
			&Four.QGMaxInf[iz],
//			&Four.QAMaxInf[iz],
			&Four.SptMinInf[iz],
			&Four.SptMaxInf[iz],
			&Four.SptVarInf[iz],
			&Four.RespTimeInf[iz],
		}//for each zone
*/
		fscanf(fp, "%s", &Texte);
		fscanf(fp, "%s %s", &Texte, &Texte2);
		fscanf(fp, "%s %s", &Texte, &Texte2);
		fscanf(fp, "%s %s", &Texte, &Texte2);
		fscanf(fp, "%s %s", &Texte, &Texte2);
		fscanf(fp, "%s %s", &Texte, &Texte2);
		fscanf(fp, "%s %s", &Texte, Four.ZUBeg);
		fscanf(fp, "%s %s", &Texte, Four.ZUEnd);
	}
	fclose(fp);
//	memcpy(Buf,&Four,sizeof(Four));
}
