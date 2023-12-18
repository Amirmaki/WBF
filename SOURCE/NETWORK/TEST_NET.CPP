#ifndef __STDIO_H
#include <stdio.h>
#endif
#ifndef __STRING_H
#include <string.h>
#endif
#ifndef __CONIO_H
#include <conio.h>
#endif

//#define ALONE	// use as a stand alone program

#ifdef ALONE
#define __OWNER_MAIN_H
#endif
#ifndef __NETWORK_H
#include "..\network\network.h"
#endif
#include "database.h"
#include "wrstatus.h"
#include "..\network\transfor.h"
#include "..\network\databloc.h"

#ifdef ALONE
//#define DEBUG
#define DISPLAY // display data
#endif
#define WITH_PLC

#ifdef CUSTOM // in NETWORK.H
static unsigned char addrPLC [] = { 0x08,0x00,0x06,0x01,0x02,0x02 };
static unsigned char addrREG [] = { 0x08,0x00,0x06,0x01,0x02,0x03 };
#endif

#ifdef ALONE
void main()
#else
int NetworkExchange()
#endif
{
	unsigned vcidrPLC=0,vcidwPLC=0,vcidrREG=0,vcidwREG=0;
	unsigned dwPLC[256],dwREG[256],dw[256],tab[256];
	int i=0, erreur=0;

	//Initialisation
	for(i=0;i<256;i++)
		{
		dwPLC[i]=0;
		dwREG[i]=0;
		dw[i]=0;
		}

#ifdef ALONE
	DBread_path();
#endif

	WriteStatus("COM: Start");

	//Communication with combustion PLC
#ifdef DISPLAY
	printf("OpenChannel with REG PLC\n");
#endif

#ifndef CUSTOM
	OpenChannel(vcidrREG,vcidwREG);
#else
	OpenChannel(vcidrREG,vcidwREG, addrREG, "OWS-PR01", "OWS-PW01");
#endif

#ifdef DISPLAY
	printf("receiveDB\n");
#endif
	receiveDB(vcidrREG,DBREG,OFFSETREG,NBREG,dwREG);
#ifdef DISPLAY
	printf("CloseChannel\n");
#endif
	CloseChannel(vcidrREG,vcidwREG);

	for (i=0; i<NBREG; i++)
		{
#ifdef DISPLAY
		printf("DW%d = %4d ", OFFSETREG+i, dwREG[i]);
		if (i && i%5 == 0) printf("\n");
#endif
		dw[RECUP_TEMP+i] = dwREG[i];
		}

//Communication with MVT PLC
#ifndef DEBUG
#ifdef WITH_PLC
#ifdef DISPLAY
	printf("\nOpenChannel with MVT PLC\n");
#endif
#ifndef CUSTOM
	OpenChannel(vcidrPLC,vcidwPLC);
#else
	OpenChannel(vcidrPLC,vcidwPLC, addrPLC, "OWS-PR01", "OWS-PW01");
#endif
#ifdef DISPLAY
	printf("receiveDB\n");
#endif
	receiveDB(vcidrPLC,DBPLC,OFFSETMVT,NBPLC,dwPLC);
#ifdef DISPLAY
	printf("CloseChannel\n");
#endif
	CloseChannel(vcidrPLC,vcidwPLC);

#ifdef DISPLAY
	for (i=0; i<NBPLC; i++)
		{
		if (i < PO_NUMBER || i >= STEEL_CODE)
			printf("DW%d = %d\t", i, (int)dwPLC[i]);
		else
			printf("DW%d = %c%c ", i, (dwPLC[i] && 0xFF00) >> 8,dwPLC[i] && 0xFF);
		}
#endif
	//if DATA_READY
	if (dwPLC[1]==1)
		{
		//copy dwPLC in dw:
		for (i=0; i<NBPLC; i++)
			dw[i] = dwPLC[i];

		echangeBdD((int *)dw, 0);

		//Send data acknowledge to MVT PLC.
		for (i=0;i<NBPLC;i++)
			dwPLC[i] = dw[i];
#ifdef DISPLAY
		printf("OpenChannel with MVT PLC\n");
#endif

#ifdef CUSTOM
		OpenChannel(vcidrPLC,vcidwPLC, addrPLC, "OWS-PR01", "OWS-PW01");
#endif

#ifdef DISPLAY
		printf("SendDB ACK\n");
#endif
		sendDB(vcidwPLC,DBPLC,OFFSETMVT,1,&dwPLC[0]);
#ifdef DISPLAY
		printf("CloseChannel\n");
#endif
		CloseChannel(vcidrPLC,vcidwPLC);
/*
		//Send SP to combustion PLC
#ifdef DISPLAY
		printf("OpenChannel with REG PLC\n");
#endif

#ifndef CUSTOM
		OpenChannel(vcidrREG,vcidwREG);
#else
		OpenChannel(vcidrREG,vcidwREG, addrREG, "OWS-PR01", "OWS-PW01");
#endif

#ifdef DISPLAY
		printf("SendSP\n");
#endif
		for (i=0; i<NBZONES_T; i++)
			{
			dwREG[i] = dw[TEMP_SP_T+i];
			sendDB(vcidwREG,DBREG,OFFSETSP+i,1,&dwREG[i]);
#ifdef DISPLAY
			printf("DW%d = %d\n", OFFSETSP+i, dwREG[i]);
#endif
			}

#ifdef DISPLAY
		printf("CloseChannel\n");
#endif
		CloseChannel(vcidrREG,vcidwREG);
*/		}
#endif
//dw[STEP_LENGTH]=380;
echangeBdD((int *)dw, 1);

#ifdef DISPLAY
	printf("\nOpenChannel with REG PLC\n");
#endif

#ifndef CUSTOM
	OpenChannel(vcidrREG,vcidwREG);
#else
	OpenChannel(vcidrREG,vcidwREG, addrREG, "OWS-PR01", "OWS-PW01");
#endif

	//Send SP to combustion PLC
#ifdef DISPLAY
	printf("SendSP\n");
#endif
	for (i=0; i<NBZONES_T; i++)
		{
		dwREG[i] = dw[TEMP_SP_T+i];
		sendDB(vcidwREG,DBREG,OFFSETSP+i,1,&dwREG[i]);
#ifdef DISPLAY
		printf("DW%d = %d\n", OFFSETSP+i, dwREG[i]);
#endif
		}

	// send ACK to tell COM is running
	dwREG[0] = 1;
	sendDB(vcidwREG,DBREG,OFFSETACK,1,&dwREG[0]);
#ifdef DISPLAY
	printf("Send ACK\nDW%d = %d\n", OFFSETACK, dwREG[0]);
	printf("CloseChannel\n");
#endif
	CloseChannel(vcidrREG,vcidwREG);

#else //Simulation to test echangeDB
	FILE *fp = fopen("read_plc.txt","rt");
	for (int j=0; j <44; j++)
		fscanf(fp,"%d",&tab[j]);
	fclose(fp);

	//Echange avec la base de donn‚es.
	echangeBdD((int *)tab);

	fp=fopen("writ_plc.txt","w+");
	for (j=0;j < 44;j++)
		fprintf(fp,"%d\n",tab[j]);
	fclose(fp);
#endif

	WriteStatus("COM: Complete");

#ifndef ALONE
	return erreur;
#endif
}
// end main
