// OWLCVT 04/04/97 15:15:40
//-------------------------------------------------------------------------
// furnwind.H
// Owner source file : furnwind.CPP
//
// Creation : JL COUSIN  date: 31/08/94
//-------------------------------------------------------------------------
void Model_InitWindow();
void Model_SetupWindow();
//void Close();
// Sauvegarde de la structure dans le fichier de configuration *.cfg
void Model_SaveInFile();
// Lecture du fichier de configuration
void Model_LoadFile();
// Variables
#ifdef __OWNER_MODCFG_H
//variables
char *Model_Buf; //buffer contenant une copie de l'ancienne structure
#else
 extern char *Model_Buf;
#endif
// Fonctions
// Sauvegarde les données de la vue de configuration dans la structure
void Model_FillBuffers();
// Transfère de la base de données vers la structure
void Model_LoadData();
// Sauvegarde de la structure dans la base de données
void Model_StoreData();
// Conversions en lecture et en ‚criture pour Hyper Screen
int Conversion_Ecriture();
char *Conversion_Lecture(int a_convertir);
