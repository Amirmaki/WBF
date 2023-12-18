#include <dir.h>
#include <fstream.h>
#include <stdlib.h>
#include <time.h>
#include "modwind.h"
#include "config.h"
#include "language.h"

#ifndef __MODCFG_H
#define __OWNER_MODCFG_H
#include "modwind.h"
#endif

#ifndef __DATABASE_H
#include "database.h"
#endif
#ifndef __DB_H
#include "db.h"
#endif

#ifndef __WINSTRUC_H
#include "winstruc.h"
#endif
#ifndef __GLOBDAT_H
#include "globdat.h"
#endif

#if LANGUAGE == 1				// Italian

// Définition des messages
#define MsgMod1	"Allarmi:"
#define MsgMod2	"Errore di lettura della Database"
#define MsgMod3	"Errore di apertura del file"
#define MsgMod4	"Sostituire il file di database corrente ?"
#define MsgMod5	"Modifica della Configurazione"
#endif

#if LANGUAGE == 2				// English

// Définition des messages
#define MsgMod1	"Alarm:"
#define MsgMod2	"Database Reading Error"
#define MsgMod3	"File Opening Error"
#define MsgMod4	"Save settings in current database ?"
#define MsgMod5	"Model configuration modified"
#endif

void Model_Config()
{
	if ( *init == INITIAL)
	{
		*init =NORMAL;
		Vue_Precedente=CONFIG_MENU;
		APPELHS ("Utilise,MODELCFG.AID");

		APPELHS ("Fusionne,logo_cc.img,1,2");
		APPELHS ("Fusionne,modl_ttl.img,1,28");
		APPELHS ("Fusionne,modl_scr.img,4,3");

		APPELHS ("Option,Couleur,S=240");
		APPELHS ("Option,Couleur,A=240");

		APPELHS ("Option,verticale");
		APPELHS ("Sortie,ESC=V,RET=V,FLH=M,FLB=D,TAB=S,TF1=V,TF2=V,TF4=V,TF5=V,TF0=V");
		APPELHS ("Ecran,Premier,SQRPRODUCT");

		Model_Buf = new char[sizeof(ModelCopy)];
		Model_InitWindow();
		Model_SetupWindow();
	}
	APPELHS ("Ecran,Saisie /nonblocant/controle");
	APPELHS ("Ecran,Courant");

	if(!strcmp(HsNom,"FACTOR    "))
	{
		if(HsIndice==5)
		{
			APPELHS("Ecran,saisie/nonblocant/controle");

			if(!strcmp(HsTouche,"*A*") || !strcmp(HsTouche,"*P*"))
				APPELHS("Ecran,Premier,THERMAL");
		}

		else	if(HsIndice==4)
		{
			APPELHS("Ecran,saisie/nonblocant/controle");

			if(!strcmp(HsTouche,"*A*")|| !strcmp(HsTouche,"*P*"))
				APPELHS("Ecran,Premier,FACTOR,5");
		}
  }

  if(!strcmp(HsNom,"TIMECORREC"))
  {
		APPELHS("Ecran,saisie/nonblocant/controle");

		if(!strcmp(HsTouche,"*P*") || !strcmp(HsTouche,"*A*"))
			APPELHS("Ecran,Premier,FACTOR,1");
	}

  if(!strcmp(HsNom,"FURNACE   "))
  {
		APPELHS("Ecran,saisie/nonblocant/controle");

		if(!strcmp(HsTouche,"*P*") || !strcmp(HsTouche,"*A*"))
			APPELHS("Ecran,Premier,SETPOINT2");
	}

	if(!strcmp(HsNom,"CONTROLLER"))
	{
		APPELHS("Ecran,saisie/nonblocant/controle");

		if(!strcmp(HsTouche,"*P*") || !strcmp(HsTouche,"*A*"))
			APPELHS("Ecran,Premier,SQRPRODUCT");
	}
	APPELHS ("Ecran,Touche");

	if(!strcmp(HsTouche,"TF0") || !strcmp(HsTouche,"TF4"))
		Teste_Buffer(HsTouche);

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

	if(!strcmp(HsTouche,"TF2"))
		Teste_Buffer(HsTouche);
}
//--------------------------------------------------------------------------
void Model_InitWindow()
{
  // Initialisation  des données dans la structure
	strcpy(ModelCopy.HCoef1,"0");
	strcpy(ModelCopy.HCoef2,"0");
	strcpy(ModelCopy.HCoef3,"0");
	strcpy(ModelCopy.Epsif1,"0");
	strcpy(ModelCopy.Epsif2,"0");
	strcpy(ModelCopy.EpsifMin,"0");
	strcpy(ModelCopy.EpsifMax,"0");
	strcpy(ModelCopy.KSurf,"0");
	strcpy(ModelCopy.KTime,"0");

	for (int i=0;i<NBZONE;i++)
		strcpy(ModelCopy.KEpsiSup[i],"0");
	//  #ifdef WITHZONEI
	//  for (i=0;i<NBZONEI;i++) strcpy(Model.KEpsiInf[i],"0");
	//  #endif

	strcpy(ModelCopy.CalcPer,"0");
	strcpy(ModelCopy.SPCalcPer,"0");
	strcpy(ModelCopy.PHisPer,"0");
	strcpy(ModelCopy.ZHisPer,"0");
	//strcpy(Model.Alarm, MsgMod1);
	ModelCopy.SPEnable  =1;
	ModelCopy.ZHisEnable=1;
	ModelCopy.PHisEnable=1;
	ModelCopy.PaceEnable=1;
	ModelCopy.PLCConnect=1;
	ModelCopy.CTLConnect=1;
	// Chargement des données de la base de données
	Model_LoadData();
	// TransferBuffer = (void far*)&(Model);
}
//--------------------------------------------------------------------------
void Model_SetupWindow()
{
	APPELHS("Affiche,SQRPRODUCT=%s",ModelCopy.HCoef1);
	APPELHS("Affiche,FLATPRODUC=%s",ModelCopy.HCoef2);
	APPELHS("Affiche,CYLINDRIC =%s",ModelCopy.HCoef3);
	APPELHS("Affiche,WASTEGAS1 =%s",ModelCopy.Epsif1);
	APPELHS("Affiche,WASTEGAS2 =%s",ModelCopy.Epsif2);
	APPELHS("Affiche,MINWASTE  =%s",ModelCopy.EpsifMin);
	APPELHS("Affiche,MAXWASTE  =%s",ModelCopy.EpsifMax);
	APPELHS("Affiche,PRODEMISS =%s",ModelCopy.KSurf);
	APPELHS("Affiche,TIMECORREC=%s",ModelCopy.KTime);

	for (int i=0;i<NBZONE;i++)
		APPELHS("Affiche,FACTOR,%d=%s",i+1,ModelCopy.KEpsiSup[i]);

	APPELHS("Affiche,THERMAL   =%s",ModelCopy.CalcPer);
	APPELHS("Affiche,SETPOINT  =%s",ModelCopy.SPCalcPer);
	APPELHS("Affiche,PRODUCT   =%s",ModelCopy.PHisPer);
	APPELHS("Affiche,FURNACE   =%s",ModelCopy.ZHisPer);
	APPELHS("Affiche,SETPOINT2 =%s",Conversion_Lecture(ModelCopy.SPEnable));
	APPELHS("Affiche,FURNACE2  =%s",Conversion_Lecture(ModelCopy.ZHisEnable));
	APPELHS("Affiche,PRODUCT2  =%s",Conversion_Lecture(ModelCopy.PHisEnable));
	APPELHS("Affiche,PACING    =%s",Conversion_Lecture(ModelCopy.PaceEnable));
	APPELHS("Affiche,PLC       =%s",Conversion_Lecture(ModelCopy.PLCConnect));
	APPELHS("Affiche,CONTROLLER=%s",Conversion_Lecture(ModelCopy.CTLConnect));
}
//--------------------------------------------------------------------------
void Model_FillBuffers()
// Sauvegarde les données de la configuration dans la structure
{
	APPELHS("Ecran,Recupere,SQRPRODUCT");
	sscanf(HsChaine,"%s",&ModelCopy.HCoef1);

	APPELHS("Ecran,Recupere,FLATPRODUC");
	sscanf(HsChaine,"%s",&ModelCopy.HCoef2);

	APPELHS("Ecran,Recupere,CYLINDRIC");
	sscanf(HsChaine,"%s",&ModelCopy.HCoef3);

	APPELHS("Ecran,Recupere,WASTEGAS1");
	sscanf(HsChaine,"%s",&ModelCopy.Epsif1);

	APPELHS("Ecran,Recupere,WASTEGAS2");
	sscanf(HsChaine,"%s",&ModelCopy.Epsif2);

	APPELHS("Ecran,Recupere,MINWASTE");
	sscanf(HsChaine,"%s",&ModelCopy.EpsifMin);

	APPELHS("Ecran,Recupere,MAXWASTE");
	sscanf(HsChaine,"%s",&ModelCopy.EpsifMax);

	APPELHS("Ecran,Recupere,PRODEMISS");
	sscanf(HsChaine,"%s",&ModelCopy.KSurf);

	APPELHS("Ecran,Recupere,TIMECORREC=");
	sscanf(HsChaine,"%s",&ModelCopy.KTime);

	APPELHS("Ecran,Recupere,THERMAL=");
	sscanf(HsChaine,"%s",&ModelCopy.CalcPer);

	APPELHS("Ecran,Recupere,SETPOINT=");
	sscanf(HsChaine,"%s",&ModelCopy.SPCalcPer);

	APPELHS("Ecran,Recupere,PRODUCT=");
	sscanf(HsChaine,"%s",&ModelCopy.PHisPer);

	APPELHS("Ecran,Recupere,FURNACE=");
	sscanf(HsChaine,"%s",&ModelCopy.ZHisPer);

	for (int i=0;i<NBZONE;i++)
	{
		APPELHS("Ecran,Recupere,FACTOR,%d=",i+1);
		sscanf(HsChaine,"%s",&ModelCopy.KEpsiSup[i]);
//		strcat(Model.KEpsiSup[i],"0");
	}

	APPELHS("Ecran,Recupere,SETPOINT2=");
	ModelCopy.SPEnable=Conversion_Ecriture();

	APPELHS("Ecran,Recupere,FURNACE2=");
	ModelCopy.ZHisEnable=Conversion_Ecriture();

	APPELHS("Ecran,Recupere,PRODUCT2=");
	ModelCopy.PHisEnable=Conversion_Ecriture();

	APPELHS("Ecran,Recupere,PACING=");
	ModelCopy.PaceEnable=Conversion_Ecriture();

	APPELHS("Ecran,Recupere,PLC=");
	ModelCopy.PLCConnect=Conversion_Ecriture();

	APPELHS("Ecran,Recupere,CONTROLLER=");
	ModelCopy.CTLConnect=Conversion_Ecriture();

	strcpy(ModelCopy.MeasPer,"30");
}
//--------------------------------------------------------------------------
int Conversion_Ecriture()
{
	if(!strcmp(HsChaine,"O"))
		return(1);
	else
		return(0);
}
//--------------------------------------------------------------------------
char *Conversion_Lecture(int a_convertir)
{
	if(a_convertir==1)
		return("O");
	else
		return("N");
}
//--------------------------------------------------------------------------
void Model_LoadData()  		// Transfert base de données => structure
{
	int status; 				// Status d'ouverture
	int erreur = 1; 			// Si pb de lecture  erreur = 0
	char *DB_path =Path;			// Répertoire où se trouve la database
	int temp;

	// Ouverture de la base de données en lecture
	status = DBopen(DB_CfgModel_Item,0,DB_path);

	if (!status)
	{ // Chargement des données modèle de la database
		erreur *= LireDBFloat(DB_CfgModel_Item,0,DB_Model_HCoef1,ModelCopy.HCoef1);
		erreur *= LireDBFloat(DB_CfgModel_Item,0,DB_Model_HCoef2,ModelCopy.HCoef2);
		erreur *= LireDBFloat(DB_CfgModel_Item,0,DB_Model_HCoef3,ModelCopy.HCoef3);
		erreur *= LireDBFloat(DB_CfgModel_Item,0,DB_Model_Epsif1,ModelCopy.Epsif1);
		erreur *= LireDBFloat(DB_CfgModel_Item,0,DB_Model_Epsif2,ModelCopy.Epsif2);
		erreur *= LireDBFloat(DB_CfgModel_Item,0,DB_Model_EpsifMin,ModelCopy.EpsifMin);
		erreur *= LireDBFloat(DB_CfgModel_Item,0,DB_Model_EpsifMax,ModelCopy.EpsifMax);
		erreur *= LireDBChar(DB_CfgModel_Item ,0,DB_Model_KSurf,ModelCopy.KSurf);
		erreur *= LireDBFloat(DB_CfgModel_Item,0,DB_Model_KTime,ModelCopy.KTime);

		for (int i=0;i<NBZONE;i++)
		{
			erreur *=LireDBFloat(DB_CfgModel_Item,0,(DB_Model_KEpsiSup
					 +i*sizeof(float)),ModelCopy.KEpsiSup[i]);
			temp = 10*atoi(ModelCopy.KEpsiSup[i]);
			itoa(temp,ModelCopy.KEpsiSup[i],10);
		}
 //		#ifdef WITHZONEI
 //		for (i=0;i<NBZONEI;i++)
//			{
//			erreur *=LireDBFloat(DB_CfgModel_Item,0,(DB_Model_KEpsiInf
//					 +i*sizeof(float)),Model.KEpsiInf[i]);
//			temp = 10*atoi(Model.KEpsiInf[i]);
//			itoa(temp,Model.KEpsiInf[i],10);
//			}
//      #endif
		erreur *= LireDBInt(DB_CfgModel_Item,0,DB_Model_MeasPer,ModelCopy.MeasPer);
		erreur *= LireDBInt(DB_CfgModel_Item,0,DB_Model_CalcPer,ModelCopy.CalcPer);
		erreur *= LireDBInt(DB_CfgModel_Item,0,DB_Model_SPCalcPer,
					  ModelCopy.SPCalcPer);
		erreur *= LireDBInt(DB_CfgModel_Item,0,DB_Model_PHisPer,ModelCopy.PHisPer);
		erreur *= LireDBInt(DB_CfgModel_Item,0,DB_Model_ZHisPer,ModelCopy.ZHisPer);
		erreur *= LireDB(DB_CfgModel_Item   ,0,DB_Model_SPEnable,&ModelCopy.SPEnable,
						 sizeof(char));
		erreur *= LireDB(DB_CfgModel_Item,0,DB_Model_ZHisEnable,
						 &ModelCopy.ZHisEnable,sizeof(char));
		erreur *= LireDB(DB_CfgModel_Item,0,DB_Model_PHisEnable,
						 &ModelCopy.PHisEnable,sizeof(char));
		erreur *= LireDB(DB_CfgModel_Item,0,DB_Model_PaceEnable,
						 &Model.PaceEnable,sizeof(char));
		erreur *= LireDB(DB_CfgModel_Item,0,DB_Model_PLCConnect,
				 &ModelCopy.PLCConnect,sizeof(char));

		// Comm° PLC et Rég. fonctionnent avec le même flag
		ModelCopy.CTLConnect = ModelCopy.PLCConnect;
		erreur *= LireDB(DB_CfgModel_Item,0,DB_Model_CTLConnect,
					 &ModelCopy.CTLConnect,sizeof(char));

		if (!erreur)
		{
			File_Error(100,DB_CfgModel_Name);
			Model_LoadFile();
		}  //Database Reading Error

		DBclose(DB_CfgModel_Item);		 // Fermeture de la database
		// Sauvegarde de la base de données dans un buffer
		memcpy(Model_Buf,&ModelCopy,sizeof(ModelCopy));
	}
	else
	{
			File_Error(101,DB_CfgModel_Name);
			Model_LoadFile();
	}
}
//--------------------------------------------------------------------------
void Model_StoreData()  // Sauvegarde la structure ds base de données
{
	int status;
	int erreur = 1;
	char *DB_path =Path;
	int temp;
	status = DBopen(DB_CfgModel_Item,1,DB_path);
	status += DBopen(DB_CfgFour_Item,1,DB_path);

	if (!status) 	     // Sauvegarde des données modèle ds la database
	{
		erreur *= DBwriteFloat(DB_CfgModel_Item,0,DB_Model_HCoef1,ModelCopy.HCoef1);
		erreur *= DBwriteFloat(DB_CfgModel_Item,0,DB_Model_HCoef2,ModelCopy.HCoef2);
		erreur *= DBwriteFloat(DB_CfgModel_Item,0,DB_Model_HCoef3,ModelCopy.HCoef3);
		erreur *= DBwriteFloat(DB_CfgModel_Item,0,DB_Model_Epsif1,ModelCopy.Epsif1);
		erreur *= DBwriteFloat(DB_CfgModel_Item,0,DB_Model_Epsif2,ModelCopy.Epsif2);
		erreur *= DBwriteFloat(DB_CfgFour_Item ,0,DB_Four_Epsif1,ModelCopy.Epsif1);
		erreur *= DBwriteFloat(DB_CfgFour_Item ,0,DB_Four_Epsif2,ModelCopy.Epsif2);
		erreur *= DBwriteFloat(DB_CfgModel_Item,0,DB_Model_EpsifMin,ModelCopy.EpsifMin);
		erreur *= DBwriteFloat(DB_CfgModel_Item,0,DB_Model_EpsifMax,ModelCopy.EpsifMax);
		erreur *= DBwriteFloat(DB_CfgFour_Item ,0,DB_Four_EpsifMin,ModelCopy.EpsifMin);
		erreur *= DBwriteFloat(DB_CfgFour_Item ,0,DB_Four_EpsifMax,ModelCopy.EpsifMax);
		erreur *= DBwriteChar(DB_CfgModel_Item ,0,DB_Model_KSurf,ModelCopy.KSurf);
		erreur *= DBwriteFloat(DB_CfgModel_Item,0,DB_Model_KTime,ModelCopy.KTime);

		for (int i=0;i<NBZONE;i++)
		{
			temp = atoi(ModelCopy.KEpsiSup[i]);
			temp /= 10;
			itoa(temp,ModelCopy.KEpsiSup[i],10);
			erreur*=DBwriteFloat(DB_CfgModel_Item,0,(DB_Model_KEpsiSup
					 +i*sizeof(float)),ModelCopy.KEpsiSup[i]);
			erreur *=DBwriteFloat(DB_CfgFour_Item,0,(DB_Four_KfSup
				+i*sizeof(float)),ModelCopy.KEpsiSup[i]);
		}

//		#ifdef WITHZONEI
//		for (i=0;i<NBZONEI;i++)
//		{
//			temp = atoi(Model.KEpsiInf[i]);
//			temp /= 10;
//			itoa(temp,Model.KEpsiInf[i],10);
//			erreur*=DBwriteFloat(DB_CfgModel_Item,0,(DB_Model_KEpsiInf
//					 +i*sizeof(float)),Model.KEpsiInf[i]);
//			erreur *=DBwriteFloat(DB_CfgFour_Item,0,(DB_Four_KfInf
//				+i*sizeof(float)),Model.KEpsiInf[i]);
//		}
 //	 #endif

		erreur *= DBwriteInt(DB_CfgModel_Item,0,DB_Model_MeasPer,
						ModelCopy.MeasPer);
		erreur *= DBwriteInt(DB_CfgModel_Item,0,DB_Model_CalcPer,
						ModelCopy.CalcPer);
		erreur *= DBwriteInt(DB_CfgModel_Item,0,DB_Model_SPCalcPer,
						ModelCopy.SPCalcPer);
		erreur *= DBwriteInt(DB_CfgModel_Item,0,DB_Model_PHisPer,
						ModelCopy.PHisPer);
		erreur *= DBwriteInt(DB_CfgModel_Item,0,DB_Model_ZHisPer,
						ModelCopy.ZHisPer);
		erreur *= DBwrite(DB_CfgModel_Item,0,DB_Model_SPEnable,
					&ModelCopy.SPEnable,sizeof(char));
		erreur *= DBwrite(DB_CfgModel_Item,0,DB_Model_ZHisEnable,
					&ModelCopy.ZHisEnable,sizeof(char));
		erreur *= DBwrite(DB_CfgModel_Item,0,DB_Model_PHisEnable,
					&ModelCopy.PHisEnable,sizeof(char));
		erreur *= DBwrite(DB_CfgModel_Item,0,DB_Model_PaceEnable,
					&ModelCopy.PaceEnable,sizeof(char));
		erreur *= DBwrite(DB_CfgModel_Item,0,DB_Model_PLCConnect,
					&ModelCopy.PLCConnect,sizeof(char));
		// Comm° PLC et Rég. fonctionnent avec le même flag
		ModelCopy.CTLConnect = ModelCopy.PLCConnect;
		erreur *= DBwrite(DB_CfgModel_Item,0,DB_Model_CTLConnect,
					&ModelCopy.CTLConnect,sizeof(char));

		if (!erreur)
			File_Error(100,DB_CfgModel_Name); //Database Reading Error

		DBclose(DB_CfgModel_Item);
		DBclose(DB_CfgFour_Item);
	}
	else
		File_Error(101,DB_CfgModel_Name); //File Opening Error
}
//--------------------------------------------------------------------------
void Model_SaveInFile()
{
	char afile[13];

	(void)strcpy (afile,NomApplication);
	(void)strcat(afile,".CFK");
	ofstream furnc;
	furnc.open(afile,ios::out|ios::trunc);
	if(furnc)
	{
		furnc << "slabwidthlimit: " << "0" << endl;
		furnc << "topscoeff1: " << ModelCopy.HCoef1 << endl;
		furnc << "topscoeff2: " << ModelCopy.HCoef2 << endl;
		furnc << "topscoeff3: " << ModelCopy.HCoef3 << endl;
		furnc << "epsif1: " <<ModelCopy.Epsif1  << endl;
		furnc << "epsif2: " << ModelCopy.Epsif2 << endl;
		furnc << "minepsif: " << ModelCopy.EpsifMin << endl;
		furnc << "maxepsif: " << ModelCopy.EpsifMax << endl;
		furnc << "prodsurfcode: " << ModelCopy.KSurf << endl;
		furnc << "tau2coeff: " << ModelCopy.KTime << endl;
		char *azt = "Z0KZ: ";
		char temp[20];
		int c;

		for (int az=0;az<NBZONE;az++)
		{
			azt[1] = (char)(48+az);
			furnc << azt << itoa(atoi(ModelCopy.KEpsiSup[az])/10, temp, 10) << endl;
		}
		for (az;az<9;az++)
		{
			azt[1] = (char)(48+az);
			furnc << azt << 0 << endl;
		}
	//	#ifdef WITHZONEI
		 //		for (az=0;az<NBZONEI;az++)
		 //			{
		//			azt[1] = (char)(48+az);
		//			furnc << azt << itoa(atoi(Model.KEpsiSup[az])/10, temp, 10) << endl;
		//			}
	  //		for (az;az<9;az++)
	 //			{
	 //			azt[1] = (char)(48+az);
	 //			furnc << azt << 0 << endl;
	 //			}
	 //  #endif

		furnc << "CalcPer: " << ModelCopy.CalcPer << endl;
		furnc << "MeasPer: " << ModelCopy.MeasPer << endl;
		furnc << "SPCalcPer: " <<ModelCopy.SPCalcPer << endl;
		furnc << "ZHisPer: " << ModelCopy.ZHisPer << endl;
		furnc << "PHisPer: " << ModelCopy.PHisPer << endl;
		// Modif PF22/12/94
		furnc << "SPEnable__: " << (char) (48+ModelCopy.SPEnable)   << endl;
		furnc << "ZHisEnable: " << (char) (48+ModelCopy.ZHisEnable) << endl;
		furnc << "PHisEnable: " << (char) (48+ModelCopy.PHisEnable) << endl;
		furnc << "PaceEnable: " << (char) (48+ModelCopy.PaceEnable) << endl;
		furnc << "PLCEnable_: " << (char) (48+ModelCopy.PLCConnect) << endl;
		furnc << "CTLEnable_: " << (char) (48+ModelCopy.CTLConnect) << endl;
		furnc << "ThermMod: " << "0" << endl;
		furnc << "NbPoint: " << "5" << endl;
		furnc.close();
	}
	else
		File_Error(101,afile); //File Opening Error
}

void Model_LoadFile()
{
	char afile[13];
	(void)strcpy (afile,NomApplication);
	(void)strcat(afile,".CFK");
	char Texte[50], Texte2[50], temp[20];;
	FILE *fp;

	fp =fopen(afile,"rt+");

	if (feof(fp)==0)
	{
		fscanf(fp, "%s %s", &Texte, &Texte2);
		fscanf(fp, "%s %s", &Texte, &ModelCopy.HCoef1);
		fscanf(fp, "%s %s", &Texte, &ModelCopy.HCoef2);
		fscanf(fp, "%s %s", &Texte, &ModelCopy.HCoef3);
		fscanf(fp, "%s %s", &Texte, &ModelCopy.Epsif1);
		fscanf(fp, "%s %s", &Texte, &ModelCopy.Epsif2);
		fscanf(fp, "%s %s", &Texte, &ModelCopy.EpsifMin);
		fscanf(fp, "%s %s", &Texte, &ModelCopy.EpsifMax);
		fscanf(fp, "%s %s", &Texte, &ModelCopy.KSurf);
		fscanf(fp, "%s %s", &Texte, &ModelCopy.KTime);

		for (int az=0;az<NBZONE;az++)
		{
			fscanf(fp, "%s %s", &Texte, &ModelCopy.KEpsiSup[az]);
			strcpy(Model.KEpsiSup[az], itoa(atoi(ModelCopy.KEpsiSup[az])*10,temp,10));
		}

		for (az;az<9;az++)
			fscanf(fp, "%s %s", &Texte, &Texte2);

		fscanf(fp, "%s %s", &Texte, &ModelCopy.CalcPer);
		fscanf(fp, "%s %s", &Texte, &ModelCopy.MeasPer);
		fscanf(fp, "%s %s", &Texte, &ModelCopy.SPCalcPer);
		fscanf(fp, "%s %s", &Texte, &ModelCopy.ZHisPer);
		fscanf(fp, "%s %s", &Texte, &ModelCopy.PHisPer);

		fscanf(fp, "%s %s", &Texte, &ModelCopy.SPEnable);
		fscanf(fp, "%s %s", &Texte, &ModelCopy.ZHisEnable);
		fscanf(fp, "%s %s", &Texte, &ModelCopy.PHisEnable);
		fscanf(fp, "%s %s", &Texte, &ModelCopy.PaceEnable);
		fscanf(fp, "%s %s", &Texte, &ModelCopy.PLCConnect);
		fscanf(fp, "%s %s", &Texte, &ModelCopy.CTLConnect);
	}
	fclose(fp);

	Model.SPEnable   = ModelCopy.SPEnable   - 48;
	Model.ZHisEnable = ModelCopy.ZHisEnable - 48;
	Model.PHisEnable = ModelCopy.PHisEnable - 48;
	Model.PaceEnable = ModelCopy.PaceEnable - 48;
	Model.PLCConnect = ModelCopy.PLCConnect - 48;
	Model.CTLConnect = ModelCopy.CTLConnect - 48;

//	memcpy(Buf2,&Model,sizeof(Model));
}

