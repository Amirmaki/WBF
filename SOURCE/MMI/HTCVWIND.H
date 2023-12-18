// OWLCVT 04/04/97 15:15:40
//-------------------------------------------------------------------------
// HtCvwind.H
// This file contains function prototype for project FIRST
//
//
//
// Owner source file : HtCvwind.CPP
//
// Creation : JL COUSIN  date: 02/09/94
//-------------------------------------------------------------------------
#define TRUE  1
#define FALSE 0

#ifndef __HTCVWIND_H
#define __HTCVWIND_H

//---------------------------------------------------

void Heat_SetupWindow();
void Heat_SaveInFile();
void Heat_LoadFile();
void Heat_Up();
void Heat_PgUp();
void Heat_Down();
void Heat_PgDown();
void Heat_Home();
void Heat_End();
void Heat_Del();

#ifdef __OWNER_HTCVWIND_H
//variables
char *Heat_Buf; //buffer contenant une copie de l'ancienne structure
int Heat_IndexList;//position dans la base de données
int Heat_NbRecord;//nombre d'enregistrements dans la base de données
#else
extern char *Heat_Buf;
extern int Heat_IndexList;
extern int Heat_NbRecord;
#endif

void Heat_InitWindow();
void Heat_FillBuffers();
void Heat_LoadData(int Index);
void Heat_StoreData(int Index);
void Heat_Affiche();
//initialisation  des données dans la strcture
void Heat_Init();

//cette fonction recherche la courbe d'objectifs selectionnée à détruire.
int Heat_ReadObjToDel();

//permutation de 2 courbes objectifs
int Heat_Permute(int n1, int n2);

//trie des courbes objectifs (Mèthode: trie à bulles)
int Heat_TriBulle();

//désélectionne les cases à cocher
void Heat_Unselect();

//affecte la valeur de num de façon croissante
void Heat_AffecteNum();

#endif
