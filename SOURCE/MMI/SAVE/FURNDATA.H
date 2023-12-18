//-------------------------------------------------------------------------
// Furn_DATA1.H
// This file contains data and function prototype for project TACT
// display number 9
//
// Owner source file : Furn_INT1.CPP
//
// Creation : C.Martinet 18/12/92
// Modified :            02/02/93 add Furn_Hour
//
//         F.Destrebecq  16/06/93 add Furn_CursorAction
//
//-------------------------------------------------------------------------

#ifndef __Furn_DATA1_H
#define __Furn_DATA1_H

#ifndef __GLOBDAT_H
#include <globdat.h>
#endif


//-------------------------------------------------------------------------

#define Top 					263-14
#define Bottom					307-14
#define Left_Rectangle		0          //V.B : coordonnees
#define Right_Rectangle		639
#define Xline					456
#define Xline2					Right_Rectangle/2
//-------------------------------------------------------------------------

//Macro definitions
//-------------------------------------------------------------------------
//Names of colums in  screen Furn_Ecrdat

//-------------------------------------------------------------------------
#ifdef _OWNER_Furn_DATA1_H

// cursor action
int Furn_CursorAction = 0 ;

//data to copy page
unsigned Furn_tailp; //memory amount
void *Furn_Gpage; //memory array

//number of zone
int Furn_NbZone;

//ordinates for values at cursor
int Furn_GyPV,Furn_GySP,Furn_GyQG,Furn_GyQA,Furn_GyTrec;
int Furn_GyPtemp,Furn_GyPmis;
//abscissa for values at cursor
int Furn_GxPV1,Furn_DGxPV1,Furn_GxTrec;
int Furn_GxTdefCal,Furn_GxTmes,Furn_GxTdecal;
int Furn_GxTsej,Furn_GxTobj,Furn_GxOxy,Furn_GxDecarb;

int Furn_GxZero,Furn_GyZero;    //origine of curve display (pixel)
int Furn_GxLarge,Furn_GyHaut; //range of curve display (pixel)

int Furn_GyFirst,Furn_GySecond;

//curve scaling data
double Furn_Gxalpha,Furn_Gxbeta; //abscissa
float Furn_Gyalpha,Furn_Gybeta;  //temperature
float Furn_Gtalpha,Furn_Gtbeta; //residence time
float Furn_Godalpha,Furn_Godbeta; //oxy,decab
float Furn_Gqalpha,Furn_Gqbeta; //flow rate %

int Furn_PaletteCi[6]; //color palette for each curve
int Furn_ColorCi[6]; //true color for each used palette

//used curve data
int *Furn_xAct[6];
int *Furn_xActp,*Furn_yActp[6];
int *Furn_Zval[4*maxNbZone+1]; //zone curve data
long *Furn_Pval[7]; //discharged product data

//data for cursor display
int Furn_ActCursorPos; //position (0->Furn_NbActPoint-1)
int Furn_CursorInc; //number of shift positions for fast cursor

int Furn_NewHistDisp; //set to 1 when a new curve must be displayed

int Furn_DispBegDate[6]; //beginning date of displayed curves
int Furn_DispEndDate[6]; //ending date of displayed curves

char *Furn_DBegDateAcqText = "00/00/0000 00h00mn00s";
char *Furn_DEndDateAcqText = "00/00/0000 00h00mn00s";

int Furn_Active;
int Furn_curAcqVal; //position of current character in date acquisition
int Furn_GxDateBeg,Furn_GxDateEnd,Furn_GyDate;
int Furn_choosingField;  //field number which is to read
		       //when choosing date 1->beg date, 2->end date


int Furn_GtempGradp[10]; //screen ordinate for temperature axis
int Furn_dtempGrad,Furn_nbtempGrad;
int Furn_GmnGradp[10]; //screen ordinate for time axis
int Furn_dmnGrad,Furn_nbmnGrad;
int Furn_GmmGradp[10]; //screen ordinate for oxydec axis
int Furn_dmmGrad,Furn_nbmmGrad;
int Furn_GpcGradp[10]; //screen ordinate for % axis
int Furn_dpcGrad,Furn_nbpcGrad;

int Furn_NbActPoint; //number of points in each curve

char *Furn_Hour = "hhhmn";
int Furn_EndDate=0;
int TextPos;
#else

#ifdef _OWNER2_Furn_DATA1_H
extern char *Furn_Hour;
extern unsigned Furn_tailp;
extern void *Furn_Gpage;
extern int Furn_NbZone;

//ordinates for values at cursor
extern int Furn_GyPV,Furn_GySP,Furn_GyQG,Furn_GyQA,Furn_GyTrec;
extern int Furn_GyPtemp,Furn_GyPmis;
//abscissa for values at cursor
extern int Furn_GxPV1,Furn_DGxPV1,Furn_GxTrec;
extern int Furn_GxTdefCal,Furn_GxTmes,Furn_GxTdecal;
extern int Furn_GxTsej,Furn_GxTobj,Furn_GxOxy,Furn_GxDecarb;

extern int Furn_GxZero,Furn_GyZero;    //origine of curve display (pixel)
extern int Furn_GxLarge,Furn_GyHaut; //range of curve display (pixel)
extern int Furn_GyFirst,Furn_GySecond;

//curve scaling data
extern double Furn_Gxalpha,Furn_Gxbeta; //abscissa
extern float Furn_Gyalpha,Furn_Gybeta;  //temperature
extern float Furn_Gtalpha,Furn_Gtbeta; //residence time
extern float Furn_Godalpha,Furn_Godbeta; //oxy,decab
extern float Furn_Gqalpha,Furn_Gqbeta; //flow rate %

extern int Furn_PaletteCi[6]; //color palette for each curve
extern int Furn_ColorCi[6]; //true color for each used palette

//used curve data
extern int *Furn_xAct[6];
extern int *Furn_xActp,*Furn_yActp[6];
extern int *Furn_Zval[4*maxNbZone+1]; //zone curve data
extern long *Furn_Pval[7]; //discharged product data

//data for cursor display
extern int Furn_ActCursorPos; //position (0->Furn_NbActPoint-1)
extern int Furn_CursorInc; //number of shift positions for fast cursor

extern int Furn_DispBegDate[6]; //beginning date of displayed curves
extern int Furn_DispEndDate[6]; //ending date of displayed curves

extern char *Furn_DBegDateAcqText;
extern char *Furn_DEndDateAcqText;

extern int Furn_Active;
extern int Furn_curAcqVal; //position of current character in date acquisition
extern int Furn_GxDateBeg,Furn_GxDateEnd,Furn_GyDate;
extern int Furn_choosingField; //field number which is to read
		       //when choosing date 1->beg date, 2->end date


extern int Furn_GtempGradp[10]; //screen ordinate for temperature axis
extern int Furn_dtempGrad,Furn_nbtempGrad;
extern int Furn_GmnGradp[10]; //screen ordinate for time axis
extern int Furn_dmnGrad,Furn_nbmnGrad;
extern int Furn_GmmGradp[10]; //screen ordinate for oxydec axis
extern int Furn_dmmGrad,Furn_nbmmGrad;
extern int Furn_GpcGradp[10]; //screen ordinate for % axis
extern int Furn_dpcGrad,Furn_nbpcGrad;

extern int Furn_NbActPoint; //number of points in each curve
extern int Furn_EndDate;
#endif

extern int Furn_NewHistDisp; //set to 1 when a new curve must be displayed

extern int Furn_CursorAction ;
extern int TextPos;

#endif

//------------------------------------------------------------------------
//function prototypes

//initialisation
void Furn_Init();

//display
void Furn_Display();

//display background
void Furn_DispBackground();

//erase
void Furn_Erase();

//Update the display with new data
void Furn_UpdateDisp();

//Update the data with the data for the display
void Furn_UpdateData();

//scales the data according to scaling information
void Furn_ScaleGraphData();

//scales the abscissa according to beg and end date of data
void Furn_ScaleGraphDates();

//update the display with cursor position, parameter action tells the
//function what to do : key code for interrupt, 0 for simple update
//second argument is action for date acquisition cursor
void Furn_UpdateGraphCursor(int action = 0,int tcurdisplac = 0);

void Save_Dates();

//end of acquisition treatment (enter key code received)
//void Furn_AcqEnd();

//character treatment in acquisition mode
//argument is the key code
//void Furn_AcqChar(int);

//acquisition cursor displaying
//argument displac is -1 for previous, 1 for next, 0 to display at current
//position
//void Furn_AcqCursor(int displac = 0);

//display the dates
//void Furn_DispDates();

//key treatment, argument is the key code
//void Furn_TestKey(int);

//hour display, argument is a string like 12h35
//void Furn_DispHour(char *);

#endif
//end Furn_DATA1.H
