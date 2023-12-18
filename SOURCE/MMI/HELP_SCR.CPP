 #include "config.h"
 #include "help.h"

void Help_Screen()
{
	if ( *init ==INITIAL)
	{
		*init = NORMAL;

		APPELHS ("Utilise,help_scr.AID");

		APPELHS ("Fusionne,logo_cc.img,1,2 ");
		APPELHS ("Fusionne,hlp_titl.img,1,36 ");
		APPELHS ("Fusionne,hlp_scrn.img,3,5");
		Affiche_Aide();
	}
	APPELHS ("Sortie,TF0=V");

	APPELHS ("Ecran,Saisie/nonblocant");

	if(!strcmp(HsTouche,"TF0"))
		Sortie_Ecran(HsTouche);
}

void Affiche_Aide()
{
		APPELHS ("Option,Couleur,A=112");
		APPELHS("Affiche,TITLEKEY=KEY");
		APPELHS("Affiche,TITLEFUNCT=FUNCTION");
		APPELHS ("Option,Couleur,A=240");

	for (int i = 0; i < 10;i++)
	{
		APPELHS("Affiche,HELPDIALOG,%d=%s",i+1,Help_Dialog[Vue_Precedente][i]);
		APPELHS("Affiche,KEY,%d=F%d",i+1,i+1);
	}
}