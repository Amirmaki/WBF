#ifndef __WINSTRUC_H
#define __WINSTRUC_H

//#ifndef __DIR_H
//#include <dir.h>
//#endif
	
#ifndef __PRHISLST_H
#include "prhislst.h"
#endif

#define MAXPATH 	80
#define NBZONE			5
//#define NBZONEI		5
#define NBCHAR			6
#define NBCOL  		10
#define NBQUAL 		20
#define NBPROFRECORD	100
#define MAXALARM	50
#define PeriodMin	5	     // Période min en simulation accéllérée

struct TypeFour
{ char Length[NBCHAR];
  char Width[NBCHAR];
  char HTop[NBCHAR];
  char Step1[NBCHAR];
  char Step2[NBCHAR];
  char WBeamLim[NBCHAR];
  char ZUBeg[NBCHAR];
  char ZUEnd[NBCHAR];
  char Alarm[MAXALARM];
  char EndAbsSup[NBZONE][NBCHAR];
  char TcplAbsSup[NBZONE][NBCHAR];
  char TcplDepthSup[NBZONE][NBCHAR];
  char QGMaxSup[NBZONE][NBCHAR];
  char QAMaxSup[NBZONE][NBCHAR];
  char SptMinSup[NBZONE][NBCHAR];
  char SptMaxSup[NBZONE][NBCHAR];
  char SptVarSup[NBZONE][NBCHAR];
  char RespTimeSup[NBZONE][NBCHAR];
};

struct TypeModel
{ char HCoef1[NBCHAR];
  char HCoef2[NBCHAR];
  char HCoef3[NBCHAR];
  char Epsif1[NBCHAR];
  char Epsif2[NBCHAR];
  char EpsifMin[NBCHAR];
  char EpsifMax[NBCHAR];
  char KSurf[NBCHAR];
  char KTime[NBCHAR];
  char KEpsiSup[NBZONE][NBCHAR];
  char MeasPer[NBCHAR];
  char CalcPer[NBCHAR];
  char SPCalcPer[NBCHAR];
  char PHisPer[NBCHAR];
  char ZHisPer[NBCHAR];
  char SPEnable;
  char ZHisEnable;
  char PHisEnable;
  char PaceEnable;
  char PLCConnect;
  char CTLConnect;
  char Alarm[MAXALARM];
};

struct TypeLObj
{ char Prod[NBCHAR];
  char PType[NBCHAR];
  char Target[NBZONE][NBCHAR];
  char ChT[NBCHAR];
};

struct TypeCompLObj
{ char Num[NBCHAR];
  char Prod[NBCHAR];
  char PType[NBCHAR];
  char Active[NBCHAR];
  char Target[NBZONE][NBCHAR];
  char ChT[NBCHAR];
};

struct TypeTProd
{ char Num[NBCHAR];
  char Quality[NBQUAL];
  char Width[NBCHAR];
  char Thick[NBCHAR];
  char ReqDisT[NBCHAR];
  char Code[NBCHAR];
};
#ifdef  HIS
typedef struct 			// Pour les noms des répertoires historiques
	{ char PHisDir[MAXPATH];	   // Directory historiques produits
	  char FHisDir[MAXPATH];	       // Directory historiques four
	  char THisDir[MAXPATH];     // Directory temporaire des historiques
	} TDirAppl;
#endif
#ifdef __OWNER_MAIN_H
double OldTime;  				     // Calcul d'élévation thermique
double OldTime1; 							 // Tracking
double OldTime2; 							// Calcul SP
double OldTime3; 							    // ARRET
double OldTime4; 					       // Simulation d'evmts PLC
double OldTime5; 					    // Historisation produit
double OldTime6; 					       // Historisation four
int ARRETPeriod;
//float ThePas;
int NbCal;
int Track;
int SP;
int ARRET;
int PHis;
int ZHis;
// Modif PF pour simulation
int SimulPLC;	  		     // Indique s'il ya une avance à simuler
int SimulMode;	  		  // =1 si mode simulation (cf simulwnd.cpp)
int SimulPLCAuto; 			 // Génération évnmts PLC en continu
int SimulCalcSP;  				       // Calcul SP autorisé
int SimulLoopSP;  				// Bouclage PV / SP autorisé
float SimulPeriod;  	       // Périodicté des avances si SimulPLCAuto = 1
int bouclage=0;
float SimulAcc=1.0;	  			    // Accélération de la simulation
//TDirAppl DirAppl;
// Données extraites de la configuration
TypeFour Four; // Avant : #ifdef __OWNER_FURNWIND_H
TypeModel Model; // Avant : #ifdef __OWNER_MODWIND_H
TypeLObj LObj[NBCOL]; // Avant : #ifdef __OWNER_HTCVWIND_H
TypeCompLObj CompLObj;
TypeTProd TProd[NBCOL]; // Avant : #ifdef __OWNER_TPDTWIND_H
TypeTProd CompTProd;
#else
#ifdef HIS
extern TDirAppl DirAppl;
#endif
extern double OldTime;
extern double OldTime1;
extern double OldTime2;
extern double OldTime3;
extern double OldTime4;
extern double OldTime5;
extern double OldTime6;
extern int ARRETPeriod;
//extern float ThePas;
extern int NbCal;
extern int Track;
extern int SP;
extern int ARRET;
extern int PHis;
extern int ZHis;
extern int SimulPLC;
extern int SimulMode;	  
extern int SimulPLCAuto;  
extern int SimulCalcSP;   
extern int SimulLoopSP;   
extern float SimulPeriod;
extern int bouclage;
extern float SimulAcc;	  
// Données extraites de la configuration
extern TypeFour Four;
extern TypeModel Model;
extern TypeLObj LObj[NBCOL];
extern TypeCompLObj CompLObj;
extern TypeTProd TProd[NBCOL];
extern TypeTProd CompTProd;
#endif

/*#ifdef  __OWNER_FURNWIND_H
TypeFour Four;
#else
extern TypeFour Four;
#endif*/

/*#ifdef __OWNER_MODWIND_H
TypeModel Model;
#else
extern TypeModel Model;
#endif*/

/*#ifdef __OWNER_HTCVWIND_H
TypeLObj LObj[NBCOL];
TypeCompLObj CompLObj;
#else
extern TypeLObj LObj[NBCOL];
extern TypeCompLObj CompLObj;
#endif*/

/*#ifdef__OWNER_HTCVOP_H
char Active[NBCOL];
#else
extern char Active[NBCOL];
#endif*/

/*#ifdef __OWNER_TPDTWIND_H
TypeTProd TProd[NBCOL];
TypeTProd CompTProd;
#else
extern TypeTProd TProd[NBCOL];
extern TypeTProd CompTProd;
#endif*/

#ifdef __OWNER_DELAYWND_H
int Heure;
int Min;
int Sec;
int Annee;
int Mois;
int Jour;
int DureH;
int DureMin;
int DureJour;
#else
extern int Heure;
extern int Min;
extern int Sec;
extern int Annee;
extern int Mois;
extern int Jour;
extern int DureH;
extern int DureMin;
extern int DureJour;
#endif

#ifdef __OWNER_HEATWIND // ARRET
char date[9]=""; //date
char date1[9]=""; //heure
char date2[14]="";//remains
#endif

#endif
