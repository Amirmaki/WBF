/*--------------------------------------------------------------------------*
 *  Request-Block-Header                                                    *
 *                                                                          *
 *------------------------------------------------------ TCHALIKIAN DTC4 ---*/
/*------------------------------------------------------ Scheubeck E851 K'he*/

/* CHANGES MADE :
+--------------------------+---------------------------------------+----------+
|   who                    | why                                   | date     |
+--------------------------+---------------------------------------+----------+
| Scheubeck E851 K'he      | Number of ta_type.subnet changed to 2 | 08.02.89 |
|                          | Necessary for INA 960 release 3.0     |          |
|                          | (release 2 requires ta_type.subnet[4])|          |
!--------------------------+---------------------------------------+----------+
|                          |                                       |          |
+--------------------------+---------------------------------------+----------+
|                          |                                       |          |
+--------------------------+---------------------------------------+----------+
*/


#define MAXBUF  5
#define DATALEN 512
#define DONE    1
#define NOT_DONE 0

#define SUCCESS 1
#define CLOSE_SUCCESS   7
#define RECV_SUCCESS    3
#define REMOTE_ABORT    14
#define TIMEOUT         16
#define CONN_REJECT     22
#define RB_FIXED                                 \
        unsigned char    res[4];                 \
        unsigned char    len;                    \
        unsigned char    uid1;                   \
        unsigned char    uid2;                   \
        unsigned char    r_port;                 \
        void              (far *anr_addr)();         \
        unsigned char    subsys;                 \
        unsigned char    opcode;                 \
        unsigned int     error;

struct RbFixed        { RB_FIXED };

struct buf_type {
        unsigned long   addr;
        unsigned int    len;
};

struct open_type {        /* OPEN */
        RB_FIXED
        unsigned int    vcid;
} ;


struct stdvc_type {        /* SEND, RECV, CLOSE, .... */
        RB_FIXED
        unsigned char    rcode;              /* Reason code */
        unsigned char    res1[15];
        unsigned int     vcid;
        unsigned char    qos;                /* Conn.Class */
        unsigned char    blen1;
        unsigned char    blen2;
        unsigned char    nbufs;
        struct buf_type buf;        /*block[MAXBUF]; */
};

struct  call_type {        /* CONNECT ...*/
        RB_FIXED
        unsigned char    rcode;              /* Reason_code */
        unsigned char    res1[4];
        unsigned int     ackdelay;           /* ack_delay_estimate */
        unsigned long    taddr;              /* ---> TA-Buffer */
        unsigned int     tries;              /* Persistenc_count */
        unsigned int     ato;                /* Abort timeout */
        unsigned int     vcid;
        unsigned char    qos;                /* Conn.Class */
        unsigned int     neg;                /* Neg. Options */
        unsigned long    ubaddr;             /* ---> User_Buffer */
        unsigned char    ublen;              /*      len */
};

struct ta_type{
        unsigned char    loc_nsap_id_len;

        unsigned char    loc_tsap_id_len;
        unsigned char    loc_tsap_id[8];          /* local TSAP IP                */

        unsigned char    rem_net_addr_len;
        unsigned char         afi;                /* New! for INA 960 release 3.0 */
        unsigned char    subnet[2];               /* New! only 2                  */
        unsigned char    host_id[6];              /* remote ethernet address      */
        unsigned char         lsap_id;                 /* New!                         */
        unsigned char    rem_net_nsap;

        unsigned char    rem_tsap_id_len;
        unsigned char    rem_tsap_id[8];          /* remote TSAP ID               */
        unsigned char    dummy [0x60];            /* Test to look if someone
                                                     kills my data */
};


