/////////////////////////////////////////////////////////////////////////////
//
//                               TDELAY.CPP
//
//      Tache de mise … jour des arrets dans la base de donnee DELAY.DTA
//
/////////////////////////////////////////////////////////////////////////////
//#define ALONE

#ifdef ALONE
#define __OWNER_MAIN_H
#endif

#define __DELAY_H
#include <stdio.h>
#include <stdlib.h>
#include "database.h"
#include "delay.h"
#include "wrstatus.h"

#ifdef ALONE
int main()
#else
int run_delay()
#endif
{
#ifdef ALONE
	DBread_path();
#endif

	WriteStatus("TDELAY:Start");
	int status = CalculDurRest();
	WriteStatus("TDELAY:Complete");
	return(0);
}
