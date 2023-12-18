//-------------------------------------------------------------------------
// REH_INT.CPP
// This file contains non overlay function for project TACT
// display number 07
//
// Owning include file : Reh_DATA1.H
//
// Creation : C.Martinet 18/12/92
// Modified :            11/01/93
//			 25/01/93 forgive password acquisition with ESC
//			 27/01/93 display production rate
//			 02/02/93 display the hour
//-------------------------------------------------------------------------

#define _OWNER_Reh_DATA1_H

#ifndef __CTYPE_H
#include <ctype.h>
#endif
#ifndef __STDLIB_H
#include <stdlib.h>
#endif

#define _OWNER_GLOBVIEW_H
#ifndef __GLOBVIEW_H
#include "globview.h"
#endif
#ifndef __REH_DATA_H
#include "Reh_data.h"
#endif
#ifndef __GLOBDAT_H
#include <globdat.h>
#endif
#ifndef __CONFIG_H
#include "config.h"
#endif
#ifndef __MTDBFURN_H
#include "mtdbfunc.h"
#endif

//------------------------------------------------------------------------
//functions
//update the display with cursor position, parameter action tells the
//function what to do : key code for interrupt, 0 for simple update
extern EndzTargetList *TheTargetList; 	  // Pointer to objective curve list

void Reh_UpdateGraphCursor() //int action
{

  //update the display
	if (VGPageVisu == 0)
		VGPageActive = 1;             // plusieurs setactivepage inutiles
	else
		VGPageActive = 0;
	setactivepage(VGPageActive);

	bar(GxActu   -34,GyTempMoy ,GxActu   -1,GyTempCore+6);
	bar(GxTarget -50,GyTempMoy ,GxTarget -1,GyTempCore+6);
	bar(GxRef    -42,GyTempMoy ,GxRef    -1,GyTempCore+6);

//	if (DelayState)
//		bar(GxRemains+65,GyTempCore,GxRemains-1,GyTempCore+6);
	Reh_DispLegendOrDelay();

	settextjustify(RIGHT_TEXT,TOP_TEXT);
	setcolor(15);

  // Affiche la temp‚rature de voute au pas actuel (CursorPos)
	char *le_texte = "        ";
	(void)itoa(TVouteAct[CursorPos],le_texte,10);
//	outtextxy(GxActu,GyTempVoute,le_texte);

	if (NbActPoint)
	{
		int xact = 0;
		if(CursorPosm[CursorPos] <= TxAct[NbActPoint-1])
		{
			long tabval[5];
			char *tabtext[5] = {"        ","        ","        ",
										 "        ","        "};
			while (xact < NbActPoint-1 &&	CursorPosm[CursorPos] > TxAct[xact])
				xact++;

			if (CursorPosm[CursorPos+1] > TxAct[xact])
				{
				//not a hole
				tabval[0] = TSejAct[xact];
				tabval[1] = TMoyAct[xact];
				tabval[2] = TSurfAct[xact];
				tabval[3] = TInfAct[xact];
				tabval[4] = TCoreAct[xact];

				for (int it = 0; it < 5; it++)
					(void)ltoa(tabval[it],tabtext[it],10);

				outtextxy(GxTarget,GyTempCore,tabtext[0]);
				outtextxy(GxActu  ,GyTempMoy ,tabtext[1]);
				outtextxy(GxActu  ,GyTempSurf,tabtext[2]);
				outtextxy(GxActu  ,GyTempInf ,tabtext[3]);
				outtextxy(GxActu  ,GyTempCore,tabtext[4]);

//       	int cposp = TxActp[xact];
				int cposp = CursorPosp[CursorPos];
				int foundType=0,markerIndex=0;
				while(!foundType && EndBatchMarker[markerIndex])
					{
					if (EndBatchMarker[markerIndex] >= cposp)
						foundType++;
					else
						markerIndex++;
					}

				if (foundType)
					{
						tabval[0] = (long)(Reh_TheBatch[markerIndex]->Id);
						tabval[1] = (long)(EndBatchType[markerIndex]->number());
//						tabval[1] = (long)(EndBatchType[markerIndex]->diam());
//						tabval[1] = (long)(1000*Reh_TheBatch[markerIndex]->Thick);
						tabval[2] = (long)(EndBatchType[markerIndex]->tdef());
//						tabval[3] =

					for (it = 0; it < 4; it++)
						(void)ltoa(tabval[it],tabtext[it],10);

//					char **thequal = EndBatchType[markerIndex]->theCostumerQual();
//					int aq = 0;

					outtextxy(GxRef,GyTempMoy ,tabtext[0]);
					outtextxy(GxRef,GyTempInf ,tabtext[1]);
					outtextxy(GxRef,GyTempSurf,Reh_TheBatch[markerIndex]->NrLotto);
					outtextxy(GxRef,GyTempCore,tabtext[2]);
				}//foundType


// Modif pour afficher la cadence equivalente

				char ecarttext[9] = "        ";
				char objtext[9]   = "        ";
				int ixref = 0;
				while(CursorPosm[CursorPos] > GPosZ[ixref]) ixref++;

#ifdef PACINGMODE
				EndzTarget& PTarg =
				TheTargetList->getCurve(TprodRate[xact],
				Reh_TheBatch[markerIndex]->TypeNumber,0);

	 // F.D : Display equivalent prodrate
/*
#ifdef DISPLAYCAD
				char EqProdText[13] = "Prod. : 0000" ;
				char ProdRate[5]    = "0000" ;
				strcpy (EqProdText,"Prod. : ");
				(void) itoa ( (int) TprodRate[xact] , ProdRate , 10 );
				strcat (EqProdText,ProdRate );
				outtextxy(GxRef+50,GyTempInf,EqProdText);
#endif
*/
#else
				EndzTarget& PTarg =
				TheTargetList->getCurve(Reh_TheBatch[markerIndex]->ProductionRate,
					Reh_TheBatch[markerIndex]->TypeNumber,0);
#endif

	 // Affiche la dur‚e pour le produit prioritaire situ‚ le plus
	 // pr‚s du d‚fournement
/*				if (Reh_Duree_seuil[0] != -1)
					{
					char chaine[15];
					sprintf(chaine,"Seuil : %02dh%02d",
						Reh_Duree_seuil[0],Reh_Duree_seuil[1] );
					outtextxy(GxRef+55,GyTempMoy,chaine);
					}
*/

/*				if (RefCurve)
					{
					char ecarttext[9];
					strcpy(ecarttext,"        ");
					int ixref = 0;
					while(CursorPosm[CursorPos] > GPosZ[ixref]) ixref++;

					if (EndZoneRef[ixref] != 0)
						{
						DeltaRefActu = TMoyAct[xact] - EndZoneRef[ixref];
						(void)ltoa(DeltaRefActu,ecarttext,10);
						int alen = strlen(ecarttext);
						(void)strcpy(&ecarttext[alen],"/");
						outtextxy(GxRef,GyTempMoy,ecarttext);
						}
					}//RefCurve
*/
				int ObjCourant;
				if ( (Object &)PTarg != NOOBJECT )
				{
					ObjCourant = PTarg.getTarget(ixref+1);
					DeltaRefActu = TMoyAct[xact] - ObjCourant;
				}
				else
					ObjCourant = DeltaRefActu = 0;

				(void)ltoa(DeltaRefActu,ecarttext,10);
				int alen = strlen(ecarttext);
				(void)strcpy(&ecarttext[alen],"øC");
				outtextxy(GxTarget,GyTempInf,ecarttext);

				(void)ltoa(ObjCourant,objtext,10);
				alen = strlen(objtext);
				(void)strcpy(&objtext[alen],"øC");
				outtextxy(GxTarget,GyTempMoy,objtext);
			}//not a hole
		}//cursorPos
	}//nbpoint

/*  ---------------------------- End Zone Ref -------------------------------
	if (RefCurve)
	{
		char objtext[9];
		(void)strcpy(objtext,"        ");
		int nz = 0;
		while(nz < GNbZone-1 && CursorPosm[CursorPos] > GPosZ[nz]) nz++;
		if (EndZoneRef[nz] != 0)
		{
			(void)ltoa(EndZoneRef[nz],objtext,10);
			int alen = strlen(objtext);
			(void)strcpy(&objtext[alen],"øC");
			settextjustify(LEFT_TEXT,TOP_TEXT);
			outtextxy(GxRef,GyTempMoy,objtext);     //vb
		}
	}//refcurve
 --------------------------------------------------------------------------
*/
	VGPageVisu = VGPageActive;
	setvisualpage(VGPageVisu);

}

void Update_Data(int action)
{
	setactivepage(VGPageVisu);

	switch(action)
	{
		case Right : //right arrow
//		if (CursorPosp[CursorPos] >= GPosZp[GNbZone-1])
		if (CursorPos >= NbPas-1)
			CursorPos = 0;
		else
		{
			CursorPos += CursorInc;
			if (CursorPosp[CursorPos]>GPosZp[GNbZone-1])
				CursorPos = 0;
		}
		break;
		case Left : //left arrow

		if (CursorPos <= 0)
		{
//			while(CursorPosp[CursorPos] <= GPosZp[GNbZone-1]) CursorPos++;
//			CursorPos--;
			CursorPos = NbPas-1;
		}
		else
		{
			CursorPos -= CursorInc;
			if (CursorPos < 0)
				CursorPos = 0;
		}
		break;
	}//switch
	setfillstyle(SOLID_FILL,Background_Color);
	bar(0,Cursor_Top,GxZero+GxLarge+5,Cursor_Bottom);

	setcolor(15);
	//display cursor
	moveto(CursorPosp[CursorPos],Cursor_Bottom);
	linerel(0,-13);
	linerel(-5,5);
	moverel(5,-5);
	linerel(5,5);
	char *labscur = "   ";

	if (!CursorPos)
	{
		(void)strcpy(labscur,"E");
	}
	else if (CursorPosp[CursorPos] >= GPosZp[GNbZone-1])
	{
		(void)strcpy(labscur,"S");
	}
	else
	{
		(void)ltoa(CursorPos,labscur,10);
	}
	settextjustify(RIGHT_TEXT,BOTTOM_TEXT);
	outtextxy(CursorPosp[CursorPos]-1,Cursor_Bottom,labscur);  //
	settextjustify(CENTER_TEXT,TOP_TEXT);
	setcolor(15);

	setactivepage(VGPageActive);
//	bar(580,2,638,12);
//	outtextxy(615,3,Reh_Hour)
}//end Reh_UpdateGraphCursor
//-------------------------------------------------------------------
//end REH_INT.CPP
