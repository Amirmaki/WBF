//---------------------------------------------------------------------------
// Module CFGINIT.C
//
// Programme de chargement de la Base de donnée TACT WINDOWS à partir des
// fichiers de configuration client.
//
// Arguments : Nom des fichiers clients
//
// Valeur renvoyée :  0  : OK
//             	     -1  : Nombre d'argument invalide
//             	     -2  : Probleme avec le fichier *.cfg
//             	     -3  : Probleme avec le fichier *.cfk
//             	     -4  : Probleme avec le fichier *.sim
//             	     -20 : Probleme avec CFGFOUR.DTA
//             	     -21 : Probleme avec CFGMODEL.DTA
//             	     -22 :               TYPPROD.DTA
//             	     -23 :               LSTOBJ.DTA
//             	     -24 :               CTRLFOUR.DTA
//             	     -25 :               PROFFOUR.DTA
//             	     -26 :               LSTBATCH.DTA
//             	     -27 :               LSTPROD.DTA
//             	     -28 :               STATUS.DTA
//             	     -29 :               PLCEVENT.DTA
//             	     -30 :               DELAYS.DTA
//             	     -31 :               RUNTIME.DTA
//						     -32 : 			 		 DISPROD.DTA
//						     -33 : 			 		 SIMUL.DTA
//
// Modifs PF04/01/95 : Initialisation du ModŠle en mˆme temps que la BdD
//        PF28/03/95 : Adaptation commø & BdD / COGNE
//---------------------------------------------------------------------------

// Modules inclus
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "database.h"
#include "dbinit.h"
#include "winstruc.h"

//--------------------------------------------------------------------------
int CFGinit (char *UserFlName)
{ char CfgFileName [13];  	     // Nom du fichier client avec extension
  FILE *CfgFile ;
  int NbZoneSup = 0, ErrorStatus = 0, erreur = 0, i = 0;
  int NbZoneInf = 0;

  float FurnLength = 0; 			    // Longueur du four (mm)
  char auxstr[82], auxchar = 0;
  int    auxint     = 0;
  long   auxlong    = 0;
  float  auxfloat   = 0;

  // ************************************ Reset et Ouverture des fichiers DB
  ErrorStatus=DBopen(DB_CfgFour_Item,4,Path);     // RàZ fichier config four
  if (ErrorStatus != 0) return(-20);
  DBclose( DB_CfgFour_Item ) ;
  ErrorStatus = DBinsert(DB_CfgFour_Item,0,Path);	 // Remplissage de 0
  if (ErrorStatus != 0) return(-20);
  ErrorStatus = DBopen(DB_CfgFour_Item,1,Path);     // Ouverture en ecriture
  if (ErrorStatus != 0) return(-20);

  // RàZ fichier config Modèle (cf config four)
  ErrorStatus=DBopen(DB_CfgModel_Item,4,Path);
  if (ErrorStatus != 0) return(-21);
  DBclose( DB_CfgModel_Item ) ;
  ErrorStatus = DBinsert (DB_CfgModel_Item,0,Path);
  if (ErrorStatus != 0) return(-21);
  ErrorStatus = DBopen( DB_CfgModel_Item , 1 , Path ) ;
  if (ErrorStatus != 0) return(-21);

  // RàZ fichier type de produits
  ErrorStatus = DBopen(DB_TypProd_Item,4,Path);
  if (ErrorStatus != 0) return(-22);
  DBclose(DB_TypProd_Item);

  // RàZ fichier liste objectifs
  ErrorStatus = DBopen( DB_LstObj_Item , 4 , Path ) ;
  if (ErrorStatus != 0) return(-23);
  DBclose(DB_LstObj_Item);

  // *************************** Lecture du fichier de configuration du four
  strcpy(CfgFileName,UserFlName);	   // Ouverture fichier client *.cfg
//  strcat(CfgFileName,".cfg"      );
  CfgFile = fopen(CfgFileName,"rt");
  if (CfgFile == NULL) return(-2);
  ErrorStatus = 1;
  fscanf(CfgFile,"%s%f",auxstr,&auxfloat); // Longueur
//  auxfloat = 20830;
  ErrorStatus *= DBwrite(DB_CfgFour_Item,0,DB_Four_Length,&auxfloat,
			 sizeof(float));
  FurnLength = auxfloat;
  fscanf(CfgFile,"%s%f",auxstr,&auxfloat); // Largeur
  ErrorStatus *=DBwrite(DB_CfgFour_Item,0,DB_Four_Width,&auxfloat,
			sizeof(float));
  fscanf(CfgFile,"%s%f",auxstr,&auxfloat); // Hauteur voute sup
  ErrorStatus *=DBwrite(DB_CfgFour_Item,0,DB_Four_HTop,&auxfloat,
			sizeof(float));

  for ( i=0 ; i < NbZonesMax ; i++) // Hauteur Zones sup
	 ErrorStatus*=DBwrite(DB_CfgFour_Item,0,DB_Four_ZHeightSup+i*sizeof(float),
				&auxfloat,sizeof(float));

  fscanf(CfgFile,"%s%f",auxstr,&auxfloat); // Hauteur voute inf
  ErrorStatus *=DBwrite(DB_CfgFour_Item,0,DB_Four_HBot,&auxfloat,
			sizeof(float));
  for ( i=0 ; i < NbZonesMax ; i++) // Hauteur Zones inf
	 ErrorStatus*=DBwrite(DB_CfgFour_Item,0,DB_Four_ZHeightInf+i*sizeof(float),
				&auxfloat,sizeof(float));

  fscanf(CfgFile,"%s%d",auxstr,&auxint); // Nb de zones sup
  NbZoneSup = auxchar = auxint ;
  ErrorStatus *=DBwrite(DB_CfgFour_Item,0,DB_Four_NbZoneSup,&auxchar,
			sizeof(char));

  fscanf(CfgFile,"%s%d",auxstr,&auxint); // Nb de zones inf
  NbZoneInf = auxchar = auxint ;
  ErrorStatus *=DBwrite(DB_CfgFour_Item,0,DB_Four_NbZoneInf,&auxchar,
			sizeof(char));

  fscanf(CfgFile,"%s%f",auxstr,&auxfloat); // Longueur pas 1
  ErrorStatus *=DBwrite(DB_CfgFour_Item,0,DB_Four_Step1,&auxfloat,
			sizeof(float));
  fscanf(CfgFile,"%s%d",auxstr,&auxint); // Nbre de pas
  auxchar = auxint ;
  ErrorStatus *=DBwrite(DB_CfgFour_Item,0,DB_Four_StepNb,&auxchar,
			sizeof(char));
  fscanf(CfgFile,"%s%f",auxstr,&auxfloat); // Pci
  ErrorStatus *=DBwrite(DB_CfgFour_Item,0,DB_Four_Pci,&auxfloat,
			sizeof(float));
  //Zone superieure***************************************************************
  fscanf(CfgFile,"%s",auxstr); // Label zone sup
  float Zabs, Ztcpl, Zprof, ZQg, ZQa, OldZabs = 0;
  int   Zsptmin, Zsptmax, Zpente, Zresptime, MinZPente = 10000;
  for ( i=0 ; i < NbZoneSup ; i++ )
	 { fscanf (CfgFile,"%s%f%f%f%f%f%d%d%d%d",auxstr,&Zabs,&Ztcpl,&Zprof,&ZQg,
			&ZQa,&Zsptmin,&Zsptmax,&Zpente,&Zresptime);
		if ( (ZQg > 0) && (Zpente < MinZPente) ) MinZPente = Zpente;
		ErrorStatus*=DBwrite(DB_CfgFour_Item,0,DB_Four_BegAbsSup+i*sizeof(float),
						&OldZabs,sizeof(float));
		OldZabs = Zabs;
		ErrorStatus*=DBwrite(DB_CfgFour_Item,0,DB_Four_EndAbsSup+i*sizeof(float),
						&Zabs,sizeof(float));
		ErrorStatus*=DBwrite(DB_CfgFour_Item,0,DB_Four_TcplAbsSup+i*sizeof(float),
						&Ztcpl,sizeof(float));
		ErrorStatus*=DBwrite(DB_CfgFour_Item,0,DB_Four_TcplDepthSup+i*sizeof(float),
						&Zprof,sizeof(float));
		ErrorStatus*=DBwrite(DB_CfgFour_Item,0,DB_Four_QGMaxSup+i*sizeof(float),
						&ZQg,sizeof(float));
		ErrorStatus*=DBwrite(DB_CfgFour_Item,0,DB_Four_QAMaxSup+i*sizeof(float),
						&ZQa,sizeof(float));
		ErrorStatus*=DBwrite(DB_CfgFour_Item,0,DB_Four_SptMinSup+i*sizeof(int),
						&Zsptmin,sizeof(int));
		ErrorStatus*=DBwrite(DB_CfgFour_Item,0,DB_Four_SptMaxSup+i*sizeof(int),
						&Zsptmax,sizeof(int));
		ErrorStatus*=DBwrite(DB_CfgFour_Item,0,DB_Four_SptVarSup+i*sizeof(int),
						&Zpente,sizeof(int));
		ErrorStatus*=DBwrite(DB_CfgFour_Item,0,DB_Four_RespTimeSup+i*sizeof(int),
						&Zresptime,sizeof(int));
	 }
  //Zone inférieure***************************************************************
  fscanf(CfgFile,"%s",auxstr); // Label zone inf
  OldZabs = 0;
  MinZPente = 10000;
  float Loss= 0.0;
  for ( i=0 ; i < NbZoneInf ; i++ )
	 { fscanf (CfgFile,"%s%f%f%f%f%f%f%d%d%d%d",auxstr,&Zabs,&Ztcpl,&Zprof,&Loss,&ZQg,&ZQa,&Zsptmin,&Zsptmax,&Zpente,&Zresptime);
		if ( (ZQg > 0) && (Zpente < MinZPente) ) MinZPente = Zpente;
		ErrorStatus*=DBwrite(DB_CfgFour_Item,0,DB_Four_BegAbsInf+i*sizeof(float),
						&OldZabs,sizeof(float));
      OldZabs = Zabs;
      ErrorStatus*=DBwrite(DB_CfgFour_Item,0,DB_Four_EndAbsInf+i*sizeof(float),
      			   &Zabs,sizeof(float));
      ErrorStatus*=DBwrite(DB_CfgFour_Item,0,DB_Four_TcplAbsInf+i*sizeof(float),
      			   &Ztcpl,sizeof(float));
      ErrorStatus*=DBwrite(DB_CfgFour_Item,0,DB_Four_TcplDepthInf+i*sizeof(float),
      			   &Zprof,sizeof(float));

      ErrorStatus*=DBwrite(DB_CfgFour_Item,0,DB_Four_LossesInf+i*sizeof(float),
      			   &Loss,sizeof(float));

      ErrorStatus*=DBwrite(DB_CfgFour_Item,0,DB_Four_QGMaxInf+i*sizeof(float),
						&ZQg,sizeof(float));
      ErrorStatus*=DBwrite(DB_CfgFour_Item,0,DB_Four_QAMaxInf+i*sizeof(float),
      			   &ZQa,sizeof(float));
      ErrorStatus*=DBwrite(DB_CfgFour_Item,0,DB_Four_SptMinInf+i*sizeof(int),
      			   &Zsptmin,sizeof(int));
      ErrorStatus*=DBwrite(DB_CfgFour_Item,0,DB_Four_SptMaxInf+i*sizeof(int),
      			   &Zsptmax,sizeof(int));
		ErrorStatus*=DBwrite(DB_CfgFour_Item,0,DB_Four_SptVarInf+i*sizeof(int),
      			   &Zpente,sizeof(int));
      ErrorStatus*=DBwrite(DB_CfgFour_Item,0,DB_Four_RespTimeInf+i*sizeof(int),
      			   &Zresptime,sizeof(int));
    }

  fscanf(CfgFile,"%s%f",auxstr,&auxfloat); // longueur produit
  fscanf(CfgFile,"%s%f",auxstr,&auxfloat); // Tø enfournement
  fscanf(CfgFile,"%s%d",auxstr,&auxint); // Double pas
  auxchar = auxint ;
  ErrorStatus *=DBwrite(DB_CfgFour_Item,0,DB_Four_DblStep,&auxchar,
			sizeof(char));
  fscanf(CfgFile,"%s%f",auxstr,&auxfloat); // Longueur pas 2
  ErrorStatus *=DBwrite(DB_CfgFour_Item,0,DB_Four_Step2,&auxfloat,
			sizeof(float));
  fscanf(CfgFile,"%s%f",auxstr,&auxfloat); // Absc. chgt de pas
  ErrorStatus *=DBwrite(DB_CfgFour_Item,0,DB_Four_WBeamLim,&auxfloat,
			sizeof(float));
  fscanf(CfgFile,"%s%f",auxstr,&auxfloat); // Absc. debut de zone utile
  ErrorStatus *=DBwrite(DB_CfgFour_Item,0,DB_Four_ZUBeg,&auxfloat,
			sizeof(float));
  fscanf(CfgFile,"%s%f",auxstr,&auxfloat); // Absc. fin de zone utile
  ErrorStatus *=DBwrite(DB_CfgFour_Item,0,DB_Four_ZUEnd,&auxfloat,
			sizeof(float));
  // Modif PF29/06/95 : Rajout configø gradient max
  auxchar = 0;
  ErrorStatus *=DBwrite(DB_CfgFour_Item,0,DB_Four_TpsPente,&auxchar,
			sizeof(char));
  MinZPente *= 60; 					    // øC/mn -> øC/h
  ErrorStatus *=DBwrite(DB_CfgFour_Item,0,DB_Four_PenteMax,(char *)&MinZPente,
			sizeof(int));
  fclose (CfgFile) ;
  if ( !ErrorStatus ) return (-20) ;

  // *************************************** Fichier de configuration Modele
  strcpy(CfgFileName,UserFlName);	   // Ouverture fichier client *.cfk
  strcat(CfgFileName,".cfk");
  erreur=1;
  CfgFile = fopen(CfgFileName,"rt");
  if (CfgFile == NULL) return(-3);
  fscanf(CfgFile,"%s%f",auxstr,&auxfloat); // Slab limit
  fscanf(CfgFile,"%s%f",auxstr,&auxfloat); // topscoeff 1
  erreur*=DBwrite(DB_CfgModel_Item,0,DB_Model_HCoef1,&auxfloat,sizeof(float));
  sprintf(Model.HCoef1,"%f",auxfloat);
  fscanf(CfgFile,"%s%f",auxstr,&auxfloat); // topscoeff 2
  erreur*=DBwrite(DB_CfgModel_Item,0,DB_Model_HCoef2,&auxfloat,sizeof(float));
  sprintf(Model.HCoef2,"%f",auxfloat);;
  fscanf(CfgFile,"%s%f",auxstr,&auxfloat); // topscoeff 3
  erreur*=DBwrite(DB_CfgModel_Item,0,DB_Model_HCoef3,&auxfloat,sizeof(float));
  sprintf(Model.HCoef3,"%f",auxfloat);;
  fscanf(CfgFile,"%s%f",auxstr,&auxfloat); // Epsif 1
  erreur*=DBwrite(DB_CfgModel_Item,0,DB_Model_Epsif1,&auxfloat,sizeof(float));
  erreur*=DBwrite(DB_CfgFour_Item ,0,DB_Four_Epsif1 ,&auxfloat,sizeof(float));
  sprintf(Model.Epsif1,"%f",auxfloat);
  fscanf(CfgFile,"%s%f",auxstr,&auxfloat); // Epsif 2
  erreur*=DBwrite(DB_CfgModel_Item,0,DB_Model_Epsif2,&auxfloat,sizeof(float));
  erreur*=DBwrite(DB_CfgFour_Item ,0,DB_Four_Epsif2 ,&auxfloat,sizeof(float));
  sprintf(Model.Epsif2,"%f",auxfloat);
  fscanf(CfgFile,"%s%f",auxstr,&auxfloat); // Epsif min
  erreur*=DBwrite(DB_CfgModel_Item,0,DB_Model_EpsifMin,&auxfloat,
		  sizeof(float));
  erreur*=DBwrite(DB_CfgFour_Item,0,DB_Four_EpsifMin,&auxfloat,sizeof(float));
  sprintf(Model.EpsifMin,"%f",auxfloat);
  fscanf(CfgFile,"%s%f",auxstr,&auxfloat); // Epsif max
  erreur*=DBwrite(DB_CfgModel_Item,0,DB_Model_EpsifMax,&auxfloat,
		  sizeof(float));
  erreur*=DBwrite(DB_CfgFour_Item ,0,DB_Four_EpsifMax ,&auxfloat,
		  sizeof(float));
  sprintf(Model.EpsifMax,"%f",auxfloat);
  fscanf(CfgFile,"%s%d",auxstr,&auxint); // prod surf code (%)
  auxchar=auxint;
  erreur*=DBwrite(DB_CfgModel_Item,0,DB_Model_KSurf,&auxchar,sizeof(char));
  sprintf(Model.KSurf,"%i",auxint);
  fscanf(CfgFile,"%s%f",auxstr,&auxfloat); // tau2 coeff
  erreur*=DBwrite(DB_CfgModel_Item,0,DB_Model_KTime,&auxfloat,sizeof(float));
  sprintf(Model.KTime,"%f",auxfloat);
  for (i=0 ; i < NbZonesMax ; i++)  // coeff de correction emissivit‚ fum‚es
	 { fscanf(CfgFile,"%s%f",auxstr,&auxfloat);
		erreur*=DBwrite(DB_CfgModel_Item,0,DB_Model_KEpsiSup+i*sizeof(float),
				&auxfloat,sizeof(float));
		erreur*=DBwrite(DB_CfgFour_Item ,0,DB_Four_KfSup+i*sizeof(float),
						&auxfloat,sizeof(float));
		sprintf(Model.KEpsiSup[i],"%f",auxfloat);
	 }
  for (i=0 ; i < NbZonesMax ; i++)  // coeff de correction emissivit‚ fum‚es
	 { fscanf(CfgFile,"%s%f",auxstr,&auxfloat);
		erreur*=DBwrite(DB_CfgModel_Item,0,DB_Model_KEpsiInf+i*sizeof(float),
						&auxfloat,sizeof(float));
		erreur*=DBwrite(DB_CfgFour_Item ,0,DB_Four_KfInf+i*sizeof(float),
						&auxfloat,sizeof(float));
//      sprintf(Model.KEpsiInf[i],"%f",auxfloat);
	 }

  fscanf(CfgFile,"%s%d",auxstr,&auxint); // Periode de calcul
  erreur*=DBwrite(DB_CfgModel_Item,0,DB_Model_CalcPer,&auxint,sizeof(int));
  sprintf(Model.CalcPer,"%i",auxint);
  fscanf(CfgFile,"%s%d",auxstr,&auxint); // Periode de mesure
  erreur*=DBwrite(DB_CfgModel_Item,0,DB_Model_MeasPer,&auxint,sizeof(int));
  sprintf(Model.MeasPer,"%i",auxint);
  fscanf(CfgFile,"%s%d",auxstr,&auxint); // Periode de calcul Spt
  erreur*=DBwrite(DB_CfgModel_Item,0,DB_Model_SPCalcPer,&auxint,sizeof(int));
  sprintf(Model.SPCalcPer,"%i",auxint);
  fscanf(CfgFile,"%s%d",auxstr,&auxint); // Periode historique zone
  erreur*=DBwrite(DB_CfgModel_Item,0,DB_Model_ZHisPer,&auxint,sizeof(int));
  sprintf(Model.ZHisPer,"%i",auxint);
  fscanf(CfgFile,"%s%d",auxstr,&auxint); // Periode historique produits
  erreur*=DBwrite(DB_CfgModel_Item,0,DB_Model_PHisPer,&auxint,sizeof(int));
  sprintf(Model.PHisPer,"%i",auxint);
  fscanf(CfgFile,"%s%d",auxstr,&auxint); // Autorisation calcul SP
  erreur*=DBwrite(DB_CfgModel_Item,0,DB_Model_SPEnable,&auxint,sizeof(char));
  Model.SPEnable=(char) auxint;
  fscanf(CfgFile,"%s%d",auxstr,&auxint); // Autorisation historisatø four
  erreur*=DBwrite(DB_CfgModel_Item,0,DB_Model_ZHisEnable,&auxint,
		  sizeof(char));
  Model.ZHisEnable=(char) auxint;
  fscanf(CfgFile,"%s%d",auxstr,&auxint); // Autorisation historisatø pdt
  erreur*=DBwrite(DB_CfgModel_Item,0,DB_Model_PHisEnable,&auxint,
		  sizeof(char));
  Model.PHisEnable=(char) auxint;
  fscanf(CfgFile,"%s%d",auxstr,&auxint); // Autorisation pacing
  erreur*=DBwrite(DB_CfgModel_Item,0,DB_Model_PaceEnable,&auxint,
		  sizeof(char));
  Model.PaceEnable=(char) auxint;
  fscanf(CfgFile,"%s%d",auxstr,&auxint); // Autorisation connexø PLC
  erreur*=DBwrite(DB_CfgModel_Item,0,DB_Model_PLCConnect,&auxint,
		  sizeof(char));
  Model.PLCConnect=(char) auxint;
  fscanf(CfgFile,"%s%d",auxstr,&auxint); // Autorisation connexø R‚gulatø
  erreur*=DBwrite(DB_CfgModel_Item,0,DB_Model_CTLConnect,&auxint,
		  sizeof(char));
  Model.CTLConnect=(char) auxint;

  fclose (CfgFile) ;
  if (!erreur) return (-21) ;

  // ************************************ Fichier liste des types de produit
  int Pnum,Pwidth,Plength,Pthick,Pcode,Preqtemp,Pnbprod;
  float Pcad;
  char Pqual[20];
  i=0;
  erreur = 1;
  strcpy(CfgFileName,UserFlName); 	   // Ouverture fichier client *.pdt
  strcat(CfgFileName,".pdt" );
  CfgFile = fopen(CfgFileName,"rt");
  ErrorStatus = 0;
  auxstr[0] = '\0' ; // init chaine nulle
  if (CfgFile != NULL)
	 { // Lecture des 2 premieres lignes de commentaire
		fgets (auxstr,80,CfgFile) ;
		if (auxstr[0] == '\0') ErrorStatus = 1;
		auxstr[0] = '\0' ;
		if (!ErrorStatus) fgets (auxstr,80,CfgFile) ;
		if (auxstr[0] == '\0') ErrorStatus = 1;
		// Lecture des donn‚es type de produit
		while (fgets (auxstr,80,CfgFile)!=NULL)
		{ // Lecture donn‚es
			  sscanf(auxstr,"%d%d%d%d%d%f%d%d%20c",&Pnum,&Pwidth,&Plength,
				&Pthick,&Pcode,&Pcad,&Preqtemp,&Pnbprod,Pqual);
			  char *pos=strchr(Pqual,'\n');
			  if (pos!=NULL) *pos = '\0';
			  // Insere un nouvel enregistrement dans le fichier DB
	  ErrorStatus = DBinsert ( DB_TypProd_Item , i , Path ) ;
			  if (ErrorStatus != 0) return(-22);
			  // Ouverture DB Cfg  en ecriture
	  ErrorStatus = DBopen( DB_TypProd_Item , 1 , Path ) ;
			  if (ErrorStatus != 0) return(-22);
	  erreur*=DBwrite(DB_TypProd_Item,i,DB_TProd_Num,&Pnum,sizeof(int));
	  erreur*=DBwrite(DB_TypProd_Item,i,DB_TProd_Width,&Pwidth,
			  sizeof(int));
	  erreur*=DBwrite(DB_TypProd_Item,i,DB_TProd_Length,&Plength,
			  sizeof(int));
	  erreur*=DBwrite(DB_TypProd_Item,i,DB_TProd_Thick,&Pthick,
			  sizeof(int));
			  auxchar = Pcode;
	  erreur*=DBwrite(DB_TypProd_Item,i,DB_TProd_Code,&auxchar,
			  sizeof(char));
	  erreur*=DBwrite(DB_TypProd_Item,i,DB_TProd_ThProd,&Pcad,
			  sizeof(float));
	  erreur*=DBwrite(DB_TypProd_Item,i,DB_TProd_Quality,Pqual,
			  20*sizeof(char));
//	  Preqtemp += 273;
	  erreur*=DBwrite(DB_TypProd_Item,i,DB_TProd_ReqDisT,&Preqtemp,
			  sizeof(int));
			  auxchar = Pnbprod;
	  erreur*=DBwrite(DB_TypProd_Item,i,DB_TProd_NbProd,&auxchar,
			  sizeof(char));
			  DBclose ( DB_TypProd_Item ) ;
	  i++; // incremente num. enr.
		} //while
	 } // le fichier existe
  if (!erreur) return (-22) ;

  // ****************************** Fichier liste des objectifs de chauffage
  i=0;
  erreur = 1;
  int Ttype, ChT, status = 0, k = 0, Ttarget[NbZonesMax];
  float Tcad;
  strcpy(CfgFileName,UserFlName); 	   // Ouverture fichier client *.ref
  strcat(CfgFileName,".ref" );
  CfgFile = fopen(CfgFileName,"rt");
  ErrorStatus = 0;
  auxstr[0] = '\0' ; // init chaine nulle
  if (CfgFile != NULL)
	 { // Lecture des 2 premieres lignes de commentaire
		fgets (auxstr,80,CfgFile) ;
		if (auxstr[0] == '\0') ErrorStatus = 1;
		auxstr[0] = '\0' ;
		if (!ErrorStatus) fgets (auxstr,80,CfgFile) ;
		if (auxstr[0] == '\0') ErrorStatus = 1;
		// Lecture des données type de produit
		while (!ErrorStatus)
		{ // Lecture donn‚es cadence + type
			  status = fscanf(CfgFile,"%f%d%d",&Tcad,&Ttype,&ChT);
			  // Lecture des objectifs fin de zone
			  for (k=0 ; k < NbZoneSup ; k++)
		 fscanf(CfgFile,"%d",&Ttarget[k]);
			  if (status != EOF)
		 { // Insere un nouvel enregistrement dans le fichier DB
			ErrorStatus = DBinsert (DB_LstObj_Item, i, Path);
			if (ErrorStatus != 0) return(-23);
			// Ouverture DB Cfg  en ecriture
			ErrorStatus = DBopen(DB_LstObj_Item, 1, Path);
			if (ErrorStatus != 0) return(-23);
			// Ecriture des donn‚es lues
			erreur*=DBwrite(DB_LstObj_Item,i,DB_LObj_Num,&i,sizeof(int));
			erreur*=DBwrite(DB_LstObj_Item,i,DB_LObj_Prod,&Tcad,
							sizeof(float));
			erreur*=DBwrite(DB_LstObj_Item,i,DB_LObj_ChT,&ChT,
							sizeof(int));
			erreur*=DBwrite(DB_LstObj_Item,i,DB_LObj_PType,&Ttype,
							sizeof(int));
			erreur*=DBwrite(DB_LstObj_Item,i,DB_LObj_Target,Ttarget,
					NbZoneSup*sizeof(int));
			DBclose(DB_LstObj_Item) ;
			i++; // incremente num. enr.
		 } // not EOF
			  else ErrorStatus = 1;
		} // while
	 } // le fichier existe
  if (!erreur) return (-23);

	 // *************************************** Initialisation de SIMUL.DTA
	strcpy(CfgFileName,UserFlName);	   // Ouverture fichier client *.sim
	strcat(CfgFileName,".sim");
	erreur=1;
	CfgFile = fopen(CfgFileName,"rt");
	if (CfgFile == NULL) return(-4);

	ErrorStatus = DBopen(DB_Simul_Item, 1, Path);
	if (ErrorStatus != 0) return(-33);

	ErrorStatus = DBopen(DB_PLCEvent_Item, 1, Path);
	if (ErrorStatus != 0) return(-29);

	fscanf(CfgFile,"%s",auxstr); 				// lecture titre
	for (i = 0; i < NbZoneSup; i++)
		fscanf(CfgFile,"%f",&auxfloat);		// Tø des zones au d‚marrage

	fscanf(CfgFile,"%s",auxstr);				// lecture titre
	fscanf(CfgFile,"%d",&auxint);				// SP enable
	fscanf(CfgFile,"%s",auxstr); 				// lecture titre
	fscanf(CfgFile,"%d",&auxint);				// Loop SP->PV
//	printf("Loop: %d\n", auxint);
	erreur*=DBwrite(DB_Simul_Item,0,DB_Simul_Boucl,&auxint,sizeof(int));
	fscanf(CfgFile,"%s",auxstr);				// lecture titre
	fscanf(CfgFile,"%d",&auxint);				// Forcage PV
	fscanf(CfgFile,"%s",auxstr);				// lecture titre
	fscanf(CfgFile,"%f",&auxfloat);			// Acceleration
//	printf("Acc: %fs/mn\n", auxfloat);
	auxfloat /= 60.0;
//	printf("Acc: %2.2f\n", auxfloat);
	erreur*=DBwrite(DB_Simul_Item,0,DB_Simul_Acc,&auxfloat,sizeof(float));
	fscanf(CfgFile,"%s",auxstr); 				// lecture titre
	fscanf(CfgFile,"%f",&auxfloat);			// cadence d'enfournement
//	printf("Cadence: %2.2f P/h\n", auxfloat);
	erreur*=DBwrite(DB_Simul_Item,0,DB_Simul_Prod,&auxfloat,sizeof(float));
	fscanf(CfgFile,"%s",auxstr); 				// lecture titre
	fscanf(CfgFile,"%d",&auxint);				// Enfournement autoris‚
	fscanf(CfgFile,"%s",auxstr); 				// lecture titre
	fscanf(CfgFile,"%d",&auxint);				// Tø d'Enfournement
	erreur*=DBwrite(DB_PLCEvent_Item,0,DB_PLCEvent_ChT,&auxint,sizeof(int));
//	fscanf(CfgFile,"%s",auxstr); 				// lecture titre
//	fscanf(CfgFile,"%d",&auxint);				// Tø de D‚fournement
	DBclose(DB_PLCEvent_Item);
	fclose(CfgFile);
//	printf("Charging Temp: %døC\n", auxint);
	double d = 0.0;
	erreur*=DBwrite(DB_Simul_Item,0,DB_Simul_TimeEvt,&d,sizeof(double));
	erreur*=DBwrite(DB_Simul_Item,0,DB_Simul_TimeSP,&d,sizeof(double));

	erreur*=DBwrite(DB_PLCEvent_Item,0,DB_PLCEvent_ChT,&auxint,sizeof(int));
	DBclose(DB_PLCEvent_Item);

	// Fermeture des fichiers
	DBclose(DB_CfgFour_Item);
	DBclose(DB_CfgModel_Item);
	DBclose(DB_Simul_Item);

	return (0);
}// fin
