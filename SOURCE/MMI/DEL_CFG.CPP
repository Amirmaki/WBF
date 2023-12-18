#ifndef __TIME_H
#include <time.h>
#endif

#ifndef __DELAYWND_H
#define __OWNER_DELAYWND_H
#define __OWNER_DELAY_CF_H
#ifndef __CONFIG_H
#include "config.h"
#endif
#ifndef __LANGUAGE_H
#include "language.h"
#endif
#ifndef __DELAYWND_H
#include "delaywnd.h"
#endif
#endif

#ifndef __DATABASE_H
#include "database.h"
#endif
#ifndef __DB_H
#include "db.h"
#endif
#ifndef __GLOBDAT_H
#include "globdat.h"
#endif

void Delay()
{
	 struct date d;
	 struct time t;
	 gettime(&t);
	 getdate(&d);

	if ( *init ==INITIAL)
	{
		Temp  = 2;
		*init = NORMAL;
		Vue_Precedente=REHEAT_CONTROL;
		APPELHS ("Utilise,DELAYS3.AID");
		APPELHS ("Option,Couleur,S=240");    // couleur saisie (fond*16+caractere)
		APPELHS ("Option,Couleur,A=240");

		APPELHS ("Fusionne,logo_cc.img,1,2");
		APPELHS ("Fusionne,del_titl.img,1,36");
		APPELHS ("Sortie,ESC=V,RET=V,HOM=V,END=V,FLH=V,FLB=V,FLD=S,FLG=P,TF1=V,TF2=V,TF3=V,TF4=V,TF5=V,TF6=V,TF7=V,TF8=V,TF9=V,TF0=V");

		Delay_InitWindow();
		Delay_SetupWindow();
	}
	APPELHS ("Ecran,Saisie/nonblocant");

	if (!strcmp(HsTouche,"TF0")||!strcmp(HsTouche,"TF4"))
		Sortie_Ecran(HsTouche);

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

	if (!strcmp(HsTouche,"TF2"))
		Delay_Record();

	if (!strcmp(HsTouche,"TF3"))
		Delay_Load();

	if (!strcmp(HsTouche,"TF6"))
		Delay_Delete();

	if (!strcmp(HsTouche,"TF8"))
		Delay_Stop();

	if (!strcmp(HsTouche,"+++"))
	{
		APPELHS("Ecran,Courant");

		if(!strcmp(HsNom,"ICO_PLUS1 "))  	Delay_IncMois();
		if(!strcmp(HsNom,"ICO_PLUS2 "))  	Delay_IncJour();
		if(!strcmp(HsNom,"ICO_PLUS3 "))  	Delay_IncAnnee();
		if(!strcmp(HsNom,"ICO_PLUS4 "))  	Delay_IncHeure();
		if(!strcmp(HsNom,"ICO_PLUS5 "))  	Delay_IncMin();
		if(!strcmp(HsNom,"ICO_PLUS6 "))  	Delay_IncSec();
		if(!strcmp(HsNom,"ICO_PLUS7 "))  	Delay_IncDureJour();
		if(!strcmp(HsNom,"ICO_PLUS8 "))  	Delay_IncDureH();
		if(!strcmp(HsNom,"ICO_PLUS9 "))  	Delay_IncDureMin();
	}

	if (!strcmp(HsTouche,"---"))
	{
		APPELHS("Ecran,Courant");

		if(!strcmp(HsNom,"ICO_MOINS1"))  	Delay_DecMois();
		if(!strcmp(HsNom,"ICO_MOINS2"))  	Delay_DecJour();
		if(!strcmp(HsNom,"ICO_MOINS3"))  	Delay_DecAnnee();
		if(!strcmp(HsNom,"ICO_MOINS4"))  	Delay_DecHeure();
		if(!strcmp(HsNom,"ICO_MOINS5"))  	Delay_DecMin();
		if(!strcmp(HsNom,"ICO_MOINS6"))  	Delay_DecSec();
		if(!strcmp(HsNom,"ICO_MOINS7"))  	Delay_DecDureJour();
		if(!strcmp(HsNom,"ICO_MOINS8"))  	Delay_DecDureH();
		if(!strcmp(HsNom,"ICO_MOINS9"))  	Delay_DecDureMin();
	}

	Delay_EnCours = Delay_TestArret();//initialisation


	if (error < 100)
		if (Delay_EnCours == 1)
			Delay_Remains();
}
//---------------------------------------------------------------------------

void Delay_InitWindow()
{
  List_Delay=-1;
  Delay_EnregSous=0;
  Delay_RecoupGauche=0;
  Delay_EnCours=Delay_TestArret();//initialisation
//  EnableAutoCreate();

}
//initialisation  des données dans la strcture
//********************************************

void Delay_GetHeure()
{
	 struct date d;
	 struct time t;
	 gettime(&t);
	 Heure = t.ti_hour;
	 Min   = t.ti_min;
	 Sec   = t.ti_sec;
	 getdate(&d);
	 Annee = (d.da_year) % 100; //correction 1994 ->94
	 Mois  = d.da_mon;
	 Jour  = d.da_day;
	 DureH = 0;
	 DureMin = 0;
	 DureJour = 0;
}

void Delay_SendData()
{
	APPELHS("Affiche,MOIS=%d",Mois);
	APPELHS("Affiche,DAY=%d",Jour);
	APPELHS("Affiche,YEAR=%d",Annee);
	APPELHS("Affiche,HOUR=%d",Heure);
	APPELHS("Affiche,MIN=%d",Min);
	APPELHS("Affiche,SEC=%d",Sec);
	APPELHS("Affiche,DAY2=%d",DureJour);
	APPELHS("Affiche,HOUR2=%d",DureH);
	APPELHS("Affiche,MIN2=%d",DureMin);
}

void Delay_SetupWindow()
{
	Delay_GetHeure();
	Delay_SendData();
	Delay_LoadData();
	AfficheDelaysList();

//	if (TestArret()) ::PostMessage(HWindow,WM_ARRET,0,0L);
}

void Delay_IncMois()
{
	if (Mois++ < 12)
		APPELHS("Affiche,MOIS=%d",Mois);
	else
		APPELHS("Affiche,MOIS=%d",Mois=1);
}

void Delay_DecMois()
{
	if (Mois-- > 1)
		APPELHS("Affiche,MOIS=%d",Mois);
	else
		APPELHS("Affiche,MOIS=%d",Mois=12);
}

void Delay_IncJour()
{
	if (Jour++ < 31 )
		APPELHS("Affiche,DAY=%d",Jour);
	else
		APPELHS("Affiche,DAY=%d",Jour=1);
}

void Delay_DecJour()
{
	if (Jour-- > 1)
		APPELHS("Affiche,DAY=%d",Jour);
	else
		APPELHS("Affiche,DAY=%d",Jour=31);
}

void Delay_IncAnnee()
{
	if (Annee++ < 99 )
		APPELHS("Affiche,YEAR=%d",Annee);
	else
		APPELHS("Affiche,YEAR=%d",Annee=0);
}

void Delay_DecAnnee()
{
	if (Annee-- > 0)
		APPELHS("Affiche,YEAR=%d",Annee);
	else
		APPELHS("Affiche,YEAR=%d",Annee=99);
}

void Delay_IncHeure()
{
	if (Heure++ < 23 )
		APPELHS("Affiche,HOUR=%d",Heure);
	else
		APPELHS("Affiche,HOUR=%d",Heure=0);
}

void Delay_DecHeure()
{
	if (Heure-- > 0)
		APPELHS("Affiche,HOUR=%d",Heure);
	else
		APPELHS("Affiche,HOUR=%d",Heure=23);
}

void Delay_IncMin()
{
	if (Min++ < 59 )
		APPELHS("Affiche,MIN=%d",Min);
	else
		APPELHS("Affiche,MIN=%d",Min=0);
}

void Delay_DecMin()
{
	if (Min-- > 0)
		APPELHS("Affiche,MIN=%d",Min);
	else
		APPELHS("Affiche,MIN=%d",Min=59);
}

void Delay_IncSec()
{
	if (Sec++ < 59 )
		APPELHS("Affiche,SEC=%d",Sec);
	else
		APPELHS("Affiche,SEC=%d",Sec=0);
}

void Delay_DecSec()
{
	if (Sec-- > 0)
		APPELHS("Affiche,SEC=%d",Sec);
	else
		APPELHS("Affiche,SEC=%d",Sec=59);
}

void Delay_IncDureJour()
{
	if (DureJour++ < 31 )
		APPELHS("Affiche,DAY2=%d",DureJour);
	else
		APPELHS("Affiche,DAY2=%d",DureJour=1);
}

void Delay_DecDureJour()
{
	if (DureJour-- > 1)
		APPELHS("Affiche,DAY2=%d",DureJour);
	else
		APPELHS("Affiche,DAY2=%d",DureJour=31);
}

void Delay_IncDureH()
{
	if (DureH++ < 23 )
		APPELHS("Affiche,HOUR2=%d",DureH);
	else
		APPELHS("Affiche,HOUR2=%d",DureH=0);
}

void Delay_DecDureH()
{
	if (DureH-- > 0)
		APPELHS("Affiche,HOUR2=%d",DureH);
	else
		APPELHS("Affiche,HOUR2=%d",DureH=23);
}

void Delay_IncDureMin()
{
	if (DureMin++ < 59 )
		APPELHS("Affiche,MIN2=%d",DureMin);
	else
		APPELHS("Affiche,MIN2=%d",DureMin=0);
}

void Delay_DecDureMin()
{
	if (DureMin-- > 0)
		APPELHS("Affiche,MIN2=%d",DureMin);
	else
		APPELHS("Affiche,MIN2=%d",DureMin=59);
}
//*****************************************************
//enregistrement des arrêts
//*****************************************************
void Delay_Record()
{
	if (Delay_EnregSous)//test si l'arrêt a été chargé
	{
//	  //interroge l'opérateur pour écraser l'arrêt chargé
		*init = INITIAL;
		vue   = DELAY_CONFIRM;
#ifdef DBG_VUE
		fstream f;
		f.open("vue.txt", ios::out);
		f << vue;
		f.close();
#endif
		Confirm_OverWrite();
	}
	else
		Delay_Continue();
}
void Delay_Continue()
{
	if (Delay_NbRecord<NBDELAY)
	{
		if(Delay_GetData())
		{
			//char aux[9];
			//char aux1[9];
			//char aux2[14];
			char deb[7];
			char fn[4];
			//affectation des données dans des tableaux
			deb[0]=(char) Jour;
			deb[1]=(char) Mois;
			deb[2]=(char) Annee;
			deb[3]=(char) Heure;
			deb[4]=(char) Min;
			deb[5]=(char) Sec;
			deb[6]= (char) '\0';
			fn[0] = (char) DureJour;
			fn[1] = (char) DureH;
			fn[2] = (char) DureMin;
			fn[3]= (char) '\0';

			int Duree = DureJour+DureH+DureMin;
			if(Duree > 0)
			{
				int ThePos= Delay_RecherchePosition(deb,fn);

				if (ThePos>=0) //insertion de l'arrêt à la position Index
				{
					(void)DBinsert(DB_Delays_Item,ThePos,"");
					Delay_StoreData(ThePos,deb,fn);
				}
			}
		}
	}
	else
	{
			error = 107;         // "Too many delays ...!"
			*init = INITIAL;
			vue   = ERROR_WINDOW;
#ifdef DBG_VUE
			fstream f;
			f.open("vue.txt", ios::out);
			f << vue;
			f.close();
#endif
			Window_Error();
	}

	//remise à jour des arrêts
	if (error < 106)
	{
		Delay_LoadData();
		AfficheDelaysList();
	}
}
 /*
//******************************************************************
//teste si la fenêtre peut être fermée
//ferme la fenêtre sur condition de validité des paramètres saisis
//******************************************************************
BOOL TConfigDlg6::CanClose()
{
  return TRUE;
}
 */

//**************************************************
//Transfert de la base de données vers la structure
//**************************************************
void Delay_LoadData()
{
	int status; //status d'ouverture
	int erreur; // si pb de lecture  erreur = 0

	//ouverture de la base de données en lecture
	status = DBopen(DB_Delays_Item,0,"");

	//lecture du nombre d'enregistrements
	Delay_NbRecord = DBNbRecord(DB_Delays_Item);

	if (!status)
	{
		// chargement des données modèle de la database
		for (int i=0;i<Delay_NbRecord;i++)
		{
			erreur *= LireDB(DB_Delays_Item,i,DB_Delays_Id,(char *) &Delay_Id[i], sizeof(char));
			erreur *= LireDB(DB_Delays_Item,i,DB_Delays_Debut,(char *) Delay_Debut[i], 6*sizeof(char));
			erreur *= LireDB(DB_Delays_Item,i,DB_Delays_Fin,(char *) Delay_DureeInit[i], 3*sizeof(char));
			erreur *= LireDB(DB_Delays_Item,i,DB_Delays_Duree,(char *) &Delay_Duree[i], sizeof(long));
		}

		if (!erreur)
			File_Error(100,DB_Delays_Name);  	//"Reading database error"

	 //fermeture de la database
		DBclose(DB_Delays_Item);
	}
	else
		File_Error(101,DB_Delays_Name); //problème d'ouverture du fichier
}

//**************************************************
//Enregistrement des arrêts dans la base de données
//**************************************************
void Delay_StoreData(int Index,char *deb, char *dur)
{
  int status; //status d'ouverture
  int erreur; // si pb de lecture  erreur = 0

  //ouverture de la base de données en lecture
  status = DBopen(DB_Delays_Item,1,"");

	if (!status)
	{
	 // écriture des données dans la database
		char Ind = (char)Index;
		erreur = DBwrite(DB_Delays_Item,Index,DB_Delays_Id,(char *) &Ind, sizeof(char));
		erreur *= DBwrite(DB_Delays_Item,Index,DB_Delays_Debut,(char *) deb, 6*sizeof(char));
		erreur *= DBwrite(DB_Delays_Item,Index,DB_Delays_Fin,(char *) dur, 3*sizeof(char));
		long DurSec = ((24*(long)dur[0] + (long)dur[1])*60 + (long) dur[2]) * 60;
		erreur *= DBwrite(DB_Delays_Item,Index,DB_Delays_Duree,(char *) &DurSec, sizeof(long));


	if (!erreur)
		File_Error(100,DB_Delays_Name);  	//"Reading database error"

	 //fermeture de la database
	DBclose(DB_Delays_Item);

	}
	else
		File_Error(101,DB_Delays_Name);	   //problème d'ouverture du fichier

	if(!status && erreur)
	{
		status = DBopen(DB_Delays_Item,0,"");
		//lecture du nombre d'enregistrements
		Delay_NbRecord = DBNbRecord(DB_Delays_Item);
		DBclose(DB_Delays_Item);
	}
}

//**************************************************
//affichage des arrêts à l'écran dans la liste
//**************************************************
void AfficheDelaysList()
{
	char aux[40];
	char aux1[40];
	char aux2[40];
	char temp1[4]=" ";

	for (int i=0;i<Delay_NbRecord;i++)
	{
		Delay_TransformeDate(aux,aux1,Delay_Debut[i]);
		strcat(aux,temp1);
		strcat(aux,aux1);
		APPELHS("Affiche,DEBUT,%d=%s",i+1,aux);

		Delay_CalcDuree(aux2, Delay_DureeInit[i]);
		APPELHS("Affiche,DUREE,%d=%s",i+1,aux2);
	}
	strcpy(aux,"");strcpy(aux1,"");strcpy(aux2,"");

	for(i;i<NBDELAY;i++)
	{
		APPELHS("Affiche,DEBUT,%d=%s",i+1,aux);
		APPELHS("Affiche,DUREE,%d=%s",i+1,aux2);
	}
}
//*************************************************
//transforme les données issue de la base de donnée
//en chaine de caractères à afficher à l'écran
//*************************************************
int Delay_TransformeDate(char *tab1, char *tab2, char *Data)
{
	int val1,val2,val3;
	val1 =(int)Data[1]; //mois
	val2 =(int)Data[0]; //jour
	val3 =(int)Data[2] % 100;//année
	sprintf(tab1,"%2.2i/%2.2i/%2.2i",val2,val1,val3);

	val1 =(int)Data[3]; //heure
	val2 =(int)Data[4]; //min
	val3 =(int)Data[5];//sec
	sprintf(tab2,"%02.2i:%02.2i:%02.2i",val1,val2,val3);

	return(0);
 }
//*************************************************
//transforme les données issue de la base de donnée
//en chaine de caractères à afficher à l'écran
//*************************************************
int Delay_CalcDuree(char *tab, char *Data)
{ //char Tab[3];
	int val1,val2,val3;
	strcpy(tab,"");
	val1 =(int)Data[0]; //jour
	val2 =(int)Data[1]; //heure
	val3 =(int)Data[2]; //min
	sprintf(tab,"%02.2id%02.2ih%02.2imn",val1,val2,val3);
	return(0);
}

//*************************************************
//lecture de l'arrêt configuré
//*************************************************
int Delay_GetData()
{
	APPELHS ("Ecran,Recupere,MOIS");
	sscanf(HsChaine,"%d",&Mois);

	APPELHS ("Ecran,Recupere,DAY");
	sscanf(HsChaine,"%d",&Jour);

	APPELHS ("Ecran,Recupere,YEAR");
	sscanf(HsChaine,"%d",&Annee);

	if (Annee > -1 && Annee < 70)
		Annee = Annee + 100;

	APPELHS ("Ecran,Recupere,HOUR");
	sscanf(HsChaine,"%d",&Heure);

	APPELHS ("Ecran,Recupere,MIN");
	sscanf(HsChaine,"%d",&Min);

	APPELHS ("Ecran,Recupere,SEC");
	sscanf(HsChaine,"%d",&Sec);

	APPELHS ("Ecran,Recupere,DAY2");
	sscanf(HsChaine,"%d",&DureJour);

	APPELHS ("Ecran,Recupere,HOUR2");
	sscanf(HsChaine,"%d",&DureH);

	APPELHS ("Ecran,Recupere,MIN2");
	sscanf(HsChaine,"%d",&DureMin);

	return(1);
}

//*************************************************
//lecture de l'arrêt configuré
//*************************************************
void Delay_Load()
{
	if (Delay_NbRecord)
	{
		 //positionnement dans la liste des enregistrements
		Delay_EnregSous=1;

		if(List_Delay<Delay_NbRecord-1)
			List_Delay++;
		else
			List_Delay=0;

		Delay_LoadData();
		Jour     = (int) Delay_Debut[List_Delay][0];
		Mois     = (int) Delay_Debut[List_Delay][1];
		Annee    = (int) Delay_Debut[List_Delay][2] % 100;
		Heure    = (int) Delay_Debut[List_Delay][3];
		Min      = (int) Delay_Debut[List_Delay][4];
		Sec      = (int) Delay_Debut[List_Delay][5];
		DureJour = (int) Delay_DureeInit[List_Delay][0];
		DureH    = (int) Delay_DureeInit[List_Delay][1];
		DureMin  = (int) Delay_DureeInit[List_Delay][2];

		Delay_SendData();
	}
	else
	{
			error = 109;       // "No delay has been programmed ...!"
			*init = INITIAL;
			vue   = ERROR_WINDOW;
#ifdef DBG_VUE
			fstream f;
			f.open("vue.txt", ios::out);
			f << vue;
			f.close();
#endif
			Window_Error();
	}
}


//*************************************************
//destruction de l'arrêt affiché
//*************************************************
void Delay_Delete()
{
	int res;
	//int status;
	Delay_EnregSous=0;//RAZ de l'indicateur de chargement(fnct load)

	res = Delay_ReadDelayToDel();

	if (res>=0)
	{
		//destruction
		(void)DBdelete(DB_Delays_Item,res,"");
		Delay_NbRecord--;
		//réaffichage de la vue;
		List_Delay=-1;
		Delay_GetHeure();
		Delay_SendData();
		Delay_LoadData();
		AfficheDelaysList();

		if (!res)//RAZ du Running
		{
			APPELHS("Affiche,PROCFROM=%s"," ");
			APPELHS("Affiche,PROCREMAIN=%s"," ");
		}
	}
	else
	{
		error = 108;         //"This programmed delay wasn't defined in the list"
		*init = INITIAL;
		vue   = ERROR_WINDOW;
#ifdef DBG_VUE
		fstream f;
		f.open("vue.txt", ios::out);
		f << vue;
		f.close();
#endif
		Window_Error();
	}
}

//*************************************************
//cette fonction renvoie l'arrêt à détruire s'il existe
// sinon -1
//*************************************************
int Delay_ReadDelayToDel()
{
	if (Delay_GetData())
	{
		int OK=0;
		int TheReturn=-1;
		for (int i=0;i<Delay_NbRecord && !OK;i++)
		{
			if( Jour 	  == (int) Delay_Debut[i][0] &&
				 Mois 	  == (int) Delay_Debut[i][1] &&
				 Annee 	  == (int) Delay_Debut[i][2] &&
				 Heure 	  == (int) Delay_Debut[i][3] &&
				 Min 		  == (int) Delay_Debut[i][4] &&
				 Sec	 	  == (int) Delay_Debut[i][5] &&
				 DureJour  == (int) Delay_DureeInit[i][0] &&
				 DureH 	  == (int) Delay_DureeInit[i][1] &&
				 DureMin   == (int) Delay_DureeInit[i][2] )
			{
				OK=1;
				TheReturn=i;
			}
		}//end for
		return(TheReturn);
	}//if (getdata())
	else
	return(-1);
}

//*********************************************
//affiche la duree restante de l'arrêt en court
//*********************************************
//LRESULT TConfigDlg6::Remains( WPARAM )
int Delay_Remains()
{

// OWLCVT: Unrecognized DDVT value 75 for 'TConfigDlg6::Remains', generic signature used
	char aux[40];
	char aux1[9];
	char aux2[14];
	char fn[4];
	char temp[10]="";

	//lecture dans la base de données
	Delay_LoadData();

	if (Delay_NbRecord)
	{
		//affichage de la date de l'arrêt programmé
		Delay_TransformeDate(aux,aux1,Delay_Debut[0]);
		//transformation de la durée sec -> jour, heure, min
		Delay_TransformeSecEnJHMin(fn,Delay_Duree[0]);
		//transformation de ces données en chaine de caractères
		//et affichage
		Delay_CalcDuree(aux2, fn);

		if (!((int)fn[0]) && !((int)fn[1]) && !((int)fn[2]))
		{
			Delay_EnCours=0;
			strcpy(aux2,"");strcpy(aux1,"");strcpy(aux,"");

			if (Delay_NbRecord > 0)
			{
				(void)DBdelete(DB_Delays_Item,0,"");
				Delay_NbRecord--;
				//réaffichage de la vue;
				List_Delay=-1;
				Delay_GetHeure();
				Delay_SendData();
				Delay_LoadData();
				AfficheDelaysList();
			}
			APPELHS("Affiche,PROCFROM=%s"," ");
			APPELHS("Affiche,PROCREMAIN=%s"," ");
		}
		else
		{
			Delay_EnCours=1;
			APPELHS("Affiche,PROCREMAIN=%s",aux2);

			strcat(aux,temp);
			strcat(aux,aux1);

		  APPELHS("Affiche,PROCFROM=%s",aux);
		}
	}
	return (0);
}
//****************************************************
//cette fonction teste si le premier arrêt est encourt
//****************************************************
int Delay_TestArret()
{
	if (Delay_NbRecord)
	{
		long dureeInit = ((24*(long)Delay_DureeInit[0][0] +(long)Delay_DureeInit[0][1])*60 + (long)Delay_DureeInit[0][2])*60;
		if (Delay_Duree[0]<dureeInit)
			return(1);
		else
			return(0);
	}
	else return(0);
 }


//*****************************************************
//cette fonction stop l'arrêt en court en le détruisant
// et en effectuant une RAZ de l'arrêt en court
//*****************************************************
void Delay_Stop()
{
	if (Delay_NbRecord>0 && Delay_EnCours)
	{
		(void)DBdelete(DB_Delays_Item,0,"");
		Delay_NbRecord--;
		//réaffichage de la vue;
		List_Delay=-1;
		Delay_GetHeure();
		Delay_SendData();
		Delay_LoadData();
		AfficheDelaysList();
	}
	APPELHS("Affiche,PROCFROM=%s","");
	APPELHS("Affiche,PROCREMAIN=%s","");
}

//*********************************************************************
//cette fonction calcule le nombre de secondes écoulées depuis le début
//de l'année jusqu'à la date ProgDate
//remarque: si l'arrêt est programmé après un changement d'année
//          ajout du nb de seconde écoulé pendant l'année
//	    ex: date courante 29/12/94, arrêt pg: 01/01/95
//	    =>ajout du nb de seconde écoulées pendant l'année 94
//cette fonction renvoie -1 si le nombre de jours est supérieur
//au nombre de jours contenu dans le mois
//*********************************************************************
long Delay_Calcsec(char ProgDate[6])
{
	struct tm PgDate;
	PgDate.tm_hour = (int)ProgDate[3];
	PgDate.tm_min  = (int)ProgDate[4];
	PgDate.tm_sec  = (int)ProgDate[5];
	PgDate.tm_year = (int)ProgDate[2];
	PgDate.tm_mon  = (int)ProgDate[1] - 1;
	PgDate.tm_mday = (int)ProgDate[0];

	if (mktime(&PgDate) ==-1)
		return (-1);

	else
	{
		//calcule du nombre de secondes.
		long resSec;
		long nbJour= PgDate.tm_yday;
		long heure = PgDate.tm_hour;
		long min = PgDate.tm_min;
		long sec = PgDate.tm_sec;
		resSec = ((nbJour*24 + heure)*60 + min)*60 + sec;

		if (PgDate.tm_mday !=((int) ProgDate[0]))
			return(-1);//date non correcte ex 31 fev
		else
		{
			//correction si changement d'année entre l'année en court
			//et l'année de l'arrêt programmé
			struct date dt;
			getdate(&dt);

			if (PgDate.tm_year> (dt.da_year % 100))
			{//changement d'année
				PgDate.tm_hour = 23;
				PgDate.tm_min =  59;
				PgDate.tm_sec =  59;
				PgDate.tm_year = (dt.da_year % 100);
				PgDate.tm_mon =  12-1;
				PgDate.tm_mday = 31;

				if (mktime(&PgDate) ==-1)
					return(-1);
				else
				{
					//ajout du nombre de secondes écoulées
					//pendant l'année précédente depuis le 01 janvier 0h00
					nbJour= PgDate.tm_yday;
					heure = PgDate.tm_hour;
					min = PgDate.tm_min;
					sec = PgDate.tm_sec;
					resSec += ((nbJour*24 + heure)*60 + min)*60 + sec;
				}
			}
		}
		return (resSec);
	}
}
//***************************************************************
//transforme une duree exprimée en sec en J Heure Minute
//***************************************************************
void  Delay_TransformeSecEnJHMin(char tab[4],long NbSec)
{
	long LaDuree = NbSec;
	long nbjour  = LaDuree/86400L;
	LaDuree      = LaDuree - nbjour*86400L;
	long nbheure = LaDuree/3600;
	LaDuree      = LaDuree - nbheure*3600;
	long nbmin   = LaDuree/60;
//     LaDuree    = LaDuree - nbmin*60;

	if (nbjour<0) nbjour=0;
	if (nbheure<0) nbheure=0;
	if (nbmin<0) nbmin=0;

	tab[0] = (char) nbjour;
	tab[1] = (char) nbheure;
	tab[2] = (char) nbmin;
	tab[3]= (char) '\0';
}

 //**************************************************************************
 //cette fonction effectue:
 //*un test  de validité de l'arrêt
 //* une recherche de l'emplacement où insérer l'arrêt
 //*une détection de recoupement d'arrêt et modification
 //valeur renvoyée:
 //>=0 : position où insérer l'arrêt
 //-1 date non valide
 //-2 recoupement à gauche
 //-3 recoupement à droite
 //**************************************************************************
int Delay_RecherchePosition(char debut[7],char fin[4])
{
	long DateSec[NBDELAY];//date de début d'arrêt en sec
	long DureSec[NBDELAY];//durée de l'arrêt en seconde
	long RefDateSec;//date de début d'arrêt à insérer en sec
	long RefDureSec;//durée de l'arrêt à insérer en seconde
	long RecoupGaucheDurSec; //durée de l'arrêt après correction recoupement gauche

	 //calcul des durées en secondes
	for (int i = 0 ; i < Delay_NbRecord ; i++)
	{
		DateSec[i] = Delay_Calcsec(Delay_Debut[i]);
		DureSec[i] = (( 24*((long)Delay_DureeInit[i][0]) + (long)Delay_DureeInit[i][1])*60
		 + (long) Delay_DureeInit[i][2]) * 60;
	}
	RefDateSec = Delay_Calcsec(debut);
	RefDureSec = (((24*(long)fin[0]) + (long)fin[1])*60 + (long) fin[2]) * 60;

	if (RefDateSec < 0) //date non valide
	{
		error = 106; 	// "The delays is unvalid ...!"
		*init = INITIAL;
		vue   = ERROR_WINDOW;
#ifdef DBG_VUE
		fstream f;
		f.open("vue.txt", ios::out);
		f << vue;
		f.close();
#endif
		Window_Error();
		return(-1);
	}

	int Index = 0; //index de la position où va s'insérer l'arrêt
	//recherche de la position où insérer l'arrêt

	for (i=0;i<Delay_NbRecord;i++)
	{
		if (RefDateSec >DateSec[i])
		Index++;
	}

	if (Index >0)
	{
		//recherche si il y a un recoupement avec la l'arrêt de gauche
		if ((DateSec[Index-1] + DureSec[Index-1])>=RefDateSec)
		{
			//calcule de la nouvelle durée
			long IncDur = RefDateSec - DateSec[Index-1] + RefDureSec;
			//transformation de la durée sec -> jour, heure, min
			Delay_TransformeSecEnJHMin(fin,IncDur);
			//ecriture dans la base de données
			Delay_StoreData(Index-1,Delay_Debut[Index-1],fin); //incrémentation de la durée de l'arrêt de gauche
			RecoupGaucheDurSec = IncDur;
			Delay_RecoupGauche=1;
		}
		else
			Delay_RecoupGauche=0;
	}
	if (Index < Delay_NbRecord)
	{
		if (RefDateSec ==DateSec[Index])
		{
			//calcule de la  durée max
			long DurMax;
			if (RefDureSec>DureSec[Index])
				DurMax = RefDureSec;
			else
				DurMax = DureSec[Index];

			Delay_TransformeSecEnJHMin(fin,DurMax);
			//ecriture dans la base de données
			Delay_StoreData(Index,debut,fin);//Modification de la date de début de l'arrêt de droite
			RecoupGaucheDurSec = DurMax;
			Delay_RecoupGauche=1;
		}

		int IndexRef=Index;
		//recherche si il y a un recoupement avec la l'arrêt de droite

		if((RefDateSec + RefDureSec)>=DateSec[Index])//test si un arrêt est redondant
		{
			if (Index<Delay_NbRecord-1)
			{
				Index++;
				if((RefDateSec + RefDureSec)>=DateSec[Index])
				{
					DBdelete(DB_Delays_Item,IndexRef,"");//destruction
					Delay_NbRecord--;
				}
				else Index--;
			}
		}

		if ((RefDateSec + RefDureSec)>=DateSec[Index])
		{
		//calcule de la nouvelle durée
			long dt =(DateSec[Index] - RefDateSec);
			long IncDur;

			if (DureSec[Index] >= (RefDureSec-dt))
				IncDur = dt +  DureSec[Index];
			else
				IncDur = RefDureSec;

			//transformation de la durée sec -> jour, heure, min
			Delay_TransformeSecEnJHMin(fin,IncDur);
			//ecriture dans la base de données

			if (Delay_RecoupGauche) //recoupement gauche et recoupement droite
			{
				//arrêt à insérer  = arrêt modifié
				long ModRefDateSec = Delay_Calcsec(debut);
				long ModRefDureSec =(((24*(int)fin[0]) + (int)fin[1])*60 + (int) fin[2]) * 60;

				//recherche si il y a un recoupement avec la l'arrêt de gauche
				if ((DateSec[Index-1] + RecoupGaucheDurSec)>=ModRefDateSec)
				{
					//calcule de la nouvelle durée
					long IncDur = ModRefDateSec - DateSec[Index-1] + ModRefDureSec;
					//transformation de la durée sec -> jour, heure, min
					Delay_TransformeSecEnJHMin(fin,IncDur);
					DBdelete(DB_Delays_Item,Index,"");//destruction
					Delay_StoreData(Index-1,Delay_Debut[Index-1],fin);//Modification de la date de début de l'arrêt de droite
				}
			}
			else
			{
				Delay_StoreData(IndexRef,debut,fin);//Modification de la date de début de l'arrêt de droite
				return(-3);
			}
		}
	}

	if (Delay_RecoupGauche)
	{
		Delay_RecoupGauche=0;
		return(-2);
	}
	return(Index);
}
