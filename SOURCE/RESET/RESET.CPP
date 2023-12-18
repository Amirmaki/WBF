
#define __OWNER_MAIN_H
// Fichiers include système
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dbinit.h"
#include "database.h"
// Fichiers include application
#include "globdat.h"
#ifndef __WINSTRUC_H
#include "winstruc.h"
#endif
//#include "batchfct.h"



void main (int argc, char *argv[])
{
	// RAZ complete ou partielle
	char choix = 'N';

	if (argc < 2)
		{
		printf("Specify an application name");
		exit(1);
		}

	printf("\n Do you want to clear the furnace (Y/N) ? ");
	scanf("%c",&choix);

	int res = DBread_path();
	if (res)
		printf("\n BAD FILE TACT.CFG \n");
	else
		{
		if (choix=='Y' || choix=='y')
			res = DBinit(argv[1]);
		else
			res = CFGinit(argv[1]);
		if (res) printf("\n DATABASE INITIALISATION ERROR = %i \n",res);
		}
}
