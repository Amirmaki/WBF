//-------------------------------------------------------------------------
// TEMPHIS.H
// This file contains data and function prototype for temperature
// history in project TACT
//
// Owner source file : TEMPHIS.CPP
//
// Creation : C.Martinet 25/08/92
// Modified :            16/02/93 add decalt,exitf in prod temp his
//				  and energy counter history
//-------------------------------------------------------------------------

#ifndef __TEMPHIS_H
#define __TEMPHIS_H

//Include files
#ifndef __PRODUCT_H
#include "product.h"
#endif

#ifndef __BATCH_H
#include "batch.h"
#endif

#ifndef __ZONE_H
#include "zone.h"
#endif

#ifndef __FURNACE_H
#include "furnace.h"
#endif

#ifndef __ENDZTLST_H
#include "endztlst.h"
#endif

#ifndef __GLOBDAT_H
#include "globdat.h"
#endif

//-------------------------------------------------------------------------
//Macro definitions
//-------------------------------------------------------------------------
#define MaxNbZTHrecord 10000
#define MaxNbPTHrecord 30000
#define MaxNbEnergyHrecord 50000

//------------------------------------------------------------------------
//Data declarations
//------------------------------------------------------------------------
#ifdef _OWNER_TEMPHIS_H
//declaration for TEMPHIS.CPP source file
char *ZHistFile = ".\\HISHEC\\ZONETEMP.HTZ";
char *PHistFile = ".\\HISHEC\\DISCTEMP.HTP";
char *EHistFile = ".\\HISHEC\\ENERGY.HTC";

#else
//declaration for other source file
extern char *ZHistFile;
extern char *PHistFile;
extern char *EHistFile;
#endif


//------------------------------------------------------------------------
//function prototypes

//update history with furnace temperatures and setpoints
void zoneTempHis(Furnace *);

//update history with discharged product values
//exitf & dect are last measurment at furnace exit and decal
void prodTempHis(Product *,EndzTargetList *, float exitf=0, float dect=0);


//update history with energy counter, argument is amount of gas Nm3
//file saved value are 10-2Nm3 unit
void gasEnergyHis(float);


//give number of starting and ending record corresponding to given
//starting and ending date accessible in ZONETEMP.HTZ, returns
//the number of records, including both ends
//arguments are pointers to beginning date, ending date,
//beginning record, ending record
long zoneTempRecNum(int *,int *,long *,long *);

//give number of starting and ending record corresponding to given
//starting and ending date accessible in DISCTEMP.HTP, returns
//the number of records, including both ends
//arguments are pointers to beginning date, ending date,
//beginning record, ending record
long prodTempRecNum(int *,int *,long *,long *);

//give number of starting and ending record corresponding to given
//starting and ending date accessible in ENERGY.HTC,
//returns the number of records, including both ends
//arguments are pointers to beginning date, ending date,
//beginning record, ending record
long gasEnergyRecNum(int *, int *, long *, long *);

//cette fonction remplit les tablaux relatifs à l'historique zone
// en fonction des dates de début et fin
//renvoie le numéro d'enregistrement 
int FillArrayZone(int TheBegDate[6], int TheEndDate[6]);

int FillArrayProd(int TheBegDate[6], int TheEndDate[6]);
//end temphis.h
#endif
