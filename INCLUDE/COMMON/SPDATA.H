#ifndef __SPDATA_H
#define __SPDATA_H
//#include <idwindow.h>
#include "winstruc.h"
#ifdef __OWNER_SPDATA_H

int SptVarSup[NBZONE]; //
int SptMaxSup[NBZONE];  //
int SptMinSup[NBZONE];  //
int SPCalcPer;//
int ZoneSetPoint[NBZONE];
float initialCalcPer; //
#else
extern int SptVarSup[NBZONE]; //
extern int SptMaxSup[NBZONE];  //
extern int SptMinSup[NBZONE];  //
extern int SPCalcPer;//
extern int ZoneSetPoint[NBZONE];
extern float initialCalcPer; //
#endif
#endif
