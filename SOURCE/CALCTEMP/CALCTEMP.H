
#ifndef __CALCTEMP_H
#define __CALCTEMP_H
void calctemp (float);

#ifdef DEBUG
float CalcPer;
#else
extern float CalcPer;
#endif
#endif