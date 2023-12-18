//-------------------------------------------------------------------------
// GLOBFOUR.H
// This file contains all global macro for project class Furnace,Zone,
// Batch,Product
//
// Owner source file : none
//
// Creation : C.Martinet 14/02/92
//-------------------------------------------------------------------------

#ifndef __GLOBFOUR_H
#define __GLOBFOUR_H

//Macro definitions
//-------------------------------------------------------------------------
//Put next line in commentary line if no debbuging are required
//#define DEBBUGING

//Maximum number of zone that can be defined for the application
#define maxNbZone 9
#define sigma 5.675e-8
//#define initialCalcPer 60

//------------------------------------------------------------------------
extern float CalcPer;
extern int MaxCalcPerDiv;
//------------------------------------------------------------------------
//end globfour.h
#endif
