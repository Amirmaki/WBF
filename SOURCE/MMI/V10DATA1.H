//-------------------------------------------------------------------------
// V10DATA1.H
// This file contains data and function prototype for project TACT
// display number 10
//
// Owner source file : V10INT1.CPP
//
// Creation : C.Martinet 31/12/92
//-------------------------------------------------------------------------

#ifndef __V10DATA1_H
#define __V10DATA1_H
/*
#ifndef __SCRNFIEL_H
#include <scrnfiel.h>
#endif
	  */
#ifndef __GLOBDAT_H
#include "globdat.h"
#endif

//Macro definitions
//-------------------------------------------------------------------------
//Names of colums in  screen V10Ecrdat
#define  V10EcrdatMenu   1
#define  V10EcrdatTitre  2
#define  V10EcrdatChoice 3
#define  V10EcrdatScale  4
#define  V10EcrdatNbCol  4

//-------------------------------------------------------------------------
#ifdef _OWNER_V10DATA1_H
int V10NbZone;

int V10DispZorP; //display temp and flow rate history (1) or discharged
		 //product history (0)

//zone temp & flow rate date
int V10DispZCurveNb[4*maxNbZone+1];//value is curve number(0-5) when
	//displayed, -1 if is not displayed,
	//index 0 to maxNbZone-1 is for zone temperature
	//index maxNbZone is for recuperator temperature
	//index maxNbZone+1 to 2*maxNbZone is for zone setpoint
	//index 2*maxNbZone+1 to 3*maxNbZone is for zone gas flow rate
	//index 3*maxNbZone+1 to 4*maxNbZone is for zone air flow rate



//discharged product data
int V10DispPCurveNb[7];//value is curve number(0-5) when
	//displayed, -1 if is not displayed,
	//index 0 is for residence time
	//index 1 is for target temp
	//index 2 is for calculated discharging temp
	//index 3 is for measured discharging temp
	//index 4 is for mill measured temp
	//index 5 is for oxydation calculated value
	//index 6 is for decarburation calculated value

//scaling array
long V10Scaler[8]; //index 0,1 -> min øC, max øC
		  //index 2,3 -> min mn, max mn
		  //index 4,5 -> min %, max %
		  //index 6,7 -> min æm, max æm

int V10ZCurvReplac[6]; //chrono order of displayed curve (0-5) T&F
int V10PCurvReplac[6]; //chrono order of displayed curve (0-5) Pdt
int V10DispZCi[6];      //1 if curve Ci must be displayed
int V10DispPCi[6];      //1 if curve Ci must be displayed

/*
//Pointer to the pointer to the active field
TextField **V10Lactif = &TextField::TheActiveField;
//Declare the ecrdat object
ScreenField V10Ecrdat(V10EcrdatNbCol);   */
//v10 active
int V10Active;

#else
extern int V10NbZone;
extern int V10DispZorP;
extern int V10DispZCurveNb[4*maxNbZone+1];
extern int V10DispPCurveNb[7];
extern long V10Scaler[8];
extern int V10ZCurvReplac[6];
extern int V10PCurvReplac[6];
extern int V10DispZCi[6];
extern int V10DispPCi[6];

//extern ScreenField V10Ecrdat;
extern int V10Active;

#endif

//------------------------------------------------------------------------
//function prototypes

//initialisation
void V10Init();

//display
void V10Display();

//erase
void V10Erase();

//Update the display with new data
void V10UpdateDisp();

//Update the data with the data in file or in the display (afile = NULL)
void V10UpdateData( char *afile = NULL);

//Update the data with a new choice
void V10UpdateChoice();

//key treatment, argument is the key code
void V10TestKey(int);

//display screen help, from_int is 1 when called from interrupt routine
void V10DispHelp(int from_int = 0);

//hour display, argument is a string like 12h35
void V10DispHour(char *);

#endif
//end V10DATA1.H
