#ifndef __BATCH_H
#define __BATCH_H

// Contents ----------------------------------------------------------------
//
// batch class
//
// Description
//
// This class defines data and functions for product data which are
// shared by many product object in steel industry furnace.
//
// Creation : C.Martinet 12/02/92
//            30/03/92 add data for cylindrical products
//            13/05/92 add PsiI and Coeff0I
//            15/05/92 add calculation data
//	      25/06/92 add TypeNumber
//            06/07/92 add copy contructor
//	      16/07/92 TypeNumber in constructor arguments
//            26/01/93 add production rate data
//	      28/01/93 add tau2coeff
//
// F Destrebecq
//            22/10/93 add NrLotto and Marca for FALCK
// End ---------------------------------------------------------------------


// Interface Dependencies ---------------------------------------------------

#ifndef __IOSTREAM_H
#include <iostream.h>
#endif

#ifndef __UCLSTYPE_H
#include "uclstype.h"
#endif

#ifndef __METAL_
#include "metal.h"
#endif

#ifndef __FURNACE_H
#include "furnace.h"
#endif

// End Interface Dependencies ------------------------------------------------

// Class //

class Batch : public Object
{
public:
//member functions
       //constructor
       Batch (int identity,float L, float w, float t, int dblf, float d, 
              Furnace *Fptr, int mcod, Metal *Mptr ,char [], char[] ,
	      float tc=20,int scod=100,int ntyp=0,int pph=0);

       //copy constructor
       Batch(const Batch&);

       //destructor
       virtual ~Batch();

//member functions

//member inherited from Object class :
       virtual classType       isA() const;
       virtual char           *nameOf() const;
       virtual hashValueType   hashValue() const;
       virtual int             isEqual( const Object& ) const;
       virtual void            printOn( ostream& ) const;

//member data
public:
   //calculation data (added 15/05/92)
   static float SlabWidthLimit; //width limit to check product type (m)
   static float topscoeff1; //topscoeff for product width <= SlabWidthLimit
   static float topscoeff2; //topscoeff for product width > SlabWidthLimit
   static float topscoeff3; //topscoeff for circular section product
   static float tau2coeff;  //tau2 dividor

   float topscoeff; //coeff to use for this product batch
   float KPF1; //coeff 1 for PerteFeu calculation
   float KPF2; //coeff 2 for PerteFeu calculation
   float KDC1; //coeff 1 for Decarb calculation
   float KDC2; //coeff 2 for Decarb calculation
   //end added

   float Length; //product length (m)
   float Width;  //product width or diameter (m)
   float Thick;  //product thickness (m)
   int   Poids;  //poids theorique (Kg) 
   int DblFile;  //1 if product is actually 2 half products, else 0
   int SteelCod; //BISRA steel code (1->22)
   int SurfCod;  //Surface code (%)
   float Epsilon; //0.8*(CodSurf/100) (0<Epsilon<=0.8) (none)
   float TempCha; //charging temp (øK)
   float DefaultAbsGap; //default value for Abscissa gap (m)
   float Psi;    //(none)
   float Coeff0; //Psi*(1-Epsilon)
   float PsiI;    //same as Psi with bottom furnace height (none)
   float Coeff0I; //Psi*(1-Epsilon)
   float tau1;   //coeff1 for thermal inertia calc. = piý/thický (m-ý)
   float Volume;  //product volume (m3)
   Furnace *TheFurn; //pointer on the furnace object
   Metal *TheSteel;  //pointer on the metal object
   int TypeNumber; //may be used to identified a product type
   int ProductionRate; //theoritical production rate (prod/h)
   int Id; //Identit‚ du lot
   //Data used for falck application
   char Marca[14];   // 13 character + \0
   char NrLotto[11]; // 9 character + \0
};

// Description -------------------------------------------------------------
//
// Constructor :
// 	Batch (float L, float w, float t, int dblf, float d, Furnace *Fptr,
//		 int mcod, Metal *MPtr, float tc=20,
//		 int scod=100,int ntyp = 0,int pph = 0)
//
//   L -> product length (m)
//   w -> product width or diameter (m)
//   t -> product thickness (m)
//   dblf -> double file indicator (0/1)
//   d -> default abscissa gap (m)
//   *Fprt -> pointer on the furnace object
//   mcod -> BISRA steel code
//   *Mptr -> pointer on the metal object
//   tc -> charging temp (øC), default value 20øC
//   scod -> surface code (0<->100), default value 100
//   ntyp -> product type number (1<->99), default type 0
//   pph -> production rate (pdts/h), default 0
//
//
// End ---------------------------------------------------------------------


#endif // ifndef __BATCH_H //

