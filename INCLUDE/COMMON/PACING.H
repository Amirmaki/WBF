//STD/0001
/*
PACING.H

Owner source file : PACING.CPP

Creation : F Destrebecq on 28/07/93

*/

#ifndef __PACING_H

#define __PACING_H
//-----------------------------------------------------------------------
// data declaration

#ifdef __OWNER_PACING_H
float RemResTime[150] ;
float RemResTimeZU[150];
int   Cadence[150];
extern Furnace *TheFurn ;
extern EndzTargetList *TheTargetList ;
#else
extern float RemResTime[150] ;
extern float RemResTimeZU[150];
extern float Cadence[150];
#endif

//------------------------------------------------------------------------
// Function prototype
int CalcResTime (int,int) ;
float DelayEndZ (float,float) ;
float CalcEqProdRate ( Product  , int ) ;
#endif
//end pacing.h
