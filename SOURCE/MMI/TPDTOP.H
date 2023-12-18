// OWLCVT 12/23/96 14:35:13
//-------------------------------------------------------------------------
// TPdtop.H
// This file contains function prototype for project FIRST
//
//
//
// Owner source file : TPdtop.CPP
//
// Creation : JL COUSIN  date: 02/09/94
//-------------------------------------------------------------------------

#ifndef __TPDTOP_H
#define __TPDTOP_H

//  virtual BOOL CanClose();

//  virtual bool Create();

  //fermeture de la fenêtre de configuration
  //	void Close();
  void PrdSel_InitWindow();
  void PrdSel_SetupWindow();

  void PrdSel_Up();
  void PrdSel_PgUp();
  void PrdSel_Down();
  void PrdSel_PgDown();
  void PrdSel_Home();
  void PrdSel_End();

//variables
#ifdef __OWNER_TPDTOP_H
//variables
char *PrdSel_Buf; //buffer contenant une copie de l'ancienne structure
int PrdSel_IndexList;//position dans la base de données
int PrdSel_NbRecord;//nombre d'enregistrements dans la base de données
int PrdSel_NBRecordBatch; //nombre de Batch Existant
int PrdSel_SelType; //numéro du type de produit selectionné

#else
extern char *PrdSel_Buf;
extern int PrdSel_IndexList;//position dans la base de données
extern int PrdSel_NbRecord;//nombre d'enregistrements dans la base de données
extern int PrdSel_NBRecordBatch; //nombre de Batch Existant
extern int PrdSel_SelType; //numéro du type de produit selectionné

#endif

//Functions
//---------

//Transfert de la base de données vers la structure
void PrdSel_LoadData(int Index);

//fonction d'affichage des données à l'écran pour remise à jour
// et déplacement
void PrdSel_Affiche();

//lecture du type de produit selectionné dans la base de données
void PrdSel_LectSelect();

//Ecriture du type de produit selectionné dans la base de données
void PrdSel_WriteSelect();

//detection du type de produit selectionné à l'écran
void GetSelect();
void PrdSel_GetSelect();
void PrdSel_AffichePoint();

//initialisation  des données dans la strcture
void PrdSel_Init();

//construit le batch
int PrdSel_MakeBatch();

//selectionne le type de produit
void PrdSel_Select(char *);

#endif