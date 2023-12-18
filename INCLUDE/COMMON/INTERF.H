//-------------------------------------------------------------------------
// INTERF.H
// This file contains declarations for project TACT interface with
// operator (keyboard)
//
// Owner source file : INTERF.CPP
//
// Creation : C.Martinet 19/12/92 ASCOMETAL APPLICATION
//-------------------------------------------------------------------------

#ifndef __INTERF_H
#define __INTERF_H

//include files
//-------------------------------------------------------------------------
#ifndef __GLOBDAT_H
#include <globdat.h>
#endif

#ifndef __GLOBVIEW_H
#include <globview.h>
#endif

//Macro definitions
//-------------------------------------------------------------------------
//Data declarations
//------------------------------------------------------------------------
#ifdef _OWNER_INTERF_H
//declaration for INTERF.CPP source file

//Pointer to normal keyboard interrupt function
//void interrupt (far *oldKbd)(...);

//HotKey indicator (set to 1 when last character was null character
int HotKey = 0;

//State of furnace : delay (1) or not (0)
//int DelayState = 0;

//ecr data acces enable (1), disable (0)
//this data is used to prevent main program and interrupt programs from
//accessing ecr object together

int ecrAccess = 1;

// nombre maximum de touches trait‚e dans l'appel de ecrEnableAccess()
const maxNbKey = 5 ;

#else
//declaration for other source file

//extern int DelayState;
extern int ecrAccess;

#endif

//--------------------------------------------------------------------------
//Function prototypes

void initKbd();
void restorKbd();

void testkey(int);
//passed argument is the integer code of the character

void ecrEnableAccess();
//Enable the access to ecr data, must be called each time the
//ecr data has been used by a non interrupt program

//--------------------------------------------------------------------------
//end interf.h
#endif
