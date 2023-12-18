/*--------------------------------------------------------------------------*
 *                                                                          *
 *          Call_INA.H                                                      *
 *          ==========                                                      *
 *                     Prototype-Declarations of Call_INA-Functions         *
*/
#ifndef __CALL_INA_H
#define __CALL_INA_H

void ina_send	    (int vcid, char sdata[], int dtlen);

void ina_recv       (int vcid, char rdata[], int dtlen);

int ina_open        (void);

void ina_close      (int vcid);

int  ina_connect         (int vcid, unsigned char addr[6], unsigned char tsap[8]);

void ina_listen     (int vcid, char addr[6], char tsap[8]);

int ina_test        (void);
#endif
