//-------------------------------------------------------------------------
// Prod_INT1.CPP
// This file contains non overlay function for project TACT
// display number 1
//
// Owning include file : Prod_DATA1.H
//
// Creation : C.Martinet 18/12/92
// Modified :            02/02/93 display the hour
//			 04/02/93 add cursor position on cursor arrow
//-------------------------------------------------------------------------

#define _OWNER2_Prod_DATA1_H

#ifndef __DATABASE_H
#include "database.h"
#endif
#ifndef __DB_H
#include "db.h"
#endif
#ifndef __STDLIB_H
#include <stdlib.h>
#endif
#ifndef __CTYPE_H
#include <ctype.h>
#endif
#ifndef __GLOBVIEW_H
#include <globview.h>
#endif
#ifndef __PRODDATA_H
#include "Proddata.h"
#endif
#ifndef __CONFIG_H
#include "config.h"
#endif
//------------------------------------------------------------------------
//functions

//update the display with cursor position, parameter action tells the
//function what to do : key code for interrupt, 0 for simple update
//second argument is action for date acquisition cursor
void Prod_UpdateHPGraphCursor(int action,int tcurdisplac)
{
  //Access to ecr table is enabled by the possible callers of this
  //routine.
  //test if cursor movement is to be performed
	switch(action)
	{
		case Right : //right arrow
			if (Prod_CursorPosp[Prod_CursorPos] >= Prod_GPosZp[Prod_GNbZone-1]-1)
			{
				Prod_CursorPos = 0;
			}
			else
			{
				Prod_CursorPos += Prod_CursorInc;

				if (Prod_CursorPosp[Prod_CursorPos] >= Prod_GPosZp[Prod_GNbZone-1]-1)
				{
					Prod_CursorPos = 0;
				}
			}  	break;

		case Left : //left arrow
			if (Prod_CursorPos <= 0)
			{
//				while(Prod_CursorPosp[Prod_CursorPos]
//					<= Prod_GPosZp[Prod_GNbZone-1]) Prod_CursorPos++;

				Prod_CursorPos=Prod_NbPas-1;
			}
			else
			{
				Prod_CursorPos -= Prod_CursorInc;

				if (Prod_CursorPos < 0)
				{
					Prod_CursorPos = 0;
				}
			}  	break;
	}//switch
/*
	if(*init==INITIAL)
	{//update the display

		if (VGPageVisu == 0)
			VGPageActive = 1;
		else
			VGPageActive = 0;

		setactivepage(VGPageActive);
	}
*/
  //update date & time
	if(*init==NORMAL)
		Prod_DispDates();

  //update acquisition cursor
	Prod_AcqHPCursor(tcurdisplac);


  //display background
	setfillstyle(SOLID_FILL,Background_Color);
	bar(0,Cursor_Top-1,Prod_GxZero+Prod_GxLarge+5,Cursor_Bottom);
	setfillstyle(SOLID_FILL,1);

//	setviewport(0,265,Prod_GxZero+Prod_GxLarge+10,282,0);   //
//	clearviewport();                                    //  VB:OLd
//  setviewport(0,0,639,349,0);                        //
  int rpoint[8];

//  end modif
	bar(Prod_GxActu   -34,Prod_GyTempMoy ,Prod_GxActu   -1,Prod_GyTempCore+6);
	bar(Prod_GxTarget -50,Prod_GyTempMoy ,Prod_GxTarget -1,Prod_GyTempCore+6);
	bar(Prod_GxRef    -42,Prod_GyTempMoy ,Prod_GxRef    -1,Prod_GyTempCore+6);

//	bar(Prod_GxActu-40,Prod_GyTempsSej,Prod_GxActu-1,Prod_GyTempCore+9);
//	bar(Prod_GxTempTV-textwidth("1234")-2,Prod_GyTempTV,Prod_GxTempTV,Prod_GyTempMoy-2);
//	bar(Prod_GxRef -60,Prod_GyTempInf ,Prod_GxRef+55,Prod_GyTempCore-2);
//	bar(Prod_GxType-40,Prod_GyTempsSej,Prod_GxType-1,Prod_GyTempCore+9);

	settextjustify(RIGHT_TEXT,TOP_TEXT);
	setcolor(15);

	if (Prod_NbActPoint)
	{
		int xact = 0;

		if(Prod_CursorPosm[Prod_CursorPos] <= Prod_TxAct[Prod_NbActPoint-1])
		{
			long tabval[7];
			char *tabtext[7] = {"        ","        ","        ","        ",
				 "        ","        ","        "};
			while (xact < Prod_NbActPoint-1 &&
				Prod_CursorPosm[Prod_CursorPos] > Prod_TxAct[xact]) xact++;

			tabval[0] = Prod_TMoyAct[xact];
			tabval[1] = Prod_TSurfAct[xact];
			tabval[2] = Prod_TInfAct[xact];
			tabval[3] = Prod_TCoreAct[xact];
			tabval[4] = Prod_TObjAct[xact];
			tabval[5] = Prod_TTvAct[xact];
			tabval[6] = Prod_TSejAct[xact];

			for (int it = 0; it<7; it++) (void)ltoa(tabval[it],tabtext[it],10);

			strcat(tabtext[4],"øC");
			strcat(tabtext[6]," ");

			outtextxy(Prod_GxActu  ,Prod_GyTempMoy ,tabtext[0]);
			outtextxy(Prod_GxActu  ,Prod_GyTempSurf,tabtext[1]);
			outtextxy(Prod_GxActu  ,Prod_GyTempInf ,tabtext[2]);
			outtextxy(Prod_GxActu  ,Prod_GyTempCore,tabtext[3]);
			outtextxy(Prod_GxTarget,Prod_GyTempMoy ,tabtext[4]);
			outtextxy(Prod_GxTarget,Prod_GyTempCore,tabtext[6]);

  //			tabval[0] = (long)(Prod_ProdType);
  //			tabval[1] = (long)(Prod_ProdWidi);

//			for (it = 0; it<2; it++)
//				(void)ltoa(tabval[it],tabtext[it],10);
			tabtext[0]=(char*)Prod_JobID;
			tabtext[1]=(char*)Prod_PONumber;
			tabtext[2]=Prod_SteelName;

			outtextxy(Prod_GxRef,Prod_GyTempMoy,tabtext[0]);
			outtextxy(Prod_GxRef,Prod_GyTempInf,tabtext[1]);
			outtextxy(Prod_GxRef,Prod_GyTempSurf,tabtext[2]);
			outtextxy(Prod_GxRef,Prod_GyTempCore,tabtext[3]);

			if (tabval[6])
			{
				char ecarttext[9];
				strcpy(ecarttext,"        ");
				Prod_DeltaRefActu = Prod_TMoyAct[xact] - tabval[4];
				(void)ltoa(Prod_DeltaRefActu,tabtext[5],10);
				strcat(tabtext[5],"øC");
				outtextxy(Prod_GxTarget,Prod_GyTempInf ,tabtext[5]);

//				int alen = strlen(ecarttext);
//				(void)strcpy(&ecarttext[alen],"/");
//				outtextxy(Prod_GxRef,Prod_GyTempInf,ecarttext);
//				alen = strlen(tabtext[6]);
//				(void)strcpy(&tabtext[6][alen],"øC");
//				(void)strcat(tabtext[6],"øC");
//				settextjustify(LEFT_TEXT,TOP_TEXT);
//				outtextxy(Prod_GxRef,Prod_GyTempInf,tabtext[6]);

//				tabval[0] = (long)(EndBatchType[markerIndex]->number());
//				tabval[2] = (long)(EndBatchType[markerIndex]->tdef());

//					for (it = 0; it < 4; it++)
//						(void)ltoa(tabval[it],tabtext[it],10);

//					char **thequal = EndBatchType[markerIndex]->theCostumerQual();
			}
		}//cursorPos
	}//nbpoint

  //display cursor

	moveto(Prod_CursorPosp[Prod_CursorPos],Cursor_Bottom);        //282
	linerel(0,-14);
	linerel(-5,5);
	moverel(5,-5);
	linerel(5,5);
	char *labscur = "   ";

	if (!Prod_CursorPos)
	{
		(void)strcpy(labscur,"E");
	}
	else if (Prod_CursorPosp[Prod_CursorPos] >= Prod_GPosZp[Prod_GNbZone-1])
	{
		(void)strcpy(labscur,"S");
	}
	else
	{
		(void)ltoa(Prod_CursorPos,labscur,10);
	}

	settextjustify(RIGHT_TEXT,BOTTOM_TEXT);
	outtextxy(Prod_CursorPosp[Prod_CursorPos]-1,Cursor_Bottom,labscur);
	settextjustify(CENTER_TEXT,TOP_TEXT);
	setcolor(15);

//	bar(580,2,638,12);
//	outtextxy(615,3,Prod_Hour);
//	VGPageVisu = VGPageActive;
//	setvisualpage(VGPageVisu);
}//end graphcursor

//--------------------------------------------------------------------
//acquisition cursor displaying
//argument displac is -1 for previous, 1 for next, 0 to display at current
//position

void Prod_AcqHPCursor(int displac)
{
	if (!Prod_Active)
		Prod_curAcqVal = 1;
	else
	{
		Prod_curAcqVal += displac;

		if (Prod_curAcqVal <= 0)
			Prod_curAcqVal = 20;

		else if (Prod_curAcqVal > 20)
			Prod_curAcqVal = 1;

		else if (Prod_curAcqVal == 3 || Prod_curAcqVal == 6 ||
			Prod_curAcqVal == 11 || Prod_curAcqVal == 14)
		{
			if (displac == -1)
				Prod_curAcqVal--;
			else
				Prod_curAcqVal++;
		}
		else if (Prod_curAcqVal == 17 || Prod_curAcqVal == 18)
		{
			if (displac == -1)
				Prod_curAcqVal = 16;
			else
				Prod_curAcqVal = 19;
		}
	}//!Prod_Active

	 //erase the cursor
	setcolor(0);
	moveto(Prod_GxDateAcq-5,Prod_GyDate+textheight("h")+2);
	lineto(Prod_GxDateAcq+textwidth(Prod_DDateAcqText)+5,
					Prod_GyDate+textheight("h")+2);

	if (Prod_Active)
	{
		//display the cursor
		setcolor(14);
		char *untext = "                    ";
		(void)strncpy(untext,Prod_DDateAcqText,Prod_curAcqVal);
		untext[Prod_curAcqVal] = '\0';
		moveto(Prod_GxDateAcq+textwidth(untext)-textwidth("0"),
				Prod_GyDate+textheight("h")+2);
		linerel(textwidth("0"),0);
	}
}//end acq cursor

//--------------------------------------------------------------------
//display the discharging date
void Prod_DispDates()
{
	APPELHS ("Option,Couleur,S=15");    //240
	APPELHS ("Option,Couleur,A=15");    //240

	APPELHS("Affiche,HEURE=%d",Prod_DispDateAcq[3]);
	APPELHS("Affiche,MIN  =%d",Prod_DispDateAcq[4]);
	APPELHS("Affiche,SEC  =%d",Prod_DispDateAcq[5]);
	APPELHS("Affiche,JOUR =%d",Prod_DispDateAcq[2]);
	APPELHS("Affiche,MOIS =%d",Prod_DispDateAcq[1]);
	APPELHS("Affiche,AN   =%d",Prod_DispDateAcq[0]);

	APPELHS("Affiche,HEURE2=%d",Prod_DispDate[3]);
	APPELHS("Affiche,MIN2  =%d",Prod_DispDate[4]);
	APPELHS("Affiche,SEC2  =%d",Prod_DispDate[5]);
	APPELHS("Affiche,JOUR2 =%d",Prod_DispDate[2]);
	APPELHS("Affiche,MOIS2 =%d",Prod_DispDate[1]);
	APPELHS("Affiche,AN2   =%d",Prod_DispDate[0]);
}
//---------------------------------------------------
/*
void Prod_DispHour(char *theHour)
{
	if (strcmp(Prod_Hour,theHour))
	{
		(void)strcpy (Prod_Hour,theHour);

 //		if (VGDispViewNb == 11)
 //		{
//			setactivepage(VGPageVisu);
			settextjustify(CENTER_TEXT,TOP_TEXT);
			setcolor(15);
			bar(580,2,638,12);
			outtextxy(615,3,Prod_Hour);
//			setactivepage(VGPageActive);
//		}
	}
}//end Prod_DispHour
//-------------------------------------------------------------------
//end Prod_INT1.CPP
*/