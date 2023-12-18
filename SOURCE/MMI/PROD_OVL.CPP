//-------------------------------------------------------------------------
// Prod_OVL1.CPP
// This file contains possible overlay function for project TACT
// display number 11
//
// Owning include file : NONE
//
// Creation : C.Martinet 18/12/92 ASCOMETAL APPLICATION
// Modified :            02/02/93 display the hour
//			 04/03/93 abscissa like V07
//			 18/02/93 color 12 set to 60
//-------------------------------------------------------------------------
#define _OWNER2_Prod_DATA1_H

#ifndef __DIR_H
#include <dir.h>
#endif
#ifndef __ALLOC_H
#include <alloc.h>
#endif
#ifndef __STDLIB_H
#include <stdlib.h>
#endif
#ifndef __PRHISLST_H
#include <prhislst.h>
#endif
#ifndef __GLOBVIEW_H
#include <globview.h>
#endif
#ifndef __PRODDATA_H
#include "Proddata.h"
#endif
#ifndef __DATABASE_H
#define "database.h"
#endif


/*
#ifndef __V03DATA1_H
#include <v03data1.h>
#endif
*/
#include "database.h"
#include "db.h"
#ifndef __CONFIG_H
#include "config.h"
#endif
#ifndef __WINSTRUC_H
#include "winstruc.h"
#endif
//------------------------------------------------------------------------
//function prototypes

//initialisation
void Prod_Init()
{
	struct date d;

	getdate(&d);

	Prod_GMaxTemp   = 1500;
	Prod_GxZero     = 40;
	Prod_GxLarge    = 560;
	Prod_GyZero     = 33;
	Prod_GyHaut     = 464-Top;  //464

	Prod_GyTempMoy = Bottom-40;
	Prod_GyTempInf = Bottom-30;
	Prod_GyTempSurf= Bottom-20;
	Prod_GyTempCore= Bottom-10;

	Prod_NbActPoint = 0;
	Prod_CursorPos  = 0;
	Prod_CursorInc  = 1;
//	Prod_Gpage	   = NULL;
	Prod_NewHispDisp = 1;
	Prod_NextOrPrev  = 0;

	Prod_DispDate[0] = Prod_DispDateAcq[0] =d.da_year;
	Prod_DispDate[1] = Prod_DispDateAcq[1] =d.da_mon;
	Prod_DispDate[2] = Prod_DispDateAcq[2] =d.da_day;

	int status = DBopen(DB_CfgFour_Item,0,"");

	char NbStep = 0 ; 			// nbre de pas pour 1 avance

	if (!status)
		LireDB(DB_CfgFour_Item,0,DB_Four_StepNb,(char *)&NbStep,sizeof(char));

	DBclose(DB_CfgFour_Item);
	Prod_NbPas=NbStep;

	for (int i = 4; i<6; i++)
	{
		Prod_DispDate[i] = Prod_DispDateAcq[i] = 0;
	}

	Prod_curAcqVal = 1;
	Prod_GxDateDis = 70;
	Prod_GxDateAcq = 380;
	Prod_GyDate    = 50;
	Prod_Active    = 0;
	Prod_ProdType  = 0;
	Prod_ProdWidi  = 0;
}//end Prod_Init

//----------------------------------------------------------------------
//display
void Prod_Display()
{
	struct ffblk fb;

	char afile[13];
	(void)strcpy (afile,DB_Path_hisprod);
	(void)strcat(afile,FichListName);

	if(!findfirst(afile,&fb,0))
	{
	 //set mode to graphic mode
		if (VGDispMode == 1)
		{
			setgraphmode(getgraphmode());
			VGDispMode = 0;
		}

//		Prod_tailp = imagesize(0,0,639,19);
//		Prod_Gpage = (void *)new char[Prod_tailp];


	if(*init==INITIAL)
	{

	 //set all palette color
		setpalette(13,61);
		setpalette(6,62);
		setpalette(10,58);
		setpalette(4,59);
		setpalette(3,63);
		setpalette(8,60);
		setpalette(12,60);

	 //read the data in V03 data
		int erreur =1;
		int status = DBopen(DB_CfgFour_Item,0,"");
		float GStep;

		if (!status)
		{
			// chargement des données modèle de la database
			LireDBFloat(DB_CfgFour_Item,0,DB_Four_Length,Four.Length);

			Prod_GFlength = atof(Four.Length);	//V03FurnLength;
			Prod_GNbZone  = NBZONE;      //V03FurnNbZone;

/*		for (int i=0; i<Prod_GNbZone; i++)
	  {
			Prod_GPosZ[i] = V03ZoneAbs[i];
	  }
  */
			for (int i=0;i<NBZONE;i++)
			{
				erreur *=LireDBFloat(DB_CfgFour_Item,0,(DB_Four_EndAbsSup+i*sizeof(float)),Four.EndAbsSup[i]);
				Prod_GPosZ[i] =atof(Four.EndAbsSup[i]);
			}

	 //set cursor data positions (pixel), specific asco93
			erreur *= LireDBFloat(DB_CfgFour_Item,0,DB_Four_Step1,Four.Step1);
			GStep   = atof(Four.Step1);//(float)V03FurnStepLength;

			if (!erreur)  //erreur de lecture
				File_Error(100,DB_CfgFour_Name);

		 //fermeture de la database
			DBclose(DB_CfgFour_Item);
		}

	 //abscissa scaling
		Prod_Gxalpha = ((float)Prod_GxLarge)/((float)Prod_GFlength);
		Prod_Gxbeta  = (float)Prod_GxZero;

	 //ordinate scaling
		Prod_Gyalpha = -1.*((float)Prod_GyHaut)/((float)Prod_GMaxTemp);
		Prod_Gybeta  = (float)(Prod_GyZero+Prod_GyHaut);

	 //float curpos = GStep/2;
		float curpos = 350;

//		for (int icp = 0; icp < 70; icp++ , curpos += GStep)
		for (int icp = 0; icp < Prod_NbPas; icp++ , curpos += GStep)
		{
			Prod_CursorPosm[icp] = (long)curpos;
			Prod_CursorPosp[icp] = (int)(Prod_Gxalpha*curpos +Prod_Gxbeta);
		}

	 //zone pixel position
		for (int xnbgrad = 0;xnbgrad<Prod_GNbZone;xnbgrad++)
		{
			Prod_GPosZp[xnbgrad] = (int)(Prod_Gxalpha*((float)Prod_GPosZ[xnbgrad])
				+ Prod_Gxbeta);
		}

		Prod_NbActPoint = 0;
  }
  //read the time
 /*
		char anhour[6];
		(void)strcpy(anhour,"hhhmn");
		struct time t;
		gettime(&t);
		(void)itoa(t.ti_hour,anhour,10);

		if (anhour[1] == '\0')
		{
			anhour[1]=anhour[0];
			anhour[0]=' ';
		}
		else
			anhour[2] = 'h';

		(void)itoa(t.ti_min,&anhour[3],10);
		if (anhour[4] == '\0')
		{
			anhour[5] = '\0';
			anhour[4] = anhour[3];
			anhour[3] = '0';
		}
*/
//		(void)strcpy(Prod_Hour,anhour);

		Prod_UpdateData();
		Prod_UpdateDisp();
//		VGDispViewNb = 11;
	}
//	else VGNextViewNb = VGDispViewNb; //no file => no display change

}//end Prod_Display

//----------------------------------------------------------------------
//erase
void Prod_Erase()
{
	//free allocated memory
	if (Prod_TxAct != NULL)
	{
		delete Prod_TxAct;
		Prod_TxAct = NULL;
		if (Prod_TMoyAct   != NULL) delete Prod_TMoyAct;
		if (Prod_TSurfAct  != NULL) delete Prod_TSurfAct;
		if (Prod_TInfAct   != NULL) delete Prod_TInfAct;
		if (Prod_TSejAct   != NULL) delete Prod_TSejAct;
		if (Prod_TCoreAct  != NULL) delete Prod_TCoreAct;
		if (Prod_TObjAct   != NULL) delete Prod_TObjAct;
		if (Prod_TTvAct    != NULL) delete Prod_TTvAct;
		if (Prod_TxActp    != NULL) delete Prod_TxActp;
		if (Prod_TMoyActp  != NULL) delete Prod_TMoyActp;
		if (Prod_TSurfActp != NULL) delete Prod_TSurfActp;
		if (Prod_TInfActp  != NULL) delete Prod_TInfActp;
		if (Prod_TSejActp  != NULL) delete Prod_TSejActp;
		if (Prod_TCoreActp != NULL) delete Prod_TCoreActp;
		if (Prod_TObjActp  != NULL) delete Prod_TObjActp;
		if (Prod_TTvActp   != NULL) delete Prod_TTvActp;
		Prod_TObjAct  = Prod_TTvAct = NULL;
		Prod_TObjActp = Prod_TTvActp = Prod_TSejActp = NULL;
	}
	Prod_NbActPoint = 0;
	Prod_Active=0;
	Prod_NewHispDisp=Prod_NextOrPrev=0;

/*
	 if (Prod_Gpage != NULL)
	  {
		delete Prod_Gpage;
		Prod_Gpage = NULL;
	  }
*/
	//erase the screen
//	setactivepage(VGPageVisu);
//	VGPageActive = VGPageVisu;
//	cleardevice();

 }//end Prod_Erase

//----------------------------------------------------------------------
//Update the display with new data
void Prod_UpdateDisp()
{
	if(*init==INITIAL)
	{
		//select active page as non visual page
//		if (VGPageVisu == 0)
//			VGPageActive = 1;
//		else
			VGPageActive = 0;

	setactivepage(VGPageActive);
	//clear the screen
	//cleardevice();
  }

  //display the background
  Prod_DispBackground();

  //display the data

  setfillstyle(SOLID_FILL,1);

	//if the surface temp. curve must be displayed
	if (Prod_NbActPoint )
												//	&& V12DispTS)  VB
	{
		setvisualpage(0);
		setcolor(13);
		moveto(Prod_GxZero+1,Prod_ProdChaTempp);

		for (int np = 0;np < Prod_NbActPoint;
				lineto(Prod_TxActp[np],Prod_TSurfActp[np]),np++);

			moveto(Prod_GxZero+1,Prod_ProdChaTempp);

		for (np = 0;np < Prod_NbActPoint;
				lineto(Prod_TxActp[np],Prod_TInfActp[np]),np++);
	}

	//if the roof temp. curve must be displayed
	if (Prod_NbActPoint)
													// && V12DispTV)   VB
	{
		setcolor(6);
		moveto(Prod_GxZero+1,Prod_TTvActp[0]);

		for (int np = 0;np < Prod_NbActPoint;
			lineto(Prod_TxActp[np],Prod_TTvActp[np]),np++);
	}

	//if the core temp. curve must be displayed
	if (Prod_NbActPoint)
														// && V12DispTC)     VB
	{
		setcolor(4);
		moveto(Prod_GxZero+1,Prod_ProdChaTempp);

		for (int np = 0;np < Prod_NbActPoint;
			lineto(Prod_TxActp[np],Prod_TCoreActp[np]),np++);
	}

	//if the target curve must be displayed
	if (Prod_NbActPoint)
															// && V12DispTarg)   VB
	{
		setcolor(10);
		moveto(Prod_GxZero+1,Prod_TObjActp[0]);

		for (int np = 0;np < Prod_NbActPoint;
			lineto(Prod_TxActp[np],Prod_TObjActp[np]),np++);
	}

	//if the resid time curve must be displayed
	if (Prod_NbActPoint)
														// && V12DispTsej)    VB
	{
		setcolor(8);
		moveto(Prod_GxZero+1,Prod_TSejActp[0]);

		for (int np = 0;np < Prod_NbActPoint;
			lineto(Prod_TxActp[np],Prod_TSejActp[np]),np++);
	}

	//if the mean temp curve must be displayed
	if (Prod_NbActPoint)
																// && V12DispTM)  VB
	{
		setcolor(3);
		moveto(Prod_GxZero+1,Prod_ProdChaTempp);

		for (int np = 0; np < Prod_NbActPoint;
			lineto(Prod_TxActp[np],Prod_TMoyActp[np]),np++);
	}
/*
#ifdef DEBBUGING
	 char memav[60];
	 (void)strcpy(memav,"MemAvail:                                          ");
	 unsigned long memAvail = farcoreleft();
	 (void)ultoa(memAvail,&memav[10],10);
	 settextjustify(CENTER_TEXT,TOP_TEXT);
	 outtextxy(320,50,memav);
#endif
*/
	//update cursor data (will switch to active page)
	Prod_UpdateHPGraphCursor();

	if(*init==INITIAL)
	{
		//update non visual page (for next cursor update)
/*		if (VGPageVisu == 0)
			VGPageActive = 1;
		else
			VGPageActive = 0;

		for (int yline = 0; yline < 349; yline+=20)
		{
			if(yline==0)
			{
	*/			struct palettetype pal;
				getpalette(&pal);
				APPELHS ("Fusionne,prdtitle.img,1,1");
				setallpalette(&pal);
				APPELHS("Option,Couleur,P,7=57");
				APPELHS("Option,Couleur,P,9=7");
				APPELHS ("Fusionne,datehour.aid");
				APPELHS ("Ouvre,PRD_BOUT.AID,22,1");
				Prod_DispDates();

				setactivepage(VGPageVisu);
/*			}
			getimage(0,yline,639,yline+19,Prod_Gpage);
			setactivepage(VGPageActive);
			putimage(0,yline,Prod_Gpage,0);
			setactivepage(VGPageVisu);
		}
		setactivepage(VGPageActive);
	*/
	}

}//end Prod_UpdateDisp

//----------------------------------------------------------------------
//Update the data with the data for the display
void Prod_UpdateData()
{
	ifstream hisfile;
	int unedate[6];
	int fileOk = 0;

	if (Prod_NewHispDisp)
	{
		//read new files
//		fileOk = TheHisList->getProdHisFile(Prod_DispDateAcq,
//					Prod_ActuFile,Prod_PrevFile,Prod_NextFile);
		fileOk = getProdHisFile(Prod_DispDateAcq,
				Prod_ActuFile,Prod_PrevFile,Prod_NextFile);

		Prod_NewHispDisp = 0;
	}

	else if (Prod_NextOrPrev == -1)
	{
		//Previous product required
		//read Previous file date
		hisfile.open(Prod_PrevFile,ios::in|ios::binary);

		if (hisfile)
		{
			hisfile.seekg(14*sizeof(int),ios::beg);
			hisfile.read((char*)unedate,6*sizeof(int));
			hisfile.close();

			fileOk = getProdHisFile(unedate,
				Prod_ActuFile,Prod_PrevFile,Prod_NextFile);
		}//prevfile ok
	}

	else if (Prod_NextOrPrev == 1)
	{
		//Next product required
		//read Next file date
		hisfile.open(Prod_NextFile,ios::in|ios::binary);

		if (hisfile)
		{
			hisfile.seekg(14*sizeof(int),ios::beg);
			hisfile.read((char*)unedate,6*sizeof(int));
			hisfile.close();

			fileOk = getProdHisFile(unedate,
				Prod_ActuFile,Prod_PrevFile,Prod_NextFile);
		}//nextfile ok
	}
	Prod_NextOrPrev = 0;

	if (!fileOk)
	{
	  //read with DispDate

		fileOk =	getProdHisFile(Prod_DispDate,
			Prod_ActuFile,Prod_PrevFile,Prod_NextFile);
	}//!fileok

	if (fileOk)
	{
	  //open ActuFile
		hisfile.open(Prod_ActuFile,ios::in|ios::binary);

		if (hisfile)
		{
			//read the header
			//type
			hisfile.read((char*)&Prod_ProdType,sizeof(int));
			//width or diameter
			int unentier;
			hisfile.read((char*)&unentier,sizeof(int));
			hisfile.read((char*)&unentier,sizeof(int));
			hisfile.read((char*)&Prod_ProdWidi,sizeof(int));

			if (!Prod_ProdWidi) Prod_ProdWidi=unentier;
			//charging temp
			for (int i = 0;i<3;i++)
				hisfile.read((char*)&Prod_ProdChaTempp,sizeof(int));

			Prod_ProdChaTempp =
				(int)(Prod_Gyalpha*((float)(Prod_ProdChaTempp))+Prod_Gybeta);
			//total residence time
			hisfile.read((char*)&unentier,sizeof(int));
			unentier   += unentier/10; //for curve scaling
			unentier    = 10*(1+unentier/10);
			Prod_Gtalpha  = -1.*((float)Prod_GyHaut)/((float)unentier);
			Prod_Gtbeta   = Prod_Gybeta;
			//calculate time axis position
			Prod_dmnGrad  = unentier/10;
			Prod_nbmnGrad = 0;

			for (int j = 0, ygrad = Prod_dmnGrad;
								(ygrad < unentier && j<10); j++,ygrad += Prod_dmnGrad)
			{
				Prod_GmnGradp[j] = (int)(Prod_Gtalpha*(float)ygrad+Prod_Gtbeta);
				Prod_nbmnGrad++;
			}

			//discharging date
			for (i=0;i<6;i++)
				hisfile.read((char*)&unentier,sizeof(int));

			hisfile.read((char*)Prod_DispDate,6*sizeof(int));
			//record amount
			hisfile.read((char*)&Prod_NbActPoint,sizeof(int));
			hisfile.read((char*)&Prod_JobID     ,sizeof(int));
			hisfile.read((char*)&Prod_PONumber  ,sizeof(int));

			//free memory used by present displayed product
			if (Prod_TxAct != NULL)
			{
				delete Prod_TxAct;
				Prod_TxAct = NULL;

				if (Prod_TMoyAct   != NULL) delete Prod_TMoyAct;
				if (Prod_TSurfAct  != NULL) delete Prod_TSurfAct;
				if (Prod_TInfAct   != NULL) delete Prod_TInfAct;
				if (Prod_TSejAct   != NULL) delete Prod_TSejAct;
				if (Prod_TCoreAct  != NULL) delete Prod_TCoreAct;
				if (Prod_TObjAct   != NULL) delete Prod_TObjAct;
				if (Prod_TTvAct    != NULL) delete Prod_TTvAct;
				if (Prod_TxActp    != NULL) delete Prod_TxActp;
				if (Prod_TMoyActp  != NULL) delete Prod_TMoyActp;
				if (Prod_TSurfActp != NULL) delete Prod_TSurfActp;
				if (Prod_TInfActp  != NULL) delete Prod_TInfActp;
				if (Prod_TSejActp  != NULL) delete Prod_TSejActp;
				if (Prod_TCoreActp != NULL) delete Prod_TCoreActp;
				if (Prod_TObjActp  != NULL) delete Prod_TObjActp;
				if (Prod_TTvActp   != NULL) delete Prod_TTvActp;
			}

			if (Prod_NbActPoint)
			{
				//allocate memory for the curves
				Prod_TxAct     = new long[Prod_NbActPoint];
				Prod_TMoyAct   = new long[Prod_NbActPoint];
				Prod_TSurfAct  = new long[Prod_NbActPoint];
				Prod_TInfAct   = new long[Prod_NbActPoint];
				Prod_TSejAct   = new long[Prod_NbActPoint];
				Prod_TCoreAct  = new long[Prod_NbActPoint];
				Prod_TObjAct   = new long[Prod_NbActPoint];
				Prod_TTvAct    = new long[Prod_NbActPoint];
				Prod_TxActp    = new int[Prod_NbActPoint];
				Prod_TMoyActp  = new int[Prod_NbActPoint];
				Prod_TSurfActp = new int[Prod_NbActPoint];
				Prod_TInfActp  = new int[Prod_NbActPoint];
				Prod_TSejActp  = new int[Prod_NbActPoint];
				Prod_TCoreActp = new int[Prod_NbActPoint];
				Prod_TObjActp  = new int[Prod_NbActPoint];
				Prod_TTvActp   = new int[Prod_NbActPoint];

				if (Prod_TTvActp == NULL) Prod_NbActPoint = 0; //error not
									//enough memory
			}
			//init all to 0
			if (Prod_NbActPoint)
			{
				for (i=0;i<Prod_NbActPoint;i++)
				{
					Prod_TxAct[i]     = 0;
					Prod_TxActp[i]    = (int)Prod_Gxbeta;
					Prod_TSejAct[i]   = 0;
					Prod_TSejActp[i]  = (int)Prod_Gtbeta;
					Prod_TTvAct[i]    = 0;
					Prod_TTvActp[i]   = (int)Prod_Gybeta;
					Prod_TSurfAct[i]  = 0;
					Prod_TSurfActp[i] = (int)Prod_Gybeta;
					Prod_TMoyAct[i]   = 0;
					Prod_TMoyActp[i]  = (int)Prod_Gybeta;
					Prod_TCoreAct[i]  = 0;
					Prod_TCoreActp[i] = (int)Prod_Gybeta;
					Prod_TInfAct[i]   = 0;
					Prod_TInfActp[i]  = (int)Prod_Gybeta;
					Prod_TObjAct[i]   = 0;
					Prod_TObjActp[i]  = (int)Prod_Gybeta;
				}
			}//init to 0

	//read each record
			long anabs;
			int atsej,fivetemp[5],atarg;
			long oxydec[2];

			hisfile.read((char*)&Prod_SteelName,Hec_SteelName);
			hisfile.read((char*)&Prod_Coulee   ,Hec_Coulee);
			hisfile.read((char*)&anabs,sizeof(long));

			for (i=0;i<Prod_NbActPoint;i++)
			{
				hisfile.read((char*)&anabs,sizeof(long));
				hisfile.read((char*)&atsej,sizeof(int));
				hisfile.read((char*)fivetemp,5*sizeof(int));
				hisfile.read((char*)&atarg,sizeof(int));
				hisfile.read((char*)oxydec,2*sizeof(long));

				if (hisfile)
				{
			//abscissa
					Prod_TxAct[i]  = anabs;

					Prod_TxActp[i] =
						(int)(Prod_Gxalpha*((float)(Prod_TxAct[i]))+Prod_Gxbeta);

			//residence time
					Prod_TSejAct[i]  = (long)atsej;
					Prod_TSejActp[i] =
					(int)(Prod_Gtalpha*((float)(Prod_TSejAct[i]))+Prod_Gtbeta);

			//temperature
					Prod_TTvAct[i]  = (long)fivetemp[0];
					Prod_TTvActp[i] =
						(int)(Prod_Gyalpha*((float)(Prod_TTvAct[i]))+Prod_Gybeta);

					Prod_TSurfAct[i]  = (long)fivetemp[1];
					Prod_TSurfActp[i] =
						(int)(Prod_Gyalpha*((float)(Prod_TSurfAct[i]))+Prod_Gybeta);

					Prod_TMoyAct[i]  = (long)fivetemp[2];
					Prod_TMoyActp[i] =
						(int)(Prod_Gyalpha*((float)(Prod_TMoyAct[i]))+Prod_Gybeta);

					Prod_TCoreAct[i]  = (long)fivetemp[3];
					Prod_TCoreActp[i] =
						(int)(Prod_Gyalpha*((float)(Prod_TCoreAct[i]))+Prod_Gybeta);

					Prod_TInfAct[i]  = (long)fivetemp[4];
					Prod_TInfActp[i] =
						(int)(Prod_Gyalpha*((float)(Prod_TInfAct[i]))+Prod_Gybeta);

			//end zone target
					Prod_TObjAct[i]  = (long)atarg;
					Prod_TObjActp[i] =
						(int)(Prod_Gyalpha*((float)(Prod_TObjAct[i]))+Prod_Gybeta);
				}//hisfile
			}//each record
			hisfile.close();
			Prod_TxActp[0] =Prod_GxZero;   //2-9-98 VB:Correction du zero

		}//actufile ok
	}//fileok
}//end Prod_UpdateData

//-------------------------------------------------------------------
//display background
void Prod_DispBackground()
{
	int rpoint[8];
	char untext[80];

	//display cursor area
	if(*init==INITIAL)
	{
		rpoint[0]=rpoint[6] = Left_Rectangle;
		rpoint[1]=rpoint[3] = Top;
		rpoint[2]=rpoint[4] = Right_Rectangle;
		rpoint[5]=rpoint[7] = Bottom;

		setfillstyle(SOLID_FILL,1);
		settextstyle(0,0,1);
		setcolor(15);
		bar(0,rpoint[1],639,rpoint[5]);
		rectangle(rpoint[0],rpoint[1],rpoint[2],rpoint[5]);
		setcolor(15);

		strcpy(untext,"Aver.  T.:        Target T.:");
		outtextxy(8,Prod_GyTempMoy,untext);
		strcpy(untext,"Bottom T.:        Deviation:");
		outtextxy(8,Prod_GyTempInf,untext);
		strcpy(untext,"Top    T.:    ");
		outtextxy(8,Prod_GyTempSurf,untext);
		strcpy(untext,"Core   T.:        Res.Time :");
		outtextxy(8,Prod_GyTempCore,untext);

		strcpy(untext,"øC");
		Prod_GxActu  =8+textwidth("Aver.  T.:    "); //10

		for(int iy = Prod_GyTempMoy;iy <= Prod_GyTempCore;iy+=10)
			outtextxy(Prod_GxActu,iy,untext);

		Prod_GxReqTemp = textwidth("Aver.  T.:        Target T. :       ") - 2;
		line (Prod_GxReqTemp,rpoint[1]+1,Prod_GxReqTemp,rpoint[5]-1);

		strcpy(untext,"Job ID     :     ");
		outtextxy(Prod_GxReqTemp+8,Prod_GyTempMoy,untext);
		strcpy(untext,"P.O  Number:     ");
		outtextxy(Prod_GxReqTemp+8,Prod_GyTempInf,untext);
		strcpy(untext,"Steel Grade:     ");
		outtextxy(Prod_GxReqTemp+8,Prod_GyTempSurf,untext);
		strcpy(untext,"Req.  Temp.:     ");
		outtextxy(Prod_GxReqTemp+8,Prod_GyTempCore,untext);
		line(Xline,Top,Xline,Bottom);

		Prod_GxTarget=8+textwidth("Aver.  T.:        Target T. :     ");
		Prod_GxRef =Prod_GxReqTemp+8+textwidth("JobID      :     ");

		strcpy(untext,"øC");
		outtextxy(Prod_GxRef,Prod_GyTempCore,untext);

}
	settextstyle(0,0,1);

	//calculate temperature axis position
	Prod_dtempGrad = (int)((Prod_GMaxTemp/100)*10);
	Prod_nbtempGrad = 0;

	for (int i=0,ygrad=Prod_dtempGrad;
		(ygrad<Prod_GMaxTemp && i<10); i++,ygrad+=Prod_dtempGrad)
	{
		Prod_GtempGradp[i] = (int)(Prod_Gyalpha*(float)ygrad+Prod_Gybeta);
		Prod_nbtempGrad++;
	}

	setfillstyle(SOLID_FILL,Background_Color);

	bar(Prod_GxZero+Prod_GxLarge,23,639,Cursor_Bottom);
	bar(0,23,639,Prod_GyZero);
	bar(0,23,Prod_GxZero,Cursor_Bottom);
	bar(Prod_GxZero,Prod_GyZero+Prod_GyHaut,Prod_GxZero+Prod_GxLarge,Cursor_Bottom);

	//display the axis
	//axis
	settextjustify(LEFT_TEXT,TOP_TEXT);
	setcolor(15);

	line(Prod_GxZero,Prod_GyZero+Prod_GyHaut,Prod_GxZero,Prod_GyZero);
	moveto(Prod_GxZero,Prod_GyZero);

	linerel(-5,5);
	moverel(5,-5);
	linerel(5,5);

	line(Prod_GxZero,Prod_GyZero+Prod_GyHaut,
		Prod_GxZero+Prod_GxLarge+19,Prod_GyZero+Prod_GyHaut);      //45
	moveto(Prod_GxZero+Prod_GxLarge+19,Prod_GyZero+Prod_GyHaut);

	linerel(-7,-4);
	moverel(7,4);
	linerel(-7,4);

/*   VB:1-21-98: ordonn‚es du temps
	line(Prod_GxZero+Prod_GxLarge,Prod_GyZero+Prod_GyHaut,
		Prod_GxZero+Prod_GxLarge,Prod_GyZero);
	moveto(Prod_GxZero+Prod_GxLarge,Prod_GyZero);

	linerel(-5,5);
	moverel(5,-5);
	linerel(5,5);

	strcpy(untext,"m");
	outtextxy(638-textwidth(untext),
		Prod_GyZero+Prod_GyHaut-textheight(untext)/2,untext);
*/

	strcpy(untext,"øC");
	outtextxy(Prod_GxZero-textwidth(untext)/2-15,
		Prod_GyZero-textheight(untext)/2-3,untext);
/*
	strcpy(untext,"mn");
	outtextxy(Prod_GxZero+Prod_GxLarge-textwidth(untext)/2,
		Prod_GyZero-4-textheight(untext)/2,untext);
*/

	 //temperature ordinate
	settextjustify(RIGHT_TEXT,CENTER_TEXT);
	int j;

	for (ygrad=Prod_dtempGrad,j=0;
		j<Prod_nbtempGrad;ygrad+=Prod_dtempGrad,j++)
	{
		(void)itoa(ygrad,untext,10);
		outtextxy(Prod_GxZero-7,Prod_GtempGradp[j],untext);
		moveto(Prod_GxZero,Prod_GtempGradp[j]);
		linerel(-5,0);
	}

/* VB 01-21-98
	//time ordinate

	settextjustify(LEFT_TEXT,CENTER_TEXT);

	for (ygrad=Prod_dmnGrad,j=0;j<Prod_nbmnGrad;ygrad+=Prod_dmnGrad,j++)
	{
		(void)itoa(ygrad,untext,10);
		outtextxy(Prod_GxZero+Prod_GxLarge+7,Prod_GmnGradp[j],untext);
		moveto(Prod_GxZero+Prod_GxLarge,Prod_GmnGradp[j]);
		linerel(5,0);
	}
*/
	//abscissa

	settextjustify(CENTER_TEXT,TOP_TEXT);
	int icp  = 1;
	int ic5p = 5;

	moveto(Prod_CursorPosp[0],Prod_GyZero+Prod_GyHaut);
	linerel(0,4);
	outtextxy(Prod_CursorPosp[0],Prod_GyZero+Prod_GyHaut+6,"E");       //VB

	while ( Prod_CursorPosp[icp] < Prod_GPosZp[Prod_GNbZone-1])  //-1
	{
		moveto(Prod_CursorPosp[icp],Prod_GyZero+Prod_GyHaut);
		linerel(0,4);

		if (icp == ic5p)
		{
			(void)itoa(icp,untext,10);
			outtextxy(Prod_CursorPosp[icp],Prod_GyZero+Prod_GyHaut+6,untext);
			ic5p += 5;
		}
		icp++;
	}
//	moveto(Prod_CursorPosp[icp],Prod_GyZero+Prod_GyHaut);
	moveto(Prod_GxZero+Prod_GxLarge,Prod_GyZero+Prod_GyHaut);
	linerel(0,4);
	outtextxy(Prod_GxZero+Prod_GxLarge+6,Prod_GyZero+Prod_GyHaut+6,"S");

	 //zone line
	setlinestyle(DOTTED_LINE,NORM_WIDTH,0);
	setcolor(12);

	for (int xnbgrad = 0;xnbgrad<Prod_GNbZone;xnbgrad++)
	{
		moveto(Prod_GPosZp[xnbgrad],Prod_GyZero+5);
		linerel(0,Prod_GyHaut-5);
	}
	setlinestyle(SOLID_LINE,NORM_WIDTH,0);
}//end background

void Save_Date()
{
	APPELHS("Ecran,Recupere,HEURE");
	sscanf(HsChaine,"%d",&Prod_DispDateAcq[3]);

	APPELHS("Ecran,Recupere,MIN  ");
	sscanf(HsChaine,"%d",&Prod_DispDateAcq[4]);

	APPELHS("Ecran,Recupere,SEC  ");
	sscanf(HsChaine,"%d",&Prod_DispDateAcq[5]);

	APPELHS("Ecran,Recupere,JOUR ");
	sscanf(HsChaine,"%d",&Prod_DispDateAcq[2]);

	APPELHS("Ecran,Recupere,MOIS ");
	sscanf(HsChaine,"%d",Prod_DispDateAcq[1]);

	APPELHS("Ecran,Recupere,AN   ");
	sscanf(HsChaine,"%d",Prod_DispDateAcq[0]);

	setviewport(Prod_GxZero+1,34,599,Prod_GyZero+Prod_GyHaut-1,0);
	clearviewport();
	setviewport(0,0,639,349,0);
	Prod_Erase();
	Prod_NewHispDisp=1;
	Prod_Display();

	Prod_UpdateHPGraphCursor(0,1);
	APPELHS("Ecran,Premier,HEURE");
}

void Previous_Prod()
{
		Prod_Erase();
		setviewport(Prod_GxZero+1,34,599,Prod_GyZero+Prod_GyHaut-1,0);
		clearviewport();
		setviewport(0,0,639,349,0);
		Prod_NextOrPrev = -1;
		Prod_Display();

		Prod_UpdateHPGraphCursor(0,1);
}

void Next_Prod()
{
		Prod_Erase();
		setviewport(Prod_GxZero+1,34,599,Prod_GyZero+Prod_GyHaut-1,0);
		clearviewport();
		setviewport(0,0,639,349,0);
		Prod_NextOrPrev = 1;
		Prod_Display();

		Prod_UpdateHPGraphCursor(0,1);

		Prod_UpdateData();
}
//-------------------------------------------------------------------
//end Prod_OVL1.CPP
