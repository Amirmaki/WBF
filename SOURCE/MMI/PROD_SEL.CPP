#ifndef __TPDTOP_H
#define __OWNER_TPDTOP_H
#include "tpdtop.h"
#endif
#ifndef __DATABASE_H
#include "database.h"
#endif
#ifndef __DB_H
#include "db.h"
#endif
#ifndef __GLOBDAT_H
#include "globdat.h"
#endif

#include <math.h>
#include "config.h"
#include "language.h"
#include <stdlib.h>

#ifdef DebugCoulee
int IndC=0;
#endif

char DPas[NBCOL];
int NewBatchSelect=0;

void Products_Select()
{
	if(*init == INITIAL)
	{
		*init = NORMAL;
		APPELHS ("Utilise,PRODCFG.AID");

		APPELHS ("Option,verticale,N");
		APPELHS ("Sortie,ESC=V,RET=V,HOM=V,END=V,FLH=M,FLB=D,TF1=V,TF2=V,TF3=V,TF4=V,TF5=V,TF6=V,TF7=V,TF8=V,TF9=V,TF0=V");
		APPELHS ("Fusionne,logo_cc.img,1,2");
		APPELHS ("Fusionne,prod.img,3,9");
		APPELHS ("Option,Couleur,S=15");    // couleur saisie (fond*16+caractere)
		APPELHS ("Option,Couleur,A=240");

		APPELHS ("Ecran,Select,-,NUM");
		APPELHS ("Ecran,Select,-,GROUP");
		APPELHS ("Ecran,Select,-,WIDTH");
		APPELHS ("Ecran,Select,-,HEIGHT");
		APPELHS ("Ecran,Select,-,DISCH");
		APPELHS ("Ecran,Select,-,CODE");

		PrdSel_Buf = new char[sizeof(TProd)];
		PrdSel_InitWindow();
		PrdSel_SetupWindow();
	}
	APPELHS ("Ecran,Saisie/nonblocant");

	if(!strcmp(HsTouche,"TF1"))
	{
		*init=INITIAL;
#ifdef DBG_VUE
		fstream f;
		f.open("vue.txt", ios::in);
		f >> vue;
		f.close();
#endif
		Vue_Precedente=vue;
		vue = HELP_SCREEN;
#ifdef DBG_VUE
		f.open("vue.txt", ios::out);
		f << vue;
		f.close();
#endif
	}

	if (!strcmp(HsTouche,"TF2"))
	{
		GetSelect();
		PrdSel_WriteSelect();
		PrdSel_MakeBatch();
	}

	if (!strcmp(HsTouche,"FLB"))
		PrdSel_Down();

	if (!strcmp(HsTouche,"FLH"))
		PrdSel_Up();

	if (!strcmp(HsTouche,"TF7"))
		PrdSel_PgUp();

	if (!strcmp(HsTouche,"TF8"))
		PrdSel_PgDown();

	if (!strcmp(HsTouche,"END"))
		PrdSel_End();

	if (!strcmp(HsTouche,"HOM"))
		PrdSel_Home();

	if(!strcmp(HsTouche,"TF4") || !strcmp(HsTouche,"TF0"))
	{
		delete PrdSel_Buf;
		Sortie_Ecran(HsTouche);
	}
}

void PrdSel_InitWindow()
{ // Initialisation
	PrdSel_IndexList=0;
	PrdSel_Init(); 		    // Initialisation  des données dans la structure
	PrdSel_LoadData(PrdSel_IndexList); 	     // Chargement des données de la base de données
	PrdSel_LectSelect();	 // Lecture type de pdt selectionné dans la base de données
	//	TransferBuffer = (void far*)&(TProd);
}
//--------------------------------------------------------------------------
void PrdSel_Init()    // Initialisation  des données dans la structure
{
	for (int i=0;i<NBCOL;i++)
	{
		strcpy(TProd[i].Num,"0");
		strcpy(TProd[i].Quality,"0");
		strcpy(TProd[i].Width,"0");
		strcpy(TProd[i].Thick,"0");
		strcpy(TProd[i].ReqDisT,"0");
		strcpy(TProd[i].Code,"0");
		DPas[i] = 0;
	}
}
// -------------------------------------------------------------------------
void PrdSel_SetupWindow()
{               //	for (int i=0;i<NBCOL;i++)
//		if (DPas[i]=='1') ::CheckDlgButton(HWindow,ID_DPas_0 + i, 1);
	PrdSel_Affiche();
	PrdSel_AffichePoint();
}
// -------------------------------------------------------------------------
void PrdSel_Home()
{
	GetSelect();//WriteSelect();
	PrdSel_IndexList= 0;
	PrdSel_Init();
	PrdSel_LectSelect();
	PrdSel_LoadData(PrdSel_IndexList);
	PrdSel_Affiche();
}
// -------------------------------------------------------------------------
void PrdSel_End()
{
	GetSelect();
	PrdSel_WriteSelect();
	PrdSel_IndexList= PrdSel_NbRecord-1;
	PrdSel_Init();
	PrdSel_LoadData(PrdSel_IndexList);
	PrdSel_Affiche();
}
// -------------------------------------------------------------------------
void PrdSel_Up()
{
	APPELHS ("Ecran,Recupere,DEL");

	if (PrdSel_IndexList > 0)
	{
//		GetSelect();
//		WriteSelect();
		PrdSel_Init();
		PrdSel_LoadData(--PrdSel_IndexList);
		PrdSel_Affiche();
	}
	else
		APPELHS ("Affiche,DEL=%d",--HsEntier);
}
// -------------------------------------------------------------------------
void PrdSel_PgUp()
{
	GetSelect();//WriteSelect();

	if (PrdSel_IndexList > NBCOL)
		PrdSel_IndexList -=NBCOL;
	else
		PrdSel_IndexList = 0;

	PrdSel_Init();
	PrdSel_LoadData(PrdSel_IndexList);
	PrdSel_Affiche();
}
// -------------------------------------------------------------------------
void PrdSel_Down()
{

	APPELHS ("Ecran,Recupere,DEL");
	if (HsEntier < 10)
		APPELHS ("Affiche,DEL=%d",++HsEntier);

	else if (PrdSel_NbRecord+1 > NBCOL && PrdSel_IndexList+1 < PrdSel_NbRecord)
	{
//		GetSelect();
//		WriteSelect();
		PrdSel_Init();
		PrdSel_LoadData(++PrdSel_IndexList);
		PrdSel_Affiche();
	}
}
// -------------------------------------------------------------------------
void PrdSel_PgDown()
{
	GetSelect();//	WriteSelect();

	if (PrdSel_IndexList < (PrdSel_NbRecord-NBCOL))
		PrdSel_IndexList +=NBCOL;
	else
		PrdSel_IndexList = PrdSel_NbRecord - 1;

	PrdSel_Init();
	PrdSel_LoadData(PrdSel_IndexList);
	PrdSel_Affiche();
}
//--------------------------------------------------------------------------
void PrdSel_LectSelect() 	   // Lecture du type de produit selectionné
{
	int status, erreur = 1;

	status = DBopen(DB_Runtime_Item,0,""); 	     // Ouverture en lecture
	if (!status)
		{
		erreur *= LireDB(DB_Runtime_Item,0,DB_Runtime_SelType,(char *)&PrdSel_SelType,
				 sizeof(int)); 	    // Lecture du produit selectioné
//		if (!erreur)
//		BWCCMessageBox(HWindow,DB_Runtime_Name,MsgTPdtOp2,MB_ICONSTOP);

		DBclose(DB_Runtime_Item); 		 // Fermeture de la database
		}
//	else BWCCMessageBox(HWindow,DB_Runtime_Name,MsgTPdtOp3,MB_ICONSTOP);
}
//--------------------------------------------------------------------------
void PrdSel_WriteSelect()   // Ecriture du type de produit selectionné
{
	int status, erreur = 1;

	status = DBopen(DB_Runtime_Item,1,""); 	     // Ouverture en ecriture

	if (!status)
	{
		erreur *= DBwrite(DB_Runtime_Item,0,DB_Runtime_SelType,(char *)&PrdSel_SelType,
				sizeof(int));

 //		if (!erreur)
 //			BWCCMessageBox(HWindow,DB_Runtime_Name,MsgTPdtOp2,MB_ICONSTOP);
		DBclose(DB_Runtime_Item);
	}
//	else BWCCMessageBox(HWindow,DB_Runtime_Name,MsgTPdtOp3,MB_ICONSTOP);
}
//--------------------------------------------------------------------------
void GetSelect()
// Detection du type de produit selectionné à l'écran
{
	APPELHS ("Ecran,Recupere,DEL");
	PrdSel_SelType = atoi(TProd[HsEntier-1].Num);
}
//--------------------------------------------------------------------------
void PrdSel_AffichePoint()
{
	if (PrdSel_SelType > 0)
	{
		APPELHS("Affiche,DEL=0");
		for (int i = 0;i<NBCOL;i++)
			if (atoi(TProd[i].Num)==PrdSel_SelType)
					APPELHS("Affiche,DEL=%d",i+1);
	}
}
//--------------------------------------------------------------------------
void PrdSel_Affiche()
// Affichage des données à l'écran pour remise à jour et déplacement
{
	for (int i=0;i<NBCOL;i++)
	{
		APPELHS("Affiche,NUM,%d=%s",i+1,TProd[i].Num);
		APPELHS("Affiche,GROUP,%d=%s",i+1,TProd[i].Quality);
		APPELHS("Affiche,WIDTH,%d=%s",i+1,TProd[i].Width);
		APPELHS("Affiche,HEIGHT,%d=%s",i+1,TProd[i].Thick);
		APPELHS("Affiche,DISCH,%d=%s",i+1,TProd[i].ReqDisT);
		APPELHS("Affiche,CODE,%d=%s",i+1,TProd[i].Code);
	}
}
//--------------------------------------------------------------------------
void PrdSel_LoadData(int Index)
//Transfert de la base de données vers la structure
{
	int status, erreur = 1;

	status = DBopen(DB_TypProd_Item,0,""); 	     // Ouverture en lecture
	PrdSel_NbRecord = DBNbRecord(DB_TypProd_Item); // Lecture du nb d'enregistrements

	if (!status) 		     // Chargement des données modèle de la database
	{
		for (int i=0;i<NBCOL && i<(PrdSel_NbRecord-Index);i++)
		{
			erreur *= LireDBInt(DB_TypProd_Item,(i+Index),DB_TProd_Num,
						TProd[i].Num);
			erreur *= LireDB(DB_TypProd_Item,(i+Index),DB_TProd_Quality,
						(char *) &TProd[i].Quality,sizeof(TProd[i].Quality));
			erreur *= LireDBInt(DB_TypProd_Item,(i+Index),DB_TProd_Width,
						TProd[i].Width);
			erreur *= LireDBInt(DB_TypProd_Item,(i+Index),DB_TProd_Thick,
						TProd[i].Thick);
			erreur *= LireDBInt(DB_TypProd_Item,(i+Index),DB_TProd_ReqDisT,
						TProd[i].ReqDisT);
			erreur *= LireDBChar(DB_TypProd_Item,(i+Index),DB_TProd_Code,
						TProd[i].Code);
//			erreur *= LireDBChar(DB_TypProd_Item,(i+Index),DB_TProd_DPas,
//			       	&(DPas[i]));
		}
//		if (!erreur)
//			BWCCMessageBox(HWindow,DB_LstObj_Name,MsgTPdtOp2,MB_ICONSTOP);
		DBclose(DB_TypProd_Item);
	}
//	else BWCCMessageBox(HWindow,DB_TypProd_Name,MsgTPdtOp3,MB_ICONSTOP);
}
//--------------------------------------------------------------------------
int PrdSel_MakeBatch()
// Cette fonction construit le batch à partir des données du type de produit
{
	char aux1, nom[LgrChLot0], datetab[6];
	int status = 0, erreur = 1, Id, aux, Width, Length, Thick, TDef;
	float aux2;
	strcpy(nom,"");
	// Ouverture de la base de données en lecture
	status += DBopen(DB_TypProd_Item,0,"");
//  status += DBopen(DB_CfgModel_Item,0,"");
	PrdSel_NbRecord = DBNbRecord(DB_TypProd_Item);

	if (!status)
	{ // Chargement des données Type de produit de la database
		int i=0, Found=0, Num;

		while ((!Found) && (i < PrdSel_NbRecord))
		{
			erreur *= LireDB(DB_TypProd_Item,i,DB_TProd_Num,(char *)&Num,
				sizeof(int));
			if (Num == PrdSel_SelType)
				Found++;
			else
				i++;
		}

		if (Found)
		{
			#ifdef DebugCoulee // Pour debug N°Coulee
			char coulee[LgrChLot0]={"\O"}, txt[1];

			strcpy(coulee,"couleenum");
			itoa(IndC,txt,10);
			strcat(coulee,txt);

			if (IndC<9)
				IndC++;
			else
				IndC=0;
			#endif // Fin debug N°Coulee

			status = DBopen(DB_ChBatch_Item,1,"");

			erreur *= LireDB(DB_TypProd_Item,i,DB_TProd_Width,(char *)&Width,
				sizeof(int));
			erreur *= LireDB(DB_TypProd_Item,i,DB_TProd_Length,(char *)&Length,
				sizeof(int));
			erreur *= LireDB(DB_TypProd_Item,i,DB_TProd_Thick,(char *)&Thick,
				sizeof(int));
			erreur *= LireDB(DB_TypProd_Item,i,DB_TProd_ReqDisT,(char *)&TDef,
				sizeof(int));

			erreur *= DBwrite(DB_ChBatch_Item,0,DB_ChBatch_BName,nom, 10*sizeof(char));
			erreur *= DBwrite(DB_ChBatch_Item,0,DB_ChBatch_CastName,nom, 10*sizeof(char));
			erreur *= LireDB(DB_TypProd_Item,i,DB_TProd_Quality,(char *) &nom,sizeof(char));
			erreur *= DBwrite(DB_ChBatch_Item,0,DB_ChBatch_SteelName,nom, 10*sizeof(char));
			erreur *= DBwrite(DB_ChBatch_Item,0,DB_ChBatch_Width,(char *) &Width, sizeof(int));
			erreur *= DBwrite(DB_ChBatch_Item,0,DB_ChBatch_Thick,(char *) &Thick, sizeof(int));
			erreur *= DBwrite(DB_ChBatch_Item,0,DB_ChBatch_Length,(char *) &Length, sizeof(int));
			aux = 0;
			erreur *= DBwrite(DB_ChBatch_Item,0,DB_ChBatch_Weight,(char *) &aux, sizeof(int));
			erreur *= LireDB(DB_TypProd_Item,i,DB_TProd_Code,&aux1,sizeof(char));
			erreur *= DBwrite(DB_ChBatch_Item,0,DB_ChBatch_SteelCode,&aux1, sizeof(char));
			aux2 = DefThProd; // Cadence théorique = valeur par défaut
			erreur *= DBwrite(DB_ChBatch_Item,0,DB_ChBatch_ThProd,&aux2, sizeof(float));
			erreur *= DBwrite(DB_ChBatch_Item,0,DB_ChBatch_TDef,(char *) &TDef, sizeof(int));

			DBclose(DB_ChBatch_Item);
		}
		DBclose(DB_TypProd_Item);
//		DBclose(DB_LstBatch_Item);
//		DBclose(DB_CfgModel_Item);

		if (erreur)
			return(1);
		else
			return(-1);
	}
	else // Problème d'ouverture du fichier
	{
		DBclose(DB_TypProd_Item);
//		DBclose(DB_LstBatch_Item);
//		DBclose(DB_CfgModel_Item);
//		BWCCMessageBox(HWindow,DB_LstBatch_Name, MsgTPdtOp3, MB_ICONSTOP);
		return (-1);
	}
}

/*
	struct date ladate;
	struct time letime;
//	Lire nb de batch actuel
	status = DBopen(DB_LstBatch_Item,2,"");
	PrdSel_NBRecordBatch = DBNbRecord(DB_LstBatch_Item);
	if (NBRecordBatch >= MaxBatchNb) 		     // Nb de batch > Max ?!
	{ BWCCMessageBox(HWindow,MsgBatchMax,MsgBatchCreat,MB_ICONSTOP);
	  DBclose(DB_LstBatch_Item);
	  return(-1);
	}

  if (PrdSel_NBRecordBatch > 0)
	 erreur = LireDB(DB_LstBatch_Item,PrdSel_NBRecordBatch-1,DB_Batch_Id,(char *)&Id,
			 sizeof(int));
	else
		Id = -1;

  Id++; 					// Identité du nouveau batch
*/

//----------------------------------------------------------------------------
/*
			erreur *= DBwrite(DB_LstBatch_Item,PrdSel_NBRecordBatch,DB_Batch_Coulee,
				 (char *)coulee,LgrChLot0*sizeof(char));

			#endif // Fin debug N°Coulee

			erreur *= DBwrite(DB_LstBatch_Item,PrdSel_NBRecordBatch,DB_Batch_Id,
				 //(char *)&aux,sizeof(int));
				 (char *)&Id,sizeof(int));

			memset(nom,0,LgrChLot0*sizeof(char));
			erreur *= DBwrite(DB_LstBatch_Item,PrdSel_NBRecordBatch,DB_Batch_Name,
				 nom,LgrChLot0*sizeof(char));
			erreur *= DBwrite(DB_LstBatch_Item,PrdSel_NBRecordBatch,DB_Batch_Coulee,
				 nom,LgrChLot0*sizeof(char));
			erreur *= DBwrite(DB_LstBatch_Item,PrdSel_NBRecordBatch,DB_Batch_Acier,
				 nom,LgrChLot0*sizeof(char));

			getdate(&ladate); gettime(&letime);
			datetab[0] = ladate.da_day; datetab[1] = ladate.da_mon;
			datetab[2] = (ladate.da_year) % 100;
			datetab[3] = letime.ti_hour; datetab[4] = letime.ti_min;
			datetab[5] = letime.ti_sec;

			erreur *= DBwrite(DB_LstBatch_Item,PrdSel_NBRecordBatch,DB_Batch_CharDate,
				 &(datetab[0]),6*sizeof(char));
			memset(datetab,0,6*sizeof(char));
			erreur *= DBwrite(DB_LstBatch_Item,PrdSel_NBRecordBatch,DB_Batch_DiscDate,
				 &(datetab[0]),6*sizeof(char));
			erreur *= LireDB(DB_TypProd_Item,i,DB_TProd_Width,(char *)&Width,
				sizeof(int));
			erreur *= LireDB(DB_TypProd_Item,i,DB_TProd_Length,(char *)&Length,
				sizeof(int));
			erreur *= LireDB(DB_TypProd_Item,i,DB_TProd_Thick,(char *)&Thick,
				sizeof(int));
			erreur *= LireDB(DB_TypProd_Item,i,DB_TProd_ReqDisT,(char *)&TDef,
				sizeof(int));
			erreur *= DBwrite(DB_LstBatch_Item,PrdSel_NBRecordBatch,DB_Batch_Width,
				 (char *)&Width,sizeof(int));
			erreur *= DBwrite(DB_LstBatch_Item,PrdSel_NBRecordBatch,DB_Batch_Thick,
				 (char *)&Thick,sizeof(int));
			erreur *= DBwrite(DB_LstBatch_Item,PrdSel_NBRecordBatch,DB_Batch_Length,
				 (char *)&Length,sizeof(int));
			erreur *= DBwrite(DB_LstBatch_Item,PrdSel_NBRecordBatch,DB_Batch_TDef,
				 (char *)&TDef,sizeof(int));
			aux = 0;
			erreur *= DBwrite(DB_LstBatch_Item,PrdSel_NBRecordBatch,DB_Batch_Weight,
				 (char *)&aux,sizeof(int));
			erreur *= LireDB(DB_TypProd_Item,i,DB_TProd_Code,&aux1,sizeof(char));
			erreur *= DBwrite(DB_LstBatch_Item,PrdSel_NBRecordBatch,DB_Batch_Code,
				 &aux1,sizeof(char));
			aux2 = DefThProd; // Cadence théorique = valeur par défaut
			erreur *= DBwrite(DB_LstBatch_Item,PrdSel_NBRecordBatch,DB_Batch_ThProd,
				 (char *)&aux2,sizeof(float));
			aux = 0; 			    // Ecrire num bill ds nv lot = 0
			erreur *= DBwrite(DB_LstBatch_Item,PrdSel_NBRecordBatch,DB_Batch_NbBill,
				 &aux,sizeof(int));
			erreur *= DBwrite(DB_LstBatch_Item,PrdSel_NBRecordBatch,DB_Batch_NbBillDef,
				 &aux,sizeof(int));
			aux1 = 0;
			erreur *= DBwrite(DB_LstBatch_Item,PrdSel_NBRecordBatch,DB_Batch_FinEnfmt,
				 &aux1,sizeof(char));
//	  erreur *= LireDB(DB_TypProd_Item,i,DB_TProd_DPas,&aux1,sizeof(char));
			erreur *= DBwrite(DB_LstBatch_Item,PrdSel_NBRecordBatch,DB_Batch_2Step,
				 &aux1,sizeof(char));   // Utilisation de 2 pas #
			erreur *= DBwrite(DB_LstBatch_Item,PrdSel_NBRecordBatch,DB_Batch_DbleFile,
				 (char *)&aux1,sizeof(char));
			erreur *= LireDB(DB_CfgModel_Item,0,DB_Model_KSurf,(char *)&aux1,
				sizeof(char));
			erreur *= DBwrite(DB_LstBatch_Item,PrdSel_NBRecordBatch,DB_Batch_SurfCode,
				 (char *) &aux1, sizeof(char));
			erreur *= LireDB(DB_CfgModel_Item,0,DB_Model_Epsif1,(char *)&aux2,
				sizeof(float));
			erreur *= DBwrite(DB_LstBatch_Item,PrdSel_NBRecordBatch,DB_Batch_Psi,
				 (char *)&aux2,sizeof(float));

			if(!Thick)
			{
				erreur *= LireDB(DB_CfgModel_Item,0,DB_Model_HCoef3,
						 (char *)&aux2,sizeof(float));
				erreur *= DBwrite(DB_LstBatch_Item,PrdSel_NBRecordBatch,DB_Batch_HCoeff,
						(char *) &aux2, sizeof(float));

				float Volume = M_PI * (float)Width * (float)Width
						  * (float)Length * 0.25;

				Volume = Volume * 1e-9;
				erreur *= DBwrite(DB_LstBatch_Item,PrdSel_NBRecordBatch,DB_Batch_Volume,
						(char *)&Volume,sizeof(float));
			}
			else
			{
				erreur*=LireDB(DB_CfgModel_Item,0,DB_Model_HCoef1,
					(char *)&aux2,sizeof(float));
				erreur*=DBwrite(DB_LstBatch_Item,PrdSel_NBRecordBatch,
					DB_Batch_HCoeff,(char *)&aux2,sizeof(float));

				float Volume = (float)Width * (float)Thick * (float)Length;
				Volume = Volume * 1e-9;

				erreur*=DBwrite(DB_LstBatch_Item,PrdSel_NBRecordBatch,
					DB_Batch_Volume,(char *)&Volume,sizeof(float));
			}

// JFC14/03/97
//	  erreur *= LireDB(DB_TypProd_Item,i,DB_TProd_Num,&aux1,sizeof(char));
			erreur *= LireDB(DB_TypProd_Item,i,DB_TProd_Num,(char*)&aux,sizeof(int));
//	  erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_ProdType,
//				 &aux1,sizeof(char));
			erreur *= DBwrite(DB_LstBatch_Item,PrdSel_NBRecordBatch,DB_Batch_ProdType,
				 (char*)&aux,sizeof(int));
// Fin JFC14/03/97

			if (PrdSel_NBRecordBatch > 0) 	       // Il existe un lot précédent
			{ // Lot préc. terminé => num bill ds lot = num dern bill
				status = DBopen(DB_LstProd_Item,0,"");
				int NbBill = DBNbRecord(DB_LstProd_Item);

				if (NbBill >0)
				{
					erreur *= LireDB(DB_LstProd_Item,NbBill-1,DB_Prod_NumLot,
						(char *)&aux,sizeof(int)); // Dernière bill.
					erreur *= DBwrite(DB_LstBatch_Item,PrdSel_NBRecordBatch-1,
						DB_Batch_NbBill,&aux,sizeof(int));
				}
				DBclose(DB_LstProd_Item);
			}
		} //end if (Found)
//		if (!erreur)
//		  BWCCMessageBox(HWindow,DB_LstBatch_Name,MsgTPdtOp2,MB_ICONSTOP);
		// Fermeture de la database
*/

//--------------------------------------------------------------------------
/*
BOOL TConfigDlg5::CanClose()
// Teste si la fenêtre peut être fermée sur validité des paramètres saisis
{
	GetSelect(); WriteSelect();
	if (SelType<0)
		{
		BWCCMessageBox(HWindow,MsgTPdtOp19,MsgTPdtOp20,MB_ICONSTOP);
		return (FALSE);
		}
	else return(TRUE);
}  */
/*
void PrdSel_Select(char *InputText )
{
	PrdSel_SelType = atoi(InputText);

	if(PrdSel_MakeBatch())
	{
		NewBatchSelect=1;
		PrdSel_WriteSelect();
	}
	else
		NewBatchSelect=0; 		 // Pb dans la construction du batch

	PrdSel_Init();
	PrdSel_LoadData(PrdSel_IndexList);
	PrdSel_Affiche();
}*/
//--------------------------------------------------------------------------
