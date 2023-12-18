#define __OWNER_MAIN_H
#include <stdlib.h>
#include <stdio.h>
#include "winstruc.h"
#include "database.h"
#include "db.h"
#include "simu.h"
#include "batchfct.h"
#include <time.h>

main()
{
 if (DBread_path()) printf("\n ERROR: Verify the file TACT.CFG \n");

 while(1)
 {
	printf ("\nStoppage: 1\nAcceleration: 2\nProduction rate: 3\nbatch setting: 4");
	printf("\nTemperature setting: 5\nPacing Mode: 6\nPV->SP loop: 7\nCharge: 8");
	printf("\nWalking beam step: 9\nBack step  : 10\nDischarge : 11\nDisch.first: 12");
	printf("\nRecharge : 13");
	printf("\nCharging Temp: 14\nBatch Config: 15\nQuit: 0\nSelection: ");
	int saisie =  0;
	scanf("%i", &saisie);
	if (saisie == 1 ) arret();
	if (saisie == 2 ) Accel();
	if (saisie == 3) Prod();
	if (saisie == 4) batch();
	if (saisie == 5) Temp();
	if (saisie == 6) Pace();
	if (saisie == 7) Loop();
	if (saisie == 8) charge();
	if (saisie == 9) step();
	if (saisie == 10) back_step();
	if (saisie == 11) discharge_out();
	if (saisie == 12) discharge_in();
	if (saisie == 13) recharge();
	if (saisie ==14) fixtemp();
	if (saisie ==15) fixBatch();
	if (saisie == 0) break;
  }
}

void fixBatch()
{
	char *DB_path ="";
	char txt[11];//10 char + 1 fin de chaine
	char name[11]; //batch name
	int erreur = 1;
	int status = DBopen(DB_ChBatch_Item,1,DB_path);

	printf ("Batch name (10 char) : ");
	scanf("%s",name);
	erreur *= DBwrite(DB_ChBatch_Item,0,DB_ChBatch_BName,name, 10*sizeof(char));
	printf ("\nCast name (10 char) : ");
	scanf("%s",txt);
	erreur = DBwrite(DB_ChBatch_Item,0,DB_ChBatch_CastName,txt, 10*sizeof(char));
	printf ("\nSteel name (10 char) : ");
	scanf("%s",txt);
	erreur *= DBwrite(DB_ChBatch_Item,0,DB_ChBatch_SteelName,txt, 10*sizeof(char));

	int  saisie =  0;
	printf("\nWidth :");
	scanf("%d", &saisie);
	erreur *= DBwrite(DB_ChBatch_Item,0,DB_ChBatch_Width,(char *) &saisie, sizeof(int));
	printf("\nThick :");
	scanf("%d", &saisie);
	erreur *= DBwrite(DB_ChBatch_Item,0,DB_ChBatch_Thick,(char *) &saisie, sizeof(int));
	printf("\nLength :");
	scanf("%d", &saisie);
	erreur *= DBwrite(DB_ChBatch_Item,0,DB_ChBatch_Length,(char *) &saisie, sizeof(int));
	printf("\nWeight :");
	scanf("%d", &saisie);
	erreur *= DBwrite(DB_ChBatch_Item,0,DB_ChBatch_Weight,(char *) &saisie, sizeof(int));

	printf("\nSteel Code :");
	scanf("%d", &saisie);
	char tt = (char) saisie;
	erreur *= DBwrite(DB_ChBatch_Item,0,DB_ChBatch_SteelCode,&tt, sizeof(char));
	float prod;
	printf("\nTheoretical production rate :");
	scanf("%f", &prod);
	erreur *= DBwrite(DB_ChBatch_Item,0,DB_ChBatch_ThProd,(char *) &prod, sizeof(float));
	printf("\nNumber of product in this Batch :");
	scanf("%d", &saisie);
	erreur *= DBwrite(DB_ChBatch_Item,0,DB_ChBatch_ProdNb,(char *) &saisie, sizeof(int));
	printf("\nDischarging Temp for this Batch :");
	scanf("%d", &saisie);
	erreur *= DBwrite(DB_ChBatch_Item,0,DB_ChBatch_TDef,(char *) &saisie, sizeof(int));

	DBclose(DB_ChBatch_Item);
}


void fixtemp()
{
	printf ("Charging Temperature: ");
	int  saisie =  0;
	scanf("%d", &saisie);
	int erreur = 1; // si pb de lecture  erreur = 0
	//chemin de direction où se trouve la database
	char *DB_path ="";
	int status = DBopen(DB_PLCEvent_Item,1,DB_path);

	if (!status)
	{
		erreur *= DBwrite(DB_PLCEvent_Item,0,DB_PLCEvent_ChT,(char *) &saisie, sizeof(int));
	}
	DBclose(DB_PLCEvent_Item);
}

void charge()
{
	char Event[6];

	for (int i=0; i<6; i++)
		Event[i]=0;

	Event[0]=1;
	Event[1]=0;       	   // Initialisation pour StorePLC()
	StorePLC((char *)Event); // Charge et avance (cf database.cpp)
}

void step()
{
	char Event[6];
	for (int i=0; i<6; i++)
		Event[i]=0;

	Event[0]=0;
	Event[1]=1;       	   // Initialisation pour StorePLC()
	StorePLC((char *)Event); // Charge et avance (cf database.cpp)
}

void back_step()
{
	char Event[6];
	for (int i=0; i<6; i++)
		Event[i]=0;

	Event[2]=1;
	Event[1]=0;       	   // Initialisation pour StorePLC()
	StorePLC((char *)Event); // Charge et avance (cf database.cpp)
}

void discharge_out()
{
	char Event[6];
	for (int i=0; i<6; i++)
		Event[i]=0;

	Event[3]=1;
	Event[1]=0;       	   // Initialisation pour StorePLC()
	StorePLC((char *)Event); // Charge et avance (cf database.cpp)
}

void discharge_in()
{
	char Event[6];
	for (int i=0; i<6; i++)
		Event[i]=0;

	Event[4]=1;
	Event[1]=0;       	   // Initialisation pour StorePLC()
	StorePLC((char *)Event); // Charge et avance (cf database.cpp)
}

void recharge()
{
	char Event[6];
	for (int i=0; i<6; i++)
		Event[i]=0;

	Event[5]=1;
	Event[1]=0;       	   // Initialisation pour StorePLC()
	StorePLC((char *)Event); // Charge et avance (cf database.cpp)
}

void Loop()
{
	printf ("Loop between PV and SP:(Yes = 1, No = 0) ");
	int  saisie =  0;
	scanf("%i", &saisie);
	int erreur = 1; // si pb de lecture  erreur = 0
	//chemin de direction où se trouve la database
	char *DB_path ="";
	int status = DBopen(DB_Simul_Item,1,DB_path);

	if (!status)
	{
		erreur *= DBwrite(DB_Simul_Item,0,DB_Simul_Boucl,(char *) &saisie, sizeof(int));
	}
	DBclose(DB_Simul_Item);
}

void Pace()
{
	printf ("Pacing Mode (Theo = 0, Meas = 1): ");
	int  saisie =  0;
	scanf("%i", &saisie);
	char choix;

	if (saisie == 0)
		choix = 1;
	else
		choix = 3;

	int erreur = 1; // si pb de lecture  erreur = 0
	//chemin de direction où se trouve la database
	char *DB_path ="";
	int status = DBopen(DB_Runtime_Item,1,DB_path);

	if (!status)
	{
		erreur *= DBwrite(DB_Runtime_Item,0,DB_Runtime_PacingMode,(char *) &choix, sizeof(char));
	}

	DBclose(DB_Runtime_Item);
}


void Temp()
{
	printf ("Zone Temperature setting: \n");
	printf ("Top zones : ");
	printf ("Recuperator temperature : ");
	int rectemp=  0;
	scanf("%i", &rectemp); rectemp += 273;
	printf("Zone Temperatures\n");
	int *temps; temps = new int[NBZONE];

	for (int i=0;i< NBZONE;i++)
	{
		printf ("Zone N: %i Temp: ",i);
		scanf("%i",&temps[i]);
		temps[i] += 273;
	}
	printf ("Bottom zones : ");
	printf ("Recuperator temperature : ");
	int rectempi=  0;
	scanf("%i", &rectempi);rectempi += 273;
	printf("Zone Temperatures\n");
	int *tempi; tempi = new int[NBZONE];

#ifdef NBZONEI
	for (i=0; i<NBZONEI; i++)
		{
		printf ("Zone N: %i Temp: ",i);
		scanf("%i",&tempi[i]);
		tempi[i] += 273;
		}
#endif
	StoreTemp(temps,tempi,rectemp,rectempi);
	delete temps; delete tempi;
}

void batch()
{
	printf ("Batch setting: \n");
	printf ("Product type : ");
	int saisie =  0;
	scanf("%i", &saisie);
	MakeBatch(saisie);
	printf ("Production rate (Prod/h) : ");
	scanf("%i", &saisie);
	AffectCadence(saisie);
}

void Accel()
{
	printf ("Acceleration: ");
	float saisie =  0.0;
	scanf("%f", &saisie);
	int erreur = 1; // si pb de lecture  erreur = 0
	//chemin de direction où se trouve la database
	char *DB_path ="";
	int status = DBopen(DB_Simul_Item,1,DB_path);

	if (!status)
	{
		erreur *= DBwrite(DB_Simul_Item,0,DB_Simul_Acc,(char *) &saisie, sizeof(float));
	}
	DBclose(DB_Simul_Item);
}

void Prod()
{
	printf ("Production rate (prod/h): ");
	float saisie =  0.0;
	scanf("%f", &saisie);
	int erreur = 1; // si pb de lecture  erreur = 0
	//chemin de direction où se trouve la database
	char *DB_path ="";
	int status = DBopen(DB_Simul_Item,1,DB_path);

	if (!status)
	{
		erreur *= DBwrite(DB_Simul_Item,0,DB_Simul_Prod,(char *) &saisie, sizeof(float));
	}
	DBclose(DB_Simul_Item);
}

void arret()
{
  int ThePos = 0;
  char deb[7];
  char fn[4];
  printf ("Enter the following parameters: \n");
  printf ("The stoppage will begin at: \n");
  printf ("Day: ");
  int saisie =  0;
  scanf("%i", &saisie);
  deb[0] = (char) saisie;  //j
  printf ("Month: ");
  scanf("%i", &saisie);
  deb[1] = (char) saisie;   //m
  printf ("Year: ");
  scanf("%i", &saisie);
  deb[2] = (char) saisie;   //y
  printf ("Hour: ");
  scanf("%i", &saisie);
  deb[3] = (char) saisie;   //h
  printf ("Minute: ");
  scanf("%i", &saisie);
  deb[4] = (char) saisie;   //mn
  printf ("Seconde: ");
  scanf("%i", &saisie);
  deb[5] = (char) saisie;   //s
  deb[6] = (char) '\0';//'\0'

  printf("\nDuration: \n");
  printf ("Day: ");
  scanf("%i", &saisie);
  fn[0] = (char) saisie;   //d
  printf ("Hour: ");
  scanf("%i", &saisie);
  fn[1] = (char) saisie;   //d
  printf ("Minute: ");
  scanf("%i", &saisie);
  fn[2] = (char) saisie;   //d
  fn[3] = (char)'\0'; 
  
   // recherche le nombre d'enregistrements dans le fichier delays.dta
  int status=DBopen(DB_Delays_Item,0,"");
  int nb_arrets = DBNbRecord(DB_Delays_Item);
  DBclose(DB_Delays_Item);

  if (nb_arrets >=0) ThePos = nb_arrets;
  if (nb_arrets < 0)
       printf("\n Error in file DELAYS.DTA \n");  
  else if (ThePos > 2)
       printf(
         "\n The delays buffer is full : TACT cannot load this stoppage\n");
  else
       SetArret(ThePos,deb,fn);
}


//**************************************************
//Enregistrement des arrêts dans la base de données 
//**************************************************
void SetArret(int Index,char *deb, char *dur)
{
  int erreur = 1; // si pb de lecture  erreur = 0
  int status = DBinsert ( DB_Delays_Item , Index , "");

  //chemin de direction où se trouve la database
  char *DB_path ="";

  //ouverture de la base de données en lecture
  status = DBopen(DB_Delays_Item,1,DB_path);

  if (!status)
   {
    // écriture des données dans la database
      char Ind = (char)Index;    
      erreur *= DBwrite(DB_Delays_Item,Index,DB_Delays_Id,(char *) &Ind, sizeof(char));
      erreur *= DBwrite(DB_Delays_Item,Index,DB_Delays_Debut,(char *) deb, 6*sizeof(char));
      erreur *= DBwrite(DB_Delays_Item,Index,DB_Delays_Fin,(char *) dur, 3*sizeof(char));
      long DurSec = ((24*(long)dur[0] + (long)dur[1])*60 + (long) dur[2]) * 60;
      erreur *= DBwrite(DB_Delays_Item,Index,DB_Delays_Duree,(char *) &DurSec, sizeof(long));
     

    //fermeture de la database
   DBclose(DB_Delays_Item);

  }
}

