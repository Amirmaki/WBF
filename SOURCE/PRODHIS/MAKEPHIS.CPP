//**************************************************************************
//
//              PROGRAMME MAKEPHIS.CPP : Historiques produits
//              ---------------------------------------------
//
//                         TACT MULTI-TACHES
//
// Creation : P.FAVALE le 03/01/95
// Modif    : PF9/03/95 Passage aux codes aciers
//**************************************************************************
#define HIS
#define __OWNER_MAKEPHIS_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// definition des classes TACT
#include "prodtlst.h"
#include "prhislst.h"
//#include "phislst1.h"
#include "database.h"
#include "endztlst.h"
#include "furnace.h"
#include "globdat.h"
//#define __OWNER_SPDATA_H
#include "spdata.h"
#include "setpoint.h"
#include "winstruc.h"
#define __OWNER_FUNC_H
#include "mtdbfunc.h"
#include "wrstatus.h"
#include "debug.h"

extern Furnace *TheFurn ;
extern Batch *BatchTab[MaxBatchNb] ;
//Product *Pdis = NULL;
extern Metal *MetalTab[NbCodSteel];
extern EndzTargetList *TheTargetList;
//extern int RunSpt = 1;
//extern float CalcPer = 60;
TDirAppl DirAppl;

//--------------------------------------------------------------------------
void makePHis(int event, Product *Pdis)
{
	(void)createMetalList(MetalTab);
	TheTargetList = createTargetList(1,Path);
	TheFurn = new Furnace (0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);

	if (!TheFurn )
			prtdebug("MAKEPHIS36 : Erreur création four");

	TheFurn ->dataload(1,Path);
	int NbBatch = createBatchList(BatchTab,TheFurn ,MetalTab,1,Path);

	if (NbBatch>0)
	{
		TheFurn->setChargingBatch(BatchTab[0]);
		TheFurn->pro_dataload(BatchTab,1,Path);
	}
	switch(event)
	{
		case 1: //charging event, first, get reference on last charged
				if (TheFurn->nbProd())
				{
					Deque& FP = TheFurn->theProd();
					Product& PCha = (Product&)(FP.peekLeft());
					updateHistory(TheFurn,TheTargetList,event,&PCha);

				} 		break;

		case 2: //periodical event
				 updateHistory(TheFurn,TheTargetList,event,NULL);
						break;

		case 5: //discharging event, first, build data of last discharged
				//prtdebug("MakePHis(2) : Appel a updateHistory()");
				updateHistory(TheFurn,TheTargetList,event,Pdis);
						break;
	} //switch event

	for(int i=1;i<NbCodSteel;i++)
	{
		if(MetalTab[i]!=NULL)
			delete MetalTab[i];
	}
	delete TheTargetList ;
	delete TheFurn ;

	for (i=0; i<NbBatch; i++)
		if (BatchTab[i] != NULL)
		{
			delete BatchTab[i];
			BatchTab[i] = NULL;
		}
} //makePHis

//--------------------------------------------------------------------------
int run_makephis(int event)
{
	if (DBread_path())
	{
		printf("\n MTPHIS ERROR: Check the file %s.DIR \n", NomApplication);
		return(-1);
	}
	//repertoires historique produit

	strcpy(DirAppl.PHisDir,DB_Path_hisprod);
	strcpy(DirAppl.THisDir,DB_Path_histemp);

	WriteStatus("MTPHIS: Start");

	makePHis(event, NULL);

	WriteStatus("MTPHIS: Complete");

	return(0);
}
