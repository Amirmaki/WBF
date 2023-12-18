/*--------------------------------------------------------------------------*/
/*                       READ AND WRITE DB                                  */
/*                      ===================                                 */
/*                                                                          */
/*-------------------------------------------------- TCHALIKIAN DTC4/Paris -*/
/*------------------------------------------------ Scheubeck E851 K'he -----*/
/*                                                                          */
/*	Programm:	H1RWDB           	 	 	            */
/*	Purpose:        This program receives a DB from a SIMATIC and then  */
/*			send a data-word to a DB. The user is asked for     */
/*			the number of the DB, startnumber and length  of    */
/*			the data field (receive job). In the second part    */
/*			the user can write a data word. Parameters are      */
/*			number of DB, which data word and the contents of   */
/*			the dataword.                                       */
/*			The remote TSAP ID are:                             */
/*			READREAD   (makes a fetch)			    */
/*			WRITWRIT   (makes a receive)			    */
/*	Input:		a valid ethernet address, thats means 12 hex        */
/*			numbers, at the command line, otherwise the         */
/*			default remote ethernet address 08 00 06 01 0E EE   */
/*			will be used.                                       */
/*	compiler notes: This code is written for TURBO C V2.0.		    */
/*	project info's: The INCLUDE FILE S5.H will be called by FETCH_4.    */
/*			This modules also needs CALL_INA.C to be linked     */
/*			with. CALL_INA needs RB.H as include file           */
/*	                Look to the project file H1RWDB.PRJ and change      */
/*			this file to your configuration.                    */
/*	further information:                                                */
/*			Refer to INA 960 Release 3.0 Programmer's 	    */
/*			Reference Manual chapter 2.3 (req_block),           */
/*			PC Link2 Software Developer's Manual chapter 10.3   */
/*			(interface specification, using INT 5BH).	    */
/*			There are also some notes for further info in 	    */
/*			CALL_INA.C                                          */
/*--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/

/* CHANGES MADE :
+--------------------------+---------------------------------------+----------+
|   who                    | why                                   | date     |
+--------------------------+---------------------------------------+----------+
| Scheubeck E851 K'he      | insert comments, repair some little   | 08.02.89 |
|			   | bugs				   |          |
+--------------------------+---------------------------------------+----------+
|                          |                                       |          |
+--------------------------+---------------------------------------+----------+
|                          |                                       |          |
+--------------------------+---------------------------------------+----------+
Last changes made : 07.09.1989 by Arno  because of cosmetical repairment

*/
//#include "call_ina.h"
#include <stdio.h>
#include <dos.h>
#include <string.h>
//#include "s5.h"
//#include "rb.h"
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
    printf ("           ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออป\n");
    printf ("           บ    COPYRIGTH (C) SIEMENS AG 1989 ALL RIGTHS RESERVED    บ\n");
    printf ("           ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ\n");
    printf("\t\t\tSICOMP PC <-> SIMATIC S5   READ_WRITE_H1\n\n\n");
    printf("\15TRY TO CONNECT TO : ");
    for (i = 0; i<6; i++)
        printf(" %02X", addr[i]);

    vcidr = ina_open();
    printf ("\nOpen to %4.4x OK, TSAP-ID: READREAD !",vcidr);
    ina_connect(vcidr, addr, "READREAD");
    printf ("\nConnect to %4.4X OK !",vcidr);
    vcidw = ina_open();
    printf ("\nOpen to %4.4x OK, TSAP-ID: WRITWRIT !",vcidw);
    ina_connect(vcidw, addr, "WRITWRIT");
    printf ("\nConnect to %4.4X OK !",vcidr);

    printf("\15SUCESSFULLY CONNECTET TO : ");
    for (i = 0; i<6; i++)
        printf(" %02X", addr[i]);
	printf ("\nVCIDR = %4.4X   VCIDW = %4.4X \n",vcidr,vcidw);
    printf("\n\n\n\n");

    for(;;)
    {
        printf("H1_READ  >>>> : "); gets(buffer);
        if (strlen(buffer) == 0) break;
			h1getdb(vcidr, buffer);        /* G E T DB !!! */
        printf("H1_WRITE >>>> : "); gets(buffer);
        if (strlen(buffer) != 0)
            h1setdb(vcidw, buffer);    /* S E T DB !!! */
    }

    ina_close(vcidr);
    ina_close(vcidw);

    printf ("\t   ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออป\n");
    printf ("\t   บ    COPYRIGTH (C) SIEMENS AG 1989 ALL RIGTHS RESERVED    บ\n");
    printf ("\t   ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ\n");

}

/*PAGE*/

#include "s5.h"

h1getdb(vcid, str)
    int vcid;
    char *str;
{					/* ===> FETCH/RW in SIMATIC */
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

    ina_send(vcid, &s5_read_req, sizeof(s5_read_req));
    ina_recv(vcid, &s5_read_ack, sizeof(s5_read_ack));
    if (!s5_read_ack.ack_error)
    {   printf("              : %s\n", s5_read_ack.dw);
        for(i=0; i<dblen; i++)
	    printf("%04X ", xhl(s5_read_ack.dw[i]));
	putchar('\n');
    }
    else
	printf("READ-Error    : %d\n", s5_read_ack.ack_error);
}


h1setdb(vcid, str)
    int vcid;
    char *str;
{					/* ===> RECEIVE/RW in SIMATIC */
    static struct  { S5_HD_REQ;
		     unsigned   dw[1];
		   } s5_write_req = { S5_WR_INIT, 0 };
    static struct  { S5_HD_ACK;
		   } s5_write_ack;
    unsigned i, dbnr, dbanf, dblen, dwx;

    sscanf(str, "%d %d %x", &dbnr, &dbanf, &dwx);
    s5_write_req.dbnr = dbnr;
    s5_write_req.a_anf = xhl(dbanf);
    s5_write_req.a_len = xhl(1); /* mots */
    s5_write_req.dw[0] = xhl(dwx);

    ina_send(vcid, &s5_write_req, sizeof(s5_write_req));
    ina_recv(vcid, &s5_write_ack, sizeof(s5_write_ack));
    if (s5_write_ack.ack_error)
	printf("WRITE-Error   : %d\n", s5_write_ack.ack_error);
}
