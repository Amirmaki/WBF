 #define __OWNER_HTCVWIND_H
 #ifndef __HTCVWIND_H
 #include "htcvwind.h"
 #endif
 #ifndef __WINSTRUC_H
 #include "winstruc.h"
 #endif
 #ifndef __DATABASE_H
 #include "database.h"
 #endif
 #ifndef __CONFIG_H
 #include "config.h"
 #endif
 #ifndef __LANGUAGE_H
 #include "language.h"
 #endif
 #ifndef __DB_H
 #include "db.h"
 #endif

void Heat_Config()
{
	if ( *init ==INITIAL)
		{
		*init = NORMAL;
		Temp = 2;  // Diff‚rencie la vue Heat Curves de la vue Product Config
					  // lorsque deux courbes sont identiques.

		Vue_Precedente = CONFIG_MENU;
		APPELHS ("Utilise,HEATCFG.AID");

		APPELHS ("Fusionne,logo_cc.img,1,2");
		APPELHS ("Fusionne,heat_ttl.img,1,30");
		APPELHS ("Fusionne,heat_scr.img,3,13");

		APPELHS ("Option,Couleur,S=240");    // couleur saisie (fond*16+caractere)
		APPELHS ("Option,Couleur,A=240");
		APPELHS ("Sortie,ESC=V,RET=V,TF1=V,TF2=V,TF3=V,TF4=V,TF6=V,TF7=V,TF8=V,TF9=V,TF0=V");

		Heat_Buf = new char[sizeof(LObj)];
		Heat_InitWindow();
		Heat_SetupWindow();
		}

	APPELHS ("Ecran,Saisie/nonblocant");

	if (!strcmp(HsTouche,"FLB"))
		Heat_Down();

	if (!strcmp(HsTouche,"FLH"))
		Heat_Up();

	if (!strcmp(HsTouche,"TF1"))
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

	if (!strcmp(HsTouche,"TF6"))
		Heat_Del();

	if (!strcmp(HsTouche,"TF7"))
		Heat_PgUp();

	if (!strcmp(HsTouche,"TF8"))
		Heat_PgDown();

	if (!strcmp(HsTouche,"END"))
		Heat_End();

	if (!strcmp(HsTouche,"HOM"))
		Heat_Home();

	if (!strcmp(HsTouche,"TF0") || !strcmp(HsTouche,"TF4"))
		Teste_Buffer(HsTouche);

	if (!strcmp(HsTouche,"TF2"))
		Teste_Buffer(HsTouche);
}

void Heat_InitWindow()
{
  //initialisation
	Heat_IndexList=0;
  //initialisation  des données dans la strcture
  //********************************************
	Heat_Init();

  //chargement des données de la base de données
  //********************************************
	Heat_LoadData(Heat_IndexList);

  //création des fenêtres d'édition avec les données correspondantes
  // à afficher ****************************************************
// 	TransferBuffer = (void far*)&(LObj);
}

  //initialisation  des données dans la structure
  //********************************************
void Heat_Init()
{
	for (int i=0;i<NBCOL;i++)
		{
		strcpy(LObj[i].Prod,"0");
		strcpy(LObj[i].PType,"0");
		strcpy(LObj[i].ChT,"20");

		for (int j=0;j<NBZONE;j++)
			strcpy(LObj[i].Target[j],"0");
		}
}

void Heat_SetupWindow()
{
  Heat_Affiche();
}

//gestion des déplacements dans le fichier
//***************************************

void Heat_Home()
{
	  //sauvegarde les données de la vue de configuration dans la structure
	Heat_FillBuffers();

	//sauvegarde de la structure dans la base de données
	Heat_StoreData(Heat_IndexList);

	Heat_IndexList=0;
	Heat_Init();
	Heat_LoadData(Heat_IndexList);
	Heat_Affiche();
	Heat_Unselect();
}

void Heat_End()
{
	  //sauvegarde les données de la vue de configuration dans la structure
	Heat_FillBuffers();

	//sauvegarde de la structure dans la base de données
	Heat_StoreData(Heat_IndexList);

	Heat_IndexList = Heat_NbRecord-1;
	Heat_Init();
	Heat_LoadData(Heat_IndexList);
	Heat_Affiche();
	Heat_Unselect();
}

void Heat_Up()
{
	Heat_FillBuffers();
	Heat_StoreData(Heat_IndexList);
	APPELHS("Ecran,Courant");

	if (Heat_IndexList > 0)
		{
		Heat_Init();
		Heat_LoadData(--Heat_IndexList);
		Heat_Affiche();
		Heat_Unselect();
		}

	else if(HsIndice > 1)
		APPELHS("Ecran,Premier,%s,%d",HsNom,HsIndice-1);
}

void Heat_PgUp()
{
	//sauvegarde les données de la vue de configuration dans la structure
	Heat_FillBuffers();
	//sauvegarde de la structure dans la base de données
	Heat_StoreData(Heat_IndexList);

	if (Heat_IndexList > NBCOL)
		Heat_IndexList -=NBCOL;
	else
		Heat_IndexList = 0;

	Heat_Init();
	Heat_LoadData(Heat_IndexList);
	Heat_Affiche();
	Heat_Unselect();
}

void Heat_Down()
{
	Heat_FillBuffers();

	Heat_StoreData(Heat_IndexList);

	APPELHS("Ecran,Courant");

	if (Heat_NbRecord+1 > NBCOL)
		{
		Heat_Init();
		Heat_LoadData(++Heat_IndexList);
		Heat_Affiche();
		Heat_Unselect();
		}

	else if(HsIndice < NBCOL)
		APPELHS("Ecran,Premier,%s,%d",HsNom,HsIndice+1);
}

void Heat_PgDown()
{
	Heat_FillBuffers();
	Heat_StoreData(Heat_IndexList);

	if (Heat_IndexList < (Heat_NbRecord-NBCOL))
		Heat_IndexList +=NBCOL;
	else
		Heat_IndexList = Heat_NbRecord - 1;

	Heat_Init();
	Heat_LoadData(Heat_IndexList);
	Heat_Affiche();
	Heat_Unselect();
}

void Heat_Del()
{
	Heat_FillBuffers();
	Heat_StoreData(Heat_IndexList);
	int TheIndexProd = Heat_ReadObjToDel();

	if (TheIndexProd!=-1 && Heat_NbRecord >0)
		{
		//sauvegarde les données de la vue de configuration dans la structure
		Heat_FillBuffers();

		//sauvegarde de la structure dans la base de données
		Heat_StoreData(Heat_IndexList);

		int status = DBdelete(DB_LstObj_Item,TheIndexProd,Path);

		//traitement du status
		if (status <0)
			{
			char aux[40];
			char val[3];
	//		strcpy (aux,MsgHtCv8);
			itoa(status,val,10);
			strcat(aux,val);
			File_Error(103,DB_LstObj_Name);
			}

		if (Heat_IndexList == Heat_NbRecord-1)
			if (Heat_IndexList>0)
				Heat_IndexList=Heat_NbRecord-2;
			else
				Heat_IndexList=0;

		Heat_Init();
		Heat_LoadData(Heat_IndexList);
		Heat_Affiche();
		}
}
//désélectionne les cases à cocher

void Heat_Unselect()
{
#ifdef DBG_VUE
	fstream f;
	f.open("vue.txt", ios::in);
	f >> vue;
	f.close();
#endif
	if (vue == HEAT_CONFIG)
		APPELHS("Affiche,DEL=0");
}

//fonction d'affichage des données à l'écran pour remise à jour
// et déplacement

void Heat_Affiche()
{
	for (int i=0;i<NBCOL;i++)
		{
		APPELHS("Affiche,PRODUIT,%d=%s",i+1,LObj[i].Prod);
		APPELHS("Affiche,TYPE,%d=%s",i+1,LObj[i].PType);
		APPELHS("Affiche,ZONE0,%d=%s",i+1,LObj[i].Target[0]);
		APPELHS("Affiche,ZONE1,%d=%s",i+1,LObj[i].Target[1]);
		APPELHS("Affiche,ZONE2,%d=%s",i+1,LObj[i].Target[2]);
		APPELHS("Affiche,ZONE3,%d=%s",i+1,LObj[i].Target[3]);
		APPELHS("Affiche,ZONE4,%d=%s",i+1,LObj[i].Target[4]);
		}
}

//sauvegarde les données de la vue de configuration dans la structure
//*******************************************************************
void Heat_FillBuffers()
{
	for (int i=0; i<NBCOL; i++)
		{
		APPELHS("Ecran,Recupere,PRODUIT,%d=",i+1);
		sscanf(HsChaine,"%s",&LObj[i].Prod);

		APPELHS("Ecran,Recupere,TYPE,%d=",i+1);
		sscanf(HsChaine,"%s",&LObj[i].PType);

		APPELHS("Ecran,Recupere,ZONE0,%d=",i+1);
		sscanf(HsChaine,"%s",&LObj[i].Target[0]);

		APPELHS("Ecran,Recupere,ZONE1,%d=",i+1);
		sscanf(HsChaine,"%s",&LObj[i].Target[1]);

		APPELHS("Ecran,Recupere,ZONE2,%d=",i+1);
		sscanf(HsChaine,"%s",&LObj[i].Target[2]);

		APPELHS("Ecran,Recupere,ZONE3,%d=",i+1);
		sscanf(HsChaine,"%s",&LObj[i].Target[3]);

		APPELHS("Ecran,Recupere,ZONE4,%d=",i+1);
		sscanf(HsChaine,"%s",&LObj[i].Target[4]);
		}
}
//cette fonction cherche la courbe objectif à détruire
// cette courbe est détectée par lecture des cases à cocher

int Heat_ReadObjToDel()
{
	int ref=-1;
	APPELHS("Recupere,DEL");
	ref = HsEntier -1;

	if (ref==-1)
		{
		*init= INITIAL;
		vue = ERROR_WINDOW;
#ifdef DBG_VUE
		fstream f;
		f.open("vue.txt", ios::out);
		f << vue << "DelH" ;
		f.close();
#endif
		error = 103;
		Window_Error();
		}
	return(ref);
}

//Transfert de la base de données vers la structure
//**************************************************
void Heat_LoadData(int Index)
{
	int status; //status d'ouverture
	int erreur = 1; // si pb de lecture  erreur = 0

  //ouverture de la base de données en lecture
	status = DBopen(DB_LstObj_Item,0,"");

  //lecture du nombre d'enregistrements
	Heat_NbRecord = DBNbRecord(DB_LstObj_Item);

	if (!status)
		{
	 // chargement des données modèle de la database
		for (int i=0;i<NBCOL && i<(Heat_NbRecord-Index);i++)
			{
			erreur *= LireDBFloat(DB_LstObj_Item,(i+Index),DB_LObj_Prod,LObj[i].Prod);
			erreur *= LireDBInt(DB_LstObj_Item,(i+Index),DB_LObj_PType,LObj[i].PType);
			erreur *= LireDBInt(DB_LstObj_Item,(i+Index),DB_LObj_ChT,LObj[i].ChT);

			for (int j=0;j<NBZONE;j++)
				erreur *= LireDBInt(DB_LstObj_Item,(i+Index),(DB_LObj_Target+j*sizeof(int)),LObj[i].Target[j]);
			}

		if (!erreur)
			{
			File_Error(100,DB_LstObj_Name);
			Heat_LoadFile();
			}  // MsgMod2="Reading Database Error"

		//fermeture de la database
		DBclose(DB_LstObj_Item);

		//sauvegarde de la base de données dans un buffer
		memcpy(Heat_Buf,&LObj,sizeof(LObj));
		}

	else
		{
		File_Error(101,DB_LstObj_Name);
		Heat_LoadFile();
		} // "Open File Error"
}


//sauvegarde de la structure dans la base de données
//**************************************************
void Heat_StoreData(int Index)
{
	int status; //status d'ouverture
	int erreur = 1; // si pb de lecture  erreur = 0

	//ouverture de la base de données en ecriture
	status = DBopen(DB_LstObj_Item,1,"");

	if (!status)
		{
	// sauvegarde des données ds database
		int NbRecordinDB = DBNbRecord(DB_LstObj_Item);
		for (int i=0;(i<NBCOL && (strcmp(LObj[i].Prod,"0")));i++)
			{
			if (i >= NbRecordinDB)
				{
				DBclose(DB_LstObj_Item);
				DBinsert(DB_LstObj_Item,(i+Index),"");
				DBopen(DB_LstObj_Item,1,"");
				}
			erreur *= DBwriteFloat(DB_LstObj_Item,(i+Index),DB_LObj_Prod,LObj[i].Prod);
			erreur *= DBwriteInt(DB_LstObj_Item,(i+Index),DB_LObj_PType,LObj[i].PType);
			erreur *= DBwriteInt(DB_LstObj_Item,(i+Index),DB_LObj_ChT, LObj[i].ChT);

			for (int j=0;j<NBZONE;j++)
				erreur *= DBwriteInt(DB_LstObj_Item,(i+Index),
									(DB_LObj_Target+j*sizeof(int)),LObj[i].Target[j]);
			}
		//		remplissage des valeurs non utilisées

		//		for (int j=0;j<NbZonesMax;j++)
		//			erreur *= DBwriteInt(DB_LstObj_Item,(i+Index),
		//							    (DB_LObj_Target+j*sizeof(int)),"0");

		if (!erreur)
			File_Error(100,DB_LstObj_Name); //"Reading Database Error"

		//fermeture de la database
		DBclose(DB_LstObj_Item);

		status = DBopen(DB_LstObj_Item,1,"");

		//lecture du nombre d'enregistrements

		if (!status)
			Heat_NbRecord = DBNbRecord(DB_LstObj_Item);
		else
			File_Error(101,DB_LstObj_Name); // "File Opening Error"

		//fermeture de la database
		DBclose(DB_LstObj_Item);
		}
	else
		File_Error(101,DB_TypProd_Name); // "File Opening Error"
}

//affecte la valeur de num de façon croissante
void AffecteNum()
{
	int erreur=1;
	char Zero = 0;
	int status = DBopen(DB_LstObj_Item,2,Path);
	if (!status)
		{
		Heat_NbRecord = DBNbRecord(DB_LstObj_Item);
		for (int i=0;i<Heat_NbRecord;i++)
			{
			erreur *= DBwrite(DB_LstObj_Item,i,DB_LObj_Num,(char *)&i,sizeof(int));
			erreur *= DBwrite(DB_LstObj_Item,i,DB_LObj_Active,(char *)&Zero,sizeof(char));
			}
		DBclose(DB_LstObj_Item);
		if (!erreur)
			File_Error(100,DB_LstObj_Name); //"Reading Database Error"
		}
	else
		File_Error(101,DB_TypProd_Name); // "File Opening Error"
}

//trie des courbes objectifs (Mèthode: trie à bulles)
int Heat_TriBulle()
{
	struct
		{
		float Prod;
		int PType;
		int ChT;
		} Obj[2];

	int erreur=1;
	int Quit = 0;
	int Test=0;

	int status = DBopen(DB_LstObj_Item,2,Path);

	if (!status)
		{
		Heat_NbRecord = DBNbRecord(DB_LstObj_Item);
		int n=Heat_NbRecord-1;
		while(n>=1 && !Quit)
			{
			//lecture des 2 blocks de données
			for (int i=0;i<2;i++)
				{
				erreur *= LireDB(DB_LstObj_Item,(n-i),DB_LObj_Prod,(char *)&Obj[i].Prod,sizeof(float));
				erreur *= LireDB(DB_LstObj_Item,(n-i),DB_LObj_PType,(char *)&Obj[i].PType,sizeof(int));
				erreur *= LireDB(DB_LstObj_Item,(n-i),DB_LObj_ChT,(char *)&Obj[i].ChT,sizeof(int));
				}

			//test de duplicité
			if ( (Obj[0].Prod ==Obj[1].Prod) && (Obj[0].PType==Obj[1].PType) && (Obj[0].ChT==Obj[1].ChT))
				Quit=1;
//			}
			//test d'infériorité
			if ( Obj[0].Prod < Obj[1].Prod )
				{
				Heat_Permute(n,n-1);
				Test=1;
				}
			else
			if ( (Obj[0].Prod == Obj[1].Prod) && (Obj[0].PType < Obj[1].PType) )
				{
				Heat_Permute(n,n-1);
				Test=1;
				}
			else
			if ( (Obj[0].Prod == Obj[1].Prod) && (Obj[0].PType == Obj[1].PType)
					&& (Obj[0].ChT < Obj[1].ChT) )
				{
				Heat_Permute(n,n-1);
				Test=1;
				}
			n=n-1;

			//on effectue la boucle tant qu'il y a des permutations
			if ((!n) && (Test) && !Quit)
				{
				n = Heat_NbRecord-1;
				Test=0;
				}

			} //end while
		//fermeture de la database
		DBclose(DB_LstObj_Item);
		if (!Quit) return(-1);
		else return (n);
		}
	else
		File_Error(101,DB_TypProd_Name); // "File Opening Error"
	return(-2);
}

//permutation de 2 courbes objectifs
int Heat_Permute(int n1, int n2)
{
	char *Buffer1; // Buffer fichier.
	char *Buffer2;
	int status=1;

	// alloue la memoire pour le buffer
	Buffer1 = new char [DB_LstObj_Size] ;
	Buffer2 = new char [DB_LstObj_Size] ;

	// chargement du buffer1
	status *= LireDB ( DB_LstObj_Item , n1 , 0 , Buffer1 , DB_LstObj_Size ) ;
	status *= LireDB ( DB_LstObj_Item , n2 , 0 , Buffer2 , DB_LstObj_Size ) ;

	status *= DBwrite ( DB_LstObj_Item , n2 , 0 , Buffer1 , DB_LstObj_Size ) ;
	status *= DBwrite ( DB_LstObj_Item , n1 , 0 , Buffer2 , DB_LstObj_Size ) ;

	delete Buffer1;
	delete Buffer2;

	return (status);
}

//sauvegarde de la structure dans le fichier de configuration
// *.ref
void Heat_SaveInFile()
{
	TypeLObj CompLObj;

	int HasToSave;
	//sauvegarde les données de la vue de configuration dans la structure
	Heat_FillBuffers();

	//sauvegarde de la structure dans la base de données
	Heat_StoreData(Heat_IndexList);

	int res=Heat_TriBulle();

	if (res==-1)
		{
		AffecteNum();
		HasToSave= TRUE;
		}
	else
		{
		if (res>=0)
			{
			Heat_IndexList=res;
			Heat_Init();
			Heat_LoadData(Heat_IndexList);
			Heat_Affiche();

			error = 105;      // "2 Products have the same type number"
			*init = INITIAL;
			vue   = ERROR_WINDOW;
#ifdef DBG_VUE
			fstream f;
			f.open("vue.txt", ios::out);
			f << vue;
			f.close();
#endif
			Window_Error();
			}
	HasToSave= FALSE;
		}

	if (HasToSave)
		{
		int erreur=1;
		//remplissage de la structure par les paramètres de configuration
		int status = DBopen(DB_LstObj_Item,0,Path);
		char afile[13];
		(void)strcpy (afile,NomApplication);
		(void)strcat(afile,".REF");
		ofstream HtCv;
		HtCv.open(afile,ios::out|ios::trunc);

		if (!status && HtCv)
			{
			Heat_NbRecord = DBNbRecord(DB_LstObj_Item);

			HtCv <<"            Reheating Objective List:" <<endl;
			char *azt = "TZ0";
			HtCv << "Prod.\tType\tChT\t";

			for (int i=0; i<NBZONE;i++)
				{
				azt[2] = (char)(48+i);
				HtCv << azt << "\t";
				}
			HtCv << endl;

			for (i=0;i<Heat_NbRecord && erreur;i++)
				{
				//lecture de la base de données
//				erreur*= LireDBInt(DB_LstObj_Item,i,DB_LObj_Num,CompLObj.Num);
				erreur*= LireDBFloat(DB_LstObj_Item,i,DB_LObj_Prod,CompLObj.Prod);
				erreur*= LireDBInt(DB_LstObj_Item,i,DB_LObj_PType,CompLObj.PType);
				erreur*= LireDBInt(DB_LstObj_Item,i,DB_LObj_ChT,CompLObj.ChT);
//				erreur*= LireDBChar(DB_LstObj_Item,i,DB_LObj_Active,CompLObj.Active);

				for (int j=0;j<NBZONE;j++)
					erreur*= LireDBInt(DB_LstObj_Item,i,(DB_LObj_Target+j*sizeof(int)),
												CompLObj.Target[j]);

				HtCv << CompLObj.Prod << "\t"
				<< CompLObj.PType << "\t"
				<< CompLObj.ChT << "\t";		// Charging Temp
//
				for (int k=0;k<NBZONE;k++)
					HtCv << CompLObj.Target[k] << "\t";

				HtCv << endl;
				}
			HtCv.close();
			DBclose(DB_LstObj_Item);

			if (!erreur)
				File_Error(100,DB_LstObj_Name); // "Reading Database Error"
			}
		else
			File_Error(101,DB_LstObj_Name); // "File Opening Error"
		}
	else
			File_Error(104,DB_LstObj_Name); // "File Savin Error"
}
void Heat_LoadFile()
{
	FILE *fp;
	char afile[13];
	char Texte[8][80];
	strcpy (afile,NomApplication);
	strcat(afile,".REF");

	fp = fopen(afile,"rt+");

	if (feof(fp)==0)
		{
		fscanf(fp, "%s %s %s",&Texte[0], &Texte[1],Texte[2]);
		fscanf(fp, "%s %s %s %s %s %s %s %s",&Texte[0], &Texte[1],Texte[2],&Texte[3],&Texte[4],&Texte[5],&Texte[6],&Texte[7]);

		for(int i = 0;feof(fp)==0;i++)
			fscanf(fp, "%s %s %s %s %s %s %s %s ",&LObj[i].Prod ,&LObj[i].PType,&LObj[i].ChT,
				&LObj[i].Target[0],&LObj[i].Target[1],&LObj[i].Target[2],&LObj[i].Target[3],&LObj[i].Target[4]);

		}
	fclose(fp);

	memcpy(Heat_Buf,&LObj,sizeof(LObj));
}
