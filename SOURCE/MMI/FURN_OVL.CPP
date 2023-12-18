//-------------------------------------------------------------------------
// Furn_OVL1.CPP
// This file contains possible overlay function for project TACT
// display number 09
//
// Owning include file : NONE
//
// Creation : C.Martinet 18/12/92 ASCOMETAL APPLICATION
// Modified :		 02/02/93 display the hour
//			 18/02/93 color 12 set to 60
//-------------------------------------------------------------------------
#define _OWNER2_Furn_DATA1_H

#ifndef _FSTREAM_H
#include <fstream.h>
#endif
#ifndef __STDLIB_H
#include <stdlib.h>
#endif
#ifndef __MATH_H
#include <math.h>
#endif
#ifndef __ALLOC_H
#include <alloc.h>
#endif
#ifndef __DIR_H
#include <dir.h>
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
#ifndef __TEMPHIS_H
#include "temphis.h"
#endif
#include "config.h"
#include "database.h"
//------------------------------------------------------------------------
//function

//initialisation
void V10Init()
{
	int i;
	V10NbZone = 5;
	V10DispZorP = 1;

	for (i = 0; i <= 4*maxNbZone; V10DispZCurveNb[i++] = -1);
		for (i = 1; i < 4; i++)
		{
			V10DispZCurveNb[i] = i-1;
			V10DispZCurveNb[i+maxNbZone+1] = i+2;
		}

	for (i = 0; i < 6; V10DispPCurveNb[i] = i,i++);
		V10DispPCurveNb[6] = -1;

	V10Scaler[0] = 0;
	V10Scaler[1] = 1500; //øC
	V10Scaler[2] = 0;
	V10Scaler[3] = 300;  //mn
	V10Scaler[4] = 0;
	V10Scaler[5] = 10000;  //%
	V10Scaler[6] = 0;
	V10Scaler[7] = 2000; //æm

	for (i = 0; i < 6; i++)
	{
		V10ZCurvReplac[i] = V10PCurvReplac[i] = i;
		V10DispPCi[i]     = 1;
	}
}//end V10Init

void Furn_Init()
{
	//nbzone
	Furn_NbZone = 0;

	//data to copy page
//	Furn_Gpage = NULL;

	//ordinates for values at cursor
	Furn_GyFirst  = Top+6;
	Furn_GySecond = Furn_GyFirst +6;
	Furn_GyPV 	  = Furn_GySecond+10;
	Furn_GySP     = Furn_GyPV+10;

	Furn_GyPtemp  = Furn_GyPV;
	Furn_GyPmis   = Furn_GySP;

	//abscissa for values at cursor
	Furn_GxPV1     = 0 ;//0
	Furn_DGxPV1    = 0;
	Furn_GxTrec    = 0;
	Furn_GxTdefCal = 0;
	Furn_GxTmes    = 0;
	Furn_GxTdecal  = 0;
	Furn_GxTsej    = 0;
	Furn_GxTobj    = 0;
	Furn_GxOxy     = 0;
	Furn_GxDecarb  = 0;

	//origine of curve display (pixel)
	Furn_GxZero = 40;
	Furn_GyZero = 33;
	//range of curve display (pixel)
	Furn_GxLarge = 560;
	Furn_GyHaut  = (Cursor_Top-2)-Furn_GyZero;

	//curve scaling data
	//abscissa
	Furn_Gxalpha = 0;
	Furn_Gxbeta  = 0;
	//temperature
	Furn_Gyalpha = 0;
	Furn_Gybeta  = 0;
	//residence time
	Furn_Gtalpha = 0;
	Furn_Gtbeta  = 0;
	//oxy,decab
	Furn_Godalpha = 0;
	Furn_Godbeta  = 0;
	//flow rate %
	Furn_Gqalpha = 0;
	Furn_Gqbeta  = 0;

	//color palette for each curve
	Furn_PaletteCi[0] = 3;
	Furn_PaletteCi[1] = 4;
	Furn_PaletteCi[2] = 6;
	Furn_PaletteCi[3] = 8;
	Furn_PaletteCi[4] = 10;
	Furn_PaletteCi[5] = 13;

	//true color for each used palette
	Furn_ColorCi[0] = 20;
	Furn_ColorCi[1] = 59;
	Furn_ColorCi[2] = 62;
	Furn_ColorCi[3] = 60;
	Furn_ColorCi[4] = 58;
	Furn_ColorCi[5] = 61;

	//used curve data
	Furn_xActp = NULL;

	for (int i=0; i<6; Furn_xAct[i] = Furn_yActp[i] = NULL,i++);
	for (i=0; i<4*maxNbZone+1; Furn_Zval[i] = NULL, i++); //zone curve data
	for (i=0; i<7; Furn_Pval[i]=NULL,i++); //discharged product data

	//data for cursor display
	Furn_ActCursorPos = 0;  //position (0->Furn_NbActPoint-1)
	Furn_CursorInc    = 10; //number of shift positions for fast cursor

	Furn_NewHistDisp  = 0; //set to 1 when a new curve must be displayed

	for (i=0; i<6; i++)
	{
		Furn_DispBegDate[i] = 0; //beginning date of displayed curves
		Furn_DispEndDate[i] = 0; //ending date of displayed curves
	}

	Furn_Active = 0;
	Furn_curAcqVal = 1; //position of current character in date acquisition
	Furn_GxDateBeg = 110;
	Furn_GxDateEnd = 360;
	Furn_GyDate 	= 300;
	Furn_choosingField = 0; //field number which is to read
								//when choosing date 1->beg date, 2->end date

	for (i=0;i<10;i++)
	{
		Furn_GtempGradp[i] = 0; //screen ordinate for temperature axis
		Furn_GmnGradp[i]   = 0; //screen ordinate for time axis
		Furn_GmmGradp[i]   = 0; //screen ordinate for oxydec axis
		Furn_GpcGradp[i]   = 0; //screen ordinate for % axis
	}

	Furn_dtempGrad = Furn_nbtempGrad = 0;
	Furn_dmnGrad   = Furn_nbmnGrad   = 0;
	Furn_dmmGrad   = Furn_nbmmGrad   = 0;
	Furn_dpcGrad   = Furn_nbpcGrad   = 0;

	Furn_NbActPoint = 0; // number of points in each curve
}//end Furn_Init

//----------------------------------------------------------------------
//display
void Furn_Display()
{
	//display
	struct ffblk fb;

//	if( (V10DispZorP && !findfirst(ZHistFile,&fb,0)) ||
//		(!V10DispZorP && !findfirst(PHistFile,&fb,0)) )
//	{
	 //set mode to graphic mode
		if (VGDispMode == 1)
		{
			setgraphmode(getgraphmode());
			VGDispMode = 0;
		}

	//	Furn_tailp = imagesize(0,0,639,19);
  //		Furn_Gpage = (void *)new char[Furn_tailp];

		//set all palette color
		for (int i = 0; i < 6; i++)
			setpalette(Furn_PaletteCi[i],Furn_ColorCi[i]);

		setpalette(12,60);


		//zone temp value positions
		Furn_GxPV1  =10+textwidth("PV(øC):  ");
		Furn_DGxPV1 = textwidth(" 1234");

		//product values
		//"TdefCalc(øC): 1234  TdefMes(øC): 1234  TDecal(øC): 1234"
		Furn_GxTdefCal = 10+textwidth("TdefCalc(øC): ");
		Furn_GxTmes    = Furn_GxTdefCal + textwidth("1234  TdefMes(øC): ");
		Furn_GxTdecal  = Furn_GxTmes    + textwidth("1234  TDecal(øC): ");

//"Obj(øC): 1234  Sej(mn): 1234  PF(æm): 12345  Dec(æm): 12345"
		Furn_GxTobj   = 10+textwidth("Obj(øC)     : ");
		Furn_GxTsej   = Furn_GxTobj+textwidth("1234  Sej(mn)    : ");
		Furn_GxOxy    = Furn_GxTsej+textwidth("1234  PF(æm)    : ");
		Furn_GxDecarb = Furn_GxOxy+textwidth("12345  Dec(æm): ");

	 //allocate memory for history display
		for (i=0;i<6;i++)
			Furn_xAct[i] = new int[100];

		Furn_xActp = new int[100];

		if (V10DispZorP)
		{
			for (i=0;i<4*maxNbZone+1;i++)
				Furn_Zval[i] = new int[100];
		}
		else
		{
			for (i=0;i<7;i++)
				Furn_Pval[i] = new long[100];
		}

		for (i=0;i<6;i++)
			Furn_yActp[i] = new int[100];


	 //check if first call
		if (!Furn_DispBegDate[0])
		{
		 //init the dates with end date as current date and beg date as
		 //current date - 8 hours
			struct date ActDate;
			getdate(&ActDate);

			struct time ActTime;
			gettime(&ActTime);

			Furn_DispEndDate[0]=Furn_DispBegDate[0] = ActDate.da_year;
			Furn_DispEndDate[1]=Furn_DispBegDate[1] = (int)(ActDate.da_mon);
			Furn_DispEndDate[2]=Furn_DispBegDate[2] = (int)(ActDate.da_day);
			Furn_DispEndDate[3]=Furn_DispBegDate[3] = (int)(ActTime.ti_hour);
			Furn_DispEndDate[4]=Furn_DispBegDate[4] = (int)(ActTime.ti_min);
			Furn_DispEndDate[5]=Furn_DispBegDate[5] = (int)(ActTime.ti_sec);

			if (Furn_DispBegDate[3] >= 8)
			{
		 //same day
				Furn_DispBegDate[3] -= 8;
			}
			else
			{
		 //previous day
				Furn_DispBegDate[3] += 16;

				if (Furn_DispBegDate[2] > 1)
				{
	  //same month
					Furn_DispBegDate[2]--;
				}
				else
				{
	  //previous month
					Furn_DispBegDate[2] = 31; //no pb if this day doesn't exists

					if (Furn_DispBegDate[1] > 1)
					{
						//same year
						Furn_DispBegDate[1]--;
					}
					else
					{
		  //previous year
						Furn_DispBegDate[1]=12;
						Furn_DispBegDate[0]--;
					}//?same year
				}//?same month
			}//?same day
		}//first call

	 //check V10Scaler;
		if (V10Scaler[0] == V10Scaler[1])
		{
			V10Scaler[0] = 0;
			V10Scaler[1] = 1500;
		}
		if (V10Scaler[2] == V10Scaler[3])
		{
			V10Scaler[2] = 0;
			V10Scaler[3] = 300;
		}

		if (V10Scaler[4] == V10Scaler[5])
		{
			V10Scaler[4] = 0;
			V10Scaler[5] = 10000;
		}

		if (V10Scaler[6] == V10Scaler[7])
		{
			V10Scaler[6] = 0;
			V10Scaler[7] = 2000;
		}


		Furn_NbActPoint = 0;
		Furn_NewHistDisp++;
		//read the time
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

		(void)strcpy(Furn_Hour,anhour);

		Furn_UpdateData();
		Furn_UpdateDisp();
		VGDispViewNb = 9;

//	}
//	else
  //		VGNextViewNb = VGDispViewNb; //no file => no display change

}//end Furn_Display

//----------------------------------------------------------------------
//erase
void Furn_Erase()
{
	//free allocated memory
	for (int i=0;i<6;i++)
	{
		if (Furn_xAct[i] != NULL)
		{
			delete Furn_xAct[i];
			Furn_xAct[i] = NULL;
		}
	}

	if (Furn_xActp != NULL)
	{
		delete Furn_xActp;
		Furn_xActp = NULL;
	}

	if (V10DispZorP)
	{
		for (i=0;i<4*maxNbZone+1;i++)
		{
			if (Furn_Zval[i] != NULL)
			{
				delete Furn_Zval[i];
				Furn_Zval[i] = NULL;
			}
		}
	}
	else
	{
		for (i=0;i<7;i++)
		{
			if (Furn_Pval[i] != NULL)
			{
				delete Furn_Pval[i];
				Furn_Pval[i] = NULL;
			}
		}
	}//V10DispZorP

	for (i=0;i<6;i++)
	{
		if (Furn_yActp[i] != NULL)
		{
			delete Furn_yActp[i];
			Furn_yActp[i] = NULL;
		}
	}

/*	if (Furn_Gpage != NULL)
	{
		delete Furn_Gpage;
		Furn_Gpage = NULL;
	}
*/

	Furn_NbActPoint  = 0;
	Furn_Active      = 0;
	Furn_NewHistDisp = 0;

	//erase the screen
//	setactivepage(VGPageVisu);
//	VGPageActive = VGPageVisu;
 //	cleardevice();
}//end Furn_Erase

//----------------------------------------------------------------------
//Update the display with new data
void Furn_UpdateDisp()
{
  //select active page as non visual page
	if(*init==INITIAL)
	{
//		if (VGPageVisu == 0)  		VGPageActive = 1;
//		else
		VGPageActive = 0;
		setactivepage(VGPageActive);
		setvisualpage(1);

//	setviewport(Furn_GxZero+1,34,589,Cursor_Top-16,0);
// clear the screen
//	clearviewport();
//	setviewport(0,0,639,349,0);
  }

  //display the background
	Furn_DispBackground();
	//display the curve
	setfillstyle(SOLID_FILL,1);

	if (Furn_NbActPoint)
	{
		if (V10DispZorP)
		{
			for (int i = 0; i < 6; i++)
			{
				if (V10DispZCi[i])
				{
					setcolor(Furn_PaletteCi[i]);
					moveto(Furn_GxZero+1,Furn_yActp[i][0]);

					for (int j = 1; j < Furn_NbActPoint ; j++)
						lineto(Furn_xActp[j],Furn_yActp[i][j]);
				}
			}
		}//V10DispZorP
		else
		{

			for (int i = 0; i < 6; i++)
			{
				if (V10DispPCi[i])
				{
					setcolor(Furn_PaletteCi[i]);
					moveto(Furn_GxZero+1,Furn_yActp[i][0]);

					for ( int j = 1; j < Furn_NbActPoint ; j++)
						lineto(Furn_xActp[j],Furn_yActp[i][j]);
				}
			}
		}//V10DispZorP
	}//Furn_NbActPoint
/*
#ifdef DEBBUGING
	 char memav[60];
	 (void)strcpy(memav,"MemAvail:                                          ");
	 unsigned long memAvail = farcoreleft();
	 (void)ultoa(memAvail,&memav[10],10);
	 settextjustify(CENTER_TEXT,TOP_TEXT);
	 outtextxy(320,25,memav);
#endif
*/

	//update cursor data (will switch to active page)
	Furn_UpdateGraphCursor();

	//update non visual page (for next cursor update)
//		if (VGPageVisu == 0)
//			VGPageActive = 1;
//		else
//			VGPageActive = 0;

	if(*init == INITIAL)
	{
//		Furn_tailp = imagesize(0,0,639,19);
//		Furn_Gpage = (void *)new char[Furn_tailp];

//		for (int yline = 0; yline < 349; yline+=20)
//		{
				struct palettetype pal;
				getpalette(&pal);

				APPELHS("Option,Couleur,P,7=57");
				APPELHS("Option,Couleur,P,9=7");

				setvisualpage(VGPageActive);

				if (V10DispZorP==1)
					APPELHS ("Fusionne,histitle.img,1,1");
				else
					APPELHS ("Fusionne,distitle.img,1,1");

				setallpalette(&pal);

				APPELHS("Option,Couleur,P,7=57");
				APPELHS("Option,Couleur,P,9=7");

				APPELHS ("Fusionne,datehour.aid");
				APPELHS ("Ouvre,ZHIS_BOT.AID,22,1");

				if (V10DispZorP==0)
					{
					APPELHS ("Ecran,Select,-,ico_botzf3");
					APPELHS ("Fusionne,botgf3.ico,23,17");
					}
				APPELHS ("Option,Couleur,S=15");
				APPELHS ("Option,Couleur,A=15");
//      }
//			setactivepage(VGPageVisu);
//			getimage(0,yline,639,yline+19,Furn_Gpage); //yline+19
//			setactivepage(VGPageActive);
//			putimage(0,yline,Furn_Gpage,0);
	}

//		if (Furn_Gpage != NULL)
//		{
//			delete Furn_Gpage;
//			Furn_Gpage = NULL;
//		}
//	}
//	setactivepage(VGPageActive);

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
}//end Furn_UpdateDisp

//----------------------------------------------------------------------
//Update the data with the data for the display
void Furn_UpdateData()
{
	//wait until ecr table is accessible
	ifstream hisfile;

	long thepos;
	int bdate[6],edate[6];
	long nbrec;
	long firstRec=0,lastRec=0;
	float nbpass=0;

	if (Furn_NewHistDisp)
	{
		//reset number of points
		Furn_NbActPoint = 0;

		//read number of record in history files
		for (int i=0;i<6;i++)
		{
			bdate[i]=Furn_DispBegDate[i];
			edate[i]=Furn_DispEndDate[i];
		}

		if (V10DispZorP)
		{
	 //diplay zone history
			struct date ActDate;
			struct time ActTime;

			nbrec=zoneTempRecNum(Furn_DispBegDate,Furn_DispEndDate,
				&firstRec,&lastRec);
	 //check the returned dates
			double date1,date2;
			char filename[MAXPATH];
			sprintf(filename,"%s%s",DB_Path_hisfour,ZONEHIST);

			if (nbrec)
			{
				date1 = (double)Furn_DispBegDate[0];
				date2 = (double)Furn_DispEndDate[0];

				for (i=1;i<6;i++)
				{
					date1 = date1*100+(double)Furn_DispBegDate[i];
					date2 = date2*100+(double)Furn_DispEndDate[i];
				}

				if (date1 != date2)
				{
				//read the zone temp data
					int maxrec = nbrec-1;

					if (nbrec>100)
						{
						nbpass = ((float)nbrec)/100 - 1;
						maxrec = 99;
						}
					else
						nbpass = 0;

					int nblu = 0;
					long numrec=firstRec;
					long totnbrec,oldest;
					int nbz = 0;
					int recupt,ztemp[maxNbZone],zspt[maxNbZone];
					int zqg[maxNbZone],zqa[maxNbZone];
					float passrec = 0;
					//open the file
					hisfile.open(filename,ios::in|ios::binary);

					if (hisfile)
						{
						hisfile.read((char*)&totnbrec,sizeof(long));
						hisfile.read((char*)&oldest,sizeof(long));
						hisfile.read((char*)&nbz,sizeof(int));
						}//hisfile

					Furn_NbZone = nbz;


					while (hisfile && nblu < maxrec)
					{
		//read a record
						thepos = 2*sizeof(long) + sizeof(int) +
						(numrec-1)*(7+4*nbz)*sizeof(int);

						hisfile.seekg(thepos,ios::beg);
						hisfile.read((char*)bdate,6*sizeof(int));
						hisfile.read((char*)&recupt,sizeof(int));
						hisfile.read((char*)ztemp,nbz*sizeof(int));
						hisfile.read((char*)zspt,nbz*sizeof(int));
						hisfile.read((char*)zqg,nbz*sizeof(int));
						hisfile.read((char*)zqa,nbz*sizeof(int));

						for (i=0;i<6;i++)
							Furn_xAct[i][nblu] = bdate[i];

						for (i=0;i<nbz;i++)
						{
							Furn_Zval[i][nblu]               = ztemp[i];
							Furn_Zval[i+maxNbZone+1][nblu]   = zspt[i];
							Furn_Zval[i+2*maxNbZone+1][nblu] = zqg[i];
							Furn_Zval[i+3*maxNbZone+1][nblu] = zqa[i];
						}
						Furn_Zval[maxNbZone][nblu] = recupt;
						nblu++;
						passrec += nbpass;

						while (passrec >= 1)
						{
							numrec++;
							if (numrec > totnbrec) numrec = 1;
								passrec--;
						}
						numrec++;

						if (numrec > totnbrec) numrec = 1;
					}//while nblu

					if (hisfile && nblu <= maxrec)
					{
						//read last record
						thepos = 2*sizeof(long) + sizeof(int) +
						(lastRec-1)*(7+4*nbz)*sizeof(int);
						hisfile.seekg(thepos,ios::beg);
						hisfile.read((char*)bdate,6*sizeof(int));
						hisfile.read((char*)&recupt,sizeof(int));
						hisfile.read((char*)ztemp,nbz*sizeof(int));
						hisfile.read((char*)zspt,nbz*sizeof(int));
						hisfile.read((char*)zqg,nbz*sizeof(int));
						hisfile.read((char*)zqa,nbz*sizeof(int));

						for (i=0;i<6;i++) Furn_xAct[i][nblu] = bdate[i];

						for (i=0;i<nbz;i++)
						{
							Furn_Zval[i][nblu] = ztemp[i];
							Furn_Zval[i+maxNbZone+1][nblu] = zspt[i];
							Furn_Zval[i+2*maxNbZone+1][nblu] = zqg[i];
							Furn_Zval[i+3*maxNbZone+1][nblu] = zqa[i];
						}
						Furn_Zval[maxNbZone][nblu] = recupt;
						nblu++;
					}//hisfile

					hisfile.close();
					Furn_NbActPoint = nblu;
				}//date1!=date2
			}//nbrec
		}//V10DispZorP
		else
		{
	 //display prod history
			nbrec = prodTempRecNum(Furn_DispBegDate,Furn_DispEndDate,
						&firstRec,&lastRec);
	 //check the returned dates
			double date1,date2;

			if (nbrec)
			{
				date1 = (double)Furn_DispBegDate[0];
				date2 = (double)Furn_DispEndDate[0];

				for (i=1;i<6;i++)
				{
					date1 = date1*100+(double)Furn_DispBegDate[i];
					date2 = date2*100+(double)Furn_DispEndDate[i];
				}

				if (date1 != date2)
				{
				//read the prod data
					int maxrec = nbrec-1;
					if (nbrec>100)
					{
						nbpass = ((float)nbrec)/100 - 1;
						maxrec = 99;
					}
					else nbpass = 0;

					int nblu = 0;
					long numrec=firstRec;
					long totnbrec,oldest;
					int pval[7],pobj;
					long poxydec[2];
					float passrec = 0;
			//open the file
					char filename[MAXPATH];
					sprintf(filename,"%s%s",DB_Path_hisfour,DEFHIST);

					hisfile.open(filename,ios::in|ios::binary);
		//			hisfile.open(PHistFile,ios::in|ios::binary);

					if (hisfile)
					{
						hisfile.read((char*)&totnbrec,sizeof(long));
						hisfile.read((char*)&oldest,sizeof(long));
					}//hisfile

					while (hisfile && nblu < maxrec)
					{
						//read a record
						thepos = 2*sizeof(long) +
						(numrec-1)*(14*sizeof(int) + 2*sizeof(long));
						hisfile.seekg(thepos,ios::beg);
						hisfile.read((char*)bdate,6*sizeof(int));
						hisfile.read((char*)pval,7*sizeof(int));
						hisfile.read((char*)poxydec,2*sizeof(long));
						hisfile.read((char*)&pobj,sizeof(int));

						for (i=0;i<6;i++) Furn_xAct[i][nblu] = bdate[i];

						Furn_Pval[2][nblu] = (long)pval[1]; //calc mean temp
						Furn_Pval[3][nblu] = (long)pval[5]; //measured temp
						Furn_Pval[4][nblu] = (long)pval[6]; //decal temp
						Furn_Pval[1][nblu] = (long)pobj; //target
						Furn_Pval[0][nblu] = (long)pval[4]; //redidence time
						Furn_Pval[5][nblu] = poxydec[0]; //oxydation
						Furn_Pval[6][nblu] = poxydec[1]; //decarburation

						nblu++;
						passrec += nbpass;

						while (passrec >= 1)
						{
							numrec++;

							if (numrec > totnbrec) numrec = 1;
								passrec--;
						}
						numrec++;

						if (numrec > totnbrec) numrec = 1;
					}//while nblu

					if (hisfile && nblu <= maxrec)
					{
					//read last record
						thepos = 2*sizeof(long) +
						(lastRec-1)*(14*sizeof(int) + 2*sizeof(long));
						hisfile.seekg(thepos,ios::beg);
						hisfile.read((char*)bdate,6*sizeof(int));
						hisfile.read((char*)pval,7*sizeof(int));
						hisfile.read((char*)poxydec,2*sizeof(long));
						hisfile.read((char*)&pobj,sizeof(int));

						for (i=0;i<6;i++) Furn_xAct[i][nblu] = bdate[i];

						Furn_Pval[2][nblu] = (long)pval[1]; //calc mean temp
						Furn_Pval[3][nblu] = (long)pval[5]; //measured temp
						Furn_Pval[4][nblu] = (long)pval[6]; //decal temp
						Furn_Pval[1][nblu] = (long)pobj; //target
						Furn_Pval[0][nblu] = (long)pval[4]; //redidence time
						Furn_Pval[5][nblu] = poxydec[0]; //oxydation
						Furn_Pval[6][nblu] = poxydec[1]; //decarburation

						nblu++;
					}//hisfile

					hisfile.close();
					Furn_NbActPoint = nblu;
				}//date1!=date2
			}//nbrec
		}//V10DispZorP

		//update data with scaling
		Furn_ScaleGraphData();

		//cursor data
		if ( Furn_ActCursorPos >= Furn_NbActPoint)

		Furn_ActCursorPos = Furn_NbActPoint-1;

		//scale the abscissa
		if(Furn_NbActPoint > 2)
			Furn_ScaleGraphDates();

	}//NewHistDisp

	//cursor increment for fast cursor
	if (Furn_NbActPoint < 10)
	{
		 Furn_CursorInc = 1;
	}
	else Furn_CursorInc = 10;

	Furn_NewHistDisp = 0;
}//end Furn_UpdateData

//-----------------------------------------------------------------------
void Furn_ScaleGraphDates()
{
	if (Furn_NbActPoint)
	{
		int bdate[6],edate[6];

	  //read beg and end dates
		int i;
		for (i=0;i<6;bdate[i]=Furn_xAct[i][0],i++);
		for (i=0;i<6;edate[i]=Furn_xAct[i][Furn_NbActPoint-1],i++);

	  //number of second since 01/01/1992 00h00mn00s
		long totday;
		double nbsecb,nbsece;
		int NbAn = bdate[0]-1992;
		int NbBis = (NbAn/4);

		if (NbAn>0 && NbAn<4) NbBis++;
			totday = (long)(NbAn*365 + NbBis);

		int isBis=0;

		if (NbAn-4*(NbAn/4) == 0)
			isBis++;

		for (int cmois = 1;cmois < bdate[1];cmois++)
		{
			if (cmois == 2)
			{
				totday += (28+isBis);
			}
			else if (cmois == 4 || cmois == 6 || cmois == 9 || cmois == 11)
			{
				totday += 30;
			}
			else
			{
				totday += 31;
			}
		}//cmois
		totday += (bdate[2]-1);
		nbsecb  = 86400*(double)totday +
					 3600*(double)bdate[3] +
					 60*(double)bdate[4] +
					 (double)bdate[5];

		NbAn = edate[0]-1992;
		NbBis = (NbAn/4);

		if (NbAn>0 && NbAn<4) NbBis++;
			totday = (long)(NbAn*365 + NbBis);

		isBis=0;

		if (NbAn-4*(NbAn/4) == 0) isBis++;

		for (cmois = 1;cmois < edate[1];cmois++)
		{
			if (cmois == 2)
			{
				totday += (28+isBis);
			}

			else if (cmois == 4 || cmois == 6 || cmois == 9 || cmois == 11)
			{
				totday += 30;
			}
			else
			{
				totday += 31;
			}
		}//cmois

		totday += (edate[2]-1);
		nbsece  = 86400*(double)totday +
					 3600*(double)edate[3] +
					 60*(double)edate[4] +
					 (double)edate[5];

		//scaling abscissa
		Furn_Gxalpha = ((double)Furn_GxLarge)/(nbsece-nbsecb);
		Furn_Gxbeta = (double)Furn_GxZero;

		for (int Ind = 0;Ind<Furn_NbActPoint;Ind++)
		{
			for (i=0;i<6;edate[i]=Furn_xAct[i][Ind],i++);
				NbAn = edate[0]-1992;
				NbBis = (NbAn/4);

			if (NbAn>0 && NbAn<4)
				NbBis++;

			totday = (long)(NbAn*365 + NbBis);
			isBis=0;

			if (NbAn-4*(NbAn/4) == 0)
				isBis++;

			for (cmois = 1;cmois < edate[1];cmois++)
			{
				if (cmois == 2)
				{
					totday += (28+isBis);
				}
				else if (cmois == 4 || cmois == 6 || cmois == 9 || cmois == 11)
				{
					totday += 30;
				}
				else
				{
					totday += 31;
				}
			}//cmois
			totday += (edate[2]-1);
			nbsece = 86400*(double)totday +
						3600*(double)edate[3] +
						60*(double)edate[4] +
						(double)edate[5];

			Furn_xActp[Ind] = (int)((nbsece-nbsecb)*Furn_Gxalpha + Furn_Gxbeta);
		}//Ind
	}//Furn_NbActPoint
}//end Furn_ScaleGraphDates

//-----------------------------------------------------------------------
void Furn_ScaleGraphData()
{

	//scale the data
	float interval;

	interval   = (float)(V10Scaler[1]-V10Scaler[0]);
	Furn_Gyalpha = -1.*((float)Furn_GyHaut)/interval;
	Furn_Gybeta  = (float)(Furn_GyZero+Furn_GyHaut)-Furn_Gyalpha*(float)V10Scaler[0];

	interval   = (float)(V10Scaler[3]-V10Scaler[2]);
	Furn_Gtalpha = -1.*((float)Furn_GyHaut)/interval;
	Furn_Gtbeta  = (float)(Furn_GyZero+Furn_GyHaut)-Furn_Gtalpha*(float)V10Scaler[2];

	interval   = (float)(V10Scaler[5]-V10Scaler[4]);
	Furn_Gqalpha = -1.*((float)Furn_GyHaut)/interval;
	Furn_Gqbeta  = (float)(Furn_GyZero+Furn_GyHaut)-Furn_Gqalpha*(float)V10Scaler[4];

	interval    = (float)(V10Scaler[7]-V10Scaler[6]);
	Furn_Godalpha = -1.*((float)Furn_GyHaut)/interval;
	Furn_Godbeta  = (float)(Furn_GyZero+Furn_GyHaut)-Furn_Godalpha*(float)V10Scaler[6];

	int curvNb;
	if (V10DispZorP)
	{
  //	  int i = 0;
	  //zone temperatures
		for (int i = 0; i < 2*maxNbZone+1 ;i++)
		{
			curvNb = V10DispZCurveNb[i];
			if (curvNb != -1)
			{
				for (int j=0;j<Furn_NbActPoint;j++)
				{
					if (Furn_Zval[i][j] <= V10Scaler[0])
					{
						Furn_yActp[curvNb][j] = Furn_GyZero+Furn_GyHaut;
					}
					else if (Furn_Zval[i][j] >= V10Scaler[1])
					{
						Furn_yActp[curvNb][j] = Furn_GyZero;
					}
					else
					{
						Furn_yActp[curvNb][j] = (int)(Furn_Gyalpha*(float)Furn_Zval[i][j] + Furn_Gybeta);
 //							Furn_yActp[curvNb][j] = (int)( (Furn_GyHaut+Furn_GyZero) - (0.132*(float)Furn_Zval[i][j]) );
					}
				}//for each point
			}//curvNb
		}//for each temperature
	  //zone flow rate

		for (i = 2*maxNbZone+1; i < 4*maxNbZone+1 ;i++)
		{
			curvNb = V10DispZCurveNb[i];
			if (curvNb != -1)
			{
				for (int j=0;j<Furn_NbActPoint;j++)
				{
					if (Furn_Zval[i][j] <= V10Scaler[4])
					{
						Furn_yActp[curvNb][j] = Furn_GyZero+Furn_GyHaut;
					}
					else if (Furn_Zval[i][j] >= V10Scaler[5])
					{
						Furn_yActp[curvNb][j] = Furn_GyZero;
					}
					else
					{

						Furn_yActp[curvNb][j] =
						(int)(Furn_Gqalpha*(float)Furn_Zval[i][j] + Furn_Gqbeta);
					}
				}//for each point
			}//curvNb
		}//for each flow rate
	}//V10DispZorP
	else
	{
	  //disch product data
	  //residence time
		int i = 0;
		curvNb = V10DispPCurveNb[i];

		if (curvNb != -1)
		{
			for ( int j=0;j<Furn_NbActPoint;j++)
			{
				if (Furn_Pval[i][j] <= V10Scaler[2])
				{
					Furn_yActp[curvNb][j] = Furn_GyZero+Furn_GyHaut;
				}
				else if (Furn_Pval[i][j] >= V10Scaler[3])
				{
					Furn_yActp[curvNb][j] = Furn_GyZero;
				}
				else
				{
					Furn_yActp[curvNb][j] =
					(int)(Furn_Gtalpha*(float)Furn_Pval[i][j] + Furn_Gtbeta);
				}
			}//for each point
		}//curvNb

		//product temperatures
		for (i = 1; i < 5; i++)
		{
			curvNb = V10DispPCurveNb[i];

			if (curvNb != -1)
			{
				for (int j=0;j<Furn_NbActPoint;j++)
				{
					if (Furn_Pval[i][j] <= V10Scaler[0])
					{
						Furn_yActp[curvNb][j] = Furn_GyZero+Furn_GyHaut;
					}
					else if (Furn_Pval[i][j] >= V10Scaler[1])
					{
						Furn_yActp[curvNb][j] = Furn_GyZero;
					}
					else
					{
						Furn_yActp[curvNb][j] =
						(int)(Furn_Gyalpha*(float)Furn_Pval[i][j] + Furn_Gybeta);
					}
				}//for each point
			}//curvNb
		}//for each temperature

	  //product oxydation & decarb.
		for (i = 5; i < 7; i++)
		{
			curvNb = V10DispPCurveNb[i];
			if (curvNb != -1)
			{
				for (int j=0;j<Furn_NbActPoint;j++)
				{
					if (Furn_Pval[i][j] <= V10Scaler[6])
					{
						Furn_yActp[curvNb][j] = Furn_GyZero+Furn_GyHaut;
					}
					else if (Furn_Pval[i][j] >= V10Scaler[7])
					{
						Furn_yActp[curvNb][j] = Furn_GyZero;
					}
					else
					{
						Furn_yActp[curvNb][j] =
						(int)(Furn_Godalpha*(float)Furn_Pval[i][j] + Furn_Godbeta);
					}
				}//for each point
			}//curvNb
		}//for oxy and decarb
	}//V10DispZorP

	//scale the axis
	int ygrad;   //int i;
	long dmm;
	Furn_nbtempGrad=Furn_nbmnGrad=Furn_nbmmGrad=Furn_nbpcGrad=0;
	//check the axis to display

	if (V10DispZorP)
	{
		for (int i=0;i<2*maxNbZone+1;i++)
		{
			if (V10DispZCurveNb[i] != -1)
				Furn_nbtempGrad++;
		}

		for (;i<4*maxNbZone+1;i++)
		{
			if (V10DispZCurveNb[i] != -1)
				Furn_nbpcGrad++;
		}
	}
	else
	{
		if (V10DispPCurveNb[0] != -1) Furn_nbmnGrad++;

		for (int i=1;i<5;i++)
		{
			if (V10DispPCurveNb[i] != -1) Furn_nbtempGrad++;
		}

		if (V10DispPCurveNb[5] != -1 ||
		V10DispPCurveNb[6] != -1) Furn_nbmmGrad++;
	}//V10DispZorP

	//calculate temperature axis position
	if (Furn_nbtempGrad)
	{
		Furn_nbtempGrad=0;
		dmm = V10Scaler[1]-V10Scaler[0];

		if (dmm >= 10)
		{
			Furn_dtempGrad = (int)(dmm/10);
		}
		else Furn_dtempGrad = 1;

		for (int i=0,ygrad=(int)V10Scaler[0]+Furn_dtempGrad;
		  (ygrad<(int)V10Scaler[1] && i<10); i++,ygrad+=Furn_dtempGrad)
		{
			Furn_GtempGradp[i] = (int)(Furn_Gyalpha*(float)ygrad+Furn_Gybeta);
			Furn_nbtempGrad++;
		}
	}

	if (Furn_nbpcGrad)
	{
		//calculate flow rate axis position
		Furn_nbpcGrad=0;
		dmm = V10Scaler[5]-V10Scaler[4];

		if (dmm >= 10)
		{
			Furn_dpcGrad = (int)(dmm/10);
		}
		else Furn_dpcGrad = 1;

		for (int i=0,ygrad=(int)V10Scaler[4]+Furn_dpcGrad;
		  (ygrad<(int)V10Scaler[5] && i<10); i++,ygrad+=Furn_dpcGrad)
		{
			Furn_GpcGradp[i] = (int)(Furn_Gqalpha*(float)ygrad+Furn_Gqbeta);
			Furn_nbpcGrad++;
		}
	}

	if (Furn_nbmnGrad)
	{
		//calculate minute axis position
		Furn_nbmnGrad=0;
		dmm = V10Scaler[3]-V10Scaler[2];

		if (dmm >= 10)
		{
			Furn_dmnGrad = (int)(dmm/10);
		}
		else Furn_dmnGrad = 1;

		for (int i=0,ygrad=(int)V10Scaler[2]+Furn_dmnGrad;
		  (ygrad<(int)V10Scaler[3] && i<10); i++,ygrad+=Furn_dmnGrad)
		{
			Furn_GmnGradp[i] = (int)(Furn_Gtalpha*(float)ygrad+Furn_Gtbeta);
			Furn_nbmnGrad++;
		}
	}

	if (Furn_nbmmGrad)
	{
		//calculate æm axis position
		Furn_nbmmGrad=0;
		dmm = V10Scaler[7]-V10Scaler[6];

		if (dmm >= 10)
		{
			Furn_dmmGrad = (int)(dmm/10);
		}
		else Furn_dmmGrad = 1;

		for (int i=0,ygrad=(int)V10Scaler[6]+Furn_dmmGrad;
		  (ygrad<(int)V10Scaler[7] && i<10); i++,ygrad+=Furn_dmmGrad)
		{
			Furn_GmmGradp[i] = (int)(Furn_Godalpha*(float)ygrad+Furn_Godbeta);
			Furn_nbmmGrad++;
		}
	}

}//end Furn_ScaleGraphData

//-------------------------------------------------------------------
//display background
void Furn_DispBackground()
 {
  //display the menu
	int rpoint[8];
	char *menu =
	"                                     ";
	char untext[80];

	if(*init==INITIAL)
	{
		settextjustify(LEFT_TEXT,TOP_TEXT);
		setcolor(14);

	//display cursor area
		setcolor(15);

		rpoint[0]=rpoint[6] = 0;
		rpoint[1]=rpoint[3] = Top;
		rpoint[2]=rpoint[4] = 639;
		rpoint[5]=rpoint[7] = Bottom;
	//  fillpoly(4,rpoint);

		setfillstyle(SOLID_FILL,1);  //1
		settextstyle(0,0,1);
	//  setcolor(15);
		bar(0,Top,639,Bottom);
		rectangle(rpoint[0],rpoint[1],rpoint[2],rpoint[5]);
//		(void)strcpy(untext,"Au Curseur");
//		outtextxy(320-textwidth(untext)/2,Furn_GyFirst,untext);

		if (V10DispZorP)
		{
			char *azone = " Zn";

			for (int i=0; i<Furn_NbZone; i++)
			{
				azone[2] = (char)(i+48);
				outtextxy(Furn_GxPV1+i*Furn_DGxPV1,Furn_GyFirst,azone);
			}
//  VB 1-21-98
			//recup temp value position
//			Furn_GxTrec = Furn_GxPV1 + Furn_NbZone*Furn_DGxPV1;
//			outtextxy(Furn_GxTrec,Furn_GySecond,"Recup(øC)");
//			Furn_GxTrec += textwidth("Rec");

		//line data
			outtextxy(10,Furn_GyPV,"PV(øC):");
			outtextxy(10,Furn_GySP,"SP(øC):");
			line(Xline2,Bottom,Xline2,Top);
			outtextxy(Xline2+10,Furn_GyPV,"QA(%):");
			outtextxy(Xline2+10,Furn_GySP,"QG(%):");

			TextPos=Xline2+10+textwidth("QG(%):    ");
		}
		else
		{
			outtextxy(10,Furn_GyPtemp,
			"TdefCalc(øC): 1234  TdefMes(øC): 1234  TDecal(øC): 1234");

			outtextxy(10,Furn_GyPmis,
			"Obj(øC)     : 1234  Sej(mn)    : 1234  PF(æm)    : 12345  Dec(æm): 12345");
		}//V10DispZorP

		setfillstyle(SOLID_FILL,Background_Color);
		bar(Furn_GxZero+Furn_GxLarge,23,639,Cursor_Bottom);
		bar(0,23,639,Furn_GyZero);
		bar(0,23,Furn_GxZero,Cursor_Bottom);
		bar(0,Cursor_Top-1,Furn_GxZero+Furn_GxLarge+5,Cursor_Bottom);

		//display the axis
		//axis
		settextjustify(LEFT_TEXT,TOP_TEXT);
	}

	if (V10DispZorP)
	{
		char *azone = " Zn";
		settextjustify(RIGHT_TEXT,TOP_TEXT);

		for (int i=0; i<Furn_NbZone; i++)
		{
			azone[2] = (char)(i+48);
			outtextxy(Furn_GxPV1+textwidth("   ")+i*(Furn_DGxPV1),Furn_GyFirst,azone);
			//VB:1-26-98
			outtextxy(TextPos+textwidth(" ")+i*Furn_DGxPV1,Furn_GyFirst,azone);
		}
	}
	// abscissa
	setcolor(15);
	line(Furn_GxZero,Furn_GyZero+Furn_GyHaut,
			Furn_GxZero+Furn_GxLarge+19,Furn_GyZero+Furn_GyHaut);

	moveto(Furn_GxZero+Furn_GxLarge+19,Furn_GyZero+Furn_GyHaut);

	linerel(-7,-4);
	moverel(7,4);
	linerel(-7,4);

/*    VB 1-21-98
		strcpy(untext,"t");

		outtextxy(638-textwidth(untext),
			Furn_GyZero+Furn_GyHaut-textheight(untext)/2,untext);
*/

	if (Furn_nbtempGrad)
	{
		 //left ordinate axis
		line(Furn_GxZero,Furn_GyZero+Furn_GyHaut,Furn_GxZero,Furn_GyZero);
		moveto(Furn_GxZero,Furn_GyZero);
		linerel(-5, 5);
		moverel( 5,-5);
		linerel( 5, 5);
	}//left ordinate axis

	if (Furn_nbmmGrad || Furn_nbmnGrad || Furn_nbpcGrad)
	{
		 //right ordinate axis
		line(Furn_GxZero+Furn_GxLarge,Furn_GyZero+Furn_GyHaut,
				Furn_GxZero+Furn_GxLarge,Furn_GyZero);
		moveto(Furn_GxZero+Furn_GxLarge,Furn_GyZero);
		linerel(-5, 5);
		moverel( 5,-5);
		linerel( 5, 5);
	}//right ordinate axis

	int j,ygrad;

	if (Furn_nbtempGrad)
	{
		 //temperature graduation
		settextjustify(LEFT_TEXT,TOP_TEXT);
		strcpy(untext,"øC");
		outtextxy(Furn_GxZero-textwidth(untext)/2-15,
		Furn_GyZero-textheight(untext)/2-3,untext);

		settextjustify(RIGHT_TEXT,CENTER_TEXT);

		for (ygrad=(int)V10Scaler[0]+Furn_dtempGrad,j=0;
			j<Furn_nbtempGrad;ygrad+=Furn_dtempGrad,j++)
		{
			(void)itoa(ygrad,untext,10);
			outtextxy(Furn_GxZero-7,Furn_GtempGradp[j],untext);
			moveto(Furn_GxZero,Furn_GtempGradp[j]);
			linerel(-5,0);
		}
	}//temperature grad

	if (Furn_nbmnGrad)
	{
		 //minute graduation
		int twoaxis = 0;

		if (!Furn_nbmmGrad)
		{
			strcpy(untext,"mn");
			settextjustify(LEFT_TEXT,CENTER_TEXT);
		}
		else
		{
			twoaxis++;
			strcpy(untext,"mn æm");
			settextjustify(RIGHT_TEXT,TOP_TEXT);
		}
		outtextxy(Furn_GxZero + Furn_GxLarge
			+ (2*twoaxis-1)*textwidth(untext)/2,
		Furn_GyZero-textheight(untext)/2-3,untext);

		for (ygrad=(int)V10Scaler[2]+Furn_dmnGrad,j=0;
			j<Furn_nbmnGrad;ygrad+=Furn_dmnGrad,j++)
		{
			(void)itoa(ygrad,untext,10);
			outtextxy(Furn_GxZero+Furn_GxLarge-7*(2*twoaxis-1),
					Furn_GtempGradp[j],untext);
			moveto(Furn_GxZero+Furn_GxLarge,Furn_GtempGradp[j]);
			linerel(-5*(2*twoaxis-1),0);
		}
	}//minute grad

	if (Furn_nbmmGrad)
	{
		//æm graduation
		if (!Furn_nbmnGrad)
		{
			strcpy(untext,"æm");
			settextjustify(RIGHT_TEXT,CENTER_TEXT);
			outtextxy(Furn_GxZero + Furn_GxLarge + textwidth(untext)/2,
				Furn_GyZero-4-textheight(untext),untext);
		}
		settextjustify(LEFT_TEXT,CENTER_TEXT);

		for (ygrad=(int)V10Scaler[6]+Furn_dmmGrad,j=0;
			j<Furn_nbmmGrad;ygrad+=Furn_dmmGrad,j++)
		{
			(void)itoa(ygrad,untext,10);
			outtextxy(Furn_GxZero+Furn_GxLarge+7,Furn_GmmGradp[j],untext);
			moveto(Furn_GxZero+Furn_GxLarge,Furn_GtempGradp[j]);
			linerel(5,0);
		}
	}//æm grad


	if (Furn_nbpcGrad)
	{
		 //% graduation
		strcpy(untext,"%");
		settextjustify(LEFT_TEXT,CENTER_TEXT);
		outtextxy(Furn_GxZero + Furn_GxLarge + textwidth(untext)/2,
		Furn_GyZero-4-textheight(untext),untext);

		for (ygrad=(int)V10Scaler[4]+Furn_dpcGrad,j=0;
			j<Furn_nbpcGrad;ygrad+=Furn_dpcGrad,j++)
		{
			(void)itoa(ygrad,untext,10);
			outtextxy(Furn_GxZero+Furn_GxLarge+7,Furn_GpcGradp[j],untext);
			moveto(Furn_GxZero+Furn_GxLarge,Furn_GpcGradp[j]);
			linerel(5,0);
		}
	}//% grad
}//end Furn_DispBackground

void Save_Dates()
{
	APPELHS("Ecran,Recupere,HEURE,1");
	sscanf(HsChaine,"%d",&Furn_DispBegDate[3]);

	APPELHS("Ecran,Recupere,MIN  ,1");
	sscanf(HsChaine,"%d",&Furn_DispBegDate[4]);

	APPELHS("Ecran,Recupere,SEC  ,1");
	sscanf(HsChaine,"%d",&Furn_DispBegDate[5]);

	APPELHS("Ecran,Recupere,JOUR ,1");
	sscanf(HsChaine,"%d",&Furn_DispBegDate[2]);

	APPELHS("Ecran,Recupere,MOIS ,1");
	sscanf(HsChaine,"%d",Furn_DispBegDate[1]);

	APPELHS("Ecran,Recupere,AN   ,1");
	sscanf(HsChaine,"%d",Furn_DispBegDate[0]);

	APPELHS("Ecran,Recupere,HEURE,2");
	sscanf(HsChaine,"%d",Furn_DispEndDate[3]);

	APPELHS("Ecran,Recupere,MIN  ,2");
	sscanf(HsChaine,"%d",Furn_DispEndDate[4]);

	APPELHS("Ecran,Recupere,SEC  ,2");
	sscanf(HsChaine,"%d",Furn_DispEndDate[5]);

	APPELHS("Ecran,Recupere,JOUR ,2");
	sscanf(HsChaine,"%d",Furn_DispEndDate[2]);

	APPELHS("Ecran,Recupere,MOIS ,2");
	sscanf(HsChaine,"%d",Furn_DispEndDate[1]);

	APPELHS("Ecran,Recupere,AN   ,2");
	sscanf(HsChaine,"%d",Furn_DispEndDate[0]);

	Furn_NewHistDisp = 1;
	setviewport(Furn_GxZero+1,34,599,Furn_GyZero+Furn_GyHaut-1,0);
	clearviewport();
	setviewport(0,0,639,349,0);
	Furn_Erase();
	Furn_Display();

	Furn_UpdateData();
	Furn_UpdateGraphCursor(0,1);
	APPELHS("Ecran,Premier,HEURE,1");

}
//-------------------------------------------------------------------
//end Furn_OVL1.CPP