#define __OWNER_MAIN_H
#include <process.h>
#include <stdlib.h>
#include <time.h>
//#include <alloc.h>

#ifndef __LANGUAGE_H
#include "language.h"
#endif
//#ifndef __DATABASE_H
//#include "database.h"
//#endif
#ifndef __CONFIG_H
#include "config.h"
#endif
//#ifndef __WINSTRUC_H
//#include "winstruc.h"
//#endif
//#ifndef __WRSTATUS_H
//#include "wrstatus.h"
//#endif
//#include "..\mttrack\mttrack.h"
//#ifndef __IHM_H
//#include "ihm.h"
//#endif
//#ifndef __REH_DATA_H
//#include "Reh_data.h"
//#endif
/*
#ifndef __Furn_DATA1_H
#include "furndata.h"
#endif
*/
//#ifndef __GLOBVIEW_H
//#include "globview.h"
//#endif
//#ifndef __GLOBDAT_H
//#include "globdat.h"
//#endif

#define SEE_MMI
//#define ONLY_MMI
//define DBG_VUE

#define NB_PROCESS		10
#define TACHE_CALC		0
#define TACHE_TRACK		1
#define TACHE_SP			2
#define TACHE_PLC			3
#define TACHE_COM			4
#define TACHE_DELAY		5
#define TACHE_HISP		6
#define TACHE_HISZ		7
#define TACHE_HISD		8
#define TACHE_IHM			9

int RunSpt    = 0;
float CalcPer = 30.0;
int NbBatch   = 0;		  				// Nb of defined batches

//ProdTypeList *TheTypeList = NULL;
//Metal *MetalTab[22];		// Array of metal pointer (max 22 metal code)
//
//Furnace *TheFurn = NULL;
//Batch *BatchTab[MaxBatchNb];
//EndzTargetList *TheTargetList = NULL;

void Affiche (char *val);
int run_plctask(char *argv);
int run_calctemp(float periode);
int run_sp();
int run_delay();
int run_defhis();
int run_makezhis();
int run_makephis(int event);
int NetworkExchange();

// nom des executables

char *nom_exec [NB_PROCESS] =
	{	"calctemp",
		"mttrack",
		"sp",
		"plc",
		"checkdb",
		"delay",
		"mtprdhis",
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
int mode_accelere  = 0; 		// 1 -> Acceleration ( option -s )
									// 0 -> Pas d'acceleration
int hisp_enf = 0; //cree un nouveau fichier historique produit
int hisp_def = 0; //archive le produit defourne.

time_t t_actu; //temps courant
time_t t_last[NB_PROCESS]; // date du dernier calcul.
int erreur=0;
float delta_t = 0.0;
int periode[NB_PROCESS],
	 periode_acc[NB_PROCESS],
	 flag[NB_PROCESS],
	 Enable[NB_PROCESS];
////////////////////////////////////////////////////////////////////////////
void Affiche (char chaine[80])
{
FILE *toto=fopen("toto.txt","a+");
fprintf(toto,chaine);
fclose(toto);
}
////////////////////////////////////////////////////////////////////////////
//             FONCTION DE RECOUVREMENT DU PROCESS PERE
////////////////////////////////////////////////////////////////////////////
/*
int exec_process(int num_proc,char *arg_1)
{
	int retour = 0;
	strcpy(liste_arg[num_proc][1],arg_1);
	retour = spawnv(P_WAIT,liste_arg[num_proc][0],liste_arg[num_proc]);
	return(retour);
}
*/
void main(int argc, char *argv[])
{
   char buf[80];
//	sprintf(buf,"Rentr‚e dans programme\n");
//	Affiche(buf);
	struct date d;
	struct time t;
	/*char Bibli_Path[MAXPATH];
	if ( DBread_path() )
		{
		printf("\n ERROR in SCHEDULER : Check the file TACT.CFG \n");
		exit (-1);
		}
	strcpy(NomApplication,"yazd1");
	sprintf(Bibli_Path,"%sico_ecr.bib", DB_Path_cfg);
#ifdef SEE_MMI
	APPELHS ("MODE,18");
	APPELHS ("BEEP,NON");
#endif*/
//	Valid=0;
//#ifdef SEE_MMI
//	APPELHS ("Bibli,Disque,%s",Bibli_Path);
//#endif
	*init = INITIAL;
	vue   = MAIN_MENU;
#ifdef DBG_VUE
	fstream f;
	f.open("vue.txt", ios::out);
	f << vue << endl;
	f.close();
#endif
	// contr“le des arguments
	if (argc > 1)
		{
		if ( !strcmp(argv[1],"-s") ) mode_accelere = 1;
		}

	// R.A.Z des donn‚es
	t_last[0] = time(NULL);
	for (int i=0;i<NB_PROCESS;i++)
		{
		flag[i]       = 0;
		Enable[i]     = 0;
		periode[i]    = 0;
		periode_acc[i]= 0;
		t_last[i]     = t_last[0];
		strcpy(liste_arg[i][0],DB_Path_exe);
		strcat(liste_arg[i][0],nom_exec[i]);
		}

#ifdef ONLY_MMI
	// initialisation des donn‚es
	Enable[TACHE_CALC]  = 0;
	Enable[TACHE_TRACK] = 0;
	Enable[TACHE_PLC]   = 0;
	Enable[TACHE_DELAY] = 0;
	Enable[TACHE_HISD]  = 0;
	Enable[TACHE_IHM]   = 0;
	Enable[TACHE_COM]   = 0;
#else
	// initialisation des donn‚es
	Enable[TACHE_CALC]  = 1;
	Enable[TACHE_TRACK] = 1;
	Enable[TACHE_PLC]   = 1;
	Enable[TACHE_DELAY] = 1;
	Enable[TACHE_HISD]  = 1;
	Enable[TACHE_IHM]   = 1;
	Enable[TACHE_COM]   = 1;
#endif

	periode[TACHE_PLC]   = 5;
	periode[TACHE_DELAY] = 15;
	periode[TACHE_TRACK] = 5;
	periode[TACHE_IHM]   = 10;

	// Lecture des periodes initiales dans CFGMODEL.CFG
/*	int auxint=5;
	DBopen(DB_CfgModel_Item,1,Path);
	erreur*=DBwrite(DB_CfgModel_Item,0,DB_Model_MeasPer,(char *)&auxint,
					  sizeof(int));
	DBclose(DB_CfgModel_Item);
*/	DBdansModel();
	periode[TACHE_CALC] = atoi(Model.CalcPer);
	periode[TACHE_COM]  = atoi(Model.MeasPer);
	periode[TACHE_SP]   = atoi(Model.SPCalcPer); //120
	periode[TACHE_HISP] = atoi(Model.PHisPer);
	periode[TACHE_HISZ] = atoi(Model.ZHisPer);

#ifdef ONLY_MMI
	Enable[TACHE_SP]	  = 0;
	Enable[TACHE_HISZ]  = 0;
	Enable[TACHE_HISP]  = 0;
#else
	Enable[TACHE_SP]	  = Model.SPEnable;
	Enable[TACHE_HISZ]  = Model.ZHisEnable;
	Enable[TACHE_HISP]  = Model.PHisEnable;
#endif

	if (!mode_accelere)
		Enable[TACHE_PLC] = 0;

	// Lire l'acc‚leration dans SIMUL.DTA

	if (mode_accelere)
		{
		erreur = DBopen(DB_Simul_Item,0,"");
		if (!erreur)
			{
			erreur = LireDB(DB_Simul_Item,0,DB_Simul_Acc,
				(char *)&Acceleration,sizeof(float));

			DBclose(DB_Simul_Item);
			if (!erreur) erreur = -1;
			else erreur = 0;
			}

		if (erreur)
			{
			printf("\n ERROR in SCHEDULER : Cannot read SIMUL.DTA \n");
			exit (-1);
			}
		}

	// initialisation des compteurs de la tache plc
	if (Enable[TACHE_PLC])
		{
		erreur = run_plctask("-r");

		if (erreur == -1)
			WriteStatus("SCHED:Error spawn PLC");
		}

	do
		{
#ifdef SEE_MMI
		APPELHS("Ecran,Touche");
#ifdef DBG_VUE
		fstream f;
/*		f.open("vue.txt", ios::in);
		f >> vue;
		f.close();
*/
		f.open("vue.txt", ios::out|ios::app);
		f << vue;
		f.close();
#endif
		switch (vue)
			{
			case MAIN_MENU:			Main_Menu();			break;
			case CONFIG_PASSWORD:	Config_Password();  	break;
			case CONFIG_MENU:			Config_Menu();	 		break;
			case PROD_HIS:				Prod_His();				break;
			case FURNACE_HIS:			Furnace_His();      	break;
			case ZHIS_CONFIG:			ZHis_Config();			break;
			case DEF_HIS:				Def_His();   	   	break;
			case FURNACE_CONFIG: 	Furnace_Config(); 	break;
			case MODEL_CONFIG: 		Model_Config(); 		break;
			case PRODUCTS_CONFIG:	Products_Config();	break;
			case HEAT_CONFIG: 		Heat_Config();	 		break;
			case PROD_SEL:				Products_Select();	break;
			case REHEAT_CONTROL:    Reheat_Control();		break;
			case PACING:				Pacing_Config();		break;
			case HEAT_SELECT:			Heat_Select();			break;
			case DELAY:					Delay();             break;

//			Fenˆtres de confirmation

			case FURNACE_CONFIRM:	Confirm();	 			break;
			case MODEL_CONFIRM:     Confirm(); 				break;
			case PRODUCT_CONFIRM:	Confirm(); 				break;
			case HEAT_CONFIRM:		Confirm(); 				break;
			case ERROR_WINDOW:		Window_Error();		break;
			case DELAY_CONFIRM:		Confirm_OverWrite();	break;
			case PACING_CONFIRM:		Pacing_Confirm();		break;

//			Ecrans d'aide
			case HELP_SCREEN:			Help_Screen();			break;

			default: break;
			}
#endif

		// calcul des periodes r‚elles ( si acc‚leration )
		if (mode_accelere)
			{
			// Lecture de l'acc‚leration
			periode_acc[TACHE_CALC]    = ((float)periode[TACHE_CALC]  * Acceleration);
			periode_acc[TACHE_SP]      = ((float)periode[TACHE_SP]    * Acceleration);
			periode_acc[TACHE_TRACK]   = ((float)periode[TACHE_TRACK] * Acceleration);
			periode_acc[TACHE_PLC]     = periode[TACHE_PLC];
			periode_acc[TACHE_COM]		= periode[TACHE_COM];
			periode_acc[TACHE_DELAY]   = periode[TACHE_DELAY];
			periode_acc[TACHE_HISZ]    = ((float)periode[TACHE_HISZ]  * Acceleration);
			periode_acc[TACHE_HISP]    = ((float)periode[TACHE_HISP]  * Acceleration);
			periode_acc[TACHE_IHM]     = periode[TACHE_IHM];

			if (periode_acc[TACHE_CALC]  == 0) periode_acc[TACHE_CALC]  = 1;
			if (periode_acc[TACHE_SP]    == 0) periode_acc[TACHE_SP]    = 1;
			if (periode_acc[TACHE_TRACK] == 0) periode_acc[TACHE_TRACK] = 1;
			if (periode_acc[TACHE_PLC]   == 0) periode_acc[TACHE_PLC]   = 1;
			if (periode_acc[TACHE_HISZ]  == 0) periode_acc[TACHE_HISZ]  = 1;
			if (periode_acc[TACHE_HISP]  == 0) periode_acc[TACHE_HISP]  = 1;
			}

		t_actu = time (NULL);

		if (!mode_accelere)
			{
			if (Enable[TACHE_CALC] && (t_actu >= t_last[TACHE_CALC]  + periode[TACHE_CALC]))    flag[TACHE_CALC]  = 1;
			if (Enable[TACHE_TRACK] && (t_actu >= t_last[TACHE_TRACK] + periode[TACHE_TRACK]))  flag[TACHE_TRACK] = 1;
			if (Enable[TACHE_SP] && (t_actu >= t_last[TACHE_SP]    + periode[TACHE_SP])) 		  flag[TACHE_SP]    = 1;
			if (Enable[TACHE_PLC] && (t_actu >= t_last[TACHE_PLC]   + periode[TACHE_PLC])) 	  flag[TACHE_PLC]   = 1;
			if (Enable[TACHE_COM] && (t_actu >= t_last[TACHE_COM]  + periode[TACHE_COM])) flag[TACHE_COM]   = 1;
			if (Enable[TACHE_DELAY] && (t_actu >= t_last[TACHE_DELAY]  + periode[TACHE_DELAY])) flag[TACHE_DELAY]   = 1;
			if (Enable[TACHE_HISP] && (t_actu >= t_last[TACHE_HISP]  + periode[TACHE_HISP])) flag[TACHE_HISP]   = 1;
			if (Enable[TACHE_HISZ] && (t_actu >= t_last[TACHE_HISZ]  + periode[TACHE_HISZ])) flag[TACHE_HISZ]   = 1;
			if (Enable[TACHE_IHM] && (t_actu >= t_last[TACHE_IHM]  + periode[TACHE_IHM])) flag[TACHE_IHM]   = 1;
			}
		else
			{
			sprintf(buf,"Enable[Tache_SP] : %d\n",Enable[TACHE_SP]);
//         Affiche(buf);
			sprintf(buf,"t_actu : %d\n",t_actu);
//         Affiche(buf);
			sprintf(buf,"t_last[TACHE_SP] : %d\n",t_last[TACHE_SP]);
//         Affiche(buf);
			sprintf(buf,"periode_acc : %d\n",periode_acc);
//         Affiche(buf);
			if (Enable[TACHE_CALC] && (t_actu >= t_last[TACHE_CALC]  + periode_acc[TACHE_CALC]))   flag[TACHE_CALC]  = 1;
			if (Enable[TACHE_TRACK] && (t_actu >= t_last[TACHE_TRACK] + periode_acc[TACHE_TRACK])) flag[TACHE_TRACK] = 1;
			if (Enable[TACHE_SP] && (t_actu >= t_last[TACHE_SP]    + periode_acc[TACHE_SP]))
				{
				 flag[TACHE_SP]    = 1;
				}
			if (Enable[TACHE_PLC] && (t_actu >= t_last[TACHE_PLC]   + periode_acc[TACHE_PLC]))     flag[TACHE_PLC]   = 1;
			if (Enable[TACHE_COM] && (t_actu >= t_last[TACHE_COM]  + periode_acc[TACHE_COM])) flag[TACHE_COM]   = 1;
			if (Enable[TACHE_DELAY] && (t_actu >= t_last[TACHE_DELAY]  + periode_acc[TACHE_DELAY])) flag[TACHE_DELAY]   = 1;
			if (Enable[TACHE_HISZ] && (t_actu >= t_last[TACHE_HISZ]  + periode_acc[TACHE_HISZ])) flag[TACHE_HISZ]   = 1;
			if (Enable[TACHE_HISP] && (t_actu >= t_last[TACHE_HISP]  + periode_acc[TACHE_HISP])) flag[TACHE_HISP]   = 1;
			if (Enable[TACHE_IHM] && (t_actu >= t_last[TACHE_IHM]  + periode_acc[TACHE_IHM])) flag[TACHE_IHM]   = 1;
			}

		// d‚clanche un appel a la COM
		if (flag[TACHE_COM])
			{
			t_last[TACHE_COM]  = t_actu;
//			erreur=exec_process(TACHE_CHECKDB,"prod");
			erreur=NetworkExchange();
			if (erreur == -1) WriteStatus("SCHED:Error spawn COM");
			//RAZ du flag
			flag[TACHE_COM] = 0;
			}

		// d‚clanche un tracking
		if (flag[TACHE_TRACK])
			{
			t_last[TACHE_TRACK]  = t_actu;
		 //	erreur=exec_process(TACHE_TRACK,"");
			#ifndef SEE_MMI
			printf("start run_mttrack\n");
			#endif
			erreur=run_mttrack();
			#ifndef SEE_MMI
			printf("completed run_mttrack\n");
			#endif
			switch (erreur)
				{
				case -1 : WriteStatus("SCHED:Error spawn TRACK");
							 break;
				case  1 : // enfournement d'une nvlle billette
							if (Enable[TACHE_HISP])
								{
								flag[TACHE_HISP] = 1;
								hisp_enf  = 1;
								}
							break;

				case  2 : // d‚fournement d'une billette
							if (Enable[TACHE_HISP])
								{
								flag[TACHE_HISP] = 1;
								hisp_def  = 1;
								}

							if (Enable[TACHE_HISD]) flag[TACHE_HISD] = 1;
							break;
				case  3 : // enf + def
							if (Enable[TACHE_HISP])
								{
								flag[TACHE_HISP] = 1;
								hisp_def  = 1;
								hisp_enf  = 1;
								}

							if (Enable[TACHE_HISD])
								flag[TACHE_HISD] = 1;
							break;
				}
			//RAZ du flag
			flag[TACHE_TRACK] = 0;
			}

		// d‚clanche un appel … l'historique produit
		if (flag[TACHE_HISP])
			{
         sprintf(buf,"Lancement historique produit\n");
//         Affiche(buf);
			if (!hisp_enf && !hisp_def)
				{   // d‚clanchement periodique
				#ifndef SEE_MMI
            printf("start run_makephis(2)\n");
				#endif
				erreur=run_makephis(2);   //2
            #ifndef SEE_MMI
            printf("completed run_makephis(2)\n");
            #endif
				if (erreur == -1) WriteStatus("SCHED:Error spawn HISP");
				t_last[TACHE_HISP]  = t_actu;
				}

			if ( hisp_enf )
				{   // d‚clanchement apres enfournement
            #ifndef SEE_MMI
            printf("start run_makephis(1)\n");
            #endif
				erreur=run_makephis(1);
            #ifndef SEE_MMI
            printf("completed run_makephis(1)\n");
				#endif
				if (erreur == -1) WriteStatus("SCHED:Error spawn HISP");
				hisp_enf = 0;
				}

			if ( hisp_def )
				{   // d‚clanchement aprŠs d‚fournement
            #ifndef SEE_MMI
            printf("start run_makephis(5)\n");
            #endif
				erreur=run_makephis(5); //5
				#ifndef SEE_MMI
				printf("completed run_makephis(5)\n");
            #endif
				if (erreur == -1) WriteStatus("SCHED:Error spawn HISP");
				hisp_def = 0;
				}
			//RAZ du flag
			flag[TACHE_HISP] = 0;
			}

		// d‚clanche un historique produits d‚fourn‚s
		if (flag[TACHE_HISD])
			{
		//	erreur=exec_process(TACHE_HISD,"");
         #ifndef SEE_MMI
      	printf("start run_defhis\n");
			#endif
			erreur = run_defhis();
         #ifndef SEE_MMI
			printf("completed run_defhis\n");
			#endif
			if (erreur == -1) WriteStatus("SCHED:Error spawn HISD");
			//RAZ du flag
			flag[TACHE_HISD] = 0;
			}

		// d‚clanche un historique zones
		if (flag[TACHE_HISZ])
			{
			t_last[TACHE_HISZ]  = t_actu;
			#ifndef SEE_MMI
			printf("start run_makeshis\n");
			#endif
			erreur = run_makezhis();
			#ifndef SEE_MMI
			printf("completed run_makezhis\n");
			#endif
 //			erreur=exec_process(TACHE_HISZ,"");
			if (erreur == -1) WriteStatus("SCHED:Error spawn HISZ");
			//RAZ du flag
			flag[TACHE_HISZ] = 0;
			}

		// d‚clanche un appel a tdelay
		if (flag[TACHE_DELAY])
			{
			t_last[TACHE_DELAY]  = t_actu;
			#ifndef SEE_MMI
			printf("start run_tdelay\n");
			#endif
			erreur=run_delay();
			#ifndef SEE_MMI
			printf("completed run_tdelay\n");
			#endif
			//erreur=exec_process(TACHE_DELAY,"");
			if (erreur == -1) WriteStatus("SCHED:Error spawn TDELAY");
			//RAZ du flag
			flag[TACHE_DELAY] = 0;
			}

		// d‚clanche un calcul thermique
		if (flag[TACHE_CALC])
			{
			if (!mode_accelere)
				{
				delta_t = t_actu - t_last[TACHE_CALC];
				if (!delta_t)
					printf("t_actu: %d t_last: %d \n", t_actu, t_last[TACHE_CALC]);
				t_last[TACHE_CALC]  = t_actu;
				}
			else
				{
				delta_t = (float)(t_actu - t_last[TACHE_CALC])/Acceleration;
				t_last[TACHE_CALC]  = t_actu;
				}
				#ifndef SEE_MMI
				printf("start run_calctemp\n");
				#endif
				erreur = run_calctemp(delta_t);
				#ifndef SEE_MMI
				printf("completed run_calctemp\n");
				#endif
			if (erreur == -1) WriteStatus("SCHED:Error spawn CALC-TEMP");
			//RAZ du flag
			flag[TACHE_CALC] = 0;
			}// flag[TACHE_CALC]

		// d‚clanche un calcul de consignes
		if (flag[TACHE_SP])
			{
			t_last[TACHE_SP]  = t_actu;
         #ifndef SEE_MMI
			printf("start run_sp\n");
         #endif
//         FILE *toto=fopen("toto.txt","a+");
//         fprintf(toto,"Lancement de run_sp\n");
//         fclose(toto);
			erreur = run_sp();
         #ifndef SEE_MMI
			printf("completed run_sp\n");
         #endif

			if (erreur == -1) WriteStatus("SCHED:Error spawn SP");
			//RAZ du flag
			flag[TACHE_SP] = 0;
			}
		// d‚clanche simulation plc
		if (flag[TACHE_PLC])
			{
			t_last[TACHE_PLC]  = t_actu;
//			erreur=exec_process(TACHE_PLC,"");
			#ifndef SEE_MMI
			printf("start run_plctask\n");
			#endif
			erreur = run_plctask("");
			#ifndef SEE_MMI
			printf("completed run_plctask\n");
			#endif
			if (erreur == -1) WriteStatus("SCHED:Error spawn PLC");
			//RAZ du flag
			flag[TACHE_PLC] = 0;
			}
		}
	#ifdef SEE_MMI
	while ( vue != QUIT );
	#endif
	#ifndef SEE_MMI
	while (!kbhit());
	#endif
	#ifdef SEE_MMI
	APPELHS ("MODE,3");
	APPELHS ("Scrolling,Monte,0,0,25,80");
	#endif
}