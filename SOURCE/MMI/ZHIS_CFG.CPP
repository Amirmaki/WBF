 #define __OWNER_ZHIS_CFG_H

 #ifndef __CONFIG_H
 #include "config.h"
 #endif
 #ifndef __LANGUAGE_H
 #include "language.h"
 #endif
 #ifndef __ZHIS_H
 #include "zhis_cfg.h"
 #endif
 #include "V10data1.h"

 #include <fstream.h>

void ZHis_Config()
{
	if ( *init == INITIAL)
	{
		Temp=3;	//Utile pour message d'erreur
		*init=NORMAL;
		APPELHS ("Utilise,zhis_cfg.AID");
		APPELHS ("Option,Couleur,S=E");    // couleur saisie (fond*16+caractere)
		APPELHS ("Option,Couleur,A=E");
		APPELHS ("Fusionne,logo_cc.img,1,2");
		APPELHS ("Ecran,Premier,CHOIX     ,1");

		APPELHS ("Sortie,ESC=V,RET=V,TF1=V,TF2=V,TF4=V,TF0=V");
		DispCurves_Init();

	}
	APPELHS("Ecran,Saisie/nonblocant");

	if(!strcmp(HsTouche,"TF0") || !strcmp(HsTouche,"TF4"))
		Sortie_Ecran(HsTouche);

	if(!strcmp(HsTouche,"TF1"))
	{
		*init=INITIAL;
		fstream f;
		f.open("vue.txt", ios::in);
		f >> vue;
		f.close();
		Vue_Precedente=vue;
		vue = HELP_SCREEN;
		f.open("vue.txt", ios::out);
		f << vue;
		f.close();
	}

	if(!strcmp(HsTouche,"TF2"))
		DispCurves_FillBuffer();
}

void DispCurves_Init()
{
	APPELHS("Affiche,CHOIX,1=%c",DispCurves.Recup[0]);

	int j=2;

	for (int i= 0;i<4;i++)
	{

		APPELHS("Affiche,CHOIX,%d=%c",j,DispCurves.PV[i]);
		APPELHS("Affiche,CHOIX,%d=%c",j+1,DispCurves.SP[i]);
		APPELHS("Affiche,CHOIX,%d=%c",j+2,DispCurves.Qa[i]);
		APPELHS("Affiche,CHOIX,%d=%c",j+3,DispCurves.Qg[i]);
		j+=4;
	}
}

void DispCurves_FillBuffer()
{
	char Tab[17];
	int Compteur=0;

	for (int i= 0;i<17;i++)
	{
		APPELHS("Ecran,Recupere,CHOIX ,%d",i+1);
		sscanf(HsChaine,"%s", &Tab[i]);

		if(!strcmp(HsChaine,"O"))
			Compteur++;
	}

	if(Compteur > 6)
	{
		error = 110;         // "You can select only SIX curves"
		*init = INITIAL;
		vue   = ERROR_WINDOW;
		fstream f;
		f.open("vue.txt", ios::out);
		f << vue;
		f.close();
		Window_Error();
	}
	else
	{
		int j=0;

		DispCurves.Recup[0]=Tab[0];

		for(int i=1;i<17;i+=4)
		{
			DispCurves.PV[j]=Tab[i];
			DispCurves.SP[j]=Tab[i+1];
			DispCurves.Qa[j]=Tab[i+2];
			DispCurves.Qg[j]=Tab[i+3];
			j++;
		}
		DispCurves_SaveData();
	}
}

void DispCurves_SaveData()
{
	FILE *fp;
	fp=fopen("Curves.cfg","w+");
	fprintf (fp,"Recup: %c\n",DispCurves.Recup[0]);

	fprintf (fp,"\nPV:\n");

	for(int i=0; i<4;i++)
		fprintf (fp,"Z%d: %c\n",i+1,DispCurves.PV[i]);

	fprintf (fp,"\nSP:\n");

	for(i=0; i<4;i++)
		fprintf (fp,"Z%d: %c\n",i+1,DispCurves.SP[i]);

	fprintf (fp,"\nQa:\n");

	for(i=0; i<4;i++)
		fprintf (fp,"Z%d: %c\n",i+1,DispCurves.Qa[i]);

	fprintf (fp,"\nQg:\n");

	for(i=0; i<4;i++)
		fprintf (fp,"Z%d: %c\n",i+1,DispCurves.Qg[i]);

	fclose(fp);
}

void DispCurves_LoadData()
{
	FILE *fp;
	char Temp[20];
	int j=0;

	fp=fopen("Curves.cfg","r");

	fscanf (fp,"%s",&Temp);
	fscanf (fp,"%s",&DispCurves.Recup);

	if (!strcmp(&DispCurves.Recup[0],"O"))
	{
		V10DispZCurveNb[0]=j;
		V10DispZCi[j] = 1;
		j++;
	}
	fscanf (fp,"%s",Temp);

	for(int i=0; i<4;i++)
	{
		fscanf (fp,"%s",Temp);
		fscanf (fp,"%s\n",&DispCurves.PV[i]);

		if (!strcmp(&DispCurves.PV[i],"O"))
		{
			V10DispZCurveNb[i+1]=j;
			V10DispZCi[j] = 1;
			j++;
		}
	}
	fscanf (fp,"%s",Temp);

	for(i=0; i<4;i++)
	{
		fscanf (fp,"%s",Temp);
		fscanf (fp,"%s\n",&DispCurves.SP[i]);

		if (!strcmp(&DispCurves.SP[i],"O"))
		{
			V10DispZCurveNb[maxNbZone+2+i]=j;
			V10DispZCi[j] = 1;
			j++;
		}
	}
	fscanf (fp,"%s",Temp);

	for(i=0; i<4;i++)
	{
		fscanf (fp,"%s",Temp);
		fscanf (fp,"%s\n",&DispCurves.Qa[i]);

		if (!strcmp(&DispCurves.Qa[i],"O"))
		{
			V10DispZCurveNb[3*maxNbZone+2+i]=j;
			V10DispZCi[j] = 1;
			j++;
		}
	}
	fscanf (fp,"%s",Temp);

	for(i=0; i<4;i++)
	{
		fscanf (fp,"%s",Temp);
		fscanf (fp,"%s\n",&DispCurves.Qg[i]);

		if (!strcmp(&DispCurves.Qg[i],"O"))
		{
			V10DispZCurveNb[2*maxNbZone+2+i]=j;
			V10DispZCi[j] = 1;
			j++;
		}
	}
	fclose(fp);

	for(j;j<6;j++)
		V10DispZCi[j]=0;
}
