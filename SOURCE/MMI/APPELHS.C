/* Interface HYPER SCREEN avec le C---r‚vision Novembre 1991                  */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#define  LG_CDE_HS5 255

/* variables communes */
int    HsEntier;
double HsReelD;
float  HsReel;
char   HsChaine[100];
int    HsMenu;
char   HsNom[11];
int    HsIndice;
char   HsTouche[4];
int    HsCr;

/* variable locale */
char HScommande[LG_CDE_HS5];

//extern "C" int far HSCALL(char far *); //pour mode C++ uniquement
extern int far HSCALL(char far *);
int HS2(int);
int HS3(char *);

int HS2(int n)
{
    int i;
    i=n ;
    while( (HScommande[i]!=1) && (HScommande[i]!=13) && (i<108) ) i++;
    HScommande[i]='\0';

    switch(HScommande[1])
            {
            case 'C' :
		    strncpy(HsNom,&HScommande[n],10);
                    HsNom[10]='\0';
                    HsIndice=atoi(&HScommande[n+11]);
                    break;

            case 'M' :
                    HsMenu=atoi(&HScommande[n]);
                    strcpy(HsChaine,&HScommande[n]);
                    break;

            case 'E' :
            HsEntier=atoi(&HScommande[n]);
		    HsReel= (float) HsEntier;
                    strcpy(HsChaine,&HScommande[n]);
                    break;

            case 'R' :
            HsReel = (float) atof(&HScommande[n]);
            HsReelD = atof(&HScommande[n]);
            if ((HsReel<(float)32767.0) && (HsReel>(float)-32768.0) )
		       {
               HsEntier= (int) HsReel;
		       }
                    strcpy(HsChaine,&HScommande[n]);
                    break;

            case 'A' :
                    strcpy(HsChaine,&HScommande[n]);
                    break;

            };

            return(0);

}

int HS3(char *commande)
{

strnset(HScommande,0,LG_CDE_HS5); 
strcpy(HScommande,commande); 
HScommande[strlen(HScommande)]=1;  
HSCALL(HScommande); 

HsEntier    = 0;
HsReel      = (float) 0.0;
HsReelD     = (double) 0.0;
HsChaine[0] = '\0';
HsMenu      = 0;
HsNom[0]    = '\0';
HsIndice    = 0;
HsTouche[0] = '\0';
HsCr        = 0;

switch(HScommande[0]) 
	{
        case '0' :
        	HS2(2);
                break;

	case '1' :
            strncpy(HsTouche,&HScommande[2],3);
                HsTouche[3]='\0';
        	HS2(6);
                break;

	case '2' :
            HScommande[3]='\0';
            HsCr=atoi(&HScommande[2]);
        	break;


	default  :
	    printf("\nAFFICHE.COM n'est pas en m‚moire");
	    exit(1);
	    break;
	};
        return(0);
}

void APPELHS(char *fmt, ...)
{
  char command[LG_CDE_HS5];
  va_list arg_ptr;
  va_start(arg_ptr, fmt);

  vsprintf(command, fmt, arg_ptr);
  va_end(arg_ptr);
  HS3(command);
}
