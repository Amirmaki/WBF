// OWLCVT 12/23/96 15:30:05
//-------------------------------------------------------------------------
// Password.cpp
// This file contains function for project TACT
//
//
// This file manages the password acquisition
//
// Owner source file : password.CPP
//
// Creation : JL COUSIN  date: 19/10/94
// Modifs PF27/02/96 : Gestion multi-langues (cf globdat.h)
//-------------------------------------------------------------------------

#ifndef __FSTREAM_H
#include <fstream.h>
#endif
#ifndef __MATH_H
#include <math.h>
#endif
#ifndef __STDIO_H
#include <stdio.h>
#endif
#ifndef __STRING_H
#include <string.h>
#endif
#ifndef __DATABASE_H
#include "database.h"
#endif
#ifndef __GLOBDAT_H
#include "globdat.h"
#endif
#ifndef __PASSWORD_H
#include "password.h"
#endif

#include "appelhs.h"

#define DefPassWord   "GITHERMW"
#define PwdExtFich    ".BIN"

#if LANGUAGE == 1 						  // Italien
#define MsgPassWord1  "Parola d'Ordine Sbagliata"
#define MsgPassWord2  "Entrare di nuovo il codice o Cancellare"
#endif

#if LANGUAGE == 2 						  // Anglais
#define MsgPassWord1  "Wrong Password"
#define MsgPassWord2  "Try a new password or Cancel"
#endif

//#define ID_TIMER      1100 			  // Identificateur du timer
//#define Duree	      15000 			      // Durée timeout en ms

//--------------------------------------------------------------------------
void extract ( unsigned char K , int Bit[8] )
{
	unsigned char masque = 1;

	for (int i=0 ; i < 8 ; i++)
	{
		Bit[i] = (K & masque) >> i ;
		masque = masque << 1 ;
	}
}
//-------------------------------------------------------------------------
unsigned char compose ( int Bit[8] )
{
	unsigned char K=0;

	for (int i=0 ; i < 8 ; i++)
		K = K + Bit[i]*(int)(pow(2,i));

	return (K);
}
//-------------------------------------------------------------------------
int ReadPassWord (char *FileName, unsigned char *PassWord1)
{
	unsigned char *CodePassWord1 = "ABCDEFGH";  // password crypted
	int  PW1Bit[8][8], BitTab[8], k=0;

	// Lecture du fichier mot de passe
	fstream passfile;
	passfile.open(FileName,ios::in|ios::binary|ios::beg);

	if (!passfile) return(0);
		passfile.read(CodePassWord1,8*sizeof(char));	     // Open success

	passfile.close();
	// Eclatement des bits.
	for (k=0 ; k<8 ; k++)
	{
		extract(CodePassWord1[k],BitTab);

		for (int n=0 ; n<8 ; n++) PW1Bit[k][n]=BitTab[n];
	}
	// Inversion ligne / colonne
	for (k=0 ; k<8 ; k++)
	{
		for (int n=0 ; n<8 ; n++) BitTab[n]=PW1Bit[n][k] ;
		PassWord1[k] = ~compose(BitTab);
	}
	PassWord1[8] = 0;   // Zéro terminal pour comparer avec une vraie chaîne !
	return (1) ;
}
int getPassWord(char PassWord[9])
{
	char PassWordS[9], NomFichier[13];

	if (stricmp(PassWord, DefPassWord) == 0)
		return(1); 		   // Défaut

	sprintf(NomFichier,"%s%s",NomApplication,PwdExtFich);

	if ( (ReadPassWord(NomFichier,PassWordS) == 1)       // Lecture et décodage
		&& (stricmp(PassWord,PassWordS) == 0) ) 		     // Vérification
		return(1);
	else
		{
		ofstream f("p.txt");
		f << PassWord << PassWordS;
		f.close();
		return(0);
		}
}