// OWLCVT 12/23/96 14:35:11
//-------------------------------------------------------------------------
// HtCvOp.H
// This file contains function prototype for project FIRST
//
// Owner source file : HtCvOp.CPP
//
// Creation : JL COUSIN  date: 02/09/94
// Modifs:
//			 JFC14/03/97: le parametre pass� � TestType est maintenant 1 int
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
    //teste si la fen�tre peut �tre ferm�e
    //ferm�e la fen�tre sur condition de validit� des param�tres saisis
    virtual BOOL CanClose();
    virtual void SetupWindow();
    //fermeture de la fen�tre de configuration
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
    int IndexList;//position dans la base de donn�es
	 int NbRecord;//nombre d'enregistrements dans la base de donn�es
	 //Functions
    //Transfert de la base de donn�es vers la structure
    void LoadData(int Index);
    //fonction d'affichage des donn�es � l'�cran pour remise � jour
    // et d�placement
    void Affiche();
    //initialisation  des donn�es dans la strcture
    void Init();
    //affiche les croix dans les cases � cocher
    void AfficheCroix();
    //cette fonction recherche l'emplacement des courbes selectionn�es dans
    // la base de donn�es
    int searchTheSelect();
    //cette fonction affecte la cadence au batch selectionn�
    //si aucun batch n'a �t� selectionn�
    //la fonction renvoie -1 sinon 1
    int AffectCadence(float cad);
    //cette fonction teste si le type de produit ThType
    //est celui du batch � l'enfournement
    //si oui elle renvoie 1 sinon -1
	 int TestType(int ThType);	// JFC14/03/97 le parametre est maintenant un int
    //cette fonction teste si le type de produit du batch correspond
    // � une courbe d'objectif selectionn�e
	 int Test();
    int Select(int);
DECLARE_RESPONSE_TABLE( TConfigDlg3 );
};
#endif