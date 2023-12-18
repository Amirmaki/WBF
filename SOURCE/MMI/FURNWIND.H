// OWLCVT 04/04/97 15:15:39
//-------------------------------------------------------------------------
// furnwind.H
// This file contains function prototype for project FIRST
//
//
//
// Owner source file : furnwind.CPP
//
// Creation : JL COUSIN  date: 31/08/94
//-------------------------------------------------------------------------
#ifndef __FURNWIND_H
#define __FURNWIND_H
#endif

#ifdef __OWNER_FURNWIND_H
//variables
char *Furn_Buf; //buffer contenant une copie de l'ancienne structure
#else
extern char *Furn_Buf;
#endif

//Functions
//---------
//sauvegarde les données de la vue de configuration dans la structure
 void Furn_FillBuffers();
//sauvegarde de la structure dans la base de données
 void Furn_StoreData();
 void Furn_InitWindow();
 void Furn_LoadData();
 void Furn_SetupWindow();
 void Furn_SaveInFile();
 void Furn_LoadFile();
