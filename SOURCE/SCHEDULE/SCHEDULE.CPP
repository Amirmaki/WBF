////////////////////////////////////////////////////////////////////////////
//
//                        MODULE SHEDULE.CPP
//
//                    Pooling des taches TACT
//
//     Syntaxe :
//     > schedule    --> pas d'acceleration, pas de simulation PLC
//     > schedule -s --> acceleration lue dans SIMUL.DTA.
//                       simulation PLC.
//
/////////////////////////////////////////////////////////////////////////////
#define __OWNER_MAIN_H

#define NB_PROCESS		10
#define TACHE_CALC		0
#define TACHE_TRACK		1
#define TACHE_SP		2
#define TACHE_PLC		3
#define TACHE_CHECKDB		4
#define TACHE_DELAY		5
#define TACHE_HISP		6
#define TACHE_HISZ		7
#define TACHE_HISD		8
#define TACHE_IHM		9


// includes librairies std
#include <time.h>
#include <string.h>
#include <process.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <alloc.h>

// includes TACT
#include "database.h"
#include "wrstatus.h"
#include "ihm.h"
#include "V07data1.h"
#include "interf.h"
#include "globview.h"
#include "c:\hs55\prog\appelhs.h"
#include "saisie.h"
//#define _OWNER_SCHEDULE_H
//#include "globdat.h"

int RunSpt = 0;
float CalcPer = 0;
int NbBatch = 0;						// Nb of defined batches
Furnace *TheFurn = NULL;
Batch *BatchTab[MaxBatchNb];
EndzTargetList *TheTargetList = NULL;
ProdTypeList *TheTypeList = NULL;
Metal *MetalTab[22];		// Array of metal pointer (max 22 metal code)


// nom des executables
char *nom_exec [NB_PROCESS] =
	{	"calctemp",
		"mttrack",
		"sp",
		"plc",
		"checkdb",
		"tdelay",
		"hisp",
		"mtfurhis",
		"mtdefhis",
		"ihm"	};

char *liste_arg [NB_PROCESS][3] =
	{ 	{"000000000000000000000000000000","000000000000000000000000000000",NULL},
		{"000000000000000000000000000000","",NULL},
		{"000000000000000000000000000000","",NULL},
		{"000000000000000000000000000000","000000000000000000000000000000",NULL},
		{"000000000000000000000000000000","000000000000000000000000000000",NULL},
		{"000000000000000000000000000000","",NULL},
		{"000000000000000000000000000000","000000000000000000000000000000",NULL},
		{"000000000000000000000000000000","",NULL},
		{"000000000000000000000000000000","",NULL},
		{"000000000000000000000000000000","",NULL}	};


float Acceleration = 1.0;	//facteur de compression du temps
int mode_accelere = 0; 		// 1 -> Acceleration ( option -s )
									// 0 -> Pas d'acceleration
int hisp_enf = 0; //cree un nouveau fichier historique produit
int hisp_def = 0; //archive le produit defourne.

time_t t_actu; //temps courant
time_t  t_last[NB_PROCESS]; // date du dernier calcul.
int erreur=0;
float delta_t = 0;
int periode[NB_PROCESS],
	 periode_acc[NB_PROCESS],
	 flag[NB_PROCESS],
	 enable[NB_PROCESS];

////////////////////////////////////////////////////////////////////////////
//             FONCTION DE RECOUVREMENT DU PROCESS PERE
////////////////////////////////////////////////////////////////////////////
int exec_process(int num_proc,char *arg_1)
	{
	int retour = 0;
	strcpy(liste_arg[num_proc][1],arg_1);
	retour = spawnv(P_WAIT,liste_arg[num_proc][0],liste_arg[num_proc]);
	return(retour);
	}
////////////////////////////////////////////////////////////////////////////
//                          PROGRAMME PRINCIPAL
////////////////////////////////////////////////////////////////////////////
void main(int argc,char *argv[])
{

//  SaisieSimParam();
  // lecture des chemins d'accŠs aux fichiers de TACT
	if ( DBread_path() )
		{
		printf("\n ERROR in SCHEDULER : Check the file TACT.CFG \n");
		exit (-1);
		}

	// contr“le des arguments
	if (argc > 1)
		{
		if ( !strcmp(argv[1],"-s") ) mode_accelere = 1;
		}

	// R.A.Z des donn‚es
	t_last[0] = time(NULL);
	for (int i=0;i<NB_PROCESS;i++)
		{
		flag[i]=0;
		enable[i]=0;
		periode[i]=0;
		periode_acc[i]=0;
		t_last[i]=t_last[0];
		strcpy(liste_arg[i][0],DB_Path_exe);
		strcat(liste_arg[i][0],nom_exec[i]);
		}

	// initialisation des donn‚es
	enable[TACHE_CALC] = 1;
	enable[TACHE_TRACK] = 1;
	enable[TACHE_PLC] = 1;
	enable[TACHE_DELAY] = 1;
	enable[TACHE_HISD] = 1;
//	enable[TACHE_IHM] = 1;

	periode[TACHE_PLC] = 5;
	periode[TACHE_DELAY] = 15;
	periode[TACHE_TRACK] = 5;
//	periode[TACHE_IHM] = 5;

	// Lecture des periodes initiales dans CFGMODEL.CFG
	DBdansModel();
	periode[TACHE_CALC] = atoi(Model.CalcPer);
	//periode_mes  = atoi(Model.MeasPer);
	periode[TACHE_SP]   = atoi(Model.SPCalcPer);
	periode[TACHE_HISP] = atoi(Model.PHisPer);
	periode[TACHE_HISZ] = atoi(Model.ZHisPer);
	enable[TACHE_SP]	  = Model.SPEnable;
	enable[TACHE_HISZ]  = Model.ZHisEnable;
	enable[TACHE_HISP]  = Model.PHisEnable;

	if (!mode_accelere) enable[TACHE_PLC] = 0;
	// Lire l'acc‚leration dans SIMUL.DTA
	if (mode_accelere)
		{
		erreur = DBopen(DB_Simul_Item,0,"");
		if (!erreur)
			{
			 erreur = LireDB(DB_Simul_Item,0,DB_Simul_Acc,
				(char *)&Acceleration,sizeof(float));
			 if (!erreur) erreur = -1;
			 else erreur = 0;
			 }
		DBclose(DB_Simul_Item);
		if (erreur)
			{
			printf("\n ERROR in SCHEDULER : Cannot read SIMUL.DTA \n");
			exit (-1);
			}
		}

	// initialisation des compteurs de la tache plc
	if (enable[TACHE_PLC])
		{
		erreur=exec_process(TACHE_PLC,"-r");
		if (erreur == -1) WriteStatus("SCHED:Error spawn PLC");
		}

	CreateObjects();
	VGInit();
	UpdateView();

	while (VGNextViewNb != 1)
//	while (1)
		{
		// calcul des periodes r‚elles ( si acc‚leration )
		if (mode_accelere)
			{
			// Lecture de l'acc‚leration
			periode_acc[TACHE_CALC] = ((float)periode[TACHE_CALC] * Acceleration);
			periode_acc[TACHE_SP] = ((float)periode[TACHE_SP] * Acceleration);
			periode_acc[TACHE_TRACK] = ((float)periode[TACHE_TRACK] * Acceleration);
			periode_acc[TACHE_PLC] = periode[TACHE_PLC];
			periode_acc[TACHE_CHECKDB] = periode[TACHE_CHECKDB];
			periode_acc[TACHE_DELAY] = periode[TACHE_DELAY];
			periode_acc[TACHE_HISZ] = ((float)periode[TACHE_HISZ] * Acceleration);
			periode_acc[TACHE_IHM] = periode[TACHE_IHM];

			if (periode_acc[TACHE_CALC] == 0) periode_acc[TACHE_CALC] = 1;
			if (periode_acc[TACHE_SP] == 0) periode_acc[TACHE_SP] = 1;
			if (periode_acc[TACHE_TRACK] == 0) periode_acc[TACHE_TRACK] = 1;
			if (periode_acc[TACHE_PLC] == 0) periode_acc[TACHE_PLC] = 1;
			if (periode_acc[TACHE_HISZ] == 0) periode_acc[TACHE_HISZ] = 1;
			}

		t_actu = time (NULL);

		if (!mode_accelere)
			{
			if (enable[TACHE_CALC] && (t_actu >= t_last[TACHE_CALC]  + periode[TACHE_CALC]))    flag[TACHE_CALC]  = 1;
			if (enable[TACHE_TRACK] && (t_actu >= t_last[TACHE_TRACK] + periode[TACHE_TRACK]))  flag[TACHE_TRACK] = 1;
			if (enable[TACHE_SP] && (t_actu >= t_last[TACHE_SP]    + periode[TACHE_SP])) 		  flag[TACHE_SP]    = 1;
			if (enable[TACHE_PLC] && (t_actu >= t_last[TACHE_PLC]   + periode[TACHE_PLC])) 	  flag[TACHE_PLC]   = 1;
			if (enable[TACHE_CHECKDB] && (t_actu >= t_last[TACHE_CHECKDB]  + periode[TACHE_CHECKDB])) flag[TACHE_CHECKDB]   = 1;
			if (enable[TACHE_DELAY] && (t_actu >= t_last[TACHE_DELAY]  + periode[TACHE_DELAY])) flag[TACHE_DELAY]   = 1;
			if (enable[TACHE_HISZ] && (t_actu >= t_last[TACHE_HISZ]  + periode[TACHE_HISZ])) flag[TACHE_HISZ]   = 1;
			if (enable[TACHE_IHM] && (t_actu >= t_last[TACHE_IHM]  + periode[TACHE_IHM])) flag[TACHE_IHM]   = 1;

			}
		else
			{
			if (enable[TACHE_CALC] && (t_actu >= t_last[TACHE_CALC]  + periode_acc[TACHE_CALC]))   flag[TACHE_CALC]  = 1;
			if (enable[TACHE_TRACK] && (t_actu >= t_last[TACHE_TRACK] + periode_acc[TACHE_TRACK])) flag[TACHE_TRACK] = 1;
			if (enable[TACHE_SP] && (t_actu >= t_last[TACHE_SP]    + periode_acc[TACHE_SP])) 		 flag[TACHE_SP]    = 1;
			if (enable[TACHE_PLC] && (t_actu >= t_last[TACHE_PLC]   + periode_acc[TACHE_PLC]))     flag[TACHE_PLC]   = 1;
			if (enable[TACHE_CHECKDB] && (t_actu >= t_last[TACHE_CHECKDB]  + periode_acc[TACHE_CHECKDB])) flag[TACHE_CHECKDB]   = 1;
			if (enable[TACHE_DELAY] && (t_actu >= t_last[TACHE_DELAY]  + periode_acc[TACHE_DELAY])) flag[TACHE_DELAY]   = 1;
			if (enable[TACHE_HISZ] && (t_actu >= t_last[TACHE_HISZ]  + periode_acc[TACHE_HISZ])) flag[TACHE_HISZ]   = 1;
			if (enable[TACHE_IHM] && (t_actu >= t_last[TACHE_IHM]  + periode_acc[TACHE_IHM])) flag[TACHE_IHM]   = 1;
			}

		// d‚clanche un calcul thermique
		if (flag[TACHE_CALC])
			{
			if (!mode_accelere)
				{
				delta_t = t_actu - t_last[TACHE_CALC];
				t_last[TACHE_CALC]  = t_actu;
				}
			else
				{
				delta_t = (float)(t_actu - t_last[TACHE_CALC])/Acceleration;
				t_last[TACHE_CALC]  = t_actu;
				}
			char *un_texte = "000000000000000000000000000000";
			sprintf(un_texte,"%f",delta_t);
			erreur=exec_process(TACHE_CALC,un_texte);
			if (erreur == -1) WriteStatus("SCHED:Error spawn CALC-TEMP");
			//RAZ du flag
			flag[TACHE_CALC] = 0;
			}// flag[TACHE_CALC]

		// d‚clanche un tracking
		if (flag[TACHE_TRACK])
			{
			t_last[TACHE_TRACK]  = t_actu;
			erreur=exec_process(TACHE_TRACK,"");
			switch (erreur)
				{
				case -1 : WriteStatus("SCHED:Error spawn TRACK");
							 break;
				case  1 : // enfournement d'une nvlle billette
							 if (enable[TACHE_HISP])
								{
								flag[TACHE_HISP] = 1;
								hisp_enf  = 1;
								}
								break;
				case  2 : // d‚fournement d'une billette
							 if (enable[TACHE_HISP])
								{
								flag[TACHE_HISP] = 1;
								hisp_def  = 1;
								}
							 if (enable[TACHE_HISD]) flag[TACHE_HISD] = 1;
							 break;
				case  3 : // enf + def
							 if (enable[TACHE_HISP])
								{
								flag[TACHE_HISP] = 1;
								hisp_def  = 1;
								hisp_enf  = 1;
								}
							 if (enable[TACHE_HISD]) flag[TACHE_HISD] = 1;
							 break;
				}
			//RAZ du flag
			flag[TACHE_TRACK] = 0;
			}

		// d‚clanche un appel … l'historique produit
		if (flag[TACHE_HISP])
			{
			if (!hisp_enf && !hisp_def)
				{   // d‚clanchement periodique
				erreur=exec_process(TACHE_HISP,"0");
				if (erreur == -1) WriteStatus("SCHED:Error spawn HISP");
				t_last[TACHE_HISP]  = t_actu;
				}
			if ( hisp_enf )
				{   // d‚clanchement apres enfournement
				erreur=exec_process(TACHE_HISP,"1");
				if (erreur == -1) WriteStatus("SCHED:Error spawn HISP");
				hisp_enf = 0;
				}
			if ( hisp_def )
				{   // d‚clanchement aprŠs d‚fournement
				erreur=exec_process(TACHE_HISP,"2");
				if (erreur == -1) WriteStatus("SCHED:Error spawn HISP");
				hisp_def = 0;
				}
			//RAZ du flag
			flag[TACHE_HISP] = 0;
			}

		// d‚clanche un historique produits d‚fourn‚s
		if (flag[TACHE_HISD])
			{
			erreur=exec_process(TACHE_HISD,"");
			if (erreur == -1) WriteStatus("SCHED:Error spawn HISD");
			//RAZ du flag
			flag[TACHE_HISD] = 0;
			}

		// d‚clanche un historique zones
		if (flag[TACHE_HISZ])
			{
			t_last[TACHE_HISZ]  = t_actu;
			erreur=exec_process(TACHE_HISZ,"");
			if (erreur == -1) WriteStatus("SCHED:Error spawn HISZ");
			//RAZ du flag
			flag[TACHE_HISZ] = 0;
			}

		// d‚clanche un calcul de consignes
		if (flag[TACHE_SP])
			{
			t_last[TACHE_SP]  = t_actu;
			erreur=exec_process(TACHE_SP,"");
			if (erreur == -1) WriteStatus("SCHED:Error spawn SP");
			//RAZ du flag
			flag[TACHE_SP] = 0;
			}
		// d‚clanche simulation plc
		if (flag[TACHE_PLC])
			{
			t_last[TACHE_PLC]  = t_actu;
			erreur=exec_process(TACHE_PLC,"");
			if (erreur == -1) WriteStatus("SCHED:Error spawn PLC");
			//RAZ du flag
			flag[TACHE_PLC] = 0;
			}

		// d‚clanche un appel a checkdb
		if (flag[TACHE_CHECKDB])
			{
			t_last[TACHE_CHECKDB]  = t_actu;
			erreur=exec_process(TACHE_CHECKDB,"prod");
			if (erreur == -1) WriteStatus("SCHED:Error spawn CHECKDB");
			//RAZ du flag
			flag[TACHE_CHECKDB] = 0;
			}

		// d‚clanche un appel a tdelay
		if (flag[TACHE_DELAY])
			{
			t_last[TACHE_DELAY]  = t_actu;
			erreur=exec_process(TACHE_DELAY,"");
			if (erreur == -1) WriteStatus("SCHED:Error spawn TDELAY");
			//RAZ du flag
			flag[TACHE_DELAY] = 0;
			}

		// actualise les objets
		if (flag[TACHE_IHM])
			{
			t_last[TACHE_IHM]  = t_actu;
			if (TheFurn != NULL)
				DeleteObjects();

			CreateObjects();
			V07Redraw = 1;
			flag[TACHE_IHM] = 0;
			}

		ecrEnableAccess();
		UpdateView();
		// Test sortie de programme
		if (kbhit())
			{
			char carac = getch();
			if ( carac == 'q' || carac == 'Q' ) break;
			}
		}//while

	if (TheFurn != NULL)
		DeleteObjects();

	VGRestoreText();

}