//STD/0001
//-------------------------------------------------------------------------
// FURNHIS.CPP
// This file contains data and function prototype for furnace
// history in project TACT
//
// Owning include file : TEMPHIS.H
//
// Creation : JF.Chanlon 19/09/96
// Modif    :
// Modif    :
//-------------------------------------------------------------------------


//include files
//-------------------------------------------------------------------------
#define _OWNER_TEMPHIS_H
#define __OWNER_MAIN_H

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
void main(void)
// Update history with furnace values
{ // Read the data
	int i;
	int ladate[6];
	int rect;
	int ztemp[maxNbZone];
	int zspt[maxNbZone];
	int zqg[maxNbZone];
	float zqgf[maxNbZone];
	int zqa[maxNbZone];
	float zqaf[maxNbZone];
	char nbzone;
	int nbz;
	char filename[MAXPATH];


	if (DBread_path())
		{
		printf("\n MTFURHIS ERROR: Check the file TACT.CFG \n");
		exit(1);
		}
	WriteStatus("MTFURHIS: Start");
	sprintf(filename,"%s%s",DB_Path_hisfour,"FURNHIST.HIS");

	int err = DBopen(DB_CfgFour_Item,0,"");
	if (err == 0)
		{
		err+= LireDB(DB_CfgFour_Item,0,DB_Four_NbZoneSup, (char *)&nbzone,
						sizeof(char));
		err+= DBclose(DB_CfgFour_Item);
		}
	else
		{
		printf("\n MTFURHIS ERROR: Cannot open CFGFOUR.DTA \n");
		exit(1);
		}

	err = DBopen(DB_CtrlFour_Item,0,"");
	if (err == 0)
		{
		err+= LireDB(DB_CtrlFour_Item,0,DB_CFour_RecTemp, (char *)&rect,
						sizeof(int));
		err+= LireDB(DB_CtrlFour_Item,0,DB_CFour_ZTempSup, (char *)ztemp,
						(int)nbzone * sizeof(int));
		err+= LireDB(DB_CtrlFour_Item,0,DB_CFour_SPSup, (char *)zspt,
						(int)nbzone * sizeof(int));
		err+= LireDB(DB_CtrlFour_Item,0,DB_CFour_QGSup, (char *)zqgf,
						(int)nbzone * sizeof(float));
		err+= LireDB(DB_CtrlFour_Item,0,DB_CFour_QASup, (char *)zqaf,
						(int)nbzone * sizeof(float));
		err+= DBclose(DB_CtrlFour_Item);
		}
	else
		{
		printf("\n MTFURHIS ERROR: Cannot open CTRLFOUR.DTA \n");
		exit(1);
		}

	nbz = (int) nbzone;
	rect -= 273;

	for (i = 0; i < nbz; i++)
		{
		ztemp[i] -= 273;
		zspt[i] -= 273;
		zqg[i] = (int) zqgf[i];
		zqa[i] = (int) zqaf[i];
		}

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
			long thepos = 2*sizeof(long) + sizeof(int)
							+ (newone-1)*((7 + 4*nbz )*sizeof(int));
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
			hisfile.write( (char*)&nbz, sizeof(int) );
			}
		else
			{
			printf("\n MTFURHIS ERROR: Cannot create %s \n", filename);
			exit(1);
			}
		} // file creation
	// Update the file
	if (hisfile)
		{
		hisfile.write( (char*)ladate,6*sizeof(int) );
		hisfile.write( (char*)&rect,sizeof(int) );
		hisfile.write( (char*)ztemp,nbz*sizeof(int) );
		hisfile.write( (char*)zspt,nbz*sizeof(int) );
		hisfile.write( (char*)zqg,nbz*sizeof(int) );
		hisfile.write( (char*)zqa,nbz*sizeof(int) );
		}
	hisfile.close();
	WriteStatus("MTFURHIS: Complete");
}

