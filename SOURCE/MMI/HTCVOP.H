// OWLCVT 12/23/96 14:35:11
//-------------------------------------------------------------------------
// HtCvOp.H
// This file contains function prototype for project FIRST
//
// Owner source file : HtCvOp.CPP
//
// Creation : JL COUSIN  date: 02/09/94
// Modifs:
//			 JFC14/03/97: le parametre passé à TestType est maintenant 1 int
//-------------------------------------------------------------------------

#ifndef __HTCVOP_H
#define __HTCVOP_H

/*#ifndef __EDITWND_H
#include "owl\compat.h"
#include <owl\editwnd.h>
#endif
#ifndef __FILEDIAL_H
#include <owl\filedial.h>
#endif*/
#ifndef __INPUTDIA_H
#include <owl\inputdia.h>
#endif
#ifndef __OWL_H
#include <owl\owlall.h>
#endif

#ifndef __IDWINDOW_H
#include <idwindow.h>
#endif

//---------------------------------------------------
//Class TConfigDlg: defines the Configuration
//Window for the Dialog CONFIG_1  
//see tact.rc
//
//---------------------------------------------------


_CLASSDEF(TConfigDlg3)
class _OWLCLASS TConfigDlg3 : public TDialog
{ public:
    // functions
    TConfigDlg3(TFrameWindow * parent, TResId name);
    //teste si la fenêtre peut être fermée
    //fermée la fenêtre sur condition de validité des paramètres saisis
    virtual BOOL CanClose();
    virtual void SetupWindow();
    //fermeture de la fenêtre de configuration
    void Close();
    void Up( UINT );
    void PgUp( UINT );
    void Down( UINT );
    void PgDown( UINT );
    void Home( UINT );
    void End( UINT );
    void B1( UINT );
    void B2( UINT );
    void B3( UINT );
    void B4( UINT );
    void B5( UINT );
    void B6( UINT );
    void B7( UINT );
    void B8( UINT );
    void B9( UINT );
	 void B10( UINT );
  private:
    //variables
    char *Buf; //buffer contenant une copie de l'ancienne structure
    int IndexList;//position dans la base de données
	 int NbRecord;//nombre d'enregistrements dans la base de données
	 //Functions
    //Transfert de la base de données vers la structure
    void LoadData(int Index);
    //fonction d'affichage des données à l'écran pour remise à jour
    // et déplacement
    void Affiche();
    //initialisation  des données dans la strcture
    void Init();
    //affiche les croix dans les cases à cocher
    void AfficheCroix();
    //cette fonction recherche l'emplacement des courbes selectionnées dans
    // la base de données
    int searchTheSelect();
    //cette fonction affecte la cadence au batch selectionné
    //si aucun batch n'a été selectionné
    //la fonction renvoie -1 sinon 1
    int AffectCadence(float cad);
    //cette fonction teste si le type de produit ThType
    //est celui du batch à l'enfournement
    //si oui elle renvoie 1 sinon -1
	 int TestType(int ThType);	// JFC14/03/97 le parametre est maintenant un int
    //cette fonction teste si le type de produit du batch correspond
    // à une courbe d'objectif selectionnée
	 int Test();
    int Select(int);
DECLARE_RESPONSE_TABLE( TConfigDlg3 );
};
#endif