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
int Prod_IndexList;//position dans la base de donn�es
int Prod_NbRecord;//nombre d'enregistrements dans la base de donn�es
#else
extern char *Prod_Buf;
extern int Prod_IndexList;//position dans la base de donn�es
extern int Prod_NbRecord;//nombre d'enregistrements dans la base de donn�es
#endif

//Functions
//---------
//sauvegarde les donn�es de la vue de configuration dans la structure
 void Prod_FillBuffers();

//Transfert de la base de donn�es vers la structure
 void Prod_LoadData(int Prod_Index);

//sauvegarde de la structure dans la base de donn�es
 void Prod_StoreData(int Prod_Index);

//fonction d'affichage des donn�es � l'�cran pour remise � jour
// et d�placement
void Affiche();

//initialisation  des donn�es dans la strcture
void Init();

//cette fonction recherche la courbe d'objectifs selectionn�e � d�truire.
int ReadObjToDel();

//permutation de 2 courbes objectifs
int Permute(int n1, int n2);

//trie des courbes objectifs (M�thode: trie � bulles)
int TriBulle();

//d�s�lectionne les cases � cocher
void Unselect();

//affecte la valeur de num de fa�on croissante
void AffecteNum();

void Prod_LoadFile();

#endif