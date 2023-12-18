/*--------------------------------------------------------------------------*/
/*                                                                          */
/*                        S 5 C O M . C                                     */
/*                        =============                                     */
/*                                                                          */
/*------------------------------------------------ TCHALIKIAN DTC4/Paris ---*/
/*------------------------------------------------ Scheubeck E851 K'he -----*/
/*------------------------------------------------ Fromm-Ayass V161 KA -----*/
/*                                                                          */
/*        Programm:       S5COM                                             */
/*        Purpose:        read and write DB in S5                           */
/*                        This program receives a DB from a SIMATIC and then*/
/*                        sends a data-word to a DB.                        */
/*                        The user is asked for the number of the DB,       */
/*                        startnumber and length  of the data field (in the */
/*                        receive job).                                     */
/*                        For writing a data word the required parameters   */
/*                        are the number of DB, which data word and the     */
/*                        contents of the dataword.                         */
/*                        Automatic setting of LED is done in S5AutoSetDb.  */
/*                        The remote TSAP ID are:                           */
/*                                   (Transport Service Access Point)       */
/*                        READREAD   (makes a fetch)                        */
/*                        WRITWRIT   (makes a receive)                      */
/*        Input:          a valid ethernet address, that  means 12 hex      */
/*                        numbers, at the command line, otherwise the       */
/*                        default remote ethernet address 08 00 06 01 0E EE */
/*                        will be used.                                     */
/*        compiler notes: This code is written for MS-C 6.0                 */
/*                        compile in small model with -Zp switch (Byte      */
/*                        alignement);                                      */
/*                        -Gs switch (no stack checking) is made through    */
/*                        program pragma        !!                          */
/*        project info's:  This file should be linked with call_ina.c       */
/*        further information:                                              */
/*                        Refer to INA 960 Release 3.0 Programmer's         */
/*                        Reference Manual chapter 2.3 (req_block),         */
/*                        PC Link2 Software Developer's Manual chapter 10.3 */
/*                        (interface specification, using INT 5BH).         */
/*                        There are also some notes for further info in     */
/*                        CALL_INA.C                                        */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/

/* CHANGES MADE :
+--------------------------+---------------------------------------+----------+
|   who                    | why                                   | date     |
+--------------------------+---------------------------------------+----------+
| Scheubeck E851 K'he      | insert comments, repair some little   | 08.02.89 |
|                          | bugs                                  |          |
+--------------------------+---------------------------------------+----------+
| Fromm-Ayass              | Prototypes; Function S5AutoSetDb      | 19.04.90 |
+--------------------------+---------------------------------------+----------+
| Fromm-Ayass              | Adaption for MS-C 6.0                 | 26.6.90  |
+--------------------------+---------------------------------------+----------+

*/

#include <stdio.h>
#include <dos.h>
#include <stdlib.h>
#include <string.h>
#include "call_ina.h"

#pragma check_stack( off )

#define MaxLen 132

/*------------------------------------------------------------------------*/
/*-----    prototypes of local functions   -----*/

void S5AutoSetDb      (int vcid, char *str);

void S5getdb          (int vcid, char *str);

void S5setdb          (int vcid, char *str);


/*------------------------------------------------------------------------*/

main(ac,av)
    int ac;                     /* to read any command line parameters */
    char *av[];
{
    static unsigned char addr [] = { 0x08,0x00,0x06,0x01,0x0e,0xee };
                                       /* preset remote ethernet address  */

    unsigned i, ccc[6], vcidr, vcidw;
    char buffer[10];

    /* now follows the test if there has been written an ethernet address */
    /* in the command-line. If yes, this address will be used.            */

    if (2 == ac &&
        6 == sscanf(av[1], "%2x%2x%2x%2x%2x%2x",
                    &ccc[0], &ccc[1], &ccc[2], &ccc[3], &ccc[4], &ccc[5]))
        for (i=0; i<6; i++)
            addr[i] = ccc[i];
    printf("\33[2J");
    printf ("         ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออป\n");
    printf ("         บ    COPYRIGHT (C) SIEMENS AG 1989 ALL RIGHTS RESERVED    บ\n");
    printf ("         ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ\n");
    printf ("\t\tSICOMP PC <--> SIMATIC S5   (read, write, SetLED)\n\n\n");

    /*----- INA-Driver loaded -----*/
    if (ina_test() != 0 )
        {
        printf ("INA_960 Interface not loaded !!!!!\n");
        _exit (255);
        }

    printf ("try to connect to         : ");
    for (i = 0; i<6; i++)
        printf(" %02X", addr[i]);
    printf ("\n");

    vcidr = ina_open();
    printf ("  Open to %4.4x OK, TSAP-ID  :  READREAD !\n",vcidr);
    ina_connect(vcidr, addr, "READREAD");
    printf ("  Connected to %4.4X !\n",vcidr);
    vcidw = ina_open();
    printf ("  Open to %4.4x OK, TSAP-ID  :  WRITWRIT !\n",vcidw);
    ina_connect(vcidw, addr, "WRITWRIT");
    printf ("  Connected to %4.4X  !\n",vcidw);

    printf ("succesfully connected to  : ");
    for (i = 0; i<6; i++)
        printf(" %02X", addr[i]);
    printf ("\n");
    printf ("VCIDR = %4.4X   VCIDW = %4.4X \n",vcidr,vcidw);
    printf("\n\n\n\n");

    for(;;)
    {
        printf(">> S5_READ [ DB DW #Words ]   : "); gets(buffer);
        if (strlen(buffer) == 0) break;
        S5getdb(vcidr, buffer);        /* G E T DB !!! */
        printf(">> S5_WRITE [ DB DW HexWord ] : "); gets(buffer);
        if (strlen(buffer) != 0)
            S5setdb(vcidw, buffer);    /* S E T DB !!! */
    }

        printf(">> S5_AutoSetLED [ DB DW ]   : "); gets(buffer);
        if (strlen(buffer) != 0)
            S5AutoSetDb (vcidw, buffer);        /* Set DB !!! */


    ina_close(vcidr);
    ina_close(vcidw);

    printf("\n");
    printf ("         ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออป\n");
    printf ("         บ    COPYRIGHT (C) SIEMENS AG 1989 ALL RIGHTS RESERVED    บ\n");
    printf ("         ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ\n");

}

/*PAGE*/

#include "s5.h"

/*------------------------------------------------------------------------*/

void S5getdb(int vcid, char *str)


{                                        /* ===> FETCH/RW in SIMATIC */
    static struct { S5_HD_REQ;
                  } s5_read_req = { S5_RD_INIT };
    static struct { S5_HD_ACK;
                    unsigned   dw[256];
                  } s5_read_ack;
    unsigned i, dbnr, dbanf, dblen;


    sscanf(str, "%d %d %d", &dbnr, &dbanf, &dblen);
    s5_read_req.dbnr = dbnr;
    s5_read_req.a_anf = xhl(dbanf);
    s5_read_req.a_len = xhl(dblen); /* words */

    ina_send(vcid, (char *)&s5_read_req, sizeof(s5_read_req));
    ina_recv(vcid, (char *)&s5_read_ack, sizeof(s5_read_ack));
    if (!s5_read_ack.ack_error)
    {   printf("              : %s\n", s5_read_ack.dw);
        for(i=0; i<dblen; i++)
            printf("%04X ", xhl(s5_read_ack.dw[i]));
        putchar('\n');
    }
    else
        printf("READ-Error    : %d\n", s5_read_ack.ack_error);
}
/*  End S5GetDb   */


/*------------------------------------------------------------------------*/

void S5setdb(int vcid, char *str)


{                                        /* ===> RECEIVE/RW in SIMATIC */
    static struct  { S5_HD_REQ;
                     unsigned   dw[1];
                   } s5_write_req = { S5_WR_INIT, 0 };
    static struct  { S5_HD_ACK;
                   } s5_write_ack;
    unsigned i, dbnr, dbanf, dblen, dwx;

    sscanf(str, "%d %d %x", &dbnr, &dbanf, &dwx);
    s5_write_req.dbnr = dbnr;
    s5_write_req.a_anf = xhl(dbanf);
    s5_write_req.a_len = xhl(1); /* words */
    s5_write_req.dw[0] = xhl(dwx);

    ina_send(vcid, (char *)&s5_write_req, sizeof(s5_write_req));
    ina_recv(vcid, (char *)&s5_write_ack, sizeof(s5_write_ack));
    if (s5_write_ack.ack_error)
        printf("WRITE-Error   : %d\n", s5_write_ack.ack_error);
}
/*  End S5SetDb   */


/*------------------------------------------------------------------------*/

void S5AutoSetDb (int vcid, char *str)


{                                        /* Automatic Setting of LED */
    static struct  { S5_HD_REQ;
                     unsigned   dw[1];
                   } s5_write_req = { S5_WR_INIT, 0 };
    static struct  { S5_HD_ACK;
                   } s5_write_ack;

    unsigned long i;
    unsigned int j, k, dbnr, dbanf, dblen, dwx;

    FILE        * FilePtr;
    char        FileName [] = "TEST.TXT";



    sscanf(str, "%d %d", &dbnr, &dbanf);


    /*----- output LED's    -----*/

    for (j = 1;  j <= 30;  j++)
    {

       /*----- open TEST.TXT for writing -----*/

       if ( (FilePtr = fopen (FileName, "a")) == NULL )
       {
           printf (" %s kann nicht geoeffnet werden", FileName );
           exit (1);
       }

       for (i = 1; i<=0x80; i=(i*2))
       {
           s5_write_req.dbnr  = dbnr;
           s5_write_req.a_anf = xhl( dbanf );
           s5_write_req.a_len = xhl( 1 );    /* words */
           s5_write_req.dw[0] = xhl( i << 8 );

           ina_send(vcid, (char *)&s5_write_req, sizeof(s5_write_req));
           ina_recv(vcid, (char *)&s5_write_ack, sizeof(s5_write_ack));
           if (s5_write_ack.ack_error)
               printf("WRITE-Error   : %d\n", s5_write_ack.ack_error);


           /*----- testoutput to TEST.TXT    -----*/

           if ( fputs ("A", FilePtr) != EOF )
              printf (" A written \n");
           else
             printf (" error writing A to TEST.TXT\n");

        }



       for (i = 1; i<=0x80; i=(i*2))
       {
           s5_write_req.dbnr  = dbnr;
           s5_write_req.a_anf = xhl( dbanf );
           s5_write_req.a_len = xhl( 1 );    /* words */
           s5_write_req.dw[0] = xhl( i );

           ina_send(vcid, (char *)&s5_write_req, sizeof(s5_write_req));
           ina_recv(vcid, (char *)&s5_write_ack, sizeof(s5_write_ack));
           if (s5_write_ack.ack_error)
               printf("WRITE-Error   : %d\n", s5_write_ack.ack_error);


           /*----- testoutput to TEST.TXT    -----*/

           if ( fputs ("B", FilePtr) != EOF )
              printf (" B written \n");
           else
             printf (" error writing B to TEST.TXT\n");

       }



       for (i = 1; i<=0x80; i=(i*2))
       {
           s5_write_req.dbnr  = dbnr;
           s5_write_req.a_anf = xhl( dbanf+1 );
           s5_write_req.a_len = xhl( 1 );    /* words */
           s5_write_req.dw[0] = xhl( i << 8 );

           ina_send(vcid, (char *)&s5_write_req, sizeof(s5_write_req));
           ina_recv(vcid, (char *)&s5_write_ack, sizeof(s5_write_ack));
           if (s5_write_ack.ack_error)
               printf("WRITE-Error   : %d\n", s5_write_ack.ack_error);



           /*----- testoutput to TEST.TXT    -----*/

           if ( fputs ("C", FilePtr) != EOF )
              printf (" C written \n");
           else
             printf (" error writing C to TEST.TXT\n");

       }

       for (i = 1; i<=0x80; i=(i*2))
       {
           s5_write_req.dbnr  = dbnr;
           s5_write_req.a_anf = xhl( dbanf+1 );
           s5_write_req.a_len = xhl( 1 );     /* words */
           s5_write_req.dw[0] = xhl( i );

           ina_send(vcid, (char *)&s5_write_req, sizeof(s5_write_req));
           ina_recv(vcid, (char *)&s5_write_ack, sizeof(s5_write_ack));
           if (s5_write_ack.ack_error)
               printf("WRITE-Error   : %d\n", s5_write_ack.ack_error);


           /*----- testoutput to TEST.TXT    -----*/

           if ( fputs ("D", FilePtr) != EOF )
              printf (" D written \n");
           else
             printf (" error writing D to TEST.TXT\n");

       }

    fclose ( FilePtr );

    }
}
/*  End S5AutoSetDb   */


