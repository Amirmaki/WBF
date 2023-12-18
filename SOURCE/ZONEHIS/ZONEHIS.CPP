//include files
//-------------------------------------------------------------------------
//#define _OWNER_TEMPHIS_H

#ifndef __DIR_H
#include <dir.h>
#endif
#ifndef __DOS_H
#include <dos.h>
#endif
#ifndef __FSTREAM_H
#include <fstream.h>
#endif
#ifndef __MATH_H
#include <math.h>
#endif
#ifndef __STDIO_H
#include <stdio.h>
#endif

#define __OWNER_HISZWIND
#define __OWNER_HISPWIND


#ifndef __HISZWIND_H
//#include "hiszwind.h"
#endif
#ifndef __HISPWIND_H
//#include "hispwind.h"
#endif
#ifndef __TEMPHIS_H
#include "temphis.h"
#endif

#include "database.h"

#include "globdat.h"
#include "winstruct.h"
//#define __OWNER_DATAHIS_H
#include "datahis.h"

//--------------------------------------------------------------------------
void zoneTempHis(Furnace *afurn)
// Update history with furnace temperatures and setpoints
{ // Read the data
	int ztemp[maxNbZone];
	int zspt[maxNbZone];
	int zqg[maxNbZone];
	int zqa[maxNbZone];
	float zqgperc,zqaperc;
	char filename[MAXPATH];
	sprintf(filename,"%s%s",DB_Path_hisfour,ZONEHIST);

	int rect;
	int ladate[6];
	Array& Z = afurn->theZone();
	int nbz = afurn->nbZone();

	for (int iz=0;iz<nbz;iz++)
	{
		ztemp[iz] = (int) ((Zone&)(Z[iz])).celTemp();
		zspt[iz] = (int) ((Zone&)(Z[iz])).celSetPoint();

		if (zspt[iz] < 0) zspt[iz] = 0;

		if (iz)
		{
			zqgperc = ((Zone&)(Z[iz])).maxGasFlowRate();
			if (zqgperc) zqgperc = 100*((Zone&)(Z[iz])).gasFlowRate()/zqgperc;
			else zqgperc = 0;
			zqg[iz] = (int)zqgperc;
			zqaperc = ((Zone&)(Z[iz])).maxAirFlowRate();
			if (zqaperc) zqaperc = 100*((Zone&)(Z[iz])).airFlowRate()/zqaperc;
			else zqaperc = 0;
			zqa[iz] = (int)zqaperc;
		}
		else zqg[iz]=zqa[iz]=0;
	}

	rect = (int) afurn->celRecupTemp();
	struct date HisDate;
	getdate(&HisDate);
	struct time HisTime;
	gettime(&HisTime);
	ladate[0] = HisDate.da_year;
	ladate[1] = (int)(HisDate.da_mon);
	ladate[2] = (int)(HisDate.da_day);
	ladate[3] = (int)(HisTime.ti_hour);
	ladate[4] = (int)(HisTime.ti_min);
	ladate[5] = (int)(HisTime.ti_sec);
	//save the data in file
	long nbRecord,oldest,newone;
	int nbzonet=nbz;
	struct ffblk fb;
	fstream hisfile;

	if ( !findfirst(filename,&fb,0) )
	{ // the file exists, read the header
		hisfile.open(filename,ios::in|ios::binary);
		if (hisfile)
		{
			hisfile.read( (char*)&nbRecord,sizeof(long) );
			hisfile.read( (char*)&oldest,sizeof(long) );
			hisfile.read( (char*)&nbzonet,sizeof(int) );

			if (nbRecord < MaxNbZTHrecord)
			{
				nbRecord++;
				newone = nbRecord;
			}
			else
			{
				newone = oldest;
				if (oldest >= MaxNbZTHrecord)
					oldest = 1;
				else
					oldest++;
			} // nbRecord
			hisfile.close();
		} // hisfile
		hisfile.open(filename,ios::out|ios::binary|ios::ate);

		if (hisfile)
		{
			hisfile.seekp(0,ios::beg);
			hisfile.write( (char*)&nbRecord,sizeof(long) );
			hisfile.write( (char*)&oldest,sizeof(long) );
			  // position the file on record to write
			long thepos = 2*sizeof(long) + sizeof(int)
				+ (newone-1)*(7+4*nbzonet)*sizeof(int);
			hisfile.seekp(thepos,ios::beg);

			if (!hisfile)
				hisfile.clear(0);
		} // hisfile open success
	} // file exists
	else
	{ // create the file
		nbRecord = oldest = 1;
		hisfile.open(filename,ios::out|ios::binary);

		if (hisfile)
		{
			hisfile.write( (char*)&nbRecord,sizeof(long) );
			hisfile.write( (char*)&oldest, sizeof(long) );
			hisfile.write( (char*)&nbzonet, sizeof(int) );
		}
	} // file creation
  // update the file
	if (nbz < nbzonet)
	{
		for (iz=nbz;iz<nbzonet;iz++)
			ztemp[iz]=zspt[iz]=zqg[iz]=zqa[iz]= 0;
	}

	if (hisfile)
	{
		hisfile.write( (char*)ladate,6*sizeof(int) );
		hisfile.write( (char*)&rect,sizeof(int) );
		hisfile.write( (char*)ztemp,nbzonet*sizeof(int) );
		hisfile.write( (char*)zspt,nbzonet*sizeof(int) );
		hisfile.write( (char*)zqg,nbzonet*sizeof(int) );
		hisfile.write( (char*)zqa,nbzonet*sizeof(int) );
	}
	hisfile.close();
}


//-------------------------------------------------------------
//update history with discharged product values
void prodTempHis(Product *disP, EndzTargetList *theTargList,
		 float exitf, float dect)
 {
// if (!V14ActiveSave)
//{
/*
 ofstream htptrack;
 htptrack.open("HTPTRACK.DAT",ios::out|ios::app);
 htptrack << "prodTempHis: entr‚e ";
 htptrack.close();
*/
  //prevent from operator interruption
  //while(!ecrAccess);
  //ecrAccess=0;
/*
 htptrack.open("HTPTRACK.DAT",ios::out|ios::app);
 htptrack << "1 ";
 htptrack.close();
*/
  //read the data
  int pval[7];
  long oxydec[2];
  int ladate[6];
  //read target curve
  int theTarget = 0;
  if (theTargList != NULL)
	{
/*
 htptrack.open("HTPTRACK.DAT",ios::out|ios::app);
 htptrack << "2 ";
 htptrack.close();
*/
    int theType = (disP->theBatch())->TypeNumber;
    int theProd = (disP->theBatch())->ProductionRate;
    int PchTemp = (disP->theBatch())->TempCha;
#ifdef MEAS_CHAR_TEMP
    EndzTarget& theTargCurve = theTargList->getCurve(theProd,theType,PchTemp) ;
#else
    EndzTarget& theTargCurve = theTargList->getCurve(theProd,theType,0) ;
#endif
    if (theTargCurve != (EndzTarget&)NOOBJECT)
      {
/*
 htptrack.open("HTPTRACK.DAT",ios::out|ios::app);
 htptrack << "3 ";
 htptrack.close();
*/
       int ntg = theTargCurve.nbZone();
		 int *targets = theTargCurve.theCurve();
       if (ntg) theTarget = targets[ntg-1];
      }
   }

/*
 htptrack.open("HTPTRACK.DAT",ios::out|ios::app);
 htptrack << "4 ";
 htptrack.close();
*/
  pval[0] = (int) disP->celTopSurfTemp();
  pval[1] = (int) disP->celMeanTemp();
  pval[2] = (int) disP->celCoreTemp();
  pval[3] = (int) disP->celBotSurfTemp();
  pval[4] = (int) ( (disP->residTime())/60 );
  if (exitf > 0)
    {
	  pval[5] = (int)exitf;
	 }
    else pval[5] = 0;

  if (dect > 0)
	 {
     pval[6] = (int)dect;
    }
    else pval[6] = 0;

  oxydec[0] = (long) (10000*disP->perteFeu());
  oxydec[1] = (long) (10000*disP->decarb());
/*
 htptrack.open("HTPTRACK.DAT",ios::out|ios::app);
 htptrack << "5 ";
 htptrack.close();
*/
  struct date HisDate;
  getdate(&HisDate);
  struct time HisTime;
  gettime(&HisTime);
  ladate[0] = HisDate.da_year;
  ladate[1] = (int)(HisDate.da_mon);
  ladate[2] = (int)(HisDate.da_day);
  ladate[3] = (int)(HisTime.ti_hour);
  ladate[4] = (int)(HisTime.ti_min);
  ladate[5] = (int)(HisTime.ti_sec);
/*
 htptrack.open("HTPTRACK.DAT",ios::out|ios::app);
 htptrack << "6 ";
 htptrack.close();
*/
  //save the data in file
  long nbRecord,oldest,newone;
  struct ffblk fb;
  fstream hisfile;
  if ( !findfirst(PHistFile,&fb,0) )
  {
	  //the file exists, read the header
	  hisfile.open(PHistFile,ios::in|ios::binary);
	  if (hisfile)
		 {
/*
 htptrack.open("HTPTRACK.DAT",ios::out|ios::app);
 htptrack << "7 ";
 htptrack.close();
*/
	hisfile.read( (char*)&nbRecord,sizeof(long) );
	hisfile.read( (char*)&oldest,sizeof(long) );

	if (nbRecord < MaxNbPTHrecord)
	{
		nbRecord++;
		newone = nbRecord;
	}
	else
	{
		newone = oldest;
		if (oldest >= MaxNbPTHrecord)
		  {
			oldest = 1;
		  }
		  else oldest++;
	  }//nbRecord
	hisfile.close();
/*
 htptrack.open("HTPTRACK.DAT",ios::out|ios::app);
 htptrack << "8 ";
 htptrack.close();
*/
       }//hisfile
     hisfile.open(PHistFile,ios::out|ios::binary|ios::ate);
/*
 htptrack.open("HTPTRACK.DAT",ios::out|ios::app);
 htptrack << "9 ";
 htptrack.close();
*/
     if (hisfile)
      {
       hisfile.seekp(0,ios::beg);
       hisfile.write( (char*)&nbRecord,sizeof(long) );
		 hisfile.write( (char*)&oldest,sizeof(long) );
       //position the file on record to write
		 long thepos = 2*sizeof(long) +
		     (newone-1)*(14*sizeof(int)+2*sizeof(long));
       hisfile.seekp(thepos,ios::beg);
       if (!hisfile) hisfile.clear(0);
		}//hisfile open success
/*
 htptrack.open("HTPTRACK.DAT",ios::out|ios::app);
 htptrack << "A ";
 htptrack.close();
*/
    }//file exists
	else
    {
     //create the file
     nbRecord = oldest = 1;
	  hisfile.open(PHistFile,ios::out|ios::binary);
     if (hisfile)
       {
	hisfile.write( (char*)&nbRecord,sizeof(long) );
	hisfile.write( (char*)&oldest, sizeof(long) );
       }
    }//file creation
  //update the file
  if (hisfile)
   {
    hisfile.write( (char*)ladate,6*sizeof(int) );
	 hisfile.write( (char*)pval,7*sizeof(int) );
    hisfile.write( (char*)oxydec,2*sizeof(long) );
    hisfile.write( (char*)(&theTarget),sizeof(int) );
   }
  hisfile.close();
/*
 htptrack.open("HTPTRACK.DAT",ios::out|ios::app);
 htptrack << "prodTempHis sortie\n";
 htptrack.close();
*/
//  ecrEnableAccess();
//  }//V14ActiveSave
 }

//------------------------------------------------------------------
//update history with energy counter, argument is amount of gas Nm3
void gasEnergyHis(float nbnm3)
 {
// if (!V14ActiveSave)
// {
  //prevent from operator interruption
  //while(!ecrAccess);
  //ecrAccess=0;
  //read the data
  long gasamount = (long)(100*nbnm3);
  int ladate[6];
  struct date HisDate;
  getdate(&HisDate);
  struct time HisTime;
  gettime(&HisTime);
  ladate[0] = HisDate.da_year;
  ladate[1] = (int)(HisDate.da_mon);
  ladate[2] = (int)(HisDate.da_day);
  ladate[3] = (int)(HisTime.ti_hour);
  ladate[4] = (int)(HisTime.ti_min);
  ladate[5] = (int)(HisTime.ti_sec);
  //save the data in file
  long nbRecord,oldest,newone;
  struct ffblk fb;
  fstream hisfile;
  if ( !findfirst(EHistFile,&fb,0) )
    {
     //the file exists, read the header
     hisfile.open(EHistFile,ios::in|ios::binary);
	  if (hisfile)
       {
	hisfile.read( (char*)&nbRecord,sizeof(long) );
	hisfile.read( (char*)&oldest,sizeof(long) );
	if (nbRecord < MaxNbEnergyHrecord)
	  {
	   nbRecord++;
		newone = nbRecord;
	  }
	 else
	  {
	   newone = oldest;
	   if (oldest >= MaxNbEnergyHrecord)
		  {
	      oldest = 1;
		  }
	     else oldest++;
	  }//nbRecord
	hisfile.close();
       }//hisfile
     hisfile.open(EHistFile,ios::out|ios::binary|ios::ate);
     if (hisfile)
      {
       hisfile.seekp(0,ios::beg);
       hisfile.write( (char*)&nbRecord,sizeof(long) );
       hisfile.write( (char*)&oldest,sizeof(long) );
		 //position the file on record to write
       long thepos = 2*sizeof(long) +
		     (newone-1)*(6*sizeof(int) + sizeof(long));
		 hisfile.seekp(thepos,ios::beg);
       if (!hisfile) hisfile.clear(0);
      }//hisfile open success
    }//file exists
   else
    {
	  //create the file
     nbRecord = oldest = 1;
     hisfile.open(EHistFile,ios::out|ios::binary);
     if (hisfile)
		 {
	hisfile.write( (char*)&nbRecord,sizeof(long) );
	hisfile.write( (char*)&oldest, sizeof(long) );
       }
    }//file creation
  //update the file
  if (hisfile)
   {
    hisfile.write( (char*)ladate,6*sizeof(int) );
    hisfile.write( (char*)(&gasamount),sizeof(long) );
   }
  hisfile.close();
//  ecrEnableAccess();
//  }//V14ActiveSave
 }


//------------------------------------------------------------------
//give number of starting and ending record corresponding to given
//starting and ending date accessible in ZONETEMP.HTZ, returns
//the number of records, including both ends
//arguments are pointers to beginning date, ending date,
//beginning record, ending record

long zoneTempRecNum(int *begDate,int *endDate,long *begRec,long *endRec)
{
	long toreturn = 0;
	long foundrec = 0;
	long nbrec=0,oldest=0;
	int nbz;
	int unedate[6];
	char filename[MAXPATH];
	sprintf(filename,"%s%s",DB_Path_hisfour,ZONEHIST);

	*begRec = 0;
	*endRec = 0;

	ifstream hisfile;
	hisfile.open(filename,ios::in|ios::binary);

	if (hisfile)
	{
		long thepos,currec,upperbound,lowerbound=1;
		double upperbdate,lowerbdate,curdate,refdate;

		hisfile.read((char*)&nbrec,sizeof(long));
		hisfile.read((char*)&oldest,sizeof(long));
		hisfile.read((char*)&nbz,sizeof(int));

		upperbound=nbrec;
		currec = oldest;
		 //search with begDate
		 //pointer to oldest record
		thepos = 2*sizeof(long) + sizeof(int) +
		(currec-1)*(7+4*nbz)*sizeof(int);

		hisfile.seekg(thepos,ios::beg);
		hisfile.read ((char*)unedate, 6*sizeof(int));
		refdate = (double)begDate[0];

		for (int i=1;i<6;refdate = refdate*100+(double)begDate[i++]);
		curdate = (double)unedate[0];

		for (i=1;i<6;curdate = curdate*100+(double)unedate[i++]);

		if (refdate <= curdate)
		{
			//older date than oldest record date
			foundrec=currec;
		}
		else
		{
	  //check for bounds
			if (currec == 1)
			{
			//oldest is the first
				lowerbound = 2;
			}
			else if (currec >= MaxNbZTHrecord)
			{
			//oldest is the last
				lowerbound = 1;
				upperbound = MaxNbZTHrecord-1;
			}
			else
			{
				//check if search rec is before of after oldest record
				lowerbound = 1;
				upperbound = currec-1;
				thepos = 2*sizeof(long) + sizeof(int) +
				(upperbound-1)*(7+4*nbz)*sizeof(int);

				hisfile.seekg(thepos,ios::beg);
				hisfile.read ((char*)unedate, 6*sizeof(int));
				upperbdate = (double)unedate[0];

				for (i=1;i<6;
				upperbdate = upperbdate*100+(double)unedate[i++]);

				if (upperbdate < refdate)
				{
					//refdate is later than most recent file
					foundrec = upperbound;
				}
				else
				{
					thepos = 2*sizeof(long) + sizeof(int) +
					(lowerbound-1)*(7+4*nbz)*sizeof(int);

					hisfile.seekg(thepos,ios::beg);
					hisfile.read ((char*)unedate, 6*sizeof(int));
					lowerbdate = (double)unedate[0];

					for (i=1;i<6;
						lowerbdate = lowerbdate*100+(double)unedate[i++]);
					if (refdate < lowerbdate)
					{
						upperbound = nbrec;
						lowerbound = currec+1;
					}
				}//refdate earlier then most recent file
			}//oldest file not the first and not the last
		}//refdate later than oldest

		if (!foundrec)
		{
	  //dichotomie entre lowerbound et upperbound
	  //lowerbound dat
			thepos = 2*sizeof(long) + sizeof(int) +
			 (lowerbound-1)*(7+4*nbz)*sizeof(int);

			hisfile.seekg(thepos,ios::beg);
			hisfile.read ((char*)unedate, 6*sizeof(int));
			lowerbdate = (double)unedate[0];

			for (i=1;i<6;lowerbdate = lowerbdate*100+(double)unedate[i++]);
			currec = (upperbound+lowerbound)/2;

			while(currec != upperbound && currec != lowerbound)
			{
		 //currec date
				thepos = 2*sizeof(long) + sizeof(int) +
				  (currec-1)*(7+4*nbz)*sizeof(int);

				hisfile.seekg(thepos,ios::beg);
				hisfile.read ((char*)unedate, 6*sizeof(int));
				curdate = (double)unedate[0];

				for (i=1;i<6;curdate = curdate*100+(double)unedate[i++]);

				if (refdate >= lowerbdate && refdate <= curdate)
				{
					upperbound = currec;
				}
				else
					lowerbound = currec;

				currec = (upperbound+lowerbound)/2;
			}//while currec
			//use rec with closest date from refdate
			//lowerbound dat
			thepos = 2*sizeof(long) + sizeof(int) +
				(lowerbound-1)*(7+4*nbz)*sizeof(int);

			hisfile.seekg(thepos,ios::beg);
			hisfile.read ((char*)unedate, 6*sizeof(int));
			lowerbdate = (double)unedate[0];

			for (i=1;i<6;lowerbdate = lowerbdate*100+(double)unedate[i++]);
			//upperbound date
			thepos = 2*sizeof(long) + sizeof(int) +
				(upperbound-1)*(7+4*nbz)*sizeof(int);

			hisfile.seekg(thepos,ios::beg);
			hisfile.read ((char*)unedate, 6*sizeof(int));
			upperbdate = (double)unedate[0];

			for (i=1;i<6;upperbdate = upperbdate*100+(double)unedate[i++]);
			if (fabs(refdate-lowerbdate) > fabs(refdate-upperbdate))
			{
				foundrec = upperbound;
			}
			else
				foundrec = lowerbound;
		}//!foundrec
		if (foundrec)
		{
			*begRec = foundrec;
			thepos = 2*sizeof(long) + sizeof(int) +
				(foundrec-1)*(7+4*nbz)*sizeof(int);

			hisfile.seekg(thepos,ios::beg);
			hisfile.read ((char*)begDate, 6*sizeof(int));
		}

		 //search endDate record
		foundrec = 0;
		lowerbound = 1;
		upperbound=nbrec;
		currec = oldest;
		 //search with endDate
		 //pointer to oldest record
		thepos = 2*sizeof(long) + sizeof(int) +
			(currec-1)*(7+4*nbz)*sizeof(int);
		hisfile.seekg(thepos,ios::beg);
		hisfile.read ((char*)unedate, 6*sizeof(int));
		refdate = (double)endDate[0];

		for (i=1;i<6;refdate = refdate*100+(double)endDate[i++]);
			curdate = (double)unedate[0];

		for (i=1;i<6;curdate = curdate*100+(double)unedate[i++]);

		if (refdate <= curdate)
		{
	  //older date than oldest record date
			foundrec=currec;
		}
		else
		{
	  //check for bounds
			if (currec == 1)
			{
				//oldest is the first
				lowerbound = 2;
			}
			else if (currec >= MaxNbZTHrecord)
			{
			//oldest is the last
				lowerbound = 1;
				upperbound = MaxNbZTHrecord-1;
			}
			else
			{
				//check if search rec is before of after oldest record
				lowerbound = 1;
				upperbound = currec-1;
				thepos = 2*sizeof(long) + sizeof(int) +
				(upperbound-1)*(7+4*nbz)*sizeof(int);

				hisfile.seekg(thepos,ios::beg);
				hisfile.read ((char*)unedate, 6*sizeof(int));

				upperbdate = (double)unedate[0];

				for (i=1;i<6;
					upperbdate = upperbdate*100+(double)unedate[i++]);
				if (upperbdate < refdate)
				{
				//refdate is later than most recent file
					foundrec = upperbound;
				}
				else
				{
					thepos = 2*sizeof(long) + sizeof(int) +
						(lowerbound-1)*(7+4*nbz)*sizeof(int);

					hisfile.seekg(thepos,ios::beg);
					hisfile.read ((char*)unedate, 6*sizeof(int));
					lowerbdate = (double)unedate[0];

					for (i=1;i<6;
						lowerbdate = lowerbdate*100+(double)unedate[i++]);

					if (refdate < lowerbdate)
					{
						upperbound = nbrec;
						lowerbound = currec+1;
					}
				}//refdate earlier then most recent file
			}//oldest file not the first and not the last
		}//refdate later than oldest

		if (!foundrec)
		{
			//dichotomie entre lowerbound et upperbound
			//lowerbound dat
			thepos = 2*sizeof(long) + sizeof(int) +
				(lowerbound-1)*(7+4*nbz)*sizeof(int);

			hisfile.seekg(thepos,ios::beg);
			hisfile.read ((char*)unedate, 6*sizeof(int));
			lowerbdate = (double)unedate[0];

			for (i=1;i<6;lowerbdate = lowerbdate*100+(double)unedate[i++]);
				currec = (upperbound+lowerbound)/2;
			while(currec != upperbound && currec != lowerbound)
			{
				//currec date
				thepos = 2*sizeof(long) + sizeof(int) +
					(currec-1)*(7+4*nbz)*sizeof(int);

				hisfile.seekg(thepos,ios::beg);
				hisfile.read ((char*)unedate, 6*sizeof(int));
				curdate = (double)unedate[0];

				for (i=1;i<6;curdate = curdate*100+(double)unedate[i++]);

				if (refdate >= lowerbdate && refdate <= curdate)
				{
					upperbound = currec;
				}
				else
					lowerbound = currec;

				currec = (upperbound+lowerbound)/2;
			}//while currec
	  //use rec with closest date from refdate
	  //lowerbound dat
			thepos = 2*sizeof(long) + sizeof(int) +
				(lowerbound-1)*(7+4*nbz)*sizeof(int);

			hisfile.seekg(thepos,ios::beg);
			hisfile.read ((char*)unedate, 6*sizeof(int));
			lowerbdate = (double)unedate[0];

			for (i=1;i<6;lowerbdate = lowerbdate*100+(double)unedate[i++]);
			//upperbound date
			thepos = 2*sizeof(long) + sizeof(int) +
			(upperbound-1)*(7+4*nbz)*sizeof(int);

			hisfile.seekg(thepos,ios::beg);
			hisfile.read ((char*)unedate, 6*sizeof(int));
			upperbdate = (double)unedate[0];

			for (i=1;i<6;upperbdate = upperbdate*100+(double)unedate[i++]);

			if (fabs(refdate-lowerbdate) > fabs(refdate-upperbdate))
			{
				foundrec = upperbound;
			}
			else
				foundrec = lowerbound;
		}//!foundrec

		if (foundrec)
		{
			*endRec = foundrec;
			thepos = 2*sizeof(long) + sizeof(int) +
				(foundrec-1)*(7+4*nbz)*sizeof(int);

			hisfile.seekg(thepos,ios::beg);
			hisfile.read ((char*)endDate, 6*sizeof(int));
		}
		 //check number of record
		if (*endRec > *begRec)
		{
			toreturn = *endRec - *begRec + 1;
		}
		else if (*begRec > *endRec)
		{
			toreturn = nbrec - *begRec + 1 + *endRec;
		}
	}//hisfile
	hisfile.close();
	return toreturn;
}
//----------------------------------------------------------------------
//give number of starting and ending record corresponding to given
//starting and ending date accessible in DISCTEMP.HTP, returns
//the number of records, including both ends
//arguments are pointers to beginning date, ending date,
//beginning record, ending record
long prodTempRecNum(int *begDate,int *endDate,long *begRec,long *endRec)
{
	long toreturn = 0;
	long foundrec = 0;
	long nbrec=0,oldest=0;
	int unedate[6];
	*begRec = 0;
	*endRec = 0;
	char filename[MAXPATH];
	sprintf(filename,"%s%s",DB_Path_hisfour,DEFHIST);

	ifstream hisfile;
	hisfile.open(filename,ios::in|ios::binary);

	if (hisfile)
	{
		long thepos,currec,upperbound,lowerbound=1;
		double upperbdate,lowerbdate,curdate,refdate;

		hisfile.read((char*)&nbrec,sizeof(long));
		hisfile.read((char*)&oldest,sizeof(long));

		upperbound=nbrec;
		currec = oldest;
		 //search with begDate
		 //pointer to oldest record
		thepos = 2*sizeof(long) +
			(currec-1)*(14*sizeof(int) + 2*sizeof(long));

		hisfile.seekg(thepos,ios::beg);
		hisfile.read ((char*)unedate, 6*sizeof(int));
		refdate = (double)begDate[0];

		for (int i=1;i<6;refdate = refdate*100+(double)begDate[i++]);
		curdate = (double)unedate[0];

		for (i=1;i<6;curdate = curdate*100+(double)unedate[i++]);

		if (refdate <= curdate)
		{
		//older date than oldest record date
			foundrec=currec;
		}
		else
		{
			//check for bounds
			if (currec == 1)
			{
				//oldest is the first
				lowerbound = 2;
			}
			else if (currec >= MaxNbPTHrecord)
			{
			//oldest is the last
				lowerbound = 1;
				upperbound = MaxNbPTHrecord-1;
			}
			else
			{
			//check if search rec is before of after oldest record
				lowerbound = 1;
				upperbound = currec-1;
				thepos = 2*sizeof(long) +
				(upperbound-1)*(14*sizeof(int) + 2*sizeof(long));

				hisfile.seekg(thepos,ios::beg);
				hisfile.read ((char*)unedate, 6*sizeof(int));
				upperbdate = (double)unedate[0];

				for (i=1;i<6;
					upperbdate = upperbdate*100+(double)unedate[i++]);
				if (upperbdate < refdate)
				{
		 //refdate is later than most recent file
					foundrec = upperbound;
				}
				else
				{
					thepos = 2*sizeof(long) +
					(lowerbound-1)*(14*sizeof(int) + 2*sizeof(long));

					hisfile.seekg(thepos,ios::beg);
					hisfile.read ((char*)unedate, 6*sizeof(int));
					lowerbdate = (double)unedate[0];

					for (i=1;i<6;
						lowerbdate = lowerbdate*100+(double)unedate[i++]);

					if (refdate < lowerbdate)
					{
						upperbound = nbrec;
						lowerbound = currec+1;
					}
				}//refdate earlier then most recent file
			}//oldest file not the first and not the last
		}//refdate later than oldest

		if (!foundrec)
		{
	  //dichotomie entre lowerbound et upperbound
	  //lowerbound dat
	  thepos = 2*sizeof(long) +
			(lowerbound-1)*(14*sizeof(int) + 2*sizeof(long));
	  hisfile.seekg(thepos,ios::beg);
	  hisfile.read ((char*)unedate, 6*sizeof(int));
	  lowerbdate = (double)unedate[0];
	  for (i=1;i<6;lowerbdate = lowerbdate*100+(double)unedate[i++]);
	  currec = (upperbound+lowerbound)/2;
	  while(currec != upperbound && currec != lowerbound)
	   {
	    //currec date
	    thepos = 2*sizeof(long) +
		     (currec-1)*(14*sizeof(int) + 2*sizeof(long));
	    hisfile.seekg(thepos,ios::beg);
		 hisfile.read ((char*)unedate, 6*sizeof(int));
	    curdate = (double)unedate[0];
	    for (i=1;i<6;curdate = curdate*100+(double)unedate[i++]);
	    if (refdate >= lowerbdate && refdate <= curdate)
	      {
	       upperbound = currec;
	      }
	      else  lowerbound = currec;
	    currec = (upperbound+lowerbound)/2;
	   }//while currec
	  //use rec with closest date from refdate
	  //lowerbound dat
	  thepos = 2*sizeof(long) +
			(lowerbound-1)*(14*sizeof(int) + 2*sizeof(long));
	  hisfile.seekg(thepos,ios::beg);
	  hisfile.read ((char*)unedate, 6*sizeof(int));
	  lowerbdate = (double)unedate[0];
	  for (i=1;i<6;lowerbdate = lowerbdate*100+(double)unedate[i++]);
	  //upperbound date
	  thepos = 2*sizeof(long) +
		   (upperbound-1)*(14*sizeof(int) + 2*sizeof(long));
	  hisfile.seekg(thepos,ios::beg);
	  hisfile.read ((char*)unedate, 6*sizeof(int));
	  upperbdate = (double)unedate[0];
	  for (i=1;i<6;upperbdate = upperbdate*100+(double)unedate[i++]);
	  if (fabs(refdate-lowerbdate) > fabs(refdate-upperbdate))
		 {
	     foundrec = upperbound;
	    }
	   else foundrec = lowerbound;
	 }//!foundrec
	if (foundrec)
	 {
	  *begRec = foundrec;
	  thepos = 2*sizeof(long) +
		   (foundrec-1)*(14*sizeof(int) + 2*sizeof(long));
	  hisfile.seekg(thepos,ios::beg);
	  hisfile.read ((char*)begDate, 6*sizeof(int));
	 }

       //search endDate record
       foundrec = 0;
       lowerbound = 1;
       upperbound=nbrec;
       currec = oldest;
       //search with endDate
       //pointer to oldest record
       thepos = 2*sizeof(long) +
		(currec-1)*(14*sizeof(int) + 2*sizeof(long));
       hisfile.seekg(thepos,ios::beg);
       hisfile.read ((char*)unedate, 6*sizeof(int));
       refdate = (double)endDate[0];
       for (i=1;i<6;refdate = refdate*100+(double)endDate[i++]);
       curdate = (double)unedate[0];
       for (i=1;i<6;curdate = curdate*100+(double)unedate[i++]);
       if (refdate <= curdate)
	 {
	  //older date than oldest record date
	  foundrec=currec;
	 }
	else
	 {
	  //check for bounds
	  if (currec == 1)
	    {
	     //oldest is the first
	     lowerbound = 2;
	    }
	    else if (currec >= MaxNbPTHrecord)
	     {
	      //oldest is the last
	      lowerbound = 1;
	      upperbound = MaxNbPTHrecord-1;
	     }
	    else
	     {
	      //check if search rec is before of after oldest record
	      lowerbound = 1;
	      upperbound = currec-1;
	      thepos = 2*sizeof(long) +
		  (upperbound-1)*(14*sizeof(int) + 2*sizeof(long));
	      hisfile.seekg(thepos,ios::beg);
	      hisfile.read ((char*)unedate, 6*sizeof(int));
	      upperbdate = (double)unedate[0];
	      for (i=1;i<6;
			upperbdate = upperbdate*100+(double)unedate[i++]);
	      if (upperbdate < refdate)
		{
		 //refdate is later than most recent file
		 foundrec = upperbound;
		}
	       else
		{
		 thepos = 2*sizeof(long) +
			(lowerbound-1)*(14*sizeof(int) + 2*sizeof(long));
		 hisfile.seekg(thepos,ios::beg);
		 hisfile.read ((char*)unedate, 6*sizeof(int));
		 lowerbdate = (double)unedate[0];
		 for (i=1;i<6;
			lowerbdate = lowerbdate*100+(double)unedate[i++]);
		 if (refdate < lowerbdate)
		   {
		    upperbound = nbrec;
		    lowerbound = currec+1;
		   }
		}//refdate earlier then most recent file
	     }//oldest file not the first and not the last
	 }//refdate later than oldest

       if (!foundrec)
	 {
	  //dichotomie entre lowerbound et upperbound
	  //lowerbound dat
	  thepos = 2*sizeof(long) +
			(lowerbound-1)*(14*sizeof(int) + 2*sizeof(long));
	  hisfile.seekg(thepos,ios::beg);
	  hisfile.read ((char*)unedate, 6*sizeof(int));
	  lowerbdate = (double)unedate[0];
	  for (i=1;i<6;lowerbdate = lowerbdate*100+(double)unedate[i++]);
	  currec = (upperbound+lowerbound)/2;
	  while(currec != upperbound && currec != lowerbound)
	   {
	    //currec date
	    thepos = 2*sizeof(long) +
		     (currec-1)*(14*sizeof(int) + 2*sizeof(long));
	    hisfile.seekg(thepos,ios::beg);
	    hisfile.read ((char*)unedate, 6*sizeof(int));
	    curdate = (double)unedate[0];
	    for (i=1;i<6;curdate = curdate*100+(double)unedate[i++]);
	    if (refdate >= lowerbdate && refdate <= curdate)
	      {
	       upperbound = currec;
	      }
	      else  lowerbound = currec;
	    currec = (upperbound+lowerbound)/2;
	   }//while currec
	  //use rec with closest date from refdate
	  //lowerbound dat
	  thepos = 2*sizeof(long) +
		   (lowerbound-1)*(14*sizeof(int) + 2*sizeof(long));
	  hisfile.seekg(thepos,ios::beg);
	  hisfile.read ((char*)unedate, 6*sizeof(int));
	  lowerbdate = (double)unedate[0];
	  for (i=1;i<6;lowerbdate = lowerbdate*100+(double)unedate[i++]);
	  //upperbound date
	  thepos = 2*sizeof(long) +
		   (upperbound-1)*(14*sizeof(int) + 2*sizeof(long));
	  hisfile.seekg(thepos,ios::beg);
	  hisfile.read ((char*)unedate, 6*sizeof(int));
	  upperbdate = (double)unedate[0];
	  for (i=1;i<6;upperbdate = upperbdate*100+(double)unedate[i++]);
	  if (fabs(refdate-lowerbdate) > fabs(refdate-upperbdate))
	    {
	     foundrec = upperbound;
	    }
	   else foundrec = lowerbound;
	 }//!foundrec
	if (foundrec)
	 {
	  *endRec = foundrec;
	  thepos = 2*sizeof(long) +
		   (foundrec-1)*(14*sizeof(int) + 2*sizeof(long));
	  hisfile.seekg(thepos,ios::beg);
	  hisfile.read ((char*)endDate, 6*sizeof(int));
	 }
       //check number of record
       if (*endRec > *begRec)
	 {
	  toreturn = *endRec - *begRec + 1;
	 }
	else if (*begRec > *endRec)
	 {
	  toreturn = nbrec - *begRec + 1 + *endRec;
	 }
      }//hisfile
      hisfile.close();
      return toreturn;
    }
//----------------------------------------------------------------------
//give number of starting and ending record corresponding to given
//starting and ending date accessible in ENERGY.HTC,
//returns the number of records, including both ends
//arguments are pointers to beginning date, ending date,
//beginning record, ending record, total number of gas Nm3
long gasEnergyRecNum(int *begDate,int *endDate,long *begRec,long *endRec)
    {
     long toreturn = 0;
     long foundrec = 0;
     long nbrec=0,oldest=0;
     int unedate[6];
     *begRec = 0;
     *endRec = 0;

     ifstream hisfile;
     hisfile.open(EHistFile,ios::in|ios::binary);
     if (hisfile)
      {
       long thepos,currec,upperbound,lowerbound=1;
       double upperbdate,lowerbdate,curdate,refdate;
       hisfile.read((char*)&nbrec,sizeof(long));
       hisfile.read((char*)&oldest,sizeof(long));
       upperbound=nbrec;
       currec = oldest;
       //search with begDate
       //pointer to oldest record
       thepos = 2*sizeof(long) +
		(currec-1)*(6*sizeof(int) + sizeof(long));
       hisfile.seekg(thepos,ios::beg);
       hisfile.read ((char*)unedate, 6*sizeof(int));
       refdate = (double)begDate[0];
       for (int i=1;i<6;refdate = refdate*100+(double)begDate[i++]);
       curdate = (double)unedate[0];
       for (i=1;i<6;curdate = curdate*100+(double)unedate[i++]);
       if (refdate <= curdate)
	 {
	  //older date than oldest record date
	  foundrec=currec;
	 }
	else
	 {
	  //check for bounds
	  if (currec == 1)
	    {
	     //oldest is the first
	     lowerbound = 2;
	    }
	    else if (currec >= MaxNbEnergyHrecord)
	     {
	      //oldest is the last
	      lowerbound = 1;
	      upperbound = MaxNbEnergyHrecord-1;
	     }
	    else
	     {
	      //check if search rec is before of after oldest record
	      lowerbound = 1;
	      upperbound = currec-1;
	      thepos = 2*sizeof(long) +
		  (upperbound-1)*(6*sizeof(int) + sizeof(long));
	      hisfile.seekg(thepos,ios::beg);
	      hisfile.read ((char*)unedate, 6*sizeof(int));
	      upperbdate = (double)unedate[0];
	      for (i=1;i<6;
			upperbdate = upperbdate*100+(double)unedate[i++]);
	      if (upperbdate < refdate)
		{
		 //refdate is later than most recent file
		 foundrec = upperbound;
		}
	       else
		{
		 thepos = 2*sizeof(long) +
			(lowerbound-1)*(6*sizeof(int) + sizeof(long));
		 hisfile.seekg(thepos,ios::beg);
		 hisfile.read ((char*)unedate, 6*sizeof(int));
		 lowerbdate = (double)unedate[0];
		 for (i=1;i<6;
			lowerbdate = lowerbdate*100+(double)unedate[i++]);
		 if (refdate < lowerbdate)
		   {
		    upperbound = nbrec;
		    lowerbound = currec+1;
		   }
		}//refdate earlier then most recent file
	     }//oldest file not the first and not the last
	 }//refdate later than oldest

       if (!foundrec)
	 {
	  //dichotomie entre lowerbound et upperbound
	  //lowerbound dat
	  thepos = 2*sizeof(long) +
			(lowerbound-1)*(6*sizeof(int) + sizeof(long));
	  hisfile.seekg(thepos,ios::beg);
	  hisfile.read ((char*)unedate, 6*sizeof(int));
	  lowerbdate = (double)unedate[0];
	  for (i=1;i<6;lowerbdate = lowerbdate*100+(double)unedate[i++]);
	  currec = (upperbound+lowerbound)/2;
	  while(currec != upperbound && currec != lowerbound)
	   {
	    //currec date
	    thepos = 2*sizeof(long) +
		     (currec-1)*(6*sizeof(int) + sizeof(long));
	    hisfile.seekg(thepos,ios::beg);
	    hisfile.read ((char*)unedate, 6*sizeof(int));
	    curdate = (double)unedate[0];
	    for (i=1;i<6;curdate = curdate*100+(double)unedate[i++]);
	    if (refdate >= lowerbdate && refdate <= curdate)
	      {
	       upperbound = currec;
	      }
	      else  lowerbound = currec;
	    currec = (upperbound+lowerbound)/2;
	   }//while currec
	  //use rec with closest date from refdate
	  //lowerbound dat
	  thepos = 2*sizeof(long) +
		   (lowerbound-1)*(6*sizeof(int) + sizeof(long));
	  hisfile.seekg(thepos,ios::beg);
	  hisfile.read ((char*)unedate, 6*sizeof(int));
	  lowerbdate = (double)unedate[0];
	  for (i=1;i<6;lowerbdate = lowerbdate*100+(double)unedate[i++]);
	  //upperbound date
	  thepos = 2*sizeof(long) +
		   (upperbound-1)*(6*sizeof(int) + sizeof(long));
	  hisfile.seekg(thepos,ios::beg);
	  hisfile.read ((char*)unedate, 6*sizeof(int));
	  upperbdate = (double)unedate[0];
	  for (i=1;i<6;upperbdate = upperbdate*100+(double)unedate[i++]);
	  if (fabs(refdate-lowerbdate) > fabs(refdate-upperbdate))
	    {
	     foundrec = upperbound;
	    }
	   else foundrec = lowerbound;
	 }//!foundrec
	if (foundrec)
	 {
	  *begRec = foundrec;
	  thepos = 2*sizeof(long) +
		   (foundrec-1)*(6*sizeof(int) + sizeof(long));
	  hisfile.seekg(thepos,ios::beg);
	  hisfile.read ((char*)begDate, 6*sizeof(int));
	 }

       //search endDate record
       foundrec = 0;
       lowerbound = 1;
       upperbound=nbrec;
       currec = oldest;
       //search with endDate
       //pointer to oldest record
       thepos = 2*sizeof(long) +
		(currec-1)*(6*sizeof(int) + sizeof(long));
       hisfile.seekg(thepos,ios::beg);
       hisfile.read ((char*)unedate, 6*sizeof(int));
       refdate = (double)endDate[0];
       for (i=1;i<6;refdate = refdate*100+(double)endDate[i++]);
       curdate = (double)unedate[0];
       for (i=1;i<6;curdate = curdate*100+(double)unedate[i++]);
       if (refdate <= curdate)
	 {
	  //older date than oldest record date
	  foundrec=currec;
	 }
	else
	 {
	  //check for bounds
	  if (currec == 1)
	    {
	     //oldest is the first
	     lowerbound = 2;
	    }
	    else if (currec >= MaxNbEnergyHrecord)
	     {
	      //oldest is the last
	      lowerbound = 1;
	      upperbound = MaxNbEnergyHrecord-1;
	     }
	    else
	     {
	      //check if search rec is before of after oldest record
	      lowerbound = 1;
	      upperbound = currec-1;
	      thepos = 2*sizeof(long) +
		  (upperbound-1)*(6*sizeof(int) + sizeof(long));
	      hisfile.seekg(thepos,ios::beg);
	      hisfile.read ((char*)unedate, 6*sizeof(int));
	      upperbdate = (double)unedate[0];
	      for (i=1;i<6;
			upperbdate = upperbdate*100+(double)unedate[i++]);
	      if (upperbdate < refdate)
		{
		 //refdate is later than most recent file
		 foundrec = upperbound;
		}
	       else
		{
		 thepos = 2*sizeof(long) +
			(lowerbound-1)*(6*sizeof(int) + sizeof(long));
		 hisfile.seekg(thepos,ios::beg);
		 hisfile.read ((char*)unedate, 6*sizeof(int));
		 lowerbdate = (double)unedate[0];
		 for (i=1;i<6;
			lowerbdate = lowerbdate*100+(double)unedate[i++]);
		 if (refdate < lowerbdate)
		   {
		    upperbound = nbrec;
		    lowerbound = currec+1;
		   }
		}//refdate earlier then most recent file
	     }//oldest file not the first and not the last
	 }//refdate later than oldest

       if (!foundrec)
	 {
	  //dichotomie entre lowerbound et upperbound
	  //lowerbound dat
	  thepos = 2*sizeof(long) +
			(lowerbound-1)*(6*sizeof(int) + sizeof(long));
	  hisfile.seekg(thepos,ios::beg);
	  hisfile.read ((char*)unedate, 6*sizeof(int));
	  lowerbdate = (double)unedate[0];
	  for (i=1;i<6;lowerbdate = lowerbdate*100+(double)unedate[i++]);
	  currec = (upperbound+lowerbound)/2;
	  while(currec != upperbound && currec != lowerbound)
	   {
	    //currec date
	    thepos = 2*sizeof(long) +
		     (currec-1)*(6*sizeof(int) + sizeof(long));
	    hisfile.seekg(thepos,ios::beg);
	    hisfile.read ((char*)unedate, 6*sizeof(int));
	    curdate = (double)unedate[0];
	    for (i=1;i<6;curdate = curdate*100+(double)unedate[i++]);
	    if (refdate >= lowerbdate && refdate <= curdate)
	      {
	       upperbound = currec;
	      }
	      else  lowerbound = currec;
	    currec = (upperbound+lowerbound)/2;
	   }//while currec
	  //use rec with closest date from refdate
	  //lowerbound dat
	  thepos = 2*sizeof(long) +
		   (lowerbound-1)*(6*sizeof(int) + sizeof(long));
	  hisfile.seekg(thepos,ios::beg);
	  hisfile.read ((char*)unedate, 6*sizeof(int));
	  lowerbdate = (double)unedate[0];
	  for (i=1;i<6;lowerbdate = lowerbdate*100+(double)unedate[i++]);
	  //upperbound date
	  thepos = 2*sizeof(long) +
		   (upperbound-1)*(6*sizeof(int) + sizeof(long));
	  hisfile.seekg(thepos,ios::beg);
	  hisfile.read ((char*)unedate, 6*sizeof(int));
	  upperbdate = (double)unedate[0];
	  for (i=1;i<6;upperbdate = upperbdate*100+(double)unedate[i++]);
	  if (fabs(refdate-lowerbdate) > fabs(refdate-upperbdate))
	    {
	     foundrec = upperbound;
	    }
	   else foundrec = lowerbound;
	 }//!foundrec
	if (foundrec)
	 {
	  *endRec = foundrec;
	  thepos = 2*sizeof(long) +
		   (foundrec-1)*(6*sizeof(int) + sizeof(long));
	  hisfile.seekg(thepos,ios::beg);
	  hisfile.read ((char*)endDate, 6*sizeof(int));
	 }
       //check number of record
       if (*endRec > *begRec)
	 {
	  toreturn = *endRec - *begRec + 1;
	 }
	else if (*begRec > *endRec)
	 {
	  toreturn = nbrec - *begRec + 1 + *endRec;
	 }
      }//hisfile
      hisfile.close();
      return toreturn;
    }
