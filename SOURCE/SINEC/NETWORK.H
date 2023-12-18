#ifndef __NETWORK_H
#define __NETWORK_H



/*-----    prototypes of local functions   -----*/


void OpenChanel(unsigned &,unsigned &);
void CloseChanel(unsigned &vcidr,unsigned &vcidw);
//int sendDB(int vcid, unsigned ch1, unsigned ch2,unsigned ch3,int dblen,unsigned dwx[256]);
int sendDB(int vcid, unsigned ch1, unsigned ch2, int dblen,unsigned dwx[256]);
int receiveDB(int vcid,int dbnr,int dbanf,int dblen,unsigned Result[256]);
/*------------------------------------------------------------------------*/
#endif