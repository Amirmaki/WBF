/*--------------------------------------------------------------------------

		  PROGRAMME MTTRACK.CPP : tache de suivi des produits
		  ****************************************************

				TACT MULTI-TACHES

Creation : F.DESTREBECQ le 28/04/94
Modifs:  PF 16/12/94 Inclut l'acc‚l‚ration pour calcul en mode simulation
			PF 09/03/95 Passage aux codes aciers
			PF 30/03/95 Rajout communication COGNE
			PF 06/07/95 Rajout calcul objectifs "‚quivalents"
			JFC19/11/96 prise en compte de la donn‚e TDef LoadChargingBatch()
---------------------------------------------------------------------------*/
#define __OWNER_MTTRACK_H

#include <stdio.h>
// Definition des classes TACT
#include "globdat.h"
#include "database.h"
#include "endztlst.h"
#include "furnace.h"
#include "..\mttrack\mttrack.h"
#include "prodtlst.h"
#include "prodtype.h"
#include "product.h"
#include "temphis.h"
#ifndef __WINSTRUC_H
#include "winstruc.h"
#endif

#define __OWNER_FUNC_H       // ATTENTION : Garder ces include dans l'ordre !
#include "mtdbfunc.h"
//#define __OWNER_SPDATA_H
#include "spdata.h"
#include "sp.h"
#include "globfnct.h" //definition de round()
#include "wrstatus.h"
#include <time.h>
#include <math.h>
#include <string.h>
//#include "config.h"

#define ChIdBillDef	"BILLETTA %i SFORNATA DALLA SCHEDA %s"
#define ChPrechauff	"--------------- Fine Preriscaldo ---------------"
#define ChNormal	"------------- Fine Normalizzazione -------------"
#define ChLegend1	"Temperature Volta %5i Sup. %5i Media %5i Cuore %5i Inf. %5i (°C)"
#define ChLegend2	"Tempo soggiorno : %i (mn) "

#define MaxProd		100
//extern int Un_Produit_Defourne, NumFichImprBillDef;
//char  CommEnabled=0;
// pour linker (inutile !)
//extern int      RunSpt =  0; //pour linker
extern int      HisEnable ;
int 	OkImprBillDef = 0;
//float    CalcPer = 0; //pour linker

extern Furnace  *TheFurn;
extern Batch    *BatchTab[MaxBatchNb] ;
extern EndzTargetList *TheTargetList;
extern void makePHis(int event, Product *Pdis);

int Pas_de_produit_def=1;

//--------------------------------------------------------------------------
//   MTTRACK : renvoit 1 si bill enf. par l'entree du four
//                     2 si bill def. par la sortie du four.
//                     3 si bill enf. + bill def.
//                     0 sinon.
//--------------------------------------------------------------------------

int mttrack ()
{
	int err, i = 0, BillDefournee = 0;
	int recharge_signal = 0;
	int hist_def = 0; // demande hist apres enf. billette par l'entree.
	int hist_enf = 0; // demande hist apres defournement billette par la sortie
	Metal *MetalTab[NbCodSteel];
	createMetalList(MetalTab);
	ProdTypeList *TheProdTypeList = createProdTypeList(1,"");
	TheTargetList = createTargetList(1,"");
	TheFurn = new Furnace (0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0) ;
	TheFurn->dataload(1,"");

	//lecture et creation du batch a l'enfournement s'il existe
	DBopen ( DB_ChBatch_Item,0,"");
	int NbChBatch = DBNbRecord(DB_ChBatch_Item);
	DBclose(DB_ChBatch_Item);

	if (NbChBatch)
		LoadChargingBatch();

	int NbBatch = createBatchList(BatchTab,TheFurn,MetalTab,1,"");

	if (NbBatch >0)
	{
		char aux, PLCEvent[6]={0,0,0,0,0,0};
		TheFurn->setChargingBatch(BatchTab[NbBatch-1]);
		TheFurn->pro_dataload(BatchTab,1,"");
		int res = LoadDataArret("") * LoadDataRunTime("");
		int cht = 0; //temperature d'enf

		if (res)
		{
			res = LoadPLCEvent(PLCEvent,&cht,"");   // Charge les évenements du PLC

			if (res)                              // Traitement des évènements
			{
				int can_charge = TheFurn->testCharging(TheFurn->chargingAbs() , 0 );

				if (PLCEvent[0] && can_charge)		     // Chargement nouvelle billette
				{
					hist_enf = 1; //demande historique nouveau produit
					TheFurn->charge(TheFurn->chargingAbs(),0,0,0,cht);
					Product& NewP = (Product&)(TheFurn->theProd().peekLeft());

		  // Modif PF06/07/95 : Recherche objectifs
					#ifdef PACINGMODE
					EndzTarget& PTarg =
					TheTargetList->getCurve(NewP.prodRate(),
						NewP.theBatch()->TypeNumber,NewP.chargingTemp());
//					TheTargetList->getCurve(30,1,293);
/*					FILE * f = fopen("batch.txt", "a+t");
					fprintf(f,"TypeNumber: %d Target: %d\n",
								NewP.theBatch()->TypeNumber, PTarg.getTarget(5));
					fclose(f);
*/					#else
					EndzTarget& PTarg =
					TheTargetList->getCurve(NewP.theBatch()->ProductionRate,
						NewP.theBatch()->TypeNumber,NewP.chargingTemp());
					#endif

					if (PTarg != (EndzTarget&)NOOBJECT)
					{
			// Modif JFC19/11/96
						int TDef=0.0, CorrTarg[NBZONE];
						float CorrObj=0.0;
						err = DBopen(DB_LstBatch_Item,0,"");  	   // R only
						// lecture Tdef pour le lot au defmt

						LireDB(DB_LstBatch_Item,0,DB_Batch_TDef,(char*)&TDef,sizeof(int));
						DBclose(DB_LstBatch_Item);

						err = DBopen(DB_CtrlFour_Item,0,"");  	   // R only
						// lecture correction d'objectifs
						LireDB(DB_CtrlFour_Item,0,DB_CFour_CorrObj,
						(char*)&CorrObj, sizeof(float));
						DBclose(DB_CtrlFour_Item);
//		  char tdef[5];
//		  sprintf(tdef,"Tø Def: %døC", TDef);
//		  WriteStatus(tdef);
//		  char corrobj[5];
//		  sprintf(corrobj, "CorrObj: %.2f", CorrObj);
//		  WriteStatus(corrobj);
//		  char corrtarg[5];
		  // Correction des objectifs pour chaque zones
						for (i = 1; i <= NBZONE; i++)
						{
							CorrTarg[i-1] = PTarg.getTarget(i);
//							sprintf(corrtarg, "Targ: %døC", CorrTarg[i-1]);
//							WriteStatus(corrtarg);
							CorrTarg[i-1] = (int)((float)CorrTarg[i-1]*(1+CorrObj)*(float)TDef
									/ (float)(PTarg.getTarget(NBZONE)));
//							sprintf(corrtarg, "CorrTarg: %døC", CorrTarg[i-1]);
//							WriteStatus(corrtarg);
						}
						// Mise … jour de l'objet
						PTarg.update(CorrTarg);
						// Fin Modif JFC19/11/96
						NewP.setTargets(PTarg.theCurve());
					}
					//positionner le flag enfornemt par enfnmt
					err = DBopen(DB_LstProd_Item,1,"");  	   // W only
					aux = 1;
					err*= DBwrite(DB_LstProd_Item,0,DB_Prod_EnfEnf,&aux,
							sizeof(char));  // Flag enfmt / enfmt
					err = DBclose(DB_LstProd_Item);
/*
#ifdef HIS
					if (Model.PHisEnable)		       // Historique Produit
					{
						Product& Pcha=(Product&)(TheFurn->theProd().peekLeft());
						makePHis(1,&Pcha);   // Historisation d'un nouveau pdt
					}
#endif
*/
				}

				if ((PLCEvent[1] == 2)||(PLCEvent[1] == 3))  // Avance long. 2
				{
					TheFurn->makeStep2(1);
					TheFurn->datasave("");      // Sauve heure dernière avance
				}

				if ((PLCEvent[1] == 1)||(PLCEvent[1] == 3))  // Avance long. 1
				{
					if (Pas_de_produit_def)
					{
						FixeCadenceTh();
						DBMeasProdRate(1);
					}
					TheFurn->makeStep(TheFurn->wbeamStep());
					TheFurn->datasave("");      // Sauve heure dernière avance
				}

				if ((PLCEvent[2] == 1)||(PLCEvent[2] == 3))   // Recul long. 1
				{
					TheFurn->makeStep(-TheFurn->wbeamStep());
					TheFurn->datasave("");      // Sauve heure dernière avance
				}

				if ((PLCEvent[2] == 2)||(PLCEvent[2] == 3))   // Recul long. 2
				{
					TheFurn->makeStep2(0);
					TheFurn->datasave("");      // Sauve heure dernière avance
				}
				// Defournement sur signql

				if (PLCEvent[3])
				{
					Product& Pdef = TheFurn->discharge(2);

					if ((Object&)Pdef != NOOBJECT)
					{
						TheFurn->DisBil_datasave(&Pdef,"");

						delete &Pdef;
//						DBMeasProdRate(0);
						BillDefournee=1;
						hist_def = 1;
					}
				}
				// Defournement par enfournement

				if (PLCEvent[4])
				{
					(void)(TheFurn->theProd()).getLeft();
				}
				// Recharge une billette par la sortie
				if (PLCEvent[5])
					recharge_signal = 1;
				else
					recharge_signal = 0;
			}
		}
		int nb_prod = TheFurn->nbProd();  // Teste dechargement en fin de four

		for (i=0 ; i < nb_prod ; i++)
		{
			Product& Pdis = TheFurn->discharge(0);     // Renvoie pdt défourné

			if ((Object&)Pdis !=NOOBJECT)
			//if ((PLCEvent[3]==1)&&((Object&)Pdis!=NOOBJECT))
			{
				Pas_de_produit_def=0;
				hist_def = 1;
//				DBMeasProdRate(0);                // Calcule la cadence mesurée
/*
#ifdef HIS
				if (Model.PHisEnable)		       // Historique Produit
				{
					makePHis(5,&Pdis);	    // Historisation du défournement
					prodTempHis(&Pdis,TheTargetList,0,0);
				}
#endif
*/

#ifdef COM
				if (OkImprBillDef) // Impression billettes défournée (cf MAIN)
				{
					char auxstr[80];
					ofstream comtrack;
					// Alterner 2 fichiers d'impression pour éviter conflits

					if (NumFichImprBillDef==0)
						comtrack.open(FichImprBillDef0,ios::out|ios::trunc);
					else
						comtrack.open(FichImprBillDef1,ios::out|ios::trunc);

					sprintf(auxstr,ChIdBillDef,Pdis.readNumOrdre(),
						Pdis.theBatch()->NrLotto);

					comtrack << endl << auxstr << endl;
					comtrack << ChPrechauff << endl;
					sprintf(auxstr,ChLegend1,Pdis.readRTPrech(),
					Pdis.readTTPrech(),Pdis.readMTPrech(),
					Pdis.readCTPrech(),Pdis.readBTPrech());
					comtrack << auxstr << endl;

					sprintf(auxstr,ChLegend2,(int)(Pdis.readResTPrech()/60.0));
					comtrack << auxstr << endl;
					comtrack << ChNormal << endl;

					sprintf(auxstr,ChLegend1,Pdis.readRTNorm(),
					Pdis.readTTNorm(),Pdis.readMTNorm(),
					Pdis.readCTNorm(),Pdis.readBTNorm());
					comtrack << auxstr << endl;

					sprintf(auxstr,ChLegend2,(int)(Pdis.readResTNorm()/60.0));
					comtrack << auxstr << endl;

					comtrack.close();
					Un_Produit_Defourne = 1;
				}
#endif

				TheFurn->DisBil_datasave(&Pdis,"");
				delete &Pdis;
				BillDefournee = 1;
			}
		} // end for

		TheFurn->pro_datasave(1,"");      // Sauve données produit ds database

		if (recharge_signal)
			recharge_billet();

		if (BillDefournee)
		{
			char NbBillLot, NbBillDef;
			// Destruction lot défourné -> défournement billette #1 lot suivant
			err = purgeBatchList();
	  // Gestion du nb de billettes défournées / lot
			err = DBopen(DB_LstBatch_Item,2,"");
			err+= LireDB(DB_LstBatch_Item,0,DB_Batch_NbBill,&NbBillLot,
				sizeof(char));    	    // Batch au défmt = N° 0
			err*= LireDB(DB_LstBatch_Item,0,DB_Batch_NbBillDef,&NbBillDef,
				sizeof(char));
			NbBillDef++;
			err*= DBwrite(DB_LstBatch_Item,0,DB_Batch_NbBillDef,&NbBillDef,
				sizeof(char));
			err+= DBclose(DB_LstBatch_Item);
			// En mode "normal" la communication des données de la billette
			// défournée se fait dans CommPLC() de SITECOMM.CPP
	  #ifdef COM

			if (CommEnabled == 1)    	    // Ecrit données billette ds BdD
			{
				err = TransfDB2PLCLot();    // MàJ données comm° selon BdD
		  #ifdef COMMUNICATION
				err = EcrirePLCLot();		 // Renvoie au PLC dw6..dw72
		  #else
				err = EcrirePLCLotSim();
		  #endif
			}
	  #endif
		}
	  //TheFurn->pro_datasave(1,"");      // Sauve données produit ds database
		FixeCadenceTh();  // Fixe la cadence théorique dans le fichier runtime
	} // end if NbBatch > 0
	delete TheTargetList;
	delete TheProdTypeList;

	for (i=1; i<NbCodSteel; i++)
		if (MetalTab[i] != NULL)
			delete MetalTab[i];

	for (i=0; i<NbBatch; i++)
		if (BatchTab[i] != NULL)
			delete BatchTab[i];

	delete TheFurn;

  // calcul du code de retour
	if (hist_def && hist_enf)
		return (3);

	if (hist_def)
		return (2);

	if (hist_enf)
		return (1);

	return(0);
}
//--------------------------------------------------------------------------
int FixeCadenceTh()     // Fixe la cadence théorique dans le fichier runtime
{
	int status = 0;       // status d'ouverture
	int erreur = 1;       // si pb de lecture  erreur = 0
	char *DB_path ="";  // répertoire de la database

	if ( TheFurn->nbProd() ) //Ajout FD 7/2/96
	{
		Product& theOldestCharged = (Product&)((TheFurn->theProd()).peekRight());
		float TheProd = theOldestCharged.theBatch()->ProductionRate;
		TheoProd = TheProd; //initialisation pour remplissage du four et calcul de la cadence
		status = DBopen(DB_Runtime_Item,1,DB_path);

		if (!status)
			erreur *= DBwrite(DB_Runtime_Item,0,DB_Runtime_TheoProd,(char*)&TheProd,
			  sizeof(float));

		DBclose(DB_Runtime_Item);
	}//nbProd()

	if (!status && erreur)
		return(1);
	else
		return(0);
}
//--------------------------------------------------------------------------
int DBMeasProdRate(int remplissage)
// GŠre le calcul de la cadence mesur‚e, et renvoie 1 si OK sinon 0
{
	int status = 0;       // status d'ouverture
	int erreur = 1;       // si pb de lecture  erreur = 0
	char *DB_path =Path;  // Répertoire de la database
	float OldMeasProd;
	long OldTimeDis;
	long OldTimeStep;

	status = DBopen(DB_Runtime_Item,2,DB_path);

	if (!status)                               // Lecture de la base de donn‚e
	{
		erreur *= LireDB(DB_Runtime_Item,0,DB_Runtime_MeasProd,
				(char*)&OldMeasProd,sizeof(float));

		erreur *= LireDB(DB_Runtime_Item,0,DB_Runtime_DateOfLastDis,
				 (char*)&OldTimeDis,sizeof(long));

		erreur *= LireDB(DB_Runtime_Item,0,DB_Runtime_DateOfLastStep,
				(char*)&OldTimeStep,sizeof(long));
	}

	if (!status && erreur)
	{
		time_t actuelTime = time(NULL);
		float NewMeasProdRate;

		if(remplissage)
		{
			NewMeasProdRate = calcMeasProdRate(OldTimeStep,OldMeasProd,
						 actuelTime);
		}
		else
		{
			// Appelle la fonction de calcul de la cadence mesur‚e (cf ci-dessous)
			NewMeasProdRate = calcMeasProdRate(OldTimeDis,OldMeasProd,
							 actuelTime);

			erreur *= DBwrite(DB_Runtime_Item,0,DB_Runtime_DateOfLastDis,
				(char*)&actuelTime,sizeof(long));
		}

		  // Sauve cadence & temps dernier d‚fmt ds la base de donn‚es
		erreur *= DBwrite(DB_Runtime_Item,0,DB_Runtime_MeasProd,
			(char*)&NewMeasProdRate,sizeof(float));

	}
	DBclose(DB_Runtime_Item);

	if (!status && erreur)
		return(1);
	else
		return(0);
}
//--------------------------------------------------------------------------
float calcMeasProdRate(long LastDisTime, float LastMeasProd, long actTime)
// Calcul de la cadence mesur‚e au d‚fournement
{
	if (LastDisTime)
		{ // Calculate theoric delta tdis
//		float theoDeltaTdis;
		double deltaTdis = difftime(actTime,LastDisTime);	     // Temps ‚coul‚

		if (SimulMode)// Mode Simulation
			{
			DBopen(DB_Simul_Item, 2,"");
			LireDB(DB_Simul_Item,0,DB_Simul_Acc,(char*) &SimulAcc,sizeof(float));
			DBclose(DB_Simul_Item);
			deltaTdis = deltaTdis/SimulAcc;
			}

		float production = (float)TheoProd;      // Lu ds BdD, cf MTDBFUNC.CPP

/*		if (production)
			theoDeltaTdis = 3600 / production;
		else
			theoDeltaTdis = 0;
*/
		if (deltaTdis > 20)
			{
			// calculate new production
			production = 3600 / deltaTdis;
			LastMeasProd = 0.7*LastMeasProd + 0.3*production;
			MeasProd = LastMeasProd;
			}
		}
	else // LastDisTime == 0, it's the first discharged prod
		{
		MeasProd = TheoProd;
		LastMeasProd = (float)MeasProd;
		}

	LastDisTime = actTime;

	if (MeasProd > MaxProd)
		MeasProd = MaxProd;

	return(MeasProd);
}

int LoadChargingBatch()
{
	char aux1, nom[LgrChLot0], datetab[6];
	char nouveau_nom[LgrChLot0]; //nom du nouveau lot
	int status = 0, erreur = 1, Id, aux, auxB, Width, Length, Thick, TDef;
	float aux2;
	struct tm *ladate;
	long ti = time(NULL);

	status = DBopen(DB_LstBatch_Item,2,"");
	int NBRecordBatch = DBNbRecord(DB_LstBatch_Item);

	if (NBRecordBatch > 0)
	{
 /*      VB: 1-19-1998

		erreur = LireDB(DB_LstBatch_Item,NBRecordBatch-1,DB_Batch_Id,(char *)&Id,
			sizeof(int));
*/
		erreur = LireDB(DB_LstBatch_Item,NBRecordBatch-1,DB_Batch_Name,nom,
			  LgrChLot0*sizeof(char));
	}
//	else    //         VB: 1-19-1998
//		Id = -1;
//	Id++;                                         // Identit‚ du nouveau batch


	char type;

	status += DBopen(DB_ChBatch_Item,2,"");
	erreur *= LireDB(DB_ChBatch_Item,0,DB_ChBatch_BName,nouveau_nom,10*sizeof(char));
	nouveau_nom[LgrChLot0-1] = '\0';
 // test si le nom du nouveau batch est different du nom du dernier lot charge

//	FILE * f = fopen("batch.txt", "a+t");

	if ((NBRecordBatch > 0) && (strlen(nom)==strlen(nouveau_nom)) )
		if ( !strncmp(nom,nouveau_nom,strlen(nom)) )
		{
			DBclose(DB_ChBatch_Item);
			DBclose(DB_LstBatch_Item);
//			fprintf(f,"batch identique: %s\n", nouveau_nom);
			return(0);
		}

	erreur *= LireDB(DB_ChBatch_Item,0,DB_ChBatch_SteelCode, (char *) &type, sizeof(char));
	int SelCodeSteel=(int)type;
	int SelWidth=0;
	int SelThick=0;
	int SelTDef=0;
	int CodeSteel=0;

	erreur *= LireDB(DB_ChBatch_Item,0,DB_ChBatch_Width,(char*)&SelWidth,
	  sizeof(int));
	erreur *= LireDB(DB_ChBatch_Item,0,DB_ChBatch_Thick,(char*)&SelThick,
	  sizeof(int));
	erreur *= LireDB(DB_ChBatch_Item,0,DB_ChBatch_TDef,(char *)&SelTDef,
		sizeof(int));

//	fprintf(f,"SelWidth: %d SelTDef: %d\n", SelWidth, SelTDef);

	// Ouverture de la base de données en lecture
	status += DBopen(DB_TypProd_Item,0,"");
	status += DBopen(DB_CfgModel_Item,0,"");

	int NbRecord = DBNbRecord(DB_TypProd_Item);

	if (!status)
	{ // Chargement des données Type de produit de la database
		int i=0, Found=0, Num;

		while ((!Found) && (i < NbRecord))
		{
			erreur *= LireDB(DB_TypProd_Item,i,DB_TProd_Num,(char *)&Num,
				sizeof(int));
			erreur *= LireDB(DB_TypProd_Item,i,DB_TProd_Width,(char*)&Width,
				sizeof(int));
			erreur *= LireDB(DB_TypProd_Item,i,DB_TProd_Thick,(char*)&Thick,
				sizeof(int));
			erreur *= LireDB(DB_TypProd_Item,i,DB_TProd_Code,&aux1,
				sizeof(char));

			erreur *= LireDB(DB_TypProd_Item,i,DB_TProd_ReqDisT,(char*)&TDef,
				sizeof(int));

			CodeSteel = (int)aux1;

			if (CodeSteel==SelCodeSteel && Width==SelWidth && Thick==SelThick
					&& TDef==SelTDef)
				Found++;
			else
				i++;
		}

//		fprintf(f,"Width: %d TDef: %d\n", Width, TDef);
//		fclose(f);

		if (Found)
		{
			aux1 = 1;
			erreur *= DBwrite(DB_ChBatch_Item,0,DB_ChBatch_ACK,&aux1,sizeof(char));
			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_Id,
				 (char *)&Id,sizeof(int));

			char nom[10];
			erreur  = LireDB(DB_ChBatch_Item,0,DB_ChBatch_BName,nom,10*sizeof(char));
			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_Name,
				 nom,10*sizeof(char));

			erreur  = LireDB(DB_ChBatch_Item,0,DB_ChBatch_CastName,nom,10*sizeof(char));
			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_Coulee,
				 nom,10*sizeof(char));
			erreur  = LireDB(DB_ChBatch_Item,0,DB_ChBatch_SteelName,nom,10*sizeof(char));

			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_Acier,
				 nom,10*sizeof(char));
			ladate  = localtime(&ti);

			datetab[0] = ladate->tm_mday; datetab[1] = ladate->tm_mon + 1;
			datetab[2] = (ladate->tm_year) ;
			datetab[3] = ladate->tm_hour; datetab[4] = ladate->tm_min;
			datetab[5] = ladate->tm_sec;

			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_CharDate,
				 &(datetab[0]),6*sizeof(char));

			memset(datetab,0,6*sizeof(char));

			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_DiscDate,
				&(datetab[0]),6*sizeof(char));
			erreur *= LireDB(DB_ChBatch_Item,0,DB_ChBatch_Width,(char *)&Width,
				sizeof(int));
			erreur *= LireDB(DB_ChBatch_Item,0,DB_ChBatch_Length,(char *)&Length,
				sizeof(int));
			erreur *= LireDB(DB_ChBatch_Item,0,DB_ChBatch_Thick,(char *)&Thick,
				sizeof(int));
			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_Width,
				(char *)&Width,sizeof(int));
			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_Thick,
				(char *)&Thick,sizeof(int));
			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_Length,
				(char *)&Length,sizeof(int));

			erreur *= LireDB(DB_ChBatch_Item,0,DB_ChBatch_Weight,(char *)&aux,
				sizeof(int));
			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_Weight,
				(char *)&aux,sizeof(int));
			erreur *= LireDB(DB_TypProd_Item,i,DB_TProd_Code,&aux1,sizeof(char));
			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_Code,
				&aux1,sizeof(char));

			erreur *= LireDB(DB_ChBatch_Item,0,DB_ChBatch_ThProd,(char *)&aux2,
				sizeof(float));
			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_ThProd,
				(char *)&aux2,sizeof(float));
			erreur *= LireDB(DB_ChBatch_Item,0,DB_ChBatch_NbProd,(char *)&aux,
				sizeof(int));
			aux1 = (char) aux;
			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_NbBill,
				&aux1,sizeof(char));
			// Modif JFC18/11/96
			erreur *= LireDB(DB_ChBatch_Item,0,DB_ChBatch_TDef,(char *)&aux,
				sizeof(int));
			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_TDef,
				(char *)&aux,sizeof(int));

//----------------------------------------------------------------------------
//VB 1-20-98 ajout de PO Number et Job ID
			erreur *= LireDB(DB_ChBatch_Item,0,DB_ChBatch_PONumber,(char *)&auxB,
				sizeof(int));
			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_PONumber,
				(char *)&auxB,sizeof(int));

			erreur *= LireDB(DB_ChBatch_Item,0,DB_ChBatch_JobID,(char *)&auxB,
				sizeof(int));
			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_Id,
				(char *)&auxB,sizeof(int));
//----------------------------------------------------------------------------
			aux1 = 0;

			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_NbBillDef,
				&aux1,sizeof(char));
			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_FinEnfmt,
				&aux1,sizeof(char));
			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_2Step,
				&aux1,sizeof(char));
			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_Hetero,
				&aux1,sizeof(char));
			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_DbleFile,
				(char *)&aux1,sizeof(char));
			erreur *= LireDB(DB_CfgModel_Item,0,DB_Model_KSurf,(char *)&aux1,
				sizeof(char));
			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_SurfCode,
				(char *) &aux1, sizeof(char));
			erreur *= LireDB(DB_CfgModel_Item,0,DB_Model_Epsif1,(char *)&aux2,
				sizeof(float));
			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_Psi,
				(char *)&aux2,sizeof(float));

			if(!Thick)
			{
				erreur *= LireDB(DB_CfgModel_Item,0,DB_Model_HCoef3,
					(char *)&aux2,sizeof(float));
				erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_HCoeff,
					(char *) &aux2, sizeof(float));
				float Volume = M_PI * (float)Width * (float)Width
					* (float)Length * 0.25;
				Volume = Volume * 1e-9;

				erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_Volume,
				(char *)&Volume,sizeof(float));
			}
			else
			{
				erreur*=LireDB(DB_CfgModel_Item,0,DB_Model_HCoef1,
						(char *)&aux2,sizeof(float));
				erreur*=DBwrite(DB_LstBatch_Item,NBRecordBatch,
				DB_Batch_HCoeff,(char *)&aux2,sizeof(float));
				float Volume = (float)Width * (float)Thick * (float)Length;
				Volume = Volume * 1e-9;
				erreur*=DBwrite(DB_LstBatch_Item,NBRecordBatch,
				DB_Batch_Volume,(char *)&Volume,sizeof(float));
			}
			int num=0;

			erreur *= LireDB(DB_TypProd_Item,i,DB_TProd_Num,(char *)&num,sizeof(int));

			aux1 = (char) num;

			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_ProdType,
				&aux1,sizeof(char));
		} //end if (Found)
		else
		{
			aux1 = -1;

			erreur *= DBwrite(DB_ChBatch_Item,0,DB_ChBatch_ACK,&aux1,sizeof(char));
		}
		// Fermeture de la database
		DBclose(DB_TypProd_Item);
		DBclose(DB_LstBatch_Item);
		DBclose(DB_CfgModel_Item);
		DBclose(DB_ChBatch_Item);
		//DBdelete(DB_ChBatch_Item,0,"");

		if (erreur)
			return(1);
		else
			return(-1);
	}
	else // ProblŠme d'ouverture du fichier
	{
		DBclose(DB_TypProd_Item);
		DBclose(DB_LstBatch_Item);
		DBclose(DB_CfgModel_Item);
		DBclose(DB_ChBatch_Item);
		return (-1);
	}
}

////////////////////////////////////////////////////////////////////////////
//                     PROGRAMME PRINCIPAL
////////////////////////////////////////////////////////////////////////////
int run_mttrack()
{
  // lecture des chemins d'accŠs aux fichiers de TACT
/*  if ( DBread_path() )
	{
	  printf("\n ERROR : Verify the file TACT.CFG \n");
	  WriteStatus ("TRACKING: Error in file TACT.CFG");
	  exit (-1);
	}
*/

  WriteStatus("TRACKING:start");
  DBdansModel();  // M…J ModŠle d'aprŠs BdD
  // Flags de calcul et timers
  SimulMode = 1; //mode simulation
  int code_retour = mttrack();
  WriteStatus("TRACKING:complete");
  return(code_retour);
}

