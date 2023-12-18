#include "batchfct.h"
#ifndef __TIME_H
#include <time.h>
#endif
#ifndef __MATH_H
#include <math.h>
#endif
#ifndef __STRING_H
#include <string.h>
#endif

#ifndef __DATABASE_H
#include "database.h"
#endif
//#ifndef __DB_H
//#include "db.h"
//#endif
#ifndef __GLOBDAT_H
#include "globdat.h"
#endif
//#ifndef __WINSTRUC_H
//#include "winstruc.h"
//#endif


//#define DebugCoulee
#ifdef DebugCoulee
int IndC=0;
#endif

int MakeBatch(int SelType)
// Cette fonction construit le batch à partir des données du type de produit
{
	char aux1, nom[LgrChLot0], datetab[6];
	int status = 0, erreur = 1, Id, aux, Width, Length, Thick;
	float aux2;
	struct tm *ladate;

	long ti = time(NULL);

	// Lire nb de batch actuel
	status = DBopen(DB_LstBatch_Item,2,"");
	int NBRecordBatch = DBNbRecord(DB_LstBatch_Item);

	if (NBRecordBatch > 0)
		erreur = LireDB(DB_LstBatch_Item,NBRecordBatch-1,DB_Batch_Id,(char *)&Id,
			sizeof(int));
	else
		Id = -1;
	Id++;                                         // Identité du nouveau batch
	// Ouverture de la base de données en lecture
	status += DBopen(DB_TypProd_Item,0,"");
	status += DBopen(DB_CfgModel_Item,0,"");
	int NbRecord = DBNbRecord(DB_TypProd_Item);

	if (!status)
	{ // Chargement des données Type de produit de la database
		int i=0, Found=0, Num;

		while ((!Found) && (i < NbRecord))
		{
			erreur *= LireDB(DB_TypProd_Item,i,DB_TProd_Num,(char *)&Num,
				sizeof(int));

			if (Num == SelType)
				Found++;
			else
				i++;
		}

		if (Found)
		{
			#ifdef DebugCoulee // Pour debug N°Coulee
			char coulee[LgrChLot0], txt[1];
			strcpy(coulee,"couleenum");
			itoa(IndC,txt,10);
			strcat(coulee,txt);

			if (IndC<9)
				IndC++;
			else
				IndC=0;

			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_Coulee,
				(char *)coulee,LgrChLot0*sizeof(char));
			#endif // Fin debug N°Coulee
			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_Id,
				 //(char *)&aux,sizeof(int));
				(char *)&Id,sizeof(int));

			memset(nom,0,LgrChLot0*sizeof(char));
			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_Name,
				nom,LgrChLot0*sizeof(char));
			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_Coulee,
				nom,LgrChLot0*sizeof(char));
			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_Acier,
				nom,LgrChLot0*sizeof(char));

			ladate = localtime(&ti);
			datetab[0] = ladate->tm_mday; datetab[1] = ladate->tm_mon + 1;
			datetab[2] = (ladate->tm_year) ;
			datetab[3] = ladate->tm_hour; datetab[4] = ladate->tm_min;
			datetab[5] = ladate->tm_sec;
			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_CharDate,
				 &(datetab[0]),6*sizeof(char));
			memset(datetab,0,6*sizeof(char));

			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_DiscDate,
				 &(datetab[0]),6*sizeof(char));
			erreur *= LireDB(DB_TypProd_Item,i,DB_TProd_Width,(char *)&Width,
				sizeof(int));
			erreur *= LireDB(DB_TypProd_Item,i,DB_TProd_Length,(char *)&Length,
				sizeof(int));
			erreur *= LireDB(DB_TypProd_Item,i,DB_TProd_Thick,(char *)&Thick,
				sizeof(int));
			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_Width,
				(char *)&Width,sizeof(int));
			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_Thick,
				(char *)&Thick,sizeof(int));
			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_Length,
				(char *)&Length,sizeof(int));

			aux = 0;
			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_Weight,
				(char *)&aux,sizeof(int));
			erreur *= LireDB(DB_TypProd_Item,i,DB_TProd_Code,&aux1,sizeof(char));
			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_Code,
				 &aux1,sizeof(char));
			aux2 = DefThProd; // Cadence théorique = valeur par défaut
			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_ThProd,
				(char *)&aux2,sizeof(float));

			aux1 = 1;                         // Ecrire num bill ds nv lot = 1
			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_NbBill,
				&aux1,sizeof(char));

			aux1 = 0;
			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_NbBillDef,
				&aux1,sizeof(char));
			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_FinEnfmt,
				&aux1,sizeof(char));
			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_2Step,
				&aux1,sizeof(char));
			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_Hetero,
				&aux1,sizeof(char));
			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_DbleFile,
				(char *)&aux1,sizeof(char));
			erreur *= LireDB(DB_CfgModel_Item,0,DB_Model_KSurf,(char *)&aux1,
				sizeof(char));
			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_SurfCode,
				(char *) &aux1, sizeof(char));
			erreur *= LireDB(DB_CfgModel_Item,0,DB_Model_Epsif1,(char *)&aux2,
				sizeof(float));
			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_Psi,
				(char *)&aux2,sizeof(float));

			if(!Thick)
			{
				erreur *= LireDB(DB_CfgModel_Item,0,DB_Model_HCoef3,
					(char *)&aux2,sizeof(float));
				erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_HCoeff,
					(char *) &aux2, sizeof(float));

				float Volume = M_PI * (float)Width * (float)Width
					* (float)Length * 0.25;

				Volume = Volume * 1e-9;
				erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_Volume,
					(char *)&Volume,sizeof(float));
			}
			else
			{
				erreur*=LireDB(DB_CfgModel_Item,0,DB_Model_HCoef1,
					(char *)&aux2,sizeof(float));
				erreur*=DBwrite(DB_LstBatch_Item,NBRecordBatch,
					DB_Batch_HCoeff,(char *)&aux2,sizeof(float));

				float Volume = (float)Width * (float)Thick * (float)Length;
				Volume = Volume * 1e-9;

				erreur*=DBwrite(DB_LstBatch_Item,NBRecordBatch,
					DB_Batch_Volume,(char *)&Volume,sizeof(float));
			}
			int num=0;

			erreur *= LireDB(DB_TypProd_Item,i,DB_TProd_Num,(char *)&num,sizeof(int));
			aux1 = (char) num;
			erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch,DB_Batch_ProdType,
				&aux1,sizeof(char));

			if (NBRecordBatch > 0)               // Il existe un lot précédent
			{ // Lot préc. terminé => num bill ds lot = num dern bill
				status = DBopen(DB_LstProd_Item,0,"");
				int NbBill = DBNbRecord(DB_LstProd_Item);
				erreur *= LireDB(DB_LstProd_Item,NbBill-1,DB_Prod_NumLot,&aux1,
					sizeof(char)); // Dernière billette enfournée
				erreur *= DBwrite(DB_LstBatch_Item,NBRecordBatch-1,

				DB_Batch_NbBill,&aux1,sizeof(char));
				DBclose(DB_LstProd_Item);
			}
		} //end if (Found)
		// Fermeture de la database
		DBclose(DB_TypProd_Item);
		DBclose(DB_LstBatch_Item);
		DBclose(DB_CfgModel_Item);

		if (erreur)
			return(1);
		else
			return(-1);
	}
	else // Problème d'ouverture du fichier
	{
		DBclose(DB_TypProd_Item);
		DBclose(DB_LstBatch_Item);
		DBclose(DB_CfgModel_Item);
		return (-1);
	}
}

int AffectCadence(float cad)
// Affecte la cadence au batch selectionné. Si aucun batch n'est présent ds
// la base de données ou pb dans l'execution la fonction renvoie -1 sinon 1
{
	int status = 0, erreur = 1;
	char *DB_path =Path;

	status = DBopen(DB_LstBatch_Item,1,DB_path);      // Ouverture en écriture
	int NbBatch = DBNbRecord(DB_LstBatch_Item);

	if (!status) // Affectation de la cadence au batch à l'enfournement
	{
		erreur *= DBwrite(DB_LstBatch_Item,NbBatch-1,DB_Batch_ThProd,
			(char*) &cad ,sizeof(float));
		DBclose(DB_LstBatch_Item);
	}
	else //problème d'ouverture du fichier
	{
		return(-1);
	}

	if (!erreur)
		return(-1);

	return(1);
}
