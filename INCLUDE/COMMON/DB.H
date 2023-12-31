//*********************
//PROJET TACT WINDOWS *
//*********************
//
//MODULE DB.CPP
//
//	=> Ce module red�finit les fonctions de lecture et
//	   d'�criture dans la base de donn�e
//
//Nom: JL Cousin 
//Date de cr�ation: 31/08/94
//
//---------------------------------------------------

#ifndef __DB_H
#define __DB_H

// lecture dans la base de donn�e d'un entier et renvoie de
// cet entier dans un chaine de caract�res
int LireDBInt(int i,int adr,int off,char *res);


// lecture dans la base de donn�e d'un flottant et renvoie de
// ce floattant dans un chaine de caract�res
int LireDBFloat(int i,int adr,int off,char *res,int virg=0);

// lecture dans la base de donn�e d'un caract�re et renvoie de
// ce caract�re dans un chaine de caract�res
int LireDBChar(int i,int adr,int off,char *res);

// �criture dans la base de donn�e d'un flottant
int DBwriteFloat(int i,int adr,int off,char *res);

// �criture dans la base de donn�e d'un entier
int DBwriteInt(int i,int adr,int off,char *res);

// �criture dans la base de donn�e d'un caract�re
int DBwriteChar(int i,int adr,int off,char *res);

//void LireProdHisDB(int i, char *buffer, char *buf1, int *buf2, int *numfich);
void LireProdHisDB(int i, char *buffer, int *numfich);
// Lit les donn�es du produit N� i ds la base de donn�es pour historisat�
// i : n� du produit ds four, buffer [] re�oit les donn�es � historiser,
// buf1[] le nom du lot et buf2[] les donn�es susceptibles d'�tre modifi�es
// numfich est affect� avec le num�ro de fichier HEC

int DBCommEnabled();
// Lit le flag "Autorisation communication" ds BdD Mod�le
// Renvoie 1 si autoris�, 0 si non autoris�, -1 si erreur

#endif