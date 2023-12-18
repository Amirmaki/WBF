//-------------------------------------------------------------------------
// REH_DATA.H
// This file contains data and function prototype for project TACT
// display number 7
//
// Owner source file : Reh_INT1.CPP
//
// Creation : C.Martinet 18/12/92
// Modif		 11/01/93 add data and function for password
//			 13/01/93 add Reh_TextBegDelay & Reh_DelayType
//			 15/01/93 add Reh_NewPdis and Reh_DischGap
//			 27/01/93 each setpoint blinks independantly
//			 02/02/93 add Reh_Hour
//			 18/02/93 blinking measurment
//
//       F Destrebecq    16/06/93 Add Data Reh_CursorAction
//
//       JL C		 01/02/94 Add RefreshMeas
//-------------------------------------------------------------------------

#ifndef __REH_DATA_H
#define __REH_DATA_H

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

#ifndef __PRODTLST_H
#include <prodtlst.h>
#endif

#ifndef __ENDZTLST_H
#include <endztlst.h>
#endif

#ifndef __GLOBDAT_H
#include <globdat.h>
#endif

#ifndef __TIME_H
#include <time.h>
#endif

/*#ifndef __V06DATA1_H
#include <v06data1.h>
#endif
*/
//Macro definitions
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------

#define Top 					263
#define Bottom					307
#define Left_Rectangle1		0          //V.B : coordonnees
#define Right_Rectangle1	456
#define Left_Rectangle2		456
#define Right_Rectangle2	639
//-------------------------------------------------------------------------

#ifdef _OWNER_Reh_DATA1_H

int Reh_CursorAction = 0 ;  // 0  :  No Action
				// 1  :  Right Arrow
				// 2  :  Left  Arrow

int Reh_NewPdis; //set to 1 when a product has been discharged since
		//last display
int Reh_DischGap; //0 when there were no gap between last and next
		 //discharged product
int Reh_BlinkingSP[maxNbZone];  //set to 1 when SP must blink
int Reh_TrueSPColor[maxNbZone]; //62 or 0
int Reh_SPColor[maxNbZone];
int Reh_BlinkingPV; //set to 1 when PV must blink
int Reh_TruePVColor; //60 or 0
int Reh_Active; //set to 1 during password acquisition
int Reh_CurAcqVal; //position of current character in password acquisition
int Reh_GxPass,Reh_GyPass; //pixel position of password
int Reh_Redraw; //set to 1 when main function must update the display
int Reh_DelayType; //0 -> nothing to display
		  //1 -> immediat delay running
		  //2 -> programmed delay running
		  //3 -> forseen delay to come

long GFlength;           //furnace length
long GNbZone;            //number of zones
long GPosZ[maxNbZone];   //end of zones abscissa
int  GPosZp[maxNbZone];  //in pixel
long GPosT[maxNbZone];   //thermocouple abscissa
int  GxTcpl[maxNbZone+1],GyTcpl[maxNbZone+1]; //Tcpl location on display
long GZtemp[maxNbZone+1];  //temperature in zones + recup
long GZtempScreen[maxNbZone+1]; //temperature in zones + recup for screen
long GZspt[maxNbZone];  //setpoint in zones
long GMinSP[maxNbZone];	//min setpoint in zones
long GMaxSP[maxNbZone];	//max setpoint in zones

int  Reh_EndZTarget[maxNbZone] ; // objectifs fin de zone pour cadencement

//ordinates for values at cursor
int GyTempsSej,GyTempMoy,GyTempSurf,GyTempInf,GyTempCore,GyTempVoute;
//abscissa for values at cursor
int GxActu,GxRef,GxType,GxQual,GxTarget,GxDeviation,GxResTime;

long GMaxTemp; //virtual range of curve display
int GxZero, GyZero;    //origine of curve display (pixel)
int GxLarge, GyHaut; //range of curve display (pixel)
int GxRemains;			//ordonn‚e de "Remains"
int GxReqTemp;			//ordonn‚e de "Req. Temp"
int TMoyEcart[600]; //use to display line to reference point (FloodDelta)
int EndZEcart[maxNbZone]; //mean product temp. at each end of zone

int EndBatchMarker[51]; //pixel abscissa of last product in a batch
ProdType *EndBatchType[51]; //product type object of this product
int EndBatchProd[51]; //production rate for the batch
//FDFALCK
Batch *Reh_TheBatch[51];

float Gxalpha,Gxbeta,Gyalpha,Gybeta; //curve scaling data

int RefCurve; // 1 if a reference curve exists
long EndZoneRef[maxNbZone]; //ordinate of reference in each zone
int EndZoneRefp[maxNbZone]; //pixel
long NextZoneRef[maxNbZone]; //actual temperatures at zone end
int NextZoneRefp[maxNbZone]; //pixel
int EnableCurveRecording;

long DeltaRefActu; // Difference between reference and actual
			  // mean temp. at cursor.

int NbActPoint;  //number of point in the curves
char *Reh_TextBegDelay;

//Curves data

//Curve data
long *TxAct = NULL,*TMoyAct,*TSurfAct,*TInfAct,*TSejAct,*TCoreAct,*TprodRate;
int *TVouteAct;

//Curve data (pixel)
int *TxActp,*TMoyActp,*TSurfActp,*TInfActp,*TCoreActp;

//roof temp curve (100 points along furnace)(pixel)
int TxRoofp[100]; //abscissa
int TRoofp[100]; //ordinate

//fumetemp provisoire pour test
int TFumep[100]; //ordinate

int NbPas;	// nbre de pas dans le four

//data for cursor display
int CursorInc; //position increase number
long *CursorPosm; //(mm)
int  *CursorPosp; // (pixel)
long CursorPos; //ascometal : cursor move on each step position

//data to copy page
unsigned tailp; //memory amount
void *Gpage; //memory array

int Reh_RecordRefReq; //set to 1 when operator wishes to record the raised
			  //temperature as targets

time_t oldt; //for refresh measurment

int Reh_Duree_seuil[2] ; //duree ecoul‚e depuis le depassement du seuil
			//de temp‚rature pour le produit situ‚ pr‚s du
			//d‚fournement
			//-1 : Le produit n'est pas prioritaire.

#else

extern int NbPas;

extern int Reh_Duree_seuil[2] ;

extern int Reh_CursorAction ;

extern char *Reh_Hour;

extern int Reh_NewPdis; //set to 1 when a product has been discharged since
		//last display
extern int Reh_DischGap; //0 when there were no gap between last and next
		 //discharged product
extern char *Reh_TextBegDelay;
extern int Reh_DelayType;

extern int Reh_Active; //set to 1 during password acquisition
//extern char *Reh_PassWord;
extern int Reh_CurAcqVal; //position of current character in password acquisition
extern int Reh_GxPass,Reh_GyPass; //pixel position of password
extern int Reh_Redraw; //set to 1 when main function must update the display
extern int Reh_BlinkingSP[maxNbZone];  //set to 1 when SP must blink
extern int Reh_TrueSPColor[maxNbZone]; //62 or 0
extern int Reh_SPColor[maxNbZone];
extern int Reh_BlinkingPV; //set to 1 when PV must blink
extern int Reh_TruePVColor; //60 or 0
extern long GFlength;           //furnace length
extern long GNbZone;            //number of zones
extern long GPosZ[maxNbZone];   //end of zones abscissa
extern int  GPosZp[maxNbZone];  //in pixel
extern long GPosT[maxNbZone];   //thermocouple abscissa
extern int GxTcpl[maxNbZone+1];
extern int GyTcpl[maxNbZone+1]; //Tcpl location on display
extern long GZtemp[maxNbZone+1];  //temperature in zones + recup
extern long GZtempScreen[maxNbZone+1];
extern long GZspt[maxNbZone];  //setpoint in zones
extern long GMinSP[maxNbZone];	//min setpoint in zones
extern long GMaxSP[maxNbZone];	//max setpoint in zones


extern int  Reh_EndZTarget[maxNbZone] ;

//ordinates for values at cursor
extern int GyTempsSej;
extern int GyTempMoy;
extern int GyTempSurf;
extern int GyTempInf;
extern int GyTempCore;
extern int GyTempVoute;

//abscissa for values at cursor
extern int GxActu;
extern int GxRef;
extern int GxType,GxQual;
extern int GxTarget;
extern int GxDeviation;
extern int GxResTime;
extern int GxReqTemp;
extern long GMaxTemp;  //virtual range of curve display
extern int  GxZero;
extern int  GyZero;    //origine of curve display (pixel)
extern int GxRemains;  //ordonn‚e de "Remains"
extern int GxLarge;
extern int GyHaut;     //range of curve display (pixel)

extern int TMoyEcart[600]; //use to display line to reference point (FloodDelta)
extern int EndZEcart[maxNbZone]; //mean product temp. at each end of zone

extern int EndBatchMarker[51]; //pixel abscissa of last product in a batch
extern ProdType *EndBatchType[51]; //product type object of this product
extern int EndBatchProd[51]; //production rate for the batch
//FDFALCK
extern Batch *Reh_TheBatch[51];

extern float Gxalpha;
extern float Gxbeta;
extern float Gyalpha;
extern float Gybeta; //curve scaling data

extern int RefCurve; // 1 if a reference curve exists
extern long EndZoneRef[maxNbZone]; //ordinate of reference in each zone
extern int EndZoneRefp[maxNbZone]; //pixel
extern long NextZoneRef[maxNbZone]; //actual temperatures at zone end
extern int NextZoneRefp[maxNbZone]; //pixel
extern int EnableCurveRecording;

extern long DeltaRefActu; // Difference between reference and actual
			  // mean temp. at cursor.

extern int NbActPoint;  //number of point in the curves

//Curves data

//Curve data
extern long *TxAct;
extern long *TMoyAct;
extern long *TSurfAct;
extern long *TInfAct;
extern long *TSejAct;
extern long *TCoreAct;
extern long *TprodRate;
extern int  *TVouteAct;

//Curve data (pixel)
extern int *TxActp;
extern int *TMoyActp;
extern int *TSurfActp;
extern int *TInfActp;
extern int *TCoreActp;

//roof temp curve (100 points along furnace)(pixel)
extern int TxRoofp[100]; //abscissa
extern int TRoofp[100]; //ordinate
//fumetemp provisoire pour test
extern int TFumep[100]; //ordinate

//data for cursor display
extern int CursorInc; //position increase number
extern long *CursorPosm; //(mm)
extern int *CursorPosp; // (pixel)
extern long CursorPos; //ascometal : cursor move on each step position

//data to copy page
extern unsigned tailp; //memory amount
extern void *Gpage; //memory array
extern int Reh_RecordRefReq;
extern time_t oldt; //for refresh measurment
#endif

//------------------------------------------------------------------------
//function prototypes

//initialisation
void Reh_Init();

//refresh measurment
void refreshMeas();

//display
void Reh_Display();

//erase
void Reh_Erase();

//Update the display with new data
void Reh_UpdateDisp();

//Update the data with the data for the display
void Reh_UpdateData();

//display the background
void Reh_DispBackground();

//read a reference curve in the list, the argument is the file name string
void Reh_ReadRefCurve();

//record a new reference curve with actual curve
//void Reh_RecordRefCurve();

//update setpoints and measurments
void Reh_UpdateGraphMeas();

//update the display with cursor position, parameter action tells the
//function what to do : key code for interrupt, 0 for simple update
void Reh_UpdateGraphCursor();  //int action = 0

//update display with legend or delay window
void Reh_DispLegendOrDelay();

//update display with password acquisition window
//void Reh_DispPassWordWindow();

//charcter treatment in password acquisition, passed argument is the key code
//void Reh_PassChar(int);

//setpoint blinking
//void Reh_BlinkSP();

//affiche les valeurs selon la position du curseur
void Update_Data(int action);
#endif

//end REH_DATA.H
