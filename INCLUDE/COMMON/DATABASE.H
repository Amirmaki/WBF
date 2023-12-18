// --------------------------------------------------------------------------
// DATABASE.H : D‚finition de la base de donn‚es commune
// (en-tˆte de database.cpp)
//
// P. Favale - 11/04/94 : Cr‚ation
// Modif PF29/11/94 : Ajout de la fonction StorePLC pour la simulation
// Modif PF19/12/94 : Ajout de StoreSPasPV() pour simuler l'action des
//		      			 consignes sur les temp. mesur‚es (simulation)
// Modif PF28/03/95 : Adaptation communication / COGNE
// Modif PF29/06/95 : Nlle fonction : Calcul pente max four
//		      			 modif -> BdD config_four & run_time
// Modif FD10/09/96 : Ajout 2 nlles donnees dans DB_Simul_Item
// Modif JFC23/10/96: Ajout de PRefAbsc et CorrObj dans CTRLFOUR.DTA
// Modif JFC15/11/96: Ajout de TDef requise dans LSTBATCH.DTA
// Modif JFC18/11/96: Ajout de TDef dans CHBATCH.DTA
// --------------------------------------------------------------------------

#ifndef __DATABASE_H
#define __DATABASE_H

#ifndef __WINSTRUC_H
#include "winstruc.h"
#endif
#define MAXPATH  80
#define NbFileDB   15		    // Nombre de fichiers de Base de Donn‚es
#define NbZonesMax 9            	 // Nombre max de zones dans le four
#define Path	   "./" 		   // R‚pertoire o— se trouve la BdD

// Pour les historiques :
#define PHisDirDef "HISTOR" 			   // R‚pertoires par d‚faut
#define FHisDirDef "HISHEC"
#define THisDirDef "TEMP"
#define LnFichPHis "Directory_Archivi_Billette__: %s\n"     // Formats *.dir
#define LnFichFHis "Directory_Archivi_Forno_____: %s\n"
#define LnFichTHis "Directory_Archivi_Provvisori: %s\n"

#define DB_CfgFour_Name	"CFGFOUR.DTA "
#define DB_CfgFour_Size		893         // Lgr d'un bloc de données
#define DB_CfgFour_Item		0		// Num‚ro d'ordre du fichier
#define DB_Four_Length		0	// Adresses des données dans le bloc
#define DB_Four_Width		4
#define DB_Four_HTop			8
#define DB_Four_HBot			12
#define DB_Four_Epsif1		16
#define DB_Four_Epsif2		20
#define DB_Four_EpsifMin	24
#define DB_Four_EpsifMax	28
#define DB_Four_Pci			32
#define DB_Four_Step1		36
#define DB_Four_Step2		40
#define DB_Four_StepNb		44
#define DB_Four_DblStep		45
#define DB_Four_WBeamLim	46
#define DB_Four_ZUBeg		50
#define DB_Four_ZUEnd		54
#define DB_Four_ChBatchId	58
#define DB_Four_NbZoneSup	60
#define DB_Four_BegAbsSup	61
#define DB_Four_EndAbsSup	97
#define DB_Four_TcplAbsSup	133
#define DB_Four_ZHeightSup	169
#define DB_Four_TcplDepthSup	205
#define DB_Four_LossesSup	241
#define DB_Four_RespTimeSup	277
#define DB_Four_QGMaxSup	295
#define DB_Four_QAMaxSup	331
#define DB_Four_SptMinSup	367
#define DB_Four_SptMaxSup	385
#define DB_Four_SptVarSup	403
#define DB_Four_KfSup		421

// Modif calcul pente max
#define DB_Four_PenteMax	457	     // Pente max en øC/h / 1 entier
#define DB_Four_TpsPente	459 	     // Temps de moyennage / 1 octet

//ajout zone inf
#define DB_Four_NbZoneInf	460
#define DB_Four_BegAbsInf	461
#define DB_Four_EndAbsInf	497
#define DB_Four_TcplAbsInf	533
#define DB_Four_ZHeightInf	569
#define DB_Four_TcplDepthInf	605
#define DB_Four_LossesInf	641
#define DB_Four_RespTimeInf	677
#define DB_Four_QGMaxInf	695
#define DB_Four_QAMaxInf	731
#define DB_Four_SptMinInf	767
#define DB_Four_SptMaxInf	785
#define DB_Four_SptVarInf	803
#define DB_Four_KfInf		821
#define DB_Four_FlowRateRatio   857


#define DB_TypProd_Name  "TYPPROD.DTA "// Fichier liste des types de produits
#define DB_TypProd_Size		36    		 // Lgr d'un bloc de donn‚es
#define DB_TypProd_Item		1			   // Num‚ro d'ordre
#define DB_TProd_Num			0				// Adresses des donn‚es dans le bloc
#define DB_TProd_Width		2
#define DB_TProd_Length		4
#define DB_TProd_Thick		6
#define DB_TProd_Code		8
#define DB_TProd_ThProd		9
#define DB_TProd_Quality	13
#define DB_TProd_ReqDisT	33
#define DB_TProd_NbProd		35


#define DB_LstObj_Name   "LSTOBJ.DTA  "	// Fichier liste des objectifs
#define DB_LstObj_Size		29				// Lgr d'un bloc de donn‚es
#define DB_LstObj_Item		2			   // Num‚ro d'ordre
#define DB_LObj_Num		0				// Adresses des donn‚es dans le bloc
#define DB_LObj_Prod		2
#define DB_LObj_PType		6
#define DB_LObj_Active     	8
#define DB_LObj_Target		9
#define DB_LObj_ChT		27


#define DB_CtrlFour_Name	"CTRLFOUR.DTA"
#define DB_CtrlFour_Size	352	// Lgr d'un bloc de donn‚es
#define DB_CtrlFour_Item	3		// Num‚ro d'ordre du fichier
#define DB_CFour_RecTemp	0     // Temp‚rature r‚cup‚rateur / 1 entier
#define DB_CFour_ZTempSup	2		// 9 temp‚ratures / 1 entier
#define DB_CFour_SPSup		20	   // 9 consignes / 1 entier
#define DB_CFour_TargSup	38	   // 9 objectifs / 1 entier
#define DB_CFour_QGSup		56	   // 9 d‚bits gaz / 1 float
#define DB_CFour_QASup		92	   // 9 d‚bits air / 1 float
#define DB_CFour_BurnStSup	128	// 9 mots d'‚tats br–leurs / 1 octet
#define DB_CFour_CorrCad	137   // Correction de la cadence / 1 entier

// zone inf
#define DB_CFour_ZTempInf	139			// 9 temp‚ratures / 1 entier
#define DB_CFour_SPInf		157		   // 9 consignes / 1 entier
#define DB_CFour_TargInf	175		   // 9 objectifs / 1 entier
#define DB_CFour_QGInf		193		   // 9 d‚bits gaz / 1 float
#define DB_CFour_QAInf		229		   // 9 d‚bits air / 1 float
#define DB_CFour_BurnStInf	265			// 9 mots d'‚tats br–leurs / 1 octet
#define DB_CFour_RecTempI	274     		// Temp‚rature r‚cup‚rateur / 1 entier
#define DB_CFour_FlowRateRatio  276		// 9 ratios / 1 float
#define DB_CFour_PRefAbsc  312			// 9 abscs prods r‚f‚rence / 1 float
#define DB_CFour_CorrObj	348			// Correction objectifs / 1 float


#define DB_ProfFour_Name	"PROFFOUR.DTA"
#define DB_ProfFour_Size	12
#define DB_ProfFour_Item	4
#define DB_PFour_Absc		0			// Abscisse / 1 long
#define DB_PFour_RoofTSup	4		   // Temp. vo–te / 1 entier
#define DB_PFour_FumeTSup	6			// Temp. fum‚es / 1 entier
#define DB_PFour_RoofTInf	8			// Temp. vo–te inf / 1 entier
#define DB_PFour_FumeTInf	10			// Temp. fum‚es inf/ 1 entier


#define DB_LstBatch_Name	"LSTBATCH.DTA"	 // Fichier liste des batchs
#define DB_LstBatch_Size	87		// Lgr d'un bloc de donn‚es
#define DB_LstBatch_Item	5		// Num‚ro d'ordre du fichier
#define DB_Batch_Id			0		// Num‚ro de lot / 1 entier
#define DB_Batch_Name		2		// Nom du lot / 10 octets
#define DB_Batch_Coulee		13	   // Nom de la coul‚e / 10 octets
#define DB_Batch_Acier		24	   // Nom de l'acier / 10 octets
#define DB_Batch_CharDate	35	   // Date d'enfournement / 6 octets
#define DB_Batch_DiscDate	41	  	// Date de d‚fournement / 6 octets
#define DB_Batch_Width		47	   // Largeur de billette / 1 entier
#define DB_Batch_Thick		49	 	// Epaisseur de billette / 1 entier
#define DB_Batch_Length		51 	// Longueur de billette / 1 entier
#define DB_Batch_Weight		53 	// Poids de billette / 1 entier
#define DB_Batch_Code		55	 	// Code acier / 1 octet
#define DB_Batch_ThProd		56	   // Cadence th‚orique / 1 float
#define DB_Batch_NbBill		60    // Nombre billettes dans lot / 1 entier
#define DB_Batch_NbBillDef	62  	// Nb bill d‚fourn‚es dans lot / 1 entier
#define DB_Batch_FinEnfmt	64		// Lot termin‚ d'enfourner / 1 octet
#define DB_Batch_2Step		65   	// Utiliser 2 pas #  / 1 octet
#define DB_Batch_Hetero		66    // H‚t‚rog‚n‚it‚ max admise / 1 octet
#define DB_Batch_DbleFile	68
#define DB_Batch_SurfCode	69
#define DB_Batch_Psi			70
#define DB_Batch_HCoeff		74
#define DB_Batch_Volume		78
#define DB_Batch_ProdType	82		// type de produit / 1 char
#define DB_Batch_TDef      83		// Tø defmt requise / 1 int
// VB 1-20-98
#define DB_Batch_PONumber  85

#define DB_LstProd_Name		"LSTPROD.DTA "
#define DB_LstProd_Size		141		// Lgr d'un bloc de données
#define DB_LstProd_Item		6			// Num‚ro d'ordre du fichier
#define DB_Prod_Num			0	 		// Num‚ro de la billette / 1 entier
#define DB_Prod_NumLot		2 			// Num. d'ordre billette ds lot / 1 char
#define DB_Prod_Width		4
#define DB_Prod_Length		6
#define DB_Prod_Thick		8
#define DB_Prod_Weight		10		   // Poids de la billette
#define DB_Prod_Code			12
#define DB_Prod_ThProd		13
#define DB_Prod_BatchId		17
#define DB_Prod_Absc			19
#define DB_Prod_ResTime		23
#define DB_Prod_ResTPrech	27     	// Temps de s‚j. en pr‚ch. / 4 octets
#define DB_Prod_ResTNorm	31			// Temps de s‚j. en norm. / 4 octets
#define DB_Prod_CharDate	35
#define DB_Prod_DiscDate	41
#define DB_Prod_MeanTemp	47
#define DB_Prod_TSurfTemp	51
#define DB_Prod_BSurfTemp	55
#define DB_Prod_CoreTemp	59
#define DB_Prod_MTPrech		63 		// Temp. moy. en fin de pr‚ch. / 1 entier
#define DB_Prod_TTPrech		65 		// Temp. sup. en fin de pr‚ch. / 1 entier
#define DB_Prod_BTPrech		67 		// Temp. inf. en fin de pr‚ch. / 1 entier
#define DB_Prod_CTPrech		69 		// Temp. … coeur fin de pr‚ch. / 1 entier
#define DB_Prod_RTPrech		71   		// Temp. vo–te fin de pr‚ch. / 1 entier
#define DB_Prod_MTNorm		73  		// Temp. moy. en fin de norm. / 1 entier
#define DB_Prod_TTNorm		75  		// Temp. sup. en fin de norm. / 1 entier
#define DB_Prod_BTNorm		77  		// Temp. inf. en fin de norm. / 1 entier
#define DB_Prod_CTNorm		79  		// Temp. … coeur fin de norm. / 1 entier
#define DB_Prod_RTNorm		81 		// Temp. vo–te en fin de norm. / 1 entier
#define DB_Prod_CharTemp	83
#define DB_Prod_DCharTemp	85
#define DB_Prod_OxyDepth	87
#define DB_Prod_DecDepth	91
#define DB_Prod_InZoneDate	95
#define DB_Prod_OutZoneDate	99
#define DB_Prod_ZoneResTime	103
#define DB_Prod_EqProd			107
#define DB_Prod_Target			111	// Tableau des objectifs / 9 int
#define DB_Prod_LThermThick	129
#define DB_Prod_RThermThick	133
#define DB_Prod_ThThickCalc	137
#define DB_Prod_InZone			138
#define DB_Prod_EnfEnf			140   // Bill. enfourn‚e par enfmt / 1 octet


#define DB_Status_Name   "STATUS.DTA  "			   // Fichier status
#define DB_Status_Size		38		 // Lgr d'un bloc de donn‚es
#define DB_Status_Item		7			   // Num‚ro d'ordre
#define DB_Status_Last		0
#define DB_Status_Date		2
#define DB_Status_Msg		8   // Message 30 caracteres max


#define DB_PLCEvent_Name "PLCEVENT.DTA"
#define DB_PLCEvent_Size			14	// Lgr d'un bloc de donn‚es
#define DB_PLCEvent_Item			8	// Num‚ro d'ordre
#define DB_PLCEvent_Date 			0	// Adresses des donn‚es dans le bloc
#define DB_PLCEvent_Charge			6
#define DB_PLCEvent_FwdStep		7
#define DB_PLCEvent_BckStep		8
#define DB_PLCEvent_DischOut		9
#define DB_PLCEvent_DischIn     10
#define DB_PLCEvent_ReCharge	  11
#define DB_PLCEvent_ChT         12	// Tø d'enfournement


#define DB_Delays_Name   "DELAYS.DTA  "
#define DB_Delays_Size		14		// Lgr d'un bloc de donn‚es
#define DB_Delays_Item		9		// Num‚ro d'ordre
#define DB_Delays_Id			0		// Adresses des donn‚es dans le bloc
#define DB_Delays_Debut		1
#define DB_Delays_Fin		7
#define DB_Delays_Duree		10


#define DB_CfgModel_Name "CFGMODEL.DTA"
#define DB_CfgModel_Size	121		 // Lgr d'un bloc de donn‚es
#define DB_CfgModel_Item	10			   // Num‚ro d'ordre
#define DB_Model_HCoef1		0
#define DB_Model_HCoef2		4
#define DB_Model_HCoef3		8
#define DB_Model_Epsif1		12
#define DB_Model_Epsif2		16
#define DB_Model_EpsifMin	20
#define DB_Model_EpsifMax	24
#define DB_Model_KSurf		28
#define DB_Model_KTime		29
#define DB_Model_KEpsiSup	33
#define DB_Model_KEpsiInf	69
#define DB_Model_CalcPer	105
#define DB_Model_MeasPer	107
#define DB_Model_SPCalcPer	109
#define DB_Model_ZHisPer	111
#define DB_Model_PHisPer	113
#define DB_Model_SPEnable	115
#define DB_Model_ZHisEnable	116
#define DB_Model_PHisEnable	117
#define DB_Model_PaceEnable	118
#define DB_Model_PLCConnect	119
#define DB_Model_CTLConnect	120


#define DB_Runtime_Name   "RUNTIME.DTA "
#define DB_Runtime_Size					25
#define DB_Runtime_Item					11
#define DB_Runtime_SelType				0
#define DB_Runtime_SelBatch			2
#define DB_Runtime_TheoProd			4
#define DB_Runtime_MeasProd			8
#define DB_Runtime_ManProd				12
#define DB_Runtime_PacingMode			16
#define DB_Runtime_DateOfLastStep	17
#define DB_Runtime_DateOfLastDis		21


#define DB_DisProd_Name		"DISPROD.DTA "
#define DB_DisProd_Size		151	// Lgr d'un bloc de donn‚es
#define DB_DisProd_Item		12		// Num‚ro d'ordre du fichier
#define DB_DisProd_Num		0	 	// Num‚ro de la billette / 1 entier
#define DB_DisProd_NumLot	2 		// Num. d'ordre billette ds lot / 1 char
#define DB_DisProd_Width	4
#define DB_DisProd_Length	6
#define DB_DisProd_Thick	8
#define DB_DisProd_Weight	10		// Poids de la billette
#define DB_DisProd_Code		12
#define DB_DisProd_ThProd	13
#define DB_DisProd_BatchId	17
#define DB_DisProd_Absc		19
#define DB_DisProd_ResTime	23
#define DB_DisProd_ResTPrech	27 // Temps de s‚j. en pr‚ch. / 4 octets
#define DB_DisProd_ResTNorm	31	// Temps de s‚j. en norm. / 4 octets
#define DB_DisProd_CharDate	35
#define DB_DisProd_DiscDate	41
#define DB_DisProd_MeanTemp	47
#define DB_DisProd_TSurfTemp	51
#define DB_DisProd_BSurfTemp	55
#define DB_DisProd_CoreTemp	59
#define DB_DisProd_MTPrech		63 // Temp. moy. en fin de pr‚ch. / 1 entier
#define DB_DisProd_TTPrech		65 // Temp. sup. en fin de pr‚ch. / 1 entier
#define DB_DisProd_BTPrech		67 // Temp. inf. en fin de pr‚ch. / 1 entier
#define DB_DisProd_CTPrech		69 // Temp. … coeur fin de pr‚ch. / 1 entier
#define DB_DisProd_RTPrech		71 // Temp. vo–te fin de pr‚ch. / 1 entier
#define DB_DisProd_MTNorm		73 // Temp. moy. en fin de norm. / 1 entier
#define DB_DisProd_TTNorm		75 // Temp. sup. en fin de norm. / 1 entier
#define DB_DisProd_BTNorm		77 // Temp. inf. en fin de norm. / 1 entier
#define DB_DisProd_CTNorm		79 // Temp. … coeur fin de norm. / 1 entier
#define DB_DisProd_RTNorm		81 // Temp. vo–te en fin de norm. / 1 entier
#define DB_DisProd_CharTemp	83
#define DB_DisProd_DCharTemp	85
#define DB_DisProd_OxyDepth	87
#define DB_DisProd_DecDepth	91
#define DB_DisProd_InZoneDate	95
#define DB_DisProd_OutZoneDate	99
#define DB_DisProd_ZoneResTime	103
#define DB_DisProd_EqProd			107
#define DB_DisProd_Target			111    // Tableau des objectifs / 9 int
#define DB_DisProd_LThermThick	129
#define DB_DisProd_RThermThick	133
#define DB_DisProd_ThThickCalc	137
#define DB_DisProd_InZone			138
#define DB_DisProd_EnfEnf			140   // Bill. enfourn‚e par enfmt / 1 octet
#define DB_DisProd_Batch			141   // Nom du lot


#define DB_Simul_Name		"SIMUL.DTA   "
#define DB_Simul_Size			26 	// Lgr d'un bloc de donn‚es
#define DB_Simul_Item			13		// Numero d'ordre du fichier
#define DB_Simul_Prod			0	   //   1 float
#define DB_Simul_Acc				4	   //   1 float
#define DB_Simul_Boucl			8	   //   1 int
#define DB_Simul_TimeEvt		10	   //   1 double
#define DB_Simul_TimeSP			18	   //   1 double


#define DB_ChBatch_Name		"CHBATCH.DTA "
#define DB_ChBatch_Size			52
#define DB_ChBatch_Item			14
#define DB_ChBatch_BName		0			// 10 char
#define DB_ChBatch_CastName	10			// 10 char
#define DB_ChBatch_SteelName	20			// 10 char
#define DB_ChBatch_Width		30			// int
#define DB_ChBatch_Thick		32			// int
#define DB_ChBatch_Length		34			// int
#define DB_ChBatch_Weight		36      	// int
#define DB_ChBatch_SteelCode	38      	// char
#define DB_ChBatch_ThProd		39      	// float
#define DB_ChBatch_NbProd		43			// int
#define DB_ChBatch_ACK        45			// char (0,1,-1,-2)
#define DB_ChBatch_TDef       46      	// Tø d‚fournement / 1 int
//VB 1-20-98
#define DB_ChBatch_PONumber   48			//1 int PO Number
#define DB_ChBatch_JobID	   50			//1 int Job ID

int LireDB(int ,int ,int ,char *,int) ;
// Lire une donn‚e dans un fichier DB :
// i => Nø fichier DB, adr => Nø de bloc, off => adresse dans le bloc
// dest => adresse o— copier, lgr=> longueur de la donn‚e … copier (octets)
int DBwrite (int,int,int,void *,int);
int DBNbRecord(int );
// Ouverture du fichier
int DBopen   ( int , int , char * );
int DBclose  ( int );
int DBdelete ( int DBfile_num , int Rec_num , char *DBpath );
int DBinsert ( int DBfile_num , int Rec_num , char *DBpath );
int StorePLC ( char *Event); 		 // Ajout PF29/11/94 pour Simulation
int StoreTemp(int *Temp, int *TempI, int RecTemp,int RecTempI); // Ajout PF29/11/94 pour Simulation
int StoreSPasPV(int duree);	     // Ajout PF19/12/94 pour bouclage PV/SP
int DBdansModel();	      // Ajout PF04/01/95 pour Initialisat° de Model
// Relit le flag "SP autorisées" ds BdD, renvoie 0/1 si succès, -1 si erreur
char LireSPenable();
int DBread_path(); //lire les chemins d'acce dans tact.cfg
#ifdef __OWNER_DATABASE_H

char DB_Path_exe[MAXPATH] = "";
char DB_Path_log[MAXPATH] = "";
char DB_Path_DB[MAXPATH]  = "";
char DB_Path_cfg[MAXPATH] = "";
char DB_Path_hisfour[MAXPATH]  = "";
char DB_Path_hisprod[MAXPATH] = "";
char DB_Path_histemp[MAXPATH] = "";
int  MaxLog = 0;
char DBFileName[NbFileDB][15] = {DB_CfgFour_Name,DB_TypProd_Name,
				DB_LstObj_Name,DB_CtrlFour_Name,DB_ProfFour_Name,DB_LstBatch_Name,
				DB_LstProd_Name,DB_Status_Name,DB_PLCEvent_Name,DB_Delays_Name,
				DB_CfgModel_Name,DB_Runtime_Name,DB_DisProd_Name,DB_Simul_Name,
				DB_ChBatch_Name};
// Taille des blocs de donn‚es dans les fichiers :
int  DBDataSize[NbFileDB] = {DB_CfgFour_Size,DB_TypProd_Size,DB_LstObj_Size,
				DB_CtrlFour_Size,DB_ProfFour_Size,DB_LstBatch_Size,
				DB_LstProd_Size,DB_Status_Size,DB_PLCEvent_Size,DB_Delays_Size,
				DB_CfgModel_Size,DB_Runtime_Size,DB_DisProd_Size, DB_Simul_Size,
				DB_ChBatch_Size};
// Nombre d'enregistrements :
int DBDataNb[NbFileDB] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

// Numero de fichier
int DBFile[NbFileDB] ;
// VB:2-12-1998        //Moditication pour yazd1 et yazd2
char NomApplication[9];	 		     // Nom de l'application


#else
// VB:2-12-1998        //Moditication pour yazd1 et yazd2
extern char NomApplication[9];	 		     // Nom de l'application
extern char DB_Path_exe[MAXPATH] ;
extern char DB_Path_log[MAXPATH] ;
extern char DB_Path_DB[MAXPATH]  ;
extern char DB_Path_cfg[MAXPATH] ;
extern char DB_Path_hisfour[MAXPATH] ;
extern char DB_Path_hisprod[MAXPATH] ;
extern char DB_Path_histemp[MAXPATH];
extern int  MaxLog;
// Noms des fichiers de base de donn‚es :
extern char DBFileName[NbFileDB][15] ;
// Taille des blocs de donn‚es dans les fichiers :
extern int  DBDataSize[NbFileDB] ;
// Nombre d'enregistrements :
extern int DBDataNb[NbFileDB] ;
// Numero de fichier
extern int DBFile[NbFileDB] ;

#endif
#endif
