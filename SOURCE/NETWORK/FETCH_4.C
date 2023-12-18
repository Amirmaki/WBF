/*--------------------------------------------------------------------------*/
/*                       GET DB BY USING A FETCH                            */
/*                      =========================                           */
/*                                                                          */
/*-------------------------------------------------- TCHALIKIAN DTC4/Paris -*/
/*------------------------------------------------ Scheubeck E851 K'he -----*/
/*                                                                          */
/*   Programm:       FETCH_4                                                */
/*   Purpose :       opens 4 connections to the SIMATIC and reads           */
/*                   DB10. The conents of the DB is displayed.              */
/*                   The first 0 terminates the display output.             */
/*                   The ethernet address is 08 00 06 01 0E EE for          */
/*                   all connections.                                       */
/*                   The remote TSAP ID are:                                */
/*                   1.:    READREAD                                        */
/*                   2.:    READREA1                                        */
/*                   3.:    READREA2                                        */
/*                   4.:    READREA3                                        */
/*                   These TSAP-ID are written in the CP535 with COM535     */
/*                   Refer to dokumentation chapter 1 for more info.        */
/*   Input:                none                                             */
/*   compiler notes: This code is written for TURBO C V2.0.                 */
/*                   but can also be compiled with MS-C 6.0                 */
/*   project info's: The INCLUDE FILE S5.H will be called by FETCH_4.       */
/*                   This modules also needs CALL_INA.C to be linked        */
/*                   with. CALL_INA needs RB.H as include file              */
/*                                                                          */
/*   further information:                                                   */
/*                   Refer to INA 960 Release 3.0 Programmer's              */
/*                   Reference Manual chapter 2.3 (req_block),              */
/*                   PC Link2 Software Developer's Manual chapter 10.3      */
/*                   (interface specification, using INT 5BH).              */
/*--------------------------------------------------------------------------*/

/* CHANGES MADE :
+--------------------------+---------------------------------------+----------+
|   who                    | why                                   | date     |
+--------------------------+---------------------------------------+----------+
| Scheubeck E851 K'he      | make a valid DS in function soft_int  | 08.02.89 |
|                          | reorganize structure declaration      |          |
+--------------------------+---------------------------------------+----------+
| Scheubeck                | change procedure soft_int, change     | 08.03.89 |
|                          | addr[] to unsigned for correct output |          |
|                          | copyright remark                      |          |
+--------------------------+---------------------------------------+----------+
| Fromm-Ayass              | adaption for MS-C 6.0                 |          |
+--------------------------+---------------------------------------+----------+
last changes made: 07.09.89 by Arno because of cosmetical repairment
*/
#include <stdio.h>
#include "call_ina.h"
#include "s5.h"


#define MAXCONN 4

#pragma check_stack( off )

/*------------------------------------------------------------------------*/
/*----- local function prototypes -----*/

void h1visu( int vcid_a [] );



/*------------------------------------------------------------------------*/



main()
{
    char *simatic_tsap[] =  {"READREAD","READREA1","READREA2","READREA3"};
    char unsigned simatic_addr[][6] =  {{ 0x08,0x00,0x06,0x01,0x0e,0xee },
                                        { 0x08,0x00,0x06,0x01,0x0e,0xee },
                                        { 0x08,0x00,0x06,0x01,0x0e,0xee },
                                        { 0x08,0x00,0x06,0x01,0x0e,0xee }};
    int i,j, vcid_array[MAXCONN], err;

    printf("\33[2J\t\t\tSICOMP PC <-> SIMATIC S5     FETCH_4\n");
    printf ("\t   ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออป\n");
    printf ("\t   บ    COPYRIGHT (C) SIEMENS AG 1989 ALL RIGHTS RESERVED    บ\n");
    printf ("\t   ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ\n");
    printf("\33[8;5HConnection   \33[8;27HCOUNTER \33[8;40HRECEIVES\33[9;1H");
    for (i=0; i<80; i++)
        putchar('ฤ');

    for (i=0; i<MAXCONN; i++)
    {
        vcid_array[i] = ina_open();
        err = ina_connect(vcid_array[i], simatic_addr[i], simatic_tsap[i]);
        if (err == 16 || err == 22) /* TIMEOUT */
        {   ina_close(vcid_array[i]);
            vcid_array[i] = 0;
        }
        else
        {   printf("\33[%d;3H", 10+i*2);
            for (j = 0; j<6; j++)
                printf(" %02X", simatic_addr[i][j]);
        }
    }

    h1visu(vcid_array);

    for(i=0; i<MAXCONN; i++)
        if (vcid_array[i])
            ina_close(vcid_array[i]);
    printf("\33[21;1H FETCH_4 terminated correctly !\n");

    for (i=0; i<80; i++)
        putchar('ฤ');
    printf("\n");


}




/*-------------------------------------------------------------------------*/

void h1visu( int vcid_a [] )

{                                /* ===> FETCH/RW in SIMATIC */
    static struct { S5_HD_REQ; } s5_read_req = { S5_RD_INIT };
    static struct { S5_HD_ACK;
                    union { char     db[512];
                            unsigned dw[256];
                          } d;
                  } s5_read_ack;
    int xx, i, cn[MAXCONN];


    FILE       * FilePtr;
    char       FileName [] = "TEST.TXT";




    s5_read_req.dbnr = 10;
    s5_read_req.a_anf = xhl(0);
    s5_read_req.a_len = xhl(12);

    for (i=0; i<MAXCONN; i++) cn[i] = 1;

    for (xx=0; xx<139; xx++)
    {
    for (i=0; i<MAXCONN; i++)
        if (vcid_a[i])
            ina_send(vcid_a[i], (char *)&s5_read_req, sizeof(s5_read_req));
    for (i=0; i<MAXCONN; i++)
        if (vcid_a[i])
        {
            ina_recv(vcid_a[i], (char *)&s5_read_ack, sizeof(s5_read_ack));
            if (!s5_read_ack.ack_error)
                printf("\33[%d;26H%5d\33[%d;40H%s", 10+2*i, cn[i]++, 10+2*i, s5_read_ack.d.db);
            else
                printf("\33[%d;26HREAD-Error : %d\n", 10+2*i, s5_read_ack.ack_error);
        }


    /*----- open TEST.TXT for writing -----*/

    if ( (FilePtr = fopen (FileName, "a")) == NULL )
    {
          printf (" %s kann nicht geoeffnet werden", FileName );
          exit (1);
    }

    /*----- testoutput to TEST.TXT    -----*/

    if ( fputs ("Test-String for FETCH_4 Disk I/O ", FilePtr) != EOF )
        printf (" String written \n");
    else
         printf (" error writing A to TEST.TXT\n");

    fclose ( FilePtr );

    }
}

