
#ifndef __MTTRACK_H
#define __MTTRACK_H
int mttrack ();
int FixeCadenceTh();
int DBMeasProdRate(int);
int LoadChargingBatch();
int run_mttrack();
float calcMeasProdRate(long LastDisTime, float LastMeasProd, long actTime);
#endif
