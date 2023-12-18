//-------------------------------------------------------------------------
// Furn_INT1.CPP
// This file contains non overlay function for project TACT
// display number 9
//
// Owning include file : Furn_DATA1.H
//
// Creation : C.Martinet 18/12/92
// Modified :		 02/02/93 display the hour
//
//         F Destrebecq  16/06/93 add Furn_CursorAction in Furn_TestKey ()
//
//                       30/05/94 modif JLC pour bugg date
//-------------------------------------------------------------------------
#define _OWNER_Furn_DATA1_H

#ifndef _GRAPHICS_H
#include <graphics.h>
#endif
#ifndef __CONIO_H
#include <conio.h>
#endif
#ifndef __STRING_H
#include <string.h>
#endif
#ifndef __DOS_H
#include <dos.h>
#endif
#ifndef __STDLIB_H
#include <stdlib.h>
#endif
#ifndef __STDIO_H
#include <stdio.h>
#endif
#ifndef __CTYPE_H
#include <ctype.h>
#endif
#ifndef __GLOBVIEW_H
#include <globview.h>
#endif
#ifndef __Furn_DATA1_H
#include "furndata.h"
#endif
#ifndef __V10DATA1_H
#include "v10data1.h"
#endif
#include "config.h"
#include "appelhs.h"
//------------------------------------------------------------------------
//functions

//update the display with cursor position, parameter action tells the
//function what to do : key code for interrupt, 0 for simple update
//second argument is action for date acquisition cursor
void Furn_UpdateGraphCursor(int action,int tcurdisplac)
{
  //Access to ecr table is enabled by the possible callers of this
  //routine.
  //test if cursor movement is to be performed
  //
	switch(action)
	{
		case Right : //right arrow
		if (Furn_ActCursorPos >= Furn_NbActPoint-1)
		{
		  Furn_ActCursorPos = 0;
		}
		else
			Furn_ActCursorPos++;
		break;

		case Left : //left arrow
		if (Furn_ActCursorPos <= 0)
		{
		  Furn_ActCursorPos = Furn_NbActPoint-1;
		}
		else
			Furn_ActCursorPos--;
		break;

		case -80 : //down arrow fast left cursor
		if (Furn_ActCursorPos == 0)
		{
			Furn_ActCursorPos = Furn_NbActPoint-1;
		}
		else if (Furn_ActCursorPos < Furn_CursorInc)
		{
			Furn_ActCursorPos = 0;
		}
		else
			Furn_ActCursorPos -= Furn_CursorInc;
		break;

		case -72 : //up arrow fast right cursor
		if (Furn_ActCursorPos == Furn_NbActPoint-1)
		{
			Furn_ActCursorPos = 0;
		}
		else if (Furn_ActCursorPos > Furn_NbActPoint - 1 - Furn_CursorInc)
		{
			Furn_ActCursorPos = Furn_NbActPoint-1;
		}
		else
			Furn_ActCursorPos += Furn_CursorInc;
		break;
	}//switch

	int rpoint[8];

	if (V10DispZorP)
	{
		bar(Furn_GxPV1,Furn_GyPV,Xline2+8,Furn_GySP+9);
		bar(TextPos-textwidth("    "),Furn_GyPV,Right_Rectangle-2,Furn_GySP+9);
		setcolor(15);
		line(Xline2,Bottom,Xline2,Top);

		if(*init==NORMAL)
		{
			APPELHS("Affiche,HEURE,1=%d",Furn_DispBegDate[3]);
			APPELHS("Affiche,MIN  ,1=%d",Furn_DispBegDate[4]);
			APPELHS("Affiche,SEC  ,1=%d",Furn_DispBegDate[5]);
			APPELHS("Affiche,JOUR ,1=%d",Furn_DispBegDate[2]);
			APPELHS("Affiche,MOIS ,1=%d",Furn_DispBegDate[1]);
			APPELHS("Affiche,AN   ,1=%d",Furn_DispBegDate[0]);

			APPELHS("Affiche,HEURE,2=%d",Furn_DispEndDate[3]);
			APPELHS("Affiche,MIN  ,2=%d",Furn_DispEndDate[4]);
			APPELHS("Affiche,SEC  ,2=%d",Furn_DispEndDate[5]);
			APPELHS("Affiche,JOUR ,2=%d",Furn_DispEndDate[2]);
			APPELHS("Affiche,MOIS ,2=%d",Furn_DispEndDate[1]);
			APPELHS("Affiche,AN   ,2=%d",Furn_DispEndDate[0]);
		}
	}

	else
	{
		int atw = textwidth("1234");

		bar(Furn_GxTdefCal-2,Furn_GyPtemp,Furn_GxTdefCal+atw+2,Furn_GyPtemp+9);
		bar(Furn_GxTmes   -2,Furn_GyPtemp,Furn_GxTmes   +atw+2,Furn_GyPtemp+9);
		bar(Furn_GxTdecal -2,Furn_GyPtemp,Furn_GxTdecal +atw+2,Furn_GyPtemp+9);

		bar(Furn_GxTsej-2,Furn_GyPmis,Furn_GxTsej+atw+2,Furn_GyPmis+9);
		bar(Furn_GxTobj-2,Furn_GyPmis,Furn_GxTobj+atw+2,Furn_GyPmis+9);
		atw = textwidth("12345");
		bar(Furn_GxOxy   -2,Furn_GyPmis,Furn_GxOxy+atw   +2,Furn_GyPmis+9);
		bar(Furn_GxDecarb-2,Furn_GyPmis,Furn_GxDecarb+atw+2,Furn_GyPmis+9);
	}//V10DispZorP

	settextjustify(LEFT_TEXT,TOP_TEXT);
	setcolor(15);

	if (Furn_NbActPoint)
	{
	//write data values
		long aval = 0;
		char *untext = "        ";
		int i,thecurv,xtextpos,ytextpos;
		int ic = Furn_ActCursorPos;

		if (V10DispZorP)
		{
			//settextjustify(RIGHT_TEXT,TOP_TEXT);

			//write zone temp data
			for (i=0;i<Furn_NbZone;i++)
			{
				aval = (long) Furn_Zval[i][ic];
				(void)ltoa(aval,untext,10);
				//check if curve is displayed and select color
				thecurv = V10DispZCurveNb[i];

				if (thecurv < 0 || thecurv > 5)
				{
					setcolor(15);
				}
				else
					setcolor(Furn_PaletteCi[thecurv]);
			//write the data
				outtextxy(Furn_GxPV1+i*Furn_DGxPV1,Furn_GyPV,untext);
			}//for each zone temp

		//write recup temp data
			i = maxNbZone;
			aval = (long) Furn_Zval[i][ic];
			(void)ltoa(aval,untext,10);
		//check if curve is displayed and select color
			thecurv = V10DispZCurveNb[i];

			if (thecurv < 0 || thecurv > 5)
			{
				setcolor(15);
			}
			else
				setcolor(Furn_PaletteCi[thecurv]);
		//	write the data

		//VB:  01-21-98 recup value
//			outtextxy(Furn_GxTrec,Furn_GyPV,untext);

			//write zone setpoint data
			for (i=maxNbZone+1;i<maxNbZone+1+Furn_NbZone;i++)
			{
				aval = (long) Furn_Zval[i][ic];
				(void)ltoa(aval,untext,10);
				//check if curve is displayed and select color
				thecurv = V10DispZCurveNb[i];

				if (thecurv < 0 || thecurv > 5)
				{
					setcolor(15);
				}
				else
					setcolor(Furn_PaletteCi[thecurv]);

				//write the data
				outtextxy(Furn_GxPV1+(i-maxNbZone-1)*Furn_DGxPV1,Furn_GySP,untext);
			}//for each zone sp

		//write zone QG data
			for (i=2*maxNbZone+1;i<2*maxNbZone+1+Furn_NbZone;i++)
			{
				aval = (long) Furn_Zval[i][ic];
				(void)ltoa(aval,untext,10);
				//check if curve is displayed and select color
				thecurv = V10DispZCurveNb[i];

				if (thecurv < 0 || thecurv > 5)
				{
					setcolor(15);
				}
				else
					setcolor(Furn_PaletteCi[thecurv]);

			//write the data
				outtextxy(TextPos+(i-2*maxNbZone-1)*Furn_DGxPV1,Furn_GySP,untext);
			}//for each zone qg

		//write zone QA data
			for (i=3*maxNbZone+1;i<3*maxNbZone+1+Furn_NbZone;i++)
			{
				aval = (long) Furn_Zval[i][ic];
				(void)ltoa(aval,untext,10);
				//check if curve is displayed and select color
				thecurv = V10DispZCurveNb[i];

				if (thecurv < 0 || thecurv > 5)
				{
					setcolor(15);
				}
				else
					setcolor(Furn_PaletteCi[thecurv]);
			//write the data
				outtextxy(TextPos+(i-3*maxNbZone-1)*Furn_DGxPV1,Furn_GyPV,untext);
			}//for each zone qg
		}//V10DispZorP
		else
		{
		//write prod data
			for (i=0;i<7;i++)
			{
				aval = Furn_Pval[i][ic];
				(void)ltoa(aval,untext,10);
			//check if curve is displayed and select color
				thecurv = V10DispPCurveNb[i];

				if (thecurv < 0 || thecurv > 5)
				{
					setcolor(15);
				}
				else
					setcolor(Furn_PaletteCi[thecurv]);
			//write the data

				if (i>1 && i<5)
				{
					ytextpos = Furn_GyPtemp;
				}
				else
					ytextpos = Furn_GyPmis;

				switch(i)
				{
					case 2: xtextpos = Furn_GxTdefCal;
					break;

					case 3: xtextpos = Furn_GxTmes;
					break;

					case 4: xtextpos = Furn_GxTdecal;
					break;

					case 1: xtextpos = Furn_GxTobj;
					break;

					case 0: xtextpos = Furn_GxTsej;
					break;

					case 5: xtextpos = Furn_GxOxy;
					break;

					case 6: xtextpos = Furn_GxDecarb;
				}//switch i

				outtextxy(xtextpos,ytextpos,untext);
			}//for each prod data
		}//V10DispZorP

	 //display cursor
		setfillstyle(SOLID_FILL,Background_Color);
		bar(0,Cursor_Top-1,Furn_GxZero+Furn_GxLarge+5,Cursor_Bottom);
	 //bar(Furn_GxPV1-18,Furn_GyPV,638,Furn_GyQA+9);

		 setfillstyle(SOLID_FILL,1);
	 //	 bar(Furn_GxPV1-18,Furn_GyPV,638,Furn_GyQA+9);

		 setcolor(15);
		 moveto(Furn_xActp[ic],Cursor_Bottom);
		 linerel(0,-13);
		 linerel(-5,5);
		 moverel(5,-5);
		 linerel(5,5);
		 //cursor date
		char labscur[20];
		(void)strcpy(labscur,"jj/mm hh:mm:ss");
		char *adate = "   ";
		int unedate[6];
		for (i=0;i<6;i++)
		{
			unedate[i] = Furn_xAct[i][ic];
			if (unedate[i] > 99 || unedate[i] < 0) unedate[i]=0;
		}

	 //month
		(void)itoa(unedate[1],adate,10);
		if (unedate[1] < 10)
		{
			labscur[3]='0';
			labscur[4]=adate[0];
		}
		else
		{
			labscur[3]=adate[0];
			labscur[4]=adate[1];
		}
	 //day
		(void)itoa(unedate[2],adate,10);

		if (unedate[2] < 10)
		{
			labscur[0]='0';
			labscur[1]=adate[0];
		}
		else
		{
			labscur[0]=adate[0];
			labscur[1]=adate[1];
		}
	 //hour
		(void)itoa(unedate[3],adate,10);

		if (unedate[3] < 10)
		{
			labscur[6]='0';
			labscur[7]=adate[0];
		}
		else
		{
			labscur[6]=adate[0];
			labscur[7]=adate[1];
		}
	 //minute
		(void)itoa(unedate[4],adate,10);

		if (unedate[4] < 10)
		{
			labscur[9]='0';
			labscur[10]=adate[0];
		}
		else
		{
			labscur[9]=adate[0];
			labscur[10]=adate[1];
		}
	 //second
		(void)itoa(unedate[5],adate,10);
		if (unedate[5] < 10)
		{
			labscur[12]='0';
			labscur[13]=adate[0];
		}
		else
		{
			labscur[12]=adate[0];
			labscur[13]=adate[1];
		}

		if (Furn_xActp[ic] < 320)
		{
			settextjustify(LEFT_TEXT,BOTTOM_TEXT);
			outtextxy(Furn_xActp[ic]+2,Cursor_Bottom,labscur);
		}
		else
		{
			settextjustify(RIGHT_TEXT,BOTTOM_TEXT);
			outtextxy(Furn_xActp[ic]-2,Cursor_Bottom,labscur);
		}

	}//Furn_NbActPoint
	settextjustify(CENTER_TEXT,TOP_TEXT);
	setcolor(15);
//	bar(580,2,638,12);
//	outtextxy(615,3,Furn_Hour);

//	VGPageVisu = VGPageActive;
//	setvisualpage(VGPageVisu);

}//end Furn_UpdateGraphCursor

/*
//---------------------------------------------------------------------
//end of acquisition treatment (enter key code received)
void Furn_AcqEnd()
{
	if (Furn_Active)
	{
	 //build the new dates with text dates
		char *untemps = "    ";
		//beginning date
		//ann‚e
		(void)strncpy(untemps,&(Furn_DBegDateAcqText[6]),4);
		untemps[4] = '\0';
		Furn_DispBegDate[0] = atoi(untemps);

		if (Furn_DispBegDate[0] < 1000) Furn_DispBegDate[0]=1000;
		//mois
		(void)strncpy(untemps,&(Furn_DBegDateAcqText[3]),2);
		untemps[2] = '\0';
		Furn_DispBegDate[1] = atoi(untemps);
		//jour
		(void)strncpy(untemps,&(Furn_DBegDateAcqText[0]),2);
		untemps[2] = '\0';
		Furn_DispBegDate[2] = atoi(untemps);
		//heure
		(void)strncpy(untemps,&(Furn_DBegDateAcqText[11]),2);
		untemps[2] = '\0';
		Furn_DispBegDate[3] = atoi(untemps);
		//minute
		(void)strncpy(untemps,&(Furn_DBegDateAcqText[14]),2);
		untemps[2] = '\0';
		Furn_DispBegDate[4] = atoi(untemps);
		//seconde
		(void)strncpy(untemps,&(Furn_DBegDateAcqText[18]),2);
		untemps[2] = '\0';
		Furn_DispBegDate[5] = atoi(untemps);

		//ending date
		//ann‚e
		(void)strncpy(untemps,&(Furn_DEndDateAcqText[6]),4);
		untemps[4] = '\0';
		Furn_DispEndDate[0] = atoi(untemps);

		if (Furn_DispEndDate[0] < 1000) Furn_DispEndDate[0]=1000;
	 //mois
		(void)strncpy(untemps,&(Furn_DEndDateAcqText[3]),2);
		untemps[2] = '\0';

		Furn_DispEndDate[1] = atoi(untemps);
	 //jour
		(void)strncpy(untemps,&(Furn_DEndDateAcqText[0]),2);
		untemps[2] = '\0';
		Furn_DispEndDate[2] = atoi(untemps);
	 //heure
		(void)strncpy(untemps,&(Furn_DEndDateAcqText[11]),2);
		untemps[2] = '\0';
		Furn_DispEndDate[3] = atoi(untemps);
	 //minute
		(void)strncpy(untemps,&(Furn_DEndDateAcqText[14]),2);
		untemps[2] = '\0';
		Furn_DispEndDate[4] = atoi(untemps);
	 //seconde
		(void)strncpy(untemps,&(Furn_DEndDateAcqText[18]),2);
		untemps[2] = '\0';
		Furn_DispEndDate[5] = atoi(untemps);

		Furn_NewHistDisp = 1;
		Furn_Active = 0;
	}

  // Modif JLC pour Bugg Date
  //Test function
//modif JLC
//date :09/05/1994

	double  som1 = (double)Furn_DispBegDate[0];
	double  som2 = (double)Furn_DispEndDate[0];

	for (int j=1;j<6;j++)
	{
		som1 = som1*100+(double)Furn_DispBegDate[j];
		som2 = som2*100+(double)Furn_DispEndDate[j];
	}

	if (som1>som2)
	{
		for (j=0;j<6;j++) Furn_DispEndDate[j] = Furn_DispBegDate[j];
	}

//raz de la position du curseur
	Furn_ActCursorPos=0;

//end test

  // Fin Modif JLC

	Furn_choosingField = 0;
	Furn_UpdateGraphCursor();
}//end Furn_AcqEnd

//---------------------------------------------------------------------
//character treatment in acquisition mode
//argument is the key code

void Furn_AcqChar(int thechar)
{
	if ((char)thechar == ' ') thechar = (int)('0');
	if (Furn_Active)
	{
		if (Furn_choosingField == 1)
		{
			Furn_DBegDateAcqText[Furn_curAcqVal-1] = (char)thechar;
		}
		else
		{
			Furn_DEndDateAcqText[Furn_curAcqVal-1] = (char)thechar;
		}
	}//Furn_Active

  //move acq cursor position
	Furn_UpdateGraphCursor(0,1);

}//end Furn_AcqChar

//---------------------------------------------------------------------
//acquisition cursor displaying
//argument displac is -1 for previous, 1 for next, 0 to display at current
//position

void Furn_AcqCursor(int displac)
{
	if (Furn_Active)
	{
		Furn_curAcqVal += displac;

		if (Furn_curAcqVal <= 0)
		{
			Furn_curAcqVal = 20;

			if (Furn_choosingField == 1)
			{
				Furn_choosingField = 2;
			}
			else
				Furn_choosingField = 1;
		}
		else if (Furn_curAcqVal > 20)
		{
			Furn_curAcqVal = 1;

			if (Furn_choosingField == 1)
			{
				Furn_choosingField = 2;
			}
			else
				Furn_choosingField = 1;
		}

		else if (Furn_curAcqVal == 3 || Furn_curAcqVal == 6 || Furn_curAcqVal == 11
			|| Furn_curAcqVal == 14)
		{
			if (displac == -1)
			{
				Furn_curAcqVal--;
			}
			else
				Furn_curAcqVal++;
		}

		else if (Furn_curAcqVal == 17 || Furn_curAcqVal == 18)
		{
			if (displac == -1)
			{
				Furn_curAcqVal = 16;
			}
			else Furn_curAcqVal = 19;
		}
	}//Furn_Active

	else
	{
		Furn_curAcqVal = 1;
	}//!choosing dates

	if (Furn_Active)
	{
		//display the cursor
		setcolor(14);
		char *untext = "                    ";
		(void)strncpy(untext,Furn_DBegDateAcqText,Furn_curAcqVal);
		untext[Furn_curAcqVal] = '\0';

		if (Furn_choosingField == 1)
		{
			moveto(Furn_GxDateBeg+textwidth(untext)-textwidth("0"),
				Furn_GyDate+textheight("h")+2);
		}
		else
		{
			moveto(Furn_GxDateEnd+textwidth(untext)-textwidth("0"),
				Furn_GyDate+textheight("h")+2);
		}//Furn_choosingField
		linerel(textwidth("0"),0);
	}//Furn_Active

}//end Furn_AcqCursor
*/

//---------------------------------------------------------------------
//display the dates
/*
void Furn_DispDates()
{
	char unedate[22];
	char untext[20];
	char *datetext[6] = {"     ","    ","    ","    ","    ","    "};
	//display beginning date
	if (!Furn_Active)
	{
		(void)strcpy(unedate,"31/12/1992 05h30mn13s");
		(void)itoa(Furn_DispBegDate[0],datetext[0],10);

		for (int i=1;i<6;i++)
		{
			(void)itoa(Furn_DispBegDate[i],datetext[i],10);

			if (strlen(datetext[i]) != 2)
			{
				datetext[i][2] = '\0';
				datetext[i][1] = datetext[i][0];
				datetext[i][0] = '0';
			}
		}
		(void)strcpy(unedate,datetext[2]);
		(void)strcat(unedate,"/");
		(void)strcat(unedate,datetext[1]);
		(void)strcat(unedate,"/");
		(void)strcat(unedate,datetext[0]);
		(void)strcat(unedate," ");
		(void)strcat(unedate,datetext[3]);
		(void)strcat(unedate,"h");
		(void)strcat(unedate,datetext[4]);
		(void)strcat(unedate,"mn");
		(void)strcat(unedate,datetext[5]);
		(void)strcat(unedate,"s");
		(void)strcpy(Furn_DBegDateAcqText,unedate);
	}
	else
		(void)strcpy(unedate,Furn_DBegDateAcqText);

  //		setviewport(Furn_GxDateBeg-5,Furn_GyDate-13,
  //				Furn_GxDateBeg+textwidth(unedate)+5,
  //				Furn_GyDate+textheight(unedate)+2,0);

  //--------------------VB--------------------
	int GxZero     = 40;

	setviewport(GxZero+1,34,589,Cursor_Top-16,0);
				//clear the screen
//	clearviewport();
 //	setviewport(0,0,639,349,0);

 //	setviewport(0,0,589,Cursor_Top-16,0);

	//	clearviewport();
		setviewport(0,0,639,349,0);

	setcolor(14);
	settextjustify(LEFT_TEXT,TOP_TEXT);
	(void)strcpy(untext,"Date de d‚but");
//	outtextxy(Furn_GxDateBeg+(textwidth(Furn_DBegDateAcqText)-textwidth(untext))/2,
//	Furn_GyDate-12,untext);
//	outtextxy(Furn_GxDateBeg,Furn_GyDate,unedate);

	 //display ending date
	if (!Furn_Active)
	{
		(void)strcpy(unedate,"31/12/1992 05h30mn13s");
		(void)itoa(Furn_DispEndDate[0],datetext[0],10);

		for (int i=1;i<6;i++)
		{
			(void)itoa(Furn_DispEndDate[i],datetext[i],10);

			if (strlen(datetext[i]) != 2)
			{
				datetext[i][2] = '\0';
				datetext[i][1] = datetext[i][0];
				datetext[i][0] = '0';
			}
		}
		(void)strcpy(unedate,datetext[2]);
		(void)strcat(unedate,"/");
		(void)strcat(unedate,datetext[1]);
		(void)strcat(unedate,"/");
		(void)strcat(unedate,datetext[0]);
		(void)strcat(unedate," ");
		(void)strcat(unedate,datetext[3]);
		(void)strcat(unedate,"h");
		(void)strcat(unedate,datetext[4]);
		(void)strcat(unedate,"mn");
		(void)strcat(unedate,datetext[5]);
		(void)strcat(unedate,"s");
		(void)strcpy(Furn_DEndDateAcqText,unedate);
	}
	else
		(void)strcpy(unedate,Furn_DEndDateAcqText);
//***********************VB ******************************
//	setactivepage(VGPageActive);

//	setviewport(GxZero+1,34,589,Cursor_Top-16,0);
	//clear the screen
//	clearviewport();
//	setviewport(0,0,639,349,0);

 //	setviewport(0,0,589,Cursor_Top-16,0);

//		setviewport(Furn_GxDateEnd-5,Furn_GyDate-13,
//				Furn_GxDateEnd+textwidth(unedate)+5,
//				Furn_GyDate+textheight(unedate)+2,0);
//		clearviewport();
//		setviewport(0,0,639,349,0);
		(void)strcpy(untext,"Date de fin");
//	outtextxy(Furn_GxDateEnd+(textwidth(Furn_DEndDateAcqText)-textwidth(untext))/2,
//		Furn_GyDate-12,untext);
//	outtextxy(Furn_GxDateEnd,Furn_GyDate,unedate);

}//end Furn_DispDates

*/

//---------------------------------------------------------------------
//key treatment, argument is the key code
/*
void Furn_TestKey(int TheKey)
{
	// init Furn_CursorAction
	Furn_CursorAction = 0 ;

	if (!Furn_Active)
	{
		switch (TheKey)
		{
//	  case  27: //ESC key
//		   if (V01WorkMode == 2)
//		    {
//		     VGNextViewNb = 16; //simulation display
//		    }
//		    else VGNextViewNb = 1;
//			 break;
//	  case -60: //F2 display product history
//		    if (!V14ActProdSave) VGNextViewNb = 11;
//		    break;
			case -59: //F1 display product temp
				VGNextViewNb = 7;
				break;
			case -62: //F4 display prod type list
				VGNextViewNb = 17;
				break;
			case -63: //F5 display target list
				VGNextViewNb = 6;
				break;
			case -64: //F6 display driving parameters
				VGNextViewNb = 13;
				break;
			case -65: //F7 display config view

				if(Furn_EndDate==0)
				{
					VGNextViewNb = 10;
				}
				break;
			case -66: //F8 change the dates
				VGChangeViewEnable = 0;
				Furn_curAcqVal = Furn_choosingField = 1;
				Furn_Active = 1;
			//Furn_UpdateGraphCursor();
				Furn_CursorAction = 1;
				Furn_EndDate=1;
				break;
			case -77: //right arrow
				Furn_CursorAction = 2 ;
				break;
			case -75: //left arrow
				Furn_CursorAction = 3 ;
				break;
				case -72: //up arrow
				Furn_CursorAction = 4 ;
				break;
			case -80: //down arrow
				Furn_CursorAction = 5 ;
				break;
		}
	}//Furn_Active
	else
	{
		switch (TheKey)
		{
			case -77: //right arrow
		  //Furn_UpdateGraphCursor(0,1);
				Furn_CursorAction = 6 ;
				break;
			case -75: //left arrow
		  //Furn_UpdateGraphCursor(0,-1);
				Furn_CursorAction = 7 ;
				break;
			case -65: //F7 display config view
				if(Furn_EndDate==0)
				{
					VGNextViewNb = 10;
				}
				Furn_AcqEnd();
				VGChangeViewEnable=1;
				Furn_EndDate=0;
				break;
			default: //acquisition or nothing
				if (isdigit(TheKey)) Furn_AcqChar(TheKey);
		}//end switch
	}//Furn_Active
}//end Furn_TestKey
*/

//-------------------------------------------------------------------
/*
void Furn_DispHour(char *theHour)
{
	if (strcmp(Furn_Hour,theHour))
	{
		(void)strcpy (Furn_Hour,theHour);

		if (VGDispViewNb == 9)
		{
			setactivepage(VGPageVisu);
			settextjustify(CENTER_TEXT,TOP_TEXT);
			setcolor(15);
			bar(580,2,638,12);
			outtextxy(615,3,Furn_Hour);
			setactivepage(VGPageActive);
		}
	}
}//end Furn_DispHour
*/
//-------------------------------------------------------------------
//end Furn_INT1.CPP
