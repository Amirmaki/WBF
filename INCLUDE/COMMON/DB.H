//*********************
//PROJET TACT WINDOWS *
//*********************
//
//MODULE DB.CPP
//
//	=> Ce module redéfinit les fonctions de lecture et
//	   d'écriture dans la base de donnée
//
//Nom: JL Cousin 
//Date de création: 31/08/94
//
//---------------------------------------------------

#ifndef __DB_H
#define __DB_H

// lecture dans la base de donnée d'un entier et renvoie de
// cet entier dans un chaine de caractères
int LireDBInt(int i,int adr,int off,char *res);


// lecture dans la base de donnée d'un flottant et renvoie de
// ce floattant dans un chaine de caractères
int LireDBFloat(int i,int adr,int off,char *res,int virg=0);

// lecture dans la base de donnée d'un caractère et renvoie de
// ce caractère dans un chaine de caractères
int LireDBChar(int i,int adr,int off,char *res);

// écriture dans la base de donnée d'un flottant
int DBwriteFloat(int i,int adr,int off,char *res);

// écriture dans la base de donnée d'un entier
int DBwriteInt(int i,int adr,int off,char *res);

// écriture dans la base de donnée d'un caractère
int DBwriteChar(int i,int adr,int off,char *res);

//void LireProdHisDB(int i, char *buffer, char *buf1, int *buf2, int *numfich);
void LireProdHisDB(int i, char *buffer, int *numfich);
// Lit les données du produit N° i ds la base de données pour historisat°
// i : n° du produit ds four, buffer [] reçoit les données à historiser,
// buf1[] le nom du lot et buf2[] les données susceptibles d'être modifiées
// numfich est affecté avec le numéro de fichier HEC

int DBCommEnabled();
// Lit le flag "Autorisation communication" ds BdD Modèle
// Renvoie 1 si autorisé, 0 si non autorisé, -1 si erreur

#endif