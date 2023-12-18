#ifndef __NETWORK_H
#define __NETWORK_H

#define CUSTOM

/*-----    prototypes of local functions   -----*/
#ifndef CUSTOM
void OpenChannel(unsigned &,unsigned &);
#else
void OpenChannel(unsigned &,unsigned &, unsigned char[6],char *, char *);
#endif

void CloseChannel(unsigned &vcidr,unsigned &vcidw);
//int sendDB(int vcid, unsigned ch1, unsigned ch2,unsigned ch3,int dblen,unsigned dwx[256]);
int sendDB(int vcid, unsigned ch1, unsigned ch2, unsigned dblen,unsigned dwx[256]);
int receiveDB(int vcid,unsigned dbnr,unsigned dbanf,unsigned dblen,unsigned Result[256]);
/*------------------------------------------------------------------------*/
#endif