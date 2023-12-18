//-------------------------------------------------------------------------
// Delay.H
// This file contains function prototype for project FIRST
//
//
//
// Owner source file : Delay.CPP
//
// Creation : JL COUSIN  date: 23/09/94
//-------------------------------------------------------------------------

#ifndef __DELAY_H
#define __DELAY_H


#ifdef __OWNER_DELAY_H
char Id;
char Debut[6];
char DureeInit[3];
long Duree;
int NotDefined=1;
long DureEnSec=0;
#else
extern char Id;
extern char Debut[6];
extern char DureeInit[3];
extern long Duree;
extern int NotDefined;
extern long DureEnSec;
#endif


//********************************************************************
//fonction principale
//cette fonction detecte si un arrêt est en court et calcule la durée
//restante.
//********************************************************************
int CalculDurRest();


//***********************************************************************
//lecture du premier arrêt de la base de donnée
//la base de données ayant été rangée par ordre croissant
//le premier arrêt correspond à celui qui interviendra le plus rapidement
//***********************************************************************
int loadDelay();


//***************************************************
//enregistrement de la durée restante
//***************************************************
int StoreData(long dur);


//*****************************************************
//compare la date de début d'arrêt avec la date système
//pour detecter le début de l'arrêt
//*****************************************************
int detecte(char ProgDate[6]);

//************************************************************************
//cette fonction transforme la date de début d'arrêt en nombre de secondes
//écoulées depuis le 01/01/70 à 0h00
//************************************************************************
long CalculDebutArretEnSec();


//**************************************************************
//cette fonction calcule le nombre de secondes qui restent avant
//la fin d'arrêt
//***************************************************************
long  CalculDureeRestanteEnSec();


//*********************************************************************
//cette fonction calcule le nombre de secondes écoulées depuis le début
//de l'année jusqu'à la date time_check
//*********************************************************************
long Calcsec(struct tm *time_check);


//*****************************************************
//renvoie le nombre de secondes séparant ProgDat de
// l'heure systeme
//si une erreur est survenue la fonction renvoie -1
//*****************************************************
int corrige(char ProgDate[6]);

//---------------------------------------------------
//
//---------------------------------------------------

#else

	extern int CalculDurRest();
	extern int loadDelay();
	extern int StoreData(long dur);
	extern int detecte(char ProgDate[6]);
	extern long CalculDebutArretEnSec();
	extern long  CalculDureeRestanteEnSec();
	extern long Calcsec(struct tm *time_check);
	extern int corrige(char ProgDate[6]);

#endif
