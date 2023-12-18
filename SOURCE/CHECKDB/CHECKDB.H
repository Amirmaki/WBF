#define NBDELAY 5
char Id[NBDELAY];
char Debut[NBDELAY][6];
char DureeInit[NBDELAY][3];
long Duree[NBDELAY];
int NbRecord;

void LoadDelayData();
int affiche(int *Data1, int *Data2);
void Delays();
int TransformeDate(char *Data, int *tab);
int TestArret();
void TransformeSecEnJHMin(long NbSec, int *tab);
void LireTProd();
void LireFour();
void LireRefCV();
void LireCad();
void LirePLC();
void LireBatch();
void LireLog();
void LireSim();