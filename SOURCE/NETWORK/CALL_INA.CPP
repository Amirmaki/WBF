/*--------------------------------------------------------------------------*/
/*  CALLING THE INA 960 INTERFACE :                                         */
/* ================================                                         */
/* External functions:                                                      */
/*  - vcid = ina_open()              Opening a channel                      */
/*  - ina_connect(vcid, addr, tsap)  Connect to a partner (activ)           */
/*  - ina_listen(vcid, addr, tsap)   Connect to a partner (passive)         */
/*  - ina_send(vcid, buff, len)      Sending a block                        */
/*  - ina_recv(vcid, buff, len)      Receive a block                        */
/*  - ina_close(vcid)                closing a channel                      */
/*                                                                          */
/* internal functions:                                                      */
/*  - call_ina                       iNA 960 call                           */
/*  - fill_ta                             fills  TSAP block                 */
/*  - ina_error                      error analysis                         */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/* Compiler-Info: use MS-C 6.0                                              */
/*                  compile in small modell                                 */
/*                  Compiler-switches: Zp (Byte Alignement)                 */
/*                                     Zi (CodeView Info)                   */
/*                                                                          */
/*                                                                          */
/*------------------------------------------------ Tchalikian DTC4 12.04.89 */
/*------------------------------------------------ Scheubeck E851 K'he      */

/* CHANGES MADE :
+--------------------------+---------------------------------------+----------+
|   who                    | why                                   | date     |
+--------------------------+---------------------------------------+----------+
| Scheubeck E851 K'he      | translate comments                    | 08.02.89 |
+--------------------------+---------------------------------------+----------+
| Scheubeck                | change procedure soft_int, make shure | 08.03.89 |
|                          | that DS has the right value           |          |
+--------------------------+---------------------------------------+----------+
|                          | install new ta_type structure         | 08.10.89 |
+--------------------------+---------------------------------------+----------+
+--------------------------+---------------------------------------+----------+
|                          | Install new error message             | 08.17.89 |
+--------------------------+---------------------------------------+----------+
+--------------------------+---------------------------------------+----------+
| Fromm-Ayass              | Prototypes and explicit TypeConversion| 19.04.90 |
|                          | Adaption for MS-C 6.0                 | 26.06.90 |
|                          |                                       |          |
+--------------------------+---------------------------------------+----------+
latest changes made: 07.09.89 by Arno because of cosmetical repairment
*/

#include "..\network\rb.h"
#include "..\network\call_ina.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>

//#pragma check_stack( off ) /* supress stack checking routines (needed with*/
									/* MS-C 6.0                                          */
/*#define debug*/        /* This is a switch to turn on error messages while
								  programm development.
							  */
//#define assembly         /* Switch to use inline assembler commands.      */


/*------------------------------------------------------------------------*/
/*-----    prototypes of local functions   -----*/

void          adjust    (char * addr, int len);

long          abs_add   (char far * adr);

void far soft_int   ();

unsigned call_ina   (struct req_block _far *rb);

ina_error            (int vcid, struct req_block *rb);

inaerror            (struct RbFixed * rb);

struct ta_type * fill_ta (int pap, char addr[], char tsap[]);



/*------------------------------------------------------------------------*/
void        adjust (char * addr, int len)
{
	int i;
	for ( i = 0; i < len; i++ )
		  * (addr +i) = * (addr + i + 1);
	return;
}
/*------------------------------------------------------------------------*/
long abs_addr (char _far * addr)
{
	 long lds = FP_SEG (addr);
	 return ((lds << 4) + FP_OFF (addr));
}
/*------------------------------------------------------------------------*/
struct req_block { RB_FIXED };

/* union        { struct req_block far * rb;
			struct { unsigned offs, segm; } adr;
		 } param;
*/

static int soft_flag = 0;

/*--------------------------------------------------------------------------*/
/*  far routine wich is called by ina 960, sets soft_flag to 1              */
/*--------------------------------------------------------------------------*/
void far soft_int()             /* far procedure soft_int. This routine is */
										  /* called by the netbios driver.            */
#ifdef assembly       /* for using MS-C Inline Assembler set assembly-switch
								 at top of this module
							 */
{
  _asm
	  {
			  push bx;
			  mov  bx , offset soft_flag;
			  mov  es:word ptr [bx], 01;
			  pop  bx;
	  }
}
#else
{
	 _SI = _DS;             /* DS-register value in INA software is not the same  */
	 _DS = _ES;             /* as in this module. The right DS-value was given to */
	 soft_flag = 1;   /* INA in the ES-register. Thus you have to save DS,  */
	 _DS = _SI;             /* set move ES to DS, set soft_flag and restore DS         */
}
#endif
/*------------------------------------------------------------------------*/
unsigned call_ina    (struct req_block _far *rb)
{
	 static union REGS regs ;
	 static struct SREGS sregs ;

	 soft_flag = 0;
	 rb->res[0] = 255;
	 rb->res[1] =  0;
	 rb->r_port = 0xff;
	 rb->error  = 0xff;
	 rb->anr_addr = soft_int;

#ifdef assembly
{
		 sregs.ds = FP_SEG (rb);
		 sregs.es = FP_SEG (rb);
}
#else
{
	 sregs.ds = _DS;
	 sregs.cs = _CS;
	 sregs.ss = _SS;
	 sregs.es = _DS;
}
#endif
	 regs.h.ah = 1;
	 regs.h.al = 0;
	 regs.x.bx = FP_OFF (rb);
	 int86x (0x5b, &regs, &regs, &sregs);

	 if (regs.h.al != 0)
		  return(255);

	 while(!soft_flag)
		  ;

	 return(rb->error);
}
/*PAGE*/
/*--------------------------------------------------------------------------*/
/*  INA_SEND                                                                */
/*  SEND_EOM_DATA              call to layer 4                              */
/*  subsystem : 40H                                                         */
/*  op code   :  6                                                          */
/*  for more details refer to INA 960 PROGRAMMERS REFERENCE MANUAL          */
/*      chapter 4.4.2.1                                                     */
/*--------------------------------------------------------------------------*/
/* remark : This programm uses only SEND_OEM_DATA because the data length is*/
/*            less then the maximum buffer size                               */

void ina_send            (int vcid, char sdata[], int dtlen)
{
	 static struct stdvc_type rb_send = {0};        /* RB for SEND  */

	 rb_send.len = sizeof(rb_send);
	 rb_send.uid1 = 0;
	 rb_send.uid2 = 0;
	 rb_send.subsys = 0x40;
	 rb_send.opcode = 6;            /* Opcode for Send_Eom */
	 rb_send.vcid = vcid;
	 rb_send.nbufs = 1;             /* Number of buffers = 1 */

	 rb_send.buf.addr = abs_addr( (char _far *)sdata );
	 rb_send.buf.len = dtlen;

	 call_ina ( (struct req_block *) &rb_send );

	 if (rb_send.error != SUCCESS)
		  {
		  inaerror ((struct RbFixed *) &rb_send);
		  ina_error (vcid, (struct req_block *) &rb_send);
		  }
}
/*--------------------------------------------------------------------------*/
/*  INA_RECV                                                                */
/*--------------------------------------------------------------------------*/
void ina_recv       (int vcid, char rdata[], int dtlen)
{
	 static struct stdvc_type rb_recv = {0};        /* RB for RECV  */

	 rb_recv.len = 42;           /* Lenfth of rb_recv */
	 rb_recv.uid1 = 0;
	 rb_recv.uid2 = 0;
	 rb_recv.subsys = 0x40;
	 rb_recv.opcode = 7;         /* Opcode for Receive_Data */

	 rb_recv.vcid = vcid;
	 rb_recv.nbufs = 1;          /* Buffers posted = 1 */

	 rb_recv.buf.addr = abs_addr( (char _far *)rdata );        /* Point Buffer Addr to rdata */
	 rb_recv.buf.len = dtlen;

	 call_ina ((struct req_block *) &rb_recv);                    /* Issue Receive_Data */

	 if (rb_recv.error != RECV_SUCCESS)
		  {
		  inaerror ((struct RbFixed *) &rb_recv);
		  ina_error (vcid, (struct req_block *) &rb_recv);               /* Process Errors */
		  }
}
/*PAGE*/
/*--------------------------------------------------------------------------*/
/*  INA_OPEN                                                                */
/*  OPEN                       call to layer 4                              */
/*  subsystem : 40H                                                         */
/*  op code   :  0                                                          */
/*  for more details refer to INA 960 PROGRAMMERS REFERENCE MANUAL          */
/*      chapter 4.4.1.1                                                     */
/*--------------------------------------------------------------------------*/
int ina_open        ()
{
	 static struct open_type rb_open = {0};         /* RB for OPEN */

	 rb_open.len = sizeof(rb_open);
	 rb_open.uid1 = 0;
	 rb_open.uid2 = 0;
	 rb_open.subsys = 0x40;
	 rb_open.opcode = 0;            /* Opcode for Open Command */

	 call_ina ( (struct req_block _far *) &rb_open );

	 if (rb_open.error != SUCCESS)
		  {
		  inaerror ((struct RbFixed *) &rb_open);
		  ina_error (0, (struct req_block *) &rb_open);
		  }
	 return(rb_open.vcid);
}
/*--------------------------------------------------------------------------*/
/*  INA_CLOSE                                                               */
/*  CLOSE                      call to layer 4                              */
/*  subsystem : 40H                                                         */
/*  op code   :  12                                                         */
/*  for more details refer to INA 960 PROGRAMMERS REFERENCE MANUAL          */
/*      chapter 4.4.3.1                                                     */
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
void ina_close      (int vcid)
{
	 static struct stdvc_type rb_close = {0};  /* RB for CLOSE */
	 int i;

	 rb_close.len = sizeof(rb_close);
	 rb_close.uid1 = 0;
	 rb_close.uid2 = 0;
	 rb_close.subsys = 0x40;
	 rb_close.opcode = 12;           /* Opcode for Close */

	 rb_close.rcode = 0;                   /* Reason Code !!! */
	 rb_close.vcid = vcid;

	 call_ina ((struct req_block *) &rb_close);

	 if (rb_close.error != CLOSE_SUCCESS &&
		  rb_close.error != REMOTE_ABORT )
			{
			inaerror ((struct RbFixed *) &rb_close);
			ina_error (0, (struct req_block *) &rb_close);    /* Process Errors */
			}
}
/*PAGE*/
/*--------------------------------------------------------------------------*/
/* FILL_TA fills the Transport Address Buffer Fields                        */
/*--------------------------------------------------------------------------*/
struct ta_type * fill_ta (int pap, char addr[], char tsap[])
{
	 static struct ta_type ta = {0};
	 int i;

	 ta.loc_tsap_id_len = 8;
	 memcpy(ta.loc_tsap_id, "PCPCPCPC", 8);

/*    for (i = 0 ; i < 0x60 ; i++)
		  ta.dummy[i] = 0x40;
*/
	 ta.rem_net_addr_len = 0xb;                /* Changed 08.10.89  */
	 ta.subnet[0] = 0;
	 ta.subnet[1] = 0;
	 if (pap)
		  {
		  memcpy(ta.host_id, addr, 6);
		  ta.afi = 0x49;                                    /* for  CONNECT REQUEST */
		  ta.lsap_id = 0xfe;
		  }
	 else
		  {
		  memcpy(ta.host_id, "\0\0\0\0\0\0", 6); /* 0 for any node to connect */
		  ta.afi = 0;
		  ta.lsap_id = 0;                            /* for AWAIT CONNECT REQUEST */
		  }

	 ta.rem_net_nsap = 0;                    /* changed 08.10.89 */

	 ta.rem_tsap_id_len = 8;
	 memcpy(ta.rem_tsap_id, tsap, 8);

#ifdef debug
	 printf ("\n");
	 dump1_rb (&ta);
	 printf ("\n");
#endif

	 return( &ta );
}
/*--------------------------------------------------------------------------*/
/*  INA_CONNECT                                                             */
/*  SEND_CONNECT_REQUEST       call to layer 4                              */
/*  subsystem : 40H                                                         */
/*  op code   :  1                                                          */
/*  for more details refer to INA 960 PROGRAMMERS REFERENCE MANUAL          */
/*      chapter 4.4.1.2                                                     */
/*--------------------------------------------------------------------------*/
ina_connect    (int vcid, unsigned char addr[6], unsigned char tsap[8])
{
	 int i;
	 static struct call_type rb_callcr;         /* RB for AWAIT_CONN_REQ */
	 struct ta_type * ta = fill_ta (1, addr, tsap);
#ifdef debug
	 printf ("\n");
	 printf ("VCID = %4.4X TSAP = ",vcid);
	 for ( i = 0 ; i < 8 ; i++)
		  printf ("%c",tsap[i]);
	 printf ("  ADDR = ");
	 for ( i = 0 ; i < 6 ; i++)
		  printf ("%2.2X ",addr[i]);
	 printf ("\n");
#endif
	 rb_callcr.len = sizeof(rb_callcr);
	 rb_callcr.uid1 = 0;
	 rb_callcr.uid2 = 0;
	 rb_callcr.subsys = 0x40;
	 rb_callcr.opcode = 1;           /* Opcode for Send_Conn_Req */

	 rb_callcr.taddr = abs_addr( (char _far *) ta ); /* Point taddr to Transport Address Buffer */
	 rb_callcr.tries = 3;            /* Retries = 3 */
	 rb_callcr.ato = 200;             /* Abort Time out = 20 * 51 ms */
	 rb_callcr.vcid = vcid;
	 rb_callcr.qos = 0;
	 rb_callcr.neg = 0;
	 rb_callcr.ubaddr = 0;           /* No data during Connection reqs */
	 rb_callcr.ublen = 0;

	 /*    adjust ( &(rb_callcr.rcode), 24);  */

	 call_ina ((struct req_block _far *) &rb_callcr);
#ifdef debug
	 printf ("\n");
	 printf ("VCID = %4.4X TSAP = ",vcid);
	 for ( i = 0 ; i < 8 ; i++)
		  printf ("%c",tsap[i]);
	 printf ("  ADDR = ");
	 for ( i = 0 ; i < 6 ; i++)
		  printf ("%2.2X ",addr[i]);
	 printf ("\n");
#endif

	 if (rb_callcr.error != SUCCESS)
		  {
		  inaerror ((struct RbFixed *) &rb_callcr);
		  ina_error (vcid, (struct req_block *) &rb_callcr);
		  }
	 return(rb_callcr.error);
}
/*PAGE*/
/*--------------------------------------------------------------------------*/
/*  INA_LISTEN                                                              */
/*  AWAIT_CONNECT_REQUEST_TRAN command                                      */
/*  subsystem : 40H                                                         */
/*  op code   :  2                                                          */
/*  for more details refer to INA 960 PROGRAMMERS REFERENCE MANUAL          */
/*      chapter 4.4.1.3                                                            */
/*--------------------------------------------------------------------------*/
void ina_listen     (int vcid, char addr[6], char tsap[8])
{
	 static struct call_type rb_listen;         /* RB for AWAIT_CONN_REQ */
	 struct  ta_type *ta = fill_ta(0, addr, tsap);

	 rb_listen.len = sizeof(rb_listen);
	 rb_listen.uid1 = 0;
	 rb_listen.uid2 = 0;
	 rb_listen.subsys = 0x40;
	 rb_listen.opcode = 2;           /* Opcode for Await_Conn_Req_Tran */

	 rb_listen.taddr = abs_addr( (char far *) ta );        /* Point taddr to Transport Address Buffer */
	 rb_listen.tries = 1;            /* Retries = 1 */
	 rb_listen.ato   = 5;            /* Abort Time out = 5 * 51 ms */
	 rb_listen.vcid  = vcid;
	 rb_listen.qos   = 0;
	 rb_listen.neg   = 0;
	 rb_listen.ubaddr = 0;           /* No data during Connection reqs */
	 rb_listen.ublen = 0;

	 call_ina ((struct req_block *) &rb_listen);

	 if (rb_listen.error != SUCCESS)
		  {
		  inaerror ( (struct RbFixed *) &rb_listen);
		  ina_error (vcid, (struct req_block *) &rb_listen);
		  }

	 memcpy(addr, ta->host_id, 6);
}
/*--------------------------------------------------------------------------*/
/* INA_ERROR                                                                */
/* This routine is used when an INA error occures.                            */
/* Some parts were activated only in debug mode.                            */
/*--------------------------------------------------------------------------*/
static char *ina_Opcode[] = { "Open_req",
										"Send_Conn_Req",
										"Await_Conn_Req_Tran",
										"Await_Conn_Req_User",
										"Accept_Conn_Req",
										"Send_Data",
										"Send_Eom_Data",
										"Receive_Data",
										"Withdraw_Rcv_Buf",
										"Send_Exp_Data",
										"Receive_Exp_Data",
										"Withdraw_Exp_Data",
										"Close_Req",
										"Await_Close",
										"Status_Req",
										"Def_Satus",
										"Reserved",
										"Send_Datagram",
										"Receive_Datagram",
										"Withdraw_dg_Buf" };

ina_error (int vcid, struct req_block *rb)
{
	 printf("\niNA960-Error : %2d by %s\n", rb->error, ina_Opcode[rb->opcode]);
	 if (vcid)
		  ina_close(vcid);
	 exit(2);
}
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* INA_ERROR checks for errors in the Request Block passed and exits after  */
/* displaying the fields.                                                   */
/*                                                                          */
/*--------------------------------------------------------------------------*/
inaerror (struct RbFixed *rb)
{
	if (rb->error != SUCCESS) {
#ifdef debug
		printf("\n --- Error ----\n");
		dump_rb(rb);
		dump1_rb(rb);
#endif
		}
}

#ifdef debug
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* DUMP_RB displays the common fields of the Request Block                  */
/*                                                                          */
/*--------------------------------------------------------------------------*/
dump_rb(rb)
struct RbFixed *rb;
{
		  printf("\n-----------------------------------\n");
		  printf("Res           : %8.8lx\n",rb->res);
		  printf("Length        : %2.2x\n",rb->len);
		  printf("Uid1          : %4.4x\n",rb->uid1);
		  printf("Uid2          : %4.4x\n",rb->uid2);
		  printf("R_Port        : %2.2x\n",rb->r_port);
		  printf("Anr_addr      : %8.8lx\n",rb->anr_addr);
		  printf("Subsys        : %2.2x\n",rb->subsys);
		  printf("Opcode        : %2.2x\n",rb->opcode);
		  printf("Error         : %2.2x\n",rb->error);
		  printf("-----------------------------------\n");
}
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* DUMP1_RB displays the fields without interpretation                      */
/*                                                                          */
/*--------------------------------------------------------------------------*/
dump1_rb(rb)
unsigned char    *rb;
{
		  int     i;
		  for (i=0; i<40; i++)
					 printf(" %2.2x ",rb[i]);
}
#endif
/*--------------------------------------------------------------------------*/
/*  INA_TEST                                                                */
/*        Test if there is the INA_960 Driver is installed.                   */
/*      Return value:                                                       */
/*                0   on success                                              */
/*                255 on failure                                              */
/*--------------------------------------------------------------------------*/
int ina_test    ()
{
	 static union REGS regs ;
	 static struct SREGS sregs ;

	 regs.x.ax = 0x0b951;
	 regs.x.bx = 0;
	 int86x(0x2f, &regs, &regs, &sregs);
	 if (regs.x.bx==0)      /* check for immediate error */
		  return(255);
	 return (0);

}