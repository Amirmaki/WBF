//-------------------------------------------------------------------------
// SETPOINT.H
// This file contains setpoint calc. function prototype for project TACT
//
// Owner source file : SETPOINT.CPP
//
// Creation : C.Martinet 07/07/92
// Modif    :            20/07/92 add data for delay checking
//			 21/07/92 add MeasIncSp
//			 10/08/92 add LastCalc,LastAdv
//			 08/01/93 add too hight setpoint alarm
//			 01/02/93 add timeBeforeDelay;
//			 03/02/93 add PunderTcplTopSt;
//          F.Destrebecq 02/06/93 add SptSlope and TimeBeforeEofZ
//                                    initialDelay and SPCalcPer (sec)
//-------------------------------------------------------------------------

#ifndef __SETPOINT_H
#define __SETPOINT_H

#include "database.h"

//Macro definitions
#define maxNbCalcInDelay 20
//-------------------------------------------------------------------------
#ifdef _OWNER_SETPOINT_H
extern float CalcPer;
extern float SPCalculPer ;
Furnace *SptFour = NULL;
Product *Pref[NbZonesMax];
int PrefIndex[NbZonesMax];
float PrefDist[NbZonesMax];
int ZoneOrder[NbZonesMax];
float PrefObj[NbZonesMax];
float timeBeforeDelay = 0;
float delayTimeToDo = 0;
float DelayCalcPer = 0;
float MeasIncSp = 10;
float LastCalc = 0;
float LastAdv = 0;
double TimeBeforeEofZ = 0;
float  initialDelay ;
float  SptSlope[NbZonesMax] ;
int HightSetpointAlarm[NbZonesMax] = {0,0,0,0,0,0,0,0,0};
float PunderTcplTopST[NbZonesMax]; //product surface temp at tcpl position

#else
//extern int HightSetpointAlarm[NbZonesMax];
#endif

//------------------------------------------------------------------------
//function prototypes
void makeSptFour(Furnace *);

void searchProdRef(int);

void simulof(int);
//simulof argument is the zone number

void calcSetPoint(Furnace *,float,float);
//arguments are pointer to the furnace to drive, time elapsed since
//last thermal calculation and time elapsed since last beam advance

//return number of second before next forseen delay
float calcTimeBeforeDelay();

//search products under tcpl, return last non empty zone
int searchPunderTcpl();

//renvoit le delta T sur l'objectif fin de zone 3
int decreaseTargetZ3( Product& );

//end setpoint.h
#endif
