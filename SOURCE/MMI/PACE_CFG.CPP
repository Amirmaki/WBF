// OWLCVT 12/23/96 15:29:04
//-------------------------------------------------------------------------
// Pacewind.cpp :  This file manages all the Pacing mode configuration
//
// Creation : JL COUSIN  date: 13/10/94
// Modifs PF27/02/96 : Gestion multi-langues (cf globdat.h)
//-------------------------------------------------------------------------
#ifndef __DATABASE_H
#include "database.h"
#endif
#ifndef __DB_H
#include "db.h"
#endif
#ifndef __GLOBDAT_H
#include "globdat.h"
#endif
#ifndef __LANGUAGE_H
#include "language.h"
#endif
#ifndef __CONFIG_H
#include "config.h"
#endif

#include "pacewind.h"

#if LANGUAGE == 1						  // Italien
#define MsgPace2	"Errore di lettura della database"
#define MsgPace3	"Errore d'apertura di file"
#define MsgPace4	"Scelta della cadenza operatore senza valore nel campo"
#define MsgPace5	"Errore nella scelta dell'operatore"
#endif

#if LANGUAGE == 2						  // Anglais
#define MsgPace2	"Reading Database Error"
#define MsgPace3	"Open File Error"
#define MsgPace4	"Operator Production selected with operator production not set"
#define MsgPace5	"Error in Operator selection"
#endif

void Pacing_Config()
{
	if (*init==INITIAL)
		{
		*init = NORMAL;
		Vue_Precedente = REHEAT_CONTROL;
		APPELHS ("Utilise,pacing.AID");
		APPELHS ("Fusionne,logo_cc.img,1,2");
		APPELHS ("Fusionne,pace_ttl.img,1,35");
//		APPELHS ("Fusionne,furn_scr.img,4,1");

		APPELHS ("Option,Couleur,S=E");    // couleur saisie (fond*16+caractere)
		APPELHS ("Option,Couleur,A=E");

		APPELHS ("Option,verticale,N");
		APPELHS ("Sortie,ESC=V,RET=V,TF1=V,TF2=V,TF4=V,TF5=V,TF0=V");
		APPELHS ("Affiche,PACETITLE=%s",PaceTitle);
		APPELHS ("Affiche,THEORIQUE=%s",Theoretical);
		APPELHS ("Affiche,MESUREE  =%s",Measured);

		PacingSetupWindow();
		}
	APPELHS ("Ecran,Saisie/nonblocant");

	if (!strcmp(HsTouche,"TF0") || !strcmp(HsTouche,"TF4"))
		TesteConfig();

	if (!strcmp(HsTouche,"TF1"))
		{
		*init = INITIAL;
#ifdef DBG_VUE
		fstream f;
		f.open("vue.txt", ios::in);
		f >> vue;
		f.close();
#endif
		Vue_Precedente = vue;
		vue = HELP_SCREEN;
#ifdef DBG_VUE
		f.open("vue.txt", ios::out);
		f << vue;
		f.close();
#endif
		}

	if (!strcmp(HsTouche,"TF2"))
		{
		APPELHS("Ecran,Recupere,CHOIX");
		char newPace=0;
		if (HsEntier==1) newPace = 1;
		if (HsEntier==2) newPace = 3;

		if (newPace != PacingMode)
			{
			PacingGetData();
			PacingStoreData();
			}
		}
}
void PacingSetupWindow()
{
	PacingLoadData();
	PacingSendData();
}
//--------------------------------------------------------------------------
void TesteConfig()
{
	char *Touche;
	APPELHS("Ecran,Touche");
	strcpy(Touche,HsTouche);

	APPELHS("Ecran,Recupere,CHOIX");

	char newPace=0;
	if (HsEntier==1) newPace = 1;
	if (HsEntier==2) newPace = 3;

	if (newPace != PacingMode)
		{
		*init = INITIAL;
		vue = PACING_CONFIRM;
#ifdef DBG_VUE
		fstream f;
		f.open("vue.txt", ios::out);
		f << vue;
		f.close();
#endif
		}
	else
		Sortie_Ecran(Touche);
}

void Pacing_Confirm()
{
	char *Touche;

	if (*init == INITIAL)
		{
		APPELHS("Ecran,Touche");
		strcpy(Touche,HsTouche);

		*init = NORMAL;
		APPELHS ("Ouvre,MESSAGE.AID");
		APPELHS ("Option,Couleur,A=E");

		APPELHS ("Affiche,TITRE=%s",PaceModifTitle);
		APPELHS ("Affiche,FINMESSAGE=%s",ConfirmQst);
		}
	APPELHS("Ecran,Saisie/nonblocant");

	if (!strcmp(HsTouche,"RET"))
		{
		APPELHS("Ferme");
		PacingGetData();
		PacingStoreData();
		Sortie_Ecran(Touche);
		}

	if (!strcmp(HsTouche,"ESC"))
		{
		APPELHS("Ferme");
		Sortie_Ecran(Touche);
		}
}
//--------------------------------------------------------------------------
void PacingSendData()
{
	int check=0;

	if ((int)PacingMode == 1) check = 1;
	if ((int)PacingMode == 3) check = 2;

	APPELHS("Affiche,CHOIX=%d", check);
}
//--------------------------------------------------------------------------
int PacingGetData()
{
	APPELHS("Ecran,Recupere,CHOIX");
	if (HsEntier == 1) PacingMode = 1;
	if (HsEntier == 2) PacingMode = 3;
	return(1);
}
//--------------------------------------------------------------------------
void PacingLoadData()	   // Transfert base de données => variables
{
	int status;
	int erreur = 1;

	status = DBopen(DB_Runtime_Item,0,""); 	 // Ouverture BdD en lecture

	if (!status)
		{ // Chargement des données modèle de la database
		erreur*=LireDB(DB_Runtime_Item,0,DB_Runtime_TheoProd,(char *)&TheoProd,
					  sizeof(float));
		erreur*=LireDB(DB_Runtime_Item,0,DB_Runtime_MeasProd,(char *)&MeasProd,
					  sizeof(float));
		erreur*=LireDB(DB_Runtime_Item,0,DB_Runtime_ManProd,(char *) &ManProd,
					  sizeof(float));
		erreur*=LireDB(DB_Runtime_Item,0,DB_Runtime_PacingMode,
					  (char *)&PacingMode, sizeof(char));
		DBclose(DB_Runtime_Item);	//ELS 10/12/97

		if (!erreur)
			File_Error(100, DB_Runtime_Name);  	//MsgPace2="Reading Database Error"
		}
	else
		File_Error(101, DB_Runtime_Name); // MsgPace3="Open Database Error"
}
//--------------------------------------------------------------------------
void PacingStoreData() 	  // Sauvegarde variables ds base de données
{
	int status;
	int erreur = 1;

	status = DBopen(DB_Runtime_Item,2,""); 	// Ouverture BdD en écriture

	if (!status)
		{ // Sauvegarde des données modèle de la database
		erreur*=DBwrite(DB_Runtime_Item,0,DB_Runtime_TheoProd,(char *)&TheoProd,
						sizeof(float));
		erreur*=DBwrite(DB_Runtime_Item,0,DB_Runtime_MeasProd,(char *)&MeasProd,
						sizeof(float));
		erreur*=DBwrite(DB_Runtime_Item,0,DB_Runtime_ManProd,(char *)&ManProd,
						sizeof(float));
		erreur*=DBwrite(DB_Runtime_Item,0,DB_Runtime_PacingMode,
				(char *)&PacingMode,sizeof(char));
		DBclose(DB_Runtime_Item);	//ELS 10/12/97

		if (!erreur)
			File_Error(100, DB_Runtime_Name);  	//MsgPace2="Reading Database Error"
		}
	else
		File_Error(101, DB_Runtime_Name); // MsgPace3="Open Database Error"
}