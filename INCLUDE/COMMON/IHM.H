//#define __OWNER_IHM_H

//#define NbCodSteel	8	// Nb of steel code
/*
#ifdef __OWNER_IHM
float CalcPer;
int RunSpt = 0; //to link
int NbBatch;						// Nb of defined batches
ProdTypeList *TheTypeList = NULL; 	   // Pointer to product list object
Furnace *TheFurn; 				// Pointer to furnace object
EndzTargetList *TheTargetList = NULL; 	  // Pointer to objective curve list
Batch *BatchTab[50];				// Array of batch pointer (max = 50)
Metal *MetalTab[22];				// Array of metal pointer (max 22 metal code)
//Product *Pdis = NULL;			// Pointer to the last discharged product

#else

extern int NbBatch;		// Nb of batches in the furnace
extern ProdTypeList *TheTypeList = NULL; 	   // Pointer to product list object
extern Furnace *TheFurn; 				// Pointer to furnace object
extern EndzTargetList *TheTargetList = NULL; 	  // Pointer to objective curve list
extern Batch *BatchTab[50]; 			// Array of batch pointer (max = 50)
extern Metal *MetalTab[22]; 	       // Array of metal pointer (max 22 metal code)
extern Product *Pdis = NULL; 		   // Pointer to the last discharged product

#endif
 */

int ihm();

// Create objects from database
void CreateObjects();

void UpdateObjects();

// Delete objects
void DeleteObjects();

// Update views 7, 9 and 11
// (Reheating, Zone historics and Product historics displays)
void UpdateView();
