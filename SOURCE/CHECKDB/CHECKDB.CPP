//#define __TACT3__
#define __OWNER_MAIN_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <dos.h>
#include <conio.h>
#include "checkdb.h"
#include "winstruc.h"
#include "database.h"
//********************************************************************
void Produit()
//Chargement des paramètres dynamiques
{
	int status = 0;                                      // status d'ouverture
	int erreur = 1;                            // si pb de lecture  erreur = 0
	int num_ligne=1;
	char *DB_path ="";        //chemin de direction où se trouve la database
	float *Abscisse, *MeanTemp, *TSurfTemp, *BSurfTemp, *Oxy, *Decarb, *CoreTemp;
	float *cadenceTh,*cadenceEq;

	int *Width,	*Thick, *BatchId, *InZone,	*InZoneTarget;
	long *ResTime;
	char *Code;
	//lecture du fichier LISTPROD.DTA
	status = DBopen(DB_LstProd_Item,0,DB_path);
	int NbRecord = DBNbRecord(DB_LstProd_Item);

	Width = new int[NbRecord];
	Thick = new int[NbRecord];
	Abscisse = new float[NbRecord];
	BatchId = new int[NbRecord];
	ResTime = new long[NbRecord];
	MeanTemp = new float[NbRecord];
	TSurfTemp = new float[NbRecord];
	BSurfTemp = new float[NbRecord];
	CoreTemp = new float[NbRecord];
	Oxy = new float [NbRecord];
	Decarb = new float [NbRecord];
	Code = new char[NbRecord];
	InZone = new int[NbRecord];
	InZoneTarget = new int[NbRecord];
	cadenceTh = new float[NbRecord];
	cadenceEq = new float[NbRecord];
	if (!status)
		{
		for (int i=0;i<NbRecord;i++)
			{
			erreur *= LireDB(DB_LstProd_Item,i,DB_Prod_Width,
				(char *) &Width[i],sizeof(int));
			erreur *= LireDB(DB_LstProd_Item,i,DB_Prod_Thick,
				(char *) &Thick[i],sizeof(int));
			erreur *= LireDB(DB_LstProd_Item,i,DB_Prod_Absc,
				(char *) &Abscisse[i],sizeof(float));
			erreur *= LireDB(DB_LstProd_Item,i,DB_Prod_BatchId,
				(char *) &BatchId[i],sizeof(int));
			erreur *= LireDB(DB_LstProd_Item,i,DB_Prod_ResTime,
				(char *) &ResTime[i],sizeof(long));
			erreur *= LireDB(DB_LstProd_Item,i,DB_Prod_MeanTemp,
				(char *) &MeanTemp[i],sizeof(float));
			erreur *= LireDB(DB_LstProd_Item,i,DB_Prod_TSurfTemp,
				(char *) &TSurfTemp[i],sizeof(float));
			erreur *= LireDB(DB_LstProd_Item,i,DB_Prod_BSurfTemp,
				(char *) &BSurfTemp[i],sizeof(float));
			erreur *= LireDB(DB_LstProd_Item,i,DB_Prod_CoreTemp,
				(char *) &CoreTemp[i],sizeof(float));
			erreur *= LireDB(DB_LstProd_Item,i,DB_Prod_OxyDepth,
				(char *) &Oxy[i],sizeof(float));
			erreur *= LireDB(DB_LstProd_Item,i,DB_Prod_DecDepth,
				(char *) &Decarb[i],sizeof(float));
			erreur *= LireDB(DB_LstProd_Item,i,DB_Prod_Code,
				(char *) &Code[i],sizeof(char));
			erreur *= LireDB(DB_LstProd_Item,i,DB_Prod_InZone,
				(char *) &InZone[i],sizeof(int));
			erreur *= LireDB(DB_LstProd_Item,i,
				(DB_Prod_Target+InZone[i]*sizeof(int)),
				(char *) &InZoneTarget[i],sizeof(int));
			erreur *= LireDB(DB_LstProd_Item,i,DB_Prod_ThProd,
				(char *) &cadenceTh[i],sizeof(float));
			erreur *= LireDB(DB_LstProd_Item,i,DB_Prod_EqProd,
				(char *) &cadenceEq[i],sizeof(float));
			}
		DBclose(DB_LstProd_Item);
		//affichage des donn‚es produit
		num_ligne=0;
		for (i=0;i<NbRecord;i++)
			{
			/*
	 printf("Pr N %3i Abs: %2.1f   Res T: %3d  Mean T: %4.1f TopT: %4.1f BotT: %4.1f\n",i,Abscisse[i],ResTime[i] /60,
	 MeanTemp[i] - 273 ,TSurfTemp[i] - 273,BSurfTemp[i] - 273);
			*/
			if (num_ligne==0)
				{
				printf("\nNum Batch  Z    Abs   Rest  Targ  TMean   TTop   TCor   TBot    EqCad    Diff");
				num_ligne++;
				}
			printf("\n%03i %05i  %1i  %5.2f   %4li  %4i %6.1f %6.1f %6.1f %6.1f    %5.2f   %5.1f",
				i,BatchId[i],InZone[i],Abscisse[i],ResTime[i]/60,InZoneTarget[i],
				MeanTemp[i]-273,TSurfTemp[i]-273,CoreTemp[i]-273,BSurfTemp[i]-273,cadenceEq[i],
				MeanTemp[i]-273-InZoneTarget[i]);
			num_ligne++;
			if (num_ligne==25)
				{
				delay(3000);
				num_ligne=0;
				}
			}//for
			delay(3000);
		}
	if (Width!=NULL) delete Width;
	if (Thick!=NULL) delete Thick;
	if (Abscisse!=NULL) delete Abscisse;
	if (BatchId!=NULL) delete BatchId;
	if (ResTime!=NULL) delete ResTime;
	if (MeanTemp!=NULL) delete MeanTemp;
	if (TSurfTemp!=NULL) delete TSurfTemp;
	if (BSurfTemp!=NULL) delete BSurfTemp;
	if (CoreTemp!=NULL) delete CoreTemp;
	if (Oxy!=NULL) delete Oxy;
	if (Decarb!=NULL) delete Decarb;
	if (Code!=NULL) delete Code;
	if (InZone!=NULL) delete InZone;
	if (InZoneTarget!=NULL) delete InZoneTarget;
	if (cadenceTh!=NULL) delete cadenceTh;
	if (cadenceEq!=NULL) delete cadenceEq;
	Width=NULL;
	Thick=NULL;
	Abscisse=NULL;
	BatchId=NULL;
	ResTime=NULL;
	MeanTemp=NULL;
	TSurfTemp=NULL;
	BSurfTemp=NULL;
	CoreTemp=NULL;
	Oxy=NULL;
	Decarb=NULL;
	Code=NULL;
	InZone=NULL;
	InZoneTarget=NULL;
	cadenceTh=NULL;
	cadenceEq=NULL;
}

void CtrlFour()
// Chargement des paramŠtres dynamiques
{ int status = 0;                                      // status d'ouverture
  int erreur = 1;                            // si pb de lecture  erreur = 0
  char *DB_path ="";        //chemin de direction où se trouve la database
  int *ZTempSup, *SPSup, *TargSup;
  float *GFlowSup, *AFlowSup;
  int RecTemp;
  char BurnState[9];
#ifdef __TACT3__
  int Spt_2 = 0;
#endif

  ZTempSup = new int[NBZONE];
  SPSup = new int[NBZONE];
  TargSup = new int[NBZONE];
  GFlowSup = new float[NBZONE];
  AFlowSup = new float[NBZONE];
  // lecture du fichier CTRLFOUR.DTA
  status = DBopen(DB_CtrlFour_Item,0,DB_path);
  if (!status)
	 { erreur *= LireDB(DB_CtrlFour_Item,0,DB_CFour_RecTemp,
				 (char *) &RecTemp,sizeof(int));
		erreur *= LireDB(DB_CtrlFour_Item,0,DB_CFour_ZTempSup,
				 (char *) ZTempSup,(NBZONE*sizeof(int)));
		erreur *= LireDB(DB_CtrlFour_Item,0,DB_CFour_SPSup,
				 (char *) SPSup,(NBZONE*sizeof(int)));
		erreur *= LireDB(DB_CtrlFour_Item,0,DB_CFour_TargSup,
				 (char *) TargSup,(NBZONE*sizeof(int)));
		erreur *= LireDB(DB_CtrlFour_Item,0,DB_CFour_QGSup,
				 (char *) GFlowSup,(NBZONE*sizeof(float)));
		erreur *= LireDB(DB_CtrlFour_Item,0,DB_CFour_QASup,
				 (char *) AFlowSup,(NBZONE*sizeof(float)));
		#ifdef __TACT3__
		  int local_pos = NBZONE*sizeof(int) ;
		  erreur *= LireDB(DB_CtrlFour_Item,0,DB_CFour_SPSup+local_pos,
					(char *) &Spt_2,sizeof(int));
		#endif

		erreur *= LireDB(DB_CtrlFour_Item, 0, DB_CFour_BurnStSup, BurnState, 9*sizeof(char));
		DBclose(DB_CtrlFour_Item);                 // fermeture de la database
	 }

	printf("RecTemp: %d\n", RecTemp-273);

	 for (int i=0; i<NBZONE; i++)
	  {
		printf("ZS nø%3i T: %4i SP: %4i Targ: %4i GFlow: %.2f AFlow: %.2f\n", i, ZTempSup[i]- 273, SPSup[i] - 273, TargSup[i], GFlowSup[i], AFlowSup[i]);
	  }
	 #ifdef __TACT3__
		printf("ZS n %3i T: %4i SP: %4i Targ: %4i\n", i, ZTempSup[i-1]- 273,Spt_2-273,TargSup[i-1] );
	 #endif

	for (i=0; i<9; i++)
		printf("BurnS %d: %d\t", i, BurnState[i]);

	printf("\n");
	if (ZTempSup!=NULL) delete ZTempSup;
	if (SPSup!=NULL) delete SPSup;
	if (TargSup!=NULL) delete TargSup;
	if (GFlowSup!=NULL) delete GFlowSup;
	if (AFlowSup!=NULL) delete AFlowSup;
  }

//****************************************
void LoadDelaysData()
{
  int status = 0; //status d'ouverture
  int erreur = 1; // si pb de lecture  erreur = 0

  //chemin de direction où se trouve la database
  char *DB_path ="";

  //ouverture de la base de donn‚es en lecture
  status = DBopen(DB_Delays_Item,0,DB_path);

  //lecture du nombre d'enregistrements
  NbRecord = DBNbRecord(DB_Delays_Item);

	if (!status)
		{
	 // chargement des donn‚es modŠle de la database
	 for (int i=0;i<NbRecord;i++)
	  {
	  erreur *= LireDB(DB_Delays_Item,i,DB_Delays_Id,(char *) &Id[i], sizeof(char));
	  erreur *= LireDB(DB_Delays_Item,i,DB_Delays_Debut,(char *) Debut[i], 6*sizeof(char));
	  erreur *= LireDB(DB_Delays_Item,i,DB_Delays_Fin,(char *) DureeInit[i], 3*sizeof(char));
	  erreur *= LireDB(DB_Delays_Item,i,DB_Delays_Duree,(char *) &Duree[i], sizeof(long));
	  }

	 //fermeture de la database
	 DBclose(DB_Delays_Item);
  }
}

int affiche(int *Data1, int*Data2)
	{
	printf("Stoppage at: %2.2i/%2.2i/%2.2i  ", Data1[0], Data1[1], Data1[2]);
	printf("%2.2i:%2.2i:%2.2i  ", Data1[3], Data1[4], Data1[5]);
	printf("Remaining duration: %2.2i days %2.2ih%2.2i\n", Data2[0], Data2[1],
		Data2[2]);
	return 0;
	}

//*********************************************
//affiche la dur‚e restante de l'arrˆt en court
//*********************************************
void Delays()
{
	int Beg[6];
	int Last[3];
	int i;

	//lecture dans la base de donn‚es
	LoadDelaysData();

	if (NbRecord)
		{
		for (i = 0; i < NbRecord; i++)
			{
			//affichage de la date de l'arrˆt programm‚
			TransformeDate(Debut[i], Beg);
			//transformation de la dur‚e sec -> jour, heure, min
			TransformeSecEnJHMin(Duree[i], Last);
			affiche(Beg, Last);
			}
		printf("\n");
		}
	else printf("No stoppage declared");
}

int TransformeDate(char *Data, int *tab)
{
	tab[0] = (int)Data[0];	//jour
	tab[1] = (int)Data[1];	//mois
	tab[2] = (int)Data[2];	//ann‚e
	tab[3] = (int)Data[3];	//heure
	tab[4] = (int)Data[4];	//min
	tab[5] = (int)Data[5];	//sec
	return 0;
}

//****************************************************
//cette fonction teste si le premier arrˆt est en court
//****************************************************
int TestArret()
{
	if (NbRecord)
		{
		long dureeInit = ((24*(long)DureeInit[0][0] +(long)DureeInit[0][1])*60
			+ (long)DureeInit[0][2])*60;
		if (Duree[0]<dureeInit) return(1);
		else return(0);
		}
	else return(0);
}

//***************************************************************
//transforme une duree exprim‚e en sec en J Heure Min
//***************************************************************
void TransformeSecEnJHMin(long NbSec, int tab[3])
{
	long LaDuree  = NbSec;
	long nbjour = LaDuree/86400;
	LaDuree -= nbjour*86400;
	long nbheure = LaDuree/3600;
	LaDuree -= nbheure*3600;
	long nbmin = LaDuree/60;
	LaDuree -= nbmin*60;
	if (nbjour < 0) nbjour = 0;
	if (nbheure < 0) nbheure = 0;
	if (nbmin < 0) nbmin = 0;
	tab[0] = (int) nbjour;
	tab[1] = (int) nbheure;
	tab[2] = (int) nbmin;
}

int keytest(void)
{
	char c;
	int toreturn;

	if (kbhit())
		{
		c = getch();
		if (c == 'q' || c == 'Q')
			toreturn = 1;
		else
			toreturn = 0;
		}
	return toreturn;
}

void LireSim(void)
{
	float Prod, Acc;
	int Boucl;
	double TimeEvt, TimeSP;

	int erreur = DBopen(DB_Simul_Item,2,Path);     // Ouverture en lecture

	if (!erreur)
		{
		erreur *= LireDB(DB_Simul_Item,0,DB_Simul_Prod,(char *) &Prod, sizeof(float));
		erreur *= LireDB(DB_Simul_Item,0,DB_Simul_Acc,(char *) &Acc, sizeof(float));
		erreur *= LireDB(DB_Simul_Item,0,DB_Simul_Boucl,(char *) &Boucl, sizeof(int));
		erreur *= LireDB(DB_Simul_Item,0,DB_Simul_TimeEvt,(char *) &TimeEvt, sizeof(double));
		erreur *= LireDB(DB_Simul_Item,0,DB_Simul_TimeSP,(char *) &TimeSP, sizeof(double));
		DBclose(DB_Simul_Item) ; //fermeture
		}

	printf("\nSimulation configuration:\n\n");
	printf("Production rate: %2.2f\n", Prod);
	printf("Acceleration: %2.2f\n", Acc);
	printf("SP->PV Loop: %d\n", Boucl);
	printf("TimeEvt: %2.2f\n", TimeEvt);
	printf("TimeSP: %2.2f\n\n", TimeSP);
}

void LireProdRef()
{
	int status = 0; // status d'ouverture
	int erreur = 1; // si pb de lecture  erreur = 0
	char *DB_path = Path; // chemin de direction o— se trouve la database
	float RefProdAbsc[NBZONE];

	// Ouverture de la base de donn‚es en lecture/‚criture
	status = DBopen(DB_CtrlFour_Item,0,DB_path);
	if (!status)
		{
		// Lecture des abscisses
		erreur=LireDB(DB_CtrlFour_Item,0,DB_CFour_PRefAbsc,(char*)RefProdAbsc,
				 sizeof(RefProdAbsc));

		// Fermeture de la database
		DBclose(DB_CtrlFour_Item);
		if (!erreur)
			{
			printf("Cannot read CTRLFOUR.DTA");
			exit(1);
			}
		}
	else
		{
		printf("Cannot open CTRLFOUR.DTA");
		exit(1);
		}

	printf("Reference Products:\n\n");

	for (int i = 0; i < NBZONE; i++)
		printf("Reference Product abscissa in zone %d: %.2fm\n", i, RefProdAbsc[i]);
}

void main(int argc, char *argv[])
{
	if (DBread_path()) printf("\n ERROR : Check the file TACT.CFG \n");
//	long oldT = time(NULL);
	if (argc>1)
		{
		if (!strcmp (argv[1], "prod"))
			{
			//while (1)
			//{
			//if (keytest()) break;
			//if (time(NULL) > oldT+ 5)
			// {
//			oldT = time(NULL);
			printf("\n\nProduct data\n\n");
			Produit();
		// }
		//}
		}
	if (!strcmp (argv[1], "curve")) LireRefCV();
	if (!strcmp (argv[1], "pt")) LireTProd();
	if (!strcmp (argv[1], "batch")) LireBatch();
	if (!strcmp (argv[1], "log")) LireLog();
	if (!strcmp (argv[1], "furn")) LireFour();
	if (!strcmp (argv[1], "plc"))
		{
		//while (1)
		//{
		  //if (keytest()) break;
		  //if (time(NULL) > oldT+ 2)
		 //{
//		oldT = time(NULL);
		LirePLC();
		 //}
	  // }
	  }
	if (!strcmp (argv[1], "cad"))
		{
		//while (1)
		//	 {
		//if (keytest()) break;
		//	if (time(NULL) > oldT+ 5)
		//	 {
//		oldT = time(NULL);
		LireCad();
	 //	 }
	  // }
		}
	if (!strcmp (argv[1], "zone"))
		{
		//while (1)
		//{
		//	  if (keytest()) break;
		//	  if (time(NULL) > oldT+ 15)
		//	 {
//			oldT = time(NULL);
			CtrlFour();
		//	 }
	  // }
		}
	if (!strcmp (argv[1], "stop"))
		{
	  // while (1)
	  // {
	  //  if (keytest()) break;
	  //	  if (time(NULL) > oldT+ 30)
	  //	 {
//			oldT = time(NULL);
			Delays();
	  //	 }
	  // }
		}
	if (!strcmp (argv[1], "sim")) LireSim();
	if (!strcmp (argv[1], "pref")) LireProdRef();
	}
else
	{
	printf("\nUse CheckDB with one of the following word:\n");
	printf("batch: Batch list.\n");
	printf("cad  : Measured production rate and pacing mode.\n");
	printf("curve: Reference curves.\n");
	printf("furn : Furnace configuration.\n");
	printf("log  : Information about multitasking runtime.\n");
	printf("plc  : PLC events.\n");
	printf("prod : Products data.\n");
	printf("pt   : Product type available.\n");
	printf("pref : Abscissa of reference products for each zone.\n");
	printf("sim  : Simulation configuration.\n");
	printf("stop : Stoppage declared.\n");
	printf("zone : Dynamic data of the zones.\n");
	}
}

void LireCad()
// Chargement des paramŠtres dynamiques
{
	int status = 0;           // status d'ouverture
	int erreur = 1;           // si pb de lecture  erreur = 0
	char *DB_path ="";        //chemin de direction o— se trouve la database
	float cad;char str[5];char car;
	// lecture du fichier RUNTIME.DTA
	status = DBopen(DB_Runtime_Item,0,DB_path);
	if (!status)
		{
		erreur *= LireDB(DB_Runtime_Item,0,DB_Runtime_MeasProd,(char *) &cad,sizeof(float));
		erreur *= LireDB(DB_Runtime_Item,0,DB_Runtime_PacingMode,(char *) &car,sizeof(char));
		DBclose(DB_Runtime_Item);                  // fermeture de la database
		}
	printf("Measured production rate: %4.2f\n", cad);
	int res = (int) car;
	if (res != 3)
		strcpy(str,"Theo");
	else
		strcpy(str,"Meas");
	printf("Pacing Mode: %s\n",str);
}

//--------------------------------------------------------------------------
void LireFour()
{
  int status = 0; //status d'ouverture
  int erreur = 1; // si pb de lecture  erreur = 0
  int auxint; float auxfloat;
  //chemin de direction où se trouve la database
  char *DB_path =Path;

  //ouverture de la base de données en lecture
  status = DBopen(DB_CfgFour_Item,0,DB_path);
  if (!status)
   {
	  // chargement des données modèle de la database
    
	 erreur *= LireDB(DB_CfgFour_Item,0,DB_Four_Length,(char *) &auxfloat, sizeof(float));
    printf("Length: %.1f\n",auxfloat);
    erreur *= LireDB(DB_CfgFour_Item,0,DB_Four_Width,(char *) &auxfloat, sizeof(float));
	 printf("Width: %.1f\n",auxfloat);
	 erreur *= LireDB(DB_CfgFour_Item,0,DB_Four_HTop,(char *) &auxfloat, sizeof(float));
	 printf("Top height: %.1f\n",auxfloat);
	 erreur *= LireDB(DB_CfgFour_Item,0,DB_Four_HBot,(char *) &auxfloat, sizeof(float));
	 printf("Bot height: %.1f\n",auxfloat);
	 erreur *= LireDB(DB_CfgFour_Item,0,DB_Four_Step1,(char *) &auxfloat, sizeof(float));
	 printf("Step1: %.1f\n",auxfloat);
	 erreur *= LireDB(DB_CfgFour_Item,0,DB_Four_ZUBeg,(char *) &auxfloat, sizeof(float));
	 printf("Beginning of the Usefull zone: %.1f\n",auxfloat);
	 erreur *= LireDB(DB_CfgFour_Item,0,DB_Four_ZUEnd,(char *) &auxfloat, sizeof(float));
	 printf("End of the Usefull zone: %.1f\n",auxfloat);

	  for (int i=0;i<NBZONE;i++)
	  {
		printf("Top Zone Number %i\n",i);
		erreur *=LireDB(DB_CfgFour_Item,0,(DB_Four_EndAbsSup+i*sizeof(float)),(char *) &auxfloat, sizeof(float));
		printf("Abs: %5.1f",auxfloat);
		erreur *=LireDB(DB_CfgFour_Item,0,(DB_Four_TcplAbsSup+i*sizeof(float)),(char *) &auxfloat, sizeof(float));
		printf("   Tcpl: %5.1f",auxfloat);
		erreur *=LireDB(DB_CfgFour_Item,0,(DB_Four_TcplDepthSup+i*sizeof(float)),(char *) &auxfloat, sizeof(float));
		printf("   Tcpl Depth: %5.1f",auxfloat);
		erreur *=LireDB(DB_CfgFour_Item,0,(DB_Four_QGMaxSup+i*sizeof(float)),(char *) &auxfloat, sizeof(float));
		printf("   QGMax: %5.1f",auxfloat);
      erreur *=LireDB(DB_CfgFour_Item,0,(DB_Four_QAMaxSup+i*sizeof(float)),(char *) &auxfloat, sizeof(float));
		printf("   QAMax: %5.1f",auxfloat);
      erreur *=LireDB(DB_CfgFour_Item,0,(DB_Four_SptMinSup+i*sizeof(int)),(char *) &auxint, sizeof(int));
      printf("   SPMin: %i",auxint);
		erreur *=LireDB(DB_CfgFour_Item,0,(DB_Four_SptMaxSup+i*sizeof(int)),(char *) &auxint, sizeof(int));
		printf("   SPMax: %i",auxint);
      erreur *=LireDB(DB_CfgFour_Item,0,(DB_Four_SptVarSup+i*sizeof(int)),(char *) &auxint, sizeof(int));
      printf("   SPVar: %i",auxint);
		erreur *=LireDB(DB_CfgFour_Item,0,(DB_Four_RespTimeSup+i*sizeof(int)),(char *) &auxint, sizeof(int));
		printf("   RespT: %i",auxint);
      printf("\n");
     }

#ifdef NBZONEI
	  for (i=0;i<NBZONEI;i++)
     {
      printf("Bottom Zone Number %i\n",i);
      erreur *=LireDB(DB_CfgFour_Item,0,(DB_Four_EndAbsInf+i*sizeof(float)),(char *) &auxfloat, sizeof(float));
      printf("Abs: %5.1f",auxfloat);
		erreur *=LireDB(DB_CfgFour_Item,0,(DB_Four_TcplAbsInf+i*sizeof(float)),(char *) &auxfloat, sizeof(float));
      printf("   Tcpl: %5.1f",auxfloat);
      erreur *=LireDB(DB_CfgFour_Item,0,(DB_Four_TcplDepthInf+i*sizeof(float)),(char *) &auxfloat, sizeof(float));
		printf("   Tcpl Depth: %5.1f",auxfloat);
      erreur *=LireDB(DB_CfgFour_Item,0,(DB_Four_LossesInf+i*sizeof(float)),(char *) &auxfloat, sizeof(float));
      printf("   Losses: %5.1f",auxfloat);
		erreur *=LireDB(DB_CfgFour_Item,0,(DB_Four_QGMaxInf+i*sizeof(float)),(char *) &auxfloat, sizeof(float));
		printf("   QGMax: %5.1f",auxfloat);
      erreur *=LireDB(DB_CfgFour_Item,0,(DB_Four_QAMaxInf+i*sizeof(float)),(char *) &auxfloat, sizeof(float));
		printf("   QAMax: %5.1f",auxfloat);
      erreur *=LireDB(DB_CfgFour_Item,0,(DB_Four_SptMinInf+i*sizeof(int)),(char *) &auxint, sizeof(int));
		printf("   SPMin: %i",auxint);
      erreur *=LireDB(DB_CfgFour_Item,0,(DB_Four_SptMaxInf+i*sizeof(int)),(char *) &auxint, sizeof(int));
      printf("   SPMax: %i",auxint);
		erreur *=LireDB(DB_CfgFour_Item,0,(DB_Four_SptVarInf+i*sizeof(int)),(char *) &auxint, sizeof(int));
		printf("   SPVar: %i",auxint);
		erreur *=LireDB(DB_CfgFour_Item,0,(DB_Four_RespTimeInf+i*sizeof(int)),(char *) &auxint, sizeof(int));
		printf("   RespT: %i",auxint);
		printf("\n");
	  }
#endif
    //fermeture de la database
	 DBclose(DB_CfgFour_Item);
	}
}

void LireTProd()
{
  int status = 0; //status d'ouverture
  int erreur = 1; // si pb de lecture  erreur = 0

  //chemin de direction où se trouve la database
  char *DB_path =Path;

  //ouverture de la base de données en lecture
  status = DBopen(DB_TypProd_Item,0,DB_path);

  //lecture du nombre d'enregistrements
 int NbRecord = DBNbRecord(DB_TypProd_Item);
 int auxint;
 char auxcar[20];
 char car;
 printf("Product type data:\n\n");
 if (!status)
	{
	 // chargement des donn‚es modŠle de la database
	 for (int i=0;i<NbRecord;i++)
	  {
		erreur *= LireDB(DB_TypProd_Item,i,DB_TProd_Num,(char *) &auxint,sizeof (int));
		printf ("Product type number: %i\n",auxint);
		erreur *= LireDB(DB_TypProd_Item,i,DB_TProd_Quality,(char *) auxcar,20*sizeof(char));
		printf ("Quality: %s\n",auxcar);
		erreur *= LireDB(DB_TypProd_Item,i,DB_TProd_Width,(char *) &auxint, sizeof (int));
		printf("Width: %i\n",auxint);
		erreur *= LireDB(DB_TypProd_Item,i,DB_TProd_Thick,(char *) &auxint, sizeof (int));
		printf("Thickness: %i\n",auxint);
		erreur *= LireDB(DB_TypProd_Item,i,DB_TProd_ReqDisT,(char *) &auxint, sizeof (int));
		printf("Aimed Discharge temperature: %i\n",auxint);
		erreur *= LireDB(DB_TypProd_Item,i,DB_TProd_Code,(char *) &car, sizeof(char));
		printf("Steel Code: %i\n\n\n", car);
	  }
	 //fermeture de la database
    DBclose(DB_TypProd_Item);
  }
}

void LireRefCV() 	       // Transfert BdD => structure
{
	int status = 0, erreur = 1;
	char *DB_path = Path;
	int auxint; float auxfloat; int num_ligne=0;
	status = DBopen(DB_LstObj_Item,0,DB_path); 	     // Ouverture en lecture
	int NbRecord = DBNbRecord(DB_LstObj_Item);
	if (!status) 		     // Chargement des donn‚es modŠle de la database
		{
		for (int i=0;i<NbRecord;i++)
			{
			if (num_ligne==0)
				{
				printf("Reference curves\n\n");
				num_ligne++;
				}
			erreur*=LireDB(DB_LstObj_Item,i,DB_LObj_Prod,
					(char *)&auxfloat, sizeof(float));
			printf("Production rate: %3.1f\n",auxfloat);
			erreur*=LireDB(DB_LstObj_Item,i,DB_LObj_PType,
				(char*) &auxint, sizeof(int));
			printf("Product type: %i\n",auxint);
			erreur*=LireDB(DB_LstObj_Item,i,DB_LObj_ChT,
				(char*) &auxint, sizeof(int));
			printf("Charging Temp: %i\n",auxint);

			for (int j=0;j<NBZONE;j++)
				{
				erreur*=LireDB(DB_LstObj_Item,i,(DB_LObj_Target+j*sizeof(int)),
					(char *) &auxint, sizeof(int));
				printf("Z%i T= %i  ",j,auxint);
				}
			printf("\n\n");

			num_ligne += 5;
			if (num_ligne==21)
				{
				printf("\n\n");
				delay(3000);
				num_ligne=0;
				}
			}
		DBclose(DB_LstObj_Item);
		}
}
//--------------------------------------------------------------------------
void LirePLC()
{ int erreur = 1, status = DBopen(DB_PLCEvent_Item,2,"");
  char Event[6];
  int cht = 0;
  erreur *= LireDB(DB_PLCEvent_Item,0,DB_PLCEvent_Charge,Event,
			sizeof(Event));
  erreur *= LireDB(DB_PLCEvent_Item,0,DB_PLCEvent_ChT,(char*) &cht,
			sizeof(int));
  DBclose(DB_PLCEvent_Item);
  printf("PLC events: \n");
  printf("Charge: %i\n",Event[0]);
  printf("FwdStep: %i\n",Event[1]);
  printf("BckStep: %i\n",Event[2]);
  printf("Dischout: %i\n",Event[3]);
  printf("Dischin: %i\n",Event[4]);
  printf("ReCharge: %i\n\n",Event[5]);
  printf("Charging Temp: %iøC\n\n",cht);
}
//--------------------------------------------------------------------------
void LireBatch()
{
 int erreur = 1;
 int status = 0;
 char auxchar,name[11],castname[11],steelname[11];
 int width,thick,length;
 int steelcode;
 float cad_theo = 0;
 int nb_prod = 0,nb_prod_def=0;
 int nb_batch = 0;
 int batch_id = 0;
 int ACK = 0;
 int tdef = 0;
 int PONum, JobID=0;
 status = DBopen(DB_LstBatch_Item,2,"");
 nb_batch = DBNbRecord(DB_LstBatch_Item);
 if (nb_batch > 0)
  {
	 printf("\n N  N.Id  BatchName   CastName  SteelName Wdh Thk Leng CadTh S Nb Dis ACK DisT\n");
	 for (int k=0; k<nb_batch ; k++)
		{
		  erreur *= LireDB(DB_LstBatch_Item,k,DB_Batch_Id,(char *)&batch_id,
								 sizeof(int) );
		  erreur *= LireDB(DB_LstBatch_Item,k,DB_Batch_Name,name,
								 11*sizeof(char));

		  erreur *= LireDB(DB_LstBatch_Item,k,DB_Batch_Coulee,castname,
								 11*sizeof(char));

		  erreur *= LireDB(DB_LstBatch_Item,k,DB_Batch_Acier,steelname,
								 11*sizeof(char));
		  erreur *= LireDB(DB_LstBatch_Item,k,DB_Batch_Width,(char*)&width,
								 sizeof(int));
		  erreur *= LireDB(DB_LstBatch_Item,k,DB_Batch_Thick,(char *)&thick,
								 sizeof(int));
		  erreur *= LireDB(DB_LstBatch_Item,k,DB_Batch_Length,(char *)&length,
								 sizeof(int));
        erreur *= LireDB(DB_LstBatch_Item,k,DB_Batch_ThProd,(char *)&cad_theo,
								 sizeof(float));
		  erreur *= LireDB(DB_LstBatch_Item,k,DB_Batch_NbBill,&auxchar,
								 sizeof(char));
		  nb_prod = (int)auxchar ;

		  erreur *= LireDB(DB_LstBatch_Item,k,DB_Batch_NbBillDef,&auxchar,
								 sizeof(char));
		  nb_prod_def = (int)auxchar;

		  erreur *= LireDB(DB_LstBatch_Item,k,DB_Batch_Code,&auxchar,
								 sizeof(char));
		  steelcode = (int)auxchar;

		  erreur *= LireDB(DB_LstBatch_Item,k,DB_Batch_TDef,(char*)&tdef,
								 sizeof(int));

		  LireDB(DB_LstBatch_Item, 0, DB_Batch_PONumber ,(char*)&PONum, sizeof(int));
//		  LireDB(DB_LstBatch_Item, 0, DB_Batch_JobID ,(char*)&JobID, sizeof(int));
		  // Affichage … l'‚cran
		  printf ("%02i %05i %10s %10s %10s %03i %03i %04i %5.1f %1i %02i %02i  XX %4i %05i %05i\n",
					 k,batch_id,name,castname,steelname,width,thick,length,
					 cad_theo,steelcode,nb_prod,nb_prod_def,tdef,PONum,JobID);
  }//for
  }//nb_batch
  DBclose(DB_LstBatch_Item);
  // lire les donnees du prochain lot enfourne
  status = DBopen(DB_ChBatch_Item,2,"");
  if (DBNbRecord(DB_ChBatch_Item) > 0)
	 {
	  printf("    Charging batch data : \n");

		  erreur *= LireDB(DB_ChBatch_Item,0,DB_ChBatch_BName,name,
								 10*sizeof(char));

		  erreur *= LireDB(DB_ChBatch_Item,0,DB_ChBatch_CastName,castname,
								 10*sizeof(char));

		  erreur *= LireDB(DB_ChBatch_Item,0,DB_ChBatch_SteelName,steelname,
								 10*sizeof(char));
		  erreur *= LireDB(DB_ChBatch_Item,0,DB_ChBatch_Width,(char*)&width,
								 sizeof(int));
		  erreur *= LireDB(DB_ChBatch_Item,0,DB_ChBatch_Thick,(char *)&thick,
								 sizeof(int));
		  erreur *= LireDB(DB_ChBatch_Item,0,DB_ChBatch_Length,(char *)&length,
								 sizeof(int));
		  erreur *= LireDB(DB_ChBatch_Item,0,DB_ChBatch_ThProd,(char *)&cad_theo,
								 sizeof(float));
		  erreur *= LireDB(DB_ChBatch_Item,0,DB_ChBatch_ProdNb,(char *)&nb_prod,
								 sizeof(int));

		  erreur *= LireDB(DB_ChBatch_Item,0,DB_ChBatch_SteelCode,&auxchar,
								 sizeof(char));
		  steelcode = (int)auxchar;

		  erreur *= LireDB(DB_ChBatch_Item,0,DB_ChBatch_ACK,&auxchar,
								 sizeof(char));
		  ACK = (int)auxchar;

		  erreur *= LireDB(DB_ChBatch_Item,0,DB_ChBatch_TDef,(char*)&tdef,
								 sizeof(int));

		  LireDB(DB_ChBatch_Item, 0, DB_ChBatch_PONumber ,(char*)&PONum, sizeof(int));

		  LireDB(DB_ChBatch_Item, 0, DB_ChBatch_JobID ,(char*)&JobID, sizeof(int));

		  // Affichage a lecran
		  name[10] = '\0';
		  castname[10] = '\0';
		  steelname[10] = '\0';
		  printf ("%10s %10s %10s %03i %03i %04i %5.1f %1i %02i XX %2i %4i %05i %05i\n",
					 name,castname,steelname,width,thick,length,
					 cad_theo,steelcode,nb_prod,ACK,tdef, PONum, JobID);
	 }
  DBclose(DB_ChBatch_Item);
}
//////////////////////////////////////////////////////////////////////////////
//             LECTURE DU FICHIER LOG STATUS.DTA
//////////////////////////////////////////////////////////////////////////////
void LireLog()
{
  int status=0;
  int bouclage=0; //liste bouclee
  int no_msg=0;   //pas de message dans la liste
  int last_msg=0; //N° du message le plus recent 1..MaxLog
  int auxint=0;
  int i,k;
  typedef char tab1[6];
  typedef char tab2[30];
  tab1 *date_msg;
  tab2 *text_msg;

  // allocation memoire
  date_msg = NULL;
  text_msg = NULL;
  date_msg = new char [MaxLog][6];
  text_msg = new char [MaxLog][30];

  //ouverture en lect.
  status = DBopen(DB_Status_Item,0,"");

  //lect du dernier enr.
  status = LireDB(DB_Status_Item,0,DB_Status_Last,(char*)&last_msg,
                  sizeof(int));
  
  if (last_msg > 0) no_msg=0;
  else              no_msg=1;

  // teste si la liste est bouclee
  if (!no_msg && (last_msg < MaxLog))
	{
     status = LireDB(DB_Status_Item,last_msg,DB_Status_Last,(char*)&auxint,
                     sizeof(int));
     if (auxint) bouclage = 1;
	  else        bouclage = 0;
   }

  // Lecture du fichier log du plus ancien au plus recent
  k=0;
  if (!no_msg && bouclage) 
    {
      for (i=last_msg;i<MaxLog;i++)
        {
          status = LireDB(DB_Status_Item,i,DB_Status_Date,
                          date_msg[k],6*sizeof(char));
          status = LireDB(DB_Status_Item,i,DB_Status_Msg,
                          text_msg[k],30*sizeof(char));
          k++;
        }//for i 
    }//bouclage
  if (!no_msg)
    {
      for (i=0;i<last_msg;i++)
        {
			 status = LireDB(DB_Status_Item,i,DB_Status_Date,
                          date_msg[k],6*sizeof(char));
          status = LireDB(DB_Status_Item,i,DB_Status_Msg,
								  text_msg[k],30*sizeof(char));
			 k++;
        }//for i        
    }//if no_msg

  // fermeture du fichier
  DBclose(DB_Status_Item);

  // affichage des messages a l'ecran
  if (no_msg)
	 printf("\n NO MESSAGE IN LOG FILE \n");
  else
	{
		int num_ligne=0;
		for (i=0;i<k;i++)
		 {
			if (num_ligne==0)
			  {
					printf("\n Date						Message");
					num_ligne++;
			  }
			printf("\n %02i/%02i/%02i %02i:%02i:%02i          %30s",
					  (int)date_msg[i][0],(int)date_msg[i][1],(int)date_msg[i][2],
					  (int)date_msg[i][3],(int)date_msg[i][4],(int)date_msg[i][5],
					  text_msg[i]);
			num_ligne++;
			if (num_ligne==25)
			  {
		 delay(3000);
		 num_ligne=0;
			  }
		 }//for

	}//messages

  if (date_msg != NULL) delete date_msg;
  if (text_msg != NULL) delete text_msg;

}

