// OWLCVT 04/04/97 15:15:40
//-------------------------------------------------------------------------
// TPdtvwind.H
// This file contains function prototype for project FIRST
//
//
//
// Owner source file : TPdtwind.CPP
//
// Creation : JL COUSIN  date: 02/09/94
//-------------------------------------------------------------------------

#ifndef __TPDTWIND_H
#define __TPDTWIND_H
#define TRUE  1
#define FALSE 0

void Prod_InitWindow();
void Prod_SetupWindow();
//sauvegarde de la structure dans le fichier de configuration
// *.cfg
void Prod_SaveInFile();
void Up();
void PgUp();
void Down();
void PgDown();
void Home();
void End();
void Del();

//variables
#ifdef __OWNER_TPDTWIND_H
//variables
char *Prod_Buf; //buffer contenant une copie de l'ancienne structure
int Prod_IndexList;//position dans la base de données
int Prod_NbRecord;//nombre d'enregistrements dans la base de données
#else
extern char *Prod_Buf;
extern int Prod_IndexList;//position dans la base de données
extern int Prod_NbRecord;//nombre d'enregistrements dans la base de données
#endif

//Functions
//---------
//sauvegarde les données de la vue de configuration dans la structure
 void Prod_FillBuffers();

//Transfert de la base de données vers la structure
 void Prod_LoadData(int Prod_Index);

//sauvegarde de la structure dans la base de données
 void Prod_StoreData(int Prod_Index);

//fonction d'affichage des données à l'écran pour remise à jour
// et déplacement
void Affiche();

//initialisation  des données dans la strcture
void Init();

//cette fonction recherche la courbe d'objectifs selectionnée à détruire.
int ReadObjToDel();

//permutation de 2 courbes objectifs
int Permute(int n1, int n2);

//trie des courbes objectifs (Mèthode: trie à bulles)
int TriBulle();

//désélectionne les cases à cocher
void Unselect();

//affecte la valeur de num de façon croissante
void AffecteNum();

void Prod_LoadFile();

#endif