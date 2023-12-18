//-------------------------------------------------------------------------
// GLOBVIEW.H
// This file contains global data and function prototype for project TACT
// displays
//
// Owner source file : GLOBVIEW.CPP
//
// Creation : C.Martinet 18/12/92
//-------------------------------------------------------------------------

#ifndef __GLOBVIEW_H
#define __GLOBVIEW_H

//-------------------------------------------------------------------------
#ifdef _OWNER_GLOBVIEW_H

int VGDispViewNb;       //displayed view number
int VGNextViewNb;       //next view to display
int VGChangeViewEnable; //enable changing of display if 1, disable if 0
int VGDispMode;         //present screen mode (texte=1,graphic=0)
int VGPageVisu;         //displayed graphic page number (0/1)
int VGPageActive;       //active graphic page number (0/1)
int VGDispHelp;         //display help (0/1)

#else

extern int VGDispViewNb;       //displayed view number
extern int VGNextViewNb;       //next view to display
extern int VGChangeViewEnable; //enable changing of display if 1, disable if 0
extern int VGDispMode;         //present screen mode (texte=0,graphic=1)
extern int VGPageVisu;         //displayed graphic page number (0/1)
extern int VGPageActive;       //active graphic page number (0/1)
extern int VGDispHelp;         //display help (0/1)

#endif

//------------------------------------------------------------------------
//function prototypes

//initialisation and call to all VnnInit()
void VGInit();

//clear text screen and init graphic driver
void VGInitScreen();

// restore 80 column text mode
void VGRestoreText();

//change display
void VGChangeDisp();

//change help display on/off
void VGChangeHelpMode();

//display the hour on screen, parameter is 1 when called from an
//interrupt routine, else 0
void VGDispHour(int from_int = 0);

#endif
//end GLOBVIEW.H
