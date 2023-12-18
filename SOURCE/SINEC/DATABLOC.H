// Fichier DATABLOC.H	Le 22 Janvier 1996		D. MASSON
// Définitions du Block de données reçu de l'automate


// Application YAZD
// Nombre de zones supérieures et inférieures
#define NBZONES_H		4
// Mettre en commentaire si aucune zone inférieure
//#define NBZONES_B
// Nb de bruleurs ds la zone préchauffage
#define NB_BURNERS		7
//#define NB_BURNERSI	7
//Numero de la donnée dans le block
//Ces informations servent à accéder à chacune des données
#define DATA_ACK			0
#define DATA_READY		DATA_ACK + 1
#define CHARGE_PROD     DATA_READY + 1
#define FIN_MVT_AV      CHARGE_PROD + 1
#define FIN_MVT_AR      FIN_MVT_AV + 1
#define DEF_PAR_DEF     FIN_MVT_AR + 1
#define ENF_PAR_DEF     DEF_PAR_DEF + 1
#define LONG_PAS        ENF_PAR_DEF + 1
#define TEMP_RECUP_H    LONG_PAS+1
#define TEMP_MESUREE_H	TEMP_RECUP_H + 1
#define TEMP_CONSIGNE_H TEMP_MESUREE_H + NBZONES_H
#define CONSI_ACTU_H    TEMP_CONSIGNE_H + NBZONES_H
#define DEBIT_GAZ_H     CONSI_ACTU_H + NBZONES_H
#define DEBIT_AIR_H     DEBIT_GAZ_H + NBZONES_H
#ifdef NB_BURNERS
#define ETAT_BRULEUR_H  DEBIT_AIR_H + NBZONES_H
#endif

#define JOB_ID				ETAT_BRULEUR_H + 1
#define PO_NUMBER       JOB_ID       + 1
#define STEEL_GRADE 		PO_NUMBER    + 1
#define STEEL_CODE		STEEL_GRADE  + 5
#define BLOOM_WIDTH		STEEL_CODE   + 1
#define BLOOM_THICK		BLOOM_WIDTH  + 1
#define BLOOM_LENGTH		BLOOM_THICK  + 1
#define BLOOM_NUMBER    BLOOM_LENGTH + 1
#define DISCH_TEMP		BLOOM_NUMBER + 1
#define BATCH_THEO	   DISCH_TEMP+1


#ifdef NBZONES_B
#define TEMP_MESUREE_B	ETAT_BRULEUR_H + NB_BURNERS
#define TEMP_CONSIGNE_B TEMP_MESUREE_B + NBZONES_B
#define CONSI_ACTU_B    TEMP_CONSIGNE_B + NBZONES_B
#define DEBIT_GAZ_B     CONSI_ACTU_B + NBZONES_B
#define DEBIT_AIR_B     DEBIT_GAZ_B + NBZONES_B
#define ETAT_BRULEUR_B  DEBIT_AIR_B + NBZONES_B
#endif

//ParamŠtres de communication
#define DBPLC 10 //No du data bloc PLC
#define DBREG 10 //No du data bloc REG
#define NBPLC 7  //Nb de donn‚es du PLC (‚v‚nements PLC)
#define NBREG 37 //Nb de donn‚es ‚chang‚es avec les r‚gulateurs

