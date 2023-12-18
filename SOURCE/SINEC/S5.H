/*--------------------------------------------------------------------------*/
/*  Header-File S5.H                                                        */
/* ==================                                                       */
/*                                                                          */
/*------------------------------------------------ Tchalikian DTC4 13.04.89 */
/*------------------------------------------------ Scheubeck E851 K'he      */

/* CHANGES MADE :
+--------------------------+---------------------------------------+----------+
|   who                    | why                                   | date     |
+--------------------------+---------------------------------------+----------+
| Scheubeck E851 K'he      | translate comments                    | 08.02.89 |
+--------------------------+---------------------------------------+----------+
|                          |                                       |          |
+--------------------------+---------------------------------------+----------+
|                          |                                       |          |
+--------------------------+---------------------------------------+----------+
*/


#define xhl(hl)  ((hl>>8) | ((hl&255)<<8))

/*--------------------------------------------*/
/*     REQUEST TELEGRAMM                      */
/*--------------------------------------------*/

#define S5_HD_REQ                                                       \
    char S5 [2];                 /* = 'S5' */                           \
    char longr;                  /* header length      = 16 */          \
    char op_indc;                /* OP-code call singn =  1 */          \
    char op_long;                /* OP-code length     =  3 */          \
    char op_code;                /* OP/code = 3 (WRITE) = 5 (READ)*/    \
    char org_bloc;               /* ORG-block = 3           */          \
    char org_long;               /* ORG-block length    = 8 */          \
    char org_indc;               /* ORG call sign (DB/MB/EB...) */      \
    unsigned char dbnr;          /* DB number (if ORG call sign = DB)*/ \
    unsigned a_anf;              /* start address    !!! hl !!! */      \
    unsigned a_len;              /* length           !!! hl !!! */      \
    char vide_bloc;              /* = FF */                             \
    char vide_long               /* = 02 */

/*--------------------------------------------*/
/*     REQUEST RECEIPT TELEGRAMM              */
/*--------------------------------------------*/

#define S5_HD_ACK                                                     \
    char S5 [2];                 /* = 'S5' */                         \
    char longr;                  /* header length      = 16 */        \
    char op_indc;                /* OP-code call sign  =  1 */        \
    char op_long;                /* OP-code lengtg     =  3 */        \
    char op_code;                /* OP-code = 4 (WRITE) = 6 (READ) */ \
    char ack_bloc;               /* ACK-block          = 15 */        \
    char ack_long;               /* ACK-block length   = 3  */        \
    char ack_error;              /* error number            */        \
    char vide_bloc;              /* = FF */                           \
    char vide_long;              /* = 07 */                           \
    char vide[5]

#define S5_RD_INIT  'S','5', 16,                                \
                     1,3,5,       /* OP-Code = 5 (RD) */        \
                     3,8, 0x01,   /* ORG     = DB     */        \
                     0, 0, 0,     /* DBnr,start,len   */        \
                     255, 2
#define S5_WR_INIT   'S','5', 16,                              \
                     1,3,3,       /* OP-Code = 3 (WR) */        \
                     3,8, 0x01,   /* ORG     = DB     */        \
                     0, 0, 0,     /* DBnr,start,len   */        \
                     255, 2

#ifdef S5_liste
unsigned s5_org_li[] = { '??',
                         'DB', /* 01  */
                         'MB', /* 02  */
                         'EB', /* 03  */
                         'AB', /* 04  */
                         'PB', /* 05  */
                         'ZB', /* 06  */
                         'TB', /* 07  */
                         'BS', /* 08  */
                         'AS', /* 09  */
                         'DX', /* 0A DB (135U)*/
                         '??', '??', '??', '??', '??',
                         'DE', /* 10 (135U)*/
                         'QB'};/* 11 (150U)*/
#endif
