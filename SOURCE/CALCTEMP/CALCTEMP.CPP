//--------------------------------------------------------------------------
/*
	     PROGRAMME CALCTEMP.CPP : Calcul temperature produits
	     ****************************************************

				TACT MULTI-TACHES

		  Creation : F.DESTREBECQ le 20/04/94
		  Modif    : PF9/03/95 : Passage aux codes aciers

-----------------------------------------------------------------------------
*/
//#define DEBUG

#define __OWNER_CALCTEMP_H

#ifdef DEBUG
#define __OWNER_MAIN_H
#endif
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
//#include <share.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include "database.h"

// definition des classes TACT
#include "prodtlst.h"
#include "prodtype.h"
#include "endztlst.h"
#include "zone.h"
#include "furnace.h"
#include "metal.h"
#include "..\calctemp\calctemp.h"
#include "batch.h"
// creation objets
#define __OWNER_FUNC_H
#include "mtdbfunc.h"
#ifdef DEBUG
#define __OWNER_SPDATA_H
#endif
#include "spdata.h"
#include "sp.h"
#include "globfnct.h" //definition de round()
#include "wrstatus.h"
#include "globdat.h"
#include "winstruc.h"
#include <fstream.h>
#ifdef DEBUG
//float CalcTemp;
int RunSpt = 0;
Furnace  *TheFurn;
Batch    *BatchTab[MaxBatchNb];
EndzTargetList *TheTargetList;
#else
extern float CalcTemp;
//extern int RunSpt = 0;
extern Furnace  *TheFurn;
extern Batch    *BatchTab[MaxBatchNb];
//extern EndzTargetList *TheTargetList;
#endif
void calctemp (float CalcPeriod)
{
	int i = 0;

	Metal *MetalTab[NbCodSteel];
	createMetalList(MetalTab);

	ProdTypeList *TheProdTypeList = createProdTypeList(1,Path);
//*  EndzTargetList *TheTargetList = createTargetList(1,"");
	TheFurn = new Furnace (0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0) ;
	TheFurn->dataload(1,Path);
	int NbBatch = createBatchList(BatchTab,TheFurn,MetalTab,1,Path);
	if (NbBatch>0)
		{
		TheFurn->setChargingBatch(BatchTab[0]);
		TheFurn->pro_dataload(BatchTab,1,Path);
		TheFurn->ctrl_dataload(1,Path);
		CalcPer = (float) CalcPeriod;	      // Réalise un calcul thermique
//		printf("AV calcul: OK\n");
		TheFurn->calcul();
//		printf("AP calcul: OK\n");
		TheFurn->updateResTime( CalcPer );
		TheFurn->UpdateDataEndZone( CalcPer); //mise à jour données fin de zone
		TheFurn->pro_datasave(0,Path); 	     // Sauve données produit ds BdD
		TheFurn->profile_datasave(0,Path);		  // Sauve le profil
		}// end if (NbBatch)

	delete TheProdTypeList;

	for (i=1; i<NbCodSteel; i++)
		delete MetalTab[i];

	for (i=0; i<NbBatch; i++)
		if (BatchTab[i] != NULL)
			delete BatchTab[i];

	delete TheFurn;
}
////////////////////////////////////////////////////////////////////////////
//                        PROGRAMME PRINCIPAL
////////////////////////////////////////////////////////////////////////////
#ifdef DEBUG
int main(int argc,char *argv[])
#else
int run_calctemp (float periode )
#endif
{
/*FILE *toto;
toto=fopen("toto.txt","a+");
if (periode>30.0||periode<30.0) fprintf(toto,"periode erronn‚e\n");
	else fprintf(toto,"periode OK\n");
fclose(toto);*/
  // pour debuger
//  printf("nbarg = %i arg1 = %s arg2 = %s \n",argc,argv[0],argv[1]);
  // lecture des chemins d'acces aux fichiers de TACT
#ifdef DEBUG
	if ( DBread_path() )
		{
		printf("\n ERROR : Verify the file TACT.CFG \n");
		return(-1);
		}
#endif
	WriteStatus("CALC-TEMP:start");

  // extraction de l'argument
#ifdef DEBUG
	if (argc != 2)
		{
		printf("\n ERROR : Bad number of arguments \n");
		WriteStatus ("CALC-TEMP:Error=Bad nb of arg");
		return (-1);
		}
#endif
  // conversion
#ifdef DEBUG
	float periode = atof(argv[1]);

	if (errno == ERANGE || periode <= 0)
		{
		printf("\n ERROR : argument must be a float \n");
		WriteStatus ("CALC-TEMP:Error=arg non float");
		return (-1);
		}
#endif
	if (periode <= 0)
		{
		printf("\n ERROR : argument must be positive (%.f)\n", periode);
		WriteStatus ("CALC-TEMP:Error=arg negative");
		return (-1);
		}

	DBdansModel();  // MàJ Modèle d'après BdD
  // Flags de calcul et timers
	SimulMode = 1; //mode simulation

	//calctemp(periode);
   calctemp(30);
	WriteStatus("CALC-TEMP:complete");
	return(0);
}
