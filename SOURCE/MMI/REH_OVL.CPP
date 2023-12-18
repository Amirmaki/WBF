//-------------------------------------------------------------------------
// REH_OVL.CPP
// This file contains possible overlay function for project TACT
// display number 07
//
// Owning include file : NONE
//
// Creation : C.Martinet 18/12/92 ASCOMETAL APPLICATION
// Modified :            11/01/93 for password acquisition
//                       13/01/93 delay display correction
//			 15/01/93 check gaps in curve display
//			 20/01/93 update with ASCOMETAL demands
//			 21/01/93 EndBatchMarker becomes beg batch marker
//			 26/01/93 select targets with prod rate and type
//			 27/01/93 display production rate
//			 02/02/93 display the hour
//			 18/02/93 blinks PV if required
//-------------------------------------------------------------------------
#define _OWNER2_Reh_DATA1_H

#ifndef _GRAPHICS_H
#include <graphics.h>
#endif
#ifndef __CONIO_H
#include <conio.h>
#endif
#ifndef __DOS_H
#include <dos.h>
#endif
#ifndef __STRING_H
#include <string.h>
#endif
#ifndef __STDLIB_H
#include <stdlib.h>
#endif
#ifndef __STDIO_H
#include <stdio.h>
#endif
#ifndef __GLOBVIEW_H
#include <globview.h>
#endif
#ifndef __REH_DATA_H
#include "Reh_data.h"
#endif
#ifndef __ALLOC_H
#include <alloc.h>
#endif
#ifndef __MTDBFUNC_H
#include <mtdbfunc.h>
#endif
#ifndef __GLOBDAT_H
#include <globdat.h>
#endif
#ifndef __CONFIG_H
#include "config.h"
#endif

#define RefreshMeasPer 5
extern ProdTypeList *TheTypeList ; 	   // Pointer to product list object
extern EndzTargetList *TheTargetList; 	  // Pointer to objective curve list

//------------------------------------------------------------------------
//initialisation
void Reh_Init()
{
	GMaxTemp  = 1500;
	GxZero    = 40;
	GxLarge   = 560;
	GyZero    = 33;
	GyHaut    = 464-Top;
	GyTempMoy = Bottom-40;
	GyTempInf = Bottom-30;
	GyTempSurf= Bottom-20;
	GyTempCore= Bottom-10;
	RefCurve  = 0;
	EnableCurveRecording = 0;
	Reh_RecordRefReq     = 0;
	DeltaRefActu         = 0;
	NbActPoint           = 0;
	CursorPos            = 0;
	CursorInc            = 1;
/*
	for (int i=0;i<maxNbZone;i++)
		{
		Reh_BlinkingSP[i] = 1;
		Reh_SPColor[i] = 0;
		Reh_TrueSPColor[i] = 60;
		}
	Reh_SPColor[1] = 5;
	Reh_SPColor[2] = 9;
	Reh_SPColor[3] = 10;
	Reh_SPColor[4] = 11;
	Reh_BlinkingPV = 0;
	Reh_TruePVColor = 62;
 */

	Reh_Active    =	Reh_Redraw   = 0;
	Reh_CurAcqVal = 0;
	Reh_GxPass    = 300;
	Reh_GyPass    = 130;
	Reh_DelayType = 0;
	Reh_NewPdis   = Reh_DischGap = 0;
	VGNextViewNb = 7;

//	Reh_Duree_seuil[0] = -1;
//	Reh_Duree_seuil[1] = -1;

	NbPas = (int) (TheFurn->length() / TheFurn->wbeamStep());

	TVouteAct  = new int[NbPas];
	CursorPosm = new long[NbPas];
	CursorPosp = new int[NbPas];

	for (int i = 0;i < NbPas; i++) TVouteAct[i] = 0;

}//end Reh_Init

//----------------------------------------------------------------------
//display
void Reh_Display()
{
  //set mode to graphic mode

	if (VGDispMode == 1)
		{
		setgraphmode(getgraphmode());
		VGDispMode = 0;
		}

	//set all palette color
	setpalette(3,63);
	setpalette(4,59);
	setpalette(6,62);
	setpalette(7,57);
	setpalette(8,60);
	setpalette(9,LIGHTGRAY);
	setpalette(10,58);
	setpalette(13,61);

	GFlength = (long) 1000.0*TheFurn->length();
	GNbZone = TheFurn->nbZone();

//	for (int nspt=1;nspt<GNbZone;nspt++)
//		setpalette(Reh_SPColor[nspt],Reh_TrueSPColor[nspt]);

//	setpalette(12,Reh_TruePVColor);

  	Array& FZ = TheFurn->theZone();

  	for (int i=0; i<GNbZone; i++)
		{
		GPosZ[i] = (long)1000.0*(((Zone&)(FZ[i])).absEnd());
		GPosT[i] = (long)1000.0*(((Zone&)(FZ[i])).absTcpl());
		GMinSP[i] = ((Zone&)(FZ[i])).celMinSetPoint();
		GMaxSP[i] = ((Zone&)(FZ[i])).celMaxSetPoint();
		}

  //abscissa scaling
	Gxalpha = ((float)GxLarge)/((float)GFlength);
	Gxbeta = (float)GxZero;

  //ordinate scaling
	Gyalpha = -1.*((float)GyHaut)/((float)GMaxTemp);
	Gybeta = (float)(GyZero+GyHaut);

  //set cursor data positions (pixel), specific asco93
	float GStep = 1000.0*TheFurn->wbeamStep();
  //float curpos = GStep/2;
	float curpos = 350;

	for (int icp = 0; icp < NbPas; icp++ , curpos += GStep)
		{
		CursorPosm[icp] = (long)curpos;
		CursorPosp[icp] = (int)(Gxalpha*curpos + Gxbeta);
		}

  //zone pixel position
	for (int xnbgrad = 0;xnbgrad<GNbZone;xnbgrad++)
		GPosZp[xnbgrad] = (int)(Gxalpha*((float)GPosZ[xnbgrad])+Gxbeta);

  //position thermocouple + recup
	for (int xnbz = 0; xnbz < GNbZone; xnbz++)
	{
		int xpos = (int)(Gxalpha*((float)GPosT[xnbz])+Gxbeta);
		GxTcpl[xnbz] = xpos;
		GyTcpl[xnbz] = GyZero+26;
	}

	GxTcpl[maxNbZone] = GxZero-textwidth("1234")/2-8;
	GyTcpl[maxNbZone] = GyZero+26;

  //init reference curve to 0
	RefCurve = 0;

	for (int nz1 = 0; nz1 < maxNbZone; nz1++)
	{
		EndZoneRef[nz1] = NextZoneRef[nz1] = 0;
		EndZoneRefp[nz1] = NextZoneRefp[nz1] = 0;
	}

  //init TRoofp
	TRoofp[0] = 0;

  //read the time
/*	char anhour[6];
	(void)strcpy(anhour,"hhhmn");
	struct time t;
	gettime(&t);
	(void)itoa(t.ti_hour,anhour,10);
	if (anhour[1] == '\0')
		{
		anhour[1]=anhour[0];
		anhour[0]=' ';
		}
	else anhour[2] = 'h';
	(void)itoa(t.ti_min,&anhour[3],10);
	if (anhour[4] == '\0')
		{
		anhour[5] = '\0';
		anhour[4] = anhour[3];
		anhour[3] = '0';
		}
	(void)strcpy(Reh_Hour,anhour);
  */
	Reh_UpdateData();
	Reh_UpdateDisp();
	VGDispViewNb = 7;

}//end Reh_Display

//----------------------------------------------------------------------
//erase
void Reh_Erase()
{
	NbActPoint = 0;
	RefCurve = 0;
	EnableCurveRecording = 0;

	//free allocated memory
	if (TxAct != NULL) delete TxAct;
	TxAct = NULL;
	if (TMoyAct != NULL) delete TMoyAct;
	if (TSurfAct != NULL) delete TSurfAct;
	if (TInfAct != NULL) delete TInfAct;
	if (TSejAct != NULL) delete TSejAct;
	if (TCoreAct != NULL) delete TCoreAct;
	if (TxActp != NULL) delete TxActp;
	if (TMoyActp != NULL) delete TMoyActp;
	if (TSurfActp != NULL) delete TSurfActp;
	if (TInfActp != NULL) delete TInfActp;
	if (TCoreActp != NULL) delete TCoreActp;
	if (CursorPosm != NULL) delete CursorPosm;
	if (CursorPosp != NULL) delete CursorPosp;
	if (TVouteAct != NULL) delete TVouteAct;

	// FD add for PACING
#ifdef PACINGMODE
	if (TprodRate != NULL) delete TprodRate;
#endif

	if (Gpage != NULL)
		{
		delete Gpage;
		Gpage = NULL;
	}
}//end Reh_Erase

//----------------------------------------------------------------------
//Update the display with new data
void Reh_UpdateDisp()
{
	if (!Reh_Active)
	{
		if (VGPageVisu == 0)
			VGPageActive = 1;
		else
			VGPageActive = 0;

		setactivepage(VGPageActive);

		setviewport(GxZero+1,34,GxZero+GxLarge,GyZero+GyHaut-1,0);

		//clear the screen
		clearviewport();
		setviewport(0,0,GxZero+GxLarge,GyZero+GyHaut-1,0);

		//display the background
		Reh_DispBackground();

		//display the data

		setfillstyle(SOLID_FILL,1);
		char *untext = "                           ";

		//check gaps
		int *hmovex;
		hmovex = NULL;

		if (NbActPoint)
		{
			hmovex = new int[NbActPoint];
			hmovex[0] = GxZero+1;

			int prevj,j = 1;

			if (TxAct[0] < GPosZ[GNbZone-1])
			{
				while (TxAct[0] > CursorPosm[j]) j++;
					if (j>1) hmovex[0] = CursorPosp[j-1];
			}
			else
				hmovex[0] = TxActp[0];

			for (int ip = 1; ip < NbActPoint; ip++)
			{
				prevj = j;
				while(TxAct[ip] > CursorPosm[j]) j++;

				if (j-prevj > 1)
				{
				//there is a gap
					if (TxAct[ip] < GPosZ[GNbZone-1])
						hmovex[ip] = CursorPosp[j-1];
					else
						hmovex[ip] = TxActp[ip];
				}
				else
					hmovex[ip] = 0;
			}//ip
		}//end check gaps

	//floodfill
		if(NbActPoint && RefCurve)
		{
			int nz = 0;
			int lastpix = GxZero+1;
			int itme = 0;
			int ip = 0;
			int tmoy,endref = EndZoneRefp[0];

		//check gaps
			while(TMoyEcart[itme] != -1 && ip < NbActPoint)
			{
				if (hmovex[ip])
				{
				//there is a gap
					while(TMoyEcart[itme] != -1 && lastpix < hmovex[ip])
					{
						TMoyEcart[itme] = -2;
						itme++;
						lastpix++;
					}
				}
				else
				{
				//no gap
					while(TMoyEcart[itme] != -1 && lastpix <= TxActp[ip])
					{
						itme++;
						lastpix++;
					}
				}
				ip++;
			}//check gaps

		//Check product temp at each end of zone
			for (int nzec=0;nzec<GNbZone;EndZEcart[nzec] = -1,nzec++);
			lastpix = GxZero;
			itme=0;

			int nze = 0;

			while(TMoyEcart[itme] != -1)
			{
				lastpix++;
				if (lastpix > GPosZp[nze] && nze < GNbZone -1) nze++;
				if (TMoyEcart[itme] != -2) EndZEcart[nze] = TMoyEcart[itme];
				itme++;
			}

		//Uses last discharged product for last zone if furnace not stopped
		//and if there were no gap else use next to discharge
	  /*	if (Pdis != NULL)
			{
			if (!DelayState && !hmovex[NbActPoint-1] && !Reh_DischGap)
				EndZEcart[GNbZone-1] = TMoyActp[NbActPoint-1];
			else
				if (NbActPoint > 1 && EndZEcart[GNbZone-1] != -1)
					EndZEcart[GNbZone-1] = TMoyActp[NbActPoint-2];

			if (Reh_NewPdis)
				{
				if (hmovex[NbActPoint-1])
					Reh_DischGap = 1;
				else Reh_DischGap = 0;
				Reh_NewPdis = 0;
				}
			}
	  */
	 //display for each point
			itme = 0;
			lastpix = GxZero;
			while(TMoyEcart[itme] != -1)
			{
				lastpix++;
				tmoy = TMoyEcart[itme];

				if (lastpix > GPosZp[nz] && nz < GNbZone -1)
				{
					nz++;
					endref = EndZoneRefp[nz];
				}

				if (tmoy != -2 && endref && lastpix != GPosZp[nz])
				{
					if (tmoy < endref)
					{
						setcolor(8);
						line(lastpix,endref-1,lastpix,tmoy+1);
					}
					else
					{
						tmoy = EndZEcart[nz];
						if (tmoy != -1 && tmoy > endref)
						{
							setcolor(7);
							tmoy = EndZEcart[nz]+1;
							line(lastpix,endref+1,lastpix,tmoy-1);
						}
					}
				}//endref
				itme++;
			}//for each point
		}//NbActPoint

	//if the surface temp. curve must be displayed
		if (NbActPoint)
		{
			setcolor(13);
			moveto(hmovex[0],TSurfActp[0]);

			for (int np = 0;np < NbActPoint-Reh_DischGap;np++)
			{
				if (hmovex[np]) moveto(hmovex[np],TSurfActp[np]);
				lineto(TxActp[np],TSurfActp[np]);
			}
			moveto(hmovex[0],TInfActp[0]);

			for (np = 0;np < NbActPoint-Reh_DischGap;np++)
			{
				if (hmovex[np])
					moveto(hmovex[np],TInfActp[np]);

				lineto(TxActp[np],TInfActp[np]);
			}
		}

	//if the roof temp. curve must be displayed
		if (TRoofp[0])
		{
			setcolor(6);
			moveto(GxZero+1,TRoofp[0]);

			for (int np = 0;np < 100;lineto(TxRoofp[np],TRoofp[np]),np++);
				lineto(GxZero+GxLarge,TRoofp[99]);
		//provisoire pour test
/*
		setcolor(2);
		moveto(GxZero+1,TFumep[0]);
		for (np = 0;np < 100;lineto(TxRoofp[np],TFumep[np]),np++);
		lineto(GxZero+GxLarge,TFumep[99]);
*/
		}

	//if the reference curve exists and must be displayed
		if (RefCurve)
		{
			setcolor(10);

			if ( EndZoneRefp[0] )
			{
				moveto(GxZero+1,EndZoneRefp[0]);
				lineto(GPosZp[0]-1,EndZoneRefp[0]);
			}

			for (int nz = 1; nz < GNbZone ; nz++)
			{
				if ( EndZoneRefp[nz] )
				{
					moveto(GPosZp[nz-1]+1,EndZoneRefp[nz]);
					lineto(GPosZp[nz],EndZoneRefp[nz]);
				}
			}
		} //RefCurve

		//if the core temp. curve must be displayed
		if (NbActPoint)
		{
			setcolor(4);
			moveto(hmovex[0],TCoreActp[0]);

			for (int np = 0;np < NbActPoint-Reh_DischGap;np++)
			{
				if (hmovex[np]) moveto(hmovex[np],TCoreActp[np]);
					lineto(TxActp[np],TCoreActp[np]);
			}
		}

	//if the mean temp curve must be displayed
	if (NbActPoint)
	{
		setcolor(3);
		moveto(hmovex[0],TMoyActp[0]);

		for (int np = 0;np < NbActPoint-Reh_DischGap;np++)
		{
			if (hmovex[np]) moveto(hmovex[np],TMoyActp[np]);
				lineto(TxActp[np],TMoyActp[np]);
		}
	}

	//delete hmovex
	if (hmovex != NULL) delete hmovex;

	//display end of batch marker
/*
	int ProdRate;
	if (V13TheoOrMeasProd)
		{
		ProdRate = V13TheoProd;
		}
	else ProdRate = V13MeasProd;


ÍÍÍÍÍÍÍÍÍÍÍÍÍÍØÍÍÍÍÍØ
 BLACK        ³  0  ³
 BLUE         ³  1  ³
 GREEN        ³  2  ³
 CYAN         ³  3  ³
 RED          ³  4  ³
 MAGENTA      ³  5  ³
 BROWN        ³  6  ³
 LIGHTGRAY    ³  7  ³
 DARKGRAY     ³  8  ³
 LIGHTBLUE    ³  9  ³
 LIGHTGREEN   ³ 10  ³
 LIGHTCYAN    ³ 11  ³
 LIGHTRED     ³ 12  ³
 LIGHTMAGENTA ³ 13  ³
 YELLOW       ³ 14  ³
 WHITE        ³ 15  ³
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÅ
 BLINK        ³128  ³


	if(ProdRate >0 && ProdRate <10)
	setcolor(1);
	else if(ProdRate >=10 && ProdRate <20)
	setcolor(2);
	else if(ProdRate >=20 && ProdRate <30)
	setcolor(3);
	else if(ProdRate >=30 && ProdRate <40)
	setcolor(4);
	else if(ProdRate >=40 && ProdRate <50)
	setcolor(5);
	else if(ProdRate >=50 && ProdRate <60)
	setcolor(6);
	else if(ProdRate >=60 && ProdRate <70)
	setcolor(7);
	else setcolor(8);
*/
	setcolor(14);

	int markerIndex = 0;
	settextjustify(CENTER_TEXT,BOTTOM_TEXT);

	while(EndBatchMarker[markerIndex])
	{
		int enbx = EndBatchMarker[markerIndex];

		for(int dpx = 4;dpx>=0;dpx--)
			line(enbx-4+dpx,GyZero+GyHaut-5+dpx,enbx+4-dpx,GyZero+GyHaut-5+dpx);

		long ntyp = (long)(EndBatchType[markerIndex++]->number());

		char *atype = "  ";
		(void)ltoa(ntyp,atype,10);

		if (enbx <= GPosZp[GNbZone-1])
			outtextxy(enbx,GyZero+GyHaut-6,atype);
		else
		{
			settextjustify(RIGHT_TEXT,BOTTOM_TEXT);
			outtextxy(enbx-5,GyZero+GyHaut-1,atype);
		}
	}
#ifdef DISPLAY_PREF
	// Dessine les produits de reference dans chaque zone
	int Xpos = 0;
	int Ypos = 0;
	char *znumtxt = " ";
	settextjustify(CENTER_TEXT,BOTTOM_TEXT);
	setcolor(2);

	for (int znum = 0 ; znum < maxNbZone ; znum++)
	{
		if (RefProdAbsc[znum] != -1)
		{
			Xpos = (int) ( 1000.0*Gxalpha*RefProdAbsc[znum] + Gxbeta ) ;
			Ypos = GyZero + GyHaut - 2 ;
			(void)itoa(znum,znumtxt,10);

			rectangle (Xpos-4,Ypos-4,Xpos,Ypos);
			outtextxy (Xpos-2,Ypos-6,znumtxt);
		}
	}
#endif
	//display the temperature
	setcolor(14);

/*
	if (!Reh_BlinkingPV)
	  {
		Reh_TruePVColor = 60;
		setpalette(12,Reh_TruePVColor);
	  }
*/
	settextjustify(CENTER_TEXT,BOTTOM_TEXT);
	for (int ntemp = 0; ntemp < GNbZone; ntemp++)
	{
		//display the data
		if (GPosT[ntemp] > 0)
		{
			(void)ltoa(GZtemp[ntemp],untext,10);
			outtextxy(GxTcpl[ntemp],GyTcpl[ntemp]-5,untext);
		}
	}
	//display trec
	(void)ltoa(GZtemp[maxNbZone],untext,10);
  //		outtextxy(GxTcpl[maxNbZone],GyTcpl[maxNbZone]-5,untext);

	outtextxy(GxZero+40,GyTcpl[maxNbZone]-5,untext);

	//display the setpoints

//	int SPTextColor = 4;
	for (ntemp = 1; ntemp < GNbZone; ntemp++)
	{
	//	  setcolor(Reh_SPColor[ntemp]);
		setcolor(10);

		if (GZspt[ntemp] >= GMaxSP[ntemp])
			setcolor(8);
		else
			if (GZspt[ntemp] <= GMinSP[ntemp]) setcolor(7);
	/*	  if (Reh_BlinkingSP[ntemp])
			{
				Reh_TrueSPColor[ntemp] = 60;
				setpalette(Reh_SPColor[ntemp],60);
			}
			else SPTextColor = Reh_SPColor[ntemp];*/
			//display the data
		(void)ltoa(GZspt[ntemp],untext,10);
		outtextxy(GxTcpl[ntemp],GyTcpl[ntemp]-15,untext);
	}

	setcolor(14);
/*
#ifdef DEBBUGING
		char memav[60];
		(void)strcpy(memav,"MemAvail:                                          ");
		unsigned long memAvail = farcoreleft();
		(void)ultoa(memAvail,&memav[10],10);
		settextjustify(CENTER_TEXT,TOP_TEXT);
		outtextxy(320,40,memav);
#endif
*/
		//update cursor data (will switch to active page)
	Reh_UpdateGraphCursor();
	Update_Data(0);

		//update non visual page (for next cursor update)
	if (VGPageVisu == 0)
		VGPageActive = 1;
	else
		VGPageActive = 0;

		if(*init == INITIAL)
		{
			tailp = imagesize(0,0,639,19);
			Gpage = (void *)new char[tailp];

			struct palettetype pal;

			for (int yline = 0; yline < 349; yline+=20)   //349
			{
				if ( yline == 0 )
				{
					getpalette(&pal);
					APPELHS ("Fusionne,rehtitle.img,1,1");
					setallpalette(&pal);
					APPELHS ("Fusionne,datehour.aid");
					APPELHS ("Ouvre,REH_BOUT.AID,23,1");
				}

				setactivepage(VGPageVisu);
				getimage(0,yline,639,yline+19,Gpage); //yline+19
				setactivepage(VGPageActive);
				putimage(0,yline,Gpage,0);
			}

			if (Gpage != NULL)
			{
				delete Gpage;
				Gpage = NULL;
			}
		}
	}//!Reh_Active
	setactivepage(VGPageActive);
	Reh_Redraw = 0;

}//end Reh_UpdateDisp
//----------------------------------------------------------------------
//display the background
void Reh_DispBackground()
{
	setfillstyle(SOLID_FILL,1);
	settextstyle(0,0,1);

	char untext[80];
	int rpoint[8];

	//valeur au curseur
	if (*init == INITIAL)
		{
		setcolor(15);

		rpoint[0]=rpoint[6]=Left_Rectangle1;
		rpoint[1]=rpoint[3]=Top;
		rpoint[2]=rpoint[4]=Right_Rectangle1;
		rpoint[5]=rpoint[7]=Bottom;

		bar(rpoint[0],rpoint[1],rpoint[2],rpoint[5]);
		rectangle(rpoint[0],rpoint[1],rpoint[2],rpoint[5]);

		setcolor(15);
		strcpy(untext,"Aver.  T.:        Target T.:");
		outtextxy(8,GyTempMoy,untext);
		strcpy(untext,"Bottom T.:        Deviation:");
		outtextxy(8,GyTempInf,untext);
		strcpy(untext,"Top    T.:    ");
		outtextxy(8,GyTempSurf,untext);
		strcpy(untext,"Core   T.:        Res.Time :");
		outtextxy(8,GyTempCore,untext);

		strcpy(untext,"øC");
		GxActu  =8+textwidth("Aver.  T.:    "); //10

		for (int iy = GyTempMoy;iy <= GyTempCore;iy+=10)
			outtextxy(GxActu,iy,untext);

//	outtextxy(GxType,GyTempCore,untext);
//	strcpy(untext,"mm");
//	outtextxy(GxType,GyTempSurf,untext);
//	strcpy(untext,"P/h");
//	outtextxy(GxType,GyTempInf,untext);

		GxReqTemp = textwidth("Aver.  T.:        Target T. :       ") - 2;
		line (GxReqTemp,rpoint[1]+1,GxReqTemp,rpoint[5]-1);

		strcpy(untext,"Job ID     :     ");
		outtextxy(GxReqTemp+8,GyTempMoy,untext);
		strcpy(untext,"P.O  Number:     ");
		outtextxy(GxReqTemp+8,GyTempInf,untext);
		strcpy(untext,"Steel Grade:     ");
		outtextxy(GxReqTemp+8,GyTempSurf,untext);
		strcpy(untext,"Req.  Temp.:     ");
		outtextxy(GxReqTemp+8,GyTempCore,untext);

		GxTarget=8+textwidth("Aver.  T.:        Target T. :     ");
		GxRef =GxReqTemp+8+textwidth("JobID      :     ");

		strcpy(untext,"øC");
		outtextxy(GxRef,GyTempCore,untext);

	//legende
		Reh_DispLegendOrDelay();
		settextstyle(0,0,1);
		setfillstyle(SOLID_FILL,Background_Color);
		bar(600,23,639,Cursor_Bottom);
		bar(0,Cursor_Top-14,639,Cursor_Top);

		//axis

		line(GxZero,GyZero+GyHaut,GxZero+GxLarge+19,GyZero+GyHaut);

	//abscissa
		settextjustify(CENTER_TEXT,TOP_TEXT);
		int icp = 0;
		int ic5p = 5;
		moveto(CursorPosp[icp],GyZero+GyHaut);
		linerel(0,4);
		outtextxy(CursorPosp[icp],GyZero+GyHaut+6,"E");

		while ( CursorPosp[icp] < GPosZp[GNbZone-1])
			{
			moveto(CursorPosp[icp],GyZero+GyHaut);
			linerel(0,4);

			if (icp == ic5p)
				{
				(void)itoa(icp,untext,10);
				outtextxy(CursorPosp[icp],GyZero+GyHaut+6,untext);
				ic5p += 5;
				}
			icp++;
			}

		moveto(GxZero+GxLarge+19,GyZero+GyHaut);//19
		linerel(-7,-4);
		moverel(7,4);
		linerel(-7,4);

 //		moveto(CursorPosp[icp],GyZero+GyHaut);
 //		linerel(0,4);
		setcolor(14);
		//position thermocouple + recup
		setlinestyle(SOLID_LINE,NORM_WIDTH,0);

		//display PV and SP

		settextjustify(CENTER_TEXT,BOTTOM_TEXT);
		strcpy(untext,"PV");
	 //	outtextxy(GxZero+GxLarge+31,GyTcpl[GNbZone-1]-5,untext); //ancienne position
		outtextxy(GxZero+GxLarge+textwidth(untext),GyTcpl[GNbZone-1]-5,untext);

		setcolor(10);
		strcpy(untext,"SP");
		outtextxy(GxZero+GxLarge+textwidth(untext),GyTcpl[GNbZone-1]-15,untext);

		strcpy(untext,"Aver.  T.:        Target T.:");
		outtextxy(8,GyTempMoy,untext);
		strcpy(untext,"Bottom T.:        Deviation:");
		outtextxy(8,GyTempInf,untext);
		strcpy(untext,"Top    T.:    ");
		outtextxy(8,GyTempSurf,untext);
		strcpy(untext,"Core   T.:        Res.Time :");
		outtextxy(8,GyTempCore,untext);

		GxActu  =8+textwidth("Aver.  T.:    "); //10
		GxTarget=8+textwidth("Aver.  T.:        Target T. :     ");

		//axis
		setfillstyle(SOLID_FILL,Background_Color);
		bar(0,23,GxZero,Cursor_Bottom);
		bar(0,23,639,GyZero);
		setcolor(15);
		line(GxZero,GyZero+GyHaut,GxZero,GyZero);

		moveto(GxZero,GyZero);
		linerel(-5,5);
//		moverel(5,-5);
//		linerel(5,5);

		//ordinate
		int dtemp = (int)((GMaxTemp/100)*10);
		settextjustify(RIGHT_TEXT,CENTER_TEXT);

		for (int ygrad=dtemp;ygrad < GMaxTemp;ygrad+=dtemp)
			{
			(void)itoa(ygrad,untext,10);
			int ypos = (int)(Gyalpha*(float)ygrad+Gybeta);
			outtextxy(GxZero-7,ypos,untext);
			moveto(GxZero,ypos);
			linerel(-5,0);
			}
		strcpy(untext,"øC");
		settextjustify(RIGHT_TEXT,TOP_TEXT);

		outtextxy(GxZero-textwidth(untext)/2,GyZero-textheight(untext)/2-3,untext);

		settextjustify(RIGHT_TEXT,CENTER_TEXT);
		outtextxy(GxZero+GxLarge+12,GyZero+GyHaut+10,"S");
		}//end if *init == INITIAL

	//zone line
	setlinestyle(DOTTED_LINE,NORM_WIDTH,0);
	setcolor(12);

	for (int xnbgrad = 0;xnbgrad<GNbZone;xnbgrad++)
	{
		moveto(GPosZp[xnbgrad],GyZero+5);
		linerel(0,GyHaut-5);
	}
	setlinestyle(SOLID_LINE,NORM_WIDTH,0);

	for (int xnbz = 0; xnbz < GNbZone; xnbz++)
	{
		if (GPosT[xnbz] > 0)
		{
			moveto(GxTcpl[xnbz],GyTcpl[xnbz]-2);
			linerel(0,4);
			moverel(-3,-2);
			linerel(6,0);
		}
	}
	setcolor(15);
	moveto(GxZero,GyZero);
	linerel(5,5);

}//end Reh_DispBackground
//----------------------------------------------------------------------
//update display with legend or delay window
void Reh_DispLegendOrDelay()
{
	char *untext = "                                       ";

	setcolor(15);

//	if (!V08DispLegend)
//	{
		int rpoint[7];
		rpoint[0]=rpoint[6]=Left_Rectangle2;
		rpoint[1]=rpoint[3]=Top;
		rpoint[2]=rpoint[4]=Right_Rectangle2;
		rpoint[5]=rpoint[7]=Bottom;
		fillpoly(4,rpoint);
//		}
		rectangle(rpoint[0],rpoint[1],rpoint[2],rpoint[5]);

/*	if (V08DispLegend)
		{
		strcpy(untext,"L‚gende");
		settextjustify(CENTER_TEXT,TOP_TEXT);
		outtextxy(524,286,untext);
		settextjustify(LEFT_TEXT,TOP_TEXT);

		int debline = 420+textwidth("Temp. moyenne :");

		if (V08DispTV)
			{
			setcolor(6);
			strcpy(untext,"Temp. voute :");
			outtextxy(415,GyTempsSej,untext);
			line(debline,GyTempsSej+4,634,GyTempsSej+4);
			}

		if (V08DispTM)
			{
			setcolor(3);
			strcpy(untext,"Temp. moyenne :");
			outtextxy(415,GyTempMoy,untext);
			line(debline,GyTempMoy+4,634,GyTempMoy+4);
			}

		if (V08DispTS)
			{
			setcolor(13);
			strcpy(untext,"Temp. surface :");
			outtextxy(415,GyTempSurf,untext);
			line(debline,GyTempSurf+4,634,GyTempSurf+4);
			}

		if (V08DispTC)
			{
			setcolor(4);
			strcpy(untext,"Temp. coeur :");
			outtextxy(415,GyTempInf,untext);
			line(debline,GyTempInf+4,634,GyTempInf+4);
			}

		if (V08DispTarg)
			{
			setcolor(10);
			strcpy(untext,"Objectifs :");
			outtextxy(415,GyTempCore,untext);
			line(debline,GyTempCore+4,634,GyTempCore+4);
			}

		}//V08DispLegend
	else
		{*/
		//display delay and prod rate data
		char *TextProdRate = "    ";
		long ProdRate;

		if (PacingMode == 1)	ProdRate = TheoProd;
		if (PacingMode == 3) ProdRate = MeasProd;
		(void)itoa(ProdRate,TextProdRate,10);

/*		if (ProdRate < 10)
			{
			TextProdRate[2] = TextProdRate[0];
			TextProdRate[0] = TextProdRate[1] = ' ';
			TextProdRate[3] = '\0';
			}
		else if (ProdRate < 100)
			{
			TextProdRate[2] = TextProdRate[1];
			TextProdRate[1] = TextProdRate[0];
			TextProdRate[0] = ' ';
			TextProdRate[3] = '\0';
			}
*/
		//delay  date
		char DelayTitle[30],DelayBeg[11],DelayDur[25];
		char TextDuree[13];
		char ad[4];

/*		switch (DelayType)
		{
			case 1: //immediat delay running
				(void)strcpy(DelayTitle,"Arret imm‚diat");
				(void)strcpy(DelayBeg,"Depuis le ");
				(void)strcpy(DelayDur,"Reste ");
				(void)ltoa(ImmDelRem,TextDuree,10);
				(void)strcat(TextDuree,"mn");
				break;
			case 2: //programmed delay running
*/
//		(void)strcpy(DelayTitle,"Arret programm‚");

		if (DelayState == 1)
			{
			int DelayRem[3];

			DelayRem[0] = (int)(ProgDelRems / 86400);
			DelayRem[1] = (int)((ProgDelRems-86400L*(long)DelayRem[0]) / 3600L);
			DelayRem[2] = (int)((ProgDelRems-86400L*(long)DelayRem[0]-3600L*(long)DelayRem[1]) / 60L)+1;

			sprintf(Reh_TextBegDelay, "%d/%d/%02d %dh%02d", ProgDelDatBeg[2],
						ProgDelDatBeg[1], ProgDelDatBeg[0], ProgDelDatBeg[3],
						ProgDelDatBeg[4]);

			strcpy(DelayTitle,"DELAY");
			strcpy(DelayBeg  ,"Since: ");
			strcpy(DelayDur  ,"Remains: ");
         sprintf(TextDuree, "%dd %02dh %02dmn", DelayRem[0], DelayRem[1], DelayRem[2]);
         strcat(DelayDur, TextDuree);
	//		}
/*				break;
			case 3: //forseen delay
				(void)strcpy(DelayTitle,"Arret programm‚");
				(void)strcpy(DelayBeg,"Le ");
				(void)strcpy(DelayDur,"Pour ");
				(void)ltoa(ProgDelInit[0],TextDuree,10);
				(void)strcat(TextDuree,"j ");
				(void)ltoa(ProgDelInit[1],ad,10);

				if (strlen(ad) == 1)
					{
					ad[1] = ad[0];
					ad[0] = ' ';
					ad[2] ='\0';
					}
				(void)strcat(TextDuree,ad);
				(void)strcat(TextDuree,"h ");

				(void)ltoa(ProgDelInit[2],ad,10);

				if (strlen(ad) == 1)
					{
					ad[1] = ad[0];
					ad[0] = ' ';
					ad[2] ='\0';
					}
				(void)strcat(TextDuree,ad);
				(void)strcat(TextDuree,"mn");
				break;
				default: //nothing to display
*/
/*			else
				{
				(void)strcpy(DelayTitle," ");
				(void)strcpy(DelayBeg," ");
				(void)strcpy(DelayDur," ");
				strcpy(Reh_TextBegDelay," ");
				}
*/
// ************************************
// Affichage du prochain arrˆt sur lot.
// ************************************
/*		int arret_fin_lot = 0;
		arret_fin_lot = (V13BatchDelInit[0] > 0) || (V13BatchDelInit[1] > 0) ;

		// recherche l'arret qui arrivera en premier (fin lot ou programm‚)
		if ( arret_fin_lot && (DelayType==3) )
			{
			int date1[6]={0,0,0,0,0,0};
			int date2[6]={0,0,0,0,0,0};

			for (int i=0;i<5;i++)
				{
				date1[i]=ProgDelDatBeg[i];
				date2[i]=V13BatchDelDatBeg[i];
				}

			if ( date_compare(date1,date2) < 2 ) //arret fin lot en premier
				arret_fin_lot=1;
			else
				arret_fin_lot=0;
			}


		if (arret_fin_lot && (DelayType!=1) && (DelayType!=2) )
			{
			DelayType = 0;
			char BufferChar[40];
			//type d'arrˆt
			strcpy(BufferChar,"Arrˆt fin de lot");
			settextjustify(CENTER_TEXT,TOP_TEXT);
			outtextxy(524,286,BufferChar);
			//date de d‚but d'arrˆt
			sprintf(BufferChar,"Pr‚vu le   %02ld/%02ld/%04ld %02ldh%02ld",
				V13BatchDelDatBeg[0],V13BatchDelDatBeg[1],V13BatchDelDatBeg[2],
				V13BatchDelDatBeg[3],V13BatchDelDatBeg[4]);

			settextjustify(LEFT_TEXT,TOP_TEXT);
			outtextxy(415,GyTempsSej+3,BufferChar);
			//date de red‚marrage production
			sprintf(BufferChar,"Dur‚e :    %02ldh%02ld",
				V13BatchDelInit[0],V13BatchDelInit[1]);
			outtextxy(415,GyTempSurf-2,BufferChar);
			}//arret fin de lot

		else if (DelayType) // autres types d'arrets
			{ */
			GxRemains = Left_Rectangle2+8+textwidth(DelayDur);

			settextjustify(CENTER_TEXT,TOP_TEXT);
			setcolor(15);
			outtextxy(Left_Rectangle2+(Right_Rectangle2-Left_Rectangle2)/2,GyTempMoy,DelayTitle);
			settextjustify(LEFT_TEXT,TOP_TEXT);
			outtextxy(Left_Rectangle2+5,GyTempSurf,DelayBeg);
			outtextxy(Left_Rectangle2+5+textwidth(DelayBeg),GyTempSurf,Reh_TextBegDelay);
			outtextxy(Left_Rectangle2+5,GyTempCore,DelayDur);
//			outtextxy(GxRemains,GyTempCore,TextDuree);
			}
	else
		{
//		line(Left_Rectangle2,GyTempCore-6,639,GyTempCore-6);
		settextjustify(LEFT_TEXT,TOP_TEXT);

		if (PacingMode == 1)
			strcpy(untext,"Theo. Prod.: ");
		if (PacingMode == 3)
			strcpy(untext,"Meas. Prod.: ");

		(void)strcat(untext,TextProdRate);
		(void)strcat(untext," B/h");
		outtextxy(Left_Rectangle2+5,GyTempCore-2,untext);

		}//DelayState
}//end Reh_DispLegendOrDelay
//----------------------------------------------------------------------
//Update the data with the data for the display
void Reh_UpdateData()
 {
  if (!Reh_Active)
  {
//	while(!ecrAccess);
//	ecrAccess = 0;

	//r‚actualiser le tableau du profile de Tø de vo–te
	float absc=0;
	for (int n_pas=0 ; n_pas < NbPas ; n_pas++)
	  {
		 if (CursorPosm[n_pas] <= GFlength)
	 {
		absc = (float)CursorPosm[n_pas] / 1000.000 ;
		TVouteAct[n_pas] = (int) (TheFurn->roofTemp(absc) - 273.00);
	 }
		 else
	 break;
	  }
	//read new furnace data
	//read setpoint and measurement
	Reh_UpdateGraphMeas();

	//read nbpoint as nb products in furnace
	EndBatchMarker[0]=0;
	int nbprod = TheFurn->nbProd();
	int nbpoint = nbprod;
	//last discharged product if any
	//if (Pdis != NULL) nbpoint++;

	//allocate memory for the table
		if (TxAct != NULL)
		{
			delete TxAct;
			TxAct = NULL;
			if (TMoyAct != NULL) delete TMoyAct;
			if (TSurfAct != NULL) delete TSurfAct;
			if (TInfAct != NULL) delete TInfAct;
			if (TSejAct != NULL) delete TSejAct;
			if (TCoreAct != NULL) delete TCoreAct;
			if (TxActp != NULL) delete TxActp;
			if (TMoyActp != NULL) delete TMoyActp;
			if (TSurfActp != NULL) delete TSurfActp;
			if (TInfActp != NULL) delete TInfActp;
			if (TCoreActp != NULL) delete TCoreActp;

#ifdef PACINGMODE
			if (TprodRate != NULL) delete TprodRate; // modif FALCK 16/12/93
#endif

		}

	if (nbpoint>0)
	 {
		TxAct = new long[nbpoint];
		TMoyAct = new long[nbpoint];
		TSurfAct = new long[nbpoint];
		TInfAct = new long[nbpoint];
		TSejAct = new long[nbpoint];
		TCoreAct = new long[nbpoint];
		TxActp = new int[nbpoint];
		TMoyActp = new int[nbpoint];
		TSurfActp = new int[nbpoint];
		TInfActp = new int[nbpoint];
		TCoreActp = new int[nbpoint];
		if (TCoreActp == NULL) nbpoint = 0; //error not enough memory

#ifdef PACINGMODE
		TprodRate = new long[nbpoint];
		for (int indice = 0 ; indice < nbpoint ; indice++)
	 TprodRate[indice]=0;
#endif
	 }
	 else nbpoint = 0;

	if (nbpoint)
	{

	  EnableCurveRecording = 1;

	//read furnace table

	//read product list reference
	Deque& Plist = TheFurn->theProd();
	int markerIndex = 0;
	Batch *ProdBatch = NULL;

	//lire les donn‚es du produit situ‚ au d‚fournement
	Reh_Duree_seuil[0] = -1;
	Reh_Duree_seuil[1] = -1;
/*	if (TheFurn->nbProd())
	  {
		 Product &dernier_prod = (Product &) Plist.peekRight();
		 if ( (dernier_prod.theBatch())->Priority )//produit prioritaire
	 {
		 long duree = dernier_prod.duree_seuil(); //lire la duree (s)
		 if (duree)
			{
			 Reh_Duree_seuil[0] = (int)(duree/3600) ; //heures
			 Reh_Duree_seuil[1] = (int)((duree - 3600*(long)Reh_Duree_seuil[0])/60);
			}
		else
			{
			 Reh_Duree_seuil[0] = 0;
			 Reh_Duree_seuil[1] = 0;
			}
	 }//produit prioritaire
	  }//four non vide
*/
	//initialise an iterator on the list
	ContainerIterator& prodIt = Plist.initIterator();
	//read the product data
	int i = 0;
	while (i < nbprod && int(prodIt))
	{
	  Product& P = (Product&)(prodIt++);
	  TxAct[i] = (long)(1000*P.abscissa());
	  TxActp[i] = (int)(Gxalpha*((float)(TxAct[i]))+Gxbeta);
	  TSejAct[i] = ((long)P.residTime())/60;

	  TMoyAct[i] = (long)P.celMeanTemp();
	  TMoyActp[i] = (int)(Gyalpha*((float)(TMoyAct[i]))+Gybeta);

	  TSurfAct[i] = (long)P.celTopSurfTemp();
	  TSurfActp[i] = (int)(Gyalpha*((float)(TSurfAct[i]))+Gybeta);

	  TInfAct[i] = (long)P.celBotSurfTemp();
	  TInfActp[i] = (int)(Gyalpha*((float)(TInfAct[i]))+Gybeta);

	  TCoreAct[i] = (long)P.celCoreTemp();
	  TCoreActp[i] = (int)(Gyalpha*((float)(TCoreAct[i]))+Gybeta);

#ifdef PACINGMODE
	  TprodRate[i] = (int)P.prodRate(); // modif FALCK
#endif

//   test if the product was first product of a new batch
	  if (ProdBatch != NULL && P.theBatch() != ProdBatch)
		{
		 EnableCurveRecording = 0;
		 int j=0;
		 while (CursorPosp[j] < TxActp[i-1]) j++;
		 if (j)
	{
	 EndBatchMarker[markerIndex] = CursorPosp[j-1];
	}
	else EndBatchMarker[markerIndex] = CursorPosp[0];

		 int numtype = ProdBatch->TypeNumber;
		 //int numtype = (P.theBatch())->TypeNumber;
		 EndBatchType[markerIndex] = &(TheTypeList->getType(numtype));
       EndBatchProd[markerIndex] = ProdBatch->ProductionRate;
		 Reh_TheBatch[markerIndex] = ProdBatch;

       EndBatchMarker[++markerIndex] = 0;
      }

	  ProdBatch = P.theBatch();
	  if (!int(prodIt))
		{
		 //next to discharge
/*		 if (Pdis != NULL)
	 {
	  if (ProdBatch != Pdis->theBatch())
		 {
		  int j=0;
		  while (CursorPosp[j] < TxActp[i]) j++;
		  if (j)
			{
			 EndBatchMarker[markerIndex] = CursorPosp[j-1];
			}
			else EndBatchMarker[markerIndex] = CursorPosp[0];
//	     EndBatchMarker[markerIndex] = TxActp[i];
		  int numtype = (P.theBatch())->TypeNumber;
		  EndBatchType[markerIndex] = &(TheTypeList->getType(numtype));
		  EndBatchProd[markerIndex] = (P.theBatch())->ProductionRate;
		  //FDFALCK
		  Reh_TheBatch[markerIndex] = P.theBatch();

		  EnableCurveRecording = 0;
//           int numtype = (P.theBatch())->TypeNumber;
//           EndBatchType[markerIndex] = &(TheTypeList->getType(numtype));
		  EndBatchMarker[++markerIndex] = 0;
		 }
	  EndBatchMarker[markerIndex] = GPosZp[GNbZone-1]+23;
	  int numtype = (Pdis->theBatch())->TypeNumber;
	  EndBatchType[markerIndex] = &(TheTypeList->getType(numtype));
	  EndBatchProd[markerIndex] = (Pdis->theBatch())->ProductionRate;

	  //FDFALCK
	  Reh_TheBatch[markerIndex] = Pdis->theBatch();
	 }
	else
	 {
*/	  int j=0;
	  while (CursorPosp[j] < TxActp[i]) j++;
	  if (j)
		{
		 EndBatchMarker[markerIndex] = CursorPosp[j-1];
		}
	  else EndBatchMarker[markerIndex] = CursorPosp[0];
//	  EndBatchMarker[markerIndex] = TxActp[i];
	  int numtype = (P.theBatch())->TypeNumber;
	  EndBatchType[markerIndex] = &(TheTypeList->getType(numtype));
	  EndBatchProd[markerIndex] = (P.theBatch())->ProductionRate;

	  //FDFALCK
	  Reh_TheBatch[markerIndex] = P.theBatch();

	  EnableCurveRecording = 0;
//	 }
//       int numtype = (P.theBatch())->TypeNumber;
//       EndBatchType[markerIndex] = &(TheTypeList->getType(numtype));
		 EndBatchMarker[++markerIndex] = 0;
		}
	  i++;
	} //end read the product data

	//destroy the iterator
	delete &prodIt;

	//update data with last discharged product
/*	if (Pdis != NULL)
	 {
	  i = nbpoint-1;
	  TxAct[i] = GPosZ[GNbZone-1]; //position at furnace exit
	  TxActp[i] = GPosZp[GNbZone-1];
	  TSejAct[i] = ((long)Pdis->residTime())/60;

	  TMoyAct[i] = (long)Pdis->celMeanTemp();
	  TMoyActp[i] = (int)(Gyalpha*((float)(TMoyAct[i]))+Gybeta);

	  TSurfAct[i] = (long)Pdis->celTopSurfTemp();
	  TSurfActp[i] = (int)(Gyalpha*((float)(TSurfAct[i]))+Gybeta);

	  TInfAct[i] = (long)Pdis->celBotSurfTemp();
	  TInfActp[i] = (int)(Gyalpha*((float)(TInfAct[i]))+Gybeta);

	  TCoreAct[i] = (long)Pdis->celCoreTemp();
	  TCoreActp[i] = (int)(Gyalpha*((float)(TCoreAct[i]))+Gybeta);

	  if (!i)
		{
		 //furnace is empty, make batch marker for Pdis
		 EndBatchMarker[markerIndex] = GPosZp[GNbZone-1]+23;
		 int numtype = (Pdis->theBatch())->TypeNumber;
		 EndBatchType[markerIndex] = &(TheTypeList->getType(numtype));
		 EndBatchProd[markerIndex] = (Pdis->theBatch())->ProductionRate;

		 //FDFALCK
		 Reh_TheBatch[markerIndex] = Pdis->theBatch() ;

		 EndBatchMarker[++markerIndex] = 0;
		}
	 }
*/	}//nbpoint

	//save number of points for other functions
	NbActPoint = nbpoint;

	//update reference curve
	Reh_ReadRefCurve();

	//uptade TMoyEcart
	int itme = 0;
	TMoyEcart[0] = -1;
//   for (int nzec=0;nzec<GNbZone;EndZEcart[nzec] = -1,nzec++);
	if (nbpoint)
	{
		float DX,Pente,Ordorg;
		int lastxp = GxZero+1;
		int lasttm = TMoyActp[0];
		TMoyEcart[0] = lasttm;

		for(int ip = 0;ip<nbpoint;ip++)
		{
			DX = (float)(TxActp[ip]-lastxp);

			if (DX)
			{
					Pente = ((float)(TMoyActp[ip]-lasttm))/DX;
					Ordorg = (float)lasttm - Pente*(float)lastxp;

					for (;lastxp<=TxActp[ip];lastxp++)
					{
						TMoyEcart[itme++] = int(lastxp*Pente+Ordorg);
					}
					lasttm = TMoyEcart[itme-1];
			}//DX
	  }//for ip
	 TMoyEcart[itme] = -1;

	 //Check product temp at each end of zone
/*
	 int lastpix = GxZero;
	 itme=0;
	 int nze = 0;
	 while(TMoyEcart[itme] != -1)
	  {
		lastpix++;
		if (lastpix > GPosZp[nze] && nze < GNbZone -1) nze++;
		EndZEcart[nze] = TMoyEcart[itme];
		itme++;
	  }
	 //Uses last discharged product for last zone if furnace not stopped
	 if (Pdis != NULL && !DelayState)
	EndZEcart[GNbZone-1] = TMoyActp[nbpoint-1];
*/
	}//nbpoint

	//update the next reference point
	//last zone : Obj is last point mean temp if point is beyong half zone
	//previous zone : Obj is intersection of mean curve with end of zone
	if (nbpoint>1)
	 {

	  //if only one zone
	  int nz2,nz = 0;
	  if (GNbZone == 1)
		{
		 if ( TxAct[nbpoint-1] >= (2*GPosZ[0])/3 )
	 {
	  NextZoneRefp[0] = TMoyActp[nbpoint-1];
	  NextZoneRef[0] = TMoyAct[nbpoint-1];
	 }
	 else
	 {
	  NextZoneRefp[0] = 0;
	  NextZoneRef[0] = 0;
	 }
		}
		else
		 //last zone with product
		{
		 while(nz < GNbZone && TxAct[nbpoint-1] > GPosZ[nz]) nz++;
		 if (nz >= GNbZone-1)
	{
		if (TxAct[nbpoint-1] >= (GPosZ[GNbZone-2]
					+ 2*GPosZ[GNbZone-1])/3)
		 {
		  NextZoneRefp[GNbZone-1] = TMoyActp[nbpoint-1];
		  NextZoneRef[GNbZone-1] = TMoyAct[nbpoint-1];
		 }
		else
	    {
		  NextZoneRefp[GNbZone-1] = 0;
		  NextZoneRef[GNbZone-1] = 0;
	    }
	  nz = GNbZone-1;
	}
       else
	{
	 for (nz2 = nz; nz2 < GNbZone; nz2++)
	  {
	   NextZoneRefp[nz2] = 0;
		NextZoneRef[nz2] = 0;
	  }
	}
	//previous zone
	int xact = 0;
	for (nz2 = 0; nz2 < nz; nz2++)
	 {
	  while(TxAct[xact] < GPosZ[nz2]) xact++;
	  if (xact)
	   {
		 if( (GPosZ[nz2] - TxAct[xact-1]) <
			(TxAct[xact] - GPosZ[nz2]) ) xact--;
	   }
	  NextZoneRefp[nz2] = TMoyActp[xact];
	  NextZoneRef[nz2] = TMoyAct[xact];
	 }
	  } //GNbZone == 1
    }
    else //nbpoint
	 {
     for (int nz3 = 0; nz3 < GNbZone; nz3++)
      {
       NextZoneRef[nz3] = 0;
       NextZoneRefp[nz3] = 0;
		}

	 } //nbpoint > 1



	//update roof data
   //read a pointer to roof temp array
   float *artemp = TheFurn->theRoofTemp();
//   float *aftemp = TheFurn->theFumeTemp(); //provisoire pour test
	long dtxRoof = GFlength/100;
   for (int h=0;h<100;h++)
	 {
     TxRoofp[h] = (int)(Gxalpha*((float)(h*dtxRoof))+Gxbeta);
     TRoofp[h] = (int)(Gyalpha*(*artemp-273) + Gybeta);
	  //provisoire pour test
//     TFumep[h] = (int)(Gyalpha*(*aftemp-273) + Gybeta);
//     aftemp++;
	  //fin provisoire pour test
	  artemp++; //point to next value
	 }// for 100 points

	//record targets if requested
//   if (Reh_RecordRefReq) Reh_RecordRefCurve();

	//free access
 //	ecrEnableAccess();
  }//!Reh_Active
 }//end Reh_UpdateData

//-------------------------------------------------------------------
//update setpoints and measurments
void Reh_UpdateGraphMeas()
 {
   Array& FZ = TheFurn->theZone();
   for (int nspt = 1; nspt < GNbZone; nspt++)
      {
		 GZspt[nspt] = (long)((Zone&)(FZ[nspt])).celSetPoint();
      }

	//read the temperature
  for (int ntemp = 0; ntemp < GNbZone; ntemp++)
     {
      GZtemp[ntemp] = (long)((Zone&)(FZ[ntemp])).celTemp();
     }
  GZtemp[maxNbZone] = (long)TheFurn->celRecupTemp();

 }//end updateGraphMeas

//---------------------------------------------------------------------
//read a reference curve in the list, the argument is the file name string
void Reh_ReadRefCurve()
 {
#ifndef PACINGMODE
  int EmptyZone[maxNbZone];
  int *targets,nbtarg;
  //newCurveSelected = 0;
  RefCurve = 0;
  //init
  for (int nz1 = 0; nz1 < maxNbZone; nz1++)
	{
	 EndZoneRef[nz1] = NextZoneRef[nz1] = 0;
	 EndZoneRefp[nz1] = NextZoneRefp[nz1] = 0;
	 EmptyZone[nz1] = 1;
	}

  //check non empty zones
  if (NbActPoint)
	{
	 int ipr = NbActPoint-1;
	 int nz1 = GNbZone-1;
	 while (ipr >= 0 && nz1 >= 0)
	  {
		long anabs = TxAct[ipr];
		if (nz1)
	{
	 if (anabs >= GPosZ[nz1-1] && anabs < GPosZ[nz1])
		{
		 EmptyZone[nz1] = 0;
		 nz1--;
		}
	}
		 else if (anabs < GPosZ[0])
	{
	 EmptyZone[0] = 0;
	 nz1--;
	}
		ipr--;
	  }//ipr
	}//NbActPoint

  int markerIndex=0;

  while(EndBatchMarker[markerIndex]) markerIndex++;
  if (markerIndex)
	{
	 markerIndex--;
	 //targets to end of furnace
	 int ntyp = EndBatchType[markerIndex]->number();
	 int pph = EndBatchProd[markerIndex];
	 //EndzTarget& aCurve = TheTargetList->getActiveCurve(ntyp);
	 EndzTarget& aCurve = TheTargetList->getCurve(pph,ntyp,0);
	 if (aCurve != (EndzTarget&)NOOBJECT)
		{
		 int iz;
		 targets = aCurve.theCurve();
		 nbtarg = aCurve.nbZone();
		 if (nbtarg>GNbZone) nbtarg=GNbZone;
		 for (iz=0;iz<nbtarg;iz++) EndZoneRef[iz] = (long)targets[iz];
		 iz = 0;
		 while (iz < GNbZone && EndBatchMarker[markerIndex] >= GPosZp[iz]) iz++;
		 for (int iz1 = iz; iz1 < GNbZone; EmptyZone[iz1] = 0, iz1++);
		 RefCurve=1;
		}//aCurve
	 markerIndex--;

  int nbpoint;
/*  if (Pdis != NULL)
	{
	 nbpoint = NbActPoint-1;
	}
	else*/ nbpoint = NbActPoint;

  if (markerIndex >= 0 && nbpoint)
	{
	 int nz,ipr,icp;
	 while (markerIndex>=0)
	 {
	  nz = 0;
	  ipr = nbpoint - 1;
	  while(nz < GNbZone && EndBatchMarker[markerIndex] >= GPosZp[nz]) nz++;
	  icp=0;
	  while (CursorPosp[icp] <= EndBatchMarker[markerIndex]) icp++;
	  //check product in nz beyong the marker
	  int found = 0;
	  while (!found && ipr >= 0)
		{
		 if (TxAct[ipr] < GPosZ[nz]
			&& TxAct[ipr] > CursorPosm[icp]) found++;
		 ipr--;
		}
	  int znb;
	  if (found)
		{
		 //make refCurve from first zone to znb = nz-1
		 znb = nz-1;
		}
		else znb = nz; //make refCurve until end of nz

	  if (znb >= 0)
		{
		 //make refCurve for forward zone if empty
		 while (znb < GNbZone-1 && EmptyZone[znb+1])
	{
	 znb++;
	 EmptyZone[znb]=0;
	}

		 ntyp = EndBatchType[markerIndex]->number();
		 pph = EndBatchProd[markerIndex];
		 //EndzTarget& bCurve = TheTargetList->getActiveCurve(ntyp);
		 EndzTarget& bCurve = TheTargetList->getCurve(pph,ntyp,0);
		 if (bCurve != (EndzTarget&)NOOBJECT)
	 {
	  targets = bCurve.theCurve();
	  nbtarg = bCurve.nbZone();
	  if (nbtarg>=znb) nbtarg=znb+1;
	  for (int iz=0;iz<nbtarg;iz++) EndZoneRef[iz] = (long)targets[iz];
	  RefCurve=1;
	 }//bCurve
	else
	 {
	  for (int iz=0;iz<=znb;iz++) EndZoneRef[iz]=0;
	 }
      }//!found
     markerIndex--;
    }//while markerIndex
	}//markerIndex && nbpoint
  }//marker index

#else
	// en mode CADENCEMENT, les objectifs de zones sont definis par la cadence
   // equivalente des produits situ‚s en fin de zone

	Deque& Plist = TheFurn->theProd();

	//modif jlc++

	DoubleListIterator& prodIt =
	  (DoubleListIterator&)(Plist.initIterator() );

	while (int(prodIt)) prodIt++;
	prodIt--;
	//end modif jlc++

	int i    = 0;
	int znum = GNbZone ;
	int fin     = 0;
	int fin_inc = 0;
	long fin_zone_prec = 0 ;
	int PrefZone[maxNbZone] ;

	RefCurve = 1;
	// init objectifs + PrefZone … 0
	for (i=0 ; i<maxNbZone ; i++)
	 {
		PrefZone[i]  = 0;
		EndZoneRef[i] =0;
		EndZoneRefp[i]=0;
	 }

//	if ( Pdis != NULL ) prodIt-- ;

	while ( int(prodIt) && !fin_inc)
	  {
		 Product& P = (Product&)(prodIt--);
		 long P_abs = (long)  1000.0*P.abscissa() ;
		 znum = GNbZone-1;
		 fin  = 0;

		 while ( !fin )
	 {
		if ( znum == 0 ) fin_zone_prec = 0 ;
		 else       fin_zone_prec = GPosZ[znum-1] ;
		if ( P_abs <= GPosZ[znum] && P_abs > fin_zone_prec && !PrefZone[znum] )
		 {
		  int P_cad  = P.prodRate();
		  int P_type = (P.theBatch())->TypeNumber;
		  int *targets ;
		  // recherche de l'objectif
		  EndzTarget& la_courbe = TheTargetList->getCurve(P_cad,P_type,0);
		  if (la_courbe != (EndzTarget&)NOOBJECT)
		{
		   targets = la_courbe.theCurve();
		   EndZoneRef[znum] = (long)targets[znum];
		}
	     else
		   EndZoneRef[znum] = 0;

	     PrefZone[znum]=1;
	     fin = 1;
	    }
	   if ( !znum ) fin = 1;
	   znum--;
	 }//while znum
		 if ( PrefZone[0] ) fin_inc = 1;
	  }//while

	  delete &prodIt ;
#endif

  if (RefCurve)
   {
     for (int jz=0;jz<GNbZone;jz++)
      {
       if (EndZoneRef[jz])
	EndZoneRefp[jz] = (int)(Gyalpha*((float)(EndZoneRef[jz]))+Gybeta);
      }
	}//refcurve
   //save the data in zone object
   Array& FZ = TheFurn->theZone();
   for (int kz=0;kz<GNbZone;kz++)
     ((Zone&)(FZ[kz])).setTarget((float)EndZoneRef[kz]);
 }

//--------------------------------------------------------------------
//record a new reference curve with actual curve
/*void Reh_RecordRefCurve()
 {

  if (EnableCurveRecording)
  {
	//calculate production (nbprod per hour)
	float nbpinf = TheFurn->length() / (Pdis->theBatch())->DefaultAbsGap;
	float tsej = Pdis->residTime();
	float fpperh = 3600*nbpinf/tsej;
	int pperh = (int)fpperh;
	if (fpperh-pperh > .5) pperh++;
	int ptype = (Pdis->theBatch())->TypeNumber;
	int targets[maxNbZone];
	for (int lp = 0; lp < GNbZone; lp++) targets[lp] = (int)NextZoneRef[lp];
	V06SaveNewTarget(pperh,ptype,(int)GNbZone,targets);
	Reh_ReadRefCurve();
//   V08DispTarg = 1;
  } //EnableCurveRecording

  Reh_RecordRefReq = 0;

 }//end Reh_RecordRefCurve
*/
//----------------------------------------------------------------------
/*
//setpoint blinking
void Reh_BlinkSP()
  {
	int nspt = 1, nextblinkingcolor = -1;
	while (nextblinkingcolor == -1 && nspt < GNbZone)
	 {

	  if (Reh_BlinkingSP[nspt])
		{
		 if (Reh_TrueSPColor[nspt] == 60)
	 {
	  nextblinkingcolor = 0;
	 }
	 else nextblinkingcolor = 60;
      }//if must blink
	 nspt++;
   }

	for (nspt = 1; nspt < GNbZone; nspt++)
	 {
	  if (Reh_BlinkingSP[nspt])
		{
		 setpalette(Reh_SPColor[nspt],nextblinkingcolor);
		 Reh_TrueSPColor[nspt] = nextblinkingcolor;
		}
	  else
		{
		 setpalette(Reh_SPColor[nspt],60);
		 Reh_TrueSPColor[nspt] = 60;
		}
	 }

	//blinks measurment
	if (Reh_BlinkingPV)
	 {
	  if (nextblinkingcolor == 62)
		{
		 nextblinkingcolor = 60;
		}
	  else if (nextblinkingcolor == -1)
		{
		 if (!Reh_TruePVColor)
	{
	 nextblinkingcolor = 60;
	}
	else nextblinkingcolor = 0;
		}
	  Reh_TruePVColor = nextblinkingcolor;
	 }
	 else Reh_TruePVColor = 60;
	setpalette(12,Reh_TruePVColor);

  }//end blinking

*/
 //-------------------------------------------------------------
 //refresh the measurment on the screen
/* void refreshMeas()
 {
	char masque[5] = {219,219,219,219,'\0'};
	char *untext = "                           ";
	time_t t;


	t = time(NULL);
	if (t%RefreshMeasPer == 0 && oldt != t && V01WorkMode == 1)

	{
	  oldt = t;
	  ReadMeas(); //lit les donn‚es dans la base applicom
	  Reh_UpdateGraphMeas();
  //update the display

//************FIRST PAGE ****************************
		setactivepage(0);
		settextjustify(CENTER_TEXT,BOTTOM_TEXT);

//effacement des valeurs de temp‚ratures
		setcolor(0);
		for (int ntemp = 0; ntemp < GNbZone; ntemp++)
	{
	 outtextxy(GxTcpl[ntemp],GyTcpl[ntemp]-5,masque);
	}

	  //display trec
	  outtextxy(GxTcpl[maxNbZone],GyTcpl[maxNbZone]-5,masque);

//affichage des valeurs de temp‚ratures
	  setcolor(14);
	  if (!Reh_BlinkingPV)
		 {
	Reh_TruePVColor = 60;
		 }

	  for (ntemp = 0; ntemp < GNbZone; ntemp++)
		{
		 //display the data
		 if (GPosT[ntemp] > 0)
	{
	 (void)ltoa(GZtemp[ntemp],untext,10);
	 outtextxy(GxTcpl[ntemp],GyTcpl[ntemp]-5,untext);
	}
		}
	  //display trec
	  (void)ltoa(GZtemp[maxNbZone],untext,10);
	  outtextxy(GxTcpl[maxNbZone],GyTcpl[maxNbZone]-5,untext);

// ********************SECOND PAGE********************
		setactivepage(1);
		settextjustify(CENTER_TEXT,BOTTOM_TEXT);
//effacement des valeurs de temp‚ratures
		setcolor(0);
		for (ntemp = 0; ntemp < GNbZone; ntemp++)
	{
	 outtextxy(GxTcpl[ntemp],GyTcpl[ntemp]-5,masque);
	}

	  //display trec
	  outtextxy(GxTcpl[maxNbZone],GyTcpl[maxNbZone]-5,masque);

//affichage des valeurs de temp‚ratures
	  setcolor(14);
	  if (!Reh_BlinkingPV)
		 {
	Reh_TruePVColor = 60;
		 }

	  for (ntemp = 0; ntemp < GNbZone; ntemp++)
		{
		 //display the data
		 if (GPosT[ntemp] > 0)
	{
	 (void)ltoa(GZtemp[ntemp],untext,10);
	 outtextxy(GxTcpl[ntemp],GyTcpl[ntemp]-5,untext);
	}
		}
	  //display trec
	  (void)ltoa(GZtemp[maxNbZone],untext,10);
	  outtextxy(GxTcpl[maxNbZone],GyTcpl[maxNbZone]-5,untext);
	}// t%

}
*/


//----------------------------------------------------------------------
//end REH_OVL.CPP
