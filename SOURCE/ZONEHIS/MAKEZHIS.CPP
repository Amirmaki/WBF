#include "..\\zonehis\\makezhis.h"
#include <io.h>
#include <conio.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <share.h>
#include <string.h>
#include <stdio.h>
#include <alloc.h>
#include <stdlib.h>
#include <dos.h>
#include "database.h"
#include "db.h"
//#include "idwindow.h"
#include "winstruc.h"


// definition des classes TACT
#include "prodtlst.h"
#include "prodtype.h"
#include "endztlst.h"
#include "zone.h"
#include "furnace.h"
#include "prhislst.h"
#include "temphis.h"
#include "sp.h"

// creation objets
#define __OWNER_FUNC_H
#include "batch.h"
#include "metal.h"
#include "mtdbfunc.h"
#include "setpoint.h"
// pour linker (inutile !)
extern int      RunSpt;
extern int      HisEnable ;
extern float    CalcPer;
extern Furnace  *TheFurn;
//extern ProdHisList *TheHisList ;
extern Batch    *BatchTab[MaxBatchNb] ;

extern EndzTargetList *TheTargetList;
//--------------------------------------------------------------------------
int run_makezhis(void)
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
		// Lecture des données SP + ARRET + PACING
		int res = LoadDataSP() * LoadDataArret("") * LoadDataRunTime("");
		if (res >0) zoneTempHis(TheFurn);
	 } // end if NbBatch>0

  delete TheProdTypeList ;
  //TheProdTypeList = NULL;
  delete TheTargetList;
  //TheTargetList = NULL;
  for (i=1; i<NbCodSteel; i++)
	 { delete MetalTab[i];
		//MetalTab[i] = NULL;
	 }
  //MetalTab[0] = NULL;
  for (i=0; i<NbBatch; i++)
	 if (BatchTab[i] != NULL)
		{
		delete BatchTab[i];
		BatchTab[i] = NULL;
		}
  delete TheFurn;
  //TheFurn = NULL;
}
