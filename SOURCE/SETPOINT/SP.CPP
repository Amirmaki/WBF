/*--------------------------------------------------------------------------
		 PROGRAMME SETPOINT.CPP : tache de calcul de consigne
		 ****************************************************

			   TACT MULTI-TACHES

		 Creation : JLC le 20/09/94
		 Modif    : PF9/03/95 Passage aux codes aciers

---------------------------------------------------------------------------*/
//#define DEBUG
#define __OWNER_SP_H
#define __TACT3__

#ifdef DEBUG
#define __OWNER_MAIN_H
#endif
//#include <io.h>
//#include <conio.h>
#include <fcntl.h>
#include <sys/stat.h>
//#include <share.h>
#include <string.h>
#include <stdio.h>
//#include <alloc.h>
#include <stdlib.h>
//#include <dos.h>
#include "database.h"
#include "db.h"
#include "globdat.h"
#include "winstruc.h"
#include "sp.h"
#define __OWNER_SPDATA_H
#include "spdata.h"

// definition des classes TACT
#include "prodtlst.h"
#include "prodtype.h"
#include "endztlst.h"
#include "zone.h"
#include "furnace.h"
//#include "prhislst.h"
#include "wrstatus.h"
// creation objets

#define __OWNER_FUNC_H
#include "batch.h"
#include "metal.h"
#include "mtdbfunc.h"
#include "setpoint.h"
#include <dos.h>
// pour linker (inutile !)
//extern int      RunSpt = 1;
#ifdef DEBUG
//float CalcTemp;
int RunSpt = 0;
float CalcPer = 0.0;
Furnace  *TheFurn;
Batch    *BatchTab[MaxBatchNb];
EndzTargetList *TheTargetList;
#else
extern int      HisEnable ;
extern float    CalcPer;
extern Furnace  *TheFurn;
extern Batch    *BatchTab[MaxBatchNb] ;

extern EndzTargetList *TheTargetList;
#endif
//--------------------------------------------------------------------------
void SetPoint ()
{ int i = 0;
  Metal *MetalTab[NbCodSteel];

  createMetalList(MetalTab);
  ProdTypeList *TheProdTypeList = createProdTypeList(1,"");
  TheTargetList = createTargetList(1,"");
  TheFurn = new Furnace (0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0) ;
  TheFurn->dataload(1,"");
  int NbBatch = createBatchList( BatchTab , TheFurn , MetalTab , 1 , "" );
  if (NbBatch >0)
	{ TheFurn->setChargingBatch(BatchTab[NbBatch-1]);
	  TheFurn->pro_dataload(BatchTab,1,"");
	  TheFurn->ctrl_dataload(1,"");
	  // Calcul des consignes
	  int nbsec=(int)(OldTime2-OldTime);      // tps depuis dernier calctemp
	  int nbwsec=(int)(OldTime2-OldTime1);  // tps depuis le dernier mttrack
	  if (nbsec < 0)  nbsec = 0;
	  if (nbwsec < 0) nbwsec = 0;
	  // Lecture des données SP + ARRET + PACING
	  int res = LoadDataSP() * LoadDataArret("") * LoadDataRunTime("");
	  if (res > 0) calcSetPoint(TheFurn,nbsec,nbwsec);
	  //printf("sp.C : SPT Z4 = %i \n",ZoneSetPoint[4] - 273);
	  WriteSPinDB();
	  // Sauvegarde les données produit dans la database
	  TheFurn->pro_datasave(1,"");
	} // end if NbBatch>0
  delete TheProdTypeList ;
  delete TheTargetList;
  for (i=1; i<NbCodSteel; i++) delete MetalTab[i];
  for (i=0; i<NbBatch; i++) if (BatchTab[i] != NULL) delete BatchTab[i];
  //for (i=0 ; i<50 ; i++) if (BatchTab[i] != NULL) delete BatchTab[i] ;
  delete TheFurn;
}
////////////////////////////////////////////////////////////////////////////
//
//                          PROGRAMME PRINCIPAL
//
////////////////////////////////////////////////////////////////////////////
#ifdef DEBUG
int main()
#else
int run_sp()
#endif
{
  // lecture des chemins d'acce aux fichiers de TACT
  if ( DBread_path() )
	{
		printf("\n ERROR : Verify the file TACT.CFG \n");
		return (-1);
	}

	WriteStatus("SP:start");
	DBdansModel();  // MàJ Modèle d'après BdD
	SetPoint();
	WriteStatus("SP:complete");
	return(0);
}
