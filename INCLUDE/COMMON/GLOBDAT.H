//-------------------------------------------------------------------------
// GLOBDAT.H
// This file contains all global macro for project FOUR
//
// Owner source file : FOUR.CPP
//
// Creation : C.Martinet 25/01/92
// Modif                    06/92
//			 16/07/92
//			 18/08/92 add TheHisList
//			 17/09/92 add delay time data
//			 16/02/93 energy his period
// Modif PF12/01/95 : Suppression de la liste des fichiers historiques
//		      temporaires TheHisList !
//		      Ajout de NbProdFour : Nombre de produits max ds four
// 	 PF09/03/95 : Gestion des codes aciers
//-------------------------------------------------------------------------

#ifndef __GLOBDAT_H
#define __GLOBDAT_H

//Macro definitions
//-------------------------------------------------------------------------
// Pacing declaration
#define PACINGMODE
// Decarburation model
#define DECARBURATION
// Setpoint filter declaration
//#define SPTFILTER
// Put next line in commentary line if no debbuging are required
#define DEBBUGING
// Pour affichage des produits de r‚f‚rence
#define DISPLAY_PREF
// Pour affichage de la temp‚rature des fum‚es
//#define FUME
// Pour lecture de la cl‚ hardware
//#define PROTECTKEY
// Gestion des fichiers historiques
#define FichListNumName "PRODHIS.NUM"       // Fichier liste des hist. bill. classé par lot et N° ordre
#define FichListName    "PRODHIS.LST"       // Fichier liste des hist. bill.
#define HecRacine 	"P000"	  // Racine commune aux fichiers HEC
#define HecExtens	".HEC"	    // Extension des fichiers hist bill temp
//VB 1-26-98

#define Hec_Entete     46
#define Hec_Length	  Hec_Entete/2
#define Hec_SteelName  11
#define Hec_Coulee	  11
//#define Hec_EnteteSize 68
#define HisExtens	".HIS"	    	 // Extension des fichiers hist bill
#define NbFichHist	5000	 // Nombre de fichiers historiques billettes
#define LgrHdrHisBil	72		 // Taille en-tête fich. hist. bill.(23 int + 22 char + 1long)
#define LgrEnrHisBil	26		  // Taille enreg. fich. hist. bill.


#define	PRODHIST 	"PRODHIS.HIS"		    // Historique production
#define LgrHdrHisPd	8     	 		 // En-tête : 2*sizeof(long)
#define LgrEnrHisPd	36     	   // Enreg. : 14*sizeof(int)+2*sizeof(long)
#define	ZONEHIST 	"ZONEHIS.HIS" 			 // Historique zones
#define DEFHIST      "DEFHIS.HIS"
#define LgrHdrHisZn	10	     // En-tête : 2*sizeof(long)+sizeof(int)
#define LgrEnrHisZn     62    // Enreg. : 7*sizeof(int)+4*NBZONE*sizeof(int)
#define TrHisBillDef	"\\HISTBILL.BLK" 	// <= Hist. bill. transférés
#define LgrChLot0	11    // Lgr chaînes Nom_Lot etc... plus \0 terminal
#define NbCodSteel	8      // Nb d'aciers différents : 1..7 + 0 = défaut
#define DefCodSteel	7 		     // Nuance par défaut : code = 7

// Gestion des fichiers d'impression
#define FichImprBillDef0	"DisBil0.txt"
#define FichImprBillDef1	"DisBil1.txt"

//Maximum number of zone that can be defined for the application
#define maxNbZone 9
#define sigma 5.675e-8

#define MaxBatchNb	50		     // Nb max de batch dans le four
#define DefThProd	20.0 	     // Cadence théorique définie par défaut

#ifdef __OWNER_MAIN_H
int NbFichHec; 			// PF12/01/95 Nombre de produits max ds four
#endif

#ifdef __OWNER_GLOBDAT_H
float SPCalculPer,SPCalculTime = 0;
float MeasReadPer,MeasReadTime = 0;
float PHisPer,HisPeriodTime = 0;
float ZHisPer,HisTempPeriodTime = 0;
float EHisPer,HisEnergyPeriodTime = 0;
float HisDelayRecorded = 0;
int firstcha=1;
int RunSpt = 0;
// Modif FD
int HisEnable = 1;
int AbsScaleMode =  1;   // 0:pas  1:metres
int ResidTimeMode = 0;  // 0:min  1:heure/min
char TempDriveName[2] = {'D','\0'}; 		    // Nom du disque virtuel
// Modif FD 08/02/94
int NewTargetZ3 = 0; 	 // =0 : pas de modif d'objectif, sinon Tobj modifié
float initialCalcPer = 60; 			   // Calculation period (s)
int MaxCalcPerDiv = 3; 	   // Number of time the CalcPer can be divided by 2
ProdTypeList *TheTypeList = NULL; 	   // Pointer to product list object
Furnace *TheFurn; 				// Pointer to furnace object
EndzTargetList *TheTargetList = NULL; 	  // Pointer to objective curve list
Batch *BatchTab[50]; 			// Array of batch pointer (max = 50)
int NbBatch = 0; 				  // Number of defined batch
Metal *MetalTab[22]; 	       // Array of metal pointer (max 22 metal code)
Product *Pdis = NULL; 		   // Pointer to the last discharged product
// Delay time data
long DelayDoneTime = 0; 			   // Elapsed delay time (s)
long DelayToDo = 0; 				// Remaining delay time (mn)
int DelayBegDate[6] = {1992,9,17,0,0,0};
int DelayEndDate[6] = {1992,9,17,0,0,0};
int ProductionRate = 30; 		  // Production rate (prdt per hour)
int NbProdBatch = 0; 		     //FALCK Nb of product in charging batch
int ProdPosition = 0; 	    //FALCK Position of new pdt in the current batch
char *NrLotto = "12345/D/2"; 				  //FALCK Batch name
int Molato = 0 ; 			//FALCK Molato = 1 : produits meulés
int ManBatchSelect = 1; 	       //FALCK Manual / Automatic batch load
#else
extern float initialCalcPer;
extern int RunSpt ;
extern int NbProdBatch ;
extern int ProdPosition ;
extern char *NrLotto ;
extern int Molato ;
extern int ManBatchSelect ;
extern float RefProdAbsc[maxNbZone]; // Affichage des pdts de réf. PF29/11/94
extern int HisEnable;
extern int AbsScaleMode;
extern int ResidTimeMode;
extern int NewTargetZ3;
extern char TempDriveName[2];
extern int NbFichHec; 		// PF12/01/95 Nombre de produits max ds four
#ifdef __FURNACE_H
extern Furnace *TheFurn;
extern Product *Pdis;
#endif
//#if defined (_OWNER_PRDTVIEW_H) || defined (_OWNER_VARIOUS_H)
#ifdef _OWNER_V05DATA1_H
extern ProdTypeList *TheTypeList;
#endif
// add 02/04/93 F.Destrebecq
#ifdef __NEWPTYPE_H
extern ProdTypeList *TheTypeList;
#endif
// end add
#ifdef _OWNER_VARIOUS_H
extern ProdTypeList *TheTypeList;
extern EndzTargetList *TheTargetList;
extern int NbBatch;
extern Batch *BatchTab[50]; //max batch number = 50
extern float SPCalculPer,SPCalculTime;
extern float MeasReadPer,MeasReadTime;
extern float PHisPer,HisPeriodTime;
extern float ZHisPer,HisTempPeriodTime;
extern float EHisPer,HisEnergyPeriodTime;
extern float HisDelayRecorded;
extern int firstcha;
extern long DelayDoneTime;
#endif
#ifdef _OWNER2_V05DATA1_H
extern ProdTypeList *TheTypeList;
#endif
#if defined (_OWNER_V06DATA1_H) || defined (_OWNER2_V06DATA1_H)
extern EndzTargetList *TheTargetList;
#endif
#ifdef _OWNER_SETPOINT_H
extern EndzTargetList *TheTargetList;
extern Batch *BatchTab[50]; //max batch number = 50
extern ProdTypeList *TheTypeList;
extern long DelayToDo;
extern int ProductionRate;
float RefProdAbsc[maxNbZone];
#endif
#ifdef __OWNER_IHM_H
extern ProdTypeList *TheTypeList; 	   	// Pointer to product list object
extern Furnace *TheFurn; 						// Pointer to furnace object
extern EndzTargetList *TheTargetList;		// Pointer to objective curve list
extern Batch *BatchTab[MaxBatchNb];			// Array of batch pointer (max = 50)
extern int NbBatch; 				  				// Number of defined batch
extern Metal *MetalTab[22];		// Array of metal pointer (max 22 metal code)
#endif
#if defined (_OWNER2_V07DATA1_H) || defined (_OWNER_V07DATA1_H)
extern ProdTypeList *TheTypeList;
extern EndzTargetList *TheTargetList;
#endif
#endif
//------------------------------------------------------------------------
//end globdat.h
#endif
