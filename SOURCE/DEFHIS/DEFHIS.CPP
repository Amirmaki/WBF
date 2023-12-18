//STD/0001
//-------------------------------------------------------------------------
// DEFHIS.CPP
// This file contains data and function prototype for discharging
// history in project TACT
//
// Owning include file : TEMPHIS.H
//
// Creation : JF.Chanlon 15/09/96
// Modif    :
// Modif    :
//-------------------------------------------------------------------------


//include files
//-------------------------------------------------------------------------
#define _OWNER_TEMPHIS_H
#define __OWNER_DEFHIS_H

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

//#define __OWNER_HISZWIND
//#define __OWNER_HISPWIND


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
#include "winstruc.h"
#define __OWNER_DATAHIS_H
#include "datahis.h"
#include "wrstatus.h"

//--------------------------------------------------------------------------
int run_defhis(void)
// Update history with discharged product values
{ // Read the data
	int ladate[6];
	float temps[4];
	int pval[7];
	long resTime;
	long oxydec[2];
	float oxydecf[2];
	int target[9] = {0,0,0,0,0,0,0,0,0};
	char nbzone;
	int theTarget;
	char filename[MAXPATH];

	if (DBread_path())
	{
		printf("\n MTDEFHIS ERROR: Check the file TACT.CFG \n");
		return(-1);
	}

	WriteStatus("MTDEFHIS: Start");
	sprintf(filename,"%s%s",DB_Path_hisfour,DEFHIST);

	int err = DBopen(DB_CfgFour_Item,0,"");
	if (err == 0)
	{
		err+= LireDB(DB_CfgFour_Item,0,DB_Four_NbZoneSup, (char *)&nbzone,
						sizeof(char));
		err+= DBclose(DB_CfgFour_Item);
	}
	else
	{
		printf("\n MTDEFHIS ERROR: Cannot open CFGFOUR.DTA \n");
		exit(1);
	}

	err = DBopen(DB_DisProd_Item,0,"");
	if (err == 0)
	{
		err+= LireDB(DB_DisProd_Item,0,DB_DisProd_TSurfTemp, (char *)&temps[0],
						sizeof(float));
		err+= LireDB(DB_DisProd_Item,0,DB_DisProd_MeanTemp, (char *)&temps[1],
						sizeof(float));
		err+= LireDB(DB_DisProd_Item,0,DB_DisProd_CoreTemp, (char *)&temps[2],
						sizeof(float));
		err+= LireDB(DB_DisProd_Item,0,DB_DisProd_BSurfTemp, (char *)&temps[3],
						sizeof(float));
		err+= LireDB(DB_DisProd_Item,0,DB_DisProd_ResTime, (char *)&resTime,
						sizeof(long));
		err+= LireDB(DB_DisProd_Item,0,DB_DisProd_OxyDepth, (char *)&oxydecf[0],
						sizeof(float));
		err+= LireDB(DB_DisProd_Item,0,DB_DisProd_DecDepth, (char *)&oxydecf[1],
						sizeof(float));
		err+= LireDB(DB_DisProd_Item,0,DB_DisProd_Target, (char *)target,
						(int)nbzone * sizeof(int));
		err+= DBclose(DB_DisProd_Item);
	}
	else
	{
		printf("\n MTDEFHIS ERROR: Cannot open DISPROD.DTA \n");
		return(-1);
	}

	pval[0] = (int) temps[0] - 273;
	pval[1] = (int) temps[1] - 273;
	pval[2] = (int) temps[2] - 273;
	pval[3] = (int) temps[3] - 273;
	pval[4] = (int) (resTime / 60);

//  if (exitf > 0) pval[5] = (int)exitf;
	pval[5] = 0;
//  if (dect > 0) pval[6] = (int)dect;
	pval[6] = 0;
	oxydec[0] = (long) oxydecf[0];
	oxydec[1] = (long) oxydecf[1];
	theTarget = target[(int)(nbzone)-1];
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
	if ( !findfirst(filename,&fb,0) )
	{ // the file exists, read the header
		hisfile.open(filename,ios::in|ios::binary);
		if (hisfile)
		{
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
				if (oldest >= MaxNbPTHrecord) oldest = 1;
				else oldest++;
			}
			hisfile.close();
		} // hisfile
		hisfile.open(filename,ios::out|ios::binary|ios::ate);
		if (hisfile)
		{
			hisfile.seekp(0,ios::beg);
			hisfile.write( (char*)&nbRecord,sizeof(long) );
			hisfile.write( (char*)&oldest,sizeof(long) );
			// position the file on record to write
			long thepos = 2*sizeof(long)
							+ (newone-1)*(14*sizeof(int)+2*sizeof(long));
			hisfile.seekp(thepos,ios::beg);
			if (!hisfile) hisfile.clear(0);
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
		}
		else
		{
			printf("\n MTDEFHIS ERROR: Cannot create %s \n", filename);
			return(-1);
		}
	} // file creation
	// Update the file
	if (hisfile)
	{
		hisfile.write( (char*)ladate,6*sizeof(int) );
		hisfile.write( (char*)pval,7*sizeof(int) );
		hisfile.write( (char*)oxydec,2*sizeof(long) );
		hisfile.write( (char*)(&theTarget),sizeof(int) );
	}
	hisfile.close();
	WriteStatus("MTDEFHIS: Complete");
	return(0);
}

