/////////////////////////////////////////////////////////////////////////////
//
//                               PLCTASK.CPP
//
//                              Version D.O.S
//       Option : -r = remise … jour des compteurs Oldtime et Oldtime2
//                     a r‚aliser au d‚marrage.
//
/////////////////////////////////////////////////////////////////////////////
#define __OWNER_PLCTASK_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dos.h>
#include <conio.h>
#include <string.h>
#include "database.h"
#include "delay.h"
#include "wrstatus.h"
#include <io.h>
#include <fcntl.h>
//#include "appelhs.h"
//-------------------------------------------------------------------------
int run_plctask(char *argv)
{

	int init_compteurs = 0;
	// controle des arguments

	if (!strcmp(argv,"-r"))
		init_compteurs = 1;

	if (DBread_path())
	{
		printf("\n ERROR: Verifie the file TACT.CFG \n");
		return(EXIT_FAILURE);
	}

	WriteStatus("PLC: Start");
	double  TheTime = time(NULL);
	double  OldTime;
	double  OldTime1;
	float SimulAcc;
	int ArretAvance = 0;
	int SimulPLC = 0;
	float SimulPeriod;
	int SPPeriod=0;
	int SP = 0;
	float auxfloat;
	int loop =0;
	int erreur = 1; // si pb de lecture  erreur = 0
	//chemin de direction o— se trouve la database
	char *DB_path = "";
	int status = 0;

	if (!init_compteurs)
	{
		status = DBopen(DB_Simul_Item,0,DB_path);

		if (!status)
			{
			erreur *= LireDB(DB_Simul_Item,0,DB_Simul_Acc,
					  (char *) &SimulAcc, sizeof(float));
			erreur *= LireDB(DB_Simul_Item,0,DB_Simul_Boucl,
					  (char *) &loop, sizeof(int));
			erreur *= LireDB(DB_Simul_Item,0,DB_Simul_Prod,
					  (char *) &auxfloat, sizeof(float));
			erreur *= LireDB(DB_Simul_Item,0,DB_Simul_TimeEvt,
					  (char *) &OldTime, sizeof(double));
			erreur *= LireDB(DB_Simul_Item,0,DB_Simul_TimeSP,
					  (char *) &OldTime1, sizeof(double));
			DBclose(DB_Simul_Item);
			}

		//lecture periode de calc SP
		status = DBopen(DB_CfgModel_Item,0,DB_path);
		if (!status)
			erreur *= LireDB(DB_CfgModel_Item,0,DB_Model_MeasPer,
					  (char *) &SPPeriod, sizeof(int));

		DBclose(DB_CfgModel_Item);

		TheTime = time(NULL);
		SimulPeriod = 3600.0/auxfloat;
		//affectation des evenements PLC sur condition Timer *******
		if (TheTime >= OldTime + SimulAcc*SimulPeriod)
		{
			SimulPLC=1;
			OldTime += SimulAcc*SimulPeriod;
//			printf("PLC:Charge + Avance\n");
		}

		if (TheTime >= OldTime1 + SimulAcc*SPPeriod)
		{
			SP=1;
			OldTime1 += SimulAcc*SPPeriod;
		}

		//bouclage conditionnel
		if (loop && SP)
		{
			StoreSPasPV(SPPeriod);
			SP=0;
		}
		//lecture des arrets pour stopper les avances
		int NotErreur = loadDelay();
		int EnCourt= 0;
		if (NotErreur && Duree > 0)
			//detection si l'arrˆt est en court
			EnCourt = detecte(Debut);

		if (SimulPLC == 1 && !EnCourt)
		{
			char Event[6];
			for (int i=0; i < 6; i++) Event[i]=0;
			if (!ArretAvance)
			{
				Event[0]=1;
				Event[1]=1;       	   // Initialisation pour StorePLC()
			}
			StorePLC((char *)Event); // Charge et avance (cf database.cpp)
			SimulPLC = 0;
			int f;
			f = open("plc.txt", O_APPEND | O_TEXT);
			write(f, &OldTime, sizeof(double));
			close(f);
		}
	}// if !init_compteurs
	else
		// initialisation des compteurs
		OldTime = OldTime1 = time(NULL);

		// sauvegarde les dates OldTime et OldTime1 dans la base de donn‚es
		status = DBopen(DB_Simul_Item, 1, DB_path);

		if (!status)
		{
			erreur *= DBwrite(DB_Simul_Item, 0, DB_Simul_TimeEvt,
						&OldTime, sizeof(double));
			erreur *= DBwrite(DB_Simul_Item, 0, DB_Simul_TimeSP,
						&OldTime1, sizeof(double));
		}
		DBclose(DB_Simul_Item);

		WriteStatus("PLC: Complete");
		return(1);
}
