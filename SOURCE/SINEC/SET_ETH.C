/*--------------------------------------------------------------------------*/
/*                       SET ETHERNET ADDRESSE                              */
/*                      =======================                             */
/*                                                                          */
/*------------------------------------------------ Scheubeck E851 K'he -----*/
/*                                                                          */
/*	Programm:	SET_ETH          	 	 	            */
/*	Purpose:        reads a ethernet address from the command line      */
/*                      and calls the DATA LINK on CP3241 to set the        */
/*   			new MAC-address.				    */
/*	Input:		a valid ethernet address, thats means 12 hex        */
/*			numbers, at the command line.			    */
/*	compiler notes: This code is written for TURBO C V2.0.		    */
/*	project info's: This is a stand alone module. There a no other      */
/*			modules to be linked with.			    */
/*	further information:                                                */
/*			Refer to INA 960 Release 3.0 Programmer's 	    */
/*			Reference Manual chapter 2.3 (req_block),           */
/*			chapter 6.4.6 (ia_setup),                           */
/*			PC Link2 Software Developer's Manual chapter 10.3   */
/*			(interface specification, using INT 5BH).	    */
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
|                          | copyright remark			   |          |
+--------------------------+---------------------------------------+----------+
|                          |                                       |          |
+--------------------------+---------------------------------------+----------+
*/

#include <stdio.h>
#include <dos.h>

unsigned int soft_flag;
struct req_block
       {
	unsigned char    res[4];
	unsigned char    len;
	unsigned int     uid;
	unsigned char    r_port;
        void  (far *anr_addr)();
	unsigned char    subsys;
	unsigned char    opcode;
	unsigned int     error;
	unsigned int     res1;
	unsigned int     adlen;
	unsigned char    addr[6];

       } ;
static struct req_block rb_ia_setup;

/*--------------------------------------------------------------------------*/
/*  main routine reads new etherent address from command line               */
/*--------------------------------------------------------------------------*/

main(ac,av)
    int ac;
    char *av[];
{
    static unsigned char addr[] = { 0x08,0x00,0x06,0x01,0x10,0xff };
    unsigned ccc[6], i;

     printf ("ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออป\n");
     printf ("บ    COPYRIGTH (C) SIEMENS AG 1989 ALL RIGTHS RESERVED    บ\n");
     printf ("ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ\n");

    if (2 == ac &&
	6 == sscanf(av[1], "%2x%2x%2x%2x%2x%2x",
		    &ccc[0], &ccc[1], &ccc[2], &ccc[3], &ccc[4], &ccc[5]))
    {
	for (i=0; i<6; i++) addr[i] = ccc[i];
        ina_set_eth(addr);   /**/

	printf("CP3241 new Ethernet address :   %02X %02X %02X %02X %02X %02X\n",
		addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
    }
    else
        printf("Usage: SET_ETH 080006011xxx\n");
}
/*PAGE*/
/*--------------------------------------------------------------------------*/
/*  INA_SET_ETH                                                             */
/*--------------------------------------------------------------------------*/

ina_set_eth(eth_addr)
    char eth_addr[];
{

    rb_ia_setup.len = sizeof(rb_ia_setup);
    rb_ia_setup.subsys = 0x20;         /* = DLL */
    rb_ia_setup.opcode = 0x89;         /* Opcode for IA_SETUP */
    rb_ia_setup.adlen = 6;
    memcpy(rb_ia_setup.addr, eth_addr, 6);

    call_ina(&rb_ia_setup);

    if (rb_ia_setup.error != 1)
        ina_error(&rb_ia_setup);
}

/*PAGE*/
/*--------------------------------------------------------------------------*/
/*  far routine wich is called by ina 960, sets soft_flag to 1              */
/*--------------------------------------------------------------------------*/


void far soft_int()             /* far procedure soft_int. This routine is */
				/* called by the netbios driver.           */

{
    _SI = _DS;       /* a little programming trick. The DS register is not */
    _DS = _ES;       /* set to the right value at this moment. The right   */
		     /* value was given to the INA software in ES. This    */
		     /* value is still alife and we will use it. To save   */
		     /* the current ds, si is used (TURBO C does the same) */
    soft_flag = 1;
    _DS = _SI;

} /* This bracket means a far return to the calling process. */
/*--------------------------------------------------------------------------*/
/*  call INA issue INT 5bH copy rb to INA                                   */
/*--------------------------------------------------------------------------*/

call_ina(rb)
    struct req_block *rb;
{
    static union REGS regs ;
    static struct SREGS sregs ;

    soft_flag = 0;

    rb->res[0] = 0xff;  	/* The first byte of the reserved field has */
				/* to be a ffH. INA 960 specification!      */
    rb->res[1] = 0x0;
    rb->r_port = 0xff;		/* response port, must be filled with ffH   */
    rb->anr_addr = soft_int;    /* address of soft_int routine (offset,     */
				/* segment)				    */

    /* Now issue a interrupt 5bH, this is the communication to th INA driver*/
    /* registers es, bx must be preset with the request block adress (dseg  */
    /* and offset). ah has to be 0, al for this call 1			    */

    sregs.es = _DS;
    regs.h.ah = 1;
    regs.h.al = 0;
    regs.x.bx = (unsigned)rb;
    int86x(0x5b, &regs, &regs, &sregs);
    if (regs.h.al!=0)      /* check for immediate error */
	return(255);

    while(!soft_flag)      /* wait for INA response, soft_flag is set in   */
	;                  /* procedure soft_int                           */

    return(rb->error);
}
/*--------------------------------------------------------------------------*/
/* INA_ERROR                                                                */
/*--------------------------------------------------------------------------*/

ina_error(rb)
    struct req_block *rb;
{
    printf("iNA960-Error : %2d (dec) by Subsys=0x%02X  Opcode=0x%02X.\n",
            rb->error, rb->subsys, rb->opcode);
    exit(2);
}