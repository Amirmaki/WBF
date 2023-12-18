#ifndef __UCLSTYPE_H
#define __UCLSTYPE_H

#ifndef __CLSTYPES_H
#include <clstypes.h>
#endif

// Description ----------------------------------------------------------
//
// Defines user class code
//-----------------------------------------------------------------------

#define  textFieldClass (__firstUserClass+1)
#define  screenFieldClass (textFieldClass+1)
#define  batchClass (screenFieldClass+1)
#define  productClass (batchClass+1)
#define  zoneClass (productClass+1)
#define  furnaceClass (zoneClass+1)
#define  metalClass (furnaceClass+1)
#define  polynomeClass (metalClass+1)
#define  prodTypeClass (polynomeClass+1)
#define  prodTypeListClass (prodTypeClass+1)
#define  endzTargetClass (prodTypeListClass+1)
#define  endzTargetListClass (endzTargetClass+1)
//#define  prodHisElementClass (endzTargetListClass+1)
//#define  prodHisListClass (prodHisElementClass+1)
#endif
