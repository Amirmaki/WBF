//-------------------------------------------------------------------------
// ProdDATA.H
// This file contains data and function prototype for project TACT
// display number 11
//
// Owner source file : Prod_INT1.CPP
//
// Creation : C.Martinet 18/12/92 ASCOMETAL APPLICATION
// Modified :            02/02/93 add Prod_Hour
//
//          F Destrebecq 16/06/93 add Prod_CursorAction
//
//-------------------------------------------------------------------------

#ifndef __PRODDATA_H
#define __PRODDATA_H

#ifndef __PRODUCT_H
#include <product.h>
#endif

#ifndef __BATCH_H
#include <batch.h>
#endif

#ifndef __ZONE_H
#include <zone.h>
#endif

#ifndef __FURNACE_H
#include <furnace.h>
#endif

#ifndef __PRHISLST_H
#include <prhislst.h>
#endif

#ifndef __GLOBDAT_H
#include <globdat.h>
#endif
/*
#ifndef __V12DATA1_H
#include <v12data1.h>
#endif
*/

//Macro definitions
//-------------------------------------------------------------------------
#define Top 					263-14   //244
#define Bottom					307-14
#define Left_Rectangle		0          //V.B : coordonnees
#define Right_Rectangle		639
#define Xline              456

//-------------------------------------------------------------------------

#ifdef _OWNER_Prod_DATA1_H

// cursor action
int  Prod_NbPas;
int  Prod_CursorAction = 0 ;

long Prod_GFlength;           //furnace length
long Prod_GNbZone;            //number of zones
long Prod_GPosZ[maxNbZone];   //end of zones abscissa
int  Prod_GPosZp[maxNbZone];  //in pixel


//ordinates for values at cursor
int Prod_GyFirst,Prod_GyTempsSej,Prod_GyTempMoy,Prod_GyTempSurf,Prod_GyTempInf,Prod_GyTempCore;
int Prod_GyTempTV,Prod_GxTempTV;
//abscissa for values at cursor
int Prod_GxActu,Prod_GxReqTemp,Prod_GxRef,Prod_GxTarget,Prod_GxType;

long Prod_GMaxTemp; //virtual range of curve display
int Prod_GxZero,Prod_GyZero;    //origine of curve display (pixel)
int Prod_GxLarge,Prod_GyHaut; //range of curve display (pixel)

float Prod_Gxalpha,Prod_Gxbeta,Prod_Gyalpha,Prod_Gybeta; //curve scaling data
float Prod_Gtalpha,Prod_Gtbeta; //curve scaling for time

long Prod_DeltaRefActu; // Difference between reference and actual
			  // mean temp. at cursor.

int Prod_NbActPoint;  //number of point in the curves
//VB 28-1-98
//--------------------------------------------------------------------------
int  Prod_JobID;
int  Prod_PONumber;
char Prod_Coulee[11];
char Prod_SteelName[11];
//--------------------------------------------------------------------------
//Curve data
long *Prod_TxAct,*Prod_TMoyAct,*Prod_TSurfAct,*Prod_TInfAct;
long *Prod_TSejAct,*Prod_TCoreAct;
long *Prod_TObjAct,*Prod_TTvAct;

//Curve data (pixel)
int *Prod_TxActp,*Prod_TMoyActp,*Prod_TSurfActp,*Prod_TInfActp,*Prod_TCoreActp;
int *Prod_TObjActp,*Prod_TTvActp,*Prod_TSejActp;

//data for cursor display
int Prod_CursorInc; //position increase number
long Prod_CursorPosm[500]; //(mm)
int  Prod_CursorPosp[500]; // (pixel)
long Prod_CursorPos; //ascometal : cursor move on each step position

//data to copy page
//unsigned Prod_tailp; //memory amount
//void *Prod_Gpage; //memory array

int Prod_NewHispDisp; //set to 1 when a new product must be displayed
int Prod_NextOrPrev; //set to -1 when previous product must be displayed
			 //set to 1 when next product must be displayed

int Prod_DispDate[6]; //discharging date of displayed product
char Prod_ActuFile[MAXPATH];//file names of product history
char Prod_PrevFile[MAXPATH];
char Prod_NextFile[MAXPATH];

int Prod_DispDateAcq[6]; //same for acquisition date
char *Prod_DDateAcqText = "00/00/0000 00h00m00s";
int Prod_curAcqVal; //position of current character in date acquisition
int Prod_GxDateDis,Prod_GxDateAcq,Prod_GyDate;
int Prod_Active; //set to 1 in acquisition mode

int Prod_ProdType; //displayed product type
int Prod_ProdWidi; //displayed product thickness or diameter
int Prod_ProdChaTempp; //displayed product charging temp(pixel)


int Prod_GtempGradp[10]; //screen ordinate for temperature axis
int Prod_dtempGrad,Prod_nbtempGrad;
int Prod_GmnGradp[10]; //screen ordinate for time axis
int Prod_dmnGrad,Prod_nbmnGrad;
//char *Prod_Hour = "hhhmn";
int Prod_EndDate=0;

#else

#ifdef _OWNER2_Prod_DATA1_H

extern int Prod_NbPas;
//extern char *Prod_Hour;
extern long Prod_GFlength;           //furnace length
extern long Prod_GNbZone;            //number of zones
extern long Prod_GPosZ[maxNbZone];   //end of zones abscissa
extern int  Prod_GPosZp[maxNbZone];  //in pixel


//ordinates for values at cursor
extern int Prod_GyFirst,Prod_GyTempsSej,Prod_GyTempMoy,Prod_GyTempSurf;
extern int Prod_GyTempInf,Prod_GyTempCore;
extern int Prod_GyTempTV,Prod_GxTempTV;
//abscissa for values at cursor
extern int Prod_GxActu,Prod_GxReqTemp,Prod_GxRef,Prod_GxTarget,Prod_GxType;

extern long Prod_GMaxTemp; //virtual range of curve display
extern int Prod_GxZero,Prod_GyZero;    //origine of curve display (pixel)
extern int Prod_GxLarge,Prod_GyHaut; //range of curve display (pixel)

extern float Prod_Gxalpha,Prod_Gxbeta,Prod_Gyalpha,Prod_Gybeta; //curve scaling data
extern float Prod_Gtalpha,Prod_Gtbeta; //curve scaling for time

extern long Prod_DeltaRefActu; // Difference between reference and actual
			  // mean temp. at cursor.

extern int Prod_NbActPoint;  //number of point in the curves

//VB 28-1-98
//--------------------------------------------------------------------------
extern int  Prod_JobID;
extern int  Prod_PONumber;
extern char Prod_Coulee[11];
extern char Prod_SteelName[11];
//--------------------------------------------------------------------------

//Curve data
extern long *Prod_TxAct,*Prod_TMoyAct,*Prod_TSurfAct,*Prod_TInfAct;
extern long *Prod_TSejAct,*Prod_TCoreAct;
extern long *Prod_TObjAct,*Prod_TTvAct;

//Curve data (pixel)
extern int *Prod_TxActp,*Prod_TMoyActp,*Prod_TSurfActp,*Prod_TInfActp,*Prod_TCoreActp;
extern int *Prod_TObjActp,*Prod_TTvActp,*Prod_TSejActp;

//data for cursor display
extern int Prod_CursorInc; //position increase number
extern long Prod_CursorPosm[500]; //(mm)
extern int  Prod_CursorPosp[500]; // (pixel)
extern long Prod_CursorPos; //ascometal : cursor move on each step position

//data to copy page
//extern unsigned Prod_tailp; //memory amount
//extern void *Prod_Gpage; //memory array


extern int  Prod_DispDate[6]; //discharging date of displayed product
extern char Prod_ActuFile[24];//file names of product history
extern char Prod_PrevFile[24];
extern char Prod_NextFile[24];

extern int Prod_DispDateAcq[6]; //same for acquisition date
extern char *Prod_DDateAcqText;
extern int Prod_curAcqVal; //position of current character in date acquisition
extern int Prod_GxDateDis,Prod_GxDateAcq,Prod_GyDate;
extern int Prod_Active; //set to 1 in acquisition mode

extern int Prod_ProdType; //displayed product type
extern int Prod_ProdWidi; //displayed product thickness or diameter
extern int Prod_ProdChaTempp; //displayed product charging temp(pixel)


extern int Prod_GtempGradp[10]; //screen ordinate for temperature axis
extern int Prod_dtempGrad,Prod_nbtempGrad;
extern int Prod_GmnGradp[10]; //screen ordinate for time axis
extern int Prod_dmnGrad,Prod_nbmnGrad;
extern int Prod_EndDate;
#endif

extern int Prod_NewHispDisp; //set to 1 when a new product must be displayed
extern int Prod_NextOrPrev; //set to -1 when previous product must be displayed
			 //set to 1 when next product must be displayed

extern int Prod_CursorAction ;
#endif

//------------------------------------------------------------------------
//function prototypes

//initialisation
void Prod_Init();

//display
void Prod_Display();

//display background
void Prod_DispBackground();

//erase
void Prod_Erase();

//Update the display with new data
void Prod_UpdateDisp();

//Update the data with the data for the display
void Prod_UpdateData();

//update the display with cursor position, parameter action tells the
//function what to do : key code for interrupt, 0 for simple update
//second argument is action for date acquisition cursor
void Prod_UpdateHPGraphCursor(int action = 0,int tcurdisplac = 0);


//acquisition cursor displaying
//argument displac is -1 for previous, 1 for next, 0 to display at current
//position
void Prod_AcqHPCursor(int displac = 0);

//display the discharging date
void Prod_DispDates();

void Save_Date();

//VB:1-21-98 gestion produit suivant produit precedent
void Previous_Prod();
void Next_Prod();

#endif
//end PRODDATA.H
