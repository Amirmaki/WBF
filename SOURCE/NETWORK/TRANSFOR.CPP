// OWLCVT 02/10/97 10:31:57
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include "..\network\transfor.h"
#include "..\network\databloc.h"
#include "database.h"

void echangeBdD(int *tab, int plcnum)
{
	int i;
if (!plcnum)
	{
	tab[DATA_ACK] = 1; // positionne l'acknowledge
	tab[DATA_READY] = 0; // reset data ready
// Rangement des données dans les differents fichiers de la BdD
	char chartab[6];
	char chartab2[11];

#ifdef NB_BURNERS
	char auxtabchar[NB_BURNERS], auxtabchar2[NB_BURNERS];
#endif
	float auxfloat;

	chartab[0] = (char)tab[PROD_CHARGED];
	chartab[1] = (char)tab[END_FWD_MVT];
	chartab[2] = (char)tab[END_BCK_MVT];
	chartab[3] = (char)tab[DIS_BY_DIS];
	chartab[4] = 0;
	chartab[5] = (char)tab[CHARG_BY_DIS];

	if (!DBopen(DB_PLCEvent_Item, 1,"" )) //Ecriture
		{
		DBwrite(DB_PLCEvent_Item, 0, DB_PLCEvent_Charge, chartab, 6*sizeof(char));
		DBclose(DB_PLCEvent_Item);
		}

	for (i=0; i < 5 ; i++)
		{
		chartab2[2*i]   = (char)((tab[STEEL_GRADE+i] >> 8) & 0xff);
		chartab2[2*i+1] = (char)(tab[STEEL_GRADE+i]        & 0xff);
		}

	if (!DBopen(DB_ChBatch_Item, 1, "")) // Ouvert en écriture
		{
		DBwrite(DB_ChBatch_Item, 0, DB_ChBatch_SteelName,(char*)&chartab2, 10*sizeof(char));
//		DBwrite(DB_ChBatch_Item, 0, DB_ChBatch_BatchId	,(char*)&tab[JOB_ID], sizeof(int));
		DBwrite(DB_ChBatch_Item, 0, DB_ChBatch_Width    ,(char*)&tab[BLOOM_WIDTH], sizeof(int));
		DBwrite(DB_ChBatch_Item, 0, DB_ChBatch_Thick    ,(char*)&tab[BLOOM_THICK], sizeof(int));
		DBwrite(DB_ChBatch_Item, 0, DB_ChBatch_Length   ,(char*)&tab[BLOOM_LENGTH], sizeof(int));
		DBwrite(DB_ChBatch_Item, 0, DB_ChBatch_NbProd   ,(char*)&tab[NB_BLOOM], sizeof(int));
		DBwrite(DB_ChBatch_Item, 0, DB_ChBatch_TDef     ,(char*)&tab[DISCH_TEMP], sizeof(int));
		DBwrite(DB_ChBatch_Item, 0, DB_ChBatch_PONumber ,(char*)&tab[PO_NUMBER], sizeof(int));
		DBwrite(DB_ChBatch_Item, 0, DB_ChBatch_JobID ,(char*)&tab[JOB_ID], sizeof(int));
		DBclose(DB_ChBatch_Item);
		}

	auxfloat = (float)tab[STEP_LENGTH];

	if (!DBopen(DB_CfgFour_Item, 1, "")) // Ouvert en écriture
		{
		DBwrite(DB_CfgFour_Item, 0, DB_Four_Step1, (char*)&auxfloat, sizeof(float));
		DBclose(DB_CfgFour_Item);
		}
	}
else
	{
// VB : 1-20-98
	int auxint = tab[RECUP_TEMP] + 273;
	int auxtabint1[NBZONE];
	float auxtabfloat1[NBZONES_T], auxtabfloat2[NBZONES_T];
#ifdef NBZONEI
	int auxtabint2[NBZONEI];
#endif
	for (i=0; i < NBZONES_T; i++)
		{
		auxtabint1[i] = tab[MEAS_TEMP_T+i] + 273;
		auxtabfloat1[i] = (float)tab[GAS_FLOW_T+i];
		auxtabfloat2[i] = (float)tab[AIR_FLOW_T+i];
		}

#ifdef NB_BURNERS
	for (i=0; i < NB_BURNERS; i++)
		auxtabchar[i] = (char)tab[BURNER_STAT_T+i];
#endif

	if (!DBopen(DB_CtrlFour_Item, 2, "")) // Ouvert en lecture/ecriture
		{
		DBwrite(DB_CtrlFour_Item, 0, DB_CFour_RecTemp   			   ,
				(char*)&auxint, sizeof(int));
		DBwrite(DB_CtrlFour_Item, 0, DB_CFour_ZTempSup + sizeof(int),
				(char*)&auxtabint1, NBZONES_T*sizeof(int));
		DBwrite(DB_CtrlFour_Item, 0, DB_CFour_QGSup +  sizeof(float),
				(char*)&auxtabfloat1, NBZONES_T*sizeof(float));
		DBwrite(DB_CtrlFour_Item, 0, DB_CFour_QASup +  sizeof(float),
				(char*)&auxtabfloat2, NBZONES_T*sizeof(float));
#ifdef NB_BURNERS
		DBwrite(DB_CtrlFour_Item, 0, DB_CFour_BurnStSup, (char*)&auxtabchar, NB_BURNERS*sizeof(char));
#endif
		}

#ifdef NBZONEI //F.H 25/08/97
	for (i=0; i < NBZONES_B; i++)
		{
		auxtabint2[i] = tab[TEMP_MESUREE_B+i] + 273;
		auxtabfloat1[i] = (float)tab[DEBIT_GAS_B+i];
		auxtabfloat2[i] = (float)tab[DEBIT_AIR_B+i];
		}

#ifdef NB_BURNERSI
	for (i=0; i < NB_BURNERSI; i++)
		auxtabchar[i] = (char)tab[ETAT_BRULEUR_B+i];
#endif
		DBwrite(DB_CtrlFour_Item, 0, DB_CFour_ZTempInf+sizeof(int), (char*)&auxtabint1, NBZONES_B*sizeof(int));
		DBwrite(DB_CtrlFour_Item, 0, DB_CFour_QGInf+sizeof(float), (char*)&auxtabfloat1, NBZONES_B*sizeof(float));
		DBwrite(DB_CtrlFour_Item, 0, DB_CFour_QAInf+sizeof(float), (char*)&auxtabfloat2, NBZONES_B*sizeof(float));
#ifdef NB_BURNERSI
		DBwrite(DB_CtrlFour_Item, 0, DB_CFour_BurnStInf, (char*)&auxtabchar, NB_BURNERSI*sizeof(char));
#endif
#endif // NBZONEI
		// lecture des consignes
		LireDB(DB_CtrlFour_Item, 0, DB_CFour_SPSup+sizeof(int),
				(char*)&auxtabint1, NBZONES_T*sizeof(int));
#ifdef NBZONEI //F.H 25/08/97
		LireDB(DB_CtrlFour_Item, 0, DB_CFour_SPInf+sizeof(int), (char*)&auxtabint2, NBZONES_B*sizeof(int));
#endif
		DBclose(DB_CtrlFour_Item);

	for (i=0; i < NBZONES_T; i++)
		tab[TEMP_SP_T+i] = auxtabint1[i] - 273;
	} //else
#ifdef NBZONEI
	for (i=0; i < NBZONES_B; i++)
		tab[TEMP_CONSIGNE_B+i] = auxtabint2[i] - 273;
#endif
}


