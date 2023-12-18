//--------------------------------------------------------------------------
//
// PRHISLST.H : ProdHisList class
//
// This class defines all data and functions for product history
// management
//
// Creation : C.Martinet 13/08/92
// Modif PF12/01/95 : Ce n'est plus une classe !
//--------------------------------------------------------------------------

#ifndef __PRHISLST_H
#define __PRHISLST_H

#ifndef __FSTREAM_H
#include <fstream.h>
#endif
#ifndef __UCLSTYPE_H
#include "uclstype.h"
#endif
#ifndef __SORTARRY_H
#include <sortarry.h>
#endif
//#define HIS
#ifndef __DATABASE_H
#include "database.h"
#endif
#ifndef __ENDZTLST_H
#include "endztlst.h"
#endif
#ifndef __FURNACE_H
#include "furnace.h"
#endif
//#ifndef __PRHISELM_H
//#include <prhiselm.h>
//#endif

void updateHistory(Furnace*,EndzTargetList*,int event = 2,
		   Product *Pdis = NULL);
// Update the whole history. Arguments : the furnace pointer, the target
// curve list pointer, the event number (1:charging, 2:periodical,
// 3:delay start, 4:delay end, 5:discharging) and a pointer to the
// discharged product when event = 5
void hisChargingEvent(Product*);
// Add a new product in history list. Not to be called by the user
void hisDischargingEvent(EndzTargetList*, Product*);
// Last record for discharged products. Not to be called by the user
int getNewestHisFile(int *number, int *newest);
// Retourne le nombre d'historiques et le numéro du plus récent
int getProdHisFile(int *,char *,char *prevFile = NULL,char *nextFile = NULL);
// Give the product history filename of the product identified by its
// discharging date (char*, use 22 characters). The int* is a pointer to
// an integer array[6] containing this date. Function returns 1 if the
// file exists else returns 0, if requested previous and next file names
// are also provided
//******************************************************
//recherche des fichiers historiques produits par N°lot et N°Ordre
int getProdHisFileWithNum(char *coulee,long NOrdre, char *theFile,char *prevFile,char *nextFile);

long getFileNumberInHis(); 	// Returns number of available history files
// FD Creation
//void DeleteProd(Product &);
//void updateFile(int numfich, int event, char *buffer, char *buf1, int *buf2);
void updateFile(int numfich, int event, char *buffer);
//void updateFile(EndzTarget& Ptarg,int event,char *buffer); //cf PRHISELM.CPP
void FixeNOrdre(int numfich,Product *disP);//fixe le N° ordre
#endif

