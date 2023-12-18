// OWLCVT 02/10/97 10:31:57
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
//#include "transfor.h"
//#include "databloc.h"
#include "winstruc.h"
#include "database.h"

void main()
{
	int tab[44];
   FILE *toto=fopen("donnees.txt","rt");
   for(int ind=0;ind<44;ind++)
   	{
   	fscanf(toto,"%d",tab);
      printf("Val %d lue : %d\n",ind,tab[ind]);
      }
   fclose(toto);

   /*
	tab[DATA_ACK] = 1; // positionne l'acknowledge
	tab[DATA_READY] = 0; // reset data ready
// Rangement des données dans les differents fichiers de la BdD
	char chartab[6];
	char chartab2[11];
   int i;

#ifdef NB_BURNERS
	char auxtabchar[NB_BURNERS], auxtabchar2[NB_BURNERS];
#endif
	float auxfloat, auxtabfloat1[NBZONES_H], auxtabfloat2[NBZONES_H];

	chartab[0] = (char)tab[CHARGE_PROD];
	chartab[1] = (char)tab[FIN_MVT_AV];
	chartab[2] = (char)tab[FIN_MVT_AR];
	chartab[3] = (char)tab[DEF_PAR_DEF];
	chartab[4] = 0;
	chartab[5] = (char)tab[ENF_PAR_DEF];

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
		DBwrite(DB_ChBatch_Item, 0, DB_ChBatch_BatchId	,(char*)&tab[JOB_ID], sizeof(int));
		DBwrite(DB_ChBatch_Item, 0, DB_ChBatch_Width    ,(char*)&tab[BLOOM_WIDTH], sizeof(int));
		DBwrite(DB_ChBatch_Item, 0, DB_ChBatch_Thick    ,(char*)&tab[BLOOM_THICK], sizeof(int));
		DBwrite(DB_ChBatch_Item, 0, DB_ChBatch_Length   ,(char*)&tab[BLOOM_LENGTH], sizeof(int));
		DBwrite(DB_ChBatch_Item, 0, DB_ChBatch_ProdNb   ,(char*)&tab[BLOOM_NUMBER], sizeof(int));
		DBwrite(DB_ChBatch_Item, 0, DB_ChBatch_TDef     ,(char*)&tab[DISCH_TEMP], sizeof(int));
		DBwrite(DB_ChBatch_Item, 0, DB_ChBatch_PONumber ,(char*)&tab[PO_NUMBER], sizeof(int));
		DBclose(DB_ChBatch_Item);
		}

	auxfloat = (float)tab[LONG_PAS];

	if (!DBopen(DB_CfgFour_Item, 1, "")) // Ouvert en écriture
		{
		DBwrite(DB_CfgFour_Item, 0, DB_Four_Step1, (char*)&auxfloat, sizeof(float));
		DBclose(DB_CfgFour_Item);
		}

// VB : 1-20-98
	int auxint = tab[TEMP_RECUP_H] + 273;
	int auxtabint1[NBZONE];
#ifdef NBZONEI
	int auxtabint2[NBZONEI];
#endif
	for (i=0; i < NBZONES_H; i++)
		{
		auxtabint1[i] = tab[TEMP_MESUREE_H+i] + 273;
		auxtabfloat1[i] = (float)tab[DEBIT_GAZ_H+i];
		auxtabfloat2[i] = (float)tab[DEBIT_AIR_H+i];
		}

#ifdef NB_BURNERS
	for (i=0; i < NB_BURNERS; i++)
		auxtabchar[i] = (char)tab[ETAT_BRULEUR_H+i];
#endif

	if (!DBopen(DB_CtrlFour_Item, 2, "")) // Ouvert en lecture/ecriture
		{
		DBwrite(DB_CtrlFour_Item, 0, DB_CFour_RecTemp   			   , (char*)&auxint, sizeof(int));
		DBwrite(DB_CtrlFour_Item, 0, DB_CFour_ZTempSup + sizeof(int), (char*)&auxtabint1, NBZONES_H*sizeof(int));
		DBwrite(DB_CtrlFour_Item, 0, DB_CFour_QGSup +  sizeof(float), (char*)&auxtabfloat1, NBZONES_H*sizeof(float));
		DBwrite(DB_CtrlFour_Item, 0, DB_CFour_QASup +  sizeof(float), (char*)&auxtabfloat2, NBZONES_H*sizeof(float));
#ifdef NB_BURNERS
		DBwrite(DB_CtrlFour_Item, 0, DB_CFour_BurnStSup, (char*)&auxtabchar, NB_BURNERS*sizeof(char));
#endif


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
		LireDB(DB_CtrlFour_Item, 0, DB_CFour_SPSup+sizeof(int), (char*)&auxtabint1, NBZONES_H*sizeof(int));
#ifdef NBZONEI //F.H 25/08/97
		LireDB(DB_CtrlFour_Item, 0, DB_CFour_SPInf+sizeof(int), (char*)&auxtabint2, NBZONES_B*sizeof(int));
#endif
		DBclose(DB_CtrlFour_Item);
	}

	for (i=0; i < NBZONES_H; i++)
		tab[TEMP_CONSIGNE_H+i] = auxtabint1[i] - 273;

#ifdef NBZONEI
	for (i=0; i < NBZONES_B; i++)
		tab[TEMP_CONSIGNE_B+i] = auxtabint2[i] - 273;
#endif
	*/
}
