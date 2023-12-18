#ifdef __OWNER_DATAHIS_H
typedef int tableau[NBZONE];
typedef int tableau1[6];
tableau *ZTemp;
tableau *ZSP;
tableau *ZQG;
tableau *ZQA;
tableau1 *Date;
int *Recup;
int *TMean, *TMeas, *DecalT, *Targ, *ResT, *Ox, *Dec;
int BegDateP[6], EndDateP[6];
#else
extern int *TMean, *TMeas, *DecalT, *Targ, *ResT, *Ox, *Dec;
typedef int tableau[NBZONE];
typedef int tableau1[6];
extern tableau1 *Date;
extern tableau *ZTemp;
extern tableau *ZSP;
extern tableau *ZQG;
extern tableau *ZQA;
extern tableau1 *Date;
extern int *Recup;
#endif
