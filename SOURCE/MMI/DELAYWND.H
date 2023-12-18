// OWLCVT 12/23/96 14:30:49
//-------------------------------------------------------------------------
// Delaywnd.H
// This file contains function prototype for project FIRST
//
//
//
// Owner source file : Delaywnd.CPP
//
// Creation : JL COUSIN  date: 23/09/94
//-------------------------------------------------------------------------

#ifndef __DELAYWND_H
#define __DELAYWND_H

#define NBDELAY 3

//---------------------------------------------------
//Class TConfigDlg: defines the Configuration
//Window for the Dialog CONFIG_1
//see tact.rc
//
//---------------------------------------------------


  //teste si la fenêtre peut être fermée
  //fermée la fenêtre sur condition de validité des paramètres saisis
//  virtual BOOL CanClose();

void Delay_InitWindow();
void Delay_SetupWindow();
void Delay_IncMois();
void Delay_DecMois();
void Delay_IncJour();
void Delay_DecJour();
void Delay_IncAnnee();
void Delay_DecAnnee();
void Delay_IncHeure();
void Delay_DecHeure();
void Delay_IncMin();
void Delay_DecMin();
void Delay_IncSec();
void Delay_DecSec();
void Delay_IncDureJour();
void Delay_DecDureJour();
void Delay_IncDureH();
void Delay_DecDureH();
void Delay_IncDureMin();
void Delay_DecDureMin();
void Delay_Record();
//void Delay_IDCancel();
void Delay_Load();
void Delay_Delete();
void Delay_Stop();
void Delay_Continue();
//  LRESULT Remains( WPARAM );
int Delay_Remains();
// OWLCVT: Unrecognized DDVT value 75 for 'Remains', generic signature used
#ifdef __OWNER_DELAYWND_H
int Delay_IndexList;//position dans la base de données
int Delay_NbRecord;//nombre d'enregistrements dans la base de données
int Delay_EnregSous;//variable pour tester si l'arrêt a été lu dans la liste des arrêts
int Delay_EnCours;//Encours=1 si arrêt en cours sinon 0
int Delay_RecoupGauche;
char Delay_Id[NBDELAY];
char Delay_Debut[NBDELAY][6];
char Delay_DureeInit[NBDELAY][3];
long Delay_Duree[NBDELAY];
int  List_Delay; //position de lecture de la fonction "Load"
#else
extern int Delay_IndexList;//position dans la base de données
extern int Delay_NbRecord;//nombre d'enregistrements dans la base de données
extern int Delay_EnregSous;//variable pour tester si l'arrêt a été lu dans la liste des arrêts
extern int Delay_EnCours;//Encours=1 si arrêt en cours sinon 0
extern int Delay_RecoupGauche;
extern char Delay_Id[NBDELAY];
extern char Delay_Debut[NBDELAY][6];
extern char Delay_DureeInit[NBDELAY][3];
extern long Delay_Duree[NBDELAY];
extern int  List_Delay; //position de lecture de la fonction "Load"
#endif
//Functions
//---------

//Transfert de la base de données vers la structure
 void Delay_LoadData();
 void Delay_StoreData(int Index,char *deb, char *dur);
 void Delay_GetHeure();
 void Delay_SendData();
 int Delay_GetData();
 int Delay_CalcDuree(char *tab, char *Data);
 int Delay_TransformeDate(char *tab1, char *tab2, char *Data);
 void AfficheDelaysList();
 int Delay_ReadDelayToDel();
 int Delay_TestArret();
 long Delay_Calcsec(char ProgDate[6]);
 void Delay_TransformeSecEnJHMin(char tab[4],long NbSec);
 int Delay_RecherchePosition(char debut[7],char fin[4]);

#endif