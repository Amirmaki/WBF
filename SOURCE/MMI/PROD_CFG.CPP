 #define __OWNER_TPDTWIND_H
 #define __OWNER_PRODCFG_H
 #ifndef __TPDTWIND_H
 #include "tpdtwind.h"
 #endif
 #ifndef __WINSTRUC_H
 #include "winstruc.h"
 #endif
 #ifndef __DB_H
 #include <db.h>
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

void Products_Config()
{
	if ( *init == INITIAL)
	{
		*init = NORMAL;
		Temp = 1;  // Diff‚rencie la vue Product Config de la vue Heat Curves
					  // lorsque deux courbes sont identiques.

		Vue_Precedente=CONFIG_MENU;
		APPELHS ("Utilise,PRODCFG.AID");
		APPELHS ("Fusionne,logo_cc.img,1,2");
		APPELHS ("Fusionne,prd_titl.img,1,26");
		APPELHS ("Fusionne,prod.img,3,9");

		APPELHS ("Option,Couleur,S=240");    // couleur saisie (fond*16+caractere)
		APPELHS ("Option,Couleur,A=240");
		APPELHS ("Option,verticale,N");
		APPELHS ("Sortie,ESC=V,RET=V,HOM=V,END=V,TAB=S,TF1=V,TF2=V,TF4=V,TF6=V,TF7=V,TF8=V,TF0=V");

		Prod_Buf=new char[sizeof(TProd)];
		Prod_InitWindow();
		Prod_SetupWindow();
	}
	APPELHS ("Ecran,Saisie/nonblocant");

	if (!strcmp(HsTouche,"FLB"))
		Down();

	if (!strcmp(HsTouche,"FLH"))
		Up();

	if(!strcmp(HsTouche,"TF1"))
	{
		*init=INITIAL;
#ifdef DBG_VUE
		fstream f;
/*		f.open("vue.txt", ios::in);
		f >> vue;
		f.close();
*/
		f.open("vue.txt", ios::out|ios::app);
		f << vue << endl;
		f.close();
#endif
		Vue_Precedente=vue;
		vue = HELP_SCREEN;
#ifdef DBG_VUE
		f.open("vue.txt", ios::out|ios::app);
		f << vue << endl;
		f.close();
#endif
	}

	if (!strcmp(HsTouche,"TF6"))
		Del();

	if (!strcmp(HsTouche,"TF7"))
		PgUp();

	if (!strcmp(HsTouche,"TF8"))
		PgDown();

	if (!strcmp(HsTouche,"END"))
		End();

	if (!strcmp(HsTouche,"HOM"))
		Home();

	if (!strcmp(HsTouche,"TF2"))
		Teste_Buffer(HsTouche);

	if(!strcmp(HsTouche,"TF0") || !strcmp(HsTouche,"TF4"))
		Teste_Buffer(HsTouche);
}

void Prod_InitWindow()
{
	Prod_IndexList=0;
	Init();
	Prod_LoadData(Prod_IndexList);
	//  TransferBuffer = (void far*)&(TProd);
}

 //initialisation  des données dans la strcture
 //********************************************
void Init()
{
	for (int i=0;i<NBCOL;i++)
	{
		strcpy(TProd[i].Num,"0");
		strcpy(TProd[i].Quality,"0");
		strcpy(TProd[i].Width,"0");
		strcpy(TProd[i].Thick,"0");
		strcpy(TProd[i].ReqDisT,"0");
		strcpy(TProd[i].Code,"0");
	//    strcpy(TProd[i].DPas,"0");
	//DPas[i] = 0;
	}
}

void Prod_SetupWindow()
{
	Affiche();
}

//gestion des déplacements dans le fichier
//***************************************

void Home()
{
	//sauvegarde les données de la vue de configuration dans la structure
	Prod_FillBuffers();

	//sauvegarde de la structure dans la base de données
	Prod_StoreData(Prod_IndexList);

	Prod_IndexList= 0;
	Init();
	Prod_LoadData(Prod_IndexList);
	Affiche();
	Unselect();
}

void End()
{

	//sauvegarde les données de la vue de configuration dans la structure
	Prod_FillBuffers();

	//sauvegarde de la structure dans la base de données
	Prod_StoreData(Prod_IndexList);

	Prod_IndexList= Prod_NbRecord-1;
	Init();
	Prod_LoadData(Prod_IndexList);
	Affiche();
	Unselect();
}

void Up()
{
	Prod_FillBuffers();
	Prod_StoreData(Prod_IndexList);
	APPELHS("Ecran,Courant");

	if(Prod_IndexList > 0)
	{
		Init();
		Prod_LoadData(--Prod_IndexList);
		Affiche();
		Unselect();
	}
	else if(HsIndice > 1)
		APPELHS("Ecran,Premier,%s,%d",HsNom,HsIndice-1);
}

void PgUp()
{
	//sauvegarde les données de la vue de configuration dans la structure
	Prod_FillBuffers();

	//sauvegarde de la structure dans la base de données
	Prod_StoreData(Prod_IndexList);

	if (Prod_IndexList > NBCOL)
		Prod_IndexList -= NBCOL;
	else
		Prod_IndexList = 0;

	Init();
	Prod_LoadData(Prod_IndexList);
	Affiche();
	Unselect();
}

void Down()
{
	Prod_FillBuffers();
	Prod_StoreData(Prod_IndexList);

	APPELHS("Ecran,Courant");

	if(Prod_NbRecord+1 > NBCOL)
	{
		Init();
		Prod_LoadData(++Prod_IndexList);
		Affiche();
		Unselect();
	}
	else if(HsIndice < NBCOL)
		APPELHS("Ecran,Premier,%s,%d",HsNom,HsIndice+1);
}

void PgDown()
{
	//sauvegarde les données de la vue de configuration dans la structure
	Prod_FillBuffers();

	//sauvegarde de la structure dans la base de données
	Prod_StoreData(Prod_IndexList);

	if (Prod_IndexList < (Prod_NbRecord-NBCOL) && Prod_NbRecord > NBCOL)
		Prod_IndexList += NBCOL;

	else if(Prod_NbRecord > NBCOL)
		Prod_IndexList = Prod_NbRecord - 1;

	Init();
	Prod_LoadData(Prod_IndexList);
	Affiche();
	Unselect();
}

void Del()
{
	Prod_FillBuffers();
	Prod_StoreData(Prod_IndexList);
	int TheIndexProd = ReadObjToDel();

	if (TheIndexProd!=-1 && Prod_NbRecord >0)
	{
		//sauvegarde les données de la vue de configuration dans la structure
		Prod_FillBuffers();
		//sauvegarde de la structure dans la base de données
		Prod_StoreData(Prod_IndexList);

		int status = DBdelete(DB_TypProd_Item,TheIndexProd,Path);

		//traitement du status
		if (status <0)
		{
			char aux[40];
			char val[3];
			itoa(status,val,10);
			File_Error(103,DB_TypProd_Name);
		}
		if (Prod_IndexList == Prod_NbRecord-1)
			if (Prod_IndexList > 0)
				Prod_IndexList = Prod_NbRecord - 2;
			else
				Prod_IndexList=0;

		Init();
		Prod_LoadData(Prod_IndexList);
		Affiche();
	}
}
//désélectionne les cases à cocher
void Unselect()
{
	APPELHS("Affiche,DEL=0");
}

//fonction d'affichage des données à l'écran pour remise à jour
// et déplacement

void Affiche()
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

//sauvegarde les données de la vue de configuration dans la structure
//*******************************************************************
void Prod_FillBuffers()
{
	for (int i=0;i<NBCOL;i++)
	{
		APPELHS("Ecran,Recupere,NUM,%d=",i+1);
		sscanf(HsChaine,"%s",&TProd[i].Num);

		APPELHS("Ecran,Recupere,GROUP,%d=",i+1);
		sscanf(HsChaine,"%s",&TProd[i].Quality);

		APPELHS("Ecran,Recupere,WIDTH,%d=",i+1);
		sscanf(HsChaine,"%s",&TProd[i].Width);

		APPELHS("Ecran,Recupere,HEIGHT,%d=",i+1);
		sscanf(HsChaine,"%s",&TProd[i].Thick);

		APPELHS("Ecran,Recupere,DISCH,%d=",i+1);
		sscanf(HsChaine,"%s",&TProd[i].ReqDisT);

		APPELHS("Ecran,Recupere,CODE,%d=",i+1);
		sscanf(HsChaine,"%s",&TProd[i].Code);

//		 if (::IsDlgButtonChecked(HWindow,ID_TProd_DPas0+i)==0)
//			strcpy(TProd[i].DPas,"0");
//		 else strcpy(TProd[i].DPas,"1");
	}
}

//cette fonction cherche la courbe objectif à détruire
// cette courbe est détectée par lecture des cases à cocher

int ReadObjToDel()
{
	int ref = -1;

	APPELHS("Recupere,DEL");
	ref = HsEntier -1;

	if (ref==-1)
	{
		*init = INITIAL;
		vue = ERROR_WINDOW;
#ifdef DBG_VUE
		fstream f;
		f.open("vue.txt", ios::out|ios::app);
		f << vue << endl;
		f.close();
#endif
		error = 103;
		Window_Error();
	}
	return(ref);
}

//Transfert de la base de données vers la structure
//**************************************************

void Prod_LoadData(int Index)
{
	int status; //status d'ouverture
	int erreur = 1; // si pb de lecture  erreur = 0

	//ouverture de la base de données en lecture
	status = DBopen(DB_TypProd_Item,0,"");//DB_path);

	//lecture du nombre d'enregistrements
	Prod_NbRecord = DBNbRecord(DB_TypProd_Item);

	if (!status)
	{
	// chargement des données modèle de la database
		for (int i=0 ;i<NBCOL && i<(Prod_NbRecord-Index);i++)
		{
			erreur *= LireDBInt(DB_TypProd_Item,(i+Index),DB_TProd_Num,TProd[i].Num);
			erreur *= LireDB(DB_TypProd_Item,(i+Index),DB_TProd_Quality,(char *) &TProd[i].Quality,sizeof(TProd[i].Quality));
			erreur *= LireDBInt(DB_TypProd_Item,(i+Index),DB_TProd_Width,TProd[i].Width);
			erreur *= LireDBInt(DB_TypProd_Item,(i+Index),DB_TProd_Thick,TProd[i].Thick);
			erreur *= LireDBInt(DB_TypProd_Item,(i+Index),DB_TProd_ReqDisT,TProd[i].ReqDisT);
			erreur *= LireDBChar(DB_TypProd_Item,(i+Index),DB_TProd_Code,TProd[i].Code);
		//      erreur *= LireDBChar(DB_TypProd_Item,(i+Index),DB_TProd_DPas,TProd[i].DPas);
		}
		if (!erreur)
		{
			File_Error(100,DB_TypProd_Name);
			Prod_LoadFile();
		}  // MsgMod2="Reading Database Error"

		//fermeture de la database
		DBclose(DB_TypProd_Item);
		//sauvegarde de la base de données dans un buffer
		memcpy(Prod_Buf,&TProd,sizeof(TProd));
	}
	else
	{
		File_Error(101,DB_TypProd_Name);
		Prod_LoadFile();
	} // "Open File Error"
}

//sauvegarde de la structure dans la base de données
//**************************************************
void Prod_StoreData(int Index)
{
	int status; //status d'ouverture
	int erreur = 1; // si pb de lecture  erreur = 0
	char Un = 1;
	  //chemin de direction où se trouve la database
	char *DB_path ="";//Path;

	//ouverture de la base de données en lecture
	status = DBopen(DB_TypProd_Item,1,DB_path);
	if (!status)
	{
		// chargement des données modèle de la database
		for (int i=0;(i<NBCOL && (strcmp(TProd[i].Width,"0")));i++)
		{
			char auxc;
			erreur *= DBwriteInt(DB_TypProd_Item,(i+Index),DB_TProd_Num,TProd[i].Num);
			erreur *= DBwrite(DB_TypProd_Item,(i+Index),DB_TProd_Quality,(char *) &TProd[i].Quality,sizeof(TProd[i].Quality));
			erreur *= DBwriteInt(DB_TypProd_Item,(i+Index),DB_TProd_Width,TProd[i].Width);
			erreur *= DBwriteInt(DB_TypProd_Item,(i+Index),DB_TProd_Thick,TProd[i].Thick);
			erreur *= DBwriteInt(DB_TypProd_Item,(i+Index),DB_TProd_ReqDisT,TProd[i].ReqDisT);
			erreur *= DBwriteChar(DB_TypProd_Item,(i+Index),DB_TProd_Code,TProd[i].Code);
			erreur *= DBwrite(DB_TypProd_Item,(i+Index),DB_TProd_NbProd,(char *) &Un, sizeof(char));
		 //	    if (::IsDlgButtonChecked(HWindow,ID_TProd_DPas0+i)==0) auxc = 0;
		 //            else auxc = 1;
		 //    erreur *= DBwrite(DB_TypProd_Item,(i+Index),DB_TProd_DPas,&auxc,
//					sizeof(char));
		 }
		if (!erreur)
			File_Error(100,DB_TypProd_Name); //"Reading Database Error"

		 //fermeture de la database
		 DBclose(DB_TypProd_Item);
		 status = DBopen(DB_TypProd_Item,1,DB_path);

		 //lecture du nombre d'enregistrements
		 if (!status)
			Prod_NbRecord = DBNbRecord(DB_TypProd_Item);
		 else
			File_Error(101,DB_TypProd_Name); // "File Opening Error"

		 //fermeture de la database
		 DBclose(DB_TypProd_Item);
	}
	else
		File_Error(101,DB_TypProd_Name); // "File Opening Error"
}

//trie des courbes objectifs (Mèthode: trie à bulles)
int TriBulle()
{
	int Num[2];
	int erreur=1;
	int Quit = 0;
	int Test = 0;

	int status = DBopen(DB_TypProd_Item,2,Path);

	if (!status)
	{
		Prod_NbRecord = DBNbRecord(DB_TypProd_Item);
		int n=Prod_NbRecord-1;
		while(n>=1 && !Quit)
	       {
			 //lecture des 2 blocks de données
			for (int i=0;i<2;i++)
			{
				erreur *= LireDB(DB_TypProd_Item,(n-i),DB_TProd_Num,(char *)&Num[i],sizeof(int));
			}

			//test de duplicité
			if ( Num[0]==Num[1])
			{
				Quit=1;
			}
			//test d'infériorité
			if ( Num[0]< Num[1] )
			{
				Permute(n,n-1);
				Test=1;
			}
			n=n-1;

			//on effectue la boucle tant qu'il y a des permutations
			if ((!n) && (Test) && !Quit)
			{
				n = Prod_NbRecord-1;
				Test=0;
			}
		} //end while
		//fermeture de la database
		DBclose(DB_TypProd_Item);
		if (!Quit) return(-1);
		else return (n);
	}
	else
		File_Error(101,DB_TypProd_Name); // "File Opening Error"

	return(-2);
}

//permutation de 2 courbes objectifs
int Permute(int n1, int n2)
{
	char *Buffer1; // Buffer fichier.
	char *Buffer2;
	int status=1;

	// alloue la memoire pour le buffer
	Buffer1 = new char [DB_TypProd_Size] ;
	Buffer2 = new char [DB_TypProd_Size] ;

	// chargement du buffer1
	status *= LireDB ( DB_TypProd_Item , n1 , 0 , Buffer1 , DB_TypProd_Size ) ;
	status *= LireDB ( DB_TypProd_Item , n2 , 0 , Buffer2 , DB_TypProd_Size ) ;

	status *= DBwrite ( DB_TypProd_Item , n2 , 0 , Buffer1 , DB_TypProd_Size ) ;
	status *= DBwrite ( DB_TypProd_Item , n1 , 0 , Buffer2 , DB_TypProd_Size ) ;

	delete Buffer1;
	delete Buffer2;

	return (status);
}

//sauvegarde de la structure dans le fichier de configuration
	//*.cfg
void Prod_SaveInFile()
{
	int HasToSave;
	//sauvegarde les données de la vue de configuration dans la structure

	 //sauvegarde les données de la vue de configuration dans la structure
	Prod_FillBuffers();

	//sauvegarde de la structure dans la base de données
	Prod_StoreData(Prod_IndexList);

	int res=TriBulle();

	if (res==-1)
		HasToSave = TRUE;
	else
	{
		if (res>=0)
		{
			Prod_IndexList=res;
			Init();
			Prod_LoadData(Prod_IndexList);
			Affiche();

			error = 105;      // "2 Products have the same type number"
			*init = INITIAL;
			vue   = ERROR_WINDOW;
#ifdef DBG_VUE
			fstream f;
			f.open("vue.txt", ios::out|ios::app);
			f << vue << endl;
			f.close();
#endif
			Window_Error();
		}
		HasToSave= FALSE;
	}

	if ( HasToSave)
	{
		int erreur=1;
		//remplissage de la structure par les paramètres de configuration
		int status = DBopen(DB_TypProd_Item,0,Path);
		char afile[13];
		(void)strcpy (afile,NomApplication);
		(void)strcat(afile,".PDT");
		ofstream TpDt;
		TpDt.open(afile,ios::out|ios::app|ios::trunc);

		if (!status && TpDt)
		{
			Prod_NbRecord = DBNbRecord(DB_TypProd_Item);
			TpDt <<"Number_of_product_types: " << Prod_NbRecord <<endl;
			TpDt << "Num\tWidth\tLength\tThick\tCode\tProd\tReqTemp\tProd/Step\tQuality"<< endl;
			for (int i=0;i<Prod_NbRecord && erreur;i++)
			{
			 //lecture de la base de données

				erreur *= LireDBInt(DB_TypProd_Item,(i),DB_TProd_Num,CompTProd.Num);
				erreur *= LireDB(DB_TypProd_Item,(i),DB_TProd_Quality,(char *) &CompTProd.Quality,sizeof(CompTProd.Quality));
				erreur *= LireDBInt(DB_TypProd_Item,(i),DB_TProd_Width,CompTProd.Width);
				erreur *= LireDBInt(DB_TypProd_Item,(i),DB_TProd_Thick,CompTProd.Thick);
				erreur *= LireDBInt(DB_TypProd_Item,(i),DB_TProd_ReqDisT,CompTProd.ReqDisT);
				erreur *= LireDBChar(DB_TypProd_Item,(i),DB_TProd_Code,CompTProd.Code);
			//      erreur *= LireDBChar(DB_TypProd_Item,(i),DB_TProd_DPas,CompTProd.DPas);
				TpDt <<  CompTProd.Num << "\t";
				TpDt <<  CompTProd.Width << "\t";
				TpDt <<  "4000"  << "\t";
				TpDt <<  CompTProd.Thick << "\t";
				TpDt <<  CompTProd.Code << "\t";
				TpDt <<  "0"  << "\t";
				TpDt <<  CompTProd.ReqDisT << "\t";
				TpDt <<  "1" << "\t\t";
//				TpDt <<  CompTProd.DPas << "  ";
				TpDt <<  CompTProd.Quality << endl;
			}
		 //			TpDt.close();
			DBclose(DB_TypProd_Item);

			if (!erreur)
					File_Error(100,DB_TypProd_Name); //"Reading Database Error"
		}
		else
				File_Error(100,DB_TypProd_Name); // MsgMod3="Open File Error"
	}
	else
		File_Error(104,DB_TypProd_Name); // "File saving error"
}

void Prod_LoadFile()
{
	char afile[13];
	(void)strcpy (afile,NomApplication);
	(void)strcat(afile,".PDT");
	char Texte[9][80];
	FILE *fp;

	fp =fopen(afile,"rt+");

	if (feof(fp)==0)
	{
		fscanf(fp, "%s %d",&Texte[0], &Prod_NbRecord);
		fscanf(fp, "%s %s %s %s %s %s %s %s %s",&Texte[0],&Texte[1],&Texte[2],
			&Texte[3],&Texte[4],&Texte[5],&Texte[6],&Texte[7],&Texte[8]);

		for(int i = 0; i <Prod_NbRecord;i++)
			fscanf(fp, "%s %s %s %s %s %s %s %s %s",&TProd[i].Num,&TProd[i].Width,&Texte[0],
				&TProd[i].Thick,&TProd[i].Code,&Texte[1],&TProd[i].ReqDisT,&Texte[2],&TProd[i].Quality);
	}
	fclose(fp);
	memcpy(Prod_Buf,&TProd,sizeof(TProd));
}
