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
//cette fonction detecte si un arr�t est en court et calcule la dur�e
//restante.
//********************************************************************
int CalculDurRest();


//***********************************************************************
//lecture du premier arr�t de la base de donn�e
//la base de donn�es ayant �t� rang�e par ordre croissant
//le premier arr�t correspond � celui qui interviendra le plus rapidement
//***********************************************************************
int loadDelay();


//***************************************************
//enregistrement de la dur�e restante
//***************************************************
int StoreData(long dur);


//*****************************************************
//compare la date de d�but d'arr�t avec la date syst�me
//pour detecter le d�but de l'arr�t
//*****************************************************
int detecte(char ProgDate[6]);

//************************************************************************
//cette fonction transforme la date de d�but d'arr�t en nombre de secondes
//�coul�es depuis le 01/01/70 � 0h00
//************************************************************************
long CalculDebutArretEnSec();


//**************************************************************
//cette fonction calcule le nombre de secondes qui restent avant
//la fin d'arr�t
//***************************************************************
long  CalculDureeRestanteEnSec();


//*********************************************************************
//cette fonction calcule le nombre de secondes �coul�es depuis le d�but
//de l'ann�e jusqu'� la date time_check
//*********************************************************************
long Calcsec(struct tm *time_check);


//*****************************************************
//renvoie le nombre de secondes s�parant ProgDat de
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
