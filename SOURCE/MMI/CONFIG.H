#ifndef __CONFIG_H
#define __CONFIG_H
#endif

#include "appelhs.h"

#ifndef __STRING_H
#include <string.h>
#endif
#ifndef __CONIO_H
#include <conio.h>
#endif
#ifndef __STDIO_H
#include <stdio.h>
#endif
#ifndef __GRAPHICS_H
#include <graphics.h>
#endif
#ifndef __DOS_H
#include <dos.h>
#endif
//#define MK_FP(seg,ofs) ((void far *)(((unsigned long)(seg) << 16)|(unsigned)(ofs)))

#define CLI() (_disable())
#define STI() (_enable())

#define NBCHAR				6
#define NBZONE				5
#define MAXALARM		  50

#define QUIT   			-1
#define MAIN_MENU       0
#define CONFIG_PASSWORD 1
#define CONFIG_MENU     2
#define REHEAT_CONTROL  3
#define PROD_HIS        4
#define FURNACE_HIS     5
#define DEF_HIS         6
#define ZHIS_CONFIG     7
#define FURNACE_CONFIG  8
#define MODEL_CONFIG    9
#define PRODUCTS_CONFIG 10
#define HEAT_CONFIG     11
#define DELAY           12
#define PACING          13
#define HEAT_SELECT     14
#define PROD_SEL        15
#define HELP_SCREEN     16

//fenˆtres de dialogue
#define FURNACE_CONFIRM	50
#define MODEL_CONFIRM	51
#define PRODUCT_CONFIRM	52
#define HEAT_CONFIRM		53
#define ERROR_WINDOW		54
#define DELAY_CONFIRM	55
#define PACING_CONFIRM  56

// messages d'erreurs
#define READING_ERROR	100
#define OPENING_ERROR	101
#define UPDATING_ERROR	102
#define DELETE_ERROR		103
#define SAVE_ERROR		104
#define SAME_ERROR      105
#define UNVALID_ERROR	106
#define RECORDING_ERROR 107
#define UNDEFINED_ERROR 108
#define LOADING_ERROR	109
#define NBCURVE_ERROR	110

#define INITIAL 1
#define NORMAL 0

//---------------------------------Vues Graphiques uniquement----------------
#define Right					1     //d‚placement de la souris
#define Left					2
#define Background_Color	9     // Couleur de fond des vues graphiques(gris)
#define Cursor_Top			Top-14
#define Cursor_Bottom		Top-1
//---------------------------------------------------------------------------
struct TypeModelCopy
{ char HCoef1[NBCHAR];
  char HCoef2[NBCHAR];
  char HCoef3[NBCHAR];
  char Epsif1[NBCHAR];
  char Epsif2[NBCHAR];
  char EpsifMin[NBCHAR];
  char EpsifMax[NBCHAR];
  char KSurf[NBCHAR];
  char KTime[NBCHAR];
  char KEpsiSup[NBZONE][NBCHAR];
  char MeasPer[NBCHAR];
  char CalcPer[NBCHAR];
  char SPCalcPer[NBCHAR];
  char PHisPer[NBCHAR];
  char ZHisPer[NBCHAR];
  char SPEnable;
  char ZHisEnable;
  char PHisEnable;
  char PaceEnable;
  char PLCConnect;
  char CTLConnect;
  char Alarm[MAXALARM];
};

#ifdef __OWNER_CONFIG_H
int vue;                 // gestion des vues
int Vue_Precedente;      //
int *init;
int error;
int Temp;
int Valid;
TypeModelCopy ModelCopy;

#else
extern vue;
extern Vue_Precedente;
extern int *init;
extern int error;
extern int Temp;
extern int Valid;
extern TypeModelCopy ModelCopy;
#endif

void Main_Menu();
void Config_Menu();
void Config_Password();
void Prod_His();
void Furnace_His();
void ZHis_Config();
void Def_His();
void Furnace_Config();
void Model_Config();
void Products_Config();
void Heat_Config();
void Pacing_Config();
void Pacing_Confirm();
void Reheat_Control();
void Delay();
void Products_Select();
void Heat_Select();
void Confirm();
void Sauvegarde(char *Touche);
void Abandon(char *Touche);
void Sortie_Ecran(char *Touche);
void Teste_Buffer(char *Touche);
void File_Error(int error, char *FileName);
void Window_Error();
void Confirm_OverWrite();
void clearKbBuffer(void);
void VGInitScreen();
void Help_Screen();
void Affiche_Aide();